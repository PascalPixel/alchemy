// Matches: differ 0 of 165 (tla-en, 2026-10-30), with three tagged barriers.
// Overlay 6ad's Func_02001bcc: calls Func_02005598 for actors 24, 23, 9
// and 10, zeroes actor 24's scaleX, sets 23, 9 and 10 to their terrain height
// with motion flags 0, then with flag 0xa0a raises 9, raises 23, moves its x
// back and turns its sprite a 32nd of a turn (child values 4 for actors 9 and
// 11); with flag 0xa0b the same for 10, x forward and turned back (10 and
// 12); with both, raises all three further and sets their motion flags to 4.
// Actor 23's x is read through FieldCoordinate (union FieldCoordinate, as
// The Broken Seal's actors keep their coordinates): its accesses then alias
// everything, so the sprite rotation's load waits for the x store and its
// pointer is copied out of r8 after the store, as the native has it; with
// plain s32 coordinates both flag branches copy it first (differ 4).
// What the barriers steer: the native loads each rise for actor 23 from the
// pool before 23's y, where alchemy-gcc's -mthumb-slow-loads lets the y load
// (which opens a read-modify-write) go first after the store before it.
// The earlier draft (differ 4 of 165, 26 October) pinned each rise to r2 as
// well; the pins are no longer needed.
#include "types.h"
#include "call.h"
#include "field_coordinate.h"
#include "field_sprite.h"

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

struct PositionedActor *Actor_GetById(s32 id);
void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);
void Func_02005598(s32 id, s32 a1, s32 a2);
s32 GameFlag_Test(s32 flag);

void Func_02001bcc(void) {
  struct PositionedActor *actor9 = Actor_GetById(9);
  struct PositionedActor *actor10 = Actor_GetById(10);
  struct PositionedActor *actor23 = Actor_GetById(23);
  struct FieldSprite *sprite = actor23->animation;

  Call3((void (*)())Func_02005598, 24, 0x2000000, 0xf80000);
  Call3((void (*)())Func_02005598, 23, 0x2000000, 0xf80000);
  Call3((void (*)())Func_02005598, 9, 0x1d80000, 0x1000000);
  Call3((void (*)())Func_02005598, 10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Value1((s32 (*)())GameFlag_Test, 0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      /* FAKEMATCH(barrier): step held apart from the y load (and r3 kept
       * from it) loads the rise before actor 23's y; as actor23->y.fixed +=
       * 0x1eb80, 2 of 165 instructions differ in tla-en. */
      __asm__("" : "+l"(step) : : "r3");
      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_GetById(9), 4);
    Animation_ApplyChildValues(Actor_GetById(11), 4);
  }
  if (Value1((s32 (*)())GameFlag_Test, 0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      /* FAKEMATCH(barrier): as above; as actor23->y.fixed += 0x1eb80, 2 of
       * 165 instructions differ in tla-en. */
      __asm__("" : "+l"(step) : : "r3");
      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_GetById(10), 4);
    Animation_ApplyChildValues(Actor_GetById(12), 4);
  }
  if (Value1((s32 (*)())GameFlag_Test, 0xa0a) &&
      Value1((s32 (*)())GameFlag_Test, 0xa0b)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      /* FAKEMATCH(barrier): as above; as actor23->y.fixed += 0x1b8480, 2 of
       * 165 instructions differ in tla-en. */
      __asm__("" : "+l"(step) : : "r3");
      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}
