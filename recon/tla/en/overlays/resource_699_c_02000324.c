// Draft, differ 38 of 75: the native keeps gMainRoutineWork base in r5 only
// through the first room and reloads it from the pool for the second entrance
// test and the second room test, and loads the two blend values from pool words
// placed before the party base; this form shares r5 differently.
#include "types.h"

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
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
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_0200015c(struct Actor *object);
void Func_020004dc(s32 mode);
struct Actor *Func_02001c04(s32 type, s32 x, s32 y, s32 z);
void Func_02001c34(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct Actor *Func_02001cdc(void);
void Func_02001d3c(s32 id);
void Func_02001d4c(s32 cue);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

s32 Func_02000324(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room1) {
    Func_020004dc(1);
    if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 4)
      Func_02001d3c(gMainRoutineWork.currentOwner);
    if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 6)
      Func_02001d3c(gMainRoutineWork.currentOwner);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room2) {
    *(volatile u16 *)0x04000052 = 0x1008;
    *(volatile u16 *)0x04000050 = 0x3f10;
    Func_02000298();
  }
  return 0;
}
