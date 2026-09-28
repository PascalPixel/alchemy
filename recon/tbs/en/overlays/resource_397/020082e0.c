/* NONMATCHING: resource_397 at 0x020082e0 (32 bytes with its pool), the
 * last function of FIELD/TORETO_EDA/BACKGROUND_SCROLL.C, stays listing.
 *
 * Remaining difference: the second call's arguments. The ROM builds the
 * priority first and loads the callback last:
 *     movs r1, #200 / lsls r1, r1, #4 / ldr r0, =Effect_SetBg3HofsSplit
 * this source schedules the callback load between the two:
 *     movs r1, #200 / ldr r0, =Effect_SetBg3HofsSplit / lsls r1, r1, #4
 * Tried without success: a prototype and an old-style declaration of the
 * scheduler, an s16 priority, the callback in a local, an inline wrapper,
 * the function in its own file, and static callbacks. Only an integer
 * constant in place of the callback's symbol reproduced the ROM order, and
 * that spells the link layout instead of the name.
 */

#include "TYPES.H"

void Effect_UpdateBg3HofsByVcount(void);
void Effect_SetBg3HofsSplit(void);
void Runtime_SetIrqHandler(s32, s32, void (*)(void));
void Engine_ScheduleCallback(void (*)(void), s32);

void State_ApplyTables826dAnd82a1(void)
{
    Runtime_SetIrqHandler(1, 0, Effect_UpdateBg3HofsByVcount);
    Engine_ScheduleCallback(Effect_SetBg3HofsSplit, 0xC80);
}
