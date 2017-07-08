#include <iostream>
#include <gtest/gtest.h>
#include <raytracer-sandbox/extent.hpp>

using namespace std;

TEST(ExtentSettings, Default_Constructor)
{
    ExtentSettings settings;
    EXPECT_EQ(settings.planeSetNormals().empty(), true);
}

TEST(ExtentSettings, Specific_Constructor)
{
    std::vector< glm::vec3 > planeSetNormals;
    
    ExtentSettings settings(planeSetNormals);
    const std::vector< glm::vec3 >& s_normals = settings.planeSetNormals();
    
    EXPECT_EQ(planeSetNormals.size(), s_normals.size());
    
    for(size_t i=0; i<planeSetNormals.size(); ++i)
    {
        for(size_t j=0; j<3; ++j)
        {
            EXPECT_EQ(planeSetNormals[i][j], s_normals[i][j]);
        }
    }
}

TEST(ExtentSettings, AABB_Constructor)
{
    std::vector< glm::vec3 > planeSetNormals;
    planeSetNormals.push_back(glm::vec3(1,0,0));
    planeSetNormals.push_back(glm::vec3(0,1,0));
    planeSetNormals.push_back(glm::vec3(0,0,1));
    
    ExtentSettingsPtr settings = ExtentSettings::AABB_Settings();
    const std::vector< glm::vec3 >& s_normals = settings->planeSetNormals();

    EXPECT_EQ(planeSetNormals.size(), s_normals.size());
    
    for(size_t i=0; i<planeSetNormals.size(); ++i)
    {
        for(size_t j=0; j<3; ++j)
        {
            EXPECT_EQ(planeSetNormals[i][j], s_normals[i][j]);
        }
    }
}

TEST(ExtentSettings, Clone_Constructor)
{
    ExtentSettingsPtr aabb_settings = ExtentSettings::AABB_Settings();
    ExtentSettings settings(*aabb_settings);

    const std::vector< glm::vec3 >& normals1 = aabb_settings->planeSetNormals();
    const std::vector< glm::vec3 >& normals2 = settings.planeSetNormals();
    
    EXPECT_EQ(normals1.size(), normals2.size());
    
    for(size_t i=0; i<normals1.size(); ++i)
    {
        for(size_t j=0; j<3; ++j)
        {
            EXPECT_EQ(normals1[i][j], normals2[i][j]);
        }
    }
}

TEST(Extent, Default_Constructor)
{
    ExtentSettingsPtr settings=nullptr;
    std::vector<glm::vec3> points;
    Extent extent(settings, points);
    EXPECT_EQ(extent.slabOffsets().empty(), true);
    EXPECT_EQ(extent.settings(), nullptr);
    const std::array<glm::vec3,2> bounds = extent.bounds();
    for(size_t i=0; i<3; ++i)
    {
        EXPECT_EQ(bounds[0][i], numeric_limits<float>::max());
        EXPECT_EQ(bounds[1][i], -numeric_limits<float>::max());
    }
}

TEST(Extent, Specific_Constructor)
{
    ExtentSettingsPtr settings=ExtentSettings::AABB_Settings();
    std::vector<glm::vec3> points;
    points.push_back( glm::vec3(44,44,44) );
    points.push_back( glm::vec3(40,40,40) );
    Extent extent(settings, points);
    //Slabs
    const std::vector< std::array<float,2> > slabs = extent.slabOffsets();
    EXPECT_EQ(slabs.size(), 3);
    for(size_t i=0; i<slabs.size(); ++i)
    {
        EXPECT_EQ(slabs[i][0], 40);
        EXPECT_EQ(slabs[i][1], 44);
    }
    //Bounds
    const std::array<glm::vec3,2> bounds = extent.bounds();
    EXPECT_EQ(bounds[0][0], 40);
    EXPECT_EQ(bounds[0][1], 40);
    EXPECT_EQ(bounds[0][2], 40);
    EXPECT_EQ(bounds[1][0], 44);
    EXPECT_EQ(bounds[1][1], 44);
    EXPECT_EQ(bounds[1][2], 44);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
