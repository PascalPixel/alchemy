#include "types.h"
#include "edition.h"
#include "io_reg.h"
#include "input.h"
#include "ram_buffer.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "task.h"
#include "window.h"

// The window that announces a Djinni, wider in French and Italian.
#if (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DJINN_JOIN_X 1
#define DJINN_JOIN_WIDTH 28
#else
#define DJINN_JOIN_X 2
#define DJINN_JOIN_WIDTH 26
#endif

extern const s16 gDjinn_ElementSprites[];
// Message 46, "<member> found the Venus Djinni <name>!", followed by the
// Mercury, Mars and Jupiter messages; still "message" in every catalog.
extern u8 MsgFoundVenusDjinni[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
s32 Portrait_GetForSpeaker(u32 value);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);
void MessageArgument_Push(u32 value, u32 kind);
s32 Message_DecodeMode1(s32 message);
// UB: Message_Start is called with five of its six arguments, so the mode it
// reads is whatever the stack word after the fifth holds (the 0 left there as
// UiGlyph_LoadEntryWithPalette's last argument); UBFIX declares it whole and
// passes 0.
#ifdef UBFIX
s32 Message_Start(struct Window *window, s32 entry, s32 x, s32 y, u16 *bounds,
                  s32 mode);
#else
s32 Message_Start();
#endif
void Sound_PlayFar(s32 cue);
void Window_Close(struct Window *window, s32 release);

void Func_08044a58(s32 member, s32 element, s32 djinn) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 name;
  struct Window *window;
  s32 sprite[3];
  s32 *entry;
  s32 tile, slot;
  s32 id;
  s32 text;
  // One zero for every 0 stored or passed below, as the Broken Seal's form
  // keeps it; as literal 0s, 58 of 124 instructions differ in tla-en.
  u32 zero;
  s32 *p;

  /* FAKEMATCH(dead): window set to NULL, which Window_Create's result replaces
   * at once; without that store, 77 of 124 instructions differ in tla-en. */
  window = NULL;
  zero = 0;
  name = gDjinn_ElementSprites[element & 3];
  entry = sprite;
  window = Window_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);

    work->dirty = 1;

    id = Portrait_GetForSpeaker(name);
    UiGlyph_LoadEntryWithPalette(id, zero, &slot, &tile, 14, zero);

    // The portrait as a bare OAM entry, pushed at depth 250 each frame: a link
    // word, attributes 0 and 1 (32x32 at y 12) and attribute 2, its tile with
    // palette 14.
    p = entry;
    *p++ = zero;
    *p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
    *p = tile | (14 << 12);

    work->speakerVoiceSound = zero;
    work->speakerVoiceCountdown = zero;

    MessageArgument_Push(member, 1);
    MessageArgument_Push(element * 20 + djinn + 300, 4);

    text = Message_DecodeMode1(element + (s32)MsgFoundVenusDjinni);

#ifdef UBFIX
    Message_Start(window, text, 36, 2, NULL, 0);
#else
    Message_Start(window, text, 36, 2, zero);
#endif

    Sound_PlayFar(81);

    do {
      // UB: Oam_AddSpriteAtDepth returns nothing; UBFIX calls it through its
      // declared type.
#ifdef UBFIX
      Oam_AddSpriteAtDepth(entry, 250);
#else
      /* FAKEMATCH(wrapper): called through a cast to the s32 function the
       * bytes show, which sets r1 to 250 before moving the entry to r0; as a
       * call by its void prototype, 2 of 124 instructions differ in tla-en. */
      ((s32 (*)(void *, s32))Oam_AddSpriteAtDepth)(entry, 250);
#endif
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    wait(1);
    Resource_ResetEntry(slot);
  }
}
