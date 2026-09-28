/* NONMATCHING: resource_3b0 0x02008030, Object_PlaceFromCameraOffset, formerly
 * FIELD/FUNE_HOBASHIRA/CAMERA_OFFSET.C (2026-09-28).
 * Reads the camera through 0x03001e70 (the main image names it gCam and
 * gMapWork) and the two coordinate pairs at 0x02009930 and 0x02009938, which lie
 * just past the loaded image in the overlay's own work RAM. The main image has
 * no name there and the overlay defines none, so those references cannot be
 * named honestly yet. Remaining: name the overlay's work RAM. */
#include "TYPES.H"

struct Other {
    u8 filler00[30];
    u16 x;
};

struct Object {
    u8 filler00[8];
    s32 x;
    s32 z;
    u8 filler10[64];
    struct Other *other;
};

extern s32 **Data_03001e70;
extern s32 Data_02009938[];
extern s32 Data_02009930[];

s32 Object_PlaceFromCameraOffset(struct Object *object)
{
    s32 *position = *Data_03001e70;
    s32 *origin = Data_02009938;
    s32 *center = Data_02009930;
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
