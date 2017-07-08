#include "./../include/fborenderer.hpp"
#include <QOpenGLFramebufferObjectFormat>
#include <QQuickWindow>

FBORenderer::~FBORenderer()
{

}

FBORenderer::FBORenderer()
{
}

void FBORenderer::paint()
{
    if(m_backgroundFbo==nullptr)
    {
        QString m_vShaderPath = "://glsl/bgVertexShader.glsl";
        QString m_fShaderPath = "://glsl/bgFragmentShader.glsl";
        m_backgroundFbo = std::make_shared<BackgroundFBO>(m_viewportSize, m_vShaderPath, m_fShaderPath);
    }
    if(m_shapeToMainFbo==nullptr)
    {
        QString m_vShaderPath = "://glsl/defaultVertexShader.glsl";
        QString m_fShaderPath = "://glsl/defaultFragmentShader.glsl";
        m_shapeToMainFbo = std::make_shared<ShapeToMainFBO>(m_viewportSize, m_vShaderPath, m_fShaderPath, m_backgroundFbo->m_fboTexColorId);
    }

    //Draw background
    m_backgroundFbo->draw();

    //Draw mixed fbo to main fbo
    m_shapeToMainFbo->draw();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}

void FBORenderer::setViewportSize(const QSize &size)
{
    m_viewportSize = size;
}

void FBORenderer::setWindow(QQuickWindow *window)
{
    m_window = window;
}

void FBORenderer::setBackgroundImage(const QImage& image)
{
    m_backgroundFbo->setImage(image);
}

QImage FBORenderer::getBackgroundImage() const
{
    return m_backgroundFbo->getImage();
}
