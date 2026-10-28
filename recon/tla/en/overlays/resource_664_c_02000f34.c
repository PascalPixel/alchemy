// Draft, differ 63 of 153 (was 107): overlay 664's copy of overlay 663's
// Func_02001b54 (the same listing, with Func_02000d48 as its shots' update),
// whose draft holds its notes.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "system/field_effect.h"
#include "system/vector3.h"

s32 GameFlag_Test(s32 flag);
void Func_02005f8c(struct ObjectRuntime *object, s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02005d5c(struct ObjectRuntime *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02000d48(void);

void Func_02000f34(struct ObjectRuntime *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  if (object->action == 240) {
    struct Vector3 *pos = &position;
    struct EffectOptions *opt = &options;

    if (!GameFlag_Test(0x250))
      Func_02005f8c(object, 202);
    if (object->angle == 0xc000) {
      pos->x = object->x;
      pos->z = object->z;
    } else {
      pos->x =
          object->x + Iwram_MulQ16(0x80000, Math_Cosine(object->angle));
      pos->z = object->z + Iwram_MulQ16(0x80000, Math_Sine(object->angle));
    }
    shot = (struct ObjectRuntime *)Import_ActorCreate(280, pos->x,
                                                       object->y, pos->z);
    Func_02005d5c(shot, 2);
    ObjectDispatch_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->update = Func_02000d48;
    shot->watchedObject = object;
    object->action = 0;
    opt->priority = 1;
    opt->palette = 7;
    Func_020002f4(pos->x, object->y, pos->z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, opt);
  }
  object->action++;
}
