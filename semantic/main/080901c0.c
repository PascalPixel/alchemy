#include "layout_guard.h"
#include "types.h"

struct FieldTransition_080901c0 {
    u8 padding000[0x528];
    u16 variant;
    u16 value;
    u8 padding52c[8];
    u16 limit;
    u16 enabled;
    u8 padding538[2];
    s8 start;
    s8 end;
    s8 duration;
    s8 step;
};

LAYOUT_OFFSET_GUARD(
    FieldTransition080901c0_Variant,
    struct FieldTransition_080901c0,
    variant,
    0x528);
LAYOUT_OFFSET_GUARD(
    FieldTransition080901c0_Value,
    struct FieldTransition_080901c0,
    value,
    0x52a);
LAYOUT_OFFSET_GUARD(
    FieldTransition080901c0_Start,
    struct FieldTransition_080901c0,
    start,
    0x53a);
LAYOUT_OFFSET_GUARD(
    FieldTransition080901c0_Step,
    struct FieldTransition_080901c0,
    step,
    0x53d);

struct FieldTransition_080901c0 *Func_0808fecc(void);
void Func_080907b0(s32);
void Func_08003bb4(s32);
void Func_08003b70(s32);
void Func_08091200(s32, s32);
void Func_08091254(s32);
void Func_080041d8(const void *, s32);
void Func_0800307c(s32, s32, const void *);
void Func_080030f8(s32);

static void StartInterpolation_080901c0(
    struct FieldTransition_080901c0 *transition,
    s8 start,
    s8 end,
    s32 duration)
{
    transition->start = start;
    transition->end = end;
    transition->duration = (s8)duration;
    transition->step = 0;
}

/*
 * Start one of five field-transition profiles encoded in the high byte of
 * `effect`.  The low byte selects a profile variant and `frames` supplies the
 * duration recorded for profiles that use the shared linear interpolator.
 */
void Func_080901c0(s32 effect, s32 frames)
{
    u16 profile = (u16)((effect >> 8) & 0xff);
    u16 variant = (u16)(effect & 0xff);
    struct FieldTransition_080901c0 *transition;

    switch (profile) {
    case 0:
        Func_08003bb4(0);
        Func_08003b70(frames);
        break;

    case 1:
        Func_08091200(0x8000, 0);
        Func_08091254(frames);
        break;

    case 2:
        transition = Func_0808fecc();
        transition->variant = variant;
        transition->value = 0x20;
        transition->limit = 0x3f;
        transition->enabled = 1;
        Func_080041d8((const void *)0x0808f52d, 0x0c80);
        Func_080041d8((const void *)0x0808f499, 0x0480);
        Func_080030f8(1);
        StartInterpolation_080901c0(transition, 0x20, 0x40, frames);
        break;

    case 3:
        transition = Func_0808fecc();
        transition->variant = variant;
        transition->value = 0x20;
        Func_080907b0(0);
        Func_080030f8(1);
        Func_080041d8((const void *)0x08090659, 0x0c80);
        StartInterpolation_080901c0(transition, 0x20, 0x40, frames);
        break;

    case 4:
        transition = Func_0808fecc();
        if (variant == 0)
            Func_080041d8((const void *)0x080903bd, 0x0c80);
        else
            Func_080041d8((const void *)0x08090489, 0x0c80);
        Func_0800307c(1, 0, (const void *)0x08090585);
        StartInterpolation_080901c0(transition, 0, 0x50, frames);
        break;
    }
}
