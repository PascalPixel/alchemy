// Matches: differ 0 of 95 (tla-en, 2026-10-30), with one tagged device:
// Oam_AddSpriteAtDepth declared returning s32. From ☀️ Party_ShowJoinedMessage.
// Declared void, 2 of 95 differ: in the wait loop the entry pointer is moved
// to r0 before r1 takes 250 (native: r1 first). sched2 gives both moves the
// same priority and then prefers the one more later instructions depend on;
// a call that sets r0 takes wait(1)'s r0 store off the entry move, so r1 goes
// first. Under -mthumb-slow-loads no single empty asm, one-pass block,
// wrapping block or swap of neighbouring statements brought the void form
// closer, nor any pair of empty asm statements or one-pass blocks (device
// search, 2026-10-26), nor a for (;;) loop with breaks or passing sprite
// itself (2026-10-30).
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "types.h"
#include "input.h"
#include "window.h"
#include "uiflow.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

void Window_DrawDividerLine(struct Window *, s32, s32, s32, s32);
void MessageArgument_Push(s32, s32);
struct UiChannelSlot *Message_Start();
void Sound_PlayFar(s32);
s32 Func_08044b80(u32 index);
extern u8 MsgJoinedParty[];
struct Window *Window_Create(s32 x, s32 y, s32 w, s32 h, s32 style);
s32 Oam_AddSpriteAtDepth(void *entry, s32 priority);

void Func_08044b98(s32 member) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct Window *window = NULL;
  s32 sprite[3];
  s32 *entry = sprite;
  s32 palette, handle;
  s32 *p;

  window = Window_Create(2, 1, 26, 5, 0);
  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);
    work->dirty = 1;
    UiGlyph_LoadEntryWithPalette(
        Portrait_GetForSpeaker(Func_08044b80(member)), 0, &handle, &palette,
        14, 0);
    p = entry;
    *p++ = 0;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = palette | 0xe000;
    work->speakerVoiceSound = 0;
    work->speakerVoiceCountdown = 0;
    MessageArgument_Push(member, 1);
    Message_Start(
        window, Message_DecodeMode1((s32)MsgJoinedParty), 36, 2, 0);
    Sound_PlayFar(81);
    /* FAKEMATCH(signature): Oam_AddSpriteAtDepth declared returning s32,
     * though it returns nothing; as void Oam_AddSpriteAtDepth(void *, s32),
     * 2 of 95 instructions differ in tla-en. */
    do {
      Oam_AddSpriteAtDepth(entry, 250);
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 && (gInput.pressed & 0x303) == 0);
    Window_Close(window, 2);
    wait(1);
    Resource_ResetEntry(handle);
  }
}
