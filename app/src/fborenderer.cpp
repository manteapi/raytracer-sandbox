#include "./../include/fborenderer.hpp"
#include <QOpenGLFramebufferObjectFormat>
#include <QQuickWindow>
#include <iostream>
#include <QImage>

FBORenderer::~FBORenderer()
{

}

FBORenderer::FBORenderer() : m_initialisation(false), m_backgroundIsDirty(false)
{
}

void FBORenderer::paint()
{
    if(m_initialisation==false)
    {
        m_initialisation=true;

        //Create a shader to render a textured mesh
        QString m_vShaderPath = "://glsl/defaultVertexShader.glsl";
        QString m_fShaderPath = "://glsl/defaultFragmentShader.glsl";
        texturedMeshShader = std::make_shared<ShaderProgram>(m_vShaderPath, m_fShaderPath);

        //Create a FBO to render a texture into it
        backgroundFBO = std::make_shared<FrameBufferObject>(m_viewportSize);

        //Create a texture to load an image on the GPU
        backgroundTexture = std::make_shared<TextureObject>();
        bool loadImageIsOk = false;
        QTransform verticalFlip;
        QImage image;
        verticalFlip.rotate(180, Qt::Axis::XAxis);
        QString imagePath = QString(":/graphical-assets/paper_background");
        loadImageIsOk = image.load(imagePath);
        if(!loadImageIsOk)
        {
            std::cout << "Image not found : " << imagePath.toStdString() << std::endl;
        }
        image = image.transformed(verticalFlip);
        backgroundTexture->loadImage(image);

        //Create a unit square mesh and transfer data to GPU
        std::vector<glm::vec2> positions, texCoords;
        std::vector<glm::vec4> colors;
        unitSquare(positions, colors, texCoords);
        unitSquareMesh = std::make_shared<MeshBufferObject>();
        unitSquareMesh->loadPositions(positions);
        unitSquareMesh->loadColors(colors);
        unitSquareMesh->loadTexCoords(texCoords);
    }

    if(m_backgroundIsDirty)
    {
        //Update the texture content if required
        m_backgroundIsDirty = false;
        QTransform verticalFlip;
        verticalFlip.rotate(180, Qt::Axis::XAxis);
        m_tempBackground = m_tempBackground.transformed(verticalFlip);
        backgroundTexture->loadImage(m_tempBackground);
    }

    //Draw the texture into the framebuffer object
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    backgroundFBO->bind();
    glFuncs.glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    glFuncs.glEnable(GL_TEXTURE_2D);
    glFuncs.glEnable(GL_DEPTH_TEST);
    glFuncs.glClearColor(1.0, 1.0, 1.0, 1.0);
    glFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    texturedMeshShader->bind();
    unitSquareMesh->draw(texturedMeshShader->m_pLocation, texturedMeshShader->m_cLocation, backgroundTexture->m_texId, texturedMeshShader->m_texSamplerLocation, texturedMeshShader->m_texCoordLocation);
    texturedMeshShader->release();
    glFuncs.glDisable(GL_DEPTH_TEST);
    glFuncs.glDisable(GL_TEXTURE_2D);
    backgroundFBO->release();

    //Draw the framebuffer texture into the main framebuffer
    glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glFuncs.glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    glFuncs.glEnable(GL_TEXTURE_2D);
    glFuncs.glDisable(GL_DEPTH_TEST);
    glFuncs.glClearColor(0, 0, 0, 1);
    glFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    texturedMeshShader->bind();
    unitSquareMesh->draw(texturedMeshShader->m_pLocation, texturedMeshShader->m_cLocation, backgroundFBO->m_fboTexColorId, texturedMeshShader->m_texSamplerLocation, texturedMeshShader->m_texCoordLocation);
    texturedMeshShader->release();
    glFuncs.glDisable(GL_DEPTH_TEST);
    glFuncs.glDisable(GL_TEXTURE_2D);
    glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    m_backgroundIsDirty = true;
    m_tempBackground = image;
}

QImage FBORenderer::getBackgroundImage() const
{
    return m_tempBackground;
}
