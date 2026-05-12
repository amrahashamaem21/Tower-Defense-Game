#include "Game.h"
#include <iostream>
#include <string>

// popup center constants
static const float POP_X = 683.f;
static const float POP_Y = 384.f;  // 768/2
static const float POP_W = 340.f;
static const float POP_H = 440.f;

Game::Game()
    : window(sf::VideoMode(1366, 768), "Tower Defense",
        sf::Style::Close | sf::Style::Resize),
    wave(&gold, &lives),
    hud(&gold, &lives, &currentWave, TOTAL_WAVES),
    gold(150), lives(10), currentWave(1),
    selectedLevel(1), unlockedLevels(1),
    towerCount(0), pendingTower(HUD::NONE),
    state(STATE_MENU),
    showHover(false), hoverCol(0), hoverRow(0),
    selectedTowerIndex(-1), waveClearTimer(0.f),
    showDetailPopup(false), detailIndex(0), detailIsEnemy(true),
    showEnemyCards(true)
{
    window.setFramerateLimit(60);
    gameView.reset(sf::FloatRect(0, 0, 1366, 768));
    window.setView(gameView);

    for (int i = 0; i < MAX_TOWERS; i++) towers[i] = nullptr;

    loadAssets();

    wave.setWaypoints(map.getWaypoints(), map.getWaypointCount(),
        map.getEntryPoint(), map.getExitPoint());
    wave.setupWave(1,selectedLevel);

    // Hover cell
    hoverCell.setSize(sf::Vector2f(Map::TILE_SIZE, Map::TILE_SIZE));
    hoverCell.setFillColor(sf::Color(255, 255, 100, 60));
    hoverCell.setOutlineColor(sf::Color(255, 255, 0, 200));
    hoverCell.setOutlineThickness(2.f);

    // Wave clear
    waveClearBg.setSize(sf::Vector2f(1366, 768));
    waveClearBg.setFillColor(sf::Color(0, 0, 0, 150));

    waveClearText.setFont(font);
    waveClearText.setString("WAVE COMPLETE!");
    waveClearText.setCharacterSize(52);
    waveClearText.setFillColor(sf::Color(100, 255, 100));
    sf::FloatRect wb = waveClearText.getLocalBounds();
    waveClearText.setPosition(683 - wb.width / 2.f, 300);

    waveClearSub.setFont(font);
    waveClearSub.setString("Next wave incoming... (click to skip)");
    waveClearSub.setCharacterSize(20);
    waveClearSub.setFillColor(sf::Color::White);
    sf::FloatRect wsb = waveClearSub.getLocalBounds();
    waveClearSub.setPosition(683 - wsb.width / 2.f, 375);

    // Key hint
    keyHint.setFont(font);
    keyHint.setCharacterSize(11);
    keyHint.setFillColor(sf::Color(160, 160, 160));
    keyHint.setString("ESC: back to menu  |  Click tower card then click grass to place");
    keyHint.setPosition(10, 752);
}

Game::~Game()
{
    for (int i = 0; i < MAX_TOWERS; i++) delete towers[i];
}

sf::Vector2f Game::getScaledMouse(int rawX, int rawY)
{
    sf::Vector2u winSize = window.getSize();
    float sx = 1366.f / winSize.x;
    float sy = 768.f / winSize.y;
    return sf::Vector2f(rawX * sx, rawY * sy);
}

