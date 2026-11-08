// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja (16 of 16: no flag clear before patch 1), tla-de,
// tla-es, tla-fr and tla-it, with no device. The key was the struct: byte
// 0x17 is read through a struct with MapWork's own fields before it (map.h:
// origin, the three shake words, script, flags, wideTiles), so the bit-field
// load may alias the s32 store to gData_02006294. That memory dependence
// makes the ldrb wait on the store (subs r3, #13 issues between) and lifts
// the global's address load to the top, the native's order. A struct of only
// u8 bytes before the bit-field cannot alias an s32, and left differ 6 plain
// or 2 with an empty asm and a once-only block (the earlier drafts). Before
// adoption: MapWork's unk017 in map.h is this bit-field (bits 3..2 a mode,
// set to 1 here); the draft keeps its own copy of the layout until then.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "version.h"

struct MapWork17 {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 bits0 : 2;
  u8 mode : 2;
};

void Func_02003a14(void) {
  struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

  gData_02006294 = 0;
  map->mode = 1;
  Func_02005338(0);
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
#endif
}
