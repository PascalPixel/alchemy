// Draft, differ 85 of 131, skeleton gap 11/8 (tla-en, 2026-11-01)
// Lowers the cells under the held actor (or the one the leader carries) and
// keeps it at 0xcbc of the event work.
// Settled: goto loop (a while hoists held + 34 out of the loop and cross-jumps
// nothing); list spills to sp+4 and work to sp+0 once the seven callee-saved
// registers go to held, layer, cells, shapes, shape, level, index; level takes
// two statements, or it dies twice and leaves local-alloc.
// Left: the native keeps one cells pointer in r10 through `cells += index`,
// the first update, `cells += 128` and the second update, with index tied to
// r6 and level in r5. Written that way (cells += index, or a cell local, an
// under local, a static inline or a one-pass macro) the tied quantity has 20
// refs across 4 calls, so CALLER_SAVE_PROFITABLE puts it in r4 once r5 and r6
// are taken: differ 101, list back in a register. The native has no r4 and no
// save slot, so its quantity has at most 16 refs and ranks under shapes, and no
// form tried gets there. Combine's refs come from the life pass, and regmove
// does not recount; thumb add never reaches regmove (LO_REGS is likely
// spilled), so only a real copy could leave a stale count, and CSE removes
// every copy tried. cells[index] below avoids r4 but ties the sum to index,
// which swaps r5 and r6 and ends in a three-operand add.
// `shape = Func_02003dd0(...); shapes[index] = shape;` gives differ 78 but gap
// 16/16 (held in r9). Bit-field views of attributeB change nothing.
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

s32 Func_02003d18(s32 owner, s32 mode);
struct Actor *Func_02003df0(void);

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

void Func_02000d78(u16 *list) {
  struct Actor *held = Func_02003df0();
  void *work = Ram_HeapSlots->eventWork;

  if (held == NULL) {
    s32 id = Func_02003d18(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  HELD_OBJECT(work) = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      struct MapCell *cells =
          Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shapes =
          gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
      s32 shape = Func_02003c10(*layer, held->x, held->z);
      s32 level = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Func_02003db0(held);

      level >>= 19;
      level -= 4;
      shapes += index;
      *shapes = Func_02003dd0(*layer, shape, level);
      cells[index].attributeB &= 63;
      cells[index + 128].attributeB &= 191;
      HELD_OBJECT(work) = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}
