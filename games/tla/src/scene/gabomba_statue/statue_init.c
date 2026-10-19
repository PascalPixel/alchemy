#include "types.h"
#include "text/msg_ids.h"
#include "object_runtime.h"
#include "party_state.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
struct ObjectRuntime *Object_GetById(s32 id);
u8 *UnitGet(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void WaitFrames(s32 frames);
void Func_02005754(s32 message);
void Func_02005764(s32 id, s32 value);
void Func_02005714(s32 id, s32 a, s32 b);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const u8 *callback);
void Func_02005674(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);

void Func_020034ec(void) { Func_02001b38(); }

static __inline__ void Map_CopyBlock(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005674(a, b, c, d, e, f);
}

static __inline__ void Exit_Set(s32 index, s32 flag, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200583c(index, flag, a, b);
}

void Func_020034f4(void) {
  if ((u16)(gPartyState.entrance - 3) <= 1) {
    ObjectMotion_EnableActionAndSetCallback(27, gData_02005944);
    ObjectMotion_EnableActionAndSetCallback(28, gData_02005944);
    Map_CopyBlock(33, 24, 3, 3, 27, 29);
  }
  Func_020053bc(gData_02005d7c);
  if (!GameFlag_Test(0x8ce) || !GameFlag_Test(0x8cf))
    Object_GetById(26)->y = 0x100000;
  Func_02005824();
  Exit_Set(0, 513, 8, 9);
  Exit_Set(1, 514, 10, 11);
  Func_02001b68();
}
