/// @file
/// Routines whose twins are in The Broken Seal's window/screens/link_timer.c.
// Part of f04a85c.c.

#include "../f04a85c_internal.h"

void Camera_ConfigureSceneFar(s32 offset);
void Link_DrawShiftedTilePair(s32 destination);
s32 Link_CreateCountdownLabelWindow(void);

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct CountdownDisplayEntry {
  u8 data[12];
};

struct LinkSignature {
  u8 padding000[8];
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

struct LinkCountdownState {
  struct CountdownDisplayEntry entries[3];
  u8 entryActive[3];
  u8 padding027;
  s32 targetOffset;
  s32 currentOffset;
  u8 padding030[20];
  s32 displayHandle;
  s32 secondaryHandle;
  s32 timer;
  s32 enabled;
};

struct LinkRuntimeState {
  u8 padding000[80];
  u8 side;
  u8 padding051;
  u8 paused;
};

extern struct LinkCountdownState *gLinkCountdownWork;
extern struct LinkRuntimeState *gBattleWork;
extern struct LinkSignature gLinkPeerSignatures[];

/* VIEW(countdown): routine Text_DrawNumberInWindow as void Text_DrawNumberInWindow(s32 value, s32 width, s32 handle, s32 arg3, s32 arg4); f04a85c has void Text_DrawNumberInWindow(s32 value, s32 digits, struct TargetWindow *window, s32 x, s32 y). */
extern void Text_DrawNumberInWindow__countdown(s32 value, s32 width, s32 handle, s32 arg3,
                             s32 arg4) __asm__("Text_DrawNumberInWindow");

void Sound_PlayFar(s32 soundId);

void UpdateLinkSessionCountdown(void) {
  struct LinkCountdownState *state =
      ((struct LinkCountdownState **)Ram_HeapSlots)[57];
  struct LinkRuntimeState *runtime;
  struct CountdownDisplayEntry *entry;
  u8 *active;
  struct LinkSignature *signature;
  s32 target;
  s32 current;
  s32 difference;
  s32 step;
  s32 newOffset;
  s32 timer;
  s32 nextTimer;
  s32 seconds;
  s32 i;

  if (state != 0) {
    target = state->targetOffset;
    current = state->currentOffset;
    if (target != current) {
      difference = target - current;
      step = __divsi3(difference, 3);
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->currentOffset = newOffset;
      Camera_ConfigureSceneFar(newOffset);
    }

    entry = state->entries;
    active = state->entryActive;
    i = 2;
    do {
      if (*active++ != 0)
        Oam_AddSpriteAtDepth(entry, 240);
      i--;
      entry++;
    } while (i >= 0);
    Link_DrawShiftedTilePair(0x06006680);

    if (state->enabled == 0)
      goto done;

    runtime = (struct LinkRuntimeState *)Ram_HeapSlots->battleWork;
    if (runtime->paused == 0)
      goto load_timer;
    state->timer = 0;
    goto done;

  signature_tail:
    if (signature->v != 'V')
      goto signature_done;
    if (signature->s != 'S')
      goto signature_done;
    state->timer = COUNTDOWN_START_FRAMES;
    timer = state->timer;
    goto signature_done;

  load_timer:
    timer = state->timer;
    if (timer >= 0)
      goto timer_ready;
    signature = &gLinkPeerSignatures[runtime->side ^ 1];
    if (signature->e != 'E')
      goto signature_done;
    if (signature->d == 'D')
      goto signature_tail;

  signature_done:
    if (timer < 0)
      goto done;

  timer_ready:
    if (state->displayHandle == 0 && state->secondaryHandle == 0) {
      state->displayHandle = Link_CreateCountdownLabelWindow();
      timer = state->timer;
    }

    if (timer > 0) {
      nextTimer = timer - 1;
      state->timer = nextTimer;
      timer = nextTimer;
    }
    if (timer < 0)
      goto done;

    seconds = __divsi3(timer + FRAMES_PER_SECOND - 1, FRAMES_PER_SECOND);
    if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
      Sound_PlayFar(108);

    if (state->displayHandle != 0)
      Text_DrawNumberInWindow__countdown(seconds, 2, state->displayHandle, 16, 8);
  }

done:
  return;
}

unsigned char Vram_FillWindowBlockPattern(void);
/* VIEW(countdown): routine Text_DrawWideStringAtOffset as void Text_DrawWideStringAtOffset(s16 *, s32, s32, s32); f04a85c has void Text_DrawWideStringAtOffset(u16 *text, struct TargetWindow *window, s32 x, s32 y). */
extern void Text_DrawWideStringAtOffset__countdown(s16 *, s32, s32, s32) __asm__("Text_DrawWideStringAtOffset");

// The Japanese edition decodes the message (Message_DecodeRaw) into at most 32
// halfwords; the others copy its text (Message_CopyExpanded) into at most 52.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY Message_CopyExpanded__countdown
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY Message_DecodeRaw
#endif

/* VIEW(countdown): routine Window_Create as s32 Window_Create(s32, s32, s32, s32, s32); f04a85c has struct TargetWindow *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags). */
extern s32 Window_Create__countdown(s32, s32, s32, s32, s32) __asm__("Window_Create");

/* VIEW(countdown): routine Message_CopyExpanded as s32 Message_CopyExpanded(s32, s16 *, s32); f04a85c has void Message_CopyExpanded(s32 message, u16 *text, s32 capacity); tried as the definition's s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity), it does not compile in tla-ja: games/tla/src/window/f04a85c.c:977: conflicting types for `Message_CopyExpanded'. */
extern s32 Message_CopyExpanded__countdown(s32, s16 *, s32) __asm__("Message_CopyExpanded");

extern s32 gKeyState;
/* VIEW(countdown): variable MsgNoTimeToRun as extern char MsgNoTimeToRun; msg_ids.h has extern u8 MsgNoTimeToRun[]. */
extern char MsgNoTimeToRun__countdown __asm__("MsgNoTimeToRun");
extern u8 MsgNoTimeToRun[];

s32 Message_ShowLocalizedAndWait(void) {
  s16 buffer[64];
  s32 work;
  s32 result;
  void *state;
  s32 mode;
  s32 attrs;

  state = Ram_HeapSlots->battleWork;
  mode = gMainRoutineWork.battleRule24b;
  if (gMainRoutineWork.battleRule24b == 2 || mode <= 2 ||
      (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if (*(s8 *)((u8 *)state + 0x43) != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    attrs = 42;
    work = Window_Create__countdown(0, 7, 30, 4, attrs);
    Vram_FillWindowBlockPattern();
    BATTLE_MENU_MESSAGE_COPY((s32)&MsgNoTimeToRun__countdown, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
    Text_DrawWideStringAtOffset__countdown(buffer, work, 0, 4);
    do {
      wait(1);
    } while ((gInput.pressed & (KEY_A | KEY_B)) == 0 &&
             *(s32 *)((u8 *)((void **)Ram_HeapSlots)[57] + 0x4C) != 0);
    Window_Close(work, 1);
  }
  return result;
}
