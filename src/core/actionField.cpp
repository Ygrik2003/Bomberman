#include "actionField.hpp"
#include <sstream>
#include <algorithm>
#include <numeric>

#define DEBUG(c) std::cout << c << std::endl;

ActionField::ActionField(FileReader* fileReaderPtr, Render* render){
    this->fileReaderPtr = fileReaderPtr;
    this->render = render;

    V_str rawField = fileReaderPtr->getLines(Config::FIELD_PATH);

    generateFields(rawField);
}

ActionField::~ActionField(){

}

void ActionField::generateFields(V_str& rawField){
    for(size_t i = 0; i < rawField.size(); i++){
        for (size_t j = 0; j < rawField[i].size(); j++){
            int8_t s = rawField[i][j];
            CellType ct = Config::charToCell.at(s);

            str coord = toStringCoord(j, i);

            switch (ct){
                case SOLID_WALL:
                    fieldSolidWall.insert(coord);
                    break;
                case WALL:
                    fieldWall.insert(coord);
                    break;
                case AIR:
                    fieldPossible.insert(coord);
                    break;
                case PLAYER:{
                    Bomberman bomber;
                    bomber.coordinate.x = i;
                    bomber.coordinate.y = i;
                    if(enemies.size() == 0){
                        bomber.side = Side::SIDE_PLAYER_1;
                        players.push_back(bomber);
                    } else if(enemies.size() == 1){
                        bomber.side = Side::SIDE_PLAYER_2;
                        players.push_back(bomber);
                    } else{
                        bomber.side = Side::SIDE_ENEMY;
                        enemies.push_back(bomber);
                    }
                    fieldPossible.insert(coord);
                    break;
                }case ENEMY:{
                    Bomberman bomber;
                    bomber.coordinate.x = i;
                    bomber.coordinate.y = i;
                    bomber.side = Side::SIDE_ENEMY;
                    enemies.push_back(bomber);
                    fieldPossible.insert(coord);
                    break;
                }default:
                    break;
            }
        }
    }
}

// True if moved successfully
bool ActionField::tryMove(Bomberman& bmb, int32_t moveX, int32_t moveY){
    if(getDeltaTime(bmb.timeInit) * TIME_MULTIPLIER >= 1){
        bmb.direction = glm::vec2(moveX, moveY);
        S_str str = {toStringCoord( bmb.coordinate.x + moveX,
                                    bmb.coordinate.y + moveY )};
        bool success = math::getIntersection(str, fieldPossible).size() == 1;
        if(success){
            bmb.coordinate.x += moveX;
            bmb.coordinate.y += moveY;
            bmb.timeInit = std::chrono::steady_clock::now();
        }
        return success;
    } else 
        return false;
}

bool ActionField::placeBomb(Bomberman& bomberman){
    if(bomberman.bombCount < 1)
        return false;

    auto it = bombs.find(toStringCoord(bomberman.coordinate));
    if (it != bombs.end()) {
        return false;
    } else {
        Bomb bomb;
        bomb.coordinate = bomberman.coordinate;
        bomb.owner = &bomberman;
        bomb.power = bomberman.power;
        bomberman.bombCount -= 1;
        bombs.insert( {toStringCoord(bomb.coordinate), bomb} );
        return true;
    }
}


void ActionField::action(Action action){
    switch (action){
        case UP_1:{
            bool succesful = tryMove(players[0], 0, -1);
            break;
        }case DOWN_1:{
            bool succesful = tryMove(players[0], 0, 1);
            break;
        }case RIGHT_1:{
            bool succesful = tryMove(players[0], 1, 0);
            break;
        }case LEFT_1:{
            bool succesful = tryMove(players[0], -1, 0);
            break;
        }case PLACE_BOMB:{
            placeBomb(players[0]);
            break;
        }case PAUSE:{
            gameState = GameState::FINISHED;
            break;
        }default:
            break;
    }
}

void ActionField::drawSets(S_str setStr, TextureType textureType){
    S_str::iterator it;
    Coordinate coordinate;

    for(it = setStr.begin(); it != setStr.end(); it++){
        coordinate = parse(*it);

        render->draw(coordinate.x, coordinate.y, 1, 1, textureType);
    }
}

