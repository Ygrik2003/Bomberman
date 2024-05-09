#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <chrono>


const int32_t EXPLOSION_STAGES = 15;
const int32_t COOLDOWN_IMMORTALITY = 25;
const double TIME_MULTIPLIER = 1.;  // [] = 1 / s

typedef std::string str;
typedef std::set<std::string> S_str;
typedef std::set<int32_t> S_i32;
typedef std::vector<std::string> V_str;
typedef std::vector<int32_t> V_i32;

struct Coordinate{
    int32_t x;
    int32_t y;
};

enum Side{
    SIDE_PLAYER_1,
    SIDE_PLAYER_2,
    SIDE_ENEMY
};

struct Bomberman{
    glm::vec2 direction;
    Coordinate coordinate;
    int32_t cooldownImmortality = 3;
    int32_t lives = 3;
    int32_t bombCount = 3;
    int32_t power = 1;
    Side side;
    std::chrono::steady_clock::time_point timeInit = std::chrono::steady_clock::now();
};

struct Bomb{
    Coordinate coordinate;
    int32_t ttl = 5;
    int32_t power;
    Bomberman* owner;
    std::chrono::steady_clock::time_point timeInit = std::chrono::steady_clock::now();
};

enum Action{
    UP_1,
    DOWN_1,
    RIGHT_1,
    LEFT_1,
    PLACE_BOMB,
    PAUSE
};

enum GameState{
    PAUSED,
    FINISHED,
    PLAYING
};

enum CellType{
    SOLID_WALL,
    WALL,
    PLAYER,
    AIR,
    ENEMY
};

enum TextureType{
    TEXTURE_ERROR,
    TEXTURE_SOLID_WALL,
    TEXTURE_WALL,
    TEXTURE_PLAYER_1,
    TEXTURE_PLAYER_2,
    TEXTURE_AIR,
    TEXTURE_BOMB_1,
    TEXTURE_BOMB_2,
    TEXTURE_BOMB_3,
    TEXTURE_ENEMY_1,
    TEXTURE_ENEMY_2,
    TEXTURE_EXPLOSION_1,
    TEXTURE_EXPLOSION_2,
    TEXTURE_EXPLOSION_3,
    TEXTURE_EXPLOSION_4,
    TEXTURE_EXPLOSION_5,
};

namespace Config{
    const str FILE_PREFIX = "./res/";
    // const str FILE_PREFIX = "./../../res/";

    const str FIELD_PATH = FILE_PREFIX + "config/gameField.txt";
    const str FRAG_SHADER_PATH = FILE_PREFIX + "shaders/simpleShader.frag";
    const str VERT_SHADER_PATH = FILE_PREFIX + "shaders/simpleShader.vert";

    const static std::map<SDL_Scancode, Action> keyEventToAction = {
        {SDL_SCANCODE_W, UP_1},
        {SDL_SCANCODE_A, LEFT_1},
        {SDL_SCANCODE_S, DOWN_1},
        {SDL_SCANCODE_D, RIGHT_1},
        {SDL_SCANCODE_RETURN, PLACE_BOMB},
        {SDL_SCANCODE_ESCAPE, PAUSE}
    };

    const static std::map<int8_t, CellType> charToCell = {
        {'#', SOLID_WALL},
        {'-', AIR},
        {'=', WALL},
        {'P', PLAYER},
        {'p', PLAYER},
        {'E', ENEMY},
        {'e', ENEMY}
    };

    const static std::map<int32_t, str> textureTypePaths = {
        {TEXTURE_ERROR, FILE_PREFIX + "images/Error.png"},
        {TEXTURE_SOLID_WALL, FILE_PREFIX + "images/Block1.png"},
        {TEXTURE_WALL, FILE_PREFIX + "images/Block2.png"},
        {TEXTURE_PLAYER_1, FILE_PREFIX + "images/Kurt1.png"},
        {TEXTURE_PLAYER_2, FILE_PREFIX + "images/Kurt2.png"},
        {TEXTURE_AIR, FILE_PREFIX + "images/BlockAsphalt.png"},
        {TEXTURE_BOMB_1, FILE_PREFIX + "images/Bomb1.png"},
        {TEXTURE_BOMB_2, FILE_PREFIX + "images/Bomb2.png"},
        {TEXTURE_BOMB_3, FILE_PREFIX + "images/Bomb3.png"},
        {TEXTURE_ENEMY_1, FILE_PREFIX + "images/Enemy1.png"},
        {TEXTURE_ENEMY_2, FILE_PREFIX + "images/Enemy2.png"},
        {TEXTURE_EXPLOSION_1, FILE_PREFIX + "images/Boom1.png"},
        {TEXTURE_EXPLOSION_2, FILE_PREFIX + "images/Boom2.png"},
        {TEXTURE_EXPLOSION_3, FILE_PREFIX + "images/Boom3.png"},
        {TEXTURE_EXPLOSION_4, FILE_PREFIX + "images/Boom4.png"},
        {TEXTURE_EXPLOSION_5, FILE_PREFIX + "images/Boom5.png"}
    };
}