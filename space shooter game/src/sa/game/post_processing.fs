#version 330 core

// Entrada y salida del shader
in vec2 TexCoords; // Coordenadas de textura de entrada
out vec4 color; // Color de salida

// Uniformes para pasar datos al shader
uniform sampler2D scene; // Textura de la escena renderizada
uniform vec2 offsets[9]; // Desplazamientos para efectos de procesamiento
uniform int edge_kernel[9]; // Kernel para detección de bordes
uniform float blur_kernel[9]; // Kernel para desenfoque

// Flags para activar diferentes efectos
uniform bool chaos; // Efecto de caos (detección de bordes)
uniform bool confuse; // Efecto de confusión (inversión de colores)
uniform bool shake; // Efecto de sacudida (desenfoque)
uniform bool parallax; // Efecto de parallax
uniform bool parallaxslow; // Efecto de parallax lento

void main()
{
    color = vec4(0.0f); // Inicializa el color a negro

    vec3 sample[9]; // Array para almacenar muestras de textura
    if(chaos || shake) // Si caos o sacudida están activados
    {
        for(int i = 0; i < 9; i++)
        {
            sample[i] = vec3(texture(scene, TexCoords.st + offsets[i])); // Muestra las texturas desplazadas
        }
    }

    if(chaos) // Si el modo caos está activado
    {           
        for(int i = 0; i < 9; i++)
        {
            color += vec4(sample[i] * edge_kernel[i], 0.0f); // Aplica el kernel de detección de bordes
        }
        color.a = 1.0f; // Establece la opacidad a 1
    }
    else if(confuse) // Si el modo confusión está activado
    {
        color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0); // Invierte los colores
    }
    else if(parallax) // Si el efecto de parallax está activado
    {
        color = texture(scene, TexCoords); // Usa la textura original
    }
    else if(parallaxslow) // Si el efecto de parallax lento está activado
    {
        color = texture(scene, TexCoords); // Usa la textura original
    }
    else if(shake) // Si el efecto de sacudida está activado
    {
        for(int i = 0; i < 9; i++)
        {
            color += vec4(sample[i] * blur_kernel[i], 0.0f); // Aplica el kernel de desenfoque
        }
        color.a = 1.0f; // Establece la opacidad a 1
    }
    else // Si no hay efectos activados
    {
        color = texture(scene, TexCoords); // Usa la textura original
    }
}
