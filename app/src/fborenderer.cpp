#include "./../include/fborenderer.hpp"
#include <QOpenGLFramebufferObjectFormat>
#include <QQuickWindow>

FBORenderer::~FBORenderer()
{

}

FBORenderer::FBORenderer()
{
}

void FBORenderer::synchronize(QQuickFramebufferObject *item)
{
     m_item = static_cast<Viewer*>(item);
 }

QOpenGLFramebufferObject* FBORenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

void FBORenderer::render()
{
    m_item->window()->resetOpenGLState();
}
