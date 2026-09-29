#include "FUNE.H"

struct Other {
    u8 filler00[30];
    u16 x;
};

extern s32 **gMapWork;

struct Object {
    u8 filler00[8];
    s32 x;
    s32 z;
    u8 filler10[64];
    struct Other *other;
};

/* Places the object where the camera has moved relative to the staging's
 * saved camera origin, around the saved centre. */
s32 Object_PlaceFromCameraOffset(struct Object *object)
{
    s32 *position = *gMapWork;
    s32 *origin = FuneHobashira_CameraOrigin;
    s32 *center = FuneHobashira_CameraCenter;
    s32 q0 = *position++;
    s32 q1 = *position;

    object->x = center[0] + (q0 - origin[0]);
    /* FAKEMATCH: the do/while keeps the z store ahead of the other load. */
    do {
        object->z = center[1] + (q1 - origin[1]) / 2;
    } while (0);
    object->other->x += 0x600;
    return 0;
}
