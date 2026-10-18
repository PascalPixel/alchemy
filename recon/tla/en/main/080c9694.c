// Draft, differ 106 of 109: picks the scene's arrival entrance. The scene
// overlay's sixth entry returns its exit table: headers (a word under
// 0x1000, 0x1ff ends the table) each followed by entries packing the scene
// (low 12 bits, 0x1ff ends), the entrance (bits 12 to 19), the exit (bits
// 20 to 27, 255 any, 0 ends; SCENE_EXIT in field_scene.h) and bit 28 marking a following flag
// word. Under the current scene's header, the first entry whose source
// matches the exit taken and whose flag (0 and -1 none) is clear sets the scene
// and entrance; an end takes the first entry's scene, entrance 0. Left:
// block order and loop invariants: the native places the header search after
// the entry loop and the entry loop's tests before its body, and rebuilds
// the 0xfff, 0x1ff and 0xff masks in each pass, where the draft's loops are
// laid out in source order with the masks hoisted (r4, ip); the logic
// matches. Tried: the entry loop as a do-while on its accept test (differ
// 160).
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);

// The scene overlay's entry table, whose sixth entry returns its exit table.
extern void *gOverlayArea[];

void Func_080c9694(s32 exit) {
  s32 current = gPartyState.scene;
  u32 *table = ((u32 * (*)(void)) gOverlayArea[5])();
  s32 entrance = 0;
  s32 scene = 999;
  u32 *entries;
  u32 word;
  s32 id;
  s32 from;
  s32 kind;
  s32 flag;

  if (exit == 999)
    return;
  for (;;) {
    do
      word = *table++;
    while (word & 0xfffff000);
    id = word & 0xfff;
    if (id == 0x1ff)
      goto done;
    if (id == current)
      break;
  }
  entries = table;
  for (;;) {
    word = *table++;
    from = (word & 0xff000) >> 12;
    kind = (word & 0xff00000) >> 20;
    id = word & 0xfff;
    flag = word & 0x10000000;
    if (flag)
      flag = *table++;
    if (id == 0x1ff || kind == 0) {
      scene = *entries & 0x1ff;
      break;
    }
    if (kind == 255 || kind == exit) {
      if (flag == 0 || flag == -1 || !GameFlag_Test(flag)) {
        scene = id;
        entrance = from;
        break;
      }
    }
  }
done:
  if (scene != 999) {
    gPartyState.scene = scene;
    gPartyState.entrance = entrance;
  }
}
