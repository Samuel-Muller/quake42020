# quake42020

> Quake 4 modding project for IT266 Spring 2020 at New Jersey Institute of Technology.

## How to use Quakio Kart mod

* Download mario-kart branch
* Purchase and download Quake 4
* Move the Quakio Kart folder to the Quake 4 folder that contains Quake4.exe and q4base
* Copy game000.pk4 and config.spec from q4base to the Quakio Kart folder
* Open game.vcproj and idlib.vcproj with Microsoft Visual Studio to upgrade them
* Open q4sdk.sln in Microsoft Visual Studio, and build idlib first followed by game
* Copy gamex86.dll (which will be generated in Win32 which is one folder up from q4sdk.sln) into game000.pk4
* Run the mod from inside Quake 4

This mod is best played using a contoller. You can use a program such as AntiMicro to emulate mouse movement and keyboard buttons on a controller. Mapping mouse look to the left joystick and the move forward key to the A button provides good results.

## Deliverables

### Common deliverables
1. Updated GUI
    * Changed main menu to display custom background and play music
    * Added new icons for the weapons and changed their names in the GUI to reflect their new functionality
2. In Game Visuals showcasing your edits
    * Changed projectiles fired from weapons to look more like the items they are supposed to replicate
    * Changed ammo_currentweapon pickup model to look more like an item box
3. A README with the description of your changes and instructions on how to see them
    * If you can read this, a README has been added
4. A Shortcut that auto-launches your mod
    * Present on my computer
    * To make one yourself, make a shortcut to Quake4.exe and add "+disconnect -novid +set fs_game "Quakio Kart"" to the end
5. Compiling with no additional Warnings (Quake's original warnings are fine)
    * This can be tested by downloading the Quake 4 source code from either the master branch of this repository or from https://github.com/engineerOfLies/Game-Mod-Q4 as well as the mario-kart branch and compiling both of them. If Visual Studio doesn't show errors by default, you can press Ctrl + \ + E to open the error list window.

### Personal deliverables
1. Make movement like Mario Kart
    * Removed ability to do things like jump and look up/down that don't make sense in Mario Kart
    * Added acceleration as you hold the forward button to simulate stepping on the gas, which goes down when you let go of the forward button
2. Have checkpoints you can go through to track your time (like a track)
    * Made a new pickup named item_lapcounter that counts your laps and lap time.
    * When the player first picks one up it starts timing, and displays lap number and lap time when following items are picked up
    * These lap counters can be placed manually with "spawn item_lapcounter" or by placing them in a map
3. Replace pickups with Mario kart pickups
    * Made the ammo_currentweapon pickup into a general Mario Kart item pickup, with randomized contents like in Mario Kart
    * The ammo_currentweapon pickup checks to see if the player currently has an item, and only lets the player pick up one item at a time
    * These pickups can be placed manually with "spawn ammo_currentweapon" or by placing them in a map
4. Add an AI that can also race
    * Not done yet as of 3/8/2020
5. Replace weapons with Mario kart weapons
    * Blaster -> empty hand: fires blanks and acts as a placeholder when the player has no items
    * Machine gun -> banana: shoots a banana that sits on the ground and damages enemies that run into it
    * Shotgun -> starman: gives the player invincibility for roughly 15 seconds when used
    * Hyperblaster -> green shell: shoots a green shell that travels in a straight line and bounces off walls
    * Grenade launcher -> Bob-omb: shoots a Bob-omb that blows up in a large circular explosion on contact
    * Nailgun -> fake item box: shoots a box that looks like the item box except with a different texture that damages enemies that run into it
    * Rocket launcher -> blue shell: shoots a projectile into the air that tracks an enemy
    * Railgun -> mushroom: gives the player a speed boost when used
    * Lightning gun -> lightning: kills all AI and projectile entities when used
    * Dark matter gun -> Bowser shell: shoots a large Bowser shell that travels in a straight line and bounces off walls
    * Napalm launcher -> fire flower: shoots 10 fireballs that bounce
