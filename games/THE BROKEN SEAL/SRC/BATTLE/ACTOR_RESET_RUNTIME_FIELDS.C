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
#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"
#include "SCENE.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "BTLSYS.H"
#include "BTLWORK.H"
#include "MENU_LIST.H"

s32 BattleActor_ResetRuntimeFields(s32 actor)
{
    struct BtlUnit *state;
    s8 *cursor;
    s32 count;
    u8 zero;

    state = UnitGetFar(actor);
    count = 3;
    zero = 0;
    cursor = &state->elm_lv[3];
    do {
        count--;
        *cursor-- = zero;
    } while (count >= 0);

    state->doku = 0;
    state->atk_cnt = 0;
    state->atk_lv = 0;
    state->def_cnt = 0;
    state->def_lv = 0;
    state->res_cnt = 0;
    state->res_lv = 0;
    state->maboroshi = 0;
    state->konran = 0;
    state->charm = 0;
    state->shibire = 0;
    state->nemuri = 0;
    state->fuuji = 0;
    state->refrain = 0;
    state->reflect = 0;
    state->akuryo = 0;
    state->shiryo = 0;
    state->unknown_142[0] = 0;
    state->unknown_142[1] = 0;
    state->sonae = 0;
    state->miugoki = 0;
    state->agi_cnt = 0;
    state->agi_lv = 0;
    state->chouhatsu = 0;

    UnitCalcFar(actor);
    return BattleUnit_BuildStatusFlags(actor, BtlObjGet(actor));
}

struct AdvanceSprite {
    struct AdvanceSprite *next;
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 attr3;
};

extern volatile u32 gFrameCount;
extern volatile u32 gKeysHeld;
void Audio_PlayCue(s32 cue);

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
    /* FAKEMATCH: the one-pass loop keeps the BLDALPHA address load after the value. */
    do {
        u16 blend = 16;
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
    if (!(gKeysHeld & KEY_B) && !(gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) && (frame <= 15 || !(gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS)))) {
        WaitFrames(1);
        frame++;
        goto loop;
    }
    Audio_PlayCue(111);
    Resource_ResetEntry(slot);
    WaitFrames(1);
}


s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y)
{
    struct MenuPoint pos;
    s32 tiles;
    struct CursorSprite sprite;
    struct CursorSprite *spr;
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
        /* FAKEMATCH: the one-pass loop keeps the BLDALPHA address load after the value. */
        do {
            u16 blend = 16;
            REG_BLDALPHA = blend;
        } while (0);
        ((s32 *)spr)[1] = MENU_SPRITE_SIZE_16;
        ((s32 *)spr)[2] = 0;
        spr->tile = Resource_GetBuffer(slot, tiles);
        spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        Runtime_PushSlotEntry((s32 *)spr, 240);
        if (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS))
            break;
        WaitFrames(1);
    }
    Resource_ResetEntry(slot);
    WaitFrames(1);
}
