#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "party_state.h"

extern s32 gData_020023c0;
extern s16 gData_0200242a;
extern struct ObjectRuntime *gData_0200242c[];
extern u16 gData_0200252e;
struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitTicks(s32 ticks);
s32 Func_020018bc(s32 value);
void Func_02001ba4(s32 value);
void Func_020000b4(s32 value);
void Func_02000108(void);
void Func_02000154(void);
void Scene_SetArrivalFlags(s32 value);
void Func_020003a4(s32 value);

void Func_02000190(void) {
  s32 flag = 512;

  /* FAKEMATCH: ⚓️ builds the flag again for the second call */
  asm("" : "+l"(flag));
  if (!GameFlag_Test(flag)) {
    GameFlag_SetBit(512);
    Func_020000b4(1);
  }
}

void Func_020001b0(void) {
  s32 flag = 512;

  /* FAKEMATCH: ⚓️ builds the flag again for the second call */
  asm("" : "+l"(flag));
  if (GameFlag_Test(flag) == 1) {
    GameFlag_ClearBit(512);
    Func_02000108();
  }
  Func_02000154();
}

void Func_020001d0(void) {
  WaitTicks(10);
  Func_02000154();
}
