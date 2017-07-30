#ifndef GPU_HELPER_H
#define GPU_HELPER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QImage>

#include <memory>

#include <glm/glm.hpp>


void unitSquare(std::vector<glm::vec2>& positions, std::vector<glm::vec4>& colors, std::vector<glm::vec2>& texCoords);

class ShaderProgram
{
public:
    ShaderProgram() = delete;
    ShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath);
    ~ShaderProgram();
    QOpenGLShaderProgram* m_program;
    void bind();
    void release();
    GLuint m_programId;
    GLint m_pLocation;
    GLint m_cLocation;
    GLint m_texCoordLocation;
    GLint m_texSamplerLocation;
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;

class FrameBufferObject
{
public:
    FrameBufferObject() = delete;
    FrameBufferObject(const QSize& size);
    ~FrameBufferObject();
    void bind();
    void release();
    GLuint m_fbo;
    GLuint m_fboTexColorId;
    GLuint m_rbo;
    QSize m_size;
};

typedef std::shared_ptr<FrameBufferObject> FrameBufferObjectPtr;

class TextureObject
{
public:
    TextureObject();
    ~TextureObject();
    void loadImage(QImage& image);
    GLuint m_texId;
};

typedef std::shared_ptr<TextureObject> TextureObjectPtr;

class MeshBufferObject
{
public:
    MeshBufferObject();
    ~MeshBufferObject();
    void loadPositions(std::vector<glm::vec2>& data);
    void loadColors(std::vector<glm::vec4>& data);
    void loadTexCoords(std::vector<glm::vec2>& data);
    void draw(GLint pLocation, GLint cLocation);
    void draw(GLint pLocation, GLint cLocation, GLuint texId, GLint tSamplerLocation, GLint tLocation);
    GLuint m_pBuffer;
    GLuint m_cBuffer;
    GLuint m_tBuffer;
    size_t m_size;
};

typedef std::shared_ptr<MeshBufferObject> MeshBufferObjectPtr;

#endif // GPU_HELPER_H
