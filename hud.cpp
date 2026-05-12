#include "HUD.h"
#include <iostream>
#include <string>

HUD::HUD(int* gold, int* lives, int* wave, int totalWaves)
    : gold_ref(gold), lives_ref(lives), waveRef(wave),
    totalWaves(totalWaves), selectedTower(NONE),
    showUpgradePopup(false), showVictoryPopup(false), showDefeatPopup(false),
    upgradeDmg(0), upgradeRange(0), upgradeRate(0), upgradeCost(0)
{
    loadAssets();
}

void HUD::loadAssets()
{
    font.loadFromFile("Fonts/Exo_2/static/Exo2-SemiBold.ttf");

    // ── TOP BAR ──
    topBar.setSize(sf::Vector2f(WIN_W, TOP_BAR_H));
    topBar.setPosition(0, 0);
    topBar.setFillColor(sf::Color(25, 15, 55, 245));
    topBar.setOutlineColor(sf::Color(100, 80, 200));
    topBar.setOutlineThickness(2.f);

    coinTex.loadFromFile("image/sprites2/ui_icons/icon_coin.png");
    coinSprite.setTexture(coinTex);
    coinSprite.setScale(0.3f, 0.3f);
    coinSprite.setPosition(15, 8);

    goldText.setFont(font); goldText.setCharacterSize(18);
    goldText.setFillColor(sf::Color(255, 215, 0));
    goldText.setPosition(58, 13);

    heartTex.loadFromFile("image/sprites2/ui_icons/icon_heart.png");
    heartSprite.setTexture(heartTex);
    heartSprite.setScale(0.25f, 0.25f);
    heartSprite.setPosition(140, 10);

    livesText.setFont(font); livesText.setCharacterSize(18);
    livesText.setFillColor(sf::Color(255, 100, 150));
    livesText.setPosition(183, 13);

    // Wave bar (center of top bar)
    waveBarBg.setSize(sf::Vector2f(320, 12));
    waveBarBg.setPosition(WIN_W / 2.f - 160, 19);
    waveBarBg.setFillColor(sf::Color(40, 25, 75));
    waveBarBg.setOutlineColor(sf::Color(130, 100, 220));
    waveBarBg.setOutlineThickness(1.5f);

    waveBarFill.setSize(sf::Vector2f(0, 12));
    waveBarFill.setPosition(WIN_W / 2.f - 160, 19);
    waveBarFill.setFillColor(sf::Color(80, 180, 255));

    waveText.setFont(font); waveText.setCharacterSize(12);
    waveText.setFillColor(sf::Color::White);
    waveText.setPosition(WIN_W / 2.f - 50, 6);

    // ── RIGHT PANEL ──
    rightPanel.setSize(sf::Vector2f(PANEL_W, WIN_H - TOP_BAR_H));
    rightPanel.setPosition(PANEL_X, TOP_BAR_H);
    rightPanel.setFillColor(sf::Color(22, 14, 48, 250));
    rightPanel.setOutlineColor(sf::Color(90, 70, 170));
    rightPanel.setOutlineThickness(2.f);

    panelTitle.setFont(font);
    panelTitle.setString("TOWERS");
    panelTitle.setCharacterSize(16);
    panelTitle.setFillColor(sf::Color(200, 170, 255));
    panelTitle.setPosition(PANEL_X + PANEL_W / 2.f - 35, TOP_BAR_H + 10);

    // Tower card data
    const char* names[NUM_TOWERS] = { "Cannon","Ice","Candy","Sunflower","Bomb" };
    const char* texPaths[NUM_TOWERS] = {
        "Sprites/towers/cannon_tower.png",
        "Sprites/towers/ice_tower.png",
        "Sprites/towers/candy_blaster.png",
        "Sprites/towers/sunflower.png",
        "Sprites/towers/bomb.png"
    };
    const char* stats[NUM_TOWERS] = {
        "DMG:50 RNG:130\nSLOW FIRE",
        "DMG:15 RNG:120\nSLOWS ENEMIES",
        "DMG:20 RNG:115\nRAPID FIRE",
        "DMG:12 RNG:140\nAREA DAMAGE",
        "DMG:80 RNG:110\nEXPLODES AREA"
    };
    const int costs[NUM_TOWERS] = { 100, 120, 150, 100, 200 };
    const bool locked[NUM_TOWERS] = { false,false,false,false,false };

    for (int i = 0; i < NUM_TOWERS; i++)
    {
        
        float cardY = TOP_BAR_H + 45.f + i * 118.f;

        cards[i].bg.setSize(sf::Vector2f(PANEL_W - 20, 108));
        cards[i].bg.setPosition(PANEL_X + 10, cardY);
        cards[i].bg.setFillColor(sf::Color(40, 28, 80));
        cards[i].bg.setOutlineColor(sf::Color(100, 80, 180));
        cards[i].bg.setOutlineThickness(1.5f);

        cards[i].tex.loadFromFile(texPaths[i]);
        cards[i].spr.setTexture(cards[i].tex);
        float scale = (i == 3) ? 0.13f : 0.29f;  // Sunflower smaller
        cards[i].spr.setScale(scale, scale);
        sf::FloatRect b = cards[i].spr.getLocalBounds();
        cards[i].spr.setOrigin(b.width / 2.f, b.height / 2.f);
        cards[i].spr.setPosition(PANEL_X + 55, cardY + 54);

        cards[i].nameTxt.setFont(font);
        cards[i].nameTxt.setString(names[i]);
        cards[i].nameTxt.setCharacterSize(13);
        cards[i].nameTxt.setFillColor(sf::Color(255, 220, 100));
        cards[i].nameTxt.setPosition(PANEL_X + 110, cardY + 10);

        cards[i].statsTxt.setFont(font);
        cards[i].statsTxt.setString(stats[i]);
        cards[i].statsTxt.setCharacterSize(10);
        cards[i].statsTxt.setFillColor(sf::Color(200, 200, 200));
        cards[i].statsTxt.setPosition(PANEL_X + 110, cardY + 32);

        cards[i].costTxt.setFont(font);
        cards[i].costTxt.setString("G " + std::to_string(costs[i]));
        cards[i].costTxt.setCharacterSize(12);
        cards[i].costTxt.setFillColor(sf::Color(255, 215, 0));
        cards[i].costTxt.setPosition(PANEL_X + 110, cardY + 78);

        cards[i].cost = costs[i];
        cards[i].selected = false;
        cards[i].locked = locked[i];
    }

    // Start Wave button
    startWaveBtn.setSize(sf::Vector2f(PANEL_W - 20, 48));
    startWaveBtn.setPosition(PANEL_X + 10, WIN_H - 58);
    startWaveBtn.setFillColor(sf::Color(40, 160, 40));
    startWaveBtn.setOutlineColor(sf::Color(80, 255, 80));
    startWaveBtn.setOutlineThickness(2.f);

    startWaveTxt.setFont(font);
    startWaveTxt.setString("START WAVE");
    startWaveTxt.setCharacterSize(14);
    startWaveTxt.setFillColor(sf::Color::White);
    sf::FloatRect swb = startWaveTxt.getLocalBounds();
    startWaveTxt.setPosition(PANEL_X + PANEL_W / 2.f - swb.width / 2.f, WIN_H - 46);

    // ── UPGRADE POPUP ──
    upgradePanel.setSize(sf::Vector2f(300, 220));
    upgradePanel.setPosition(362, 274);
    upgradePanel.setFillColor(sf::Color(35, 22, 70, 245));
    upgradePanel.setOutlineColor(sf::Color(150, 100, 255));
    upgradePanel.setOutlineThickness(3.f);

    upgradeTitleTxt.setFont(font); upgradeTitleTxt.setCharacterSize(15);
    upgradeTitleTxt.setFillColor(sf::Color(255, 220, 100));
    upgradeTitleTxt.setPosition(380, 284);

    upgradeDmgTxt.setFont(font); upgradeDmgTxt.setCharacterSize(12);
    upgradeDmgTxt.setFillColor(sf::Color::White);
    upgradeDmgTxt.setPosition(380, 315);

    upgradeRngTxt.setFont(font); upgradeRngTxt.setCharacterSize(12);
    upgradeRngTxt.setFillColor(sf::Color::White);
    upgradeRngTxt.setPosition(380, 340);

    upgradeRateTxt.setFont(font); upgradeRateTxt.setCharacterSize(12);
    upgradeRateTxt.setFillColor(sf::Color::White);
    upgradeRateTxt.setPosition(380, 365);

    upgradeBtn.setSize(sf::Vector2f(260, 45));
    upgradeBtn.setPosition(382, 430);
    upgradeBtn.setFillColor(sf::Color(50, 170, 50));
    upgradeBtn.setOutlineColor(sf::Color::White);
    upgradeBtn.setOutlineThickness(2.f);

    upgradeBtnTxt.setFont(font); upgradeBtnTxt.setCharacterSize(13);
    upgradeBtnTxt.setFillColor(sf::Color::White);
    upgradeBtnTxt.setPosition(420, 442);

    // ── VICTORY POPUP ──
    victoryPanel.setSize(sf::Vector2f(380, 300));
    victoryPanel.setPosition(WIN_W / 2.f - 190, WIN_H / 2.f - 170);
    victoryPanel.setFillColor(sf::Color(30, 130, 60, 245));
    victoryPanel.setOutlineColor(sf::Color(100, 255, 120));
    victoryPanel.setOutlineThickness(3.f);

    victoryTitle.setFont(font);
    victoryTitle.setString("VICTORY!");
    victoryTitle.setCharacterSize(42);
    victoryTitle.setFillColor(sf::Color::White);
    sf::FloatRect vt = victoryTitle.getLocalBounds();
    victoryTitle.setPosition(WIN_W / 2.f - vt.width / 2.f, WIN_H / 2.f - 160);

    for (int i = 0; i < 3; i++)
    {
        stars[i].setRadius(22.f);
        stars[i].setPointCount(5);
        stars[i].setFillColor(sf::Color(255, 215, 0));
        stars[i].setOutlineColor(sf::Color(200, 150, 0));
        stars[i].setOutlineThickness(2.f);
        stars[i].setPosition(WIN_W / 2.f - 75 + i * 60, WIN_H / 2.f - 90);
    }

    victoryGoldTxt.setFont(font); victoryGoldTxt.setCharacterSize(18);
    victoryGoldTxt.setFillColor(sf::Color(255, 215, 0));
    victoryGoldTxt.setPosition(WIN_W / 2.f - 80, WIN_H / 2.f - 20);

    victoryBtn.setSize(sf::Vector2f(260, 50));
    victoryBtn.setPosition(WIN_W / 2.f - 130, WIN_H / 2.f + 40);
    victoryBtn.setFillColor(sf::Color(40, 160, 40));
    victoryBtn.setOutlineColor(sf::Color::White);
    victoryBtn.setOutlineThickness(2.f);

    victoryBtnTxt.setFont(font); victoryBtnTxt.setCharacterSize(16);
    victoryBtnTxt.setFillColor(sf::Color::White);
    victoryBtnTxt.setString("CONTINUE");
    sf::FloatRect vb = victoryBtnTxt.getLocalBounds();
    victoryBtnTxt.setPosition(WIN_W / 2.f - vb.width / 2.f, WIN_H / 2.f + 52);

    // ── DEFEAT POPUP ──
    defeatPanel.setSize(sf::Vector2f(380, 320));
    defeatPanel.setPosition(WIN_W / 2.f - 190, WIN_H / 2.f - 190);
    defeatPanel.setFillColor(sf::Color(160, 30, 60, 245));
    defeatPanel.setOutlineColor(sf::Color(255, 100, 120));
    defeatPanel.setOutlineThickness(3.f);

    defeatTitle.setFont(font);
    defeatTitle.setString("DEFEAT");
    defeatTitle.setCharacterSize(42);
    defeatTitle.setFillColor(sf::Color::White);
    sf::FloatRect dt = defeatTitle.getLocalBounds();
    defeatTitle.setPosition(WIN_W / 2.f - dt.width / 2.f, WIN_H / 2.f - 180);

    ghostTex.loadFromFile("Sprites/enemies/floaty.png");
    ghostSprite.setTexture(ghostTex);
    ghostSprite.setScale(0.22f, 0.22f);
    sf::FloatRect gb = ghostSprite.getLocalBounds();
    ghostSprite.setOrigin(gb.width / 2.f, gb.height / 2.f);
    ghostSprite.setPosition(WIN_W / 2.f, WIN_H / 2.f - 70);

    defeatSubTxt.setFont(font); defeatSubTxt.setCharacterSize(14);
    defeatSubTxt.setFillColor(sf::Color::White);
    defeatSubTxt.setString("Better luck next time!");
    sf::FloatRect dsb = defeatSubTxt.getLocalBounds();
    defeatSubTxt.setPosition(WIN_W / 2.f - dsb.width / 2.f, WIN_H / 2.f + 10);

    retryBtn.setSize(sf::Vector2f(260, 50));
    retryBtn.setPosition(WIN_W / 2.f - 130, WIN_H / 2.f + 55);
    retryBtn.setFillColor(sf::Color(180, 40, 40));
    retryBtn.setOutlineColor(sf::Color::White);
    retryBtn.setOutlineThickness(2.f);

    retryBtnTxt.setFont(font); retryBtnTxt.setCharacterSize(16);
    retryBtnTxt.setFillColor(sf::Color::White);
    retryBtnTxt.setString("RETRY");
    sf::FloatRect rb = retryBtnTxt.getLocalBounds();
    retryBtnTxt.setPosition(WIN_W / 2.f - rb.width / 2.f, WIN_H / 2.f + 67);
}

