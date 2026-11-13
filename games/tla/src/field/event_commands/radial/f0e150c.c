/// @file
/// The arc effect, and the map cells and events a field Psynergy touches.
// Part of radial.c.

#include "../radial_internal.h"

/* VIEW(f0e150c): tag struct ArcSprite as struct ArcSprite__f0e150c; radial.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/event_commands/radial.c:1599: structure has no member named `x'. */
struct ArcSprite__f0e150c {
  u8 unk00[12];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  s16 depth;
  u8 unk20[8];
};

/* VIEW(f0e150c): tag struct ArcWork as struct ArcWork__f0e150c; radial.c has another definition. */
struct ArcWork__f0e150c {
  struct ArcSprite__f0e150c sprites[2][2];
  s16 age[2];
  s16 resource;
  s16 group;
  struct Actor *target;
};

/* VIEW(f0e150c): routine Camera_WorldToScreen as void Camera_WorldToScreen(s32 *position); launch has void Camera_WorldToScreen(struct SpinVector *position). */
extern void Camera_WorldToScreen__f0e150c(s32 *position) __asm__("Camera_WorldToScreen");
void Camera_WorldToScreen(struct EffectVector *position);
s32 BattleEffect_GetObjectDepth(struct Actor *object);

void BattleEffect_UpdateArcEffect(void) {
  struct ArcWork__f0e150c *work = Ram_HeapSlots->block62;
  struct Actor *target = work->target;
  s32 group;
  s32 position[3];
  s32 i;

  for (group = 0; group < 2; group++) {
    s32 scale = Trig_Sin(work->age[group] << 10);

    if (work->age[group] < 0)
      continue;
    if (work->age[group] < 32) {
      struct ArcSprite__f0e150c *sprite;
      s32 x, y;

      position[0] = target->x;
      position[1] = target->y + (work->age[group] << 16);
      position[2] = target->z;
      Camera_WorldToScreen__f0e150c(position);
      x = position[0];
      y = position[2];
      for (i = 0; i < 2; i++) {
        sprite = &work->sprites[group][i];
        sprite->x = x;
        sprite->y = y;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        if (i == 0) {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) - 1;
        } else {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) + 1;
          sprite->y += 0xffff0000;
          sprite->scaleY = -sprite->scaleY;
        }
        Func_080eb01c(sprite);
      }
      work->age[group]++;
    }
  }
}


void BattleEffect_StartArcEffect(void) {
  struct ArcEffectWork *work = HeapSlot_Alloc(248, sizeof(*work));

  work->resource = Resource_FindFreeEntry();
  VramCache_Load(work->resource, 0x100, gBattleEffect_ArcTiles);
  work->group = 0;
  work->age[0] = 999;
  work->age[1] = 999;
  work->target = 0;
  Task_Add((s32)BattleEffect_UpdateArcEffect, TASK_PASS_VBLANK);
}

void BattleEffect_EndArcEffect(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->block62;

  Resource_ResetEntry(work->resource);
  Task_Remove((u32)BattleEffect_UpdateArcEffect);
  HeapSlot_Release(248);
}


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

struct BowEvent {
  u8 unk00[20];
  struct Actor *objects[80];
};

extern const s32 Data_080f0f34[8];
extern const s32 Data_080f0f54[4];
extern const u8 Data_080eda0c[];

/* VIEW(f0e150c): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position); f0dfd00 has void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *p). */
extern void Vector_AddPolarOffset__f0e150c(s32 distance, s32 angle, s32 *position) __asm__("Vector_AddPolarOffset");
s32 Func_08020210(struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height);

#define BOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e1678(void) {
  struct BowScene *scene = Ram_HeapSlots->effectWork;
  struct BowWork *work = (struct BowWork *)Ram_HeapSlots->battleFxWork[0];
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->unkec[1];
  struct BowEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct BowMotion *motion;
  struct EffectSprite *sprite;
  s32 pos[3];
  s32 objectIndex;
  s32 i;
  s32 dx;
  s32 dz;