void Game::loadAssets()
{
    font.loadFromFile("Fonts/Exo_2/static/Exo2-SemiBold.ttf");

    // MENU
    menuBgTex.loadFromFile("image/sprites2/ui_panels/sky_background.png");
    menuBgSprite.setTexture(menuBgTex);
    float scaleX = 1366.f / menuBgTex.getSize().x;
    float scaleY = 768.f / menuBgTex.getSize().y;
    float scale = std::max(scaleX, scaleY);
    menuBgSprite.setScale(scale, scale);
    menuBgSprite.setPosition(
        (1366.f - menuBgTex.getSize().x * scale) / 2.f,
        (768.f - menuBgTex.getSize().y * scale) / 2.f);

    menuTitle.setFont(font);
    menuTitle.setString("TOWER DEFENSE");
    menuTitle.setCharacterSize(68);
    menuTitle.setFillColor(sf::Color(255, 220, 50));
    menuTitle.setOutlineColor(sf::Color(100, 60, 0));
    menuTitle.setOutlineThickness(4.f);
    sf::FloatRect mt = menuTitle.getLocalBounds();
    menuTitle.setPosition(683 - mt.width / 2.f, 150);

    menuSub.setFont(font);
    menuSub.setString("Defend your kingdom!");
    menuSub.setCharacterSize(24);
    menuSub.setFillColor(sf::Color::White);
    sf::FloatRect ms = menuSub.getLocalBounds();
    menuSub.setPosition(683 - ms.width / 2.f, 248);

    playBtnTex.loadFromFile("image/sprites2/ui_buttons/btn_play_large.png");
    playBtnSprite.setTexture(playBtnTex);
    playBtnSprite.setScale(0.55f, 0.55f);
    sf::FloatRect pb = playBtnSprite.getLocalBounds();
    playBtnSprite.setOrigin(pb.width / 2.f, pb.height / 2.f);
    playBtnSprite.setPosition(683, 370);

    // LEVEL SELECT
    islandTex.loadFromFile("image/sprites2/ui_panels/map_new.png");
    islandSprite.setTexture(islandTex);
    islandSprite.setScale(1.6f, 1.6f);
    islandSprite.setPosition(30, 120);

    levelSelectTitle.setFont(font);
    levelSelectTitle.setString("SELECT LEVEL");
    levelSelectTitle.setCharacterSize(38);
    levelSelectTitle.setFillColor(sf::Color(255, 220, 50));
    levelSelectTitle.setOutlineColor(sf::Color(80, 50, 0));
    levelSelectTitle.setOutlineThickness(2.f);
    sf::FloatRect lst = levelSelectTitle.getLocalBounds();
    levelSelectTitle.setPosition(683 - lst.width / 2.f, 65);

    // Level bubbles
    float bx[3] = { 190, 330, 430 };
    float by[3] = { 450, 310, 420 };
    sf::Color bc[3] = {
        sf::Color(240, 80, 130),
        sf::Color(255, 165, 0),
        sf::Color(120, 120, 120)
    };
    for (int i = 0; i < NUM_LEVELS; i++)
    {
        levelBubbles[i].circle.setRadius(38.f);
        levelBubbles[i].circle.setOrigin(38.f, 38.f);
        levelBubbles[i].circle.setFillColor(bc[i]);
        levelBubbles[i].circle.setOutlineColor(sf::Color::White);
        levelBubbles[i].circle.setOutlineThickness(3.f);
        levelBubbles[i].circle.setPosition(bx[i], by[i]);

        levelBubbles[i].numText.setFont(font);
        levelBubbles[i].numText.setString(std::to_string(i + 1));
        levelBubbles[i].numText.setCharacterSize(26);
        levelBubbles[i].numText.setFillColor(sf::Color::White);
        sf::FloatRect nb = levelBubbles[i].numText.getLocalBounds();
        levelBubbles[i].numText.setPosition(
            bx[i] - nb.width / 2.f - 2, by[i] - nb.height / 2.f - 8);

        levelBubbles[i].locked = (i >= unlockedLevels);
        levelBubbles[i].x = bx[i];
        levelBubbles[i].y = by[i];
    }

    // CHARACTER PANEL
    charPanel.setSize(sf::Vector2f(680, 540));
    charPanel.setPosition(640, 120);
    charPanel.setFillColor(sf::Color(20, 28, 58, 210));
    charPanel.setOutlineColor(sf::Color(120, 160, 220, 180));
    charPanel.setOutlineThickness(2.f);

    charPanelTitle.setFont(font);
    charPanelTitle.setCharacterSize(20);
    charPanelTitle.setFillColor(sf::Color(220, 240, 255));
    charPanelTitle.setPosition(660, 128);

    toggleBtn.setSize(sf::Vector2f(160, 34));
    toggleBtn.setPosition(1140, 125);
    toggleBtn.setFillColor(sf::Color(255, 200, 80));
    toggleBtn.setOutlineColor(sf::Color(200, 150, 30));
    toggleBtn.setOutlineThickness(2.f);

    toggleTxt.setFont(font);
    toggleTxt.setCharacterSize(12);
    toggleTxt.setFillColor(sf::Color(60, 35, 0));
    toggleTxt.setPosition(1152, 133);

    // ENEMY CARDS
    const char* enemyNames[5] = { "SLIME","BEEBEE","ROCKO","FLOATY","JELLY" };
    const char* enemyDescs[5] = {
        "HP:100  SPD:80\nBasic enemy.\nClick for details!",
        "HP:60   SPD:160\nVery fast!\nClick for details!",
        "HP:400  SPD:40\nSlow tank.\nClick for details!",
        "HP:120  SPD:120\nFlies straight!\nClick for details!",
        "HP:150  SPD:90\nSplits on death!\nClick for details!"
    };
    const char* enemyTexPaths[5] = {
        "Sprites/enemies/slime.png",
        "Sprites/enemies/beebee.png",
        "Sprites/enemies/rocko.png",
        "Sprites/enemies/floaty.png",
        "Sprites/enemies/jelly.png"
    };
    sf::Color enemyBgColors[5] = {
        sf::Color(80,  180, 220, 200),
        sf::Color(255, 200, 60,  200),
        sf::Color(140, 140, 150, 200),
        sf::Color(180, 120, 220, 200),
        sf::Color(240, 100, 140, 200),
    };
    sf::Color enemyOutlineColors[5] = {
        sf::Color(40,  140, 190),
        sf::Color(200, 150, 20),
        sf::Color(100, 100, 110),
        sf::Color(130, 70,  180),
        sf::Color(200, 50,  90),
    };

    for (int i = 0; i < NUM_CHARS; i++)
    {
        float cx = 655.f + (i % 3) * 218.f;
        float cy = 168.f + (i / 3) * 230.f;

        enemyCards[i].bg.setSize(sf::Vector2f(205, 215));
        enemyCards[i].bg.setPosition(cx, cy);
        enemyCards[i].bg.setFillColor(enemyBgColors[i]);
        enemyCards[i].bg.setOutlineColor(enemyOutlineColors[i]);
        enemyCards[i].bg.setOutlineThickness(2.5f);

        enemyCards[i].tex.loadFromFile(enemyTexPaths[i]);
        enemyCards[i].spr.setTexture(enemyCards[i].tex);
        // Uniform size for all enemy sprites
        float targetSize = 85.f;
        float sScale = targetSize / std::max(
            (float)enemyCards[i].tex.getSize().x,
            (float)enemyCards[i].tex.getSize().y);
        enemyCards[i].spr.setScale(sScale, sScale);
        sf::FloatRect eb = enemyCards[i].spr.getLocalBounds();
        enemyCards[i].spr.setOrigin(eb.width / 2.f, eb.height / 2.f);
        enemyCards[i].spr.setPosition(cx + 102, cy + 88);

        enemyCards[i].nameTxt.setFont(font);
        enemyCards[i].nameTxt.setString(enemyNames[i]);
        enemyCards[i].nameTxt.setCharacterSize(14);
        enemyCards[i].nameTxt.setFillColor(sf::Color(20, 20, 40));
        enemyCards[i].nameTxt.setPosition(cx + 8, cy + 8);

        enemyCards[i].descTxt.setFont(font);
        enemyCards[i].descTxt.setString(enemyDescs[i]);
        enemyCards[i].descTxt.setCharacterSize(10);
        enemyCards[i].descTxt.setFillColor(sf::Color(30, 30, 60));
        enemyCards[i].descTxt.setPosition(cx + 8, cy + 148);
    }

    // TOWER CARDS
    const char* towerNames[5] = { "CANNON","ICE","CANDY","SUNFLWR","BOMB" };
    const char* towerDescs[5] = {
        "DMG:50  RNG:130\nHigh damage.\nClick for details!",
        "DMG:15  RNG:120\nSlows enemies.\nClick for details!",
        "DMG:20  RNG:115\nRapid fire!\nClick for details!",
        "DMG:12  RNG:140\nHits ALL in range!\nClick for details!",
        "DMG:80  RNG:110\nExplosion area!\nClick for details!"
    };
    const char* towerTexPaths[5] = {
        "Sprites/towers/cannon_tower.png",
        "Sprites/towers/ice_tower.png",
        "Sprites/towers/candy_blaster.png",
        "Sprites/towers/sunflower.png",
        "Sprites/towers/bomb.png"
    };
    sf::Color towerBgColors[5] = {
        sf::Color(220, 160, 60,  200),
        sf::Color(100, 200, 240, 200),
        sf::Color(240, 100, 160, 200),
        sf::Color(100, 200, 100, 200),
        sf::Color(80,  80,  90,  200),
    };
    sf::Color towerOutlineColors[5] = {
        sf::Color(180, 110, 20),
        sf::Color(40,  150, 200),
        sf::Color(190, 40,  110),
        sf::Color(40,  150, 40),
        sf::Color(140, 140, 160),
    };

    for (int i = 0; i < NUM_CHARS; i++)
    {
        float cx = 655.f + (i % 3) * 218.f;
        float cy = 168.f + (i / 3) * 230.f;

        towerCards[i].bg.setSize(sf::Vector2f(205, 215));
        towerCards[i].bg.setPosition(cx, cy);
        towerCards[i].bg.setFillColor(towerBgColors[i]);
        towerCards[i].bg.setOutlineColor(towerOutlineColors[i]);
        towerCards[i].bg.setOutlineThickness(2.5f);

        towerCards[i].tex.loadFromFile(towerTexPaths[i]);
        towerCards[i].spr.setTexture(towerCards[i].tex);
        float targetSize = 85.f;
        float sScale = targetSize / std::max(
            (float)towerCards[i].tex.getSize().x,
            (float)towerCards[i].tex.getSize().y);
        towerCards[i].spr.setScale(sScale, sScale);
        sf::FloatRect tb = towerCards[i].spr.getLocalBounds();
        towerCards[i].spr.setOrigin(tb.width / 2.f, tb.height / 2.f);
        towerCards[i].spr.setPosition(cx + 102, cy + 88);

        towerCards[i].nameTxt.setFont(font);
        towerCards[i].nameTxt.setString(towerNames[i]);
        towerCards[i].nameTxt.setCharacterSize(14);
        towerCards[i].nameTxt.setFillColor(sf::Color(20, 20, 40));
        towerCards[i].nameTxt.setPosition(cx + 8, cy + 8);

        towerCards[i].descTxt.setFont(font);
        towerCards[i].descTxt.setString(towerDescs[i]);
        towerCards[i].descTxt.setCharacterSize(10);
        towerCards[i].descTxt.setFillColor(sf::Color(30, 30, 60));
        towerCards[i].descTxt.setPosition(cx + 8, cy + 148);
    }

    // START BUTTON
    startBtn.setSize(sf::Vector2f(230, 58));
    startBtn.setPosition(190, 590);
    startBtn.setFillColor(sf::Color(110, 50, 180));
    startBtn.setOutlineColor(sf::Color(180, 130, 255));
    startBtn.setOutlineThickness(3.f);

    startBtnText.setFont(font);
    startBtnText.setString("START");
    startBtnText.setCharacterSize(26);
    startBtnText.setFillColor(sf::Color::White);
    sf::FloatRect sbt = startBtnText.getLocalBounds();
    startBtnText.setPosition(305 - sbt.width / 2.f, 602);

    // DETAIL POPUP SETUP
    float px = POP_X - POP_W / 2.f;
    float py = POP_Y - POP_H / 2.f;

    detailPanel.setSize(sf::Vector2f(POP_W, POP_H));
    detailPanel.setPosition(px, py);
    detailPanel.setFillColor(sf::Color(18, 12, 40, 252));
    detailPanel.setOutlineColor(sf::Color(255, 200, 80));
    detailPanel.setOutlineThickness(3.f);

    // Close button top right
    detailCloseBtn.setRadius(16.f);
    detailCloseBtn.setOrigin(16.f, 16.f);
    detailCloseBtn.setFillColor(sf::Color(210, 50, 70));
    detailCloseBtn.setOutlineColor(sf::Color::White);
    detailCloseBtn.setOutlineThickness(2.f);
    detailCloseBtn.setPosition(px + POP_W - 18, py + 18);

    detailCloseTxt.setFont(font);
    detailCloseTxt.setString("X");
    detailCloseTxt.setCharacterSize(16);
    detailCloseTxt.setFillColor(sf::Color::White);
    detailCloseTxt.setPosition(px + POP_W - 26, py + 6);

    // Name
    detailName.setFont(font);
    detailName.setCharacterSize(26);
    detailName.setFillColor(sf::Color(255, 220, 80));

    // Dividers
    detailDivider1.setSize(sf::Vector2f(POP_W - 40, 2));
    detailDivider1.setFillColor(sf::Color(255, 200, 80, 160));
    detailDivider1.setPosition(px + 20, py + 210);

    detailDivider2.setSize(sf::Vector2f(POP_W - 40, 2));
    detailDivider2.setFillColor(sf::Color(255, 200, 80, 160));
    detailDivider2.setPosition(px + 20, py + 325);

    // Stats
    detailStat1.setFont(font); detailStat1.setCharacterSize(14);
    detailStat1.setFillColor(sf::Color(220, 240, 255));
    detailStat1.setPosition(px + 65, py + 222);

    detailStat2.setFont(font); detailStat2.setCharacterSize(14);
    detailStat2.setFillColor(sf::Color(220, 240, 255));
    detailStat2.setPosition(px + 65, py + 252);

    detailStat3.setFont(font); detailStat3.setCharacterSize(14);
    detailStat3.setFillColor(sf::Color(255, 215, 0));
    detailStat3.setPosition(px + 65, py + 282);

    // Description
    detailDesc.setFont(font);
    detailDesc.setCharacterSize(13);
    detailDesc.setFillColor(sf::Color(200, 215, 235));
    detailDesc.setPosition(px + 20, py + 338);

    // Ability
    detailAbility.setFont(font);
    detailAbility.setCharacterSize(13);
    detailAbility.setFillColor(sf::Color(130, 255, 160));
    detailAbility.setPosition(px + 20, py + 390);

    // Icons
    heartIconTex.loadFromFile("image/sprites2/ui_icons/icon_heart.png");
    heartIconSpr.setTexture(heartIconTex);
    heartIconSpr.setScale(0.16f, 0.16f);
    heartIconSpr.setPosition(px + 22, py + 220);

    boltIconTex.loadFromFile("image/sprites2/ui_icons/icon_star.png");
    boltIconSpr.setTexture(boltIconTex);
    boltIconSpr.setScale(0.16f, 0.16f);
    boltIconSpr.setPosition(px + 22, py + 250);

    coinIconTex.loadFromFile("image/sprites2/ui_icons/icon_coin.png");
    coinIconSpr.setTexture(coinIconTex);
    coinIconSpr.setScale(0.16f, 0.16f);
    coinIconSpr.setPosition(px + 22, py + 280);
}

