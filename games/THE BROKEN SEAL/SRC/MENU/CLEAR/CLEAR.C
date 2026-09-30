#include "TYPES.H"
#include "SCENE.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"
#include "RESOURCE_IDS.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "FIELD_SCENE.H"

extern u8 Clear_ScriptTable[];
extern u8 Clear_MessageTable[];
extern u8 Clear_ActorTable[];
extern u8 Clear_EffectTable[];

extern struct MapRenderWork *gMapWork;
extern u16 gBgScroll[];
void Resource_DecodeType01(const u8 *source, void *destination);

/* FAKEMATCH: an inline call wrapper keeps the decode's source in r4 and
   reloads the cell buffer's address after it, as the game does. */
static __inline__ void DecodeBackground(const u8 *res)
{
    Resource_DecodeType01(res, (void *)Ram_MapCellBuffer);
}

struct ClearWork {
    u8 unknown_00[20];
    u16 mode;
};

struct ScrollPair {
    u16 x;
    u16 y;
};

#define DMA3 ((volatile u32 *)0x040000d4)

extern u16 Clear_BlendFrame;
void Clear_UpdateBlend(void);

static __inline__ void RestoreInterrupts(u32 saved)
{
    /* FAKEMATCH: keep the final hardware address local to restoration. */
    do { REG_IME = saved; } while (0);
}

/* FAKEMATCH: the one-pass IME read keeps the saved copy before masking;
 * the count cast preserves the queue's original publication order. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
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

/* FAKEMATCH: volatile keys make each test read them again, as the game does. */
extern volatile u32 gKeysHeld;
extern s16 Clear_CodeUnlocked;
extern s16 Clear_ExtraCodeUnlocked;
extern s16 Clear_CodeProgress;
extern s16 Clear_ExtraCodeProgress;
extern s16 Clear_CodeHeld;
extern s16 Clear_ExtraCodeHeld;
extern u16 Clear_CodeSequence[];
extern u16 Clear_ExtraCodeSequence[];

/* Each getter is eight bytes including the one pool word that holds the
 * table it returns. */
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

/* Load the clear-screen background: palette, tiles and a 30 x 20 map counting up from
 * tile 0x1a0, then clear the scroll registers. */
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
    *(volatile u16 *)0x0400000c = 0x681;
    gBgScroll[5] = 0;
    blank = 0x1ff;
    res = Engine_ResourceGetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    DecodeBackground(res);
    Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
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
    (*(struct ClearWork **)&gMapWork)->mode = 0x1400;
    {
        struct FieldActor *leader;

        leader = Object_GetById(gGameState.selected_actor);
        /* FAKEMATCH: a halfword zero keeps the reference's short-reach pool. */
        zero = 0;
        leader->motion_flags = zero;
    }
}

/* Fade the blend in step by step each frame; remove itself once full. */
void Clear_UpdateBlend(void)
{
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

/* Write the one-character string for a name-entry grid cell. */
void Clear_NameEntryCharacter(s32 cell, u8 *text)
{
    text[1] = 0;
    text[2] = 0;
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
}

s32 Scene_GetModeMask(void)
{
    s32 mode;

    if (Engine_GameFlagIsSet(324) == 0) {
        return 0;
    }
    if (gGameState.unknown_23e == 2) {
        return 0;
    }
    mode = gGameState.scene ^ (s32)&SceneId_WorldMap;
    return -(s32)((u32)(-mode | mode) >> 31);
}