again:
  switch (work->phase) {
  hit:
    work->done = 1;
    work->result = objectIndex;
    BOW_NEXT_PHASE(work);
    goto again;
  case 0:
    if (work->timer == 0) {
      work->sparking = 0;
      work->sparksRepeat = 1;
      work->arrowMoving = 0;
      work->bowGrowing = 1;
      work->frame = 999;
    } else if (work->bowGrowing == 0) {
      work->frame = 0;
      BOW_NEXT_PHASE(work);
    }
    break;
  case 1:
    if (work->timer == 0) {
      sprite = &work->arrow;
      sprite->scaleX = sprite->scaleY = 0x10000;
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset__f0e150c(0x2666, scene->angle + 0x8000, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      work->arrowMoving = 1;
    }
    if (work->timer == 20)
      work->sparking = 1;
    if (work->timer == 56) {
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset__f0e150c(0x50000, scene->angle, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      BOW_NEXT_PHASE(work);
    }
    break;
  case 2:
    motion = &work->arrowMotion;
    if (work->timer == 90) {
      BOW_NEXT_PHASE(work);
      break;
    }
    dx = work->target[0] - work->start[0];
    if (dx < 0)
      dx = work->start[0] - work->target[0];
    dz = motion->x - work->start[0];
    if (dz < 0)
      dz = work->start[0] - motion->x;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    dx = work->target[2] - work->start[2];
    if (dx < 0)
      dx = work->start[2] - work->target[2];
    dz = motion->z - work->start[2];
    if (dz < 0)
      dz = work->start[2] - motion->z;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    pos[0] = motion->x;
    pos[1] = motion->y;
    pos[2] = motion->z;
    if (Func_08020210(scene->mainObject, pos) > 0) {
      work->done = 1;
      work->result = -1;
      work->phase++;
      work->timer = -1;
      goto again;
    }
    for (objectIndex = 0; objectIndex < 80; objectIndex++) {
      object = event->objects[objectIndex];
      if (object != NULL && object->script != NULL &&
          (object->collisionFlags & 1) && object != scene->mainObject &&
          Func_080dbe80(&motion->x, 2, &object->x, object->radius - 2) >= 0)
        goto hit;
    }
    break;
  case 3:
    if (work->done != 0) {
      work->arrowMoving = 0;
    } else {
      work->sparksRepeat = 0;
      work->bowShrinking = 1;
    }
    motion = &work->arrowMotion;
    pos[0] = pos[1] = pos[2] = 0;
    Vector_AddPolarOffset__f0e150c(0x20000, scene->angle, pos);
    motion->velocityX = pos[0];
    motion->velocityY = pos[1];
    motion->velocityZ = pos[2];
    BOW_NEXT_PHASE(work);
    break;
  case 4:
    sprite = &work->arrow;
    if (work->done != 0) {
      if (work->timer < 32) {
        s32 wobble[8];
        s32 fade[4];
        s32 w;

        memcpy(wobble, Data_080f0f34, sizeof(wobble));
        memcpy(fade, Data_080f0f54, sizeof(fade));
        w = wobble[work->timer & 7];
        sprite->angle = scene->angle + w * fade[work->timer / 8] / 0x10000;
      }
      if (work->timer == 32)
        work->bowShrinking = 1;
      if (work->timer == 50) {
        sprite->scaleX = sprite->scaleY = 0;
        work->arrowMoving = 1;
        work->sparksRepeat = 0;
        BOW_NEXT_PHASE(work);
      }
    } else {
      sprite->scaleX += -0x3000;
      sprite->scaleY = sprite->scaleX;
      if (sprite->scaleX < 0) {
        sprite->scaleX = sprite->scaleY = 0;
        BOW_NEXT_PHASE(work);
      }
    }
    break;
  case 5:
    if (work->bowShrinking == 0)
      work->phase = 999;
    break;
  }

  if (work->bowGrowing != 0) {
    sprite = &work->bow;
    sprite->scaleX += 0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += 0x2000;
    if (sprite->scaleX >= 0x10000) {
      sprite->scaleX = sprite->scaleY = 0x10000;
      sprite->angle = scene->angle;
      work->bowGrowing = 0;
    }
  }
  if (work->bowShrinking != 0) {
    sprite = &work->bow;
    sprite->scaleX += -0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += 0x2000;
    if (sprite->scaleX <= 0x6665) {
      sprite->scaleX = sprite->scaleY = 0;
      work->bowShrinking = 0;
    }
  }

  // The bow's drawing pose, by frame: drawn back, then loosed and recovered.
  if (work->frame != 999) {
    work->frame++;
    sprite = &work->bow;
    switch (work->frame) {
    case 10:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 20:
      sprite->oam.index = work->bowTile + 64;
      break;
    case 54:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 62:
      sprite->oam.index = work->bowTile + 96;
      break;
    case 65:
      sprite->oam.index = work->bowTile + 128;
      break;
    case 68:
      sprite->oam.index = work->bowTile + 160;
      break;
    case 58:
    case 71:
      sprite->oam.index = work->bowTile;
      break;
    case 72:
      work->frame = 999;
      break;
    }
  }

  motion = &work->bowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  sprite = &work->bow;
  Camera_WorldToScreen__f0e150c(pos);
  sprite->x = pos[0];
  sprite->y = pos[2];
  Func_080eb01c(sprite);
  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Camera_WorldToScreen__f0e150c(pos);
  sprite = &work->arrow;
  sprite->x = pos[0];
  sprite->y = pos[2];
  Func_080eb01c(sprite);
  if (work->arrowMoving != 0)
    BattleEffect_IntegrateVector3(&motion->x, 64, 0);

  if (work->sparking != 0) {
    motion = work->sparks;
    for (i = 9; i >= 0; i--) {
      if (motion->delay == 0) {
        s32 speed;
        s32 angle;

        speed = (Random16() & 255) + 256;
        angle = (Random16() & 0x1fff) + 0x7000;
        motion->x = 0x3c0000;
        motion->y = 0x1c0000;
        motion->velocityX = Trig_Cos(angle) * speed >> 6;
        motion->velocityY = Trig_Sin(angle) * speed >> 6;
      }
      if (motion->delay > 0) {
        s32 x = (s16)(motion->x >> 16);

        BattleEffect_DrawCanvasBlock(Data_080eda0c + (motion->delay & 1) * 64,
                                     x, (s16)(motion->y >> 16), 8, 8);
        BattleEffect_IntegrateVector2(&motion->x, 60, 0);
        /* FAKEMATCH(barrier): an empty asm after the step, which keeps the
         * picture's address add after the y's load for the draw; without it,
         * 2 of 923 instructions differ in tla-en. */
        asm("");
      }
      motion->delay++;
      if (work->sparksRepeat == 1 && motion->delay == 10)
        motion->delay = 0;
      motion++;
    }
  }

  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Vector_AddPolarOffset__f0e150c(-0x240000, scene->angle, pos);
  Camera_WorldToScreen__f0e150c(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->command = CANVAS_UPLOAD_AND_FADE3;
  work->timer++;
}

void Func_080e1f2c(void);

/// Effect 29 cast on a target: runs the effect. Nothing calls it; the
/// targeted dispatch has no case for animation 29.
void BattleEffect_CallEffect29(void) { Func_080e1f2c(); }


// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.

extern char ResourceId_BowDraw, ResourceId_FlamingArrow;

s32 BattleEffect_GetObjectPriority(struct Actor *object);

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
  Vector_AddPolarOffset__f0e150c(0x80000, scene->angle, pos);
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
  Vector_AddPolarOffset__f0e150c(0x120000, scene->angle, pos);
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
  Vector_AddPolarOffset__f0e150c(0x500000, scene->angle, work->target);

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

struct PointWork {
  u8 unk_0[0xcc0];
  s16 points[62];
  u16 unk_d3c;
  u16 unk_d3e;
  u16 unk_d40;
  u16 unk_d42;
};

extern const s8 Data_080f0f64[];

s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);

/// Looks at 21 map cells around the effect owner, at the tile offsets of
/// Data_080f0f64 (signed x, z byte pairs), on the owner's layer. Each cell of
/// kind 1, 2 or 3 that Func_080cda84 accepts for the effect's id is listed as a
/// tile in the event work's point list at 0xcc0, which ends with -1, -1; a
/// kind-1 cell also stores its kind in the heap slot 0x5c work's byte 0x1b34,
/// and the last kind-2 and kind-3 cells are kept in pixels, centred on their
/// tile, at 0xd3c and 0xd40.
void Func_080e2264(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  struct PointWork *event = Ram_HeapSlots->eventWork;
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s16 *points = event->points;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s32 position[3];
  s32 x, z;
  s32 kind;
  u32 i;
  s32 count = 0;

  work[0x1b34] = count;
  event->unk_d3c = -1;
  event->unk_d3e = -1;
  event->unk_d40 = -1;
  event->unk_d42 = -1;
  for (i = 0; i <= 20; i++) {
    x = owner->x + (Data_080f0f64[i * 2] << 20);
    z = owner->z + (Data_080f0f64[i * 2 + 1] << 20);
    kind = Func_080dbda8(x, z, owner->terrainId);
    // position[1] is never set: only x and z reach Func_080cda84.
    position[0] = x;
    position[2] = z;
    if (kind == 1 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
      work[0x1b34] = kind;
    }
    if (kind == 2 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d3c = x >> 16;
      event->unk_d3e = z >> 16;
      event->unk_d3c = (event->unk_d3c & 0xfff0) + 8;
      event->unk_d3e = (event->unk_d3e & 0xfff0) + 8;
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
    }
    if (kind == 3 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d40 = x >> 16;
      event->unk_d42 = z >> 16;
      event->unk_d40 = (event->unk_d40 & 0xfff0) + 8;
      event->unk_d42 = (event->unk_d42 & 0xfff0) + 8;
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
    }
  }
  points[count * 2] = -1;
  points[count * 2 + 1] = -1;
}

// Runs the field events an effect touched.

struct TouchOwner {
  u8 unk00[34];
  u8 layer;
};

struct TouchWork {
  u8 unk00[16];
  struct TouchOwner *owner;
  u8 unk14[4];
  s16 argument0;
  s16 argument1;
  u8 unk1c[2];
  s16 kind;
};

struct TouchCell {
  s16 x;
  s16 z;
};

s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
/* VIEW(f0e150c): routine Field_RunPsynergyEvent as void Field_RunPsynergyEvent(s32 event, s32 argument0, s32 argument1); event_work.h has s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target). */
extern void Field_RunPsynergyEvent__f0e150c(s32 event, s32 argument0, s32 argument1) __asm__("Field_RunPsynergyEvent");

// Only for the compiler: see the FAKEMATCH tag in Func_080e249c.
static inline s32 Index(s32 i) { return i; }

/// Resolves each cell in the event work's touched list to an event id at the
/// owner's layer, sorts the distinct ids and runs each one's event for the
/// effect's kind.
void Func_080e249c(void) {
  struct TouchWork *work = Ram_HeapSlots->effectWork;
  struct TouchCell *cells =
      (struct TouchCell *)((u8 *)Ram_HeapSlots->eventWork + 0xcc0);
  struct TouchOwner *owner = work->owner;
  s32 count = 0;
  s16 ids[24];
  s32 i, j;

  for (i = 0; cells[i].x != -1; i++) {
    s16 id = Func_080dbdc8(cells[i].x << 20, cells[i].z << 20, owner->layer);

    for (j = 0; j < count; j++) {
      if (ids[j] == id)
        break;
    }
    if (j == count)
      ids[count++] = id;
  }
  ids[count] = -1;

  if (count > 1) {
    for (i = 0; i < count - 1; i++) {
      for (j = i + 1; j < count; j++) {
        if (ids[i] > ids[j]) {
          s16 swap = ids[i];

          ids[i] = ids[j];
          ids[j] = swap;
        }
      }
    }
  }

  for (i = 0; ids[i] != -1; i++) {
    /* FAKEMATCH(wrapper): ids[Index(i)], through an inline that returns i,
     * so the id is read before the kind; as ids[i], 18 of 160 instructions
     * differ in tla-en. */
    s32 event = Func_080ce458(0x50000005, work->kind, ids[Index(i)]);

    if (event != 0)
      Field_RunPsynergyEvent__f0e150c(event, work->argument0, work->argument1);
  }
}

struct TouchLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[40];
};

