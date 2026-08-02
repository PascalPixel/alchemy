#include "layout_guard.h"
#include "types.h"

/* 0x03000380 is the IWRAM-relocated signed divide routine. */
typedef s32 (*IwramDivide_08090488)(s32 numerator, s32 denominator);

struct FieldTransition_08090488 {
    u8 padding000[0x52a];
    u16 value;
    u8 padding52c[0x0e];
    s8 start;
    s8 end;
    s8 duration;
    s8 step;
};

struct DisplayCoefficients_08090488 {
    u8 padding000[0x100];
    s16 first;
    s16 second;
};

LAYOUT_OFFSET_GUARD(
    FieldTransition08090488_Value,
    struct FieldTransition_08090488,
    value,
    0x52a);
LAYOUT_OFFSET_GUARD(
    FieldTransition08090488_Start,
    struct FieldTransition_08090488,
    start,
    0x53a);
LAYOUT_OFFSET_GUARD(
    FieldTransition08090488_Duration,
    struct FieldTransition_08090488,
    duration,
    0x53c);
LAYOUT_OFFSET_GUARD(
    DisplayCoefficients08090488_First,
    struct DisplayCoefficients_08090488,
    first,
    0x100);

extern struct FieldTransition_08090488 *Data_03001ecc;
extern struct DisplayCoefficients_08090488 *Data_03001e70;
extern s32 Data_03001e40;

void Func_08004278(const void *);
void Func_0800307c(s32, s32, const void *);

/*
 * Advance the shared field-transition interpolator and publish its display
 * coefficients.  The divide is an ordinary indirect C call to the relocated
 * ARM helper; no embedded assembly or synthetic veneer is needed.
 */
void Func_08090488(void)
{
    struct FieldTransition_08090488 *transition = Data_03001ecc;
    struct DisplayCoefficients_08090488 *display = Data_03001e70;
    u16 value;

    if (transition->duration != 0) {
        if (transition->step >= transition->duration) {
            transition->duration = 0;
            Func_08004278((const void *)0x08090489);
            Func_0800307c(1, 0, 0);
            return;
        }

        transition->step = (s8)((u8)transition->step + 1);
        transition->value = (u16)(
            transition->start +
            ((IwramDivide_08090488)0x03000380)(
                transition->step * (transition->end - transition->start),
                transition->duration));
    }

    value = transition->value;
    if (value > 0x4f) {
        display->first = 0xc8;
        display->second = 0xfa;
    } else if (value != 0 && (Data_03001e40 & 1) != 0) {
        display->first = (s16)(value + 0x50);
        display->second = (s16)(0x50 - value);
    } else {
        display->first = 0;
        display->second = 0x9f;
    }
}
