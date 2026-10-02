#include "TYPES.H"
#include "SCENE.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_WORK.H"
#include "MENU_LIST.H"
s32 BattleUnit_BuildStatusFlags(s32, s32);

void Owner_RecalculateStatsFar(s32);
struct BattleObjectSlot *GetBattleObjectSlot(s32);

s32 BattleActor_ResetRuntimeFields(s32 actor)
{
    struct BattleUnit *state;
    s8 *cursor;
    s32 count;
    u8 zero;

    state = Owner_GetStateFar(actor);
    count = 3;
    zero = 0;
    cursor = &state->element_modifier[3];
    do {
        count--;
        *cursor-- = zero;
    } while (count >= 0);

    state->poison = 0;
    state->attack_modifier_turns = 0;
    state->attack_modifier = 0;
    state->defense_modifier_turns = 0;
    state->defense_modifier = 0;
    state->res_modifier_turns = 0;
    state->res_modifier = 0;
    state->delusion = 0;
    state->confusion = 0;
    state->charm = 0;
    state->stun = 0;
    state->sleep = 0;
    state->psy_seal = 0;
    state->refrain = 0;
    state->reflect = 0;
    state->evil_spirit = 0;
    state->death_count = 0;
    state->unknown_142[0] = 0;
    state->unknown_142[1] = 0;
    state->ready_pose = 0;
    state->cannot_move = 0;
    state->agility_modifier_turns = 0;
    state->agility_modifier = 0;
    state->battle_end_state = 0;

    Owner_RecalculateStatsFar(actor);
    return BattleUnit_BuildStatusFlags(actor, (s32)GetBattleObjectSlot(actor));
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

extern u8 BattlePres_AdvanceArrowTiles[];
extern volatile u32 gFrameCount;
extern volatile u32 gKeysHeld;
void Audio_PlayCue(s32 cue);
void Resource_ResetEntry(s32 id);

/* Draw the bobbing advance arrow beside the battle message until a button
   is pressed (after 16 frames, a held button also counts). */
s32 BattlePresentation_WaitForAdvance(void)
{
    struct AdvanceSprite *spr;
    struct AdvanceSprite sprite;
    s32 frame;
    s32 slot;
    s32 src;
    struct UiWindow *window;
    struct BattleDisplayOffset *offset;
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
    x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
    attr1 = spr->attr1;
    spr->attr1 = (attr1 & ~0x1ff) | x;
    *(u8 *)&spr->attr0 = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
    Runtime_PushSlotEntry((s32 *)spr, 240);
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

s32 Ui_GetTableWordZeroFar(s32 index);

/* Draw the prompt arrow at (x, y), nudged up and left every fourth frame,
   until A, B, L or R is pressed. */
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y)
{
    struct MenuPoint pos;
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
        ((s32 *)spr)[1] = MENU_SPRITE_SIZE_16;
        ((s32 *)spr)[2] = 0;
        spr->tile = Resource_GetBuffer(slot, tiles);
        spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        Runtime_PushSlotEntry((s32 *)spr, 240);
        if (gKeyState & 0x303)
            break;
        WaitFrames(1);
    }
    Resource_ResetEntry(slot);
    WaitFrames(1);
}
