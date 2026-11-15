/// @file
/// Burst: its glow and sparks, and its explosions on the target.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_psynergy_work.h"
#include "system/main_routine_work.h"
#include "system/io_reg.h"

/// Burst's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the glow disc that swells
/// in front of the caster on the canvas sprite, the 32 sparks that fly into it,
/// the 32 explosions round the target, and the stage of the cast.
struct BurstWork {
  u8 tiles[0x400];
  struct FieldPsynergyParticle sparks[32];
  struct EffectSprite sparkSprites[32];
  struct EffectSprite burstSprites[32];
  struct FieldPsynergyParticle bursts[32];
  s16 phase; ///< the stage of the cast; 999 once it ends
  s16 timer; ///< frames into the stage
  /// Passed to the target's event each frame: 2 for a small explosion, 3 for
  /// the great one, 1 as it dies down, 0 otherwise.
  s16 shake;
  u16 burstTile; ///< the explosion's first tile
  s16 nextBurst; ///< the explosion slot used next
  s16 newBursts; ///< explosions to start this frame
  /// 1 cycles OBJ palette 14's greens each frame, 2 loads the explosion's
  /// palette once.
  s8 paletteMode;
};

extern const u16 Data_080f0fa0[];

void Camera_WorldToScreen(s32 *position);

#define BURST_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

/// Task of the Burst: cycles its particles' colours and draws them.
void FieldPsynergy_UpdateBurst(void) {
  struct BurstWork *work = (struct BurstWork *)Ram_HeapSlots->battleEffect;
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->canvasSprite;
  struct FieldPsynergyParticle *particle;
  struct EffectSprite *sprite;
  volatile u16 *palette;
  s32 pos[3];
  s32 i;
  s32 *frames;
  s32 red;
  s32 blue;

  work->shake = 0;
  if (work->paletteMode == 1) {
    palette = (volatile u16 *)&OBJ_PLTT_COLOR(14, 0);
    i = 0;
    frames = &canvas->drawnFrames;
    red = 31;
    blue = 31 << 10;
    for (; i <= 31; i++) {
      s32 green =
          (((Trig_Sin((((u32)*frames >> 1) + i << 16) + 0x90000 >> 5) * 12) >>
            16) +
           16)
          << 5;

      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
      *palette++ = green | blue | red;
      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
    }
  } else if (work->paletteMode == 2) {
    Dma_Set(Data_080f0fa0, (void *)&OBJ_PLTT_COLOR(15, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    work->paletteMode = 0;
  }
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIELD_PIXEL(pos[0]);
  canvas->y = (s16)FIELD_PIXEL(pos[2]);

  if (scene->enabled == 0) {
    particle = work->sparks;
    for (i = 0; i < 32; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + px(canvas->x);
        sprite->y = particle->y + px(canvas->y);
        EffectSprite_Draw(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      SET_POSITION(particle, scene->x, scene->y, scene->z);
      Vector_AddPolarOffset(Random16() * 12 + px(4), Random16(), &particle->x);
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 71)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 6:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 12:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 32:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 42:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 52:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 62:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(SE_SPIN);
      if (work->timer == 10)
        BURST_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x300;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX >= Q16_ONE / 2) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 40)
        BURST_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0)
        work->paletteMode = 2;
      if (work->timer == 0)
        Sound_PlayFar(SE_CLATTER);
      if ((work->timer & 7) == 0) {
        MapGrid_SetScreenShakeFar(px(3), px(3), 0xe666);
        work->newBursts = 1;
        work->shake = 2;
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        BURST_NEXT_PHASE(work);
      break;
    case 4:
      if ((work->timer & 15) == 0)
        Sound_PlayFar(SE_CLATTER);
      if ((work->timer & 7) == 0) {
        MapGrid_SetScreenShakeFar(px(3), px(3), 0xe666);
        work->newBursts = 1;
        work->shake = 2;
      }
      if (work->timer == 128)
        BURST_NEXT_PHASE(work);
      break;
    case 5:
      if (work->timer == 35)
        BURST_NEXT_PHASE(work);
      break;
    case 6:
      if (work->timer == 0)
        work->newBursts = 10;
      if (work->timer == 4)
        work->newBursts = 10;
      if (work->timer == 0) {
        MapGrid_SetScreenShakeFar(px(12), px(12), 0xe666);
        Sound_PlayFar(SE_CRASH);
        if (work->timer == 0)
          work->shake = 3;
      }
      if (work->timer == 20)
        work->shake = 1;
      if (work->timer == 72) {
        work->phase = FIELD_PSYNERGY_FINISHED;
        work->timer = -1;
      }
      break;
    }
  } else {
    particle = work->sparks;
    for (i = 0; i < 12; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + px(canvas->x);
        sprite->y = particle->y + px(canvas->y);
        EffectSprite_Draw(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      SET_POSITION(particle, scene->x, scene->y, scene->z);
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 47)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 4:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 8:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 14:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 28:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 35:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 41:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(SE_SPIN);
      if (work->timer == 10)
        BURST_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x500;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX > 0x4ccb) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 16)
        BURST_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0) {
        work->paletteMode = 2;
        if (work->timer == 0) {
          MapGrid_SetScreenShakeFar(px(3), px(3), 0xe666);
          work->newBursts = 1;
          Sound_PlayFar(SE_CLATTER);
        }
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        BURST_NEXT_PHASE(work);
      break;
    case 4:
      if (work->timer == 5)
        BattleEffect_SetObjectUpdateMode(scene->mainActor, 0);
      if (work->timer == 20) {
        work->phase = FIELD_PSYNERGY_FINISHED;
        work->timer = -1;
      }
      break;
    }
  }
  work->timer++;
}

