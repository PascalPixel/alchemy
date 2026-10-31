// Matches: differ 0 of 157 (tla-en, 2026-10-30).
// Overlay 653's Func_020019d4, translated from its listing. The earlier
// 4-instruction difference was the flag's `movs r6, #0`: the native clears it
// once before the first test, not in the first test's else branch. Calls stay
// in the Call1 and Value1 forms: as direct calls, GameFlag_Test's argument
// 0x20e is kept in a register and 37 of 157 instructions differ.
#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_02000778();
void Func_02005388();
void GameFlag_ClearBit();
void GameFlag_SetBit();
s32 GameFlag_Test();
s32 Actor_GetById();
extern u8 gData_02006298[];
extern u8 gData_0200629c[];

void Func_020019d4(void) {
  s32 v_r5_1, v_r8_1, v_r7_1, p_r6;

  v_r5_1 = Value1((s32 (*)())Actor_GetById, gMainRoutineWork.currentOwner);
  v_r8_1 = FIELD_AT(v_r5_1, s32, 8) >> 20;
  v_r7_1 = FIELD_AT(v_r5_1, s32, 16) >> 20;
  p_r6 = 0;
  if (v_r8_1 == 13 && v_r7_1 == 13) {
    Call1((void (*)())GameFlag_SetBit, 0x208);
    p_r6 = 1;
  } else {
    Call1((void (*)())GameFlag_ClearBit, 0x208);
  }
  if (v_r8_1 == 23 && v_r7_1 == 15) {
    Call1((void (*)())GameFlag_SetBit, 0x209);
    p_r6 = 1;
  } else {
    Call1((void (*)())GameFlag_ClearBit, 0x209);
  }
  if (p_r6)
    Call1((void (*)())GameFlag_SetBit, 0x17d);
  else
    Call1((void (*)())GameFlag_ClearBit, 0x17d);
  if (FIELD_AT(gData_02006298, s32, 0) != 0 &&
      FIELD_AT(v_r5_1, s32, 12) == 0 &&
      FIELD_AT(v_r5_1, s32, 8) > 0x11c0000 &&
      FIELD_AT(v_r5_1, s32, 8) < 0x1340000 &&
      FIELD_AT(v_r5_1, s32, 16) < 0xc80000 &&
      Value1((s32 (*)())GameFlag_Test, 0x20b) == 0)
    Func_02000778(v_r5_1, 0x26666);
  if (FIELD_AT(gData_0200629c, s32, 0) != 0 &&
      FIELD_AT(v_r5_1, s32, 12) == 0 &&
      FIELD_AT(v_r5_1, s32, 8) > 0x14a0000 &&
      FIELD_AT(v_r5_1, s32, 8) <= 0x165ffff &&
      FIELD_AT(v_r5_1, s32, 16) < 0xd80000 &&
      Value1((s32 (*)())GameFlag_Test, 0x20b) == 0)
    Func_02000778(v_r5_1, 0x39999);
  if (Value1((s32 (*)())GameFlag_Test, 0x20e)) {
    if (!Value1((s32 (*)())GameFlag_Test, 0x20c) &&
        !Value1((s32 (*)())GameFlag_Test, 0x20d)) {
      Call1((void (*)())Func_02005388, -1);
      Call1((void (*)())GameFlag_ClearBit, 0x20e);
    }
  } else if (Value1((s32 (*)())GameFlag_Test, 0x20c) ||
             Value1((s32 (*)())GameFlag_Test, 0x20d)) {
    Call1((void (*)())Func_02005388, 162);
    Call1((void (*)())GameFlag_SetBit, 0x20e);
  }
}
