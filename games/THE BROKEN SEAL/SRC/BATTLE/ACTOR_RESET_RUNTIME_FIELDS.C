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

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

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
extern u32 gFrameCount;
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
#endif
