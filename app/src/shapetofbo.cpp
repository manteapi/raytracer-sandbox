#include "shapetofbo.hpp"

#include <QOpenGLExtraFunctions>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace  std;

BackgroundFBO::BackgroundFBO()
{
    m_program = nullptr;
    m_programId = 0;

    m_fbo = 0;
    m_fboTexColorId = 0;
    m_rbo = 0;


    //GPU Location
    m_brushColorLocation = -1;
    m_mLocation = -1;
    m_pLocation = -1;
    m_cLocation = -1;
    m_texCoordLocation = -1;
    m_texSamplerLocation = -1;

    //GPU Buffer
    m_pBuffer = 0;
    m_cBuffer = 0;
    m_texCoordBuffer = 0;

    //GPU Texture
    m_texId = -1;
    m_fboTexColorId = -1;

    //CPU Data
    m_viewportSize = QSize(0,0);
    m_image = QImage();
    m_positions.clear();
    m_colors.clear();
    m_texCoords.clear();
}

BackgroundFBO::~BackgroundFBO()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());

    glFuncs.glDeleteFramebuffers(1, &m_fbo);
    glFuncs.glDeleteTextures(1, &m_fboTexColorId);
    glFuncs.glDeleteRenderbuffers(1, &m_rbo);

    glFuncs.glDeleteProgram(m_programId);

    glFuncs.glDeleteBuffers(1, &m_pBuffer);
    glFuncs.glDeleteBuffers(1, &m_cBuffer);
    glFuncs.glDeleteBuffers(1, &m_texCoordBuffer);

    glFuncs.glDeleteTextures(1, &m_texId);
}

BackgroundFBO::BackgroundFBO(const QSize &viewportSize, const QString& vShaderFilename, const QString& fShaderFilename)
{
    m_viewportSize = viewportSize;
    m_imageIsDirty = false;

    //Create shader program and get attributes/uniform locations
    {
        QOpenGLShader* vShader = new QOpenGLShader(QOpenGLShader::Vertex);
        vShader->compileSourceFile(vShaderFilename);
        QOpenGLShader* fShader = new QOpenGLShader(QOpenGLShader::Fragment);
        fShader->compileSourceFile(fShaderFilename);
        m_program = new QOpenGLShaderProgram();
        m_program->addShader(vShader);
        m_program->addShader(fShader);
        m_program->link();
        m_programId = m_program->programId();
        m_pLocation = m_program->attributeLocation("vertices");
        m_cLocation = m_program->attributeLocation("colors");
        m_texCoordLocation = m_program->attributeLocation("texCoord");
        m_texSamplerLocation = m_program->uniformLocation("texSampler");
    }


    //Init CPU Data
    {
        bool loadImageIsOk = false;
        QTransform verticalFlip;
        verticalFlip.rotate(180, Qt::Axis::XAxis);

        if(m_image.isNull())
        {
            QString imagePath = QString(":/graphical-assets/paper_background");
            loadImageIsOk = m_image.load(imagePath);
            if(!loadImageIsOk)
            {
                std::cout << "Image not found : " << imagePath.toStdString() << std::endl;
            }
            m_image = m_image.transformed(verticalFlip);
        }

        std::vector< glm::vec2 > positions =
        {
            glm::vec2(-1.0,-1.0), glm::vec2(1.0,-1.0), glm::vec2(1.0,1.0),
            glm::vec2(-1.0,-1.0), glm::vec2(1.0,1.0), glm::vec2(-1.0,1.0)
        };
        m_positions = positions;

        std::vector< glm::vec2 > texCoords =
        {
            glm::vec2(0.0,0.0), glm::vec2(1.0,0.0), glm::vec2(1.0,1.0),
            glm::vec2(0.0,0.0), glm::vec2(1.0,1.0), glm::vec2(0.0,1.0)
        };
        m_texCoords = texCoords;

        std::vector< glm::vec4 > colors =
        {
            glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0),
            glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0)
        };
        m_colors = colors;
    }

    //Allocation GPU memory
    {
        QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());

        //Position buffer
        glFuncs.glGenBuffers(1, &m_pBuffer);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
        glFuncs.glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec2), m_positions.data(), GL_STATIC_DRAW);

        //Color buffer
        glFuncs.glGenBuffers(1, &m_cBuffer);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
        glFuncs.glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW);

        //TexCoords Buffer
        glFuncs.glGenBuffers(1, &m_texCoordBuffer);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
        glFuncs.glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);

        //Load texture 0
        //Tex Gen + options
        glFuncs.glGenTextures(1, &m_texId);
        glFuncs.glBindTexture(GL_TEXTURE_2D, m_texId);
        glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_image.width(), m_image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (const GLvoid*)m_image.bits());
        glFuncs.glBindTexture(GL_TEXTURE_2D, 0);

        //Create a frame buffer
        glFuncs.glGenFramebuffers(1, &m_fbo);
        glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        //Create a texture for writting color
        glFuncs.glGenTextures(1, &m_fboTexColorId);
        glFuncs.glBindTexture(GL_TEXTURE_2D, m_fboTexColorId);
        glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_viewportSize.width(), m_viewportSize.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        //Attach the texture to the framebuffer - Set the texture as the color attachment 0
        glFuncs.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexColorId, 0);

        glFuncs.glGenRenderbuffers(1, &m_rbo);
        glFuncs.glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glFuncs.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_viewportSize.width(), m_viewportSize.height());
        glFuncs.glBindRenderbuffer(GL_RENDERBUFFER, 0);
        //Attach the render buffer to the framebuffer
        glFuncs.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
        //Check framebuffer status
        if(glFuncs.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Incomplete FrameBuffer" << std::endl;
            exit(EXIT_FAILURE);
        }
        glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void BackgroundFBO::reloadImageOnGPU()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindTexture(GL_TEXTURE_2D, m_texId);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_image.width(), m_image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (const GLvoid*)m_image.bits());
    glFuncs.glBindTexture(GL_TEXTURE_2D, 0);
}

