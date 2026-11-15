/// @file
/// Legend, and the Bramble, Frost, Flame and Thunder Cards.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The sparks live in the map cell buffer's second 16 KB, 128 for each sword.
#define SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

// A texture for the model kernels: its size as powers of two, then its
// pixels.

/// Where BladeSheet is decoded in the effect's pictures.
#define LEGEND_BLADE_PICTURES 0x4000

/// Where FlameGradientTexture is decoded in the effect's pictures.
#define LEGEND_FLAME_GRADIENT 0x5000

/// Legend: a fan of eight blades opens over the target for
/// 50 frames while a rune, mirrored from one corner, rises and turns behind
/// it. Then one sword for each step of the variant (variant + 1 in all, 16
/// frames apart) falls on the target from above the user's side: it strikes
/// at 59 frames plus its delay, throwing 128 sparks that bounce on the ground,
/// while a flash grows and fades where it lands. The effect lasts 86 frames
/// and 16 more for each further sword.
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

  // Set up the canvas and a clipped MAX blitter, mirrored to face the
  // target; mirror the rune's corner into the sheet; load the sparks, the
  // sword and the flash.
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
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneASheet, cells, 1, 1);
  Picture_MirrorIntoQuadrants(cells, work->sheet, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BladeSheet,
                             work->sheet + LEGEND_BLADE_PICTURES, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0E9,
                             work->sheet + LEGEND_FLAME_GRADIENT, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, cells, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != args->variant * 16 + 86; frame++) {
    // Frame 0: the swords set out from 80 pixels above and beside the
    // target, on the user's side, toward it.
    if (frame == 0) {
      EffectPosition_ProjectBattler(args->targets[0], &target);
      if (args->side == EFFECT_SIDE_ENEMIES)
        start.x = target.x + 80;
      else
        start.x = target.x - 80;
      start.y = target.y - 80;
      for (i = 0; i != 3; i++) {
        step = &work->particles[i];
        step->x = start.x << 15;
        step->y = px(start.y);
        step->velocityX = (target.x - start.x) << 11;
        step->velocityY = (target.y - start.y) << 12;
        step->variant = 0;
      }
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
    }

    // The swords, 16 frames apart.
    for (i = 0; i != args->variant + 1; i++) {
      step = &work->particles[i];
      if (frame == i * 16 + 54)
        Sound_PlayFar(SE_POWER_UP);

      // The strike: the target reels and flashes, and 128 sparks fly up
      // from it. The last sword ends the action.
      if (frame == i * 16 + 59) {
        if (i == args->variant)
          EFFECT_SHOW_DAMAGE(SE_CLATTER);
        else
          Sound_PlayFar(SE_CLATTER);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, 0, 100);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        for (j = 0; j != 128; j++) {
          struct Particle *spark = &SPARKS[i * 128 + j];
          s32 angle;
          s32 force;

          spark->x = target.x << 15;
          spark->y = px(96);
          angle = Random16() % (TRIG_TURN * 3 / 4) + TRIG_TURN / 8;
          force = Random16() & 127;
          spark->velocityX = (Trig_Sin(angle) * force) >> 4;
          spark->velocityY = (Trig_Cos(angle) * force) >> 3;
          spark->variant = (Random16() & 7) + 16;
        }
      }

      // From frame 50 plus its delay the sword falls, faster each frame,
      // until it sticks in the ground 12 frames later.
      if (frame >= i * 16 + 50) {
        s32 x = Q_16_16_UPPER_HALF(step->x) - 20;
        s32 y = Q_16_16_UPPER_HALF(step->y) - 50;
        s32 height = 80;

        if (y > 24)
          height = 104 - y;
        draw[0](canvas, work->sheet + LEGEND_BLADE_PICTURES, x, y, 40, height);
        if (step->variant <= 12) {
          step->x += step->velocityX;
          step->y += step->velocityY;
          step->velocityX += (target.x - start.x) << 9;
          step->velocityY += (target.y - start.y) << 10;
        }
        step->variant++;
      }
    }

    // The sparks bounce once on the ground at y = 104.
    for (i = 0; i != SPARK_COUNT; i++) {
      step = &SPARKS[i];
      if (step->variant >= 0) {
        s32 size = (step->variant >> 2) + 1;

        draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                Q_16_16_UPPER_HALF(step->x) - size / 2,
                Q_16_16_UPPER_HALF(step->y) - size, size, size * 2);
        if (step->y > px(104)) {
          step->y = px(104);
          step->velocityY = -step->velocityY / 2;
        }
        BattleEffectKernels_IntegrateVector2(step, 62, px(1) / 4);
        step->variant--;
      }
    }

    // This frame's models: one quad, retextured for each part.
    {
      struct RenderModel *model;
      void *vertices;
      s32 scale;

      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet;
      // Stored as an integer: as a pointer this store follows the kind store.
      *(s32 *)&model->mesh = (s32)gBattleEffect_LargeSquareMesh;
      model->kind = 7;
      model->texture = &texture;
      model->vertices = vertices;

      // Frames 16-107: the rune rises behind the target, turning, then fades.
      if (frame >= 16 && frame <= 107) {
        s32 height = frame * 2 - 96;
        s32 alpha = 0;

        scale = (frame << 13) - Q16_ONE * 2;
        if (height > -48)
          height = -48;
        if (frame >= args->variant * 16 + 60)
          // Spelled with + -240 so that the negation is not folded into 240 -
          // x.
          alpha = -((frame - args->variant * 16) * 4 + -240);
        if (scale > Q16_ONE * 2)
          scale = Q16_ONE * 2;
        model->shade = alpha;
        Transform_Reset();
        Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyTranslation(px(start.x - 128), px(height), 0);
        Transform_ApplyPitch(TRIG_TURN * 89 / 256);
        if (args->side == EFFECT_SIDE_ENEMIES)
          Transform_ApplyYaw(TRIG_TURN * 25 / 256);
        else
          Transform_ApplyYaw(-TRIG_TURN * 25 / 256);
        Transform_ApplyRoll(frame << 9);
        Transform_ApplyUniformScale(scale);
        Transform_ApplyAxisScale(Q16_ONE, Q16_ONE, Q16_ONE);
        Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
        Render_DrawModelList(model);
      }

      // Each sword's flash grows where it lands and fades over 16 frames.
      texture.pixels = cells;
      for (i = 0; i != args->variant + 1; i++) {
        if (frame >= i * 16 + 50) {
          s32 age = frame - i * 16 - 50;
          s32 alpha = 64 - age * 8;

          if (alpha > 0)
            alpha = 0;
          if (alpha > -64) {
            scale = age * 12384 + Q16_ONE / 2;
            model->shade = alpha;
            Transform_Reset();
            Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
            Transform_ApplyTranslation(px(start.x - 128), -px(48), 0);
            Transform_ApplyPitch(TRIG_TURN * 89 / 256);
            if (args->side == EFFECT_SIDE_ENEMIES)
              Transform_ApplyYaw(TRIG_TURN * 25 / 256);
            else
              Transform_ApplyYaw(-TRIG_TURN * 25 / 256);
            Transform_ApplyRoll(frame << 9);
            Transform_ApplyUniformScale(scale);
            Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
            Render_DrawModelList(model);
          }
        }
      }

      // Frames 0-49: the fan of eight blades opens, pulsing, and closes.
      if (frame <= 49) {
        texture.logWidth = 5;
        texture.logHeight = 6;
        texture.pixels = work->sheet + LEGEND_FLAME_GRADIENT;
        model->mesh = gBattleEffect_TallMesh;
        for (i = 0; i != 8; i++) {
          model->shade = 0;
          Transform_Reset();
          Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          Transform_ApplyTranslation(px(start.x - 128), -px(80), 0);
          Transform_ApplyPitch(0x47d0);
          Transform_ApplyRoll((frame + i * 16) << 9);
          scale = (frame << 12) - Q16_ONE / 4;
          if (scale > Q16_ONE)
            scale = Q16_ONE;
          if (frame > 31)
            scale = Q16_ONE * 3 - (frame << 12);
          Transform_ApplyAxisScale(
              0x8000, 0x10000,
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

/// Where the card's element pictures are decoded in the effect's pictures:
/// CrystalSheet, FlashBurstSheet, SmokeSheet or JaggedStreakSheet.
#define ELEMENT_CARDS_PICTURES 0x800

/// The four cards (Bramble, Frost, Flame and Thunder Card): a card rises from
/// the user, spinning and growing, for 48 frames, then shows its picture.
/// From frame 48 the background takes the card's colours; the Frost and
/// Flame cards throw shards or smoke at the targets, and the Bramble and
/// Thunder cards spread four pictures over the field. Each target is struck
/// every five frames for a while, reeling, flashing and throwing up sparks.
/// From frame 96 the card shrinks away.
void BattleEffect_RunElementCards(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *graphics;
  s32 scale;
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
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  if (mode == CARD_BRAMBLE) {
    Resource_LoadAndDecompress((s32)&ResourceId_VineCardSheet, work->sheet, 1,
                               1);
  } else if (mode == CARD_FROST) {
    Resource_LoadAndDecompress((s32)&ResourceId_DragonCardSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet,
                               work->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
  } else if (mode == CARD_FLAME) {
    Resource_LoadAndDecompress((s32)&ResourceId_SunCardSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + ELEMENT_CARDS_PICTURES, 1, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_BoltCardSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0F8,
                               work->sheet + ELEMENT_CARDS_PICTURES, 0, 0);
  }
  REG_BLDCNT = 0;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  for (frame = 0; frame != gElementCards_Frames[mode]; frame++) {
    // Frame 0: clear the particles, give each member a strike countdown, and
    // find the user and every target.
    if (frame == 0) {
      Sound_PlayFar(SE_WARP);
      EFFECT_FREE_PARTICLES(i, work->particles, 56);
      for (i = 0; i != args->targetCount; i++)
        work->particles[56 + i].variant = (Random16() & 3) + 2;
      for (i = 0; i != SPARK_COUNT; i++)
        SPARKS[i].variant = -1;
      EffectPosition_ProjectSprite(args->user, &card);
      for (i = 0; i != args->targetCount; i++)
        EffectPosition_ProjectScaledBattler(args->targets[i], &targets[i]);
      REG_BLDCNT = 0;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      scale = Q16_ONE / 4;
    }

    // The card rises and grows for 48 frames, and from frame 96 rises
    // faster and shrinks.
    if (frame <= 47) {
      card.y--;
      scale += 0x200;
      if (scale > Q16_ONE / 2)
        scale = Q16_ONE / 2;
    }
    if (frame > 95) {
      scale += -0x200;
      card.y -= 2;
    }

    // Frame 48: the card shows its picture.
    if (frame == 48) {
      if (mode == CARD_BRAMBLE)
        Sound_PlayFar(SE_BOSS_ROAR);
      if (mode == CARD_FROST)
        Sound_PlayFar(SE_CLATTER);
      if (mode == CARD_FLAME)
        Sound_PlayFar(SE_104);
      if (mode == CARD_THUNDER)
        Sound_PlayFar(SE_RISE_WITH_SPARKS);
      if (mode != CARD_BRAMBLE) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      }
    }

    if (frame > 47) {
      // The background steps toward the card's colours.
      if (mode != CARD_BRAMBLE) {
        if (mode == CARD_FROST)
          BattleEffect_StepPaletteToResource((s32)&ResourceId_CrystalSheet);
        else if (mode == CARD_THUNDER)
          BattleEffect_StepPaletteToResource((s32)&ResourceId_VioletPaletteF);
        else
          BattleEffect_StepPaletteToResource((s32)&ResourceId_OrangePaletteB);
      }

      // The Frost and Flame cards throw one particle a frame at a target in
      // turn, until frame 95.
      if (mode == CARD_FROST || mode == CARD_FLAME) {
        s32 member = frame % args->targetCount;

        aim.x = targets[member].x + (Random16() & 63) - 32;
        if (mode == CARD_FROST)
          aim.y = targets[member].y + (Random16() & 255) - 96;
        else if (mode == CARD_FLAME)
          aim.y = targets[member].y + (Random16() & 63) + 32;
        if (frame <= 95) {
          struct Particle *step = &work->particles[frame % 56];

          step->x = card.x << 15;
          step->y = px(card.y);
          step->velocityX = (aim.x - card.x) << 11;
          step->velocityY = (aim.y - card.y) << 11;
          step->z = 32;
          step->variant = 0;
        }
      }

      if (mode == CARD_FROST) {
        // The shards tumble through their three cells, bounce and slow.
        for (i = 0; i != 56; i++) {
          struct Particle *step = &work->particles[i];

          if ((u32)step->variant <= 1023) {
            s32 cell = i % 5 * 3 + step->variant / 96 % 3;

            draw[0](canvas, work->sheet + gPrism_ShardPictures[cell] + 0x1000,
                    Q_16_16_UPPER_HALF(step->x) - gPrism_ShardWidths[cell] / 2,
                    Q_16_16_UPPER_HALF(step->y) - gPrism_ShardHeights[cell] / 2,
                    gPrism_ShardWidths[cell], gPrism_ShardHeights[cell]);
            BattleEffectKernels_IntegrateVector2(step, 64, 0);
            if (step->y > px(96))
              step->velocityY = -step->velocityY / 2;
            step->variant += step->z;
            if (step->z > 1 && (frame & 1))
              step->z--;
          }
        }
      } else if (mode == CARD_FLAME) {
        // The smoke flies to the ground and plays its six cells.
        for (i = 0; i != 56; i++) {
          struct Particle *step = &work->particles[i];

          if (step->variant >= 0) {
            if (step->variant > 1)
              draw[0](canvas,
                      work->sheet + ELEMENT_CARDS_PICTURES +
                          step->variant / 4 * PICTURE_BYTES(SMOKE_PUFF),
                      Q_16_16_UPPER_HALF(step->x) - 12,
                      Q_16_16_UPPER_HALF(step->y) - 24, SMOKE_PUFF_WIDTH,
                      SMOKE_PUFF_HEIGHT);
            step->x += step->velocityX;
            step->y += step->velocityY;
            if (step->y > px(96)) {
              step->y = px(96);
              step->velocityY = 0;
            }
            step->variant++;
            if (step->variant == 24)
              step->variant = -1;
          }
        }
      }
    }

    // The strikes: every five frames each target in turn reels, flashes and
    // throws up 16 sparks.
    if (mode == CARD_THUNDER)
      firstHit = 52;
    else if (mode == CARD_BRAMBLE)
      firstHit = 60;
    else
      firstHit = 68;
    if (mode == CARD_BRAMBLE)
      lastHit = 96;
    else
      lastHit = 110;
    if (frame >= firstHit && frame < lastHit) {
      s32 speed = (frame - 68) * 3;

      for (i = 0; i != args->targetCount; i++) {
        struct Particle *step = &work->particles[56 + i];

        if (--step->variant == 0) {
          s32 burst;
          s32 j;

          step->variant = 5;
          work->shakeFrames = 4;
          if (mode == CARD_FROST)
            Sound_PlayFar(SE_HEAVY_IMPACT);
          else if (mode == CARD_THUNDER)
            Sound_PlayFar(SE_177);
          else
            Sound_PlayFar(SE_IMPACT);
          if (i == 0)
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          BattleEffect_StartActorMotion(args->targets[i], 1, px(2), px(8),
                                        px(1), 100 + speed);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
          burst = 0;
          EffectPosition_ProjectScaledBattler(args->targets[i], &aim);
          for (j = 0; j != 64; j++) {
            struct Particle *spark = &SPARKS[i * 64 + j];

            if (spark->variant < 0) {
              s32 angle;
              s32 force;

              spark->x = aim.x << 15;
              spark->y = px(aim.y + 8);
              angle = Random16() % (TRIG_TURN * 3 / 4) + TRIG_TURN / 8;
              force = Random16() & 127;
              spark->velocityX = (Trig_Sin(angle) * force) >> 5;
              spark->velocityY = (Trig_Cos(angle) * force) >> 4;
              if (mode == CARD_THUNDER) {
                spark->velocityX /= 2;
                spark->velocityY /= 2;
              }
              spark->variant = (Random16() & 7) + 16;
              if (++burst == 16)
                break;
            }
          }
        }
      }
    }

    // This frame's models: the card, then the pictures.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    texture.logHeight = 6;
    texture.pixels = work->sheet;
    model->kind = 8;
    model->texture = &texture;
    model->mesh = gBattleEffect_TallMesh;
    model->vertices = vertices;
    if (frame >= 0) {
      // The card: it swings from side to side as it rises, tilted back.
      Transform_Reset();
      Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
      Transform_ApplyTranslation(px(card.x) - px(128), px(card.y) - px(64), 0);
      if (frame <= 47) {
        Transform_ApplyPitch((48 - frame) << 10);
        if (args->side == EFFECT_SIDE_ENEMIES)
          Transform_ApplyTranslation(-Trig_Sin(frame * 682) << 7, 0, 0);
        else
          Transform_ApplyTranslation(Trig_Sin(frame * 682) << 7, 0, 0);
      }
      Transform_ApplyYaw(0x1770);
      Transform_ApplyRoll(frame << 10);
      Transform_ApplyUniformScale(scale);
      Transform_ApplyAxisScale(Q16_ONE, Q16_ONE * 2, Q16_ONE);
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(model);
      if (mode == CARD_BRAMBLE || mode == CARD_THUNDER) {
        Transform_ApplyUniformScale(Q16_ONE / 4);
        Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      }
    }

    // The Bramble and Thunder cards spread four pictures from the card's
    // corners for 64 frames; the Thunder Card's fade from frame 96.
    if ((mode == CARD_THUNDER || mode == CARD_BRAMBLE) &&
        (frame >= 48 && frame <= 111)) {
      s32 alpha = 0;

      if (mode == CARD_THUNDER && frame > 95)
        alpha = frame * -4 + 384;
      texture.logWidth = 5;
      texture.logHeight = 7;
      model->shade = alpha;
      model->texture = &texture;
      model->mesh = gBattleEffect_ColumnMesh;
      model->vertices = vertices;
      if (mode == CARD_BRAMBLE) {
        // The vines sway, each from its own phase.
        model->kind = 9;
        if (frame == 48) {
          s32 first = Random16();

          for (i = 0; i != 4; i++) {
            struct Particle *step = &work->particles[i];

            step->velocityY = i << 13;
            step->z = ((first + i) % 3 << 12) + 0x800;
          }
        }
        for (i = 0; i != 4; i++) {
          struct Particle *step = &work->particles[i];

          step->velocityZ =
              Trig_Sin(step->velocityY + (frame << 10)) / 16 + 12192;
        }
      } else {
        // The bolts flicker every other frame.
        model->kind = 7;
        if ((frame & 1) == 0) {
          s32 first = Random16();

          for (i = 0; i != 4; i++) {
            struct Particle *step = &work->particles[i];

            step->velocityZ = (Random16() & 0x1fff) + 8096;
            step->z = ((first + i) % 3 << 12) + 0x800;
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
        Transform_Reset();
        Transform_ApplyTranslation(px(step->x - 60), px(step->y - 64), 0);
        Transform_ApplyAxisScale(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        if (args->side == EFFECT_SIDE_PARTY)
          Transform_ApplyYaw(TRIG_TURN / 2);
        Transform_ApplyRoll(step->velocityZ);
        if (mode == CARD_BRAMBLE) {
          s32 length = (frame << 13) - 0x60000;

          if (length > 0x20000)
            length = 0x20000;
          if (frame > 95)
            length = length - (frame << 13) + 0xc0000;
          Transform_ApplyAxisScale(Q16_ONE * 4, length, Q16_ONE);
        } else {
          Transform_ApplyAxisScale(Q16_ONE * 2, Q16_ONE * 2, Q16_ONE);
        }
        Render_TransformVertices(gBattleEffect_BeamCorners, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    // The sparks bounce once on the ground at y = 104.
    for (i = 0; i != SPARK_COUNT; i++) {
      struct Particle *step = &SPARKS[i];

      if (step->variant >= 0) {
        s32 size = step->variant >> 3;
        s32 j;

        if (mode == CARD_BRAMBLE || mode == CARD_THUNDER)
          size += 1;
        else
          size += 3;
        for (j = 0; j != gElementCards_SparkMotions[mode].steps; j++) {
          draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                  Q_16_16_UPPER_HALF(step->x) - size / 2,
                  Q_16_16_UPPER_HALF(step->y) - size, size, size * 2);
          if (step->y > px(104)) {
            step->y = px(104);
            step->velocityY = -step->velocityY / 2;
          }
          BattleEffectKernels_IntegrateVector2(
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
#undef BLITTER
#undef SPARKS
#undef SPARK_COUNT
