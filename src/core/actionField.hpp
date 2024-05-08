#pragma once
#include <string>
#include <vector>
#include <map>
#include "../config.hpp"
#include "../engine/fileReader.hpp"
#include "../engine/render.hpp"

class ActionField{
    private:
        FileReader* fileReaderPtr;
        Render* render;
        bool isPaused = false;

        std::vector<Bomberman> players;
        std::vector<Bomberman> enemies;
        std::map<str, Bomb> bombs;
        std::map<str, int32_t> fieldExplosion;
        S_str fieldPossible;
        S_str fieldSolidWall;
        S_str fieldWall;
    private:
        S_str cartesian_of(S_str& s1, S_str& s2);
        S_str getUnion(S_str& s1, S_str& s2);
        S_str getIntersection(S_str& s1, S_str& s2);
        S_str getRange(int32_t a, int32_t b);
        
        template<typename V>
        S_str getSetOfKeys(std::map<str, V> map);

        Coordinate parse(str coord);
        str toStringCoord(int32_t a, int32_t b);
        str toStringCoord(Coordinate coordinate);

        std::vector<Coordinate> getNeighbors(Coordinate coordinate);
        void explode(Coordinate coordinate, int32_t power);
        void explodeDirect(Coordinate coordinateFrom, Coordinate coordinateTo, int32_t power);
        void insertExplosion(Coordinate coordinate);
        void generateFields(V_str& rawField);
        bool tryMove(Bomberman& bmb, int32_t moveX, int32_t moveY);
        bool placeBomb(Bomberman& bmb);

        void drawPlayers();
        void drawExplosion();
        void drawBombs();
        void drawSets(S_str setStr, TextureType textureType);
    public:
        ActionField(FileReader* fileReaderPtr, Render* render);
        ~ActionField();
        void action(Action action);
        void draw();
        void calculate();
};