#include "gpuhelper.hpp"

#include <QOpenGLExtraFunctions>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <raytracer-sandbox/io.hpp>

using namespace  std;

ShaderProgram::ShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath)
{
    QOpenGLShader* vShader = new QOpenGLShader(QOpenGLShader::Vertex);
    vShader->compileSourceFile(vertexShaderPath);
    QOpenGLShader* fShader = new QOpenGLShader(QOpenGLShader::Fragment);
    fShader->compileSourceFile(fragmentShaderPath);
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

ShaderProgram::~ShaderProgram()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glDeleteProgram(m_programId);
    delete m_program;
}

void ShaderProgram::bind()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glUseProgram(m_programId);
}

void ShaderProgram::release()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glUseProgram(0);
}

FrameBufferObject::FrameBufferObject(const QSize &size) : m_fbo(0), m_fboTexColorId(0), m_rbo(0), m_size(size)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());

    //Create a frame buffer
    glFuncs.glGenFramebuffers(1, &m_fbo);
    glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    //Create a texture for writting color
    glFuncs.glGenTextures(1, &m_fboTexColorId);
    glFuncs.glBindTexture(GL_TEXTURE_2D, m_fboTexColorId);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_size.width(), m_size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    //Attach the texture to the framebuffer - Set the texture as the color attachment 0
    glFuncs.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexColorId, 0);

    //Create a render buffer
    glFuncs.glGenRenderbuffers(1, &m_rbo);
    glFuncs.glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glFuncs.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size.width(), m_size.height());
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

FrameBufferObject::~FrameBufferObject()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glDeleteFramebuffers(1, &m_fbo);
    glFuncs.glDeleteTextures(1, &m_fboTexColorId);
    glFuncs.glDeleteRenderbuffers(1, &m_rbo);
}

void FrameBufferObject::bind()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferObject::release()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

MeshBufferObject::MeshBufferObject() : m_pBuffer(0), m_cBuffer(0), m_tBuffer(0), m_iBuffer(0), m_size(-1), m_indexedMesh(false)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glGenBuffers(1, &m_pBuffer);
    glFuncs.glGenBuffers(1, &m_cBuffer);
    glFuncs.glGenBuffers(1, &m_tBuffer);
    glFuncs.glGenBuffers(1, &m_iBuffer);
}

MeshBufferObject::MeshBufferObject( const std::string& objPath) : m_pBuffer(0), m_cBuffer(0), m_tBuffer(0), m_iBuffer(0), m_size(-1), m_indexedMesh(false)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glGenBuffers(1, &m_pBuffer);
    glFuncs.glGenBuffers(1, &m_cBuffer);
    glFuncs.glGenBuffers(1, &m_tBuffer);
    glFuncs.glGenBuffers(1, &m_iBuffer);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec4> colors;
    std::vector<unsigned int> indices;

    bool success = read_obj(objPath, positions, indices, normals, texCoords);
    if(!success)
    {
        qInfo() << " [MeshBufferObject] Failed to load " << objPath.c_str();
    }
    else
    {
        colors.resize(positions.size(), glm::vec4(1.0,1.0,1.0,1.0));
        loadPositions(positions);
        loadColors(colors);
        loadTexCoords(texCoords);
        loadIndices(indices);
        qInfo() << "[MeshBufferObject] Succeeded to load " << objPath.c_str();
        qInfo() << "[MeshBufferObject] #Vertices " << positions.size();
        qInfo() << "[MeshBufferObject] #Indices " << indices.size();
        qInfo() << "[MeshBufferObject] #Normals " << normals.size();
        qInfo() << "[MeshBufferObject] #TexCoords " << texCoords.size();
        qInfo() << "[MeshBufferObject] #Colors " << colors.size();
    }
}


MeshBufferObject::~MeshBufferObject()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glDeleteBuffers(1, &m_pBuffer);
    glFuncs.glDeleteBuffers(1, &m_cBuffer);
    glFuncs.glDeleteBuffers(1, &m_tBuffer);
    glFuncs.glDeleteBuffers(1, &m_iBuffer);
}

void MeshBufferObject::loadPositions(std::vector<glm::vec3>& data)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glFuncs.glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), data.data(), GL_STATIC_DRAW);
    if(!m_indexedMesh)
    {
        m_size = data.size();
    }
}

void MeshBufferObject::loadColors(std::vector<glm::vec4>& data)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
    glFuncs.glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec4), data.data(), GL_STATIC_DRAW);
    if(!m_indexedMesh)
    {
        m_size = data.size();
    }
}

void MeshBufferObject::loadTexCoords(std::vector<glm::vec2>& data)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer);
    glFuncs.glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec2), data.data(), GL_STATIC_DRAW);
    if(!m_indexedMesh)
    {
        m_size = data.size();
    }
}

