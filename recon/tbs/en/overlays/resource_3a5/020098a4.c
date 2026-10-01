/*
 * Draft of resource_3a5 0x020098a4 (Func_020018a4), the sand effect task
 * that RamakanSabaku_ClaimSandEffectVram starts; it sits between SABAKU3
 * and SAND_SETUP of games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU and links
 * as disassembly (section .text.x020098a4 of the overlay listing).
 *
 * Names it still needs where its bytes are: the halfwords at 0x0200a6bc
 * and 0x0200a6c0 beside RamakanSabaku_SandCounter, the 16-colour palette at
 * 0x02009f80 after RamakanSabaku_SafePointsOther, and the import veneers at
 * 0x02009cac, 0x02009cb4, 0x02009cc4, 0x02009cdc and 0x02009ce4.
 *
 * 2026-10-01 (matcher 2): first draft, 6296 against the listing with the
 * overlay ELF (the names above still unresolved). Every call, store and
 * loop is in place. Notes for the next pass: the zero stored into the flash
 * counter comes from the pool, which is what a hoisted halfword zero that
 * reload rematerialises gives (as in FuneHeya_RunWalkerStep); the 144-line
 * scroll, palette and copy loops count up and are not reversed in the
 * reference, where for loops here are reversed into count-downs (goto loops
 * keep them counting up but score 6561); the palette loop keeps 31 in fp.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "GAME_STATE.H"
#include "DMA.H"

extern s16 RamakanSabaku_SandVramSlot;
extern s16 RamakanSabaku_SandCounter;
extern s16 RamakanSabaku_SandPhase;
extern s16 RamakanSabaku_SandLevel;
extern s16 RamakanSabaku_SandFlash;
extern const u16 RamakanSabaku_SandPalette[16];
extern u8 RamakanSabaku_SandWork[];
extern u8 RamakanSabaku_SandTileBuffer[];
extern u8 *Data_03001ecc;
extern u8 gVramBlockCache[];
extern u32 gFrameCount;

void *Runtime_BumpAllocate(s32 size);
void Sys_Free(void *block);
void Resource_ActivateEntry(s32 slot);
void Runtime_PushSlotEntry(void *entry, s32 priority);
void QueueIoWriteDelay3(u32 address, u32 value);
s32 Engine_VramLoad(s32 slot, s32 size, const void *data);

/* The sandstorm: while the storm flag is set it thickens, otherwise it
   thins; its strength shifts the screen's scanlines, warms the sand
   palette, and fills the column of sand sprites the storm draws. */
void RamakanSabaku_RunSandstorm(void)
{
    u8 *buffer;
    s16 *color;
    u8 *display;
    u32 *column;
    u8 *source;
    u8 *destination;
    u32 *sprite;
    s32 tile;
    s32 i;
    u32 red;
    u32 green;
    u32 blue;
    s32 level;
    u32 attributes;
    s32 x;
    u32 offset;

    tile = *(u16 *)(gVramBlockCache + RamakanSabaku_SandVramSlot * 4 + 2) >> 5;
    if (RamakanSabaku_SandPhase != 0) {
        RamakanSabaku_SandCounter = 2;
    } else if (Engine_GameFlagIsSet(0x104) != 0) {
        if (RamakanSabaku_SandCounter > 0)
            RamakanSabaku_SandCounter--;
    } else if (RamakanSabaku_SandCounter <= 1) {
        if (++RamakanSabaku_SandCounter == 1)
            Dma_Set(RamakanSabaku_SandPalette, (void *)0x050003c0, 0x80000010,
                    (volatile u32 *)0x040000d4);
    }
    if (RamakanSabaku_SandCounter == 0) {
        Resource_ActivateEntry(RamakanSabaku_SandVramSlot);
        return;
    }
    display = Data_03001ecc;
    if (display != NULL) {
        s16 *line = (s16 *)(display + display[0x539] * 644 + 38);
        s16 shift = RamakanSabaku_SandCounter << 3;

        for (i = 0; i < 144; i++) {
            *line = shift;
            line += 2;
        }
    }
    buffer = Runtime_BumpAllocate(0x900);
    Dma_Set(RamakanSabaku_SandPalette, buffer, 0x80000010, (volatile u32 *)0x040000d4);
    color = (s16 *)(buffer + 12);
    for (i = 6; i < 12; i++) {
        u16 value = *color;

        red = value & 31;
        green = (value >> 5) & 31;
        blue = (value >> 10) & 31;
        level = RamakanSabaku_SandLevel;
        red += level / 3;
        blue -= level / 6;
        if (level > 60 && (gFrameCount & 1))
            green += (level << 6) / 120 - 32;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;
        *color++ = (blue << 10) | (green << 5) | red;
    }
    QueueIoWriteDelay3(0x050003cc, *(u32 *)(buffer + 12));
    QueueIoWriteDelay3(0x050003d0, *(u32 *)(buffer + 16));
    QueueIoWriteDelay3(0x050003d4, *(u32 *)(buffer + 20));
    RamakanSabaku_SandLevel = gGameState.unknown_232 * 120 / (s16)gGameState.unknown_22c;
    if (RamakanSabaku_SandLevel > 118)
        RamakanSabaku_SandFlash = 119;
    if (RamakanSabaku_SandFlash != 0) {
        RamakanSabaku_SandLevel = RamakanSabaku_SandFlash;
        RamakanSabaku_SandFlash -= 8;
        if (RamakanSabaku_SandFlash <= 0)
            RamakanSabaku_SandFlash = 0;
    }
    Dma_Set(RamakanSabaku_SandTileBuffer, buffer, 0x84000240, (volatile u32 *)0x040000d4);
    if (RamakanSabaku_SandFlash <= 118) {
        column = (u32 *)(buffer + 80);
        for (i = 12; i < 128 - RamakanSabaku_SandLevel; i++) {
            column[8] = 0xeeeeeeee;
            *column++ = 0xeeeeeeee;
            if ((i & 7) == 7)
                column += 8;
        }
        column[0] = ((u32 *)buffer)[0];
        column[8] = ((u32 *)buffer)[8];
    }
    source = buffer + 0x480;
    destination = buffer;
    for (i = 0; i < 0x480; i++) {
        if (*source != 0)
            *destination = *source;
        source++;
        destination++;
    }
    Engine_VramLoad(RamakanSabaku_SandVramSlot, 0x480, buffer);
    attributes = 0x80008000;
    sprite = (u32 *)RamakanSabaku_SandWork;
    offset = 0;
    x = 8;
    for (i = 0; i < 5; i++) {
        u32 y = (RamakanSabaku_SandCounter * 8 - 16) & 0x1ff;

        if (i == 4)
            attributes = 0x40000000;
        sprite[0] = 0;
        sprite[1] = (y << 16) | x | attributes;
        sprite[2] = 0xe400 | tile;
        Runtime_PushSlotEntry(RamakanSabaku_SandWork + offset, 255);
        tile += 8;
        sprite += 3;
        offset += 12;
        x += 32;
    }
    Sys_Free(buffer);
}
