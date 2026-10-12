/* Draft of WorldMap_UpdateBobbingMarker (overlay 64B, section .text.x020082c4; diff it with
 * --symbol WorldMap_UpdateBobbingMarker), from the twin of the same name in The Broken
 * Seal's world map. differ 4 of 56, two reordered pairs:
 *  - the native copies the radius (adds r0, r4, #0) right after the y and z stores, before
 *    the angle is read for the polar call; here it is the last argument set up. In sched2
 *    the copy has the lowest priority of the three arguments, so the native had it ahead of
 *    a scheduling barrier;
 *  - at the end the native starts 0x400 (movs r2, #128) before it reads the angle again;
 *    here the read comes first (equal priority, so the constant came first in its RTL).
 * Measured on the way from differ 14:
 *  - x, y and z as plain s32 fields: the y and z stores sink below the angle read. As the
 *    fixed/pixel union they alias the s16 read and stay above it;
 *  - the multiply as a direct Iwram_MulQ16 call: both halves of 0x40000 come before the
 *    load of its address; through the inline the load falls between them, as the native;
 *  - y and z as one statement each: 0x100000 is built before the z constant is loaded; with
 *    the two forced temporaries and the loop note the load comes first, as the native;
 *  - without the second loop note the x store sinks below the angle address;
 *  - against the radius copy: the polar call through an inline forwarder, the radius as a
 *    forced temporary before or after the loop note, a loop note inside the angle argument
 *    (no change), an in-out asm barrier on the radius (differ 16);
 *  - against the last pair: the step as a forced temporary (no change).
 * Every device below needs its FAKEMATCH tag when this is adopted. */
#include "types.h"
#include "iwram_call.h"

// 16.16 固定小数点の座標
union FieldCoordinate {
    s32 fixed;
    struct {
        u16 fraction;
        s16 pixel;
    } part;
};

struct BobbingMarker {
    u8 unknown_00[6];
    u16 facing;
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unknown_14[80];
    s16 angle;
    s16 stopped;
};

extern u32 gFrameCount;

s32 Math_Sine(s32 angle);
void Animation_ApplyChildValues(struct BobbingMarker *marker, s32 value);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, union FieldCoordinate *position);

static inline s32 MulQ16(s32 a, s32 b)
{
    return Iwram_MulQ16(a, b);
}

// 目印を点滅させ、円を描きながら上下に揺らす
void WorldMap_UpdateBobbingMarker(struct BobbingMarker *marker)
{
    s16 *angle;
    s32 y;
    s32 z;

    if (gFrameCount & 2)
        Animation_ApplyChildValues(marker, 10);
    else
        Animation_ApplyChildValues(marker, 7);
    if (marker->stopped == 0) {
        angle = &marker->angle;
        marker->x.fixed = 0x27880000;
        y = MulQ16(Math_Sine(*angle << 3), 0x40000);
        z = 0x20a40000;
        do { } while (0);
        marker->y.fixed = y + 0x100000;
        marker->z.fixed = z;
        do { } while (0);
        Vector_AddPolarOffsetFar(0x100000, *angle, &marker->x);
        marker->facing = *angle + 0x4000;
        *angle += 0x400;
    }
}
