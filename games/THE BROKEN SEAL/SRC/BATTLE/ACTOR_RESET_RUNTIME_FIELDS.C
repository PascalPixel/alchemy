#include "TYPES.H"
#include "SCENE.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "FIXED_MATH.H"
#include "UI.H"
s32 BattleUnit_BuildStatusFlags(s32, s32);

u8 *Owner_GetStateFar(s32);
void Owner_RecalculateStatsFar(s32);
s32 GetBattleObjectSlot(s32);

s32 BattleActor_ResetRuntimeFields(s32 actor)
{
    u8 *state;
    u8 *cursor;
    s32 count;
    u8 zero;

    state = Owner_GetStateFar(actor);
    count = 3;
    zero = 0;
    cursor = state + 0x12f;
    do {
        count--;
        *cursor-- = zero;
    } while (count >= 0);

    state[0x131] = 0;
    state[0x132] = 0;
    state[0x133] = 0;
    state[0x134] = 0;
    state[0x135] = 0;
    state[0x136] = 0;
    state[0x137] = 0;
    state[0x138] = 0;
    state[0x139] = 0;
    state[0x13a] = 0;
    state[0x13b] = 0;
    state[0x13c] = 0;
    state[0x13d] = 0;
    state[0x13e] = 0;
    state[0x13f] = 0;
    state[0x140] = 0;
    state[0x141] = 0;
    state[0x142] = 0;
    state[0x143] = 0;
    state[0x144] = 0;
    state[0x145] = 0;
    state[0x146] = 0;
    state[0x147] = 0;
    state[0x148] = 0;

    Owner_RecalculateStatsFar(actor);
    return BattleUnit_BuildStatusFlags(actor, GetBattleObjectSlot(actor));
}

/* The arrow sprite queued each frame: the list link, then the three OAM
   attribute halfwords and the unused fourth. */
struct AdvanceSprite {
    struct AdvanceSprite *next;
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 attr3;
};

/* The message window the arrow sits on; its position is in tiles. */
struct AdvanceWindow {
    s32 state;
    void *self;
    u16 width;
    u16 height;
    u16 col;
    u16 row;
};

/* The window's scroll offset in 8.8 fixed point. */
struct AdvanceOffset {
    s32 unknown_00;
    u16 x;
    u16 y;
};

struct BattleDisplayWork {
    struct AdvanceWindow *window;
    struct AdvanceOffset *offset;
};

extern u8 BattlePres_AdvanceArrowTiles[];
extern volatile u32 gFrameCount;
extern struct BattleDisplayWork *gBattleDisplayWork;
extern volatile u32 gKeysHeld;
extern volatile u32 gKeyState;
void Audio_PlayCue(s32 cue);
void Resource_ResetEntry(s32 id);
void Runtime_PushSlotEntry(void *entry, s32 slot);

/* Draw the bobbing advance arrow beside the battle message until a button
   is pressed (after 16 frames, a held button also counts). */
s32 BattlePresentation_WaitForAdvance(void)
{
    struct AdvanceSprite *spr;
    struct AdvanceSprite sprite;
    s32 frame;
    s32 slot;
    s32 src;
    struct AdvanceWindow *window;
    struct AdvanceOffset *offset;
    s32 attr2;
    s32 attr1;
    s32 x;
    s16 tile;

    while (!UiWork_IsCompleteFar())
        WaitFrames(1);
    spr = &sprite;
    slot = Resource_LoadIntoFreeSlot(128);
    frame = 0;
loop:
    src = ((gFrameCount >> 2) & 7) * 128 + (s32)BattlePres_AdvanceArrowTiles;
    window = gBattleDisplayWork->window;
    offset = gBattleDisplayWork->offset;
    QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
    QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
    /* FAKEMATCH: a one-pass loop with a block-local s32 temporary around the
       BLDALPHA write; the temporary makes 16 a movs (a u16 constant is a pool
       halfword) and the loop notes keep it ahead of the register address. */
    do { /* FAKEMATCH: one-pass loop and forced temporary, see above */
        s32 blend = 16;
        REG_BLDALPHA = blend;
    } while (0);
    *(u32 *)&spr->attr0 = 0xa400;
    *(u32 *)&spr->attr2 = 0;
    tile = Resource_GetBuffer(slot, src) & 0x3ff;
    attr2 = spr->attr2;
    spr->attr2 = (attr2 & ~0x3ff) | tile;
    x = (u16)((window->col * 8 + (offset->x >> 8) + 4) & 0x1ff);
    attr1 = spr->attr1;
    spr->attr1 = (attr1 & ~0x1ff) | x;
    *(u8 *)&spr->attr0 = Trig_Sin(gFrameCount << 12) / 32768 + window->row * 8 + (offset->y >> 8) + 6;
    Runtime_PushSlotEntry(spr, 240);
    if (!(gKeysHeld & 2) && !(gKeyState & 0x303) && (frame <= 15 || !(gKeysHeld & 0x303))) {
        WaitFrames(1);
        frame++;
        goto loop;
    }
    Audio_PlayCue(111);
    Resource_ResetEntry(slot);
    WaitFrames(1);
}

/* The same queued sprite with its OAM attributes as fields. */
struct PromptSprite {
    struct PromptSprite *next;
    u16 y : 8;
    u16 affine : 2;
    u16 mode : 2;
    u16 mosaic : 1;
    u16 colors : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u16 unused;
};

struct PromptPoint {
    s32 x;
    s32 y;
};

s32 Ui_GetTableWordZeroFar(s32 index);

/* Draw the prompt arrow at (x, y), nudged up and left every fourth frame,
   until A, B, L or R is pressed. */
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y)
{
    struct PromptPoint pos;
    s32 tiles;
    struct PromptSprite sprite;
    struct PromptSprite *spr;
    s32 slot;

    tiles = Ui_GetTableWordZeroFar(0);
    pos.x = x;
    pos.y = y;
    while (!UiWork_IsCompleteFar())
        WaitFrames(1);
    spr = &sprite;
    slot = Resource_LoadIntoFreeSlot(128);
    while (1) {
        QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
        QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
        /* FAKEMATCH: the same one-pass loop and block-local s32 temporary
           as BattlePresentation_WaitForAdvance's BLDALPHA write: the
           temporary makes 16 a movs and the loop notes keep it ahead of
           the register address. */
        do { /* FAKEMATCH: one-pass loop and forced temporary, see above */
            s32 blend = 16;
            REG_BLDALPHA = blend;
        } while (0);
        ((s32 *)spr)[1] = 0x40000000;
        ((s32 *)spr)[2] = 0;
        spr->tile = Resource_GetBuffer(slot, tiles);
        spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        Runtime_PushSlotEntry(spr, 240);
        if (gKeyState & 0x303)
            break;
        WaitFrames(1);
    }
    Resource_ResetEntry(slot);
    WaitFrames(1);
}
