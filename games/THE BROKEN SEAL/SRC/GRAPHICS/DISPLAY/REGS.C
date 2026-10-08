#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BTLWORK.H"
#include "B5_CONTEXT.H"
#include "IWRAM_CALL.H"
#include "HEAP_STATE.H"
#include "IO_REG.H"

#define gBattleFxWork (*(struct BattleEffectWork * *)&gBattleFxWork)


#define FADE_STEP 1092

void Graphics_ApplyWindowBlendRegisters(void)
{
    struct BattleEffectWork *work = gBattleFxWork;

    REG_WIN0H = work->win0h;
    REG_WIN0V = work->win0v;
    REG_WIN1H = work->win1h;
    REG_WIN1V = work->win1v;
    REG_WININ = work->winin;
    REG_WINOUT = work->winout;
    REG_DISPCNT = work->dispcnt;
    REG_BLDCNT = work->bldcnt;
    REG_BLDALPHA = work->bldalpha;
}

void Display_ApplyBg2Reference(void)
{
    struct BattleEffectWork *work = gBattleFxWork;

    REG_BG2X = work->bg2x;
    REG_BG2Y = work->bg2y;
}

void Palette_StepFadeTransfer(void)
{
    /* FAKEMATCH: needed to match: relative heap cells 9 and 39 avoid address instructions added by named globals or full heap indexing. */
    void **slots = (void **)((u8 *)gWorkSlot + 9 * sizeof(void *));
    struct BattleEffectWork *work = slots[39 - 9];
    struct BtlWork *battle = slots[0];

    if (work->fade_frames > 0) {
        s32 step = ++work->fade_step;

        Graphics_ScaleRgb555ClampedFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
            0x10000 - step * FADE_STEP, 128);
        work->fade_frames--;
    }
}

void BattleFx_ClearActorTimers(void)
{
    Iwram_ClearWords(gBattleFxWork->actor_timers, sizeof(gBattleFxWork->actor_timers));
}

void ObjectGroup_TickMemberTimers(void)
{
    struct BattleEffectWork *work;
    s32 i;

    work = gBattleFxWork;
    i = 0;
    do {
        if (work->actor_timers[i] != 0) {
            if ((work->actor_timers[i] = work->actor_timers[i] - 1) == 0) {
                ObjectGroup_UpdateMembers(
                    work->effect->actors[i],
                    0, -1, -1, 0);
            }
        }
        i += 1;
    } while (i != 8);
}
