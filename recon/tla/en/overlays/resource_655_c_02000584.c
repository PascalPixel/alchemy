// Matches (tla-en, 2026-11-08, -mtune=gba); ja, de, es, fr and it measure 0 too.
// Closed from 8: the last call reads object->terrainId rather than through
// terrain (4), and the grid's address is a local held for the whole function
// (0). No device. Everything below is the history of older compilers.
// Draft, differ 62 of 150, skeleton gap 12 (tla-en, 2026-11-01)
// Func_020001d0 with actors 8 to 10 placed at the leader first and attributes
// copied for set flags. Ported from 67F's Func_02000584, the same function; the
// old Call forwarding is gone (no devices here). Control flow and data access
// are right; what is left is one allocation.
// Mechanism, read from global.c and the lreg and greg dumps: the listing's frame
// 16 is map 12, flag 8, cell 4, and index * 4 at 0 with its store deleted. Cell
// and index * 4 are tied in r6 by local-alloc, then kicked out (find_reg's
// local_reg_n_refs test) by cells, which is what puts cells in r6 and spills
// both; sp+0 is index * 4's slot, not a missing local. 020001d0 does the same.
// For that, three counts must hold (here, or in the listing's order):
// - terrain ranks above cells, so it takes r7: 10 refs over T insns against 6
//   over C. `next` before `terrain` gives T 45, C 18, a tie that the lower pseudo
//   number wins, so terrain is declared first.
// - cells kicks r6 only if 6 / C > 8 / L, L being the two tied live lengths. T 45,
//   C 18, L 24 is exactly equal, and the test is strict: this is the miss.
// - id ranks above list, or they swap r9 and r11: one `list += 2` (refs 12), not
//   one per arm (16). With one per arm the swap shows at differ 58, gap 16, with
//   index reused as the shape pointer; that reuse is a second value, so not kept.
// Computing height before cell->attributeB = 128 gives L 26 but T 47, so cells
// goes first (differ 90). Only a load moved before the cell add also gives C 19:
// `x = object->x` as a local does it, and cells, terrain and frame all land (gap
// 5/7 with one per arm), but x is a forced temporary and the native rereads it
// for the second call. So the lead is a form with C 19 or L 25 that adds no
// instruction: every insn the listing has is already here, so it is an order.
// Open: with that local and one `list += 2`, cell takes r11 and list loses it,
// though list ranks higher; not understood (no spill of r11 shows in the dump).
// Tried, no gain: goto loop (66), goto with one per arm (87), cell set before
// slot (89), height reusing ground (102), a split
// offset local (folds, 90), one-constant locals for 0, 1 or 9 in r6 (cprop folds
// them, 58 unchanged), the 691 draft's two-word pair (a forced store).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  // Holds the grid's address for the whole function.
  u8 *grid = gMapShapeGrid;

  Func_02003c78(8, gMainRoutineWork.currentOwner);
  Func_02003c78(9, gMainRoutineWork.currentOwner);
  Func_02003c78(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_0200056c(8);
  Func_0200056c(9);
  Func_0200056c(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_02003c68(8, 0, 0);
  Func_02003c68(9, 0, 0);
  Func_02003c68(10, 0, 0);
  wait(1);
  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    u8 *terrain = &object->terrainId;
    struct MapCell *cells = map->layers[*terrain].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(next)) {
      Func_02003c68(id, 0, 0);
    } else {
      s32 index = Func_02003db0(object);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape = Func_02003c10(*terrain, object->x, object->z);
      ground = Map_GetTerrainHeight(*terrain, object->x, object->z);

      Func_02003cc0(id);
      cell->attributeB = 128;
      *slot = Func_02003dd0(object->terrainId, shape, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
  wait(10);
}
