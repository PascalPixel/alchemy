#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "GAME_STATE.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"
#include "MENU_LIST.H"
#include "RAM_BUFFER.H"
#include "VRAM_BLOCK.H"
#include "IO_REG.H"

#define RamakanSabaku_SandVramSlot (*(s16 *)&RamakanSabaku_SandVramSlot)
#define RamakanSabaku_SandCounter (*(s16 *)&RamakanSabaku_SandCounter)
#define RamakanSabaku_SandPhase (*(s16 *)&RamakanSabaku_SandPhase)

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

void RamakanSabaku_RunSandstorm(void)
{
    u8 *buffer;
    s32 tile;
    s16 *colors;
    s16 *color;
    u8 *display;
    u32 *column;
    u32 *word;
    u8 *source;
    u8 *destination;
    struct MenuSprite *sprite;
    s32 counter;
    u32 i;
    u32 fill;
    u32 *palette;
    u32 red;
    u32 green;
    u32 blue;
    s32 level;
    u32 attributes;
    s32 x;
    u32 offset;

    tile = gVramBlockCache[RamakanSabaku_SandVramSlot].offset >> 5;
    if (RamakanSabaku_SandPhase != 0) {
        RamakanSabaku_SandCounter = 2;
    } else if (Engine_GameFlagIsSet(0x104) != 0) {
        if (RamakanSabaku_SandCounter > 0) {
            RamakanSabaku_SandCounter--;
        }
    } else if (RamakanSabaku_SandCounter <= 1) {
        if (++RamakanSabaku_SandCounter == 1) {
            Dma_Set(RamakanSabaku_SandPalette, (void *)0x050003c0, 0x80000010,
                    REG_DMA3);
        }
    }
    counter = RamakanSabaku_SandCounter;
    if (counter == 0) {
        Resource_ActivateEntry(RamakanSabaku_SandVramSlot);
        return;
    }
    display = *Ram_DisplayWork;
    if (display != NULL) {
        s16 *line = (s16 *)(display + display[0x539] * 644);
        s32 shift = (s16)(counter << 3);

        line = (s16 *)((u8 *)line + 38);
        for (i = 0; i < 144; i++) {
            *line = shift;
            line += 2;
        }
    }

    buffer = MemAlloc(0x900);
    Dma_Set(RamakanSabaku_SandPalette, buffer, 0x80000010, REG_DMA3);
    colors = (s16 *)(buffer + 12);
    color = colors;
    for (i = 6; i < 12; i++) {
        u16 value = *color;

        red = value & 31;
        green = (value >> 5) & 31;
        blue = ((value >> 10) & 31) - 20;
        level = RamakanSabaku_SandLevel;
        red += level / 3;
        blue = blue - level / 6 + 20;
        if (level > 60 && (gFrameCount & 1)) {
            green = green + (level << 6) / 120 - 32;
        }
        if (red > 31) {
            red = 31;
        }
        if (green > 31) {
            green = 31;
        }
        if (blue > 31) {
            blue = 31;
        }
        *color++ = (blue << 10) | (green << 5) | red;
    }
    palette = (u32 *)0x050003cc;
    word = (u32 *)(buffer + 12);
    QueueIoWriteDelay3((u32)palette++, *word++);
    QueueIoWriteDelay3((u32)palette++, *word++);
    QueueIoWriteDelay3((u32)palette++, *word++);

    RamakanSabaku_SandLevel = gGameState.hazard_steps * 120 / gGameState.hazard_limit;
    if (RamakanSabaku_SandLevel > 118) {
        RamakanSabaku_SandFlash = 119;
    }
    if (RamakanSabaku_SandFlash != 0) {
        RamakanSabaku_SandLevel = RamakanSabaku_SandFlash;
        RamakanSabaku_SandFlash -= 8;
        if (RamakanSabaku_SandFlash <= 0) {
            RamakanSabaku_SandFlash = 0;
        }
    }

    Dma_Set(RamakanSabaku_SandTileBuffer, buffer, 0x84000240, REG_DMA3);
    if (RamakanSabaku_SandFlash <= 118) {
        column = (u32 *)(buffer + 80);
        fill = 0xeeeeeeee;
        for (i = 12; i < 128 - RamakanSabaku_SandLevel; i++) {
            column[8] = fill;
            *column++ = fill;
            if ((i & 7) == 7) {
                column += 8;
            }
        }
        column[0] = ((u32 *)buffer)[0];
        column[8] = ((u32 *)buffer)[8];
    }
    destination = buffer;
    source = buffer + 0x480;
    for (i = 0; i < 0x480; i++) {
        u8 value = *source++;

        if (value != 0) {
            *destination = value;
        }
        destination++;
    }
    Engine_VramLoad(RamakanSabaku_SandVramSlot, 0x480, buffer);

    attributes = 0x80008000;
    sprite = RamakanSabaku_SandWork;
    for (i = 0, offset = 0, x = 8; i < 5; i++) {
        u32 y = (RamakanSabaku_SandCounter * 8 - 16) & 0x1ff;

        if (i == 4) {
            attributes = 0x40000000;
        }
        sprite->next = NULL;
        sprite->oam.word.attr01 = (y << 16) | x | attributes;
        sprite->oam.word.attr23 = 0xe400 | tile;
        Runtime_PushSlotEntry((s32 *)((u8 *)RamakanSabaku_SandWork + offset), 255);
        sprite++;
        tile += 8;
        offset += 12;
        x += 32;
    }
    MemFree(buffer);
}
