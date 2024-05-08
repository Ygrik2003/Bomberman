#include "game.hpp"
#include <algorithm>

const int32_t NUMBER_OF_KEYS = 256;

int32_t Game::init(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        successfulInitialization = false;
        return -1;
    }

    window = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 16*100, 16*40, SDL_WINDOW_OPENGL);
    if (!window) {
        successfulInitialization = false;
        return -1;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        successfulInitialization = false;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        successfulInitialization = false;
        return -1;
    }

    fileReader = new FileReader();

    // std::vector<std::string> texturePaths;
    // std::transform(Config::textureTypePaths.begin(), Config::textureTypePaths.end(), std::back_inserter(texturePaths),
    //                [](const auto& pair) { return pair.second; });

    pressedKeys = new int8_t[NUMBER_OF_KEYS];

    for(int32_t i = 0; i < NUMBER_OF_KEYS; i++){
        pressedKeys[i] = 0;
    }

    render = new Render(Config::textureTypePaths);
    render->initShader(Config::VERT_SHADER_PATH, Config::FRAG_SHADER_PATH);
    actionField = new ActionField(fileReader, render);

    successfulInitialization = true;
    return 0;
}

int32_t Game::start(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    while (!quit) {

        // int32_t widthScreen, heightScreen;
        // SDL_GetWindowSize(window, &widthScreen, &heightScreen);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        uint32_t start_time = SDL_GetTicks();
        eventHandler();

        SDL_Delay(DELAY_TIME);

        actionField->calculate();
        actionField->draw();

        SDL_GL_SwapWindow(window);
    }

    gameQuit();

    return 0;
}

void Game::eventHandler(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type){
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                pressedKeys[event.key.keysym.scancode] = 1;
                break;

            case SDL_KEYUP:
                pressedKeys[event.key.keysym.scancode] = 0;
                break;

            default:
                break;
        }
    }

    std::map<SDL_Scancode, Action>::const_iterator it;
    for(it = Config::keyEventToAction.begin(); it != Config::keyEventToAction.end(); it++){
        if(pressedKeys[it->first]){
            actionField->action(it->second);
        }
    }
}

void Game::gameQuit(){
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


Game::Game(){

}

Game::~Game(){
    if(successfulInitialization){
        delete render;
        delete fileReader;
        delete actionField;
        delete pressedKeys;
    }
}