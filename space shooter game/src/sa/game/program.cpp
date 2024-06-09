#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game.h"
#include "resource_manager.h"
#include <iostream>

// Callback para cambiar el tamaño del framebuffer
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Callback para eventos de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Definición de las dimensiones de la pantalla
const unsigned int SCREEN_WIDTH = 1800;
const unsigned int SCREEN_HEIGHT = 1000;

// Inicialización del juego con las dimensiones de pantalla
Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
    // Inicializa GLFW
    glfwInit();
    // Especifica la versión de OpenGL que se va a utilizar (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Selecciona el perfil de OpenGL (core profile)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para compatibilidad con MacOS
#endif
    glfwWindowHint(GLFW_RESIZABLE, false); // Desactiva la capacidad de cambiar el tamaño de la ventana

    // Crea la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter", nullptr, nullptr);
    // Establece el contexto de OpenGL
    glfwMakeContextCurrent(window);

    // Carga todos los punteros de función de OpenGL utilizando GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configura los callbacks para el teclado y el tamaño del framebuffer
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Define la ventana de visualización
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Habilita la mezcla de colores (transparencia)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Inicializa los recursos y el estado del juego
    Breakout.Init();

    // Variables para controlar el tiempo entre frames
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Bucle principal del juego
    while (!glfwWindowShouldClose(window))
    {
        // Calcula el tiempo transcurrido entre frames
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Procesa eventos
        glfwPollEvents();

        // Procesa la entrada del jugador
        Breakout.ProcessInput(deltaTime);

        // Actualiza el estado del juego
        Breakout.Update(deltaTime);

        // Renderiza la escena
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        // Intercambia los buffers de la ventana
        glfwSwapBuffers(window);
    }

    // Limpia los recursos utilizados
    ResourceManager::Clear();

    // Termina GLFW
    glfwTerminate();
    return 0;
}

// Callback para gestionar la entrada del teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Cierra la ventana si se presiona la tecla ESCAPE
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = true; // Marca la tecla como presionada
        else if (action == GLFW_RELEASE)
        {
            Breakout.Keys[key] = false; // Marca la tecla como liberada
            Breakout.KeysProcessed[key] = false; // Resetea el estado de la tecla
        }
    }
}

// Callback para gestionar el cambio de tamaño del framebuffer
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // Ajusta la vista a las nuevas dimensiones
}
