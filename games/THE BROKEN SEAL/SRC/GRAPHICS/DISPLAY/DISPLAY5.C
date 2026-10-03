#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_WORK.H"
#include "B5_CONTEXT.H"
#include "IWRAM_CALL.H"
#include "HEAP_STATE.H"

/* The battle effect work, seen through its shadows of the window, blend and
   BG2 reference registers, which are copied to the hardware once a frame. */

#define REG_WIN0H (*(u16 *)0x04000040)
#define REG_WIN1H (*(u16 *)0x04000042)
#define REG_WIN0V (*(u16 *)0x04000044)
#define REG_WIN1V (*(u16 *)0x04000046)
#define REG_WININ (*(u16 *)0x04000048)
#define REG_WINOUT (*(u16 *)0x0400004a)
#define REG_DISPCNT_PLAIN (*(u16 *)0x04000000)
#define REG_BLDCNT (*(u16 *)0x04000050)
#define REG_BLDALPHA (*(u16 *)0x04000052)
#define REG_BG2X (*(s32 *)0x04000028)
#define REG_BG2Y (*(s32 *)0x0400002c)
extern struct BattleEffectWork *gBattleFxWork;

void Graphics_ScaleRgb555ClampedFar(void *, void *, s32, s32);
#define FADE_BG_PALETTE ((void *)0x050000c0) /* BG palettes 6 to 9 */
#define FADE_STEP 1092 /* one sixtieth of full brightness */

void Graphics_ApplyWindowBlendRegisters(void)
{
    struct BattleEffectWork *work = gBattleFxWork;

    REG_WIN0H = work->win0h;
    REG_WIN0V = work->win0v;
    REG_WIN1H = work->win1h;
    REG_WIN1V = work->win1v;
    REG_WININ = work->winin;
    REG_WINOUT = work->winout;
    REG_DISPCNT_PLAIN = work->dispcnt;
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
    /* FAKEMATCH: keep the existing relative pointer-cell bank transport.
       Named globals and full HeapState indexing add
       address instructions. These are slots 9 and 39 of the real heap owner. */
    void **slots = (void **)((u8 *)gWorkSlot + 9 * sizeof(void *));
    struct BattleEffectWork *work = slots[39 - 9];
    struct BattleSession *battle = slots[0];

    if (work->fade_frames > 0) {
        s32 step = ++work->fade_step;

        Graphics_ScaleRgb555ClampedFar(battle->palette, FADE_BG_PALETTE,
            0x10000 - step * FADE_STEP, 128);
        work->fade_frames--;
    }
}

void Runtime_ApplyValueToWork7818(void)
{
    /* IWRAM_CALL.H owns the actual two-argument resident entry. */
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