void HUD::update()
{
    goldText.setString(std::to_string(*gold_ref));
    livesText.setString(std::to_string(*lives_ref));

    float ratio = (float)*waveRef / (float)totalWaves;
    waveBarFill.setSize(sf::Vector2f(320.f * ratio, 12));
    waveText.setString("WAVE " + std::to_string(*waveRef) + " / " + std::to_string(totalWaves));

    victoryGoldTxt.setString("Gold remaining: " + std::to_string(*gold_ref));

    if (showUpgradePopup)
    {
        upgradeDmgTxt.setString("Damage:    " + std::to_string((int)upgradeDmg));
        upgradeRngTxt.setString("Range:     " + std::to_string((int)upgradeRange));
        upgradeRateTxt.setString("Fire Rate: " + std::to_string((int)(upgradeRate * 10) / 10));
        upgradeBtnTxt.setString("UPGRADE  G" + std::to_string(upgradeCost));
    }

    // Grey out cards if not enough gold
    for (int i = 0; i < NUM_TOWERS; i++)
    {
        if (!cards[i].locked && *gold_ref < cards[i].cost)
            cards[i].bg.setFillColor(sf::Color(25, 18, 50));
        else if (cards[i].selected)
            cards[i].bg.setFillColor(sf::Color(80, 55, 150));
        else
            cards[i].bg.setFillColor(sf::Color(40, 28, 80));
    }
}

