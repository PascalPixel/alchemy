#include "types.h"
#include "actor.h"
#include "iwram_call.h"
#include "field_coordinate.h"
#include "event_inline.h"

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

void Animation_ApplyChildValues(struct BobbingMarker *marker, s32 value);

static __inline__ s32 MulQ16(s32 a, s32 b) {
  /* FAKEMATCH: through the inline the multiply's address loads between the
   * halves of 0x40000, as in the native (called directly, both halves come
   * first). */
  return Iwram_MulQ16(a, b);
}

/// The marker blinks and bobs up and down on a circle.
// As The Broken Seal's WorldMap_UpdateBobbingMarker.
void WorldMap_UpdateBobbingMarker(struct BobbingMarker *marker) {
  s16 *angle;
  s32 y;
  s32 z;
  s32 radius;
  s32 rise;

  if (gFrameCount & 2)
    Animation_ApplyChildValues(marker, 10);
  else
    Animation_ApplyChildValues(marker, 7);
  if (marker->stopped == 0) {
    angle = &marker->angle;
    marker->x.fixed = 0x27880000;
    y = MulQ16(Math_Sine(*angle << 3), 0x40000);
    z = 0x20a40000;

    rise = 0x100000;
    marker->y.fixed = y + rise;
    marker->z.fixed = z;
    radius = rise;
    Vector_AddPolarOffsetFar(radius, *angle, &marker->x);
    marker->facing = *angle + 0x4000;
    *angle += 0x400;
  }
}

s32 StoryActor_Initialize(struct Actor *actor) {
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
