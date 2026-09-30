#include "TYPES.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

struct State_080935b0 {
    u8 filler0[0xEC];
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

extern struct State_080935b0 *gCam;

extern u8 Data_03001af4[];
extern u8 *gMapWork;
u8 *Runtime_AllocateBlock(s32 kind, s32 size);
void Scheduler_RemoveCallback(s32 (*callback)(void));
s32 BattleFx_StepRatioTransition(void);

struct Work_080936a0 {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

void Scheduler_AddOrUpdateCallback(const void *arg0, s32 arg1);

s32 BattleFx_StepRatioTransition(void);

void Map_SetWorkFourValues(s32 first, s32 second, s32 third, s32 fourth)
{
    struct State_080935b0 *work = gCam;

    work->first = first;
    work->second = second;
    work->third = third;
    work->fourth = fourth;
}

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
    *(u32 *)Data_03001af4 = *(u16 *)(work + 0x118) + 1;
    if (*step == *duration) {
        *duration = 0;
        Scheduler_RemoveCallback(BattleFx_StepRatioTransition);
    }
}

/*
 * Record a ratio-driven transition on the battle effect work block and
 * schedule its callback.
 */

/* the transition callback, Thumb address */
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
