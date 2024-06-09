#include "post_processor.h"

#include <iostream>

// Constructor de la clase PostProcessor
PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height)
    : PostProcessingShader(shader), Texture(), Width(width), Height(height), Confuse(false), Chaos(false), Shake(false)
{
    // Genera el framebuffer multisample (MSFBO)
    glGenFramebuffers(1, &this->MSFBO);
    // Genera el framebuffer normal (FBO)
    glGenFramebuffers(1, &this->FBO);
    // Genera el renderbuffer (RBO)
    glGenRenderbuffers(1, &this->RBO);

    // Configura el framebuffer multisample
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height); 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); 
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Error al inicializar MSFBO" << std::endl;

    // Configura el framebuffer normal
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->Texture.Generate(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0); 
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Error al inicializar FBO" << std::endl;

    // Desvincula el framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Inicializa los datos de renderizado
    this->initRenderData();

    // Configura los uniformes del shader de post-procesamiento
    this->PostProcessingShader.SetInteger("scene", 0, true);

    // Define los offsets para el kernel de convolución
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // arriba-izquierda
        {  0.0f,    offset  },  // arriba-centro
        {  offset,  offset  },  // arriba-derecha
        { -offset,  0.0f    },  // centro-izquierda
        {  0.0f,    0.0f    },  // centro-centro
        {  offset,  0.0f    },  // centro-derecha
        { -offset, -offset  },  // abajo-izquierda
        {  0.0f,   -offset  },  // abajo-centro
        {  offset, -offset  }   // abajo-derecha    
    };
    glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (float*)offsets);

    // Define el kernel para detección de bordes
    int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);

    // Define el kernel para el desenfoque (blur)
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

// Inicia el proceso de renderizado
void PostProcessor::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Finaliza el proceso de renderizado y copia los datos del framebuffer multisample al normal
void PostProcessor::EndRender()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

// Renderiza la textura con los efectos de post-procesamiento
void PostProcessor::Render(float time)
{
    this->PostProcessingShader.Use();
    this->PostProcessingShader.SetFloat("time", time);
    this->PostProcessingShader.SetInteger("confuse", this->Confuse);
    this->PostProcessingShader.SetInteger("chaos", this->Chaos);
    this->PostProcessingShader.SetInteger("shake", this->Shake);
    this->PostProcessingShader.SetInteger("parallax", this->Parallax);
    this->PostProcessingShader.SetInteger("parallaxslow", this->ParallaxSlow);
    glActiveTexture(GL_TEXTURE0);
    this->Texture.Bind();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// Inicializa los datos de renderizado para el quad de pantalla completa
void PostProcessor::initRenderData()
{
    unsigned int VBO;
    // Define los vértices para un quad que cubre toda la pantalla
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    // Enlaza los datos de los vértices al buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configura los atributos de los vértices
    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
