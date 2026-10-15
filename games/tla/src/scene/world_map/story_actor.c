#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "field_coordinate.h"

struct BobbingMarker {
    u8 unk00[6];
    u16 facing;
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unk14[80];
    s16 angle;
    s16 stopped;
};

extern u32 gFrameCount;

s32 GameFlag_Test(s32 flag);
s32 Math_Sine(s32 angle);
void Animation_ApplyChildValues(struct BobbingMarker *marker, s32 value);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, union FieldCoordinate *position);

static __inline__ s32 MulQ16(s32 a, s32 b)
{
    /* FAKEMATCH: through the inline the multiply's address loads between the halves of 0x40000, as in the native (called directly, both halves come first). */
    return Iwram_MulQ16(a, b);
}

// The marker blinks and bobs up and down on a circle, as The Broken Seal's
// WorldMap_UpdateBobbingMarker.
void WorldMap_UpdateBobbingMarker(struct BobbingMarker *marker)
{
    s16 *angle;
    s32 y;
    s32 z;
    s32 radius;
    s32 step;
    register s32 rise asm("r4"); /* FAKEMATCH: pinned, the rise is shared with the radius in r4 and the z constant takes r3, as in the native (unpinned the radius copy comes last again: differ 2). */

    if (gFrameCount & 2)
        Animation_ApplyChildValues(marker, 10);
    else
        Animation_ApplyChildValues(marker, 7);
    if (marker->stopped == 0) {
        angle = &marker->angle;
        marker->x.fixed = 0x27880000;
        y = MulQ16(Math_Sine(*angle << 3), 0x40000);
        z = 0x20a40000;
        /* FAKEMATCH: the one-pass block loads the z constant before 0x100000 is built, as in the native (without it differ 2). */
        do {
        } while (0);
        rise = 0x100000;
        marker->y.fixed = y + rise;
        marker->z.fixed = z;
        /* FAKEMATCH: after this one-pass block the radius, a forced temporary, is copied to its argument before the angle is read, as in the native (passing the rise itself, the copy is the last argument set up: differ 2). */
        do {
        } while (0);
        radius = rise;
        Vector_AddPolarOffsetFar(radius, *angle, &marker->x);
        marker->facing = *angle + 0x4000;
        /* FAKEMATCH: the one-pass block and the empty asm start the step 0x400 before the angle is read again, as in the native (plain, the read comes first: differ 2). */
        do {
        } while (0);
        step = 0x400;
        asm("" : "+r"(step));
        *angle += step;
    }
}

s32 StoryActor_Initialize(struct ObjectRuntime *actor)
{
    if (GameFlag_Test(0x37) != 0) {
        return 0;
    }
    if (GameFlag_Test(0x16E) != 0) {
        return 0;
    }
    actor->update = (void (*)(void))WorldMap_UpdateBobbingMarker;
    actor->motionFlags = 0;
    actor->action = 0;
    actor->unk66 = 0;
    actor->scaleX = 0x8000;
    actor->scaleY = 0x8000;
    return 0;
}
