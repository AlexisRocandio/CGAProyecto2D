#include "game_level.h"
#include <fstream>
#include <sstream>

// Función para cargar un nivel desde un archivo
void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
    // Limpia los datos antiguos
    this->Bricks.clear();
    
    // Carga los datos del archivo
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;

    if (fstream)
    {
        // Lee cada línea del archivo
        while (std::getline(fstream, line)) 
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;

            // Extrae los códigos de tiles de la línea
            while (sstream >> tileCode) 
                row.push_back(tileCode);

            // Añade la fila al conjunto de datos de tiles
            tileData.push_back(row);
        }

        // Si se han leído datos, inicializa el nivel
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

// Función para dibujar el nivel
void GameLevel::Draw(SpriteRenderer &renderer)
{
    // Dibuja cada ladrillo que no esté destruido
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

// Verifica si el nivel está completado
bool GameLevel::IsCompleted()
{
    // El nivel está completado si todos los ladrillos no sólidos están destruidos
    for (GameObject &tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

// Inicializa el nivel a partir de los datos de tiles
void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int height = tileData.size(); // Altura del nivel en tiles
    unsigned int width = tileData[0].size(); // Ancho del nivel en tiles

    // Calcula el ancho y alto de cada tile en unidades
    float unit_width = levelWidth / static_cast<float>(width), 
          unit_height = levelHeight / height; 	

    // Recorre cada tile y crea el objeto correspondiente
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            if (tileData[y][x] == 1) // Tile sólido
            {
                glm::vec2 pos(unit_width * x, unit_height * y); // Posición del tile
                glm::vec2 size(unit_width, unit_height); // Tamaño del tile
                GameObject obj(pos, size, ResourceManager::GetTexture("empty"), glm::vec3(1.0f, 1.0f, 1.0f)); // Crea el objeto
                obj.IsSolid = true; // Marca como sólido
                this->Bricks.push_back(obj); // Añade el objeto a la lista de ladrillos
            }
            else if (tileData[y][x] == 2) // Tile no sólido (tipo 2)
            {
                glm::vec3 color = glm::vec3(0.0f, 1.3f, 0.0f); // Color verde
                glm::vec2 pos(unit_width * x, unit_height * y); // Posición del tile
                glm::vec2 size(unit_width, unit_height); // Tamaño del tile
                this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("nave"), color)); // Crea y añade el objeto
            }
            else if (tileData[y][x] == 3) // Tile no sólido (tipo 3)
            {
                glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f); // Color púrpura
                glm::vec2 pos(unit_width * x, unit_height * y); // Posición del tile
                glm::vec2 size(unit_width, unit_height); // Tamaño del tile
                this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("nave"), color)); // Crea y añade el objeto
            }
            else if (tileData[y][x] > 3) // Otros tiles no sólidos
            {
                glm::vec3 color = glm::vec3(1.0f); // Color blanco por defecto

                // Asigna colores basados en el código del tile
                if (tileData[y][x] == 4)
                    color = glm::vec3(0.0f, 0.0f, 1.0f); // Color azul
                else if (tileData[y][x] == 5)
                    color = glm::vec3(0.0f, 1.5f, 1.5f); // Color cian

                glm::vec2 pos(unit_width * x, unit_height * y); // Posición del tile
                glm::vec2 size(unit_width, unit_height); // Tamaño del tile
                this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("nave"), color)); // Crea y añade el objeto
            }
        }
    }
}