struct TouchMap {
  u8 unk000[264];
  struct TouchLayer layers[1];
};

struct TouchCopy {
  u8 toX;
  u8 toZ;
  u8 fromX;
  u8 fromZ;
  u8 cellX;
  u8 cellZ;
  u8 unk06[2];
};

/* VIEW(f0e150c): tag struct MapCell as struct MapCell__f0e150c; map.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/event_commands/radial.c:2328: dereferencing pointer to incomplete type. */
struct MapCell__f0e150c {
  u8 unk00[2];
  u8 unk02;
};

struct TouchCopies {
  struct TouchCell unk0000[1491];
  struct TouchCopy copies[64];
  s8 count;
};

extern struct TouchCopies gData_02001000;

s32 Party_GetLeaderActor(void);
struct Actor *Actor_GetById(u32 objectId);
void Func_080201f0(s32 toX, s32 toZ, s32 width, s32 height, s32 fromX,
                   s32 fromZ);
struct MapCell__f0e150c *Func_080dbb78(s32 x, s32 z, s32 layer);

void Func_080e25e8(s32 toX, s32 toZ, s32 width, s32 height, s32 originX,
                   s16 originZ, s32 layerIndex) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 ownerHeight = owner->terrainHeight >> 20;
  struct TouchMap *map = (struct TouchMap *)Ram_HeapSlots->mapWork;
  struct TouchLayer *layer = &map->layers[layerIndex];
  s32 i;

  for (i = 0; cell[i].x != -1; i++) {
    s32 x = cell[i].x - originX;
    s32 z = cell[i].z - originZ - ownerHeight;

    x += layer->x >> 20;
    z += layer->z >> 20;

    if (x >= 0 && x < width && z >= 0 && z < height) {
      s32 n;

      Func_080201f0(toX + x, toZ + z, 1, 1, originX + x, originZ + z);
      Func_080dbb78(cell[i].x << 20, cell[i].z << 20, 2)->unk02 = 0;
      n = gData_02001000.count;
      if (n < 64) {
        gData_02001000.copies[n].toX = toX + x;
        gData_02001000.copies[n].toZ = toZ + z;
        gData_02001000.copies[n].fromX = originX + x;
        gData_02001000.copies[n].fromZ = originZ + z;
        gData_02001000.copies[n].cellX = cell[i].x;
        gData_02001000.copies[n].cellZ = cell[i].z;
        gData_02001000.count++;
      }
    }
  }
}
#undef BOW_NEXT_PHASE
