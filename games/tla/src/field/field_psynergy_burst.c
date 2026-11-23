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

/// The sparks that fly into Burst's glow, and its explosions round the target.
#define BURST_SPARK_COUNT 32
#define BURST_EXPLOSION_COUNT 32
/// The tiles of one of an explosion's eight pictures, which follow each
/// other from BurstWork.burstTile.
#define BURST_EXPLOSION_PICTURE_TILES 4
/// The OBJ palette the explosions are drawn in.
#define BURST_EXPLOSION_PALETTE 15
/// How much of a screen shake is kept each frame.
#define BURST_SHAKE_DECAY (FIXED_ONE * 9 / 10)
/// The explosions the last and largest blast starts, on its first frame and
/// again on its fifth.
#define BURST_FINAL_EXPLOSIONS 10
/// The sparks drawn when Burst is cast at nothing.
#define BURST_SPARKS_AT_NOTHING 12

/// Burst's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the glow disc that swells
/// in front of the caster on the canvas sprite, the 32 sparks that fly into it,
/// the 32 explosions round the target, and the stage of the cast.
struct BurstWork {
  /// The explosion's eight pictures, unpacked before they are loaded.
  u8 tiles[8 * BURST_EXPLOSION_PICTURE_TILES * TILE_SIZE_4BPP];
  struct FieldPsynergyParticle sparks[BURST_SPARK_COUNT];
  struct EffectSprite sparkSprites[BURST_SPARK_COUNT];
  struct EffectSprite burstSprites[BURST_EXPLOSION_COUNT];
  struct FieldPsynergyParticle bursts[BURST_EXPLOSION_COUNT];
  s16 phase; ///< the stage of the cast; FIELD_PSYNERGY_FINISHED once it ends
  s16 timer; ///< frames into the stage
  s16 shake; ///< passed to the target's event each frame (BurstStage)
  u16 burstTile;  ///< the explosion's first tile
  s16 nextBurst;  ///< the explosion slot used next
  s16 newBursts;  ///< explosions to start this frame
  s8 paletteMode; ///< an enum BurstPaletteMode
};

/// BurstWork.paletteMode: what FieldPsynergy_UpdateBurst does to the palettes.
enum BurstPaletteMode {
  BURST_PALETTE_KEPT,      ///< nothing
  BURST_PALETTE_SHINE,     ///< cycles the glow's greens each frame
  BURST_PALETTE_EXPLOSIONS ///< loads the explosions' palette, once
};

/// BurstWork.phase: the stages of the cast, each lasting the frames its case
/// counts.
enum BurstPhase {
  BURST_PHASE_START,        ///< SE_SPIN
  BURST_PHASE_SWELL,        ///< the glow grows
  BURST_PHASE_GATHER,       ///< the sparks fly into it
  BURST_PHASE_FIRST_BLASTS, ///< explosions start, the screen darkens
  BURST_PHASE_BLASTS,       ///< an explosion every 8 frames
  BURST_PHASE_HUSH,         ///< a pause
  BURST_PHASE_GREAT_BLAST   ///< ten explosions, ten more 4 frames on
};

/// The palette of Burst's explosions, loaded into BURST_EXPLOSION_PALETTE.
extern const u16 gBurst_ExplosionPalette[];

/// Moves the work on to its next phase, its timer restarted.
#define BURST_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

