#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/vector3.h"
#include "event_inline.h"

s32 Resource_ResetEntry(u32 slot);
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
    wobble = Math_Sine((actor->timer + (s16)actor->unk66) * 9 << 9) * 6;
  object = (union Whirl *)actor;
  action = &object->actor.timer;
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
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    object->actor.scaleX += 0x666;
    object->actor.scaleY += 0x666;
    if (object->actor.scaleX > 0x18000 &&
        ((struct FieldSprite *)object->actor.sprite)->rotation == 0) {
      *stage = 1;
      *timer = 1;
    }
    break;
  case 1:
    *action += 4;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    if (((struct FieldSprite *)object->actor.sprite)->rotation == 0 &&
        --*timer == 0) {
      Sound_Play(220);
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
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    object->actor.scaleX += -0x1000;
    object->actor.scaleY += -0x1000;
    if (object->actor.scaleX < 0x1200) {
      object->actor.targetX = 100;
      object->actor.targetZ = 100;
      *stage = 7;
    }
    break;
  case 7:
    Resource_ResetEntry(
        ((struct FieldSprite *)object->actor.sprite)->vramBlock);
    Func_02005a3c(&object->actor);
    break;
  }
}
