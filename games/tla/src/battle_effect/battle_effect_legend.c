/// @file
/// Legend, and the Bramble, Frost, Flame and Thunder Cards.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Legend's sparks, and the cards', are the first LEGEND_SPARK_COUNT of
/// EXTRA_PARTICLES, 128 for each of Legend's swords; they bounce once on the
/// ground at SPARK_GROUND_Y.
#define LEGEND_SPARK_COUNT 512
#define SPARK_GROUND_Y 104
#define LEGEND_SWORD_SPARKS 128

/// BladeSheet's sword, point down, in the effect's pictures; it is drawn cut
/// off at SPARK_GROUND_Y, stuck in the ground.
#define LEGEND_BLADE_PICTURES 16384
#define LEGEND_SWORD_WIDTH 40
#define LEGEND_SWORD_HEIGHT 80

/// Where FlameGradientTexture is decoded in the effect's pictures.
#define LEGEND_FLAME_GRADIENT 20480

/// The fan of LEGEND_FAN_FLAMES flames, FlameGradientTexture's dark red to
/// white, tilted LEGEND_FAN_TILT.
#define LEGEND_FAN_FLAMES 8
#define LEGEND_FAN_TILT 18384
/// Each sword's ring of fire grows by this share of its size a frame, from
/// half.
#define LEGEND_FIRE_RING_GROWTH 12384

/// The swords set out LEGEND_SWORD_INTERVAL frames apart; each falls from
/// LEGEND_SWORD_FALL_FRAME frames after it sets out, rings out at
/// LEGEND_SWORD_SOUND_FRAME and strikes at LEGEND_SWORD_STRIKE_FRAME.
#define LEGEND_SWORD_INTERVAL 16
#define LEGEND_SWORD_FALL_FRAME 50
#define LEGEND_SWORD_SOUND_FRAME 54
#define LEGEND_SWORD_STRIKE_FRAME 59
/// Each sword falls for LEGEND_SWORD_FALL_FRAMES frames, from
/// LEGEND_SWORD_START_DISTANCE pixels above and beside the target.
#define LEGEND_SWORD_FALL_FRAMES 12
#define LEGEND_SWORD_START_DISTANCE 80
#define LEGEND_FRAMES 86 ///< with one sword; each further one adds an interval

