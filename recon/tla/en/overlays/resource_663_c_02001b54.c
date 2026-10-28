// Draft, differ 63 of 153 (was 107): a launcher's update, overlay 664's
// Func_02001290 with a new shot each time: while none of the event work's
// four busy counters is set it counts frames in its action parameter, and at
// 240 (cue 202 unless flag 0x250 is set) it creates a shot of type 280 8
// pixels ahead along its angle, with Func_02001908 as its update, and a dust
// effect below it. The same listing as overlay 664's Func_02000f34. The
// position and the options are read through pointers taken at the top of the
// launch, as the native forms both addresses there (sp+56 in r6, sp+16 in
// r8). Left: the native builds 0x80000 afresh for each product and keeps
// IwramMulQ16's address in r5; this draft keeps 0x80000 in r5, the routine in
// r8 and the options' address in r9.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "system/field_effect.h"
#include "system/vector3.h"

s32 GameFlag_Test(s32 flag);
void Func_02003cd0(struct Actor *object, s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02003b28(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02001908(void);

void Func_02001b54(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  if (object->action == 240) {
    struct Vector3 *pos = &position;
    struct EffectOptions *opt = &options;

    if (!GameFlag_Test(0x250))
      Func_02003cd0(object, 202);
    if (object->angle == 0xc000) {
      pos->x = object->x;
      pos->z = object->z;
    } else {
      pos->x =
          object->x + Iwram_MulQ16(0x80000, Math_Cosine(object->angle));
      pos->z = object->z + Iwram_MulQ16(0x80000, Math_Sine(object->angle));
    }
    shot = (struct Actor *)Import_ActorCreate(280, pos->x,
                                                       object->y, pos->z);
    Func_02003b28(shot, 2);
    Actor_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->update = Func_02001908;
    shot->watchedObject = object;
    object->action = 0;
    opt->priority = 1;
    opt->palette = 7;
    Func_0200015c(pos->x, object->y, pos->z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, opt);
  }
  object->action++;
}
