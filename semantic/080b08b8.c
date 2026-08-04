#include "layout_guard.h"
#include "types.h"

typedef struct DisplayObject_080b08b8 {
    u8 padding00[6];
    u16 x;
    u16 y;
    u8 padding0a[0x0a];
    u8 screen_y;
    u8 padding15;
    u16 attr1;
} DisplayObject_080b08b8;

typedef struct PositionTween_080b08b8 {
    DisplayObject_080b08b8 *object;
    s16 start_x;
    s16 start_y;
    s16 end_x;
    s16 end_y;
    s8 step;
    s8 duration;
} PositionTween_080b08b8;

LAYOUT_OFFSET_GUARD(
    DisplayObject080b08b8_X,
    DisplayObject_080b08b8,
    x,
    6);
LAYOUT_OFFSET_GUARD(
    DisplayObject080b08b8_ScreenY,
    DisplayObject_080b08b8,
    screen_y,
    0x14);
LAYOUT_OFFSET_GUARD(
    DisplayObject080b08b8_Attr1,
    DisplayObject_080b08b8,
    attr1,
    0x16);
LAYOUT_OFFSET_GUARD(
    PositionTween080b08b8_Step,
    PositionTween_080b08b8,
    step,
    0x0c);
LAYOUT_OFFSET_GUARD(
    PositionTween080b08b8_Duration,
    PositionTween_080b08b8,
    duration,
    0x0d);

s32 Func_080022ec(s32 dividend, s32 divisor);

/* Advance one frame of a display object's signed linear position tween. */
void Func_080b08b8(PositionTween_080b08b8 *tween)
{
    DisplayObject_080b08b8 *object;
    s32 step;
    s32 x;
    s32 y;

    if (tween == 0 || tween->duration == 0)
        return;

    object = tween->object;
    tween->step++;
    step = tween->step;

    x = (u16)tween->start_x + Func_080022ec(
        step * (tween->end_x - tween->start_x), tween->duration);
    object->x = (u16)x;
    object->attr1 =
        (object->attr1 & 0xfe00) | (x & 0x01ff);

    y = (u16)tween->start_y + Func_080022ec(
        step * (tween->end_y - tween->start_y), tween->duration);
    object->y = (u16)y;
    object->screen_y = (u8)y;

    if (step == tween->duration) {
        tween->duration = 0;
        tween->step = 0;
    }
}
