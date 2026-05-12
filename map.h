#pragma once
#include <SFML/Graphics.hpp>
class Map
{
public:
    static const int COLS = 16;
    static const int ROWS = 11;
    static const int TILE_SIZE = 64;
    static const int MAP_WIDTH = COLS * TILE_SIZE;
    static const int MAP_HEIGHT = ROWS * TILE_SIZE;
    static const int TILE_GRASS = 0;
    static const int TILE_PATH = 1;
    static const int TILE_BLOCKED = 2;
    static const int TILE_BUSH = 3;
    static const int TILE_FLOWER = 4;
    static const int TILE_SAND = 5;
private:
    int grid[ROWS][COLS];
    sf::Texture grassTexture;
    sf::Texture pathTexture;
    sf::Texture pathCornerTexture;
    sf::Texture pathVerticalTexture;
    sf::Texture bushTexture;
    sf::Texture flowerTexture;
    sf::Texture sandTexture;
    sf::Sprite grassSprite;
    sf::Sprite pathSprite;
    sf::Sprite pathCornerSprite;
    sf::Sprite pathVerticalSprite;
    sf::Sprite bushSprite;
    sf::Sprite flowerSprite;
    sf::Sprite sandSprite;
    static const int MAX_DECO = 20;
    sf::Sprite decorationSprite[MAX_DECO];
    float      decorationX[MAX_DECO];
    float      decorationY[MAX_DECO];
    int        decoCount;
    sf::Texture flowerPinkTex;
    sf::Texture flowerWhiteTex;
    sf::Texture castleTexture;
    sf::Sprite  castleSprite;
    sf::Vector2f waypoints[20];
    int waypointCount;
    sf::Vector2f entryPoint;
    sf::Vector2f exitPoint;
    int pathDir[ROWS][COLS];
public:
    Map();
    void buildMap();
    void loadTextures();
    void render(sf::RenderWindow& window);
    bool isPlaceable(int col, int row) const;
    bool pixelToGrid(float mx, float my,int& col, int& row) const;
    sf::Vector2f gridToWorld(int col, int row) const;
    void setTile(int r, int c, int type)
    {
        grid[r][c] = type;
    }
    int getTile(int r, int c) const
    {
        return grid[r][c];
    }
    sf::Vector2f* getWaypoints()
    {
        return waypoints;
    }
    int getWaypointCount()
    {
        return waypointCount;
    }
    sf::Vector2f getEntryPoint()
    {
        return entryPoint;
    }
    sf::Vector2f getExitPoint()
    {
        return exitPoint;
    }
private:
    void loadTex(sf::Texture& tex,
        sf::Sprite& spr,
        const std::string& path);
    void addDecoration(sf::Texture& tex,float x,float y,float scale);
};