void Game::setupDetailPopup(int index, bool isEnemy)
{
    showDetailPopup = true;
    detailIndex = index;
    detailIsEnemy = isEnemy;

    float px = POP_X - POP_W / 2.f;
    float py = POP_Y - POP_H / 2.f;

    // Enemy data
    const char* eNames[5] = { "SLIME","BEEBEE","ROCKO","FLOATY","JELLY" };
    const char* eStat1[5] = { "HP:  100","HP:  60","HP:  400","HP:  120","HP:  150" };
    const char* eStat2[5] = { "Speed:  80","Speed:  160","Speed:  40","Speed:  120","Speed:  90" };
    const char* eStat3[5] = { "Gold Reward:  +10","Gold Reward:  +15","Gold Reward:  +25","Gold Reward:  +20","Gold Reward:  +30" };
    const char* eDesc[5] = {
        "The most basic enemy.\nAverage speed and health.\nGood for learning the game.",
        "Extremely fast but fragile.\nPlace towers early on path!\nHard to kill before it escapes.",
        "A slow moving armored tank.\nRequires sustained heavy damage.\nUse Cannon towers against it!",
        "Flies in a straight line.\nCompletely ignores the path!\nPlace towers on its flight route.",
        "Splits into 2 mini jellies\nwhen killed. Plan carefully!\nYou need to kill all the minis too."
    };
    const char* eAbility[5] = {
        "No special ability",
        "SPECIAL: Extreme Speed",
        "SPECIAL: Very High HP",
        "SPECIAL: Ignores Path",
        "SPECIAL: Splits On Death!"
    };

    // Tower data
    const char* tNames[5] = { "CANNON","ICE TOWER","CANDY BLASTER","SUNFLOWER","BOMB TOWER" };
    const char* tStat1[5] = { "Damage:  50","Damage:  15","Damage:  20","Damage:  12","Damage:  80" };
    const char* tStat2[5] = { "Range:  130","Range:  120","Range:  115","Range:  140","Range:  110" };
    const char* tStat3[5] = { "Cost:  100 Gold","Cost:  120 Gold","Cost:  150 Gold","Cost:  100 Gold","Cost:  200 Gold" };
    const char* tDesc[5] = {
        "High damage per shot.\nSlow fire rate but devastating.\nBest choice against tank enemies.",
        "Low damage but slows enemies.\nSlowed enemies are easier to kill.\nPair with other towers!",
        "Rapid fire medium damage.\nVery effective against swarms.\nGreat for wave 2 and beyond.",
        "Hits ALL enemies in range\nat the same time!\nPerfect for crowded areas.",
        "Fires explosive bombs!\nMassive area damage on impact.\nExplosion hits nearby enemies too."
    };
    const char* tAbility[5] = {
        "SPECIAL: High Damage Per Shot",
        "SPECIAL: Slows Enemy Speed",
        "SPECIAL: Rapid Fire Rate",
        "SPECIAL: Area Attack - Hits All",
        "SPECIAL: Explosion Area Damage!"
    };

    if (isEnemy)
    {
        detailName.setString(eNames[index]);
        detailStat1.setString(eStat1[index]);
        detailStat2.setString(eStat2[index]);
        detailStat3.setString(eStat3[index]);
        detailDesc.setString(eDesc[index]);
        detailAbility.setString(eAbility[index]);
        detailSprite = enemyCards[index].spr;
    }
    else
    {
        detailName.setString(tNames[index]);
        detailStat1.setString(tStat1[index]);
        detailStat2.setString(tStat2[index]);
        detailStat3.setString(tStat3[index]);
        detailDesc.setString(tDesc[index]);
        detailAbility.setString(tAbility[index]);
        detailSprite = towerCards[index].spr;
    }

    // Center name
    sf::FloatRect nb = detailName.getLocalBounds();
    detailName.setPosition(POP_X - nb.width / 2.f, py + 28);

    // Big sprite centered
    detailSprite.setScale(0.42f, 0.42f);
    sf::FloatRect sb = detailSprite.getLocalBounds();
    detailSprite.setOrigin(sb.width / 2.f, sb.height / 2.f);
    detailSprite.setPosition(POP_X, py + 145);
}

