#include "IWRAM_CALL.H"

extern u8 *gMapWork;

u8 *Runtime_AllocateBlock(s32 kind, s32 size);
s32 Math_Div(s32 dividend, s32 divisor);
void Scheduler_RemoveCallback(s32 (*callback)(void));
s32 BattleFx_StepRatioTransition(void);

/* Eases the ratio at work + 0x34c from the start to the end value over the
   transition's duration, one step per frame, then unschedules itself.
   Declared int: the reference returns through r1, with no value. */
s32 BattleFx_StepRatioTransition(void)
{
    u8 *work;
    s16 *duration;
    s32 *from;
    s16 *step;
    s32 offset;
    s32 delta;

    work = gMapWork;
    if ((*(u8 **)(Runtime_AllocateBlock(27, 0xccc) + 480))[91] != 0)
        return;
    duration = (s16 *)(work + 0x358);
    if (*duration == 0)
        return;
    from = (s32 *)(work + 0x350);
    delta = *(s32 *)(work + 0x354) - *from;
    step = (s16 *)(work + 0x35a);
    (*step)++;
    offset = *from + Math_Div(delta * *step, *duration);
    *(s32 *)(work + 0x34c) = Iwram_MulQ16(*(s32 *)(work + 0x348), offset);
    *(u32 *)0x03001af4 = *(u16 *)(work + 0x118) + 1;
    if (*step == *duration) {
        *duration = 0;
        Scheduler_RemoveCallback(BattleFx_StepRatioTransition);
    }
}
