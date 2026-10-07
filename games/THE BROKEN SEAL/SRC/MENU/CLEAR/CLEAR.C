#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "GLOBAL_CELLS.H"
#include "EDITION.H"
#include "TBS_EDITION.H"
#include "TEXT_FONT.H"
#include "TYPES.H"
#include "SCENE.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"
#include "RESOURCE_IDS.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "FIELD_SCENE.H"

extern u16 gBgScroll[];

static __inline__ void DecodeBackground(const u8 *res)
{
    /* FAKEMATCH: an inline call wrapper keeps the decode's source in r4 and reloads the cell buffer's address after it, as the game does. */
    Resource_DecodeType01(res, (void *)Ram_MapCellBuffer);
}

struct ClearWork {
    u8 unknown_00[20];
    u16 mode;
};

#define DMA3 (REG_DMA3)

static __inline__ void RestoreInterrupts(u32 saved)
{
    REG_IME = saved;
}

#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                           \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        RestoreInterrupts(saved);                                          \
    } while (0)

extern volatile u32 gKeysHeld;

u8 *SceneData_GetScriptTable(void)
{
    return Clear_ScriptTable;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Clear_MessageTable;
}

u8 *SceneData_GetActorTable(void)
{
    return Clear_ActorTable;
}

u8 *SceneData_GetEffectTable(void)
{
    return Clear_EffectTable;
}

void Clear_LoadBackground(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s32 tile;
    struct ScrollPair *scroll;
    s32 blank;
    u16 zero;

    id = (s32)&ResourceId_GoldenSunLogo;
    Engine_BlendSetDarkenTarget16(0);
    REG_BG2CNT = 0x681;
    gBgScroll[5] = 0;
    blank = 511;
    res = Engine_ResourceGetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 448;
    DecodeBackground(res);
    Dma_Set(Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 416;
    y = 0;
col:
    {
        x = 0;
    row:
        {
            s32 old = tile;

            /* FAKEMATCH: keep the signed tile wrap in the high half. */
            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
    scroll = (struct ScrollPair *)gBgScroll;
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, DMA3);
    ((struct ClearWork *)gMapWork[0])->mode = 0x1400;
    {
        struct FieldActor *leader;

        leader = Object_GetById(gGameState.selected_actor);
        /* FAKEMATCH: a halfword zero keeps the reference's short-reach pool. */
        zero = 0;
        leader->motion_flags = zero;
    }
}

void Clear_UpdateBlend(void)
{
    /* FAKEMATCH: the one-pass IME read keeps the saved copy before masking; the count cast preserves the queue's original publication order. */
    struct IoWriteQueue *q;
    s32 frame;
    s32 level;

    frame = Clear_BlendFrame + 1;
    Clear_BlendFrame = frame;
    q = &gIoWriteQueue;
    level = (u16)frame >> 1;
    QUEUE_WRITE(0x4000050, 0x2e51);
    QUEUE_WRITE(0x4000052, ((16 - (u16)level) << 8) | (u16)level);
    if ((u16)level > 15)
        Engine_TaskRemoveCallback(Clear_UpdateBlend);
}

void Clear_CheckButtonCodes(void)
{
    /* FAKEMATCH: volatile keys make each test read them again, as the game does. */
    if (Clear_CodeUnlocked == 0) {
        if (Clear_CodeHeld != 0) {
            if (gKeysHeld == 0)
                Clear_CodeHeld = 0;
        } else if (gKeysHeld != 0) {
            if (gKeysHeld == Clear_CodeSequence[Clear_CodeProgress]) {
                Clear_CodeProgress++;
                Clear_CodeHeld = 1;
                if (Clear_CodeSequence[Clear_CodeProgress] == 0) {
                    Clear_CodeUnlocked = 1;
                    Engine_AudioPlayCue(110);
                }
            } else {
                Clear_CodeProgress = 0;
            }
        }
    }
    if (Clear_ExtraCodeUnlocked == 0) {
        if (Clear_ExtraCodeHeld != 0) {
            if (gKeysHeld == 0)
                Clear_ExtraCodeHeld = 0;
        } else if (gKeysHeld != 0) {
            if (gKeysHeld == Clear_ExtraCodeSequence[Clear_ExtraCodeProgress]) {
                Clear_ExtraCodeProgress++;
                Clear_ExtraCodeHeld = 1;
                if (Clear_ExtraCodeSequence[Clear_ExtraCodeProgress] == 0) {
                    Clear_ExtraCodeUnlocked = 1;
                    Engine_AudioPlayCue(110);
                }
            } else {
                Clear_ExtraCodeProgress = 0;
            }
        }
    }
}

void Clear_NameEntryCharacter(s32 cell, u8 *text)
{
    text[1] = 0;
    text[2] = 0;
#if EDITION_INTERNATIONAL
    if (cell <= 7)
        text[0] = cell + 'A';
    else if (cell <= 12)
        text[0] = cell + 'A' + 1;
    else if (cell <= 23)
        text[0] = cell + 'A' + 2;
    else if (cell <= 31)
        text[0] = cell + 26;
    else if (cell <= 42)
        text[0] = cell + 'A';
    else if (cell <= 44)
        text[0] = cell + 'A' + 1;
    else if (cell <= 55)
        text[0] = cell + 'A' + 2;
    else if (cell == 56)
        text[0] = '!';
    else if (cell == 57)
        text[0] = '?';
    else if (cell == 58)
        text[0] = '#';
    else if (cell == 59)
        text[0] = '&';
    else if (cell == 60)
        text[0] = '$';
    else if (cell == 61)
        text[0] = '%';
    else if (cell == 62)
        text[0] = '+';
    else
        text[0] = '=';
#else
    if (cell <= 14)
        text[0] = cell - 111;
    else if (cell <= 44)
        text[0] = cell - 47;
    else if (cell == 45)
        text[0] = 134;
    else {
        if (cell <= 55)
            text[0] = cell + 104;
        else if (cell == 56)
            text[0] = 224;
        else if (cell == 57)
            text[0] = 227;
        else if (cell == 58)
            text[0] = 228;
        else
            text[0] = cell - 81;
        text[1] = TEXT_MARK_DAKUTEN;
    }
#endif
}

s32 Scene_GetModeMask(void)
{
    s32 mode;

    if (Engine_GameFlagIsSet(324) == 0) {
        return 0;
    }
    if (gGameState.scene_variant == 2) {
        return 0;
    }
    mode = gGameState.scene ^ (s32)&SceneId_WorldMap;
    return -((u32)(-mode | mode) >> 31);
}
