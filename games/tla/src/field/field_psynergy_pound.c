/// @file
/// Pound: its shine, the object it drives into the ground, and a picture's
/// columns shifted by a wave.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

/// Pound's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the star tiles, the
/// shine's 32 sparks, the glow under the fist, the fist itself and the stage of
/// its fall.
struct PoundWork {
  u8 tiles[0x1000];
  struct FieldPsynergyParticle particles[32];
  struct EffectSprite sprites[32];
  struct EffectSprite glow;
  struct Actor *object; ///< the fist, then what sinks into the ground
  s16 state;            ///< the stage of the fall; 999 once it ends
  s16 timer;            ///< frames into the stage
  s8 sparkles;          ///< nonzero draws the sparks
  s8 fading; ///< set on the frame the fist lands, passed to the event
};

/// Task of the Pound: its shine's colours and particles, each frame.
void FieldPsynergy_UpdatePound(void) {
  struct PoundWork *work = Ram_HeapSlots->battleEffect;
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->canvasSprite;
  struct FieldPsynergyParticle *particle;
  struct EffectSprite *sprite;
  s32 i;
  s32 base, blue;
  s32 color;
  s32 red, green;
  s32 *frames;

  work->fading = 0;
  if (work->state == 1) {
    i = 0;
    /* FAKEMATCH(temporary): the frame count read through a pointer kept across
     * the loop. */
    frames = &canvas->drawnFrames;
    base = 9;
    base <<= 16;
    blue = 0x7c00;
  palette:
    red = Trig_Sin((((((u32)*frames >> 1) + i) << 16) + base) >> 5) * 12;
    green = Trig_Sin((((((u32)*frames >> 1) + i) << 16) + base) >> 5) * 12;
    red = (red >> 16) + 16;
    green = (green >> 16) + 16;
    /* FAKEMATCH(dead): the colour in a once-only block. */
    do {
      color = (green << 5) | blue | red;
      *(u16 *)(0x050003c0 + i * 2) = color;
    } while (0);
    if (++i < 32)
      goto palette;
  }
  if (work->sparkles) {
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 12; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 16) {
        sprite->x = particle->x;
        sprite->y = particle->y;
        EffectSprite_Draw(sprite);
        BattleEffect_IntegrateVector2(particle, 64, 0);
      }
      particle->timer++;
    }
  }
  switch (work->state) {
  case 0:
    work->object->y.fixed -= px(7) / 2;
    if (work->timer == 8) {
      canvas->scaleX = Q16_ONE / 8;
      canvas->scaleY = Q16_ONE / 8;
      MapGrid_SetScreenShakeFar(px(13) / 10, px(13) / 10, Q16_ONE);
      Sound_PlayFar(SE_POUND);
      work->sparkles = 1;
      work->fading = 1;
    }
    if (work->timer == 8) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    canvas->scaleX += Q16_ONE / 16;
    canvas->scaleY = canvas->scaleX;
    if (canvas->scaleX > px(4) / 5 - 1) {
      canvas->scaleX = px(4) / 5;
      canvas->scaleY = px(4) / 5;
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    canvas->command = CANVAS_UPLOAD_AND_FADE1;
    if (work->timer == 32) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0)
      MapGrid_SetScreenShakeFar(1, 1, 1);
    if (work->timer == 10) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 4:
    work->object->y.fixed += px(3) / 2;
    if (work->timer == 13)
      work->state = 999;
    break;
  }
  work->timer++;
}

void Camera_WorldToScreen(s32 *position);

/// Per-frame callback: until done, moves the effect object down 2.5 pixels
/// and shrinks it by 1/64 each frame, marking done at frame 8.
void BattleEffect_SinkAndShrinkObject(void) {
  struct PoundWork *work = Ram_HeapSlots->battleEffect;

  if (work->state == 0) {
    struct Actor *object = work->object;

    object->y.fixed += -(px(5) / 2);
    object->scaleX += -0x400;
    object->scaleY += -0x400;
    if (work->timer == 8)
      work->state = 999;
  }
  work->timer++;
}

/// Pound cast on a target: runs FieldPsynergy_Pound.
void FieldPsynergy_PoundOnTarget(void) { FieldPsynergy_Pound(); }

void Func_080dba5c(void);
void Camera_WorldToScreen(s32 *position);
void FieldPsynergy_ShrinkToCaster(struct Actor *object, s32 x, s32 y, s32 z);

/// Pound ("Drive objects into the ground."): a giant fist forms over the spot
/// 32 pixels before the caster, or over the target, and comes down on it. Out
/// of reach, or with a solid actor in the way, it shatters with the error
/// sound; else the place's Pound event runs.
void FieldPsynergy_Pound(void) {
  struct PoundWork *work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 0x18b4);
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *child = scene->child;
  struct Actor *caster = scene->mainActor;
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
  struct FieldPsynergyParticle *particle;

  lift = 0x200000;
  if (child == NULL) {
    SET_POSITION(scene, caster->x.fixed, caster->y.fixed, caster->z.fixed);
    Vector_AddPolarOffset(lift, scene->angle, &scene->x);
  } else if (child->motionFlags & 4) {
    /* FAKEMATCH(dead): lift cleared under a test of the ground's height. */
    if (child->groundHeight == 300)
      lift = 0;
    scene->y = child->groundHeight;
    lift = child->y.fixed - child->groundHeight + px(32);
  }
  if (scene->enabled != 0) {
    // As x != -1 this splits the -1 and loses the mvns.
    spot =
        ~Field_MeasurePsynergyHeight(scene->fieldPsynergy, caster, &scene->x);
    inReach = NONZERO(spot);
    if (inReach == 1) {
      for (j = 0; j < 80; j++) {
        object = event->actors[j];
        if (object != NULL && object->script != NULL &&
            (object->collisionFlags & 1) &&
            FieldPsynergy_CheckOverlap(&scene->x, 4, &object->x.fixed,
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
    if ((shine->spriteStorageKind & 15) == 1) {
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
  canvas = Ram_HeapSlots->canvasSprite;
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainActor);
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
                      Ram_HeapSlots->battleCanvas);
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
    shine->y.fixed += px(1) / 2;
    rise--;
    wait(1);
  } while (rise >= 0);
  wait(15);
  work->state = 0;
  work->timer = 0;
  work->sparkles = 0;
  if (!inReach) {
    Task_Add(BattleEffect_SinkAndShrinkObject, TASK_PASS_VBLANK);
    while (work->state != 999)
      wait(1);
    Task_Remove(BattleEffect_SinkAndShrinkObject);
    for (j = 15; j >= 0; j--)
      BattleEffect_SpawnScatterParticle(shine->x.fixed, shine->y.fixed,
                                        shine->z.fixed);
    Sound_PlayFar(SE_MENU_ERROR);
    wait(1);
  } else {
    pending = Scene_FindPsynergyTrigger(
        0x50000005, scene->fieldPsynergy,
        Scene_SelectPsynergyTarget(scene->fieldPsynergy));
    Task_Add(FieldPsynergy_UpdatePound, TASK_PASS_VBLANK);
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
    FieldPsynergy_ShrinkToCaster(shine, shine->x.fixed, shine->y.fixed,
                                 shine->z.fixed);
  }
  Actor_ReleaseFar(shine);
  VramCache_FreeEntry(starEntry);
  VramCache_FreeEntry(glowEntry);
  BattleEffect_EndCanvasSprite();
end:
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
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
