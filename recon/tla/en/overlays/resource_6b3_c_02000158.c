// Draft, differ 24 of 100: rewritten from the listing. Opens the epilogue's
// message window at x and y and, unless bit 15 of the message is set, a name
// window at left and line that Func_0200005c follows; in mode 2 it restarts the
// window's text, waits for the text, closes both windows and waits the given
// frames. Left, all in the prologue: the native copies y out of r2 first and
// builds 0x8000 there, copying x late, where this copies x first and builds
// 0x8000 in r1, which moves the zero for the name window earlier. Permute 3
// minutes found nothing better.
#include "types.h"

struct EpilogueWindow {
  u8 unk00[8];
  u16 unk08;
  u16 unk0a;
  u8 unk0c[2];
  u16 unk0e;
};

extern s32 gData_02000a28;
extern s8 gData_02000a34;
extern s16 gData_02000a38;
extern s16 gData_02000a2c;

s32 BattleEffect_GetResourceId(s32 message);
struct EpilogueWindow *UiText_OpenMessageWindow(s32 id, s32 x, s32 y,
                                                s32 resource);
struct EpilogueWindow *Func_02000804(s32 message, s32 a, s32 left, s32 line);
s32 UiWork_IsComplete(void);
void UiWork_Finalize(struct EpilogueWindow *window, s32 mode);
void WaitFrames(s32 frames);

void Func_02000158(s32 message, s32 x, s32 y, s32 left, s32 line, s32 wait,
                   s32 mode) {
  s32 wide = 0x8000 & message;
  struct EpilogueWindow *window;
  struct EpilogueWindow *name;

  message &= 0x7fff;
  name = 0;
  window = UiText_OpenMessageWindow(
      gData_02000a28, x, y, (BattleEffect_GetResourceId(message) << 16) | 4);
  if (!wide) {
    name = Func_02000804(message, 0, left, line);
    gData_02000a34 = 1;
    gData_02000a38 = left << 3;
    gData_02000a2c = line << 3;
  }
  if (mode == 2) {
    window->unk0e++;
    window->unk08 = 0;
    window->unk0a = 0;
  }
  while (!UiWork_IsComplete())
    WaitFrames(1);
  WaitFrames(1);
  UiWork_Finalize(window, 2);
  if (!wide) {
    UiWork_Finalize(name, 2);
    gData_02000a34 = wide;
  }
  if (wait > 0) {
    s32 i = wait;

    do {
      WaitFrames(1);
    } while (--i != 0);
  }
  gData_02000a28++;
}
