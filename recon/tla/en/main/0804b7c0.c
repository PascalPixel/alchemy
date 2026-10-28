// Under -mthumb-slow-loads (2026-10-26): differ 2 of 82 in tla-en, instruction
// order only. The native sets UiWindow_Create's second argument before its
// fourth; no single empty asm and 1 minute of permute lower it.
// Near miss: listing 82 instructions, draft 82, common 77, differ 10; the rest
// differs in instruction order.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "scene.h"
#include "system.h"
unsigned char Ui_FillVramBlockPattern(void);
void UiText_RenderWideStringAtOffset(s16 *, s32, s32, s32);

#if defined(TBS_EDITION_JA)
#define TEXT_COUNT 32
#else
#define TEXT_COUNT 52
#endif

s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiWork_Finalize(struct Work *work, s32 release);

#if defined(TBS_EDITION_JA)
void UiText_CopyMessageString(s32, s16 *, s32);
#else
s32 UiText_CopyMessageString(s32, s16 *, s32);
#endif

extern u8 gAdventure[];
extern void *gBattleWork;
extern volatile s32 gKeyState;
extern void *gLinkCountdownWork;
extern char MsgNoTimeToRun;

s32 UiText_ShowLocalizedMessageAndWait(void) {
  s16 buffer[64];
  s32 work;
  s32 result;
  void *state;
  s32 mode;

  state = Ram_HeapSlots->battleWork;
  mode = gAdventure[0x24B];
  if (gAdventure[0x24B] == 2 || mode <= 2 || (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if (*(s8 *)((u8 *)state + 0x43) != 0) {
      result = 1;
    }
    if (result != 0) {
      goto active;
    }
  } else {
  active:
    work = UiWindow_Create(0, 7, 30, 4, 42);
    Ui_FillVramBlockPattern();
    UiText_CopyMessageString((s32)&MsgNoTimeToRun, buffer, TEXT_COUNT);
    UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
    do {
      WaitFrames(1);
    } while ((gInput.pressed & (KEY_A | KEY_B)) == 0 &&
             *(s32 *)((u8 *)((void **)Ram_HeapSlots)[57] + 0x4C) != 0);
    UiWork_Finalize(work, 1);
  }
  return result;
}
