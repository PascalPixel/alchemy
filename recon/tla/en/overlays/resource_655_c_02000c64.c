// Draft, differ 109 of 123: raises each listed actor's cells (the shape
// grid and the cell flags of its cell and the one below). Left: register
// allocation (the native spills the shape and keeps the level in r10).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct ObjectRuntime *object, struct ObjectRuntime *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct ObjectRuntime *object, s32 mode);
void Func_02003ba8(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct ObjectRuntime *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct ObjectRuntime *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
void Effect_AdvanceMotionAtLeaderPriority(struct ObjectRuntime *object);

/// The effect fields of an ObjectRuntime, as field_effect.h names them.
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
  void (*update)(struct ObjectRuntime *object);
};

void Func_02000c64(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ObjectRuntime *leader;
  s32 height;

  while (*list != 0xffff) {
    struct ObjectRuntime *object = Object_GetById(*list);
    u8 *layer = &object->terrainId;
    s32 shape = Func_02003c10(*layer, object->x, object->z);
    s32 level = Map_GetTerrainHeight(*layer, object->x, object->z);
    s32 index;
    u8 *cells;
    u8 *shapes;

    level >>= 19;
    index = Func_02003db0(object);
    cells = (u8 *)map->layers[*layer].cells;
    shapes = gMapShapeGrid +
             (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    level += 4;
    shape = Func_02003dd0(*layer, shape, level);
    shapes += index;
    cells += index * 4;
    cells[3] |= 192;
    cells += 0x200;
    cells[3] |= 64;
    list++;
    *shapes = shape;
  }
  leader = Object_GetById(gPartyState.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
