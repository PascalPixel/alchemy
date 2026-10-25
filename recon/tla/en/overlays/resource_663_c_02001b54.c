// Draft, differ 107 of 153: a launcher's update, overlay 664's Func_02001290
// with a new shot each time: while none of the event work's four busy counters
// is set it counts frames in its action parameter, and at 240 (cue 202
// unless flag 0x250 is set) it creates a shot of type 280 8 pixels ahead
// along its angle, with Func_02001908 as its update, and a dust effect
// below it. The same listing as overlay 664's Func_02000f34. Left: the
// native keeps the launcher in r7 and the position at sp+56 behind the
// options, and builds the position's address once; most rows move with it.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "system/field_effect.h"
#include "system/vector3.h"

s32 GameFlag_Test(s32 flag);
void Func_02003cd0(struct ObjectRuntime *object, s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02003b28(struct ObjectRuntime *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02001908(void);

void Func_02001b54(struct ObjectRuntime *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  if (object->action == 240) {
    if (!GameFlag_Test(0x250))
      Func_02003cd0(object, 202);
    if (object->angle == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      position.x =
          object->x + Iwram_MulQ16(0x80000, Math_Cosine(object->angle));
      position.z = object->z + Iwram_MulQ16(0x80000, Math_Sine(object->angle));
    }
    shot = (struct ObjectRuntime *)Engine_ObjectCreate(280, position.x,
                                                       object->y, position.z);
    Func_02003b28(shot, 2);
    ObjectDispatch_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->update = Func_02001908;
    shot->linkedObject = object;
    object->action = 0;
    options.priority = 1;
    options.palette = 7;
    Func_0200015c(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->action++;
}
