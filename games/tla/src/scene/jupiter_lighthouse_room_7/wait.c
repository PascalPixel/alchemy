#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"

extern const u8 gData_020031c4[];
extern s32 gData_02003594;
extern s16 gData_020035fe;
extern struct ObjectRuntime *gData_02003600[];
extern u16 gData_02003702;
struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitTicks(s32 ticks);
void Func_02002be4(s32 a, s32 b);
void Func_02000060(s32 value);
void Func_020000b4(void);
void Func_02000100(void);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c0c(s32 value);
void Func_02001a48(const u8 *data, s32 a, s32 b);
void Func_02002bec(void);

void Func_0200013c(void) {
  s32 flag = 514;

  /* FAKEMATCH: ⚓️ builds the flag again for the second call */
  asm("" : "+l"(flag));
  if (!GameFlag_Test(flag)) {
    GameFlag_SetBit(514);
    Func_02000060(1);
  }
}

void Func_02000160(void) {
  s32 flag = 514;

  /* FAKEMATCH: ⚓️ builds the flag again for the second call */
  asm("" : "+l"(flag));
  if (GameFlag_Test(flag) == 1) {
    GameFlag_ClearBit(514);
    Func_020000b4();
  }
  Func_02000100();
}

void Func_02000184(void) {
  WaitTicks(10);
  Func_02000100();
}