void ActionField::drawExplosion(){
    std::map<str, int32_t>::iterator it;
    Coordinate coordinate;

    TextureType tt;

    for(it = fieldExplosion.begin(); it != fieldExplosion.end(); it++){
        coordinate = parse(it->first);
        render->draw(coordinate.x, coordinate.y, 1, 1, TEXTURE_EXPLOSION_1);
    }
}

void ActionField::drawBombs(){
    std::map<str, Bomb>::iterator it;
    Coordinate coordinate;

    for(it = bombs.begin(); it != bombs.end(); it++){
        coordinate = parse(it->first);

        TextureType tt;
        switch (it->second.ttl){
            case 5:
            case 4:
                tt = TextureType::TEXTURE_BOMB_1;
                break;
            case 3:
            case 2:
                tt = TextureType::TEXTURE_BOMB_2;
                break;
            case 1:
            case 0:
                tt = TextureType::TEXTURE_BOMB_3;
                break;
            default:
                break;
        }
        render->draw(coordinate.x, coordinate.y, 1, 1, tt);
    }
}

void ActionField::drawPlayers(){
    std::vector<Bomberman>::iterator it;
    Coordinate coordinate;

    for(it = players.begin(); it != players.end(); it++){
        render->setUniformFloat("dt", getDeltaTime(it->timeInit));
        render->setUniformVec2("direction", it->direction);
        render->draw(it->coordinate.x, it->coordinate.y, 1, 1, TEXTURE_PLAYER_1);
        render->setUniformFloat("dt", 0);
        render->setUniformVec2("direction", glm::vec2(0, 0));

    }

    for(it = enemies.begin(); it != enemies.end(); it++){
        render->setUniformFloat("dt", getDeltaTime(it->timeInit));
        render->setUniformVec2("direction", it->direction);
        render->draw(it->coordinate.x, it->coordinate.y, 1, 1, TEXTURE_ENEMY_1);
        render->setUniformFloat("dt", 0);
        render->setUniformVec2("direction", glm::vec2(0, 0));
    }
}

void ActionField::draw(){
    render->setUniformFloat("screenRatio", 4./10.);
    drawSets(fieldSolidWall, TEXTURE_SOLID_WALL);
    drawSets(fieldPossible, TEXTURE_AIR);
    drawSets(fieldWall, TEXTURE_WALL);
    drawBombs();
    drawPlayers();
    drawExplosion();
}


void ActionField::calculate(){
    if(gameState == GameState::PAUSED)
        return;

    if(players.size() == 0 || enemies.size() == 0){
        gameState = GameState::FINISHED;
    }

    // Calculation for bombs
    std::map<str, Bomb>::iterator itBomb;
    for(itBomb = bombs.begin(); itBomb != bombs.end();){
        Bomb* bomb;
        bomb = &itBomb->second;
        if(getDeltaTime(bomb->timeInit) * TIME_MULTIPLIER >= 1){
            bomb->ttl--;
            bomb->timeInit = std::chrono::steady_clock::now();
        }

        if(bomb->ttl <= 0){
            explode(bomb->coordinate, bomb->power);
            bomb->owner->bombCount++;
            itBomb = bombs.erase(itBomb);
            // bombs.erase(itBomb);
        } else{
            itBomb++;
        }
    }

    // Calculating for players and enemies collision
    std::vector<Bomberman>::iterator itBomberman;
    S_str explosion = getSetOfKeys(fieldExplosion);

    for(itBomberman = players.begin(); itBomberman != players.end(); ){
        itBomberman->cooldownImmortality--;
        S_str playerCoord = {toStringCoord(itBomberman->coordinate)};
        if(math::getIntersection(explosion, playerCoord).size() == 1){
            if(itBomberman->cooldownImmortality <= 0){
                itBomberman->lives--;
                itBomberman->cooldownImmortality = COOLDOWN_IMMORTALITY;
            }
        }
        if(itBomberman->lives == 0){
            itBomberman = players.erase(itBomberman);
        } else {
            itBomberman++;
        }
    }
    for(itBomberman = enemies.begin(); itBomberman != enemies.end();){
        itBomberman->cooldownImmortality--;
        S_str playerCoord = {toStringCoord(itBomberman->coordinate)};
        if(math::getIntersection(explosion, playerCoord).size() == 1){
            if(itBomberman->cooldownImmortality <= 0){
                itBomberman->lives--;
                itBomberman->cooldownImmortality = COOLDOWN_IMMORTALITY;
            }
        }
        if(itBomberman->lives == 0){
            itBomberman = enemies.erase(itBomberman);
        } else {
            itBomberman++;
        }
    }

    // Calculating for explosion
    std::map<str, int32_t>::iterator itExplosion;
    for(itExplosion = fieldExplosion.begin(); itExplosion != fieldExplosion.end();){
        itExplosion->second--;
        if(itExplosion->second == 0){
            itExplosion = fieldExplosion.erase(itExplosion);
        } else {
            itExplosion++;
        }
    }
}

