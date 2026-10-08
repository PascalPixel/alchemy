#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


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
