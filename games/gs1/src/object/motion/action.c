#include "object_runtime.h"

u16 ArcTan2(s32, s32);
void *Object_GetById(u32);
void Object_SetCallback(void *, void *);
extern s16 Data_02000240[];
extern const u8 Data_0809fe00[];
extern const u8 Data_0809fd44[];
extern const u8 Data_0809fe10[];
extern const u8 Data_0809fecc[];
extern const u8 Data_0809ff18[];
extern const u8 Data_0809ff2c[];
extern const u8 Data_0809fe04[];

s32 FixedPoint_Ratio(s32, s32);
s32 Object_SetPosition(s32, s32, s32, s32);
s32 Object_SetMode(s32, s32);

s32 ObjectMotion_MoveTowardTarget(s32 arg0)
{
    s32 object;
    void *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;

    object = arg0;
    target = *(void **)(object + 0x68);
    if (target != 0) {
        deltaX = *(s32 *)(target + 8) - *(s32 *)(object + 8);
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = *(s32 *)(target + 0x10) - *(s32 *)(object + 0x10);
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = ((s32 (*)(s32))0x030001d8)(cellX * cellX + cellY * cellY);
        arg0 = *(s16 *)(object + 0x64);
        if (distance >= arg0) {
            newX = *(s32 *)(object + 8) +
                FixedPoint_Ratio(cellX << 20, arg0);
            Object_SetPosition(object, newX, *(s32 *)(object + 0x0c),
                          *(s32 *)(object + 0x10) +
                              FixedPoint_Ratio(cellY << 20, arg0));
            Object_SetMode(object, 2);
        } else {
            Object_SetMode(object, 1);
        }
    }
    return 1;
}

s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object)
{
    s16 turn_step;
    u16 current_angle;
    s32 target_angle;
    struct ObjectRuntime *target;

    target = object->linked_object;
    if (target != NULL) {
        object->action_flags &= 0xfe;
        target_angle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
        current_angle = *(u16 *)((u8 *)object + 6);
        turn_step = target_angle - current_angle;
        if (turn_step != 0) {
            if (turn_step > 0x1000)
                turn_step = 0x1000;
            if (turn_step < -0x1000)
                turn_step = -0x1000;
            *(u16 *)((u8 *)object + 6) = (u16)(current_angle + turn_step);
        }
    }
    return 1;
}

void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind)
{
    s32 index;

    switch ((u32)(kind - 1)) {
    case 0:
        kind = (s32)Data_0809fe00;
        break;
    case 1:
        kind = (s32)Data_0809fd44;
        break;
    case 2:
        kind = (s32)Data_0809fe10;
        break;
    case 3:
        kind = (s32)Data_0809fecc;
        break;
    case 4:
        kind = (s32)Data_0809ff18;
        break;
    case 5:
        index = 250;
        object->linked_object = Object_GetById(*(u32 *)&Data_02000240[index]);
        kind = (s32)Data_0809ff2c;
        break;
    case 6:
        kind = (s32)Data_0809fe04;
        break;
    default:
        break;
    }
    Object_SetCallback(object, (void *)kind);
}
