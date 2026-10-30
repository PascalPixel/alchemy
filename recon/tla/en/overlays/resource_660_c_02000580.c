// Near miss, differ 2 of 158 (tla-en, 2026-10-30): the same row as overlay
// 65e's Func_02000340 (resource_65e_c_02000340.c, whose header gives sched2's
// priorities): -mthumb-slow-loads counts the load of gMainRoutineWork's address
// one cycle above its plain latency, so it ties lsls r2 and wins on its
// dependents; the native has it at the plain latency, after the add into the
// event work. No reshaping of the store or of the reads after it moves it.
// Draft, differ 2 of 158 under -mthumb-slow-loads: the compiler issues
// gMainRoutineWork's load (ldr r5) before lsls r2, r2, #1 and the add into the event
// work; the native issues it after them, as in overlay 65e's Func_02000340.
// Adopted in 78b8a48d42, when it matched without the option; back on the
// listing since.
#include "../../../../games/tla/src/scene/garoh_interiors/garoh_interiors.h"

void GameFlag_ClearBit(s32 flag);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetMovementTypeById(s32 id, s32 action);
void Func_02000710(void);
void Func_020007dc(s32 value);
void Func_0200081c(s32 id, s32 x, s32 z);
void Func_02000824(s32 id, s32 x, s32 z, s32 angle);
void Func_0200087c(s32 value);
void Func_02000894(s32 id, s32 angle);

s32 Func_02000580(void) {
  s32 turn;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_0200087c(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov660Room1) {
    if (GameFlag_Test(0x91b) || gMainRoutineWork.entrance == 99) {
      Func_0200081c(19, 0, 0);
      Func_0200081c(20, 0, 0);
      Actor_SetMovementTypeById(18, 3);
    } else {
      {
        s32 x;
        s32 z;
        s32 id;

        turn = 0x2000;
        x = 153;
        id = 17;
        /* FAKEMATCH: and this forced id keeps its load right after that half;
         * unforced it loads last. */
        asm volatile("" : "+l"(id));
        x <<= 17;
        z = 335 << 16;
        Func_02000824(id, x, z, turn);
      }
      {
        s32 angle = 0x6000;
        s32 z = 336 << 16;

        Func_02000824(18, 345 << 16, z, angle);
      }
      Actor_SetSingleChildFlags(Actor_GetById(19), 0);
      Actor_SetSingleChildFlags(Actor_GetById(20), 0);
      Actor_GetById(17)->motionFlags = 0;
      Actor_GetById(17)->y = 0;
      Actor_GetById(18)->motionFlags = 0;
      Actor_GetById(18)->y = 0;
      Actor_GetById(19)->motionFlags = 0;
      Actor_GetById(19)->y = 0;
      Actor_GetById(20)->motionFlags = 0;
      Actor_GetById(20)->y = 0;
      Func_02000894(17, 0xe000);
      Actor_SetSingleChildFlags(Actor_GetById(17), 0);
      Func_02000894(18, turn);
      Actor_SetSingleChildFlags(Actor_GetById(18), 0);
    }
    if (gMainRoutineWork.entrance == 99 && !GameFlag_Test(0x91b))
      Func_02000710();
  }
  if (GameFlag_Test(0x91b))
    GameFlag_ClearBit(0x310);
  if (GameFlag_Test(0x310))
    Func_020007dc(0xa00);
  return 0;
}
