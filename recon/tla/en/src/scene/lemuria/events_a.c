#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "scene_tables.h"

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
s32 ArcTan2(s32, s32);
struct Actor *Actor_GetById(s32 id);
void Animation_SetIndexAndInitializeObjects(struct Actor *object, s32 index);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_TurnToFaceEachOther(s32 id, s32 other, s32 mode);
void Actor_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001b7c(struct Actor *object, s32 mode);
void Func_02001b84(s32 scene, s32 x, s32 z);
void Func_02001ba4(struct Actor *object, s32 value);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001c2c(s32 id, s32 x, s32 z);
void Func_02001c6c(s32 message);
void Func_02001c7c(s32 id, s32 a, s32 b);
void Func_02001c84(s32 a, s32 b);
void Func_02001cac(s32 id, s32 a);
void Func_02001cd4(s32 a);
void Func_02001cf4(s32 a, s32 b, s32 c);
void Func_02001d04(s32 a);
void Func_02001d44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001d54(struct Actor *object);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);
void Func_02001d7c(s32 a);

extern const struct WarpTarget gData_020021bc;
extern const struct WarpTarget gData_020021c4;
extern const struct WarpTarget gData_020021cc;
extern const struct WarpTarget gData_020021d4;
extern const struct WarpTarget gData_020021dc;
extern const struct WarpTarget gData_020021e4;
extern const struct WarpTarget gData_020021ec;

extern u8 MsgTheDoorIs;
extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;
extern u8 MsgWeAreThe;
extern u8 MsgAreThereOther;

static __inline__ void Func_02001c2c_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c2c(id, x, z);
}

static __inline__ void Func_02001d44_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d44(a, b, c, d, e, f, g);
}

void Func_02000428(void) { Func_02001d44_(37, 72, 2, 2, 39, 71, 2); }

void Func_02000448(void) { Func_02001d44_(59, 64, 4, 3, 49, 71, 2); }

void Func_02000468(void) { Func_02001d44_(52, 69, 1, 1, 53, 69, 2); }

void Func_02000488(void) { Func_02001d44_(53, 64, 1, 1, 52, 64, 2); }

void Func_020004a8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001d44_(59, 64, 4, 3, 49, 71, 2);
  if (GameFlag_Test(0xfbb) == 0) {
    Func_02001c2c_(69, PIXELS(808), PIXELS(1176));
    if (leader->x >> 20 == 50 && leader->z >> 20 == 73) {
      Func_02001bcc();
      Func_02001d04(0);
      Func_02001cac(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001bd4();
    }
  }
}
