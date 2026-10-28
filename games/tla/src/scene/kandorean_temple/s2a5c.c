#include "local.h"
#include "io_reg.h"
#include "edition.h"
#include "task.h"

s32 Task_Add(void (*callback)(void), s32 priority);
void Func_02005398(void);
s32 Func_020053a0(s32 a, s32 b, s32 c);
void Func_020053b8(const void *data);
void Func_02000b78(s32 id);
void Func_02000bac(s32 id);
void Func_02000c74(void);
void Func_02001634(void);
void Func_02001804(void);
void Func_02001b50(void);
void Func_02003684(void);
void Func_020037e0(void);
extern s32 gData_020062a0;
extern s32 gData_020062a4;
extern u16 gData_020062a8;
extern u16 gData_020062aa;

void Func_0200290c(void) {
  Func_02005398();
  Func_020053a0(0, 8, 9);
  Func_020053a0(1, 10, 11);
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 4);
  Actor_SetModeById(10, 4);
  Actor_SetModeById(11, 4);
  Func_020053b8(gData_020054dc);
  {
    /* FAKEMATCH: the blend value pinned to r3, so it loads from the pool
     * before the register address is started; as a plain store, 2
     * instructions differ in tla-en. */
    register u16 alpha __asm__("r3") = 0xc08;

    /* FAKEMATCH: the one-pass block's loop note holds the address until the
     * pinned value is loaded; the empty asm of overlay 696's s43f8.c with r2
     * clobbered is differ 10 here. */
    do {
    } while (0);
    REG_BLDALPHA = alpha;
  }
  REG_BLDCNT = 0x3f10;
  Func_02005388(245);
  Task_Add(Func_02000c74, TASK_PASS_VBLANK);
  Func_02005230(13, 0, 0);
  Func_02005230(14, 0, 0);
  Func_02005230(15, 0, 0);
  Func_02005230(16, 0, 0);
  Func_02000b78(13);
  Func_02000b78(14);
  Func_02000bac(15);
  Func_02000bac(16);
  gData_020062a0 = 0;
  gData_020062a4 = 0;
  gData_020062a8 = 0;
  gData_020062aa = 0;
  if (GameFlag_Test(0x109) == 0) {
    if (gAdventure.entrance == 5)
      Func_02003684();
    else if (gAdventure.entrance == 6)
      Func_020037e0();
  }
  if (GameFlag_Test(0x301))
    Task_Add(Func_02001634, TASK_PASS_VBLANK);
  else if (GameFlag_Test(0x300))
    Task_Add(Func_02001804, TASK_PASS_VBLANK);
  Task_Add(Func_02001b50, TASK_PASS_VBLANK);
  Func_02005338(0);
}

void Func_02002a5c(void) {
  Func_02005338(0);
#if (LANGUAGE != LANGUAGE_JAPANESE) /* Absent from the Japanese edition. */
  if (gAdventure.entrance == 1)
    GameFlag_ClearBit(0x12f);
#endif
}
