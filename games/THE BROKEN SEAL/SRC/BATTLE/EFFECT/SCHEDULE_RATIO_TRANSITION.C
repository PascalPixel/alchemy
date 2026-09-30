#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
extern u8 gCam[];

/*
 * Record a ratio-driven transition on the battle effect work block and
 * schedule its callback.
 */

struct Work_080936a0 {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Scheduler_AddOrUpdateCallback(const void *arg0, s32 arg1);
extern u8 BattleFx_StepRatioTransition[]; /* the transition callback, Thumb address */

void BattleFx_ScheduleRatioTransition(s32 arg0, s32 arg1)
{
    struct Work_080936a0 *state = *(struct Work_080936a0 **)((u32)&gCam);
    s32 handle;
    s32 result;

    handle = Runtime_AllocateBlock(27, 0xccc);
    if (*(s16 *)(handle + 414) != 3)
        return;
    {
        s32 (*ratio)(s32, s32) = Iwram_RatioMulQ14;
        result = ratio(arg0, 0x10000);
    }
    state->previous = state->current;
    state->current = result;
    state->kind = arg1;
    state->flags = 0;
    Scheduler_AddOrUpdateCallback(BattleFx_StepRatioTransition, 0xc94);
}
