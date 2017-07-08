#ifndef SHAPETOFBO_H
#define SHAPETOFBO_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QImage>

#include <memory>

#include <glm/glm.hpp>

class BackgroundFBO
{
public:
    BackgroundFBO();
    ~BackgroundFBO();
    BackgroundFBO(const QSize& viewportSize, const QString& vShaderFilename, const QString& fShaderFilename);
    void reloadImageOnGPU();
    void draw();
    void setImage(const QImage& image);
    QImage getImage() const;
    bool getImageIsDirty();
    void setImageIsDirty(const bool& value);

public:

    //FBO & RBO
    GLuint m_fbo;
    GLuint m_fboTexColorId;
    GLuint m_rbo;

    //Shader program
    QOpenGLShaderProgram* m_program;
    GLuint m_programId;

    //GPU Location
    GLint m_brushColorLocation;
    GLint m_mLocation;
    GLint m_pLocation;
    GLint m_cLocation;
    GLint m_texCoordLocation;
    GLint m_texSamplerLocation;

    //GPU Buffer
    GLuint m_pBuffer;
    GLuint m_cBuffer;
    GLuint m_texCoordBuffer;

    //GPU Texture
    GLuint m_texId;

    //CPU Data
    QSize m_viewportSize;
    QImage m_image;
    bool m_imageIsDirty;
    std::vector< glm::vec2 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec2 > m_texCoords;
};

typedef std::shared_ptr<BackgroundFBO> BackgroundFBOPtr;

class ShapeToMainFBO
{
public:
    ShapeToMainFBO();
    ~ShapeToMainFBO();
    ShapeToMainFBO(const QSize& viewportSize, const QString& vShaderFilename, const QString& fShaderFilename, const int &fboTexColorId);
    void draw();

public:

    //FBO & RBO
    GLuint m_fbo;
    GLuint m_fboTexColorId;

    //Shader program
    QOpenGLShaderProgram* m_program;
    GLuint m_programId;

    //GPU Location
    GLint m_pLocation;
    GLint m_cLocation;
    GLint m_texCoordLocation;
    GLint m_texSamplerLocation;

    //GPU Buffer
    GLuint m_pBuffer;
    GLuint m_cBuffer;
    GLuint m_texCoordBuffer;

    //GPU Texture
    GLuint m_texId;

    //CPU Data
    QSize m_viewportSize;
    std::vector< glm::vec2 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec2 > m_texCoords;
};

typedef std::shared_ptr<ShapeToMainFBO> ShapeToMainFBOPtr;

#endif // SHAPETOFBO_H