void Game::renderDetailPopup()
{
    // Dark overlay
    sf::RectangleShape overlay(sf::Vector2f(1366, 768));
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);

    window.draw(detailPanel);
    window.draw(detailName);
    window.draw(detailSprite);
    window.draw(detailDivider1);

    window.draw(heartIconSpr);
    window.draw(boltIconSpr);
    window.draw(coinIconSpr);
    window.draw(detailStat1);
    window.draw(detailStat2);
    window.draw(detailStat3);

    window.draw(detailDivider2);
    window.draw(detailDesc);
    window.draw(detailAbility);

    window.draw(detailCloseBtn);
    window.draw(detailCloseTxt);
}

void Game::run()
{
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized)
        {
            gameView.reset(sf::FloatRect(0, 0, 1366, 768));
            window.setView(gameView);
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                if (showDetailPopup)
                {
                    showDetailPopup = false;
                    return;
                }
                if (state == STATE_PLAYING || state == STATE_WAVE_CLEAR)
                {
                    pendingTower = HUD::NONE;
                    hud.clearSelection();
                    hud.setShowUpgrade(false);
                    state = STATE_LEVEL_SELECT;
                }
                else if (state == STATE_LEVEL_SELECT)
                    state = STATE_MENU;
            }
        }

        if (event.type == sf::Event::MouseMoved && state == STATE_PLAYING)
        {
            sf::Vector2f m = getScaledMouse(event.mouseMove.x, event.mouseMove.y);
            float mx = m.x, my = m.y;
            float mapY = my - HUD::TOP_BAR_H;
            if (mx < Map::MAP_WIDTH && mapY > 0 && pendingTower != HUD::NONE)
            {
                int col, row;
                if (map.pixelToGrid(mx, mapY, col, row) && map.isPlaceable(col, row))
                {
                    showHover = true;
                    hoverCol = col;
                    hoverRow = row;
                    hoverCell.setPosition(
                        col * Map::TILE_SIZE,
                        row * Map::TILE_SIZE + HUD::TOP_BAR_H);
                }
                else showHover = false;
            }
            else showHover = false;
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f m = getScaledMouse(event.mouseButton.x, event.mouseButton.y);
            float mx = m.x, my = m.y;

            // Always check victory/defeat popups first
            if (hud.isContinueClicked(mx, my))
            {
                hud.setShowVictory(false);
                state = STATE_LEVEL_SELECT;
                return;
            }
            if (hud.isRetryClicked(mx, my))
            {
                hud.setShowDefeat(false);
                startLevel(selectedLevel);
                return;
            }

            // Detail popup close button
            if (showDetailPopup)
            {
                float dx = mx - detailCloseBtn.getPosition().x;
                float dy = my - detailCloseBtn.getPosition().y;
                if (dx * dx + dy * dy <= 16.f * 16.f)
                {
                    showDetailPopup = false;
                    return;
                }
                // Block all other clicks when popup open
                return;
            }

            if (state == STATE_MENU)
            {
                if (playBtnSprite.getGlobalBounds().contains(mx, my))
                    state = STATE_LEVEL_SELECT;
            }
            else if (state == STATE_LEVEL_SELECT)
            {
                // Check card clicks FIRST for detail popup
                CharCard* cards = showEnemyCards ? enemyCards : towerCards;
                for (int i = 0; i < NUM_CHARS; i++)
                {
                    if (cards[i].bg.getGlobalBounds().contains(mx, my))
                    {
                        setupDetailPopup(i, showEnemyCards);
                        return;
                    }
                }

                // Level bubbles
                for (int i = 0; i < NUM_LEVELS; i++)
                {
                    if (levelBubbles[i].locked) continue;
                    float dx = mx - levelBubbles[i].x;
                    float dy = my - levelBubbles[i].y;
                    if (dx * dx + dy * dy <= 38.f * 38.f)
                    {
                        selectedLevel = i + 1;
                        for (int j = 0; j < NUM_LEVELS; j++)
                            levelBubbles[j].circle.setOutlineThickness(3.f);
                        levelBubbles[i].circle.setOutlineThickness(6.f);
                    }
                }

                if (startBtn.getGlobalBounds().contains(mx, my))
                    startLevel(selectedLevel);

                if (toggleBtn.getGlobalBounds().contains(mx, my))
                    showEnemyCards = !showEnemyCards;
            }
            else if (state == STATE_PLAYING)
            {
                float mapY = my - HUD::TOP_BAR_H;
                if (mx < Map::MAP_WIDTH && mapY > 0)
                    handleMapClick(mx, mapY);
                else if (mx >= HUD::PANEL_X)
                    handleHUDClick(mx, my);
            }
            else if (state == STATE_WAVE_CLEAR)
            {
                startNextWave();
                wave.start();
                state = STATE_PLAYING;
            }
        }
    }
}

