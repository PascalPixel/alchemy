#include "task.h"
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "render_input.h"
#include "resource.h"
#include "ram_buffer.h"
#include "window.h"
#include "input.h"

extern u8 MsgPairJoinedParty[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
s32 Func_08044b80(u32 index);
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
void wait(s32 frames);
s32 Sound_GetBgmRestoreDelayFar(void);
void Window_Close(struct Window *window, s32 release);

void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct Window *window;
  s32 sprite2[3];
  s32 sprite1[3];
  s32 *entry1;
  s32 *entry2;
  s32 handle1, palette, handle2;
  s32 *p;

  /* FAKEMATCH(dead): window set to NULL, which Window_Create's result replaces
   * at once; without that store, 27 of 135 instructions differ in tla-en. */
  window = NULL;
  entry1 = sprite1;
  window = Window_Create(1, 1, 28, 5, 0);

  if (window != NULL) {
    Window_DrawDividerLine(window, 8, 0, 4, 4);

    work->dirty = 1;

    UiGlyph_LoadEntryWithPalette(Portrait_GetForSpeaker(Func_08044b80(first)),
                                 0, &handle1, &palette, 14, 0);
    // The portraits as bare OAM entries, pushed at depth 250 each frame: a
    // link word, attributes 0 and 1 (32x32 at y 12, x 12 and 44) and
    // attribute 2, the tile with palette 14 or 15.
    p = entry1;
    *p++ = 0;
    *p++ = 0x800c000c;
    *p = palette | 0xe000;

    entry2 = sprite2;
    UiGlyph_LoadEntryWithPalette(Portrait_GetForSpeaker(Func_08044b80(second)),
                                 0, &handle2, &palette, 15, 0);
    p = entry2;
    *p++ = 0;
    *p++ = 0x802c000c;
    *p = palette | 0xf000;

    work->speakerVoiceSound = 0;
    work->speakerVoiceCountdown = 0;

    MessageArgument_Push(first, 1);
    MessageArgument_Push(second, 1);

#ifdef UBFIX
    Message_Start(window, Message_DecodeMode1((s32)MsgPairJoinedParty), 68, 2,
                  NULL, 0);
#else
    Message_Start(window, Message_DecodeMode1((s32)MsgPairJoinedParty), 68, 2,
                  0);
#endif

    Sound_PlayFar(81);

    do {
      // UB: Oam_AddSpriteAtDepth returns nothing; UBFIX calls it through its
      // declared type.
#ifdef UBFIX
      Oam_AddSpriteAtDepth(entry1, 250);
      Oam_AddSpriteAtDepth(entry2, 250);
#else
      /* FAKEMATCH(wrapper): both pushes called through a cast to the s32
       * function the bytes show, which sets r1 to 250 before moving each entry
       * to r0; as calls by its void prototype, 4 of 135 instructions differ in
       * tla-en. */
      ((s32 (*)(void *, s32))Oam_AddSpriteAtDepth)(entry1, 250);
      ((s32 (*)(void *, s32))Oam_AddSpriteAtDepth)(entry2, 250);
#endif
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    wait(1);
    Resource_ResetEntry(handle1);
    Resource_ResetEntry(handle2);
  }
}
