#include <iostream>
#include <raytracer-sandbox/camera.hpp>
#include <gtest/gtest.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

//Camera::Constructor tests the specific constructor of the camera
//along with all its getters.
TEST(Camera, Constructor)
{
    float fov=100.0, width=1280, height=720, near=0, far=100;
    float ratio = (float)width/(float)height;
    glm::mat4 view(1.0), projection(1.0);
    projection = glm::perspective(fov, ratio, near, far);
    Camera camera(fov, width, height, near, far);

    EXPECT_EQ(camera.fov(), fov);
    EXPECT_EQ(camera.width(), width);
    EXPECT_EQ(camera.height(), height);
    EXPECT_EQ(camera.znear(), near);
    EXPECT_EQ(camera.zfar(), far);
    EXPECT_EQ(camera.ratio(), ratio);
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            EXPECT_EQ(camera.view()[i][j],view[i][j]);
            EXPECT_EQ(camera.projection()[i][j],projection[i][j]);
        }
    }
}

TEST(Camera, GetterSetter)
{
    float fov=100.0, width=1280, height=720, near=0, far=100;
    glm::mat4 view = glm::rotate(view, float(M_PI), glm::vec3(0.0,1.0,0.0));
    Camera camera(fov, width, height, near, far);

    //Setter
    camera.view() = view;
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            EXPECT_EQ(camera.view()[i][j],view[i][j]);
        }
    }

    //Getter
    const Camera constCamera(fov, width, height, near, far);
    const glm::mat4& constView = constCamera.view();
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            EXPECT_EQ(constCamera.view()[i][j],constView[i][j]);
        }
    }
}

TEST(Camera, ComputePosition)
{
    float fov=100.0, width=1280, height=720, near=0, far=100;
    Camera camera(fov, width, height, near, far);

    glm::vec3 cP1 = camera.computePosition();
    EXPECT_EQ(cP1[0], 0);
    EXPECT_EQ(cP1[2], 0);
    EXPECT_EQ(cP1[2], 0);

    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(1,-2,3));
    camera.view() = view;
    glm::vec3 cP2 = camera.computePosition();
    EXPECT_EQ(cP2[0], -1);
    EXPECT_EQ(cP2[1], 2);
    EXPECT_EQ(cP2[2], -3);
}

TEST(Camera, PixelToWorld)
{
    float fov=100.0, width=1280, height=720, near=1, far=100;
    Camera camera(fov, width, height, near, far);

    //The camera is looking to -Z.
    //The near plane distance is 1.
    //So the pixel at the middle of the screen should be at (0,0,-1).
    glm::vec3 p1 = camera.pixelToWorld(width/2.0, height/2.0);
    EXPECT_EQ(p1[0], 0);
    EXPECT_EQ(p1[1], 0);
    EXPECT_EQ(p1[2], -1);
}

TEST(Camera, ComputeRayThroughPixel)
{
    float fov=100.0, width=1280, height=720, near=1, far=100;
    Camera camera(fov, width, height, near, far);

    //The camera is looking to -Z.
    //The near plane distance is 1.
    //So the pixel at the middle of the screen should be at (0,0,-1).
    //The ray should start from camera position and have a direction of (0,0,-1).
    Ray r = camera.computeRayThroughPixel(width/2.0, height/2.0);
    glm::vec3 cameraPosition = camera.computePosition();
    EXPECT_EQ(r.origin()[0], cameraPosition[0]);
    EXPECT_EQ(r.origin()[1], cameraPosition[1]);
    EXPECT_EQ(r.origin()[2], cameraPosition[2]);
    EXPECT_EQ(r.direction()[0], 0.0);
    EXPECT_EQ(r.direction()[1], 0.0);
    EXPECT_EQ(r.direction()[2], -1.0);
}

TEST(Camera, OutputStream)
{
    float fov=100.0, width=1280, height=720, near=1, far=100;
    Camera camera(fov, width, height, near, far);
    ostringstream cStream1;
    cStream1 << camera;

    ostringstream cStream2;
    cStream2 << "FOV : " << camera.fov() << endl;
    cStream2 << "Ratio : " << camera.ratio() << endl;
    cStream2 << "width : " << camera.width() << endl;
    cStream2 << "height : " << camera.height() << endl;
    cStream2 << "znear: " << camera.znear() << endl;
    cStream2 << "zfar: " << camera.zfar() << endl;
    glm::vec3 cameraPosition = camera.computePosition();
    cStream2 << "Camera position : " << cameraPosition[0] << ", " << cameraPosition[1] << ", " << cameraPosition[2] << endl;
    glm::vec3 screenPosition = camera.pixelToWorld((int)(camera.width()/2.0f), (int)(camera.height()/2.0f));
    cStream2 << "Screen center position : " << screenPosition[0] << ", " << screenPosition[1] << ", " << screenPosition[2] << endl;

    string cString1 = cStream1.str();
    string cString2 = cStream2.str();

    EXPECT_EQ(cString1, cString2);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