void Game::startLevel(int level)
{
    resetGame();
    selectedLevel = level;
    if (level == 1) gold = 150;
    else if (level == 2) gold = 120;
    else if (level == 3) gold = 100;
    state = STATE_PLAYING;
}

void Game::resetGame()
{
    gold = 150;
    lives = 10;
    currentWave = 1;
    towerCount = 0;
    showHover = false;
    selectedTowerIndex = -1;
    pendingTower = HUD::NONE;
    waveClearTimer = 0.f;
    showDetailPopup = false;

    for (int i = 0; i < MAX_TOWERS; i++)
    {
        delete towers[i];
        towers[i] = nullptr;
    }

    map.buildMap();

    sf::Vector2f* wps = map.getWaypoints();
    for (int i = 0; i < map.getWaypointCount(); i++)
        wps[i].y += HUD::TOP_BAR_H;

    wave.setWaypoints(map.getWaypoints(), map.getWaypointCount(),
        map.getEntryPoint(), map.getExitPoint());
    wave.setupWave(1,selectedLevel);

    hud.clearSelection();
    hud.setShowUpgrade(false);
    hud.setShowVictory(false);
    hud.setShowDefeat(false);
}

void Game::update(float dt)
{
    if (state == STATE_MENU || state == STATE_LEVEL_SELECT) return;

    if (state == STATE_WAVE_CLEAR)
    {
        waveClearTimer -= dt;
        if (waveClearTimer <= 0.f)
        {
            startNextWave();
            wave.start();
            state = STATE_PLAYING;
        }
        return;
    }

    if (state == STATE_PLAYING ||
        state == STATE_VICTORY ||
        state == STATE_GAME_OVER)
    {
        hud.update();
        if (wave.hasStarted() && state == STATE_PLAYING)
        {
            wave.update(dt);
            towerAttack(dt);
            checkGameState();
        }
    }
}

