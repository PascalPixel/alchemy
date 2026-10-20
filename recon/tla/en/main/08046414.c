// Draft, differ 96 (alchemy-devtools diff ... 08046414; the stand-in parent's 26
// instructions count as draft-only): a nested function of Func_080464dc, the
// stat comparison window, which saves its static chain in its first frame
// slot and reads the parent's window from it. It puts an up or down arrow
// sprite (gDjinnMenu_ArrowUpTiles / ArrowDownTiles) at (x, y) of that
// window. It adopts with its parent; the parent here is a stand-in that only
// creates the window and calls it. Left (inside the nested function, about
// 72 rows): register choice (output and its sprite cell swap r6 and r7, x in
// r9 against r8), the attr2 zero held in r8 from the top, and the cell
// index's ldrb placed after the y store in the native.
#include "types.h"
#include "window.h"
#include "uiwindow.h"

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 flags);

extern const u8 gDjinnMenu_ArrowUpTiles[];
extern const u8 gDjinnMenu_ArrowDownTiles[];

// The sprite cell at RenderOutput.unk10: a link, then the OAM words.
struct ArrowSprite {
  struct ArrowSprite *next;
  u32 attr01;
  u32 attr2;
};

struct ArrowOam {
  struct ArrowSprite *next;
  u16 y : 8;
  u16 attr0 : 8;
  u16 x : 9;
  u16 attr1 : 7;
  u16 tile : 10;
  u16 attr2 : 6;
};

// Stand-in for the parent, Func_080464dc, which compares a unit's stats.
void Func_080464dc(struct UiWindow *window) {
  void Func_08046414(s32 x, s32 y, s32 up) {
    struct RenderOutput *output = RenderOutput_AcquireFree();
    struct ArrowOam *sprite;
    s32 entry;

    if (output == NULL)
      return;
    output->active = 1;
    output->kind = 1;
    entry = VramCache_AllocEntry(128);
    output->sentinel = 240;
    output->x = 120;
    output->y = 120;
    output->index = entry;
    sprite = (struct ArrowOam *)output->unk10;
    ((struct ArrowSprite *)sprite)->attr01 = 0x40000400;
    ((struct ArrowSprite *)sprite)->attr2 = 0;
    sprite->x = (s16)window->x * 8 + x;
    sprite->y = (s16)window->y * 8 + y;
    sprite->tile = VramCache_Reload((u8)output->index, up ? gDjinnMenu_ArrowUpTiles
                                                      : gDjinnMenu_ArrowDownTiles);
    RenderOutput_AppendToList(&window->output, output);
  }

  window = UiWindow_Create(0, 8, 20, 11, 6);
  Func_08046414(80, 14, 1);
}
