#include "IMIRU.H"

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj)
{
    switch (obj->counter) {
    case 6:
        obj->x += 0xffffc000;
        obj->z += 0x2000;
        break;
    case 4:
        obj->x += 0x2000;
        obj->z -= 0x1000;
        break;
    case 2:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        break;
    case 0:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        if (obj->mode != 0) {
            obj->counter = Engine_MathModulo(Random_Next(), 40) + 40;
        } else {
            obj->counter = Engine_MathModulo(Random_Next(), 20) + 20;
        }
        break;
    }
    obj->counter--;
    return 1;
}

s32 OverlayObject_UpdateFacingTowardTarget(void *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    void *target;
    target = (*(void * *)((u8 *)(obj) + (0x68)));
    if (target != NULL) {
        (*(u8 *)((u8 *)(obj) + (0x5A))) = (u8)(0xFE & (*(u8 *)((u8 *)(obj) + (0x5A))));
        angle = (u16)ArcTan2((*(s32 *)((u8 *)(target) + (0x10))) - (*(s32 *)((u8 *)(obj) + (0x10))), (*(s32 *)((u8 *)(target) + (8))) - (*(s32 *)((u8 *)(obj) + (8))));
        old = (*(u16 *)((u8 *)(obj) + (6)));
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) delta = 0x1000;
            if (delta < -0x1000) delta = -0x1000;
            (*(u16 *)((u8 *)(obj) + (6))) = (u16)(old + delta);
        }
    }
    return 1;
}
