_.o0 SavageWheels 0o._
==========================

Savage Wheels is a 2D car crashing arcade game similar to the old classic Destruction Derby. 

Up to 4 vehicles take part in a deadly tournament. The goal is to eliminate the other derby contestants. Players may choose from a variety of vehicles each with it's unique characteristics: speed, acceleration, armor, damage and driver. Various bonuses can be collected in order to increase speed and damage or to fix vehicles. 

All drivers have background stories which are revealed when playing the campaign mode. Gameplay modes include a campaign, hot-seat mode between two players or melee mode where vehicles are controlled by the _fearsome_ game AI ;-). 


![alt text](http://i.imgur.com/09h8j1M.jpg "Gameplay screenshot")


  * Please check [HISTORY](HISTORY) for latest changes.
  * Please check [INSTALL](INSTALL) for how to compile the game.
  * Please check [LICENSE](LICENSE) for licensing (without sound fx). Check LICENSE file in bin/sound folder for sound fx license.

# Running the game

  * On *Windows* - Start the game by simply running the **savagewheels.exe** file.
  * On *Linux* - It is **reqiured** that you have 'sdl 1.2' package already installed. Start the game by running the **savagewheels** binary or the **run.sh** script in case you get an error that *libfmodex.so* is missing.

  You may also run the game in fullscreen mode by specifying *-fullscreen* on the command line.

# How to play

Please check the [Readme.html](http://htmlpreview.github.io/?https://github.com/petarov/savagewheels/blob/master/bin/Readme.html) for additional game information.

# Used Libraries 
  * Graphihcs
    * [libSDL](http://www.libsdl.org/) - Simple DirectMedia Layer is a cross-platform multimedia library.
  * XML
    * [TinyXML](http://www.grinninglizard.com/tinyxml/) - TinyXML is a simple, small, C++ XML parser that can be easily integrating into other programs.
  * Sound (Compiled with SDL_mixer)
    * [SDL_mixer](http://www.libsdl.org/projects/SDL_mixer/) - SDL_mixer is a sample multi-channel audio mixer library.
    * [libmikmod](http://mikmod.shlomifish.org/) - Mikmod is a module player and library supporting many formats, including mod, s3m, it, and xm.
  * Sound (Compiled with FModEx)
    * [FMod Ex](http://www.fmod.org/fmod-sales.html) - Audio engine : FMOD Sound System by Firelight Technologies  
