// Draft, differ 92 of 150: Func_020001d0 with actors 8 to 10 placed at
// the leader first and attributes copied for set flags. Left: register
// choice (the native keeps the list in r11; this draft spills it).
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

  Call2((void (*)())Func_02003c78, 8, gMainRoutineWork.currentOwner);
  Call2((void (*)())Func_02003c78, 9, gMainRoutineWork.currentOwner);
  Call2((void (*)())Func_02003c78, 10, gMainRoutineWork.currentOwner);
  Call1((void (*)())wait, 1);
  Call1((void (*)())Func_0200056c, 8);
  Call1((void (*)())Func_0200056c, 9);
  Call1((void (*)())Func_0200056c, 10);
  Call2((void (*)())Actor_SetModeById, 9, 0);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_02003c68, 8, 0, 0);
  Call3((void (*)())Func_02003c68, 9, 0, 0);
  Call3((void (*)())Func_02003c68, 10, 0, 0);
  Call1((void (*)())wait, 1);
  goto test;
next:
  {
    s32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    u8 *layer = &object->terrainId;
    struct MapCell *cells = map->layers[*layer].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(flag + 1)) {
      Func_02003c68(id, 0, 0);
    } else {
      s32 index = Func_02003db0(object);
      u8 *cell = (u8 *)(cells + index);
      u8 *slot = shapes + index;
      s32 shape = Func_02003c10(*layer, object->x, object->z);
      s32 height = Map_GetTerrainHeight(*layer, object->x, object->z);

      Func_02003cc0(id);
      cell[3] = 128;
      *slot = Func_02003dd0(*layer, shape, (height >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
test:
  if (*list != 0xffff)
    goto next;
  Call1((void (*)())wait, 10);
}
