#include "types.h"
#include "fixed_math.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 ArcTan2(s32, s32);

s32 ObjectMotion_TurnTowardLinkedTarget(void *object) {
    s16 turn_step;
    u16 current_angle;
    s32 target_angle;
    void *linked_target;

    linked_target = FIELD_AT_OFFSET(object, void *, 0x68);
    if (linked_target != NULL) {
        FIELD_AT_OFFSET(object, u8, 0x5A) = (u8) (0xFE & FIELD_AT_OFFSET(object, u8, 0x5A));
        target_angle = (u16) ArcTan2(FIELD_AT_OFFSET(linked_target, s32, 0x10) - FIELD_AT_OFFSET(object, s32, 0x10), FIELD_AT_OFFSET(linked_target, s32, 8) - FIELD_AT_OFFSET(object, s32, 8));
        current_angle = FIELD_AT_OFFSET(object, u16, 6);
        turn_step = target_angle - current_angle;
        if (turn_step != 0) {
            if (turn_step > 0x1000) {
                turn_step = 0x1000;
            }
            if (turn_step < -0x1000) {
                turn_step = -0x1000;
            }
            FIELD_AT_OFFSET(object, u16, 6) = (u16) (current_angle + turn_step);
        }
    }
    return 1;
}
