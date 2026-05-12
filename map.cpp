#include "Map.h"
#include <iostream>

Map::Map() : waypointCount(0), decoCount(0)
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
        {
            grid[r][c] = TILE_GRASS;
            pathDir[r][c] = 0;
        }
    buildMap();
    loadTextures();
}

void Map::loadTex(sf::Texture& tex, sf::Sprite& spr, const std::string& path)
{
    if (!tex.loadFromFile(path))
        std::cerr << "Map: failed to load " << path << "\n";
    else
    {
        spr.setTexture(tex);
        spr.setScale(
            (float)TILE_SIZE / tex.getSize().x,
            (float)TILE_SIZE / tex.getSize().y);
    }
}

void Map::addDecoration(sf::Texture& tex, float x, float y, float scale)
{
    if (decoCount >= MAX_DECO) return;
    decorationSprite[decoCount].setTexture(tex);
    decorationSprite[decoCount].setScale(scale, scale);
    decorationSprite[decoCount].setPosition(x, y);
    decorationX[decoCount] = x;
    decorationY[decoCount] = y;
    decoCount++;
}

void Map::loadTextures()
{
    loadTex(grassTexture, grassSprite, "image/sprites2/tiles/grass_plain.png");
    loadTex(pathTexture, pathSprite, "Sprites/path/path_horizontal.png");
    loadTex(pathCornerTexture, pathCornerSprite, "Sprites/path/path_corner.png");
    loadTex(pathVerticalTexture, pathVerticalSprite, "Sprites/path/path_vertical.png");
    loadTex(bushTexture, bushSprite, "Sprites/title/bush_flowers.png");
    loadTex(flowerTexture, flowerSprite, "image/sprites2/tiles/flower_tile.png");
    loadTex(sandTexture, sandSprite, "image/sprites2/tiles/sand_tile.png");

    // Castle at exit
    if (!castleTexture.loadFromFile("Sprites/decorations/castle_exit.png"))
        std::cerr << "Castle texture failed\n";
    else
    {
        castleSprite.setTexture(castleTexture);
        castleSprite.setScale(0.35f, 0.35f);
        sf::FloatRect b = castleSprite.getLocalBounds();
        castleSprite.setOrigin(b.width / 2.f, b.height / 1.f);
        castleSprite.setPosition(exitPoint.x, exitPoint.y - 20.f + 50.f);
    }

    // Decorations textures
    flowerPinkTex.loadFromFile("Sprites/decorations/flower_pink.png");
    flowerWhiteTex.loadFromFile("Sprites/decorations/flower_white.png");

    // Add flowers scattered on grass tiles
    // These are hardcoded positions on grass areas
    addDecoration(flowerPinkTex, 80, 70, 0.18f);
    addDecoration(flowerWhiteTex, 200, 60, 0.18f);
    addDecoration(flowerPinkTex, 500, 230, 0.18f);
    addDecoration(flowerWhiteTex, 650, 210, 0.18f);
    addDecoration(flowerPinkTex, 100, 470, 0.18f);
    addDecoration(flowerWhiteTex, 750, 470, 0.18f);
    addDecoration(flowerPinkTex, 820, 350, 0.18f);
    addDecoration(flowerWhiteTex, 920, 230, 0.18f);
    addDecoration(flowerPinkTex, 50, 610, 0.18f);
    addDecoration(flowerWhiteTex, 950, 610, 0.18f);
}

