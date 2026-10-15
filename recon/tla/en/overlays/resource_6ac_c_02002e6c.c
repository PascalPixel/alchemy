/* Near miss, overlay 6AC Func_02002e6c (0x0200ae6c): differ 12 of 86, all scheduling. The native
 * starts both calls' first arguments early (movs r0, #128 and adds r1, r6 between the position's
 * stores; adds r0, r7 ahead of the three position loads), stores the animation's angle before
 * the scale is cleared, and builds the action's address (adds r3, #100) after the zero's copy.
 * The byte zero is a pool word in r10 in both. An inline position helper changes nothing;
 * six minutes of alchemy permute reached differ 8 (a one-pass block from the angle store to the
 * scale stores) at the cost of six registers. */
#include "../../../../games/tla/src/scene/mars_room_m/mars_room_m.h"

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void Func_02002ae8(void);

void Func_02002e6c(struct ObjectRuntime *self)
{
    struct ObjectRuntime *object = Object_GetById(((u8 *)self)[0x62]);
    s32 angle = (s16)self->unknown_66;
    struct MarsRoomMAnimation *animation = object->animation;
    s32 position[3];

    position[0] = self->x;
    position[1] = self->y - 0xc0000;
    position[2] = self->z;
    Vector_AddPolarOffsetFar(0x200000, angle, position);
    object->angle = angle;
    object->motion_flags = 0;
    Object_SetPositionAndResetMotion(object, position[0], position[1], position[2]);
    object->update = Func_02002ae8;
    object->action_flags = 0;
    object->speed_limit = 0x80000;
    object->acceleration = 0x2000;
    animation->angle = angle - 0x4000;
    object->scale_x = 0;
    object->scale_y = 0;
    object->action = 0;
    object->linked_object = self->linked_object;
    Func_020032b8(104);
}