void BackgroundFBO::draw()
{
    if(m_imageIsDirty)
    {
        m_imageIsDirty = false;        
        reloadImageOnGPU();
    }

    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glFuncs.glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    glFuncs.glEnable(GL_TEXTURE_2D);
    glFuncs.glEnable(GL_DEPTH_TEST);
    glFuncs.glClearColor(1.0, 1.0, 1.0, 1.0);
    glFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //FBO Program
    {
        glFuncs.glUseProgram(m_programId);

        glFuncs.glEnableVertexAttribArray(m_pLocation);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
        glFuncs.glVertexAttribPointer(m_pLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glFuncs.glEnableVertexAttribArray(m_cLocation);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
        glFuncs.glVertexAttribPointer(m_cLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glFuncs.glActiveTexture(GL_TEXTURE0);
        glFuncs.glBindTexture(GL_TEXTURE_2D, m_texId);
        glFuncs.glUniform1i(m_texSamplerLocation, 0);
        glFuncs.glEnableVertexAttribArray(m_texCoordLocation);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
        glFuncs.glVertexAttribPointer(m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glFuncs.glDrawArrays(GL_TRIANGLES,0, m_positions.size());

        glFuncs.glBindTexture(GL_TEXTURE_2D, 0);
        glFuncs.glDisableVertexAttribArray(m_pLocation);
        glFuncs.glDisableVertexAttribArray(m_cLocation);
        glFuncs.glDisableVertexAttribArray(m_texCoordLocation);

        glFuncs.glUseProgram(0);
    }

    glFuncs.glDisable(GL_DEPTH_TEST);
    glFuncs.glDisable(GL_TEXTURE_2D);

    glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BackgroundFBO::setImage(const QImage& image)
{
    QTransform verticalFlip;
    verticalFlip.rotate(180, Qt::Axis::XAxis);
    m_image = image.transformed(verticalFlip);
    m_imageIsDirty = true;
}

QImage BackgroundFBO::getImage() const
{
    return m_image;
}

bool BackgroundFBO::getImageIsDirty()
{
    return m_imageIsDirty;
}

void BackgroundFBO::setImageIsDirty(const bool& value)
{
    m_imageIsDirty = value;
}

////////////////////////////////////////////////////////////////////

ShapeToMainFBO::ShapeToMainFBO()
{
    m_program = nullptr;
    m_programId = 0;

    m_fbo = 0;
    m_fboTexColorId = 0;

    //GPU Location
    m_pLocation = -1;
    m_cLocation = -1;
    m_texCoordLocation = -1;
    m_texSamplerLocation = -1;

    //GPU Buffer
    m_pBuffer = 0;
    m_cBuffer = 0;
    m_texCoordBuffer = 0;

    //GPU Texture
    m_texId = -1;

    //CPU Data
    m_viewportSize = QSize(0,0);
    m_positions.clear();
    m_colors.clear();
    m_texCoords.clear();
}

ShapeToMainFBO::ShapeToMainFBO(const QSize &viewportSize, const QString& vShaderFilename, const QString& fShaderFilename, const int& fboTexColorId)
{
    m_viewportSize = viewportSize;
    m_fboTexColorId = fboTexColorId;

    //Init CPU Data
    {
        std::vector< glm::vec2 > positions =
        {
            glm::vec2(-1.0,-1.0), glm::vec2(1.0,-1.0), glm::vec2(1.0,1.0),
            glm::vec2(-1.0,-1.0), glm::vec2(1.0,1.0), glm::vec2(-1.0,1.0)
        };
        m_positions = positions;

        std::vector< glm::vec2 > texCoords =
        {
            glm::vec2(0.0,0.0), glm::vec2(1.0,0.0), glm::vec2(1.0,1.0),
            glm::vec2(0.0,0.0), glm::vec2(1.0,1.0), glm::vec2(0.0,1.0)
        };
        m_texCoords = texCoords;

        std::vector< glm::vec4 > colors =
        {
            glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0),
            glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0)
        };
        m_colors = colors;
    }

    //Create shader program and get attributes/uniform locations
    {
        QOpenGLShader* vShader = new QOpenGLShader(QOpenGLShader::Vertex);
        vShader->compileSourceFile(vShaderFilename);
        QOpenGLShader* fShader = new QOpenGLShader(QOpenGLShader::Fragment);
        fShader->compileSourceFile(fShaderFilename);
        m_program = new QOpenGLShaderProgram();
        m_program->addShader(vShader);
        m_program->addShader(fShader);
        m_program->link();
        m_pLocation = m_program->attributeLocation("vertices");
        m_cLocation = m_program->attributeLocation("colors");
        m_texCoordLocation = m_program->attributeLocation("texCoord");
        m_texSamplerLocation = m_program->uniformLocation("texSampler");
    }

    //Allocation GPU memory
    {
        QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
        glFuncs.glGenBuffers(1, &m_pBuffer);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
        glFuncs.glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec2), m_positions.data(), GL_STATIC_DRAW);
        glFuncs.glGenBuffers(1, &m_cBuffer);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
        glFuncs.glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW);
        glFuncs.glGenBuffers(1, &m_texCoordBuffer);
        glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
        glFuncs.glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);
    }
}

