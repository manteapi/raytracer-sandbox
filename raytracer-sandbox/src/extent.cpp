#include "./../include/raytracer-sandbox/extent.hpp"

using namespace std;

const vector<glm::vec3>& ExtentSettings::planeSetNormals()
{
    return m_planeSetNormals;
}

ExtentSettings::ExtentSettings( const vector<glm::vec3>& planeSetNormals )
{
    m_planeSetNormals = planeSetNormals;
}

ExtentSettingsPtr ExtentSettings::AABB_Settings()
{
    vector<glm::vec3> normals = { glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1) };
    ExtentSettingsPtr settings = make_shared<ExtentSettings>(normals);
    return settings;
}

Extent::Extent( std::array<glm::vec3,2>& bounds )
{
    std::vector<glm::vec3> points = {{ bounds[0], bounds[1] }};
    m_settings = ExtentSettings::AABB_Settings();
    computeSlabOffsets(points);
    computeBounds(points);
}

Extent::Extent( const ExtentSettingsPtr& settings, vector<glm::vec3>& points) : m_settings(settings)
{
    computeSlabOffsets(points);
    computeBounds(points);
}

void Extent::computeSlabOffsets(const vector<glm::vec3>& points)
{
    if(m_settings!=nullptr)
    {
        //Resize the result to the number of plane-set normals
        vector< array<float,2> > slabOffsets;
        const vector<glm::vec3>& planeSetNormals = m_settings->planeSetNormals();
        size_t numberOfPlaneSet = planeSetNormals.size();
        slabOffsets.resize(numberOfPlaneSet);

        //Initialize the slabs offsets.
        for( size_t i=0; i<numberOfPlaneSet; ++i)
        {
            slabOffsets[i][0] = numeric_limits<float>::max();
            slabOffsets[i][1] = -numeric_limits<float>::max();
        }

        //For each plane, for each points, update slab offsets
        for( size_t i=0; i< numberOfPlaneSet; ++i)
        {
            for(size_t j=0; j<points.size(); ++j)
            {
                float dij = glm::dot( planeSetNormals[i], points[j] );
                slabOffsets[i][0] = min(dij, slabOffsets[i][0]);
                slabOffsets[i][1] = max(dij, slabOffsets[i][1]);
            }
        }
        m_slabOffsets = slabOffsets;
    }
}

void Extent::computeBounds( const vector<glm::vec3>& points )
{
    array<glm::vec3,2> bounds;
    for(size_t i=0; i<3; ++i)
    {
        bounds[0][i] = numeric_limits<float>::max();
        bounds[1][i] = -numeric_limits<float>::max();
    }

    for(size_t i=0; i<points.size(); ++i)
    {
        for(size_t j=0; j<3; ++j)
        {
            bounds[0][j] = min(bounds[0][j], points[i][j]);
            bounds[1][j] = max(bounds[1][j], points[i][j]);
        }
    }
    m_bounds = bounds;
}

const ExtentSettingsPtr& Extent::settings()
{
    return m_settings;
}

const vector< array<float, 2> > &Extent::slabOffsets()
{
    return m_slabOffsets;
}

const std::array<glm::vec3,2>& Extent::bounds()
{
    return m_bounds;
}
