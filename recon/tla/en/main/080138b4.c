/* Current ordinary input repeat/direction trial, uncredited.
 * EN: score1205,37 differing aligned instructions. Native232 bytes;
 * broad allocation, branch and field re-read residual remains. The current
 * VBlank handler writes other record fields, not filtered_repeat or
 * selected_direction; their volatility has no independent justification.
 * No artificial read, volatility, barrier or register device was added. */
/* Preserved against the maintained InputState: direction is the former
 * filtered_repeat field, previous_direction the former selected_direction.
 * The recorded score predates this field-name reconciliation. */
#include "INPUT.H"

void Input_UpdateKeyRepeatAndDirection(void)
{
    u32 held;
    u32 repeat;
    u32 count;
    u32 mask = 0;
    s32 timer;

    held = gInput.held;
    timer = gInput.repeat_timer;
    if (timer <= 0) {
        repeat = held;
        gInput.repeat = held;
        if (timer == 0)
            gInput.repeat_timer = 6;
        else
            gInput.repeat_timer = 19;
    } else {
        repeat = 0;
        gInput.repeat = 0;
    }
    if (repeat != 0) {
        count = 0;
        if (repeat & 0x40) count = 1;
        if (repeat & 0x80) count++;
        if (repeat & 0x20) count++;
        if (repeat & 0x10) count++;
        gInput.direction = repeat;
        switch (count) {
        case 0:
            gInput.previous_direction = 0x30;
            break;
        case 1:
            gInput.previous_direction = repeat & 0xf0;
            break;
        case 2:
            if ((gInput.previous_direction & gInput.direction) == 0)
                gInput.previous_direction = 0x30;
            gInput.direction &= gInput.previous_direction ^ 0xffff;
            break;
        case 3:
            if (gInput.previous_direction & 0x30) mask = 0x30;
            if (gInput.previous_direction & 0xc0) mask = 0xc0;
            gInput.previous_direction = repeat & (mask ^ 0xffff);
            gInput.direction &= mask ^ 0xffff;
            break;
        default:
            gInput.previous_direction = 0x30;
            gInput.direction &= 0xff0f;
            break;
        }
    } else {
        gInput.direction = repeat;
    }
    gInput.pressed = held & ~gInput.previous_held;
    gInput.previous_held = held;
}
