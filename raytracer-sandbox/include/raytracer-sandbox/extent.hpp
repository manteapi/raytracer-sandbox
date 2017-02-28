#ifndef EXTENT_HPP
#define EXTENT_HPP

#include <vector>
#include <glm/glm.hpp>
#include <memory>

class ExtentSettings
{
public:
    typedef std::shared_ptr<ExtentSettings> ExtentSettingsPtr;

    ~ExtentSettings() = default;
    ExtentSettings() = default;
    ExtentSettings( const std::vector<glm::vec3>& planeSetNormals );
    ExtentSettings( const ExtentSettings& extentSettings ) = default;
    const std::vector<glm::vec3>& planeSetNormals();
    static ExtentSettingsPtr AABB_Settings();
private:
    std::vector<glm::vec3> m_planeSetNormals;
};

typedef std::shared_ptr<ExtentSettings> ExtentSettingsPtr;

class Extent
{
public:
    ~Extent() = default;
    Extent() = delete;
    Extent( const ExtentSettingsPtr& settings, std::vector<glm::vec3>& points );
    Extent( const Extent& extent ) = default;
    Extent( const ExtentSettingsPtr& settings );
    const ExtentSettingsPtr& settings();
    const std::vector< std::array<float,2> >& slabOffsets();
    const std::array<float,2> & bounds();
    static std::vector< std::array<float,2> > computeSlabOffsets( const ExtentSettingsPtr& settings, const std::vector<glm::vec3>& points);
    static std::array< glm::vec3, 2 > computeBounds( const std::vector<glm::vec3>& points );
private:
    ExtentSettingsPtr m_settings;
    std::vector< std::array<float,2> > m_slabOffsets; /*!< Pair of in/out distance to origins for each plane-set normal*/
    std::array< glm::vec3, 2 > m_bounds;
};

#endif // EXTENT_HPP