GameState ActionField::getState(){
    return gameState;
}

std::vector<Coordinate> ActionField::getNeighbors(Coordinate coordinate){
    std::vector<Coordinate> setCoordinates;

    Coordinate coord = coordinate;
    coord.x++;
    setCoordinates.push_back(coord);

    coord = coordinate;
    coord.x--;
    setCoordinates.push_back(coord);

    coord = coordinate;
    coord.y++;
    setCoordinates.push_back(coord);

    coord = coordinate;
    coord.y--;
    setCoordinates.push_back(coord);
    return setCoordinates;
}

void ActionField::insertExplosion(Coordinate coordinate){
    fieldExplosion.insert({toStringCoord(coordinate), EXPLOSION_STAGES});
}

void ActionField::explode(Coordinate coordinate, int32_t power){
    S_str strCoord = {toStringCoord(coordinate)};
    std::vector<Coordinate>setNeighbors = getNeighbors(coordinate);
    std::vector<Coordinate>::iterator setIt;

    insertExplosion(coordinate);

    if(power > 0){
        for(setIt = setNeighbors.begin(); setIt != setNeighbors.end(); setIt++){
            explodeDirect(coordinate, *setIt, power);
        }
    }
}

// Fireball places on coordinateTo
void ActionField::explodeDirect(Coordinate coordinateFrom, Coordinate coordinateTo, int32_t power){
    if(power == 0)
        return;

    S_str strCoord = {toStringCoord(coordinateTo)};

    if(math::getIntersection(strCoord, fieldSolidWall).size() == 1)
        return;

    insertExplosion(coordinateTo);

    Coordinate coordinateToNew = coordinateTo;
    int32_t dx = coordinateTo.x - coordinateFrom.x;
    int32_t dy = coordinateTo.y - coordinateFrom.y;
    coordinateToNew.x += dx;
    coordinateToNew.y += dy;

    if(math::getIntersection(strCoord, fieldWall).size() == 1){
        fieldWall = math::getDifference(fieldWall, strCoord);
        fieldPossible = math::getUnion(fieldPossible, strCoord);
        return;
    }
    explodeDirect(coordinateTo, coordinateToNew, power - 1);
}

template<typename V>
S_str ActionField::getSetOfKeys(std::map<str, V> map){
    typename std::map<str, V>::iterator it;
    S_str setString;
    for(it = map.begin(); it != map.end(); it++){
        setString.insert(it->first);
    }
    return setString;
}

Coordinate ActionField::parse(str coord){
    Coordinate coordinate;
    std::stringstream ss(coord);
    std::string token;

    if (std::getline(ss, token, 'x')) {
        coordinate.x = std::stoi(token);
    }

    if (std::getline(ss, token)) {
        coordinate.y = std::stoi(token);
    }

    return coordinate;
}

str ActionField::toStringCoord(int32_t a, int32_t b){
    return std::to_string(a) + "x" + std::to_string(b);
}

str ActionField::toStringCoord(Coordinate coordinate){
    return std::to_string(coordinate.x) + "x" + std::to_string(coordinate.y);
}

double ActionField::getDeltaTime(std::chrono::steady_clock::time_point time){
    std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - time);
    double t = timeSpan.count();
    return t > 1 ? 1.0 : t;
}