void ShapeToMainFBO::draw()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());

    GLuint m_programId = m_program->programId();
    glFuncs.glUseProgram(m_programId);

    glFuncs.glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    glFuncs.glDisable(GL_DEPTH_TEST);
    glFuncs.glClearColor(0, 0, 0, 1);
    glFuncs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFuncs.glEnableVertexAttribArray(m_pLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glFuncs.glVertexAttribPointer(m_pLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glFuncs.glEnableVertexAttribArray(m_cLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
    glFuncs.glVertexAttribPointer(m_cLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glFuncs.glActiveTexture(GL_TEXTURE0);
    glFuncs.glBindTexture(GL_TEXTURE_2D, m_fboTexColorId);
    glFuncs.glUniform1i(m_texSamplerLocation, 0);
    glFuncs.glEnableVertexAttribArray(m_texCoordLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glFuncs.glVertexAttribPointer(m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glFuncs.glDrawArrays(GL_TRIANGLES, 0, 6);

    glFuncs.glBindTexture(GL_TEXTURE_2D, 0);

    glFuncs.glDisableVertexAttribArray(m_pLocation);
    glFuncs.glDisableVertexAttribArray(m_cLocation);
    glFuncs.glDisableVertexAttribArray(m_texCoordLocation);

    glFuncs.glDisable(GL_TEXTURE_2D);

    glFuncs.glUseProgram(0);
}

ShapeToMainFBO::~ShapeToMainFBO()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());

    glFuncs.glDeleteFramebuffers(1, &m_fbo);
    glFuncs.glDeleteTextures(1, &m_fboTexColorId);

    glFuncs.glDeleteProgram(m_programId);

    glFuncs.glDeleteBuffers(1, &m_pBuffer);
    glFuncs.glDeleteBuffers(1, &m_cBuffer);
    glFuncs.glDeleteBuffers(1, &m_texCoordBuffer);

    glFuncs.glDeleteTextures(1, &m_texId);
}

