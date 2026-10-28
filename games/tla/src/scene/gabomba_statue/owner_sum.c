#include "types.h"
#include "text/msg_ids.h"
#include "object_runtime.h"
#include "adventure.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
struct ObjectRuntime *Object_GetById(s32 id);
u8 *Unit_Get(s32 owner);
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

s32 Func_02003604(void) {
  u8 *state = Unit_Get(gAdventure.currentOwner);
  s32 sum = 0;
  u32 i;

  for (i = 0; i < 14; i++)
    sum += state[i];
  return sum & 15;
}
