// Near miss, differ 2 of 82 (tla-en, 2026-10-30): scheduler order only. After
// storing the fifth argument (42) through r3, the native sets Window_Create's
// fourth argument (r3 = 4) before its second (r1 = 7). In sched2 here, r3 = 4
// is anti-dependent on that store, which arm_adjust_cost prices at 0, so it
// ranks below the independent r1 = 7; were that dependence 1, its four
// dependents would put it first and give the native's r3, r1, r2, r0. No
// source form reaches it: The Broken Seal's if form (kept here), u8/u16,
// unprototyped and variadic Window_Create prototypes, a single empty asm and
// 1 + 4 minutes of permute all leave differ 2; an empty asm cannot sit
// between a call's stack store and its register loads.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "scene.h"
#include "system.h"
unsigned char Vram_FillWindowBlockPattern(void);
void Text_DrawWideStringAtOffset(s16 *, s32, s32, s32);

#if defined(TBS_EDITION_JA)
#define TEXT_COUNT 32
#else
#define TEXT_COUNT 52
#endif

s32 Window_Create(s32, s32, s32, s32, s32);
void Window_Close(struct Work *work, s32 release);

#if defined(TBS_EDITION_JA)
void Message_CopyExpanded(s32, s16 *, s32);
#else
s32 Message_CopyExpanded(s32, s16 *, s32);
#endif

extern u8 gMainRoutineWork[];
extern void *gBattleWork;
extern volatile s32 gKeyState;
extern void *gLinkCountdownWork;
extern char MsgNoTimeToRun;

s32 Message_ShowLocalizedAndWait(void) {
  s16 buffer[64];
  s32 work;
  s32 result;
  void *state;
  s32 mode;

  state = Ram_HeapSlots->battleWork;
  mode = gMainRoutineWork[0x24B];
  if (gMainRoutineWork[0x24B] == 2 || mode <= 2 || (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if (*(s8 *)((u8 *)state + 0x43) != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    work = Window_Create(0, 7, 30, 4, 42);
    Vram_FillWindowBlockPattern();
    Message_CopyExpanded((s32)&MsgNoTimeToRun, buffer, TEXT_COUNT);
    Text_DrawWideStringAtOffset(buffer, work, 0, 4);
    do {
      wait(1);
    } while ((gInput.pressed & (KEY_A | KEY_B)) == 0 &&
             *(s32 *)((u8 *)((void **)Ram_HeapSlots)[57] + 0x4C) != 0);
    Window_Close(work, 1);
  }
  return result;
}
