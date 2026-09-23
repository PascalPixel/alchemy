/* Draft, not exact (2026-09-24): 1 differing halfword, 204 of 204 bytes.
   Residual: "adds r1, r2, r1" where the reference has "adds r1, r1, r2"
   for the eased offset plus the start value. Returns int: the epilogue
   returns through r1. */

#include "IWRAM_CALL.H"

extern u8 *Data_03001e70;

u8 *Runtime_AllocateBlock(s32 kind, s32 size);
s32 Func_080022ec(s32 dividend, s32 divisor);
void Scheduler_RemoveCallback(s32 (*callback)(void));

s32 Func_080935d4(void)
{
    u8 *work;
    s16 *duration;
    s32 *from;
    s16 *step;
    s32 offset;
    s32 delta;

    work = Data_03001e70;
    if ((*(u8 **)(Runtime_AllocateBlock(27, 0xccc) + 480))[91] != 0)
        return;
    duration = (s16 *)(work + 0x358);
    if (*duration == 0)
        return;
    from = (s32 *)(work + 0x350);
    delta = *(s32 *)(work + 0x354) - *from;
    step = (s16 *)(work + 0x35a);
    (*step)++;
    offset = Func_080022ec(delta * *step, *duration);
    offset += *from;
    *(s32 *)(work + 0x34c) = Iwram_MulQ16(*(s32 *)(work + 0x348), offset);
    *(u32 *)0x03001af4 = *(u16 *)(work + 0x118) + 1;
    if (*step == *duration) {
        *duration = 0;
        Scheduler_RemoveCallback(Func_080935d4);
    }
}