void Game::render()
{
    window.clear(sf::Color(20, 12, 45));

    if (state == STATE_MENU)              renderMenu();
    else if (state == STATE_LEVEL_SELECT)      renderLevelSelect();
    else if (state == STATE_PLAYING ||
        state == STATE_VICTORY ||
        state == STATE_GAME_OVER)         renderPlaying();
    else if (state == STATE_WAVE_CLEAR)        renderWaveClear();

    window.display();
}

void Game::renderMenu()
{
    window.draw(menuBgSprite);
    window.draw(menuTitle);
    window.draw(menuSub);
    window.draw(playBtnSprite);
}

void Game::renderLevelSelect()
{
    window.draw(menuBgSprite);
    window.draw(levelSelectTitle);
    window.draw(islandSprite);

    for (int i = 0; i < NUM_LEVELS; i++)
    {
        if (levelBubbles[i].locked)
        {
            sf::CircleShape lc = levelBubbles[i].circle;
            lc.setFillColor(sf::Color(70, 70, 70, 200));
            window.draw(lc);
            sf::Text lt;
            lt.setFont(font);
            lt.setString("L");
            lt.setCharacterSize(24);
            lt.setFillColor(sf::Color(160, 160, 160));
            sf::FloatRect lb = lt.getLocalBounds();
            lt.setPosition(levelBubbles[i].x - lb.width / 2.f,
                levelBubbles[i].y - lb.height / 2.f - 8);
            window.draw(lt);
        }
        else
        {
            window.draw(levelBubbles[i].circle);
            window.draw(levelBubbles[i].numText);
        }
    }

    window.draw(startBtn);
    window.draw(startBtnText);

    // Character panel
    window.draw(charPanel);

    if (showEnemyCards)
    {
        charPanelTitle.setString("ENEMIES  — click any card for details");
        toggleTxt.setString("SHOW TOWERS");
    }
    else
    {
        charPanelTitle.setString("TOWERS  — click any card for details");
        toggleTxt.setString("SHOW ENEMIES");
    }
    window.draw(charPanelTitle);
    window.draw(toggleBtn);
    window.draw(toggleTxt);

    CharCard* cards = showEnemyCards ? enemyCards : towerCards;
    for (int i = 0; i < NUM_CHARS; i++)
    {
        window.draw(cards[i].bg);
        window.draw(cards[i].spr);
        window.draw(cards[i].nameTxt);
        window.draw(cards[i].descTxt);
    }

    // Hint
    sf::Text hint;
    hint.setFont(font);
    hint.setString("Click level bubble then START   |   ESC = back to menu");
    hint.setCharacterSize(13);
    hint.setFillColor(sf::Color(180, 180, 180));
    hint.setPosition(60, 670);
    window.draw(hint);

    // Detail popup on top
    if (showDetailPopup)
        renderDetailPopup();
}