void HUD::render(sf::RenderWindow& window)
{
    // Top bar
    window.draw(topBar);
    window.draw(coinSprite);
    window.draw(goldText);
    window.draw(heartSprite);
    window.draw(livesText);
    window.draw(waveBarBg);
    window.draw(waveBarFill);
    window.draw(waveText);

    // Right panel
    window.draw(rightPanel);
    window.draw(panelTitle);

    for (int i = 0; i < NUM_TOWERS; i++)
    {
        window.draw(cards[i].bg);
        if (cards[i].locked)
        {
            sf::RectangleShape lockOverlay(sf::Vector2f(PANEL_W - 20, 108));
            lockOverlay.setPosition(cards[i].bg.getPosition());
            lockOverlay.setFillColor(sf::Color(0, 0, 0, 140));
            window.draw(lockOverlay);
            sf::Text lockTxt;
            lockTxt.setFont(font);
            lockTxt.setString("LOCKED");
            lockTxt.setCharacterSize(14);
            lockTxt.setFillColor(sf::Color(150, 150, 150));
            sf::FloatRect lb = lockTxt.getLocalBounds();
            lockTxt.setPosition(
                cards[i].bg.getPosition().x + (PANEL_W - 20) / 2.f - lb.width / 2.f,
                cards[i].bg.getPosition().y + 40);
            window.draw(lockTxt);
        }
        else
        {
            window.draw(cards[i].spr);
            window.draw(cards[i].nameTxt);
            window.draw(cards[i].statsTxt);
            window.draw(cards[i].costTxt);
        }
    }

    window.draw(startWaveBtn);
    window.draw(startWaveTxt);

    // Popups (drawn on top of everything)
    if (showUpgradePopup)
    {
        window.draw(upgradePanel);
        window.draw(upgradeTitleTxt);
        window.draw(upgradeDmgTxt);
        window.draw(upgradeRngTxt);
        window.draw(upgradeRateTxt);
        window.draw(upgradeBtn);
        window.draw(upgradeBtnTxt);
    }

    if (showVictoryPopup)
    {
        window.draw(victoryPanel);
        window.draw(victoryTitle);
        for (int i = 0; i < 3; i++) window.draw(stars[i]);
        window.draw(victoryGoldTxt);
        window.draw(victoryBtn);
        window.draw(victoryBtnTxt);
    }

    if (showDefeatPopup)
    {
        window.draw(defeatPanel);
        window.draw(defeatTitle);
        window.draw(ghostSprite);
        window.draw(defeatSubTxt);
        window.draw(retryBtn);
        window.draw(retryBtnTxt);
    }
}

