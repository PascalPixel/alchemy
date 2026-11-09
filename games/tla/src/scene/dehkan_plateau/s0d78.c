#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
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

/// A shape grid byte seen as a bit-field, so its store takes the struct's
/// alias set (see the FAKEMATCH tag in Func_02000d78).
struct ShapeBits {
  u32 shape : 8;
};

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

void Func_02000d78(u16 *list) {
  struct Actor *held = Func_02003df0();
  void *work = Ram_HeapSlots->eventWork;
  u8 *shapeGrid = gMapShapeGrid;

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
      struct MapCell *cells = Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shapes = shapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
      s32 shape = Func_02003c10(*layer, held->x, held->z);
      s32 level = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Func_02003db0(held);
      s32 value;

      shapes += index;
      value = Func_02003dd0(*layer, shape, (level >> 19) - 4);
      cells += index;
      /* FAKEMATCH(signature): the shape grid byte stored through a bit-field
         view (struct ShapeBits), whose alias set lets the cell's attribute be
         read first; as `*shapes = value;`, 4 of 131 instructions differ in
         tla-en. */
      ((struct ShapeBits *)shapes)->shape = value;
      cells->attributeB &= 63;
      cells += 128;
      cells->attributeB &= 191;
      HELD_OBJECT(work) = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}
