// Draft, differ 63 of 184 (was 65): an object's update: while none of the
// event work's four busy counters (0x15a, 0x15e, 0x166, 0x168) is set it
// counts frames in its action parameter, and at 240 (playing cue 202 unless
// flag 0x250 is set) it launches the first free one of objects 13 to 15 from
// 8 pixels ahead along its angle, with Func_020010b4 as its update, and a
// dust effect below it. The position is declared before the options, so the
// frame matches (position at sp+56, options at sp+16). Left: the native keeps
// the action field's address in r10 and IwramMulQ16's in r5, builds 0x80000
// afresh for each product and the position's address once; this draft keeps
// 0x80000 in r5 and the routine in r10, and saves r9 too. Measured: Value2
// for the products (67).
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "system/field_effect.h"
#include "system/vector3.h"

struct ObjectRuntime *Object_GetById(u32 id);
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
void Func_020010b4(void);

void Func_02001290(struct ObjectRuntime *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *shot;
  struct Vector3 position;
  struct EffectOptions options;
  s32 i;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  for (i = 0; i != 3;)
    Object_GetById(13 + i++);
  if (object->action == 240) {
    shot = 0;
    if (!GameFlag_Test(0x250))
      Func_02005f8c(object, 202);
    if (object->angle == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      position.x =
          object->x + Iwram_MulQ16(0x80000, Math_Cosine(object->angle));
      position.z = object->z + Iwram_MulQ16(0x80000, Math_Sine(object->angle));
    }
    for (i = 0; i != 3; i++) {
      shot = Object_GetById(13 + i);
      if (shot->update == 0)
        break;
    }
    shot->x = position.x;
    shot->y = object->y;
    shot->z = position.z;
    Func_02005d5c(shot, 2);
    ObjectDispatch_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->priorityFlags |= 32;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->watchedObject = object;
    shot->update = Func_020010b4;
    object->action = 0;
    options.priority = 1;
    options.palette = 7;
    Func_020002f4(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->action++;
}
