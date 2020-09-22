MREGA

========================================

Build 20200922

(c) 2020 Franticware

www.franticware.com/mrega
github.com/Franticware/MREGA
www.high-voltage.cz
www.doshaven.eu

========================================

Description

This split-screen racing game is a loose demake of Franticware's
own MultiRacer - www.franticware.com/multiracer

WARNING! Do not play this game alone, it will be no fun ;-)

Use run_win.bat to launch the game in Windows (DOSBox is required)

========================================

Controls

Player 1:
W/Left shift, S/Left ctrl, A, D

Player 2:
Up/Right shift, Down/Right ctrl, Left, Right

========================================

Command line arguments

-swap   Swap left and right audio channels (might be useful on some ESS
        cards for example); press Left/Right on the title screen to test
-mute   Disable audio
-aXXX   Specify OPL2/3 address (default is -a388), should not be necessary

========================================

System Requirements

386
8 MB RAM
VGA compatible graphics card (not compatible with EGA despite the name...)
OPL2/OPL3 compatible sound card (AdLib, SoundBlaster, ...)

OR

DOSBox - www.dosbox.com

========================================

Special thanks to:

Sledge for organizing the #hvdosdev2020 competition 

Wohlstand for his awesome OPL3BankEditor - github.com/Wohlstand/OPL3BankEditor

Shayde/Reality for the Opal OPL3 emulator used in SDL port of the game

Vladimir Arnost for OPL3 programmer's guide:
    www.fit.vutbr.cz/~arnost/opl/opl3.html
    
Authors of the following libraries:
    SDL2 - www.libsdl.org
    libsamplerate - www.mega-nerd.com/SRC/
    
Authors of the following tutorials and examples (among others):
    www.brackeen.com/vga/
    www.agidev.com/articles/agispec/examples/sound/
