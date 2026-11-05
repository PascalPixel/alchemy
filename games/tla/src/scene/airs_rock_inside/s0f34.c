#include "types.h"
#include "call.h"
#include "actor.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "system/field_effect.h"
#include "system/vector3.h"

s32 GameFlag_Test(s32 flag);
void Func_02005f8c(struct Actor *object, s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02005d5c(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02000d48(void);

// A launcher's update with a new shot each time: while none of the event work's
// four busy counters is set it counts frames in its action parameter, and at
// 240 launches a shot 8 pixels ahead along its angle with a dust effect below
// it.
void Func_02000f34(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  if (object->action == 240) {
    if (!GameFlag_Test(0x250))
      Func_02005f8c(object, 202);
    if (object->angle == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded, so each product builds its own constant and
       * they share the routine's address; as direct calls, 106 of 153
       * instructions differ in tla-en. */
      position.x = object->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Cosine(object->angle));
      position.z = object->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Sine(object->angle));
    }
    shot = (struct Actor *)Import_ActorCreate(280, position.x,
                                                       object->y, position.z);
    Func_02005d5c(shot, 2);
    Actor_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->update = Func_02000d48;
    shot->watchedObject = object;
    object->action = 0;
    options.priority = 1;
    options.palette = 7;
    Func_020002f4(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->action++;
}
