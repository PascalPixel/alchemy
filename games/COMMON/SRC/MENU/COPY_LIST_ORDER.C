#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


void Menu_CopyListOrder(s32 mode, u8 *order)
{
    u8 *src;
    s32 count;

    src = Menu_ListOrderDefault;
    switch (mode) {
    case 0:
        src = Menu_ListOrderMode0;
        break;
    case 1:
        src = Menu_ListOrderMode1;
        break;
    case 2:
        src = Menu_ListOrderMode2;
        break;
    }
    *order = *src;
    count = 0;
    if (*order != 0xff) {
        do {
            if (++count > 31)
                break;
            src++;
            order++;
            *order = *src;
        } while (*order != 0xff);
    }
}
