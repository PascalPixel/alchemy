#include "types.h"

/* battle/effects/misc/schedule_ratio_transition.c */
/*
 * Record a ratio-driven transition on the battle effect work block and
 * schedule its callback.
 *
 * The ratio helper is reached through a call-via-register veneer, so it is a
 * typed indirect call rather than a call to a function at the branch target.
 * It takes exactly two arguments, numerator and denominator; r2 is never set
 * at the call site, so a third argument must not be added.
 */
typedef s32 (*ArmRatio)(s32 numerator, s32 denominator);

struct Work_080936a0 {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void ScheduleCallbackAfterFrames(const void *arg0, s32 arg1);
extern u8 gRom;

void BattleFx_ScheduleRatioTransition(s32 arg0, s32 arg1)
{
    struct Work_080936a0 *state = *(struct Work_080936a0 **)ADDR_03001E70;
    s32 handle;
    s32 result;

    handle = Runtime_AllocateBlock(27, 0xccc);
    if (*(s16 *)(handle + 414) != 3)
        return;
    {
        ArmRatio ratio = (ArmRatio)0x0300013c;
        result = ratio(arg0, 0x10000);
    }
    state->previous = state->current;
    state->current = result;
    state->kind = arg1;
    state->flags = 0;
    ScheduleCallbackAfterFrames(&gRom, 0xc94);
}

/* event/wait_for_display_field358_clear.c */
s32 WaitFrames(s32);

void Event_WaitForDisplayField358Clear(void)
{
  s32 cnt;
  void *work;
  u8 *p;
  work = *((void **)ADDR_03001E70);
  if ((*((s16 *)(((u8 *)Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E))) == 3)
  {
    cnt = 0;
    if ((*((s16 *)(((u8 *)work) + 0x358))) != 0)
    {
 do { loop_3: WaitFrames(1); cnt += 1; if (cnt <= 0x12B) { if ((*((s16 *)((p = (u8 *)work) + 0x358))) != 0) { goto loop_3; } } } while (0);
    }
  }
}