/// Burst cast on a target: runs FieldPsynergy_Burst.
void FieldPsynergy_BurstOnTarget(void) { FieldPsynergy_Burst(); }
#undef BURST_NEXT_PHASE

extern const u8 gBurst_GlowTiles[];

void Camera_WorldToScreen(s32 *position);
s32 FieldPsynergy_GetCanvasPriority(struct Actor *object, s32 facing);

/// Burst: a glow disc swells in front of the caster as sparks fly into it, then
/// explosions burst round the target, shaking the screen, the last and largest
/// at 12 pixels; the target's event runs each frame with the stage (shake).
/// Cast at nothing (FieldEffectWork.enabled still set), it is shorter: 12
/// sparks and one small explosion.
void FieldPsynergy_Burst(void) {
  struct BurstWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BurstWork));
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  void *pending;
  s32 pos[3];
  s32 sparkEntry;
  s32 sparkTile;
  s32 burstEntry;
  s32 tile;
  s32 radius;
  s32 angle;
  s32 kind;
  s32 frame;
  s32 (*mul)(s32, s32);
  struct EffectSprite *burstSprite;
  s32 i;

  scene->y += px(10);
  FieldPsynergy_FadeToCastColor();
  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->canvasSprite;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIELD_PIXEL(pos[0]);
  canvas->y = (s16)FIELD_PIXEL(pos[2]);
  canvas->priority =
      FieldPsynergy_GetCanvasPriority(scene->mainActor, scene->angle);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainActor) + 2;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  if (scene->angle == TRIG_TURN * 3 / 4)
    canvas->depth -= 4;

  sparkEntry = VramCache_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 64, gBurst_GlowTiles);
  for (i = 0, particle = work->sparks, mul = Iwram_MulQ16,
      sprite = work->sparkSprites;
       i < 32;) {
    EffectSprite_Init(sprite, 4, 4, 0, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = canvas->priority;
    sprite->depth = canvas->depth + 1;
    sprite->scaleY = sprite->scaleX = Q16_ONE;
    radius = Random16() * 20 + px(20);
    angle = Random16();
    particle->x = mul(Trig_Cos(angle), radius);
    particle->y = mul(Trig_Sin(angle), radius);
    radius /= 12;
    angle += TRIG_TURN / 2;
    particle->velocityX = mul(Trig_Cos(angle), radius);
    particle->velocityY = mul(Trig_Sin(angle), radius);
    particle->timer = -i * 2;
    i++;
    sprite++;
    particle++;
  }

  Decompress_TaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion), work);
  burstEntry = VramCache_FindFreeEntry();
  tile = VramCache_Load(burstEntry, 0x400, work);
  work->burstTile = tile;
  particle = work->bursts;
  burstSprite = work->burstSprites;
  for (i = 0; i < 32; i++) {
    EffectSprite_Init(burstSprite, 8, 8, 0x40000000, tile);
    burstSprite->depth = canvas->depth + 1;
    burstSprite->oam.priority = canvas->priority;
    burstSprite->oam.palette256 = 0;
    burstSprite->oam.palette = 15;
    particle->timer = -1;
    burstSprite++;
    particle++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                      Ram_HeapSlots->battleCanvas);
  canvas->command = CANVAS_UPLOAD;
  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  work->phase = 0;
  work->timer = 0;
  work->paletteMode = 1;
  work->nextBurst = 0;
  work->newBursts = 0;
  Task_Add(FieldPsynergy_UpdateBurst, TASK_PASS_VBLANK);
  kind = scene->fieldPsynergy;
  pending = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                      kind, Scene_SelectPsynergyTarget(kind));
  frame = 0;
  while (work->phase != FIELD_PSYNERGY_FINISHED) {
    wait(1);
    if (work->phase == 999)
      frame = 999;
    Field_RunPsynergyEvent(pending, work->shake, frame);
    frame++;
  }
  Task_Remove(FieldPsynergy_UpdateBurst);
  VramCache_FreeEntry(sparkEntry);
  VramCache_FreeEntry(burstEntry);
  FieldPsynergy_FadeFromCastColor();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}
