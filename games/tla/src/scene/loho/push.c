#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "scene_tables.h"

extern const u8 gData_020017d8[];
extern const u8 gData_020017a4[];
extern const u8 gData_0200171c[];
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Actor_SetMovementTypeById(s32 id, const u8 *callback);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 value);
void Func_02001400(s32 a, s32 b, s32 c);
void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_020013f8(s32 value);
void Func_02000f38(void);

void Func_02000ffc(void) {
  s32 back;
  s32 id;
  s32 x;

  Func_020012e0();
  Func_020013f8(0);
  back = 14;
  id = 4;
  x = 0;
  /* FAKEMATCH: ⚓️ sets the other arguments before it negates the step */
  asm("" : "+l"(back), "+l"(id), "+l"(x));
  Actor_CommitPositionAndActivate(id, x, -back);
  Actor_CommitPositionAndActivate(4, 16, 0);
  FieldEvent_Wait(10);
  Func_02000f38();
}

void Func_0200102c(void) {
  s32 back;
  s32 id;
  s32 x;

  Func_020012e0();
  Func_020013f8(0);
  back = 14;
  id = 4;
  x = 0;
  /* FAKEMATCH: ⚓️ sets the other arguments before it negates the step */
  asm("" : "+l"(back), "+l"(id), "+l"(x));
  Actor_CommitPositionAndActivate(id, x, -back);
  Actor_CommitPositionAndActivate(4, -16, 0);
  FieldEvent_Wait(10);
  Func_02000f38();
}

void Func_0200105c(void) {
  Func_020012e0();
  Func_020013f8(0);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Func_02000f38();
}

void Func_02001080(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Actor_SetSingleChildFlags(Actor_GetById(20), 0);
}

void Func_020010a0(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 521;
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
}
