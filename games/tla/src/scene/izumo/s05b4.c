#include "izumo.h"

extern u32 gFrameCount;

static inline s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarding through the inline, as overlay 679's GameFlag_Test_
   * does, keeps the actor in r5 and the message in r6 (differ 16, the two
   * swapped, as a direct call). */
  return GameFlag_Test(flag);
}

/// The ring of stones: one line after the flag is set, otherwise the full talk
/// and its answer.
void Func_020004bc(s32 actor) {
  Func_02000d00();
  Func_02000dd0(0);
  if (GameFlag_Test_(0x924)) {
    Func_02000d60(MsgCanYouImagine);
    Func_02000d70(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRing;

    Func_02000d60(message);
    UiText_OpenMessageAtObject(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000e00(), 0) == 0) {
      FieldEvent_WaitFrames(10);
      Func_02000d60(message + 1);
    } else {
      FieldEvent_WaitFrames(20);
      Func_02000d60(message + 2);
    }
    Func_02000d70(actor, 0);
  }
  Func_02000d08();
}

void Func_02000538(s32 actor) {
  Func_02000d00();
  Func_02000dd0(0);
  if (GameFlag_Test_(0x924)) {
    Func_02000d60(MsgButItS);
    Func_02000d70(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRingOf;

    Func_02000d60(message);
    UiText_OpenMessageAtObject(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000e00(), 0) == 0) {
      FieldEvent_WaitFrames(10);
      Func_02000d60(message + 1);
    } else {
      FieldEvent_WaitFrames(20);
      Func_02000d60(message + 2);
    }
    Func_02000d70(actor, 0);
  }
  Func_02000d08();
}

void Func_020005b4(s32 actor) {
  const u8 *message;

  Func_02000d00();
  Func_02000dd0(0);
  message = MsgLadyUzumeAnd;
  Func_02000d60(message);
  UiText_OpenMessageAtObject(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02000e00(), 0) == 0) {
    FieldEvent_WaitFrames(10);
    Func_02000d60(message + 1);
  } else {
    FieldEvent_WaitFrames(20);
    Func_02000d60(message + 2);
  }
  Func_02000d70(actor, 0);
  Func_02000d08();
}

/// Every fourth frame, colours 1 to 14 of palettes 3, 4 and 5 rotate up by one.
void Func_0200060c(void) {
  volatile u16 *palette;
  s16 saved;
  s32 index;

  /* FAKEMATCH: the three rotations are goto loops, which loop.c leaves alone,
   * as the native's are; as do-while, while or for loops, 90 instructions
   * differ in tla-en. */
  if ((gFrameCount & 3) == 0) {
    saved = *(volatile u16 *)0x0500007c;
    index = 14;
  first:
    palette = (u16 *)0x05000060;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)0x05000062 = saved;

    saved = *(volatile u16 *)0x0500009c;
    index = 14;
  second:
    palette = (u16 *)0x05000080;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)0x05000082 = saved;

    saved = *(volatile u16 *)0x050000bc;
    index = 14;
  third:
    palette = (u16 *)0x050000a0;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto third;
    *(volatile u16 *)0x050000a2 = saved;
  }
}