void Map::buildMap()
{
    // Initialize all grass
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            grid[r][c] = TILE_GRASS;

    // ── S-shaped path ──
    // Row 1: cols 0-5  (horizontal)
    for (int c = 0; c <= 5; c++) { grid[1][c] = TILE_PATH; pathDir[1][c] = 0; }
    // Turn at col5 row1→4 (vertical)
    for (int r = 1; r <= 4; r++) { grid[r][5] = TILE_PATH; pathDir[r][5] = 1; }
    // Row 4: cols 5-11 (horizontal)
    for (int c = 5; c <= 11; c++) { grid[4][c] = TILE_PATH; pathDir[4][c] = 0; }
    // Turn at col11 row4→8 (vertical)
    for (int r = 4; r <= 8; r++) { grid[r][11] = TILE_PATH; pathDir[r][11] = 1; }
    // Row 8: cols 11-15 (horizontal to exit)
    for (int c = 11; c <= 15; c++) { grid[8][c] = TILE_PATH; pathDir[8][c] = 0; }

    // ── Decorative tiles scattered on grass ──
    // Bush tiles
    grid[0][2] = TILE_BUSH;
    grid[0][8] = TILE_BUSH;
    grid[0][13] = TILE_BUSH;
    grid[3][0] = TILE_BUSH;
    grid[3][8] = TILE_BUSH;
    grid[6][0] = TILE_BUSH;
    grid[6][14] = TILE_BUSH;
    grid[9][2] = TILE_BUSH;
    grid[10][7] = TILE_BUSH;
    grid[10][13] = TILE_BUSH;

    // Flower tiles
    grid[0][5] = TILE_FLOWER;
    grid[2][9] = TILE_FLOWER;
    grid[5][2] = TILE_FLOWER;
    grid[7][7] = TILE_FLOWER;
    grid[9][14] = TILE_FLOWER;
    grid[10][3] = TILE_FLOWER;

    // Sand tiles
    grid[0][11] = TILE_SAND;
    grid[3][14] = TILE_SAND;
    grid[7][2] = TILE_SAND;
    grid[10][10] = TILE_SAND;

    // ── Waypoints ──
   // FIND your waypoints section and REPLACE entirely:
    waypointCount = 6;
    waypoints[0] = sf::Vector2f(0 * TILE_SIZE + TILE_SIZE / 2.f, 1 * TILE_SIZE + TILE_SIZE / 2.f);
    waypoints[1] = sf::Vector2f(5 * TILE_SIZE + TILE_SIZE / 2.f, 1 * TILE_SIZE + TILE_SIZE / 2.f);
    waypoints[2] = sf::Vector2f(5 * TILE_SIZE + TILE_SIZE / 2.f, 4 * TILE_SIZE + TILE_SIZE / 2.f);
    waypoints[3] = sf::Vector2f(11 * TILE_SIZE + TILE_SIZE / 2.f, 4 * TILE_SIZE + TILE_SIZE / 2.f);
    waypoints[4] = sf::Vector2f(11 * TILE_SIZE + TILE_SIZE / 2.f, 8 * TILE_SIZE + TILE_SIZE / 2.f);
    waypoints[5] = sf::Vector2f(15 * TILE_SIZE + TILE_SIZE / 2.f, 8 * TILE_SIZE + TILE_SIZE / 2.f);

    entryPoint = waypoints[0];
    exitPoint = waypoints[waypointCount - 1];
}

void Map::render(sf::RenderWindow & window)
{
    const float Y_OFFSET = 50.f;  // TOP_BAR_H
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            float px = c * TILE_SIZE;
            float py = r * TILE_SIZE + Y_OFFSET;
            int   t = grid[r][c];

            if (t == TILE_PATH)
            {
                // Choose correct path sprite based on direction
                int dir = pathDir[r][c];
                if (dir == 1)
                {
                    pathVerticalSprite.setPosition(px, py);
                    window.draw(pathVerticalSprite);
                }
                else if (dir == 2 || dir == 3 || dir == 4 || dir == 5)
                {
                    pathCornerSprite.setPosition(px, py);
                    window.draw(pathCornerSprite);
                }
                else
                {
                    pathSprite.setPosition(px, py);
                    window.draw(pathSprite);
                }
            }
            else if (t == TILE_BUSH)
            {
                // Draw grass underneath first
                grassSprite.setPosition(px, py);
                window.draw(grassSprite);
                bushSprite.setPosition(px, py);
                window.draw(bushSprite);
            }
            else if (t == TILE_FLOWER)
            {
                grassSprite.setPosition(px, py);
                window.draw(grassSprite);
                flowerSprite.setPosition(px, py);
                window.draw(flowerSprite);
            }
            else if (t == TILE_SAND)
            {
                sandSprite.setPosition(px, py);
                window.draw(sandSprite);
            }
            else
            {
                grassSprite.setPosition(px, py);
                window.draw(grassSprite);
            }
        }
    }

    // Draw small flower decorations
    for (int i = 0; i < decoCount; i++)
        window.draw(decorationSprite[i]);

    // Draw castle at exit
    window.draw(castleSprite);
}

bool Map::isPlaceable(int col, int row) const
{
    if (col < 0 || col >= COLS || row < 0 || row >= ROWS) return false;
    return grid[row][col] == TILE_GRASS;
}

bool Map::pixelToGrid(float mx, float my, int& col, int& row) const
{
    col = (int)(mx / TILE_SIZE);
    row = (int)(my / TILE_SIZE);
    return (col >= 0 && col < COLS && row >= 0 && row < ROWS);
}

sf::Vector2f Map::gridToWorld(int col, int row) const
{
    return sf::Vector2f(
        col * TILE_SIZE + TILE_SIZE / 2.f,
        row * TILE_SIZE + TILE_SIZE / 2.f);
}