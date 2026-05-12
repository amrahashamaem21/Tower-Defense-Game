#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.h"
#include "Wave.h"
#include "HUD.h"
#include "Tower.h"
#include "CannonTower.h"
#include "IceTower.h"
#include "CandyBlaster.h"
#include "SunflowerTower.h"
#include "BombTower.h"
class Game
{
public:
    static const int MAX_TOWERS = 40;
    static const int TOTAL_WAVES = 5;
    static const int NUM_LEVELS = 3;
    static const int NUM_CHARS = 5;
    static const int STATE_MENU = 0;
    static const int STATE_LEVEL_SELECT = 1;
    static const int STATE_PLAYING = 2;
    static const int STATE_WAVE_CLEAR = 3;
    static const int STATE_VICTORY = 4;
    static const int STATE_GAME_OVER = 5;
private:
    sf::RenderWindow window;
    sf::View         gameView;
    sf::Clock        clock;
    int              state;
    sf::Music        bgMusic;
    sf::SoundBuffer victorySoundBuffer;
    sf::Sound victorySound;
    sf::SoundBuffer defeatSoundBuffer;
    sf::Sound defeatSound;
    Map   map;
    Wave  wave;
    HUD   hud;
    int gold, lives, currentWave;
    int selectedLevel;
    int unlockedLevels;
    Tower* towers[MAX_TOWERS];
    int    towerCount;
    int    pendingTower;
    int    selectedTowerIndex;
    bool   showHover;
    int    hoverCol, hoverRow;
    float  waveClearTimer;
    sf::Font font;
    sf::RectangleShape hoverCell;
    // MENU
    sf::Texture menuBgTex;
    sf::Sprite  menuBgSprite;
    sf::Text    menuTitle, menuSub;
    sf::Texture playBtnTex;
    sf::Sprite  playBtnSprite;
    // LEVEL SELECT
    sf::Texture islandTex;
    sf::Sprite  islandSprite;
    sf::Text    levelSelectTitle;
    sf::CircleShape levelBubbleCircle[NUM_LEVELS];
    sf::Text        levelBubbleNumText[NUM_LEVELS];
    bool            levelBubbleLocked[NUM_LEVELS];
    float           levelBubbleX[NUM_LEVELS];
    float           levelBubbleY[NUM_LEVELS];
    sf::RectangleShape startBtn;
    sf::Text           startBtnText;
    // CHARACTER PANEL
    sf::RectangleShape charPanel;
    sf::Text           charPanelTitle;
    // Enemy cards - flat arrays
    sf::RectangleShape enemyCardBg[NUM_CHARS];
    sf::Texture        enemyCardTex[NUM_CHARS];
    sf::Sprite         enemyCardSpr[NUM_CHARS];
    sf::Text           enemyCardNameTxt[NUM_CHARS];
    sf::Text           enemyCardDescTxt[NUM_CHARS];
    // Tower cards - flat arrays
    sf::RectangleShape towerCardBg[NUM_CHARS];
    sf::Texture        towerCardTex[NUM_CHARS];
    sf::Sprite         towerCardSpr[NUM_CHARS];
    sf::Text           towerCardNameTxt[NUM_CHARS];
    sf::Text           towerCardDescTxt[NUM_CHARS];
    bool     showEnemyCards;
    sf::RectangleShape toggleBtn;
    sf::Text           toggleTxt;
    // DETAIL POPUP
    bool               showDetailPopup;
    int                detailIndex;
    bool               detailIsEnemy;
    sf::RectangleShape detailPanel;
    sf::Sprite         detailSprite;
    sf::Text           detailName;
    sf::Text           detailStat1;
    sf::Text           detailStat2;
    sf::Text           detailStat3;
    sf::Text           detailDesc;
    sf::Text           detailAbility;
    sf::CircleShape    detailCloseBtn;
    sf::Text           detailCloseTxt;
    sf::Texture        heartIconTex;
    sf::Texture        boltIconTex;
    sf::Texture        coinIconTex;
    sf::Sprite         heartIconSpr;
    sf::Sprite         boltIconSpr;
    sf::Sprite         coinIconSpr;
    sf::RectangleShape detailDivider1;
    sf::RectangleShape detailDivider2;
    // WAVE CLEAR
    sf::RectangleShape waveClearBg;
    sf::Text           waveClearText, waveClearSub;
    // KEY HINT
    sf::Text keyHint;
public:
    Game();
    ~Game();
    void run();
private:
    void loadAssets();
    void processEvents();
    void update(float dt);
    void render();
    void renderMenu();
    void renderLevelSelect();
    void renderPlaying();
    void renderWaveClear();
    void renderDetailPopup();
    void setupDetailPopup(int index, bool isEnemy);
    sf::Vector2f getScaledMouse(int rawX, int rawY);
    void handleMapClick(float mx, float my);
    void handleHUDClick(float mx, float my);
    void placeTower(int col, int row);
    void upgradeTower(int index);
    void towerAttack(float dt);
    void startNextWave();
    void checkGameState();
    void resetGame();
    void startLevel(int level);
};