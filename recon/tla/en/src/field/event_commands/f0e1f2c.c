#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"
#include "resource.h"
#include "task.h"
#include "io_reg.h"

// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.
struct BowMotion {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 delay;
};

struct BowWork {
  u8 *tiles;
  struct BowMotion sparks[32];
  struct BowMotion arrowMotion;
  struct EffectSprite arrow;
  struct BowMotion bowMotion;
  struct EffectSprite bow;
  s32 start[3];
  s32 target[3];
  s16 phase;
  s16 timer;
  u16 bowTile;
  s16 frame;
  s8 bowGrowing;
  s8 bowShrinking;
  s8 sparking;
  s8 sparksRepeat;
  s16 result;
  s8 done;
  s8 arrowMoving;
};

struct BowScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
};

extern char ResourceId_BowDraw, ResourceId_FlamingArrow;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void BattleEffect_InitializeSharedScene(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e1678(void);

void Func_080e1f2c(void) {
  struct BowWork *work = HeapSlot_Alloc(92, sizeof(struct BowWork));
  struct BowScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct BowMotion *motion;
  s32 pos[3];
  s32 arrowSlot;
  s32 bowSlot;
  s32 tile;
  u16 *palette;
  s32 blue;
  s32 shade;
  s32 i;

  BattleEffect_InitializeSharedScene();
  // The spark pointer and the sprite pointer are one variable.
  sprite = (struct EffectSprite *)work->sparks;
  for (i = 0; i < 32; i++) {
    ((struct BowMotion *)sprite)->delay = -i;
    sprite = (struct EffectSprite *)((struct BowMotion *)sprite + 1);
  }

  work->tiles = HeapSlot_Alloc(96, 0x1800);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_BowDraw),
                        work->tiles);
  bowSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(bowSlot, 0x1800, work->tiles);
  work->bowTile = tile;
  sprite = &work->bow;
  Func_080eaf98(sprite, 16, 16, 0x80000000, tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainObject);
  // volatile keeps the depth store before the scale stores.
  *(volatile s16 *)&sprite->depth =
      BattleEffect_GetObjectDepth(scene->mainObject) + 1;
  *(volatile s32 *)&sprite->scaleX = 0x6666;
  *(volatile s32 *)&sprite->scaleY = 0x6666;
  sprite->angle = scene->angle;
  pos[0] = scene->mainObject->x;
  pos[2] = scene->mainObject->z;
  motion = &work->bowMotion;
  Vector_AddPolarOffset(0x80000, scene->angle, pos);
  motion->x = pos[0];
  motion->y = 0x100000;
  motion->z = pos[2];

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_FlamingArrow),
                        work->tiles);
  arrowSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(arrowSlot, 0x100, work->tiles);
  sprite = &work->arrow;
  Func_080eaf98(sprite, 12, 8, 0x40000000, tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainObject);
  *(volatile s16 *)&sprite->depth =
      BattleEffect_GetObjectDepth(scene->mainObject) + 2;
  *(volatile s32 *)&sprite->scaleX = 0;
  *(volatile s32 *)&sprite->scaleY = 0;
  sprite->angle = scene->angle;
  pos[0] = scene->mainObject->x;
  pos[2] = scene->mainObject->z;
  motion = &work->arrowMotion;
  Vector_AddPolarOffset(0x120000, scene->angle, pos);
  motion->x = pos[0];
  motion->y = 0x100000;
  motion->z = pos[2];
  HeapSlot_Release(96);

  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 3;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->command = 0;
  canvas->angle = scene->angle;
  work->start[0] = scene->mainObject->x;
  work->start[1] = scene->mainObject->y;
  work->start[2] = scene->mainObject->z;
  blue = 31 << 10;
  work->target[0] = scene->mainObject->x;
  work->target[1] = scene->mainObject->y;
  work->target[2] = scene->mainObject->z;
  Vector_AddPolarOffset(0x500000, scene->angle, work->target);

  palette = (u16 *)&OBJ_PLTT_COLOR(14, 0);
  for (i = 0; i < 16; i++) {
    shade = i + 16;
    // A once-only block: the store stays ahead of the loop's increments.
    do {
      *palette = (shade << 5 | blue) | shade;
    } while (0);
    palette++;
  }

  Sound_PlayFar(130);
  wait(1);
  work->bowGrowing = 0;
  work->bowShrinking = 0;
  work->done = 0;
  work->result = -1;
  work->phase = 0;
  work->timer = 0;
  Task_Add(Func_080e1678, TASK_PASS_VBLANK);
  while (work->phase != 999)
    wait(1);
  Task_Remove(Func_080e1678);
  if (work->done != 0 && work->result != -1)
    event[0xd30] = work->result;
  Resource_ResetEntry(arrowSlot);
  Resource_ResetEntry(bowSlot);
  BattleEffect_PrepareBufferInterpolation();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}
