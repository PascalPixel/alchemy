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
void ObjectDispatch_ApplyValueToChildren(struct ObjectRuntime *object,
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

void Func_020015cc(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x;
  s32 z = leader->z;

  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x40000, 0x20000);
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads x and z before
   * it copies the object to r10 (one reordered row). */
  do {
  } while (0);
  x >>= 20;
  z >>= 20;
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, (x << 4) + 8,
                                    (z << 4) + 8);
  WaitFrames(1);
  Object_SetPositionAndResetMotion(leader, (x << 20) + 0x80000, leader->y,
                                   (z << 20) + 0x80000);
  FieldEvent_WaitFrames(4);
  Func_02001cdc()->y -= 0x80000;
}
