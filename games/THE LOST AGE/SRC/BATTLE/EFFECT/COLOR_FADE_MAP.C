#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


void ColorBuffer_MapToFadeStep1(u8 *buffer, s32 count)
{
    if (count > 0) {
        const u8 *table = ColorBuffer_FadeStep1Map;

        do {
            if (*buffer != 0)
                *buffer = table[*buffer & 31];
            count--;
            buffer++;
        } while (count != 0);
    }
}

void ColorBuffer_MapToFadeStep2(u8 *buffer, s32 count)
{
    if (count > 0) {
        const u8 *table = ColorBuffer_FadeStep2Map;

        do {
            if (*buffer != 0)
                *buffer = table[*buffer & 31];
            count--;
            buffer++;
        } while (count != 0);
    }
}

void ColorBuffer_MapToFadeStep3(u8 *buffer, s32 count)
{
    if (count > 0) {
        const u8 *table = ColorBuffer_FadeStep3Map;

        do {
            if (*buffer != 0)
                *buffer = table[*buffer & 31];
            count--;
            buffer++;
        } while (count != 0);
    }
}
