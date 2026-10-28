// Near miss, differ 34 (2026-10-26, statements reordered).
// First draft of Func_080e1f2c, written from the listing: a bow game on the
// field (resources 0x1e7, FieldEffect_BowDraw, and 0x1e6,
// FieldEffect_FlamingArrow): the bow and arrow sprites before the caster, the
// canvas sprite, palette 14 a ramp of blues, then Func_080e1678 runs as a
// task until its counter reaches 999; a result other than -1 is
// stored in the event work at 0xd30. Differ 58 of 370: instruction order
// only, the native issuing loads earlier than this (the sprite's byte 9, the
// caster's x, the second half of 0x7c00) at about a dozen places. The entry
// pointer and the sprite pointer are one variable, as the native's r7.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"
#include "resource.h"
#include "task.h"

struct BowEntry {
  u8 unk00[24];
  s32 delay;
};

struct BowVector {
  s32 x;
  s32 y;
  s32 z;
};

struct BowWork {
  u8 *tiles;
  struct BowEntry entries[32];
  struct BowVector arrowPosition;
  u8 unk390[0x10];
  struct EffectSprite arrow;
  struct BowVector bowPosition;
  u8 unk3d4[0x10];
  struct EffectSprite bow;
  struct BowVector start;
  struct BowVector target;
  s16 counter;
  s16 unk426;
  u16 bowTile;
  u8 unk42a[2];
  s8 unk42c;
  s8 unk42d;
  u8 unk42e[2];
  s16 result;
  s8 done;
  u8 unk433;
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
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct BowVector *pos);
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
  struct BowVector *vector;
  struct BowVector pos;
  s32 arrowSlot;
  s32 bowSlot;
  s32 tile;
  u16 *palette;
  s32 blue;
  s32 i;

  BattleEffect_InitializeSharedScene();
  sprite = (struct EffectSprite *)work->entries;
  for (i = 0; i < 32; i++) {
    ((struct BowEntry *)sprite)->delay = -i;
    sprite = (struct EffectSprite *)((struct BowEntry *)sprite + 1);
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
  sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 1;
  sprite->scaleY = sprite->scaleX = 0x6666;
  sprite->angle = scene->angle;
  pos.z = scene->mainObject->z;
  pos.x = scene->mainObject->x;
  vector = &work->bowPosition;
  Vector_AddPolarOffset(0x80000, scene->angle, &pos);
  vector->x = pos.x;
  vector->y = 0x100000;
  vector->z = pos.z;

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_FlamingArrow),
                        work->tiles);
  arrowSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(arrowSlot, 0x100, work->tiles);
  sprite = &work->arrow;
  Func_080eaf98(sprite, 12, 8, 0x40000000, tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainObject);
  sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 2;
  sprite->scaleY = sprite->scaleX = 0;
  sprite->angle = scene->angle;
  pos.z = scene->mainObject->z;
  pos.x = scene->mainObject->x;
  vector = &work->arrowPosition;
  Vector_AddPolarOffset(0x120000, scene->angle, &pos);
  vector->x = pos.x;
  vector->y = 0x100000;
  vector->z = pos.z;
  HeapSlot_Release(96);

  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 3;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->command = 0;
  canvas->angle = scene->angle;
  work->start.x = scene->mainObject->x;
  work->start.y = scene->mainObject->y;
  work->start.z = scene->mainObject->z;
  blue = 31 << 10;
  work->target.x = scene->mainObject->x;
  work->target.y = scene->mainObject->y;
  work->target.z = scene->mainObject->z;
  Vector_AddPolarOffset(0x500000, scene->angle, &work->target);

  palette = (u16 *)0x050003c0;
  for (i = 0; i < 16; i++)
    *palette++ = ((i + 16) << 5 | blue) | (i + 16);

  Sound_PlayFar(130);
  wait(1);
  work->unk42c = 0;
  work->unk42d = 0;
  work->done = 0;
  work->result = -1;
  work->counter = 0;
  work->unk426 = 0;
  Task_Add(Func_080e1678, TASK_PASS_VBLANK);
  while (work->counter != 999)
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
