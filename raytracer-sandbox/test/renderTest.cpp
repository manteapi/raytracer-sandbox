#include <iostream>
#include <gtest/gtest.h>

#include <raytracer-sandbox/camera.hpp>
#include <raytracer-sandbox/ray.hpp>
#include <raytracer-sandbox/utils.hpp>
#include <raytracer-sandbox/light.hpp>
#include <raytracer-sandbox/material.hpp>
#include <raytracer-sandbox/object.hpp>
#include <raytracer-sandbox/pathtracing.hpp>

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

TEST(Render, Coverage)
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
    lights.push_back(pointLight);

    //Spot lights
    SpotLightPtr spotLight = std::make_shared<SpotLight>();
    spotLight->setPosition( glm::vec3(0.0,0.0,0.0) );
    spotLight->setSpotDirection( glm::vec3(0.0,0.0,-1.0) );
    float innerCutOff = std::cos( glm::radians(8.0f) );
    float outerCutOff = std::cos( glm::radians(16.0f) );
    spotLight->setInnerCutOff(innerCutOff);
    spotLight->setOuterCutOff(outerCutOff);
    spotLight->setAmbient( glm::vec3(0.8,0.8,0.8) );
    spotLight->setDiffuse( glm::vec3(0.8,0.8,0.8) );
    spotLight->setSpecular( glm::vec3(0.8,0.8,0.8) );
    spotLight->setConstant( 1.0 );
    spotLight->setLinear( 0.0 );
    spotLight->setQuadratic( 0.0 );
    lights.push_back(spotLight);

    //Scene's object setup
    std::vector< ObjectPtr > objects;

    objects.push_back( std::make_shared<Sphere>(glm::vec3(0.0,1.0,0.0), 1.0, PhongMaterial::Emerald()) );

    GlossyMaterialPtr glossyMaterial = std::make_shared<GlossyMaterial>();
    objects.push_back( std::make_shared<Sphere>(glm::vec3(-3,1.0,0.0), 1.0, glossyMaterial) );

    float ior = 1.5; //Glass
    FresnelMaterialPtr fresnelMaterial = std::make_shared<FresnelMaterial>(ior);
    objects.push_back( std::make_shared<Sphere>(glm::vec3(3,1.0,0.0), 1.0, fresnelMaterial) );

    objects.push_back( std::make_shared<Plane>(glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,-1,0.0), PhongMaterial::Pearl()) );

    //string meshFilename ="./../../raytracer-sandbox/meshes/suzanneLowRes.obj";
    //objects.push_back( std::make_shared<TMesh>(meshFilename, PhongMaterial::Emerald()) );

    std::vector<glm::vec2> pixelOffset;
    pixelOffset.push_back(glm::vec2(0,0));
    pixelOffset.push_back(glm::vec2(0.5,0));
    pixelOffset.push_back(glm::vec2(0,0.5));
    pixelOffset.push_back(glm::vec2(0.5,0.5));
    int depth = 0;

    bool success = true;
    for(int i=0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            glm::vec3 pixelColor(0,0,0);
            for(size_t k=0; k<pixelOffset.size(); ++k)
            {
                Ray viewRay = camera.computeRayThroughPixel( i+pixelOffset[k][0], j+pixelOffset[k][1] );
                pixelColor += castRay(viewRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth);
            }
            pixelColor/=(float)(pixelOffset.size());
        }
    }
    EXPECT_EQ(success,true);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
