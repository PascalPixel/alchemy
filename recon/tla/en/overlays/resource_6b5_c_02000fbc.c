// Draft of overlay 6b5's Func_02000fbc, the lobby's sound test (a number from 0
// to 96 picks a song from gData_02001b02, skipping songs Func_02001adc does
// not allow unless L, Up and Start are held), modeled on ☀️'s
// BattleArena_ChooseDebugPartyPreset as goto loops: differ 34 of 153. Left:
// jump.c moves the cancel block (song = -1) up to its test, where the native
// keeps it after the outer loop's jump (a second use of the label keeps it
// there, differ 16), and the actor and the shown number trade r8 and sl. As
// for(;;) loops, loop.c hoists the input address (differ 73 to 158).
#include "types.h"
#include "input.h"
#include "main_routine_work.h"

extern u8 MsgYouHaveA[];
extern u8 MsgComeBackWhen[];
extern u16 gData_02001b02[];
extern s16 gData_02003860;

void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
s32 Func_02001a74(s32 actor, s32 flags);
void Func_02001ab4(s32 skip);
void Func_02001ad4(s32 song);
s32 Func_02001adc(s32 song);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
s32 Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void WindowSprite_PrepareForRedraw(s32 window);
void Text_DrawNumber(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Window_Close(s32 window, s32 release);
void wait(s32 frames);

void Func_02000fbc(s32 actor) {
  s32 window;
  s32 song;
  s32 shown;
  s32 step;
  s32 all;

  Func_02001a14();
  Func_02001ab4(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  Func_02001a64((s32)MsgYouHaveA);
  Func_02001a74(actor, 0);
  song = 0;
  all = 0;
  if ((gInput.held & 0x248) == 0x248)
    all = 1;
  Func_02001ad4(78);
open:
  window = Window_Create(12, 8, 6, 3, 2);
  shown = -1;
redraw:
  if (song != shown) {
    WindowSprite_PrepareForRedraw(window);
    Text_DrawNumber(song, 3, window, 0, 0);
    shown = song;
  }
  step = 0;
  if (gInput.repeat & 32)
    step--;
  if (gInput.repeat & 16)
    step = 1;
  goto next;
skip:
  if (Func_02001adc(gData_02001b02[song]) != 0)
    goto keys;
next:
  if (step != 0) {
    song += step;
    if (song < 0)
      song = 96;
    if ((u32)song > 96)
      song = 0;
    if (all == 0 && gMainRoutineWork.unk22d[2] == 0 && gData_02003860 == 0)
      goto skip;
  }
keys:
  if (gInput.pressed & 1)
    goto chosen;
  if (gInput.pressed & 2)
    goto cancel;
  wait(1);
  goto redraw;
chosen:
  Window_Close(window, 1);
  if (song < 0)
    goto done;
  Func_02001ad4(gData_02001b02[song]);
  goto open;
cancel:
  song = -1;
  goto chosen;
done:
  Func_02001ad4(78);
  wait(120);
  Func_02001ad4(0x2c5);
  Func_02001a64((s32)MsgComeBackWhen);
  Func_02001a74(actor, 0);
  wait(10);
  Func_02001a1c();
}
