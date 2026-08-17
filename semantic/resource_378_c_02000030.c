#include "types.h"
#include "facing_object.h"
#define NULL ((void *)0)

s16 Func_02003588(s32, s32);
#define CalculateFacingAngle Func_02003588
struct FacingObject *Func_0200364e(s16);
#define ResolveFacingObject Func_0200364e

#define UpdateFacingFromResolvedObject Func_02000030
s32 UpdateFacingFromResolvedObject(struct FacingObject *object) {
    struct FacingObject *target;

    target = ResolveFacingObject(object->unknown_64);
    object->facing = CalculateFacingAngle(
        target->position_z - object->position_z,
        target->position_x - object->position_x
    );
    return 0;
}
