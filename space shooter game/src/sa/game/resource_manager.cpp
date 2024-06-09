#include "resource_manager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "stb_image.h"

// Inicializa los mapas estáticos para almacenar los shaders y texturas
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

// Carga un shader desde archivos y lo almacena en el mapa Shaders
Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile); // Carga el shader y lo almacena con el nombre dado
    return Shaders[name];
}

// Obtiene un shader del mapa Shaders usando su nombre
Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

// Carga una textura desde un archivo y la almacena en el mapa Textures
Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha); // Carga la textura y la almacena con el nombre dado
    return Textures[name];
}

// Obtiene una textura del mapa Textures usando su nombre
Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

// Limpia todos los shaders y texturas cargados
void ResourceManager::Clear()
{
    // Elimina todos los programas de shaders
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // Elimina todas las texturas
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

// Función auxiliar para cargar un shader desde archivos
Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Abre los archivos de los shaders
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Lee el contenido de los archivos
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // Si hay un archivo de shader geométrico, también lo lee
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    // Compila los shaders
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

// Función auxiliar para cargar una textura desde un archivo
Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    Texture2D texture;
    // Configura el formato de la textura
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // Carga la imagen usando stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // Genera la textura
    texture.Generate(width, height, data);
    stbi_image_free(data); // Libera la memoria de la imagen
    return texture;
}
