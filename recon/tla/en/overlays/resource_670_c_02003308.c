// Matches: differ 0 (tla-en, 2026-11-01)
// The update of a whirling object in Kibombo: it circles its centre (targetX
// to targetZ) at the radius in unk4c, wobbling while its timer runs, through
// eight stages: it grows spinning to 1.5 size, spins until its timer ends
// (cue 220), rises to 200 pixels, widens to 40, waits ten frames, narrows
// while sinking to 136, shrinks spinning, and is removed with its sprite's
// VRAM entry.
// Three forms carry the match and want a reviewer's eye; none is tagged.
// - union Whirl: stage and timer sit at 0x62 and 0x63, inside Actor's callback,
//   so this object's bytes overlay Actor. Reading its fields through the union
//   gives them the alias sets the listing's schedule shows: no load passes
//   the store to action, and unk4c's store stays ahead of the sprite's load.
//   Without the union differ is 24 (all load order). The type is inferred, and
//   no other function is known to use it.
// - object is set once, after the test, with one address for its whole life.
//   Set at the top or dropped, GCSE's PRE shares 0x64 and 0x66 across the test
//   and adds a block the listing lacks (differ 104).
// - radius, spin and turn are read before the call so r0 and r1 are busy at
//   the ldrsh scratches, which puts r4 in the reload set the listing uses.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/vector3.h"

s32 Math_Sine(s32 angle);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, struct Vector3 *v);
void Func_02005c9c(s32 cue);
void Resource_ResetEntry(s32 block);
void Func_02005a3c(struct Actor *object);

union Whirl {
  struct Actor actor;
  struct {
    u8 unused[0x62];
    u8 stage;
    u8 timer;
  } whirl;
};

void Func_02003308(struct Actor *actor) {
  s32 wobble = 0;
  union Whirl *object;
  u8 *timer = &((union Whirl *)actor)->whirl.timer;
  s16 *action;
  u8 *stage;
  struct Vector3 centre;
  s32 radius;
  s32 spin;
  s32 turn;

  if (*timer != 0)
    wobble = Math_Sine((actor->action + (s16)actor->unk66) * 9 << 9) * 6;
  object = (union Whirl *)actor;
  action = &object->actor.action;
  centre.x = object->actor.targetX;
  centre.y = object->actor.targetY;
  centre.z = object->actor.targetZ;
  radius = object->actor.unk4c;
  spin = (s16)object->actor.unk66;
  turn = *action;
  Vector_AddPolarOffsetFar((radius << 16) + wobble, turn * 3 * 256 + spin,
                           &centre);
  object->actor.x = centre.x;
  object->actor.y = centre.y;
  object->actor.z = centre.z;
  stage = &object->whirl.stage;
  switch (*stage) {
  case 0:
    *action += 4;
    object->actor.unk4c = 1;
    ((struct FieldSprite *)object->actor.animation)->rotation += 0x1000;
    object->actor.scaleX += 0x666;
    object->actor.scaleY += 0x666;
    if (object->actor.scaleX > 0x18000 &&
        ((struct FieldSprite *)object->actor.animation)->rotation == 0) {
      *stage = 1;
      *timer = 1;
    }
    break;
  case 1:
    *action += 4;
    ((struct FieldSprite *)object->actor.animation)->rotation += 0x1000;
    if (((struct FieldSprite *)object->actor.animation)->rotation == 0 &&
        --*timer == 0) {
      Func_02005c9c(220);
      *stage = 2;
    }
    break;
  case 2:
    *action += 1;
    object->actor.targetY += 0x40000;
    if (object->actor.targetY > 0x640000)
      *stage = 3;
    break;
  case 3:
    *action += 1;
    object->actor.unk4c += 2;
    *timer = 10;
    if (object->actor.unk4c > 40)
      *stage = 4;
    break;
  case 4:
    *action += 1;
    if (--*timer == 0)
      *stage = 5;
    break;
  case 5:
    *action += 1;
    object->actor.unk4c -= 2;
    if (object->actor.targetY < 0x880000)
      object->actor.targetY += 0x20000;
    if (object->actor.unk4c <= 1) {
      object->actor.unk4c = 0;
      *stage = 6;
    }
    break;
  case 6:
    *action += 4;
    ((struct FieldSprite *)object->actor.animation)->rotation += 0x1000;
    object->actor.scaleX += -0x1000;
    object->actor.scaleY += -0x1000;
    if (object->actor.scaleX < 0x1200) {
      object->actor.targetX = 100;
      object->actor.targetZ = 100;
      *stage = 7;
    }
    break;
  case 7:
    Resource_ResetEntry(((struct FieldSprite *)object->actor.animation)->vramBlock);
    Func_02005a3c(&object->actor);
    break;
  }
}