/// Burst each frame: shines the glow, draws the sparks and the explosions, and
/// moves the cast on through its stages, shaking the screen at each blast.
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

  work->shake = BURST_STAGE_QUIET;
  if (work->paletteMode == BURST_PALETTE_SHINE) {
    palette = (volatile u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
    i = 0;
    frames = &canvas->drawnFrames;
    red = RGB_LEVEL_MAX;
    blue = RGB(0, 0, RGB_LEVEL_MAX);
    // The glow's 32 colours: red and blue full, green on a sine wave a 32nd
    // of a turn on from colour to colour, which moves on a colour every two
    // frames.
    for (; i < PALETTE_COLORS * 2; i++) {
      u32 angle = ((((u32)*frames >> 1) + i) * TRIG_TURN + TRIG_TURN * 9) / 32;
      s32 green = RGB(0,
                      FIXEDtoINT(Trig_Sin(angle) * CANVAS_SHINE_SWING) +
                          CANVAS_SHINE_MIDDLE,
                      0);

      Io_Write16(green | blue | red, palette++);
    }
  } else if (work->paletteMode == BURST_PALETTE_EXPLOSIONS) {
    Dma_Set(gBurst_ExplosionPalette,
            (void *)&OBJ_PLTT_COLOR(BURST_EXPLOSION_PALETTE, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    work->paletteMode = BURST_PALETTE_KEPT;
  }
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIXEDtoINT(pos[0]);
  canvas->y = (s16)FIXEDtoINT(pos[2]);

  if (scene->enabled == 0) {
    particle = work->sparks;
    for (i = 0; i < BURST_SPARK_COUNT; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + toFIXED(canvas->x);
        sprite->y = particle->y + toFIXED(canvas->y);
        EffectSprite_Draw(sprite);
        Field_AdvanceWithGravity2D(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      SET_POSITION(particle, scene->x, scene->y, scene->z);
      Vector_AddPolarOffset(Random16() * 12 + toFIXED(4), Random16(),
                            &particle->x);
      // UB: nextBurst is incremented and assigned, unsequenced.
#ifdef UBFIX
      work->nextBurst = (work->nextBurst + 1) % BURST_EXPLOSION_COUNT;
#else
      work->nextBurst = ++work->nextBurst % BURST_EXPLOSION_COUNT;
#endif
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i < BURST_EXPLOSION_COUNT; i++, particle++) {
      if ((u32)particle->timer > 71)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 6:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES;
        break;
      case 12:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 2;
        break;
      case 22:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 3;
        break;
      case 32:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 4;
        break;
      case 42:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 5;
        break;
      case 52:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 6;
        break;
      case 62:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 7;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case BURST_PHASE_START:
      if (work->timer == 0)
        Sound_PlayFar(SE_SPIN);
      if (work->timer == 10)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_SWELL:
      canvas->scaleX += FIXED_ONE * 3 / 256;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX >= FIXED_ONE / 2) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case BURST_PHASE_GATHER:
      if (work->timer == 40)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_FIRST_BLASTS:
      if (work->timer == 0)
        work->paletteMode = BURST_PALETTE_EXPLOSIONS;
      if (work->timer == 0)
        Sound_PlayFar(SE_CLATTER);
      if ((work->timer & 7) == 0) {
        MapGrid_SetScreenShakeFar(toFIXED(3), toFIXED(3), BURST_SHAKE_DECAY);
        work->newBursts = 1;
        work->shake = BURST_STAGE_SMALL_BLAST;
      }
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
      if (work->timer == 16)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_BLASTS:
      if ((work->timer & 15) == 0)
        Sound_PlayFar(SE_CLATTER);
      if ((work->timer & 7) == 0) {
        MapGrid_SetScreenShakeFar(toFIXED(3), toFIXED(3), BURST_SHAKE_DECAY);
        work->newBursts = 1;
        work->shake = BURST_STAGE_SMALL_BLAST;
      }
      if (work->timer == 128)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_HUSH:
      if (work->timer == 35)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_GREAT_BLAST:
      if (work->timer == 0)
        work->newBursts = BURST_FINAL_EXPLOSIONS;
      if (work->timer == 4)
        work->newBursts = BURST_FINAL_EXPLOSIONS;
      if (work->timer == 0) {
        MapGrid_SetScreenShakeFar(toFIXED(12), toFIXED(12), BURST_SHAKE_DECAY);
        Sound_PlayFar(SE_CRASH);
        if (work->timer == 0)
          work->shake = BURST_STAGE_GREAT_BLAST;
      }
      if (work->timer == 20)
        work->shake = BURST_STAGE_DYING_DOWN;
      if (work->timer == 72) {
        work->phase = FIELD_PSYNERGY_FINISHED;
        work->timer = -1;
      }
      break;
    }
  } else {
    particle = work->sparks;
    for (i = 0; i < BURST_SPARKS_AT_NOTHING; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + toFIXED(canvas->x);
        sprite->y = particle->y + toFIXED(canvas->y);
        EffectSprite_Draw(sprite);
        Field_AdvanceWithGravity2D(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      SET_POSITION(particle, scene->x, scene->y, scene->z);
      // UB: nextBurst is incremented and assigned, unsequenced.
#ifdef UBFIX
      work->nextBurst = (work->nextBurst + 1) % BURST_EXPLOSION_COUNT;
#else
      work->nextBurst = ++work->nextBurst % BURST_EXPLOSION_COUNT;
#endif
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i < BURST_EXPLOSION_COUNT; i++, particle++) {
      if ((u32)particle->timer > 47)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 4:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES;
        break;
      case 8:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 2;
        break;
      case 14:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 3;
        break;
      case 22:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 4;
        break;
      case 28:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 5;
        break;
      case 35:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 6;
        break;
      case 41:
        sprite->oam.index = work->burstTile + BURST_EXPLOSION_PICTURE_TILES * 7;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case BURST_PHASE_START:
      if (work->timer == 0)
        Sound_PlayFar(SE_SPIN);
      if (work->timer == 10)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_SWELL:
      canvas->scaleX += FIXED_ONE * 5 / 256;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX > toFIXED(0.3) - 1) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case BURST_PHASE_GATHER:
      if (work->timer == 16)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_FIRST_BLASTS:
      if (work->timer == 0) {
        work->paletteMode = BURST_PALETTE_EXPLOSIONS;
        if (work->timer == 0) {
          MapGrid_SetScreenShakeFar(toFIXED(3), toFIXED(3), BURST_SHAKE_DECAY);
          work->newBursts = 1;
          Sound_PlayFar(SE_CLATTER);
        }
      }
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
      if (work->timer == 16)
        BURST_NEXT_PHASE(work);
      break;
    case BURST_PHASE_BLASTS:
      // Cast at nothing, the one blast is all: the caster lets go, and it ends.
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

/// The tile of the sparks that fly into Burst's glow, one 8x8 tile of 256
/// colours.
extern const u8 gBurst_GlowTiles[];

s32 FieldPsynergy_GetCanvasPriority(struct Actor *actor, s32 facing);

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

  scene->y += toFIXED(10);
  FieldPsynergy_FadeToCastColor();
  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->canvasSprite;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIXEDtoINT(pos[0]);
  canvas->y = (s16)FIXEDtoINT(pos[2]);
  canvas->priority =
      FieldPsynergy_GetCanvasPriority(scene->mainActor, scene->angle);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainActor) + 2;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  if (scene->angle == TRIG_TURN * 3 / 4)
    canvas->depth -= 4;

  sparkEntry = VramCache_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, TILE_SIZE_8BPP, gBurst_GlowTiles);
  for (i = 0, particle = work->sparks, mul = Iwram_MulFX,
      sprite = work->sparkSprites;
       i < BURST_SPARK_COUNT;) {
    EffectSprite_Init(sprite, 4, 4, OBJ_SIZE_8X8, sparkTile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = canvas->priority;
    sprite->depth = canvas->depth + 1;
    sprite->scaleY = sprite->scaleX = FIXED_ONE;
    radius = Random16() * 20 + toFIXED(20);
    angle = Random16();
    particle->x = mul(Trig_Cos(angle), radius);
    particle->y = mul(Trig_Sin(angle), radius);
    radius /= 12;
    angle += DEGtoANG(180);
    particle->velocityX = mul(Trig_Cos(angle), radius);
    particle->velocityY = mul(Trig_Sin(angle), radius);
    particle->timer = -i * 2;
    i++;
    sprite++;
    particle++;
  }

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION),
                      work);
  burstEntry = VramCache_FindFreeEntry();
  tile = VramCache_Load(burstEntry, sizeof(work->tiles), work);
  work->burstTile = tile;
  particle = work->bursts;
  burstSprite = work->burstSprites;
  for (i = 0; i < BURST_EXPLOSION_COUNT; i++) {
    EffectSprite_Init(burstSprite, 8, 8, OBJ_SIZE_16X16, tile);
    burstSprite->depth = canvas->depth + 1;
    burstSprite->oam.priority = canvas->priority;
    burstSprite->oam.fullColor = 0;
    burstSprite->oam.palette = BURST_EXPLOSION_PALETTE;
    particle->timer = -1;
    burstSprite++;
    particle++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GLOW_DISC),
                      Ram_HeapSlots->battleCanvas);
  canvas->command = CANVAS_UPLOAD;
  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  work->phase = BURST_PHASE_START;
  work->timer = 0;
  work->paletteMode = BURST_PALETTE_SHINE;
  work->nextBurst = 0;
  work->newBursts = 0;
  Task_Add(FieldPsynergy_UpdateBurst, TASK_PASS_FRAME);
  kind = scene->fieldPsynergy;
  pending = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                      kind, Scene_SelectPsynergyTarget(kind));
  frame = 0;
  while (work->phase != FIELD_PSYNERGY_FINISHED) {
    wait(1);
    if (work->phase == FIELD_PSYNERGY_FINISHED)
      frame = FIELD_PSYNERGY_FINISHED;
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
