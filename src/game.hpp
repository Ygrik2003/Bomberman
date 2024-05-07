#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <glad/glad.h>
#include "config.hpp"
#include "core/actionField.hpp"

// int32_t widthScreen, heightScreen;

class Game{
    private:
        // SDL variables
        SDL_Window* window;
        SDL_GLContext context;

        FileReader* fileReader;
        ActionField* actionField;
        Render* render;

        int32_t DELAY_TIME = 1000 / 60;
        int8_t* pressedKeys;
        bool successfulInitialization;
        bool quit = false;
    private:
        void gameQuit();
        void eventHandler();
    public:
        Game();
        ~Game();
        int32_t init();
        int32_t start();
};