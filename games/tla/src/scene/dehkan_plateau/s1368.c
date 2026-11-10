#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);

void Func_02001368(void) {
  s32 y;

  Func_02000708(gData_02003f52);
  Func_020012d8();
  y = Actor_GetById(gMainRoutineWork.currentOwner)->y;
  /* FAKEMATCH: ⚓️ loads the height before it builds the limit; the plain test
   * starts the constant first (one pair of instructions swapped). */
  do {
  } while (0);
  if (y < 0x80000) {
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
}
