#include "./../include/viewer.hpp"
#include "./../include/fborenderer.hpp"

#include <raytracer-sandbox/camera.hpp>
#include <raytracer-sandbox/ray.hpp>
#include <raytracer-sandbox/utils.hpp>
#include <raytracer-sandbox/light.hpp>
#include <raytracer-sandbox/material.hpp>
#include <raytracer-sandbox/object.hpp>
#include <raytracer-sandbox/pathtracing.hpp>

#include <iostream>
#include <memory>
#include <QImage>
#include <qmath.h>
#include <omp.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QColor>

using namespace std;

QColor toColor(const glm::vec3& c)
{
    return QColor(255*clamp(c[0],0,1), 255*clamp(c[1],0,1), 255*clamp(c[2],0,1));
}

Viewer::~Viewer()
{

}

Viewer::Viewer()
{
}

QQuickFramebufferObject::Renderer* Viewer::createRenderer() const
{
    return new FBORenderer();
}

void Viewer::compute()
{
    //Camera steup
    float fov=glm::radians(100.0f), nearPlane=1.5, farPlane=100.0;
    int width=640, height=480;
    Camera camera(fov, width, height, nearPlane, farPlane);
    glm::mat4 cameraView(1.0f), rotation(1.0f), translation(1.0f);
    rotation = glm::rotate(rotation, 0.0f, glm::vec3(1.0,0.0,0.0));
    translation = glm::translate(translation, glm::vec3(0.0,0,-8.0));
    camera.view() = translation*rotation*camera.view();

    glm::vec3 backgroundColor(0.0,0.0,0.0), shadowColor(0.0,0.0,0.0);
    float bias = 0.001;
    int maxDepth = 4;
    //Light setup
    std::vector<LightPtr> lights;

    //Directional light
    DirectionalLightPtr dirLight = std::make_shared<DirectionalLight>();
    dirLight->setDirection( glm::vec3(0.0,-1.0,0.0) );
    dirLight->setAmbient( glm::vec3(0.8,0.8,0.8) );
    dirLight->setDiffuse( glm::vec3(0.8,0.8,0.8) );
    dirLight->setSpecular( glm::vec3(0.8,0.8,0.8) );
    lights.push_back(dirLight);

    //Point lights
    PointLightPtr pointLight = std::make_shared<PointLight>();
    pointLight->setPosition( glm::vec3(0.0,10.0,0.0) );
    pointLight->setAmbient( glm::vec3(0.8,0.8,0.8) );
    pointLight->setDiffuse( glm::vec3(0.8,0.8,0.8) );
    pointLight->setSpecular( glm::vec3(0.8,0.8,0.8) );
    pointLight->setConstant( 1.0 );
    pointLight->setLinear( 0.0 );
    pointLight->setQuadratic( 0.0 );
    //lights.push_back(pointLight);

    //Spot lights
    SpotLightPtr spotLight = std::make_shared<SpotLight>();
    spotLight->setPosition( glm::vec3(0.0,0.0,0.0) );
    spotLight->setSpotDirection( glm::vec3(0.0,0.0,-1.0) );
    float innerCutOff = std::cos( qDegreesToRadians(8.0f) );
    float outerCutOff = std::cos( qDegreesToRadians(16.0f) );
    spotLight->setInnerCutOff(innerCutOff);
    spotLight->setOuterCutOff(outerCutOff);
    spotLight->setAmbient( glm::vec3(0.8,0.8,0.8) );
    spotLight->setDiffuse( glm::vec3(0.8,0.8,0.8) );
    spotLight->setSpecular( glm::vec3(0.8,0.8,0.8) );
    spotLight->setConstant( 1.0 );
    spotLight->setLinear( 0.0 );
    spotLight->setQuadratic( 0.0 );
    //lights.push_back(spotLight);

    //Scene's object setup
    std::vector< ObjectPtr > objects;

    objects.push_back( std::make_shared<Sphere>(glm::vec3(0.0,1.0,0.0), 1.0, PhongMaterial::Emerald()) );

    GlossyMaterialPtr glossyMaterial = std::make_shared<GlossyMaterial>();
    objects.push_back( std::make_shared<Sphere>(glm::vec3(-3,1.0,0.0), 1.0, glossyMaterial) );

    float ior = 1.5; //Glass
    FresnelMaterialPtr fresnelMaterial = std::make_shared<FresnelMaterial>(ior);
    objects.push_back( std::make_shared<Sphere>(glm::vec3(3,1.0,0.0), 1.0, fresnelMaterial) );

    objects.push_back( std::make_shared<Plane>(glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,-1,0.0), PhongMaterial::Pearl()) );

    //Warning : This chunk of code does not work properly
    /*
    string meshFilename ="./../meshes/suzanneLowRes.obj";
    objects.push_back( std::make_shared<TMesh>(meshFilename, PhongMaterial::Emerald()) );
    */
    std::cout << "Debug: compute begin" << std::endl;

    QImage result(width, height, QImage::Format_ARGB32);
    result.fill( Qt::GlobalColor::black);

    std::vector<QVector2D> pixelOffset;
    pixelOffset.push_back(QVector2D(0,0));
    pixelOffset.push_back(QVector2D(0.5,0));
    pixelOffset.push_back(QVector2D(0,0.5));
    pixelOffset.push_back(QVector2D(0.5,0.5));
    int depth = 0;

    auto startTime = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for(int i=0; i<result.width(); ++i)
    {
        for(int j=0; j<result.height(); ++j)
        {
            glm::vec3 pixelColor(0,0,0);
            for(size_t k=0; k<pixelOffset.size(); ++k)
            {
                Ray viewRay = camera.computeRayThroughPixel( i+pixelOffset[k][0], j+pixelOffset[k][1] );
                pixelColor += castRay(viewRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth);
            }
            pixelColor/=(float)(pixelOffset.size());
            result.setPixelColor(i, j, toColor(pixelColor));
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_ms = endTime - startTime;
    std::cout << "Time : " << time_ms.count() << " ms" << std::endl;

    QString filename = "./../images/computed.png";
    result.save(filename, "PNG", 100);
    std::cout << "Debug: compute end" << std::endl;
}
