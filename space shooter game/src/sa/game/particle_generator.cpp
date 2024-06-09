#include "particle_generator.h"

// Constructor de la clase ParticleGenerator
ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    this->init(); // Inicializa los buffers y partículas
}

// Actualiza las partículas
void ParticleGenerator::Update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset)
{
    // Genera nuevas partículas
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
    }
    // Actualiza todas las partículas existentes
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt; // Reduce la vida de cada partícula
        if (p.Life > 0.0f) // Solo actualiza las partículas activas
        {	
            p.Position -= p.Velocity * dt; // Mueve la partícula
            p.Color.a -= dt * 2.5f; // Reduce la opacidad de la partícula
        }
    }
}

// Dibuja las partículas en pantalla
void ParticleGenerator::Draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Configura la función de mezcla
    this->shader.Use(); // Usa el shader para las partículas
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f) // Dibuja solo las partículas activas
        {
            this->shader.SetVector2f("offset", particle.Position); // Establece la posición de la partícula
            this->shader.SetVector4f("color", particle.Color); // Establece el color de la partícula
            this->texture.Bind(); // Vincula la textura de la partícula
            glBindVertexArray(this->VAO); // Vincula el VAO de la partícula
            glDrawArrays(GL_TRIANGLES, 0, 6); // Dibuja la partícula
            glBindVertexArray(0); // Desvincula el VAO
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Restaura la función de mezcla original
}

// Inicializa los datos de las partículas y el VAO/VBO
void ParticleGenerator::init()
{
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    }; 
    glGenVertexArrays(1, &this->VAO); // Genera el VAO
    glGenBuffers(1, &VBO); // Genera el VBO
    glBindVertexArray(this->VAO); // Vincula el VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vincula el VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW); // Carga los datos en el VBO
    glEnableVertexAttribArray(0); // Habilita el atributo del vértice
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // Especifica el formato de los datos de vértices
    glBindVertexArray(0); // Desvincula el VAO

    // Inicializa el vector de partículas
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// Encuentra la primera partícula no utilizada (muerta)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}

// Reaparece una partícula en una nueva posición y con nuevas propiedades
void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f; // Genera un valor aleatorio para variar la posición de la partícula
    float rColor = 0.5f + ((rand() % 100) / 100.0f); // Genera un valor aleatorio para el color
    particle.Position = object.Position + random + offset; // Calcula la nueva posición de la partícula
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f); // Establece el nuevo color de la partícula
    particle.Life = 1.0f; // Restablece la vida de la partícula
    particle.Velocity = object.Velocity * 0.1f; // Establece la velocidad de la partícula
}
