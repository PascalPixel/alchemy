// Draft, differ 31 of 64: a goto loop keeps the constants inside the pass as
// the native does (a for loop hoists 0x8000 and the zero: differ 74). Left: the
// native builds 0x8000 and the sprite priority mask -13 in r0 and copies the
// mask to r2, computes the next index (adds r0, r6, #1) before the parity
// branch, and builds 212 and 116 with movs where this form loads halfwords from
// the pool.
#include "types.h"

#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "frame.h"
#include "field_sprite.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

struct CellTarget {
  u8 unk00[6];
  s16 x;
  s16 z;
};

s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
s32 Math_Cosine(s32 angle);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object,
                                         s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_0200015c(struct ObjectRuntime *object);
void Func_020004dc(s32 mode);
struct ObjectRuntime *Func_02001c04(s32 type, s32 x, s32 y, s32 z);
void Func_02001c34(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct ObjectRuntime *Func_02001cdc(void);
void Func_02001d3c(s32 id);
void Func_02001d4c(s32 cue);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_02000298(void) {
  s32 i;

  struct ObjectRuntime *object;
  s16 *action;
  s16 *timer;

  i = 0;
  {
  next:
    object = Func_02001c04(0x2a0, 0, 0, 0);

    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (i << 12) + 0x8000;
    object->acceleration = (i << 12) + 0x8000;
    object->update = (void (*)(void))Func_020001d8;
    OBJECT_BYTE(object, 99) = i << 3;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetPartAttribute(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
    Animation_SetStateFlags(object, 1);
    action = &object->action;
    timer = (s16 *)&object->unk66;
    if (i & 1) {
      *action = 212;
      *timer = 0;
    } else {
      *action = 116;
      *timer = 0;
    }
    i++;
    if (i != 16)
      goto next;
  }
}