void MeshBufferObject::loadIndices(std::vector<unsigned int> &data)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer);
    glFuncs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
    m_indexedMesh = true;
    m_size = data.size();
}

void MeshBufferObject::draw(GLint pLocation, GLint cLocation)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glEnableVertexAttribArray(pLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glFuncs.glVertexAttribPointer(pLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glFuncs.glEnableVertexAttribArray(cLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
    glFuncs.glVertexAttribPointer(cLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if(m_indexedMesh)
    {
        glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer);
        glFuncs.glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, (void*)0);
    }
    else
    {
        glFuncs.glDrawArrays(GL_TRIANGLES,0, m_size);
    }

    glFuncs.glDisableVertexAttribArray(pLocation);
    glFuncs.glDisableVertexAttribArray(cLocation);
}

void MeshBufferObject::draw(GLint pLocation, GLint cLocation, GLuint texId, GLint tSamplerLocation, GLint tLocation)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glEnableVertexAttribArray(pLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glFuncs.glVertexAttribPointer(pLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glFuncs.glEnableVertexAttribArray(cLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
    glFuncs.glVertexAttribPointer(cLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glFuncs.glActiveTexture(GL_TEXTURE0);
    glFuncs.glBindTexture(GL_TEXTURE_2D, texId);
    glFuncs.glUniform1i(tSamplerLocation, 0);
    glFuncs.glEnableVertexAttribArray(tLocation);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer);
    glFuncs.glVertexAttribPointer(tLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if(m_indexedMesh)
    {
        glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer);
        glFuncs.glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, (void*)0);
    }
    else
    {
        glFuncs.glDrawArrays(GL_TRIANGLES,0, m_size);
    }

    glFuncs.glBindTexture(GL_TEXTURE_2D, 0);
    glFuncs.glDisableVertexAttribArray(pLocation);
    glFuncs.glDisableVertexAttribArray(cLocation);
    glFuncs.glDisableVertexAttribArray(tLocation);
}

void unitSquare(std::vector<glm::vec3>& positions, std::vector<glm::vec4>& colors, std::vector<glm::vec2>& texCoords, std::vector<unsigned int> &indices)
{
    positions.clear();
    colors.clear();
    texCoords.clear();
    std::vector< glm::vec3 > samplePositions =
    {
        glm::vec3(-1.0,-1.0,0.0), glm::vec3(1.0,-1.0,0.0), glm::vec3(1.0,1.0,0.0), glm::vec3(-1.0,1.0,0.0)
    };
    positions = samplePositions;

    std::vector< glm::vec2 > sampleTexCoords =
    {
        glm::vec2(0.0,0.0), glm::vec2(1.0,0.0), glm::vec2(1.0,1.0), glm::vec2(0.0,1.0)
    };
    texCoords = sampleTexCoords;

    std::vector< glm::vec4 > sampleColors =
    {
        glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0)
    };
    colors = sampleColors;

    std::vector< unsigned int > sampleIndices =
    {
        0, 1, 2, 0, 2, 3
    };
    indices = sampleIndices;
}

void unitSquare(std::vector<glm::vec3>& positions, std::vector<glm::vec4>& colors, std::vector<glm::vec2>& texCoords)
{
    positions.clear();
    colors.clear();
    texCoords.clear();
    std::vector< glm::vec3 > samplePositions =
    {
        glm::vec3(-1.0,-1.0,0.0), glm::vec3(1.0,-1.0,0.0), glm::vec3(1.0,1.0,0.0),
        glm::vec3(-1.0,-1.0,0.0), glm::vec3(1.0,1.0,0.0), glm::vec3(-1.0,1.0,0.0)
    };
    positions = samplePositions;

    std::vector< glm::vec2 > sampleTexCoords =
    {
        glm::vec2(0.0,0.0), glm::vec2(1.0,0.0), glm::vec2(1.0,1.0),
        glm::vec2(0.0,0.0), glm::vec2(1.0,1.0), glm::vec2(0.0,1.0)
    };
    texCoords = sampleTexCoords;

    std::vector< glm::vec4 > sampleColors =
    {
        glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0),
        glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0)
    };
    colors = sampleColors;
}

TextureObject::TextureObject() : m_texId(-1)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glGenTextures(1, &m_texId);
}

TextureObject::~TextureObject()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glDeleteTextures(1, &m_texId);
}

void TextureObject::loadImage(QImage& image)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glBindTexture(GL_TEXTURE_2D, m_texId);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (const GLvoid*)image.bits());
    glFuncs.glBindTexture(GL_TEXTURE_2D, 0);
}

