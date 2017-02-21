#include "./../headers/viewer.hpp"

#include "./../headers/fborenderer.hpp"

#include "./../headers/camera.hpp"
#include "./../headers/ray.hpp"
#include "./../headers/utils.hpp"
#include "./../headers/light.hpp"
#include "./../headers/material.hpp"
#include "./../headers/object.hpp"
#include "./../headers/pathtracing.hpp"

#include <iostream>
#include <memory>
#include <QImage>
#include <qmath.h>
#include <omp.h>
#include <chrono>

using namespace std;

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
    float fov=100, nearPlane=1.5, farPlane=100.0;
    int width=1200, height=800;
    Camera camera(fov, width, height, nearPlane, farPlane);
    QMatrix4x4 cameraView, rotation, translation;
    cameraView.setToIdentity();
    rotation.setToIdentity(); rotation.rotate(0.0f, QVector3D(1.0,0.0,0.0));
    translation.setToIdentity(); translation.translate(QVector3D(0.0,0,-8.0));
    camera.view() = translation*rotation*camera.view();

    QVector3D backgroundColor(0.0,0.0,0.0), shadowColor(0.0,0.0,0.0);
    float bias = 0.001;
    int maxDepth = 4;
    //Light setup
    std::vector<LightPtr> lights;

    //Directional light
    DirectionalLightPtr dirLight = std::make_shared<DirectionalLight>();
    dirLight->setDirection( QVector3D(0.0,-1.0,0.0) );
    dirLight->setAmbient( QVector3D(0.8,0.8,0.8) );
    dirLight->setDiffuse( QVector3D(0.8,0.8,0.8) );
    dirLight->setSpecular( QVector3D(0.8,0.8,0.8) );
    lights.push_back(dirLight);

    //Point lights
    PointLightPtr pointLight = std::make_shared<PointLight>();
    pointLight->setPosition( QVector3D(0.0,10.0,0.0) );
    pointLight->setAmbient( QVector3D(0.8,0.8,0.8) );
    pointLight->setDiffuse( QVector3D(0.8,0.8,0.8) );
    pointLight->setSpecular( QVector3D(0.8,0.8,0.8) );
    pointLight->setConstant( 1.0 );
    pointLight->setLinear( 0.0 );
    pointLight->setQuadratic( 0.0 );
    //lights.push_back(pointLight);

    //Spot lights
    SpotLightPtr spotLight = std::make_shared<SpotLight>();
    spotLight->setPosition( QVector3D(0.0,0.0,0.0) );
    spotLight->setSpotDirection( QVector3D(0.0,0.0,-1.0) );
    float innerCutOff = std::cos( qDegreesToRadians(8.0f) );
    float outerCutOff = std::cos( qDegreesToRadians(16.0f) );
    spotLight->setInnerCutOff(innerCutOff);
    spotLight->setOuterCutOff(outerCutOff);
    spotLight->setAmbient( QVector3D(0.8,0.8,0.8) );
    spotLight->setDiffuse( QVector3D(0.8,0.8,0.8) );
    spotLight->setSpecular( QVector3D(0.8,0.8,0.8) );
    spotLight->setConstant( 1.0 );
    spotLight->setLinear( 0.0 );
    spotLight->setQuadratic( 0.0 );
    //lights.push_back(spotLight);

    //Scene's object setup
    std::vector< ObjectPtr > objects;

    /*
    objects.push_back( std::make_shared<Sphere>(QVector3D(0.0,1.0,0.0), 1.0, PhongMaterial::Emerald()) );

    GlossyMaterialPtr glossyMaterial = std::make_shared<GlossyMaterial>();
    objects.push_back( std::make_shared<Sphere>(QVector3D(-3,1.0,0.0), 1.0, glossyMaterial) );

    float ior = 1.5; //Glass
    FresnelMaterialPtr fresnelMaterial = std::make_shared<FresnelMaterial>(ior);
    objects.push_back( std::make_shared<Sphere>(QVector3D(3,1.0,0.0), 1.0, fresnelMaterial) );

    objects.push_back( std::make_shared<Plane>(QVector3D(0.0,1.0,0.0), QVector3D(0.0,-1,0.0), PhongMaterial::Pearl()) );
    */

    //Warning : This chunk of code does not work properly
    string meshFilename ="./../meshes/suzanneLowRes.obj";
    objects.push_back( std::make_shared<TMesh>(meshFilename, PhongMaterial::Emerald()) );
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

//#pragma omp parallel for
    for(int i=0; i<result.width(); ++i)
    {
        for(int j=0; j<result.height(); ++j)
        {
            QVector3D pixelColor(0,0,0);
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