int HUD::handleClick(float mx, float my)
{
    for (int i = 0; i < NUM_TOWERS; i++)
    {
        if (cards[i].locked) continue;
        if (cards[i].bg.getGlobalBounds().contains(mx, my))
        {
            for (int j = 0; j < NUM_TOWERS; j++) cards[j].selected = false;
            cards[i].selected = true;
            selectedTower = i;
            return i;
        }
    }
    return NONE;
}

bool HUD::isStartWaveClicked(float mx, float my)
{
    return startWaveBtn.getGlobalBounds().contains(mx, my);
}

bool HUD::isUpgradeClicked(float mx, float my)
{
    return showUpgradePopup && upgradeBtn.getGlobalBounds().contains(mx, my);
}

bool HUD::isContinueClicked(float mx, float my)
{
    return showVictoryPopup && victoryBtn.getGlobalBounds().contains(mx, my);
}

bool HUD::isRetryClicked(float mx, float my)
{
    return showDefeatPopup && retryBtn.getGlobalBounds().contains(mx, my);
}

void HUD::setUpgradeInfo(float dmg, float range, float rate, int cost)
{
    upgradeDmg = dmg;
    upgradeRange = range;
    upgradeRate = rate;
    upgradeCost = cost;
    upgradeTitleTxt.setString("UPGRADE TOWER");
}

void HUD::clearSelection()
{
    for (int i = 0; i < NUM_TOWERS; i++) cards[i].selected = false;
    selectedTower = NONE;
}