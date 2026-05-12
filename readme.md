How to compile and run your project

This project is a 2D Tower Defense game developed in C++ using the SFML 2.6.2 library on Windows (64-bit).

To compile and run the project:

Install SFML 2.6.2 (64-bit) and set it up in your development environment (Visual Studio recommended).
Link the required SFML modules in your project settings:
sfml-graphics
sfml-window
sfml-system
sfml-audio
Ensure all asset folders (Sprites, Sounds, Fonts, image, etc.) are placed in the correct working directory (same level as the executable or configured project root).
Build the project in Release or Debug mode.
Run the executable generated after compilation.

Make sure the working directory is correctly set; otherwise, textures, fonts, and sound files will not load.

Which GUI library you used and how to install it

This project uses SFML (Simple and Fast Multimedia Library) version 2.6.2 (64-bit) for all graphics, audio, input handling, and window management.

SFML was used for:

Rendering sprites, shapes, text, and UI elements
Handling mouse and keyboard input
Playing background music and sound effects
Managing game window and view system

Installation steps for SFML (Windows 64-bit):

Download SFML 2.6.2 from the official SFML website.
Extract the library to a folder (e.g., C:\SFML).
In Visual Studio:
Add include directory: SFML\include
Add library directory: SFML\lib
Link the required .lib files:
sfml-graphics.lib
sfml-window.lib
sfml-system.lib
sfml-audio.lib
Copy required DLL files from SFML\bin into the executable folder.
Set project architecture to x64.
Project Overview

This is a 2D Tower Defense game where the player defends a path against multiple waves of enemies by placing different types of towers strategically.

The game is structured using a state-based system with the following screens:

Main Menu
Level Selection
Gameplay
Wave Clear Screen
Victory Screen
Game Over Screen

Game Flow:

The player starts from the menu and selects a level.
Each level consists of multiple waves of enemies.
The player places towers on valid grid positions to stop enemies from reaching the end.
Gold is used to place and upgrade towers.
Lives are reduced when enemies escape.
Completing all waves results in level completion and unlocking the next level.
Enemies

The game contains five enemy types:

Basic Enemy (Slime)
Balanced speed and health
Standard early-game enemy
Fast Enemy (BeeBee)
Very high speed
Low health
Tank Enemy (Rocko)
Very high health
Slow movement
Floating Enemy (Floaty)
Flies in a straight path
Ignores ground path constraints
Splitting Enemy (Jelly)
Splits into smaller enemies upon death
Requires strategic targeting
Towers

The game includes five tower types:

Cannon Tower
High damage
Slow attack speed
Ice Tower
Low damage
Slows enemies
Candy Blaster
Medium damage
Fast attack rate
Sunflower Tower
Area attack
Hits all enemies in range
Bomb Tower
High damage
Projectile-based with explosion damage (area effect)
Core Gameplay Systems
Grid-based map system for tower placement
Wave-based enemy spawning system
Tower targeting and attack system
Upgrade system for towers using gold
HUD system for controls and information display
Level selection and unlocking system
Popup system for detailed enemy/tower information

The game uses object-oriented programming principles including:

Inheritance (Tower base class with derived towers)
Polymorphism (different tower behaviors)
Dynamic casting (special tower abilities like Bomb and Sunflower)
State machine for screen management
Any known issues or limitations
Memory Management
Towers are dynamically allocated using new and manually deleted in reset functions.
Although handled, this approach can be error-prone if not carefully maintained.
Hardcoded Level Data
Gold and difficulty scaling are hardcoded inside functions instead of being data-driven.
This limits scalability for adding more levels.
Fixed Resolution Dependency
The game is designed primarily for 1366x768 resolution.
While resizing exists, UI scaling is not fully adaptive.
Limited Level System
Only three levels are supported currently.
Level progression logic is basic.
Input Blocking During Popups
While detail popup is open, all other interactions are blocked intentionally.
This may feel restrictive but prevents UI conflicts.
No Save System
Progress (like unlocked levels) resets when the game restarts unless extended externally.
