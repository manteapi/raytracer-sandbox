#ifndef FBORENDERER_HPP
#define FBORENDERER_HPP

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>

#include "gpuhelper.hpp"

class FBORenderer : public QObject, protected QOpenGLFunctions
{
public:
    ~FBORenderer();
    FBORenderer();

public Q_SLOTS:
    void paint();
    void setViewportSize(const QSize &size);
    void setWindow(QQuickWindow *window);
    void setBackgroundImage(const QImage& image);
    QImage getBackgroundImage() const;
private:
    QSize m_viewportSize;
    QQuickWindow *m_window;
    bool m_initialisation;

    bool m_backgroundIsDirty;
    QImage m_tempBackground;

    ShaderProgramPtr texturedMeshShader = nullptr;
    FrameBufferObjectPtr secondaryFBO = nullptr;
    TextureObjectPtr backgroundTexture = nullptr;
    MeshBufferObjectPtr unitSquareMesh = nullptr;

    MeshBufferObjectPtr objMesh = nullptr;
    TextureObjectPtr objTexture = nullptr;
};

#endif // FBORENDERER_HPP