void Game::renderPlaying()
{
    map.render(window);

    if (selectedTowerIndex >= 0 && towers[selectedTowerIndex])
        towers[selectedTowerIndex]->renderRange(window);

    for (int i = 0; i < towerCount; i++)
        if (towers[i]) towers[i]->render(window);

    wave.render(window);

    if (showHover) window.draw(hoverCell);

    hud.render(window);
    window.draw(keyHint);
}

void Game::renderWaveClear()
{
    renderPlaying();
    window.draw(waveClearBg);
    window.draw(waveClearText);
    window.draw(waveClearSub);
}

void Game::handleMapClick(float mx, float my)
{
    int col, row;
    if (!map.pixelToGrid(mx, my, col, row)) return;

    if (pendingTower != HUD::NONE)
    {
        if (map.isPlaceable(col, row))
            placeTower(col, row);
        else
        {
            pendingTower = HUD::NONE;
            hud.clearSelection();
            showHover = false;
        }
        return;
    }

    sf::Vector2f wp = map.gridToWorld(col, row);
    wp.y += HUD::TOP_BAR_H;
    for (int i = 0; i < towerCount; i++)
    {
        if (!towers[i]) continue;
        float dx = towers[i]->getX() - wp.x;
        float dy = towers[i]->getY() - wp.y;
        if (dx * dx + dy * dy < 1200.f)
        {
            selectedTowerIndex = i;
            hud.setShowUpgrade(true);
            hud.setUpgradeInfo(
                towers[i]->getDamage(),
                towers[i]->getRange(),
                towers[i]->getFireRate(),
                towers[i]->getUpgradeCost());
            return;
        }
    }

    selectedTowerIndex = -1;
    hud.setShowUpgrade(false);
}