/// Legend, Excalibur's unleash: a fan of flames opens over the target
/// until the first sword falls, while a rune, mirrored from one corner, rises
/// and turns behind it. Then one sword for each step of the variant (variant +
/// 1 in all) falls on the target from above the user's side: it strikes,
/// throwing sparks that bounce on the ground, while a ring of fire grows and
/// fades where it lands.
///
/// Abilities: Legend.
void BattleEffect_RunLegend(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *graphics;
  struct ScreenPoint start;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct Particle *step;
  s32 i;
  s32 j;
  u8 *cells = Ram_MapCellBuffer;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas and a clipped additive blitter, mirrored to face the
  // target; mirror the rune's corner into the sheet; load the sparks, the
  // sword and the ring of fire.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_CORNER_RUNE_A_SHEET, cells, 1, 1);
  Picture_MirrorIntoQuadrants(cells, work->sheet, 64, 64);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_BLADE_SHEET,
                             work->sheet + LEGEND_BLADE_PICTURES, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FLAME_GRADIENT_TEXTURE,
                             work->sheet + LEGEND_FLAME_GRADIENT, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0;
       frame != args->variant * LEGEND_SWORD_INTERVAL + LEGEND_FRAMES;
       frame++) {
    // Frame 0: the swords set out from above and beside the target, on the
    // user's side, toward it.
    if (frame == 0) {
      EffectPosition_ProjectBattler(args->targets[0], &target);
      if (args->side == EFFECT_SIDE_ENEMIES)
        start.x = target.x + LEGEND_SWORD_START_DISTANCE;
      else
        start.x = target.x - LEGEND_SWORD_START_DISTANCE;
      start.y = target.y - LEGEND_SWORD_START_DISTANCE;
      for (i = 0; i != 3; i++) {
        step = &work->particles[i];
        step->x = start.x * (toFIXED(1) / 2);
        step->y = INTtoFIXED(start.y);
        step->velocityX = (target.x - start.x) * (toFIXED(1) / 32);
        step->velocityY = (target.y - start.y) * (toFIXED(1) / 16);
        step->variant = 0;
      }
      EFFECT_FREE_PARTICLES(i, EXTRA_PARTICLES, LEGEND_SPARK_COUNT);
    }

    // The swords, one each LEGEND_SWORD_INTERVAL frames.
    for (i = 0; i != args->variant + 1; i++) {
      step = &work->particles[i];
      if (frame == i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_SOUND_FRAME)
        Sound_PlayFar(SE_POWER_UP);

      // The strike: the target reels and flashes, and sparks fly up from it.
      // The last sword ends the action.
      if (frame == i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_STRIKE_FRAME) {
        if (i == args->variant)
          EFFECT_SHOW_DAMAGE(SE_CLATTER);
        else
          Sound_PlayFar(SE_CLATTER);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, 0, 100);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        for (j = 0; j != LEGEND_SWORD_SPARKS; j++) {
          struct Particle *spark =
              &EXTRA_PARTICLES[i * LEGEND_SWORD_SPARKS + j];
          s32 angle;
          s32 force;

          spark->x = target.x * (toFIXED(1) / 2);
          spark->y = toFIXED(96);
          angle = Random16() % (TRIG_TURN * 3 / 4) + DEGtoANG(45);
          force = Random16() & 127;
          spark->velocityX = (Trig_Sin(angle) * force) >> 4;
          spark->velocityY = (Trig_Cos(angle) * force) >> 3;
          spark->variant = (Random16() & 7) + 16;
        }
      }

      // The sword falls, faster each frame, until it sticks in the ground.
      if (frame >= i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_FALL_FRAME) {
        s32 x = FIXED_INTEGER_PART(step->x) - LEGEND_SWORD_WIDTH / 2;
        s32 y = FIXED_INTEGER_PART(step->y) - 50;
        s32 height = LEGEND_SWORD_HEIGHT;

        if (y > SPARK_GROUND_Y - LEGEND_SWORD_HEIGHT)
          height = SPARK_GROUND_Y - y;
        draw[0](canvas, work->sheet + LEGEND_BLADE_PICTURES, x, y,
                LEGEND_SWORD_WIDTH, height);
        if (step->variant <= LEGEND_SWORD_FALL_FRAMES) {
          step->x += step->velocityX;
          step->y += step->velocityY;
          step->velocityX += (target.x - start.x) * (toFIXED(1) / 128);
          step->velocityY += (target.y - start.y) * (toFIXED(1) / 64);
        }
        step->variant++;
      }
    }

    // The sparks bounce on the ground.
    for (i = 0; i != LEGEND_SPARK_COUNT; i++) {
      step = &EXTRA_PARTICLES[i];
      if (step->variant >= 0) {
        s32 size = (step->variant >> 2) + 1;

        EFFECT_DRAW_MOTE(draw[0], canvas, graphics, FIXED_INTEGER_PART(step->x),
                         FIXED_INTEGER_PART(step->y), size);
        if (step->y > toFIXED(SPARK_GROUND_Y)) {
          step->y = toFIXED(SPARK_GROUND_Y);
          step->velocityY = -step->velocityY / 2;
        }
        Particle_AdvanceWithGravity2D(step, 62, toFIXED(1) / 4);
        step->variant--;
      }
    }

    // This frame's models: one quad, retextured for each part.
    {
      struct RenderModel *model;
      void *vertices;
      FIXED scale;

      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->vertices = vertices;

      // Frames 16-107: the rune rises behind the target, turning, then fades.
      if (frame >= 16 && frame <= 107) {
        s32 height = frame * 2 - 96;
        s32 alpha = 0;

        scale = frame * (FIXED_ONE / 8) - FIXED_ONE * 2;
        if (height > -48)
          height = -48;
        // It fades from 60 frames after the last sword sets out, four levels a
        // frame.
        if (frame >= args->variant * LEGEND_SWORD_INTERVAL + 60)
          alpha = -((frame - args->variant * LEGEND_SWORD_INTERVAL) * 4 + -240);
        if (scale > FIXED_ONE * 2)
          scale = FIXED_ONE * 2;
        model->shade = alpha;
        Matrix_Initialize();
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_Translate(INTtoFIXED(start.x - 128), INTtoFIXED(height), 0);
        Matrix_RotateX(TRIG_TURN * 89 / 256);
        if (args->side == EFFECT_SIDE_ENEMIES)
          Matrix_RotateY(TRIG_TURN * 25 / 256);
        else
          Matrix_RotateY(-TRIG_TURN * 25 / 256);
        Matrix_RotateZ(frame * (TRIG_TURN / 128));
        Matrix_ScaleUniform(scale);
        Matrix_Scale(FIXED_ONE, FIXED_ONE, FIXED_ONE);
        Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
        Render_DrawModelList(model);
      }

      // Each sword's ring of fire, FireRingTexture's, grows where it lands,
      // holds 8 frames and fades over 8.
      texture.pixels = cells;
      for (i = 0; i != args->variant + 1; i++) {
        if (frame >= i * LEGEND_SWORD_INTERVAL + LEGEND_SWORD_FALL_FRAME) {
          s32 age = frame - i * LEGEND_SWORD_INTERVAL - LEGEND_SWORD_FALL_FRAME;
          s32 alpha = -MODEL_SHADE_INVISIBLE - age * 8;

          if (alpha > 0)
            alpha = 0;
          if (alpha > MODEL_SHADE_INVISIBLE) {
            scale = age * LEGEND_FIRE_RING_GROWTH + FIXED_ONE / 2;
            model->shade = alpha;
            Matrix_Initialize();
            Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
            Matrix_Translate(INTtoFIXED(start.x - 128), -toFIXED(48), 0);
            Matrix_RotateX(TRIG_TURN * 89 / 256);
            if (args->side == EFFECT_SIDE_ENEMIES)
              Matrix_RotateY(TRIG_TURN * 25 / 256);
            else
              Matrix_RotateY(-TRIG_TURN * 25 / 256);
            Matrix_RotateZ(frame * (TRIG_TURN / 128));
            Matrix_ScaleUniform(scale);
            Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
            Render_DrawModelList(model);
          }
        }
      }

      // Until the swords fall: the fan of flames opens, pulsing, and closes.
      if (frame < LEGEND_SWORD_FALL_FRAME) {
        texture.logWidth = 5;
        texture.logHeight = 6;
        texture.pixels = work->sheet + LEGEND_FLAME_GRADIENT;
        model->mesh = gBattleEffect_TallMesh;
        for (i = 0; i != LEGEND_FAN_FLAMES; i++) {
          model->shade = 0;
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(INTtoFIXED(start.x - 128), -toFIXED(80), 0);
          Matrix_RotateX(LEGEND_FAN_TILT);
          Matrix_RotateZ((frame + i * 16) << 9);
          scale = frame * (FIXED_ONE / 16) - FIXED_ONE / 4;
          if (scale > FIXED_ONE)
            scale = FIXED_ONE;
          if (frame > 31)
            scale = FIXED_ONE * 3 - frame * (FIXED_ONE / 16);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE,
                       (Trig_Sin((frame + i * 8) << 9) / 8 + scale) * 2);
          Render_TransformVertices(gLegend_WingCorners, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Abilities: Bramble Card.
void BattleEffect_RunBrambleCard(struct BattleEffectParams *args) {
  BattleEffect_RunElementCards(args, CARD_BRAMBLE);
}

/// Abilities: Frost Card.
void BattleEffect_RunFrostCard(struct BattleEffectParams *args) {
  BattleEffect_RunElementCards(args, CARD_FROST);
}

/// Abilities: Flame Card.
void BattleEffect_RunFlameCard(struct BattleEffectParams *args) {
  BattleEffect_RunElementCards(args, CARD_FLAME);
}

/// Abilities: Thunder Card.
void BattleEffect_RunThunderCard(struct BattleEffectParams *args) {
  BattleEffect_RunElementCards(args, CARD_THUNDER);
}

/// A card's picture, CARD_WIDTH by CARD_HEIGHT, at its sprite sheet's start;
/// the Bramble Card's sprite sheet has its three vines after it, 32 by 128 as
/// JaggedStreakSheet's streaks are.
#define CARD_STREAK_WIDTH 32
#define CARD_STREAK_HEIGHT 128
/// Where the card's element pictures are decoded in the effect's pictures,
/// after the card: CrystalSheet, FlashBurstSheet, SmokeSheet or
/// JaggedStreakSheet.
#define ELEMENT_CARDS_PICTURES PICTURE_BYTES(CARD)
/// The work's particles: the shards, smoke or pictures the card throws, then
/// one strike countdown for each target. Each target has its own run of the
/// sparks, and a strike throws up the next free 16 of them. The sparks are
/// Legend's, LEGEND_SPARK_COUNT of EXTRA_PARTICLES.
#define CARD_PIECES 56
#define CARD_STRIKE_TIMERS CARD_PIECES
#define CARD_SPARKS_PER_TARGET 64
#define CARD_STRIKE_SPARKS 16

/// The card rises and grows for CARD_RISE_FRAMES frames, then shows its
/// picture; from CARD_SHRINK_FRAME it rises faster and shrinks away. It swings
/// from side to side once every 96 frames as it rises, and leans
/// CARD_TILT.
#define CARD_RISE_FRAMES 48
#define CARD_SHRINK_FRAME 96
#define CARD_TILT 6000
/// The shards and smoke land on the ground at CARD_GROUND_Y. A Frost Card's
/// shard starts tumbling CARD_SHARD_TUMBLE_SPEED a frame, slowing by one every
/// other frame, and is drawn until its tumble reaches CARD_SHARD_TUMBLE_END.
#define CARD_GROUND_Y 96
#define CARD_SHARD_TUMBLE_SPEED 32
#define CARD_SHARD_TUMBLE_END 1024
/// The frames the strikes run between: the Thunder Card's start first and the
/// Bramble Card's end first.
#define CARD_STRIKES_FROM 68
#define CARD_THUNDER_STRIKES_FROM 52
#define CARD_BRAMBLE_STRIKES_FROM 60
#define CARD_STRIKES_UNTIL 110
#define CARD_BRAMBLE_STRIKES_UNTIL 96
/// The vines lean CARD_VINE_ANGLE, each swaying TRIG_TURN / CARD_VINE_SWAY
/// either way from its own phase.
#define CARD_VINE_ANGLE 12192
#define CARD_VINE_SWAY 16

/// The cards of the four suits (Bramble, Frost, Flame and Thunder Card): a
/// card rises from the user, spinning, swinging and growing, then shows its
/// picture, and the background takes its colours. The Frost Card throws
/// crystal shards and the Flame Card puffs of fiery smoke at the targets,
/// while the Bramble Card's thorny vines and the Thunder Card's bolts of
/// lightning reach out over them. Each target is struck every five frames for
/// a while, reeling, flashing and throwing up sparks, before the card shrinks
/// away.
void BattleEffect_RunElementCards(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *graphics;
  FIXED scale;
  struct ScreenPoint card;
  struct ScreenPoint targets[8];
  struct ScreenPoint aim;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  s32 i;
  s32 firstHit;
  s32 lastHit;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas, a clipped additive blitter, the sparks, and the
  // card's picture and its extras.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  scale = 0;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  if (mode == CARD_BRAMBLE) {
    Resource_LoadAndDecompress(RESOURCE_VINE_CARD_SHEET, work->sheet, 1, 1);
  } else if (mode == CARD_FROST) {
    Resource_LoadAndDecompress(RESOURCE_DRAGON_CARD_SHEET, work->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_CRYSTAL_SHEET,
                               work->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
  } else if (mode == CARD_FLAME) {
    Resource_LoadAndDecompress(RESOURCE_SUN_CARD_SHEET, work->sheet, 1, 1);
    // DEAD: SmokeSheet is decoded over FlashBurstSheet straight after; the ROM
    // keeps both loads.
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               work->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET,
                               work->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
  } else {
    Resource_LoadAndDecompress(RESOURCE_BOLT_CARD_SHEET, work->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_JAGGED_STREAK_SHEET,
                               work->sheet + ELEMENT_CARDS_PICTURES, 0, 0);
  }
  REG_BLDCNT = 0;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  for (frame = 0; frame != gElementCards_Frames[mode]; frame++) {
    // Frame 0: clear the particles, give each member a strike countdown, and
    // find the user and every target.
    if (frame == 0) {
      Sound_PlayFar(SE_WARP);
      EFFECT_FREE_PARTICLES(i, work->particles, CARD_PIECES);
      for (i = 0; i != args->targetCount; i++)
        work->particles[CARD_STRIKE_TIMERS + i].variant = (Random16() & 3) + 2;
      EFFECT_FREE_PARTICLES(i, EXTRA_PARTICLES, LEGEND_SPARK_COUNT);
      EffectPosition_ProjectSprite(args->user, &card);
      for (i = 0; i != args->targetCount; i++)
        EffectPosition_ProjectScaledBattler(args->targets[i], &targets[i]);
      REG_BLDCNT = 0;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      scale = FIXED_ONE / 4;
    }

    // The card rises and grows, and later rises faster and shrinks.
    if (frame < CARD_RISE_FRAMES) {
      card.y--;
      scale += FIXED_ONE / 128;
      if (scale > FIXED_ONE / 2)
        scale = FIXED_ONE / 2;
    }
    if (frame >= CARD_SHRINK_FRAME) {
      scale -= FIXED_ONE / 128;
      card.y -= 2;
    }

    // The card shows its picture.
    if (frame == CARD_RISE_FRAMES) {
      if (mode == CARD_BRAMBLE)
        Sound_PlayFar(SE_BOSS_ROAR);
      if (mode == CARD_FROST)
        Sound_PlayFar(SE_CLATTER);
      if (mode == CARD_FLAME)
        Sound_PlayFar(SE_LAUNCH);
      if (mode == CARD_THUNDER)
        Sound_PlayFar(SE_RISE_WITH_SPARKS);
      if (mode != CARD_BRAMBLE) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      }
    }

    if (frame >= CARD_RISE_FRAMES) {
      // The background steps toward the card's colours.
      if (mode != CARD_BRAMBLE) {
        if (mode == CARD_FROST)
          BattleEffect_StepPaletteToResource(RESOURCE_CRYSTAL_SHEET);
        else if (mode == CARD_THUNDER)
          BattleEffect_StepPaletteToResource(RESOURCE_VIOLET_PALETTE_F);
        else
          BattleEffect_StepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
      }

      // The Frost and Flame cards throw one shard or puff of smoke a frame at
      // a target in turn, until the card shrinks.
      if (mode == CARD_FROST || mode == CARD_FLAME) {
        s32 member = frame % args->targetCount;

        aim.x = targets[member].x + (Random16() & 63) - 32;
        if (mode == CARD_FROST)
          aim.y = targets[member].y + (Random16() & 255) - 96;
        else if (mode == CARD_FLAME)
          aim.y = targets[member].y + (Random16() & 63) + 32;
        if (frame < CARD_SHRINK_FRAME) {
          struct Particle *step = &work->particles[frame % CARD_PIECES];

          step->x = card.x * (toFIXED(1) / 2);
          step->y = INTtoFIXED(card.y);
          step->velocityX = (aim.x - card.x) * (toFIXED(1) / 32);
          step->velocityY = (aim.y - card.y) * (toFIXED(1) / 32);
          step->z = CARD_SHARD_TUMBLE_SPEED;
          step->variant = 0;
        }
      }

      if (mode == CARD_FROST) {
        // The shards tumble through their three cells, bounce and slow.
        for (i = 0; i != CARD_PIECES; i++) {
          struct Particle *step = &work->particles[i];

          if (step->variant >= 0 && step->variant < CARD_SHARD_TUMBLE_END) {
            s32 cell = i % 5 * 3 + step->variant / SHARD_TURN % 3;

            // UB: a fifth of the shards (i % 5 == 4) take cells 12 to 14, past
            // the end of gPrism_ShardPictures' twelve; the heights read there
            // are 0, so they draw nothing.
#ifdef UBFIX
            if (i % 5 != 4)
#endif
              draw[0](
                  canvas,
                  work->sheet + gPrism_ShardPictures[cell] +
                      ELEMENT_CARDS_PICTURES + CRYSTAL_SHARD_PICTURES,
                  FIXED_INTEGER_PART(step->x) - gPrism_ShardWidths[cell] / 2,
                  FIXED_INTEGER_PART(step->y) - gPrism_ShardHeights[cell] / 2,
                  gPrism_ShardWidths[cell], gPrism_ShardHeights[cell]);
            Particle_AdvanceWithGravity2D(step, PARTICLE_DAMPING_NONE, 0);
            if (step->y > toFIXED(CARD_GROUND_Y))
              step->velocityY = -step->velocityY / 2;
            step->variant += step->z;
            if (step->z > 1 && (frame & 1))
              step->z--;
          }
        }
      } else if (mode == CARD_FLAME) {
        // The smoke flies to the ground and plays its six cells.
        for (i = 0; i != CARD_PIECES; i++) {
          struct Particle *step = &work->particles[i];

          if (step->variant >= 0) {
            if (step->variant > 1)
              draw[0](canvas,
                      work->sheet + ELEMENT_CARDS_PICTURES +
                          step->variant / 4 * PICTURE_BYTES(SMOKE_PUFF),
                      FIXED_INTEGER_PART(step->x) - 12,
                      FIXED_INTEGER_PART(step->y) - 24, SMOKE_PUFF_WIDTH,
                      SMOKE_PUFF_HEIGHT);
            step->x += step->velocityX;
            step->y += step->velocityY;
            if (step->y > toFIXED(CARD_GROUND_Y)) {
              step->y = toFIXED(CARD_GROUND_Y);
              step->velocityY = 0;
            }
            step->variant++;
            if (step->variant == SMOKE_PUFF_FRAMES)
              step->variant = -1;
          }
        }
      }
    }

    // The strikes: every five frames each target in turn reels, flashes and
    // throws up 16 sparks.
    if (mode == CARD_THUNDER)
      firstHit = CARD_THUNDER_STRIKES_FROM;
    else if (mode == CARD_BRAMBLE)
      firstHit = CARD_BRAMBLE_STRIKES_FROM;
    else
      firstHit = CARD_STRIKES_FROM;
    if (mode == CARD_BRAMBLE)
      lastHit = CARD_BRAMBLE_STRIKES_UNTIL;
    else
      lastHit = CARD_STRIKES_UNTIL;
    if (frame >= firstHit && frame < lastHit) {
      s32 speed = (frame - CARD_STRIKES_FROM) * 3;

      for (i = 0; i != args->targetCount; i++) {
        struct Particle *step = &work->particles[CARD_STRIKE_TIMERS + i];

        if (--step->variant == 0) {
          s32 burst;
          s32 j;

          step->variant = 5;
          work->shakeFrames = 4;
          if (mode == CARD_FROST)
            Sound_PlayFar(SE_HEAVY_IMPACT);
          else if (mode == CARD_THUNDER)
            Sound_PlayFar(SE_THUNDER_CARD);
          else
            Sound_PlayFar(SE_IMPACT);
          if (i == 0)
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(2),
                                        toFIXED(8), toFIXED(1), 100 + speed);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
          burst = 0;
          EffectPosition_ProjectScaledBattler(args->targets[i], &aim);
          for (j = 0; j != CARD_SPARKS_PER_TARGET; j++) {
            struct Particle *spark =
                &EXTRA_PARTICLES[i * CARD_SPARKS_PER_TARGET + j];

            if (spark->variant < 0) {
              s32 angle;
              s32 force;

              spark->x = aim.x * (toFIXED(1) / 2);
              spark->y = INTtoFIXED(aim.y + 8);
              angle = Random16() % (TRIG_TURN * 3 / 4) + DEGtoANG(45);
              force = Random16() & 127;
              spark->velocityX = (Trig_Sin(angle) * force) >> 5;
              spark->velocityY = (Trig_Cos(angle) * force) >> 4;
              if (mode == CARD_THUNDER) {
                spark->velocityX /= 2;
                spark->velocityY /= 2;
              }
              spark->variant = (Random16() & 7) + 16;
              if (++burst == CARD_STRIKE_SPARKS)
                break;
            }
          }
        }
      }
    }

    // This frame's models: the card, then the textures.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    texture.logHeight = 6;
    texture.pixels = work->sheet;
    model->kind = MODEL_TEXTURED;
    model->texture = &texture;
    model->mesh = gBattleEffect_TallMesh;
    model->vertices = vertices;
    // DEAD: frame is never negative; the ROM keeps the test.
    if (frame >= 0) {
      // The card: it swings from side to side as it rises, tilted back.
      Matrix_Initialize();
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
      Matrix_Translate(INTtoFIXED(card.x) - toFIXED(128),
                       INTtoFIXED(card.y) - toFIXED(64), 0);
      if (frame < CARD_RISE_FRAMES) {
        Matrix_RotateX((CARD_RISE_FRAMES - frame) * (TRIG_TURN / 64));
        if (args->side == EFFECT_SIDE_ENEMIES)
          Matrix_Translate(-Trig_Sin(frame * (TRIG_TURN / 96)) << 7, 0, 0);
        else
          Matrix_Translate(Trig_Sin(frame * (TRIG_TURN / 96)) << 7, 0, 0);
      }
      Matrix_RotateY(CARD_TILT);
      Matrix_RotateZ(frame * (TRIG_TURN / 64));
      Matrix_ScaleUniform(scale);
      Matrix_Scale(FIXED_ONE, FIXED_ONE * 2, FIXED_ONE);
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(model);
      if (mode == CARD_BRAMBLE || mode == CARD_THUNDER) {
        Matrix_ScaleUniform(FIXED_ONE / 4);
        Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      }
    }

    // The Bramble Card's vines and the Thunder Card's bolts reach out from the
    // card's corners for 64 frames; the bolts fade as the card shrinks.
    if ((mode == CARD_THUNDER || mode == CARD_BRAMBLE) &&
        (frame >= CARD_RISE_FRAMES && frame <= 111)) {
      s32 alpha = 0;

      if (mode == CARD_THUNDER && frame >= CARD_SHRINK_FRAME)
        alpha = frame * -4 + 384;
      texture.logWidth = 5;
      texture.logHeight = 7;
      model->shade = alpha;
      model->texture = &texture;
      model->mesh = gBattleEffect_ColumnMesh;
      model->vertices = vertices;
      if (mode == CARD_BRAMBLE) {
        // The vines sway, each from its own phase.
        model->kind = MODEL_SHADED;
        if (frame == 48) {
          s32 first = Random16();

          for (i = 0; i != 4; i++) {
            struct Particle *step = &work->particles[i];

            step->velocityY = i * (TRIG_TURN / 8);
            step->z = (first + i) % 3 * PICTURE_BYTES(CARD_STREAK) +
                      PICTURE_BYTES(CARD);
          }
        }
        for (i = 0; i != 4; i++) {
          struct Particle *step = &work->particles[i];

          step->velocityZ =
              Trig_Sin(step->velocityY + frame * (TRIG_TURN / 64)) /
                  CARD_VINE_SWAY +
              CARD_VINE_ANGLE;
        }
      } else {
        // The bolts flicker every other frame.
        model->kind = MODEL_SHADED_ADDITIVE;
        if ((frame & 1) == 0) {
          s32 first = Random16();

          for (i = 0; i != 4; i++) {
            struct Particle *step = &work->particles[i];

            step->velocityZ = (Random16() & 8191) + 8096;
            step->z = (first + i) % 3 * PICTURE_BYTES(CARD_STREAK) +
                      ELEMENT_CARDS_PICTURES;
          }
        }
      }
      for (i = 0; i != 4; i++) {
        work->particles[i].x = ((s16 *)vertices)[(i & 3) * 4];
        work->particles[i].y = ((s16 *)vertices)[(i & 3) * 4 + 1];
      }
      for (i = 0; i != 4; i++) {
        struct Particle *step = &work->particles[i];

        texture.pixels = work->sheet + step->z;
        Matrix_Initialize();
        // Each vine or bolt stands 4 pixels across from its corner.
        Matrix_Translate(INTtoFIXED(step->x + 4 - CANVAS_SIZE / 2),
                         INTtoFIXED(step->y - CANVAS_SIZE / 2), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        if (args->side == EFFECT_SIDE_PARTY)
          Matrix_RotateY(DEGtoANG(180));
        Matrix_RotateZ(step->velocityZ);
        if (mode == CARD_BRAMBLE) {
          s32 length = frame * (FIXED_ONE / 8) - FIXED_ONE * 6;

          if (length > FIXED_ONE * 2)
            length = FIXED_ONE * 2;
          if (frame >= CARD_SHRINK_FRAME)
            length = length - frame * (FIXED_ONE / 8) + FIXED_ONE * 12;
          Matrix_Scale(FIXED_ONE * 4, length, FIXED_ONE);
        } else {
          Matrix_Scale(FIXED_ONE * 2, FIXED_ONE * 2, FIXED_ONE);
        }
        Render_TransformVertices(gBattleEffect_BeamCorners, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    // The sparks bounce on the ground.
    for (i = 0; i != LEGEND_SPARK_COUNT; i++) {
      struct Particle *step = &EXTRA_PARTICLES[i];

      if (step->variant >= 0) {
        s32 size = step->variant >> 3;
        s32 j;

        if (mode == CARD_BRAMBLE || mode == CARD_THUNDER)
          size += 1;
        else
          size += 3;
        for (j = 0; j != gElementCards_SparkMotions[mode].steps; j++) {
          EFFECT_DRAW_MOTE(draw[0], canvas, graphics,
                           FIXED_INTEGER_PART(step->x),
                           FIXED_INTEGER_PART(step->y), size);
          if (step->y > toFIXED(SPARK_GROUND_Y)) {
            step->y = toFIXED(SPARK_GROUND_Y);
            step->velocityY = -step->velocityY / 2;
          }
          Particle_AdvanceWithGravity2D(
              step, gElementCards_SparkMotions[mode].damping,
              gElementCards_SparkMotions[mode].gravity);
        }
        step->variant--;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Abilities: Sleep Card.
void BattleEffect_RunSleepCard(struct BattleEffectParams *args) {
  BattleEffect_RunCards(args, CARD_SLEEP);
}

/// Abilities: Baffle Card.
void BattleEffect_RunBaffleCard(struct BattleEffectParams *args) {
  BattleEffect_RunCards(args, CARD_BAFFLE);
}

/// Abilities: Death Card.
void BattleEffect_RunDeathCard(struct BattleEffectParams *args) {
  BattleEffect_RunCards(args, CARD_DEATH);
}

/// Abilities: Sword Card.
void BattleEffect_RunSwordCard(struct BattleEffectParams *args) {
  BattleEffect_RunCards(args, CARD_SWORD);
}
#undef LEGEND_SPARK_COUNT
#undef SPARK_GROUND_Y
#undef LEGEND_SWORD_SPARKS
#undef LEGEND_BLADE_PICTURES
#undef LEGEND_SWORD_WIDTH
#undef LEGEND_SWORD_HEIGHT
#undef LEGEND_FLAME_GRADIENT
#undef LEGEND_FAN_FLAMES
#undef LEGEND_FAN_TILT
#undef LEGEND_FIRE_RING_GROWTH
#undef LEGEND_SWORD_INTERVAL
#undef LEGEND_SWORD_FALL_FRAME
#undef LEGEND_SWORD_SOUND_FRAME
#undef LEGEND_SWORD_STRIKE_FRAME
#undef LEGEND_SWORD_FALL_FRAMES
#undef LEGEND_SWORD_START_DISTANCE
#undef LEGEND_FRAMES
#undef CARD_STREAK_WIDTH
#undef CARD_STREAK_HEIGHT
#undef ELEMENT_CARDS_PICTURES
#undef CARD_PIECES
#undef CARD_STRIKE_TIMERS
#undef CARD_SPARKS_PER_TARGET
#undef CARD_STRIKE_SPARKS
#undef CARD_RISE_FRAMES
#undef CARD_SHRINK_FRAME
#undef CARD_TILT
#undef CARD_GROUND_Y
#undef CARD_SHARD_TUMBLE_SPEED
#undef CARD_SHARD_TUMBLE_END
#undef CARD_STRIKES_FROM
#undef CARD_THUNDER_STRIKES_FROM
#undef CARD_BRAMBLE_STRIKES_FROM
#undef CARD_STRIKES_UNTIL
#undef CARD_BRAMBLE_STRIKES_UNTIL
#undef CARD_VINE_ANGLE
#undef CARD_VINE_SWAY
