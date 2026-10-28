// Draft, differ 146 of 228: the update of a whirling object in Kibombo: it
// circles its centre (targetX to targetZ) at the radius in unk4c, wobbling
// while its timer (byte 0x63) runs, through eight stages (byte 0x62): it
// grows spinning to 1.5 size, spins until its timer ends (cue 220), rises
// to 200 pixels, widens to 40, waits ten frames, narrows while sinking to
// 136, shrinks spinning, and is removed with its sprite's VRAM entry.
// Left: the native computes the wobble's field addresses inside its test and the action field's address after it (this draft shares them across the test), keeps the timer's address in r8 and the stage's in r5, and reads the sprite afresh in each stage.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/vector3.h"

s32 Math_Sine(s32 angle);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, struct Vector3 *v);
void Func_02005c9c(s32 cue);
void Resource_ResetEntry(s32 block);
void Func_02005a3c(struct Actor *object);

void Func_02003308(struct Actor *object) {
  u8 *timer = (u8 *)object + 0x63;
  u8 *stage;
  struct Vector3 centre;
  struct FieldSprite *sprite;
  s32 wobble = 0;

  if (*timer != 0)
    wobble = Math_Sine((object->action + (s16)object->unk66) * 9 << 9) * 6;
  centre.x = object->targetX;
  centre.y = object->targetY;
  centre.z = object->targetZ;
  Vector_AddPolarOffsetFar((object->unk4c << 16) + wobble,
                           object->action * 3 * 256 + (s16)object->unk66,
                           &centre);
  object->x = centre.x;
  object->y = centre.y;
  object->z = centre.z;
  stage = (u8 *)object + 0x62;
  switch (*stage) {
  case 0:
    object->action += 4;
    object->unk4c = 1;
    sprite = object->animation;
    sprite->rotation += 0x1000;
    object->scaleY += 0x666;
    object->scaleX += 0x666;
    if (object->scaleX > 0x18000 &&
        ((struct FieldSprite *)object->animation)->rotation == 0) {
      *stage = 1;
      *timer = 1;
    }
    break;
  case 1:
    object->action += 4;
    ((struct FieldSprite *)object->animation)->rotation += 0x1000;
    if (((struct FieldSprite *)object->animation)->rotation == 0 &&
        --*timer == 0) {
      Func_02005c9c(220);
      *stage = 2;
    }
    break;
  case 2:
    object->action += 1;
    object->targetY += 0x40000;
    if (object->targetY > 0x640000)
      *stage = 3;
    break;
  case 3:
    object->action += 1;
    object->unk4c += 2;
    *timer = 10;
    if (object->unk4c > 40)
      *stage = 4;
    break;
  case 4:
    object->action += 1;
    if (--*timer == 0)
      *stage = 5;
    break;
  case 5:
    object->action += 1;
    object->unk4c -= 2;
    if (object->targetY < 0x880000)
      object->targetY += 0x20000;
    if (object->unk4c <= 1) {
      object->unk4c = 0;
      *stage = 6;
    }
    break;
  case 6:
    object->action += 4;
    ((struct FieldSprite *)object->animation)->rotation += 0x1000;
    object->scaleX += -0x1000;
    object->scaleY += -0x1000;
    if (object->scaleX < 0x1200) {
      object->targetX = 100;
      object->targetZ = 100;
      *stage = 7;
    }
    break;
  case 7:
    Resource_ResetEntry(((struct FieldSprite *)object->animation)->vramBlock);
    Func_02005a3c(object);
    break;
  }
}
