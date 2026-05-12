#pragma once
#include <SFML/Graphics.hpp>

class HUD
{
public:
    static const int NONE = -1;
    static const int CANNON = 0;
    static const int ICE = 1;
    static const int CANDY = 2;
    static const int SUNFLOWER = 3;
    static const int BOMB = 4;
    static const int NUM_TOWERS = 5;

    static const int TOP_BAR_H = 50;
    static const int PANEL_X = 1024;
    static const int PANEL_W = 342;
    static const int WIN_W = 1366;
    static const int WIN_H = 768;

private:
    sf::Font font;

    // TOP BAR
    sf::RectangleShape topBar;
    sf::Texture coinTex, heartTex;
    sf::Sprite  coinSprite, heartSprite;
    sf::Text    goldText, livesText;
    sf::RectangleShape waveBarBg, waveBarFill;
    sf::Text    waveText;

    // RIGHT PANEL
    sf::RectangleShape rightPanel;
    sf::Text           panelTitle;

    struct TowerCard {
        sf::RectangleShape bg;
        sf::Texture        tex;
        sf::Sprite         spr;
        sf::Text           nameTxt;
        sf::Text           statsTxt;
        sf::Text           costTxt;
        int                cost;
        bool               selected;
        bool               locked;
    };
    TowerCard cards[NUM_TOWERS];

    sf::RectangleShape startWaveBtn;
    sf::Text           startWaveTxt;

    // UPGRADE POPUP
    bool               showUpgradePopup;
    sf::RectangleShape upgradePanel;
    sf::Text           upgradeTitleTxt;
    sf::Text           upgradeDmgTxt;
    sf::Text           upgradeRngTxt;
    sf::Text           upgradeRateTxt;
    sf::RectangleShape upgradeBtn;
    sf::Text           upgradeBtnTxt;
    float upgradeDmg, upgradeRange, upgradeRate;
    int   upgradeCost;

    // VICTORY POPUP
    bool               showVictoryPopup;
    sf::RectangleShape victoryPanel;
    sf::Text           victoryTitle;
    sf::Text           victoryGoldTxt;
    sf::RectangleShape victoryBtn;
    sf::Text           victoryBtnTxt;
    sf::CircleShape    stars[3];

    // DEFEAT POPUP
    bool               showDefeatPopup;
    sf::RectangleShape defeatPanel;
    sf::Text           defeatTitle;
    sf::Text           defeatSubTxt;
    sf::Texture        ghostTex;
    sf::Sprite         ghostSprite;
    sf::RectangleShape retryBtn;
    sf::Text           retryBtnTxt;

    int* gold_ref;
    int* lives_ref;
    int* waveRef;
    int  totalWaves;
    int  selectedTower;

public:
    HUD(int* gold, int* lives, int* wave, int totalWaves = 5);
    void loadAssets();
    void update();
    void render(sf::RenderWindow& window);

    int  handleClick(float mx, float my);
    bool isStartWaveClicked(float mx, float my);
    bool isUpgradeClicked(float mx, float my);
    bool isContinueClicked(float mx, float my);
    bool isRetryClicked(float mx, float my);

    void setShowUpgrade(bool b) { showUpgradePopup = b; }
    void setShowVictory(bool b) { showVictoryPopup = b; }
    void setShowDefeat(bool b) { showDefeatPopup = b; }
    void setUpgradeInfo(float dmg, float range, float rate, int cost);
    void clearSelection();
    int  getSelectedTower() const { return selectedTower; }
};