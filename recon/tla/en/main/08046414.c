// Draft, differ 24 of 80, skeleton gap 24 (tla-en, 2026-11-01): every one of
// the 24 is the stand-in parent's code, counted as draft-only; the 80
// instructions of the nested function itself compile exactly. Twin of The
// Broken Seal's DjinnMenu_DrawStatArrow, nested in
// BattleDjinnMenu_ShowChangePreview (games/tbs/src/window/screens/
// battle_djinn_preview.c): the same call order, the same 16 by 16 sprite on
// layer 240 at 0x40000400, the same window chain and arrow tiles. It is a
// nested function of Func_080464dc, the stat comparison window, which saves
// its static chain in its first frame slot and reads the parent's window from
// it. It adopts with its parent; the parent here is a stand-in that only
// creates the window and calls it. The twin's window device carries over,
// tagged; the twin's index cast is needed (one row differs without it). Its
// pointer-typed attribute stores and zero temporary are not, and are plain
// here. Left: nothing in the function; the differ reaches 0 only when it is
// measured with its real parent.
#include "types.h"
#include "window.h"
#include "uiwindow.h"

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 flags);

extern const u8 gDjinnMenu_ArrowUpTiles[];
extern const u8 gDjinnMenu_ArrowDownTiles[];

// The sprite cell at WindowSprite.unk10: a link, then the OAM words.
struct ArrowSprite {
  u32 link;
  union {
    u32 word;
    struct {
      unsigned y : 8;
      unsigned flags : 8;
      unsigned x : 9;
      unsigned high : 7;
    } bits;
  } attributes;
  union {
    u32 value;
    struct {
      unsigned index : 10;
      unsigned high : 6;
    } bits;
  } tile;
};

// Stand-in for the parent, Func_080464dc, which compares a unit's stats.
void Func_080464dc(struct Window *window) {
  void Func_08046414(s32 x, s32 y, s32 up) {
    struct WindowSprite *output = WindowSprite_AcquireFree();
    struct ArrowSprite *entry;

    if (output) {
      output->active = 1;
      output->kind = 1;
      // Stored through a cast, the index stays after the position stores.
      *(s8 *)&output->index = VramCache_AllocEntry(128);
      entry = (struct ArrowSprite *)output->unk10;
      output->sentinel = 240;
      output->x = 120;
      output->y = 120;
      entry->attributes.word = 0x40000400;
      entry->tile.value = 0;
      entry->attributes.bits.x = (s16)window->x * 8 + x;
      entry->attributes.bits.y = (s16)window->y * 8 + y;
      /* FAKEMATCH(dead): advancing and restoring the captured pointer keeps
       * the window chain in r9; as no pair, 32 of 80 instructions differ in
       * tla-en. */
      window++;
      window--;
      entry->tile.bits.index = VramCache_Reload(
          (u8)output->index,
          up ? gDjinnMenu_ArrowUpTiles : gDjinnMenu_ArrowDownTiles);
      WindowSprite_AppendToList(&window->output, output);
    }
  }

  window = Window_Create(0, 8, 20, 11, 6);
  Func_08046414(80, 14, 1);
}
