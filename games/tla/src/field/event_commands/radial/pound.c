/// @file
/// Two routines not yet understood.
// Part of radial.c.

#include "../radial_internal.h"
#include "sounds.h"
#include "macros.h"

struct PoundParticle {
  s32 x;
  s32 y;
  s32 unk08;
  s32 velocityX;
  s32 velocityY;
  s32 unk14;
  s32 timer;
};

struct PoundWork {
  u8 tiles[0x1000];
  struct PoundParticle particles[32];
  struct EffectSprite sprites[32];
  struct EffectSprite glow;
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  s8 fading;
};

extern char ResourceId_GlowDisc, ResourceId_GoldStar;

void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Field_MeasurePsynergyHeight(s32 id, struct Actor *object, s32 *position);
s32 FieldPsynergy_CheckOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_080dba5c(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
void FieldPsynergy_GrowFromCaster(struct Actor *object, s32 x, s32 y, s32 z);
void FieldPsynergy_ShrinkToCaster(struct Actor *object, s32 x, s32 y, s32 z);
void BattleEffect_SinkAndShrinkObject(void);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
void FieldPsynergy_UpdatePound(void);

void FieldPsynergy_Pound(void) {
  struct PoundWork *work = HeapSlot_Alloc(92, 0x18b4);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct FxEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *child = scene->child;
  struct Actor *caster = scene->mainObject;
  struct Actor *object;
  struct Actor *shine;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct EffectSprite *glow;
  void *pending;
  s32 pos[3];
  s32 starEntry;
  s32 starTile;
  s32 glowEntry;
  s32 inReach;
  s32 lift;
  s32 glowTile;
  s32 spot;
  s32 size = 0x800;
  s32 rise;
  s32 speed;
  s32 angle;
  s32 frame;
  s32 j;
  struct PoundParticle *particle;

  lift = 0x200000;
  if (child == NULL) {
    SET_POSITION(scene, caster->x, caster->y, caster->z);
    Vector_AddPolarOffset(lift, scene->facing, &scene->x);
  } else if (child->motionFlags & 4) {
    /* FAKEMATCH(dead): lift cleared under a test of the ground's height. */
    if (child->terrainHeight == 300)
      lift = 0;
    scene->y = child->terrainHeight;
    lift = child->y - child->terrainHeight + 0x200000;
  }
  if (scene->enabled != 0) {
    // As x != -1 this splits the -1 and loses the mvns.
    spot = ~Field_MeasurePsynergyHeight(scene->fieldKind, caster, &scene->x);
    inReach = (u32)(-spot | spot) >> 31;
    if (inReach == 1) {
      for (j = 0; j < 80; j++) {
        object = event->objects[j];
        if (object != NULL && object->script != NULL &&
            (object->collisionFlags & 1) &&
            FieldPsynergy_CheckOverlap(&scene->x, 4, &object->x,
                                       object->radius - 2) >= 0)
          goto outOfReach;
      }
    }
  } else {
    inReach = 1;
    if (child != NULL && ((u8 *)child)[99] != 0) {
    outOfReach:
      inReach = 0;
    }
  }

  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  shine = Actor_Spawn(352, 0, 0, 0);
  work->object = shine;
  if (shine == NULL)
    goto end;
  shine->motionFlags = 0;
  if (*(u16 *)((u8 *)event + 0xd4c) & 2) {
    if ((shine->animationKind & 15) == 1) {
      ((struct EffectSprite *)shine->sprite)->oam.priority = 1;
      ((u8 *)shine)[35] &= 254;
    }
  } else {
    ((u8 *)shine)[35] = 1;
  }
  shine->facing = caster->facing;
  Actor_SelectAnimationFar(shine, 2);
  if (!(*(u16 *)((u8 *)event + 0xd4c) & 1))
    Func_080dba5c();

  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->unkec[1];
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  canvas->depth = 1;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                      Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;

  IWRAM_FILL_WORDS(work, size, 0);
  glowEntry = VramCache_FindFreeEntry();
  glowTile = VramCache_Load(glowEntry, size, work);
  glow = &work->glow;
  EffectSprite_Init(glow, 32, 32, 0xc0000000, glowTile);
  glow->oam.priority = 0;
  glow->oam.palette256 = 0;
  glow->oam.palette = 0;
  glow->depth = 0;
  glow->scaleY = glow->scaleX = 0xcccc;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  glow->x = pos[0];
  glow->y = pos[2];

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GoldStar), work);
  starEntry = VramCache_FindFreeEntry();
  starTile = VramCache_Load(starEntry, 0x100, work);
  for (j = 0, particle = work->particles; j < 32; j++, particle++) {
    sprite = &work->sprites[j];
    speed = Random16() * 18 / 10;
    speed += 0x28000;
    angle = j * 0x1555 + (Random16() >> 5);
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Vector_AddPolarOffset(px(4), angle, pos);
    Camera_WorldToScreen(pos);
    particle->x = pos[0];
    particle->y = pos[2];
    particle->velocityX = Iwram_MulQ16(Trig_Cos(angle), speed);
    particle->velocityY = Iwram_MulQ16(Trig_Sin(angle), speed);
    particle->timer = -j;
    EffectSprite_Init(sprite, 8, 8, 0x40000000, starTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    sprite->scaleY = sprite->scaleX = 0xcccc;
  }
  wait(1);
  FieldPsynergy_GrowFromCaster(shine, scene->x, scene->y + lift, scene->z);
  Actor_SelectAnimationFar(shine, 1);
  wait(15);
  rise = 24;
  do {
    shine->y += px(1) / 2;
    rise--;
    wait(1);
  } while (rise >= 0);
  wait(15);
  work->state = 0;
  work->timer = 0;
  work->sparkles = 0;
  if (!inReach) {
    Task_Add(BattleEffect_SinkAndShrinkObject, 0x480);
    while (work->state != 999)
      wait(1);
    Task_Remove(BattleEffect_SinkAndShrinkObject);
    for (j = 15; j >= 0; j--)
      BattleEffect_SpawnScatterParticle(shine->x, shine->y, shine->z);
    Sound_PlayFar(SE_MENU_ERROR);
    wait(1);
  } else {
    pending =
        Scene_FindPsynergyTrigger(0x50000005, scene->fieldKind,
                                  Scene_SelectPsynergyTarget(scene->fieldKind));
    Task_Add(FieldPsynergy_UpdatePound, 0x480);
    frame = 0;
    while (work->state != 999) {
      wait(1);
      scene->stage = work->fading;
      scene->frame = frame;
      if (((s8 *)event)[394] != 3 && !(*(u16 *)((u8 *)event + 0xd4c) & 1)) {
        glow = &work->glow;
        EffectSprite_Draw(glow);
      }
      Field_RunPsynergyEvent(pending, work->fading, frame++);
    }
    Task_Remove(FieldPsynergy_UpdatePound);
    wait(10);
    Sound_PlayFar(SE_MENU_ERROR);
    FieldPsynergy_ShrinkToCaster(shine, shine->x, shine->y, shine->z);
  }
  Actor_ReleaseFar(shine);
  VramCache_FreeEntry(starEntry);
  VramCache_FreeEntry(glowEntry);
  BattleEffect_EndCanvasSprite();
end:
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(92);
}

/// Copies a 32 by 32 byte picture into a cleared one stretched down, its rows
/// four pixels apart, each column slid down by phase (1.6 pixels a step, from
/// 124 pixels up) less a sine wave across the columns; what falls outside is
/// dropped. The
/// Scoop draws its rising picture with it.
void Picture_ShiftColumnsByWave(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height;
  s32 column;
  s32 row;

  Iwram_ClearWordsEntry(destination, 32 * 32);
  // Set after the clear, which keeps it out of the call's registers.
  height = 32;
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + Q16_ONE * 8;

    for (row = 0; row < height; row++) {
      s32 position = phase * 0x19999 - Q16_ONE * 124 + row * (Q16_ONE * 4);
      // In two steps: the wave subtracted, then the shift.
      s32 shift = position - wave;
      shift >>= 16;

      if (shift >= 0 && shift < height)
        destination[(shift << 5) + column] = source[row * width + column];
    }
  }
}
