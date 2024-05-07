#include "actionField.hpp"
#include <sstream>
#include <algorithm>
#include <numeric>

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
                case ENEMY:{
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
                }case PLAYER:{
                    Bomberman bomber;
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
    S_str str = {toStringCoord( bmb.coordinate.x + moveX,
                                bmb.coordinate.y + moveY )};
    bool success = getIntersection(str, fieldPossible).size() == 1;
    if(success){
        bmb.coordinate.x += moveX;
        bmb.coordinate.y += moveY;
    }

    return success;
}

bool ActionField::placeBomb(Bomberman& bomberman){
    S_str str = {toStringCoord(bomberman.coordinate)};

    if(bomberman.bombCount < 1)
        return false;

    bool success = getIntersection(str, fieldBombs).size() == 1;

    if(success){
        Bomb bomb;
        bomb.coordinate = bomberman.coordinate;
        bomb.owner = &bomberman;
        bomb.power = bomberman.power;
        fieldBombs = getUnion(str, fieldBombs);
        bomberman.bombCount -= 1;
        bombs.insert( {toStringCoord(bomb.coordinate), bomb} );
    }

    return success;
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
            isPaused = true;
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

    for(it = fieldExplosion.begin(); it != fieldExplosion.end(); it++){
        coordinate = parse(it->first);
        render->draw(coordinate.x, coordinate.y, 1, 1, TEXTURE_BOMB_1);
    }
}

void ActionField::drawBombs(){
    std::map<str, Bomb>::iterator it;
    Coordinate coordinate;

    for(it = bombs.begin(); it != bombs.end(); it++){
        coordinate = parse(it->first);
        render->draw(coordinate.x, coordinate.y, 1, 1, TEXTURE_BOMB_1);
    }
}

void ActionField::drawPlayers(){
    std::vector<Bomberman>::iterator it;
    Coordinate coordinate;

    for(it = players.begin(); it != players.end(); it++){
        render->draw(it->coordinate.x, it->coordinate.y, 1, 1, TEXTURE_PLAYER_1);
    }

    for(it = enemies.begin(); it != enemies.end(); it++){
        render->draw(it->coordinate.x, it->coordinate.y, 1, 1, TEXTURE_ENEMY_1);
    }
}

void ActionField::draw(){
    render->setUniformFloat("screenRation", 6./8.);
    drawSets(fieldSolidWall, TEXTURE_SOLID_WALL);
    drawSets(fieldPossible, TEXTURE_AIR);
    drawSets(fieldWall, TEXTURE_WALL);
    drawPlayers();
    drawExplosion();
}


void ActionField::calculate(){
    // Calculation for bombs
    S_str::iterator itBomb;
    for(itBomb = fieldBombs.begin(); itBomb != fieldBombs.end(); itBomb++){
        Bomb* bomb;
        bomb = &bombs.at(*itBomb);
        bomb->ttl--;
        if(bomb->ttl <= 0){
            explode(bomb->coordinate, bomb->power);
            fieldBombs.erase(itBomb);
        }
        bomb->owner->bombCount++;
    }

    // Calculating for players and enemies collision
    std::vector<Bomberman>::iterator itBomberman;
    S_str explosion = getSetOfKeys(fieldExplosion);
    for(itBomberman = players.begin(); itBomberman != players.end(); itBomberman++){
        itBomberman->cooldownImmortality--;
        S_str playerCoord = {toStringCoord(itBomberman->coordinate)};
        if(getIntersection(explosion, playerCoord).size() == 1){
            if(itBomberman->cooldownImmortality == 0){
                itBomberman->lives--;
                itBomberman->cooldownImmortality = COOLDOWN_IMMORTALITY;
            }
        }
    }
    for(itBomberman = enemies.begin(); itBomberman != enemies.end(); itBomberman++){
        itBomberman->cooldownImmortality--;
        S_str playerCoord = {toStringCoord(itBomberman->coordinate)};
        if(getIntersection(explosion, playerCoord).size() == 1){
            if(itBomberman->cooldownImmortality == 0){
                itBomberman->lives--;
                itBomberman->cooldownImmortality = COOLDOWN_IMMORTALITY;
            }
        }
    }

    // Calculating for explosion
    std::map<str, int32_t>::iterator itExplosion;
    for(itExplosion = fieldExplosion.begin(); itExplosion != fieldExplosion.end(); itExplosion++){
        itExplosion->second--;
        if(itExplosion->second == 0){
            fieldExplosion.erase(itExplosion);
        }
    }
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

    insertExplosion(coordinateTo);

    Coordinate coordinateToNew = coordinateTo;
    int32_t dx = coordinateTo.x - coordinateFrom.x;
    int32_t dy = coordinateTo.y - coordinateFrom.y;
    coordinateToNew.x += dx;
    coordinateToNew.y += dy;

    explodeDirect(coordinateTo, coordinateToNew, power - 1);
}

S_str ActionField::cartesian_of(S_str& s1, S_str& s2){
    S_str::iterator it1;
    S_str::iterator it2;
    S_str res;
    for(it1 = s1.begin(); it1 != s1.end(); it1++){
        for(it2 = s1.begin(); it2 != s1.end(); it2++){
            res.insert(*it1 + 'x' + *it2);
        }
    }
    return res;
}

S_str ActionField::getUnion(S_str& s1, S_str& s2){
    S_str res = s1;
    res.insert(s2.begin(), s2.end());

    return res;
}

S_str ActionField::getIntersection(S_str& s1, S_str& s2){
    S_str res;
    std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(res, res.begin()));
    return res;
}

S_str ActionField::getRange(int32_t a, int32_t b){
    V_i32 range(b - a);
    S_str res;
    std::iota(range.begin(), range.end(), 0);
    V_i32::iterator it;
    for(it = range.begin(); it != range.end(); it++)
        res.insert(std::to_string(*it));

    return res;
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