void Game::handleHUDClick(float mx, float my)
{
    if (hud.isStartWaveClicked(mx, my))
    {
        if (!wave.hasStarted()) wave.start();
        return;
    }
    if (hud.isUpgradeClicked(mx, my) && selectedTowerIndex >= 0)
    {
        upgradeTower(selectedTowerIndex);
        return;
    }

    int choice = hud.handleClick(mx, my);
    if (choice != HUD::NONE)
    {
        pendingTower = choice;
        selectedTowerIndex = -1;
        hud.setShowUpgrade(false);
    }
}

void Game::placeTower(int col, int row)
{
    int costs[5] = { 100, 120, 150, 100, 200 };
    int cost = costs[pendingTower];
    if (gold < cost) { std::cout << "Not enough gold!\n"; return; }
    if (towerCount >= MAX_TOWERS) return;

    sf::Vector2f pos = map.gridToWorld(col, row);
    pos.y += HUD::TOP_BAR_H;

    Tower* t = nullptr;
    if (pendingTower == HUD::CANNON)    t = new CannonTower(pos.x, pos.y);
    else if (pendingTower == HUD::ICE)       t = new IceTower(pos.x, pos.y);
    else if (pendingTower == HUD::CANDY)     t = new CandyBlaster(pos.x, pos.y);
    else if (pendingTower == HUD::SUNFLOWER) t = new SunflowerTower(pos.x, pos.y);
    else if (pendingTower == HUD::BOMB)      t = new BombTower(pos.x, pos.y);
    else return;

    t->setPlaced(true);
    towers[towerCount++] = t;
    map.setTile(row, col, Map::TILE_BLOCKED);
    gold -= cost;

    pendingTower = HUD::NONE;
    hud.clearSelection();
    showHover = false;
}

void Game::upgradeTower(int index)
{
    if (index < 0 || !towers[index]) return;
    int cost = towers[index]->getUpgradeCost();
    if (gold < cost) { std::cout << "Not enough gold!\n"; return; }
    towers[index]->upgrade();
    gold -= cost;
    hud.setShowUpgrade(false);
}

void Game::towerAttack(float dt)
{
    Enemy** enemies = wave.getEnemies();
    int     enemyCount = wave.getEnemyCount();

    for (int i = 0; i < towerCount; i++)
    {
        if (!towers[i] || !towers[i]->getIsPlaced()) continue;
        towers[i]->update(dt);

        SunflowerTower* st = dynamic_cast<SunflowerTower*>(towers[i]);
        if (st) { st->attackAll(enemies, enemyCount); continue; }

        BombTower* bt = dynamic_cast<BombTower*>(towers[i]);
        if (bt)
        {
            // If bullet was active but now reached target → explode!
            bool wasActive = bt->isBulletActive();
            bt->update(dt);  // this moves bullet

            // Bullet just reached target this frame → trigger explosion
            if (wasActive && !bt->isBulletActive())
                bt->explode(enemies, enemyCount);

            // Find new target and fire if cooldown ready
            Enemy* target = bt->findTarget(enemies, enemyCount);
            if (target) bt->attack(target);
            continue;
        }

        Enemy* target = towers[i]->findTarget(enemies, enemyCount);
        if (target) towers[i]->attack(target);
    }
}

void Game::startNextWave()
{
    currentWave++;
    if (currentWave > TOTAL_WAVES)
    {
        if (selectedLevel >= unlockedLevels && unlockedLevels < NUM_LEVELS)
        {
            unlockedLevels++;
            for (int i = 0; i < NUM_LEVELS; i++)
                levelBubbles[i].locked = (i >= unlockedLevels);
        }
        hud.setShowVictory(true);
        return;
    }
    wave.setupWave(currentWave,selectedLevel);
}

void Game::checkGameState()
{
    if (lives <= 0)
    {
        hud.setShowDefeat(true);
        state = STATE_GAME_OVER;
        return;
    }
    if (wave.isComplete())
    {
        if (currentWave >= TOTAL_WAVES)
        {
            if (selectedLevel >= unlockedLevels && unlockedLevels < NUM_LEVELS)
            {
                unlockedLevels++;
                for (int i = 0; i < NUM_LEVELS; i++)
                    levelBubbles[i].locked = (i >= unlockedLevels);
            }
            hud.setShowVictory(true);
            state = STATE_VICTORY;
        }
        else
        {
            state = STATE_WAVE_CLEAR;
            waveClearTimer = 2.5f;
        }
    }
}