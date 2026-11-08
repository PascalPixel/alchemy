#include "types.h"
#include "edition.h"
#include "input.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "scene.h"
#include "system.h"
unsigned char Vram_FillWindowBlockPattern(void);
void Text_DrawWideStringAtOffset(s16 *, s32, s32, s32);

// The Japanese edition decodes the message (Message_DecodeRaw) into at most 32
// halfwords; the others copy its text (Message_CopyExpanded) into at most 52.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY Message_CopyExpanded
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY Message_DecodeRaw
#endif

s32 Window_Create(s32, s32, s32, s32, s32);
void Window_Close(struct Work *work, s32 release);

s32 Message_CopyExpanded(s32, s16 *, s32);
void Message_DecodeRaw(s32, s16 *, s32);

extern u8 gMainRoutineWork[];
extern void *gBattleWork;
extern s32 gKeyState;
extern void *gLinkCountdownWork;
extern char MsgNoTimeToRun;

s32 Message_ShowLocalizedAndWait(void) {
  s16 buffer[64];
  s32 work;
  s32 result;
  void *state;
  s32 mode;
  s32 attrs;

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
    attrs = 42;
    work = Window_Create(0, 7, 30, 4, attrs);
    Vram_FillWindowBlockPattern();
    BATTLE_MENU_MESSAGE_COPY((s32)&MsgNoTimeToRun, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
    Text_DrawWideStringAtOffset(buffer, work, 0, 4);
    do {
      wait(1);
    } while ((gInput.pressed & (KEY_A | KEY_B)) == 0 &&
             *(s32 *)((u8 *)((void **)Ram_HeapSlots)[57] + 0x4C) != 0);
    Window_Close(work, 1);
  }
  return result;
}
