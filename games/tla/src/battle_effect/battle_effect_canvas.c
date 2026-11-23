/// @file
/// Eclipse and the Doom Dragon's change of form, then the canvas layer: its
/// transfers, opening and closing it, its maps, the battle background's
/// darkening behind it, the targets' flash timers, and lines.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "gamepak/far_call/field.h"
#include "system/system.h"
#include "resource_ids.h"
#include "battle/formations.h"

/// The form change's flashes, in the effect scratch, where Eclipse decodes its
/// swirl texture.
#define FLASHES ((struct Particle *)gEffectScratch)

/// Where TornadoSheet is decoded in the effect's pictures.
#define ECLIPSE_TORNADO_PICTURES 8192

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define ECLIPSE_GLOW_BAND_A 16384

/// Where GlowBandTextureB is decoded in the effect's pictures.
#define ECLIPSE_GLOW_BAND_B 14400

/// Where FlameBandTexture is decoded in the effect's pictures.
#define ECLIPSE_FLAME_BAND 1024

/// A glow band texture's bytes: GlowBandTextureA and B are 32 by 4.
#define GLOW_BAND_BYTES (32 * 4)
/// The blast's glow, drawn as shaded circles after GlowBandTextureB.
#define ECLIPSE_BLAST_GLOW (ECLIPSE_GLOW_BAND_B + GLOW_BAND_BYTES)

/// The golden dragon: a sprite for each of GoldenDragonTiles' pictures and one
/// for the first of GoldenDragonHeadTiles', 32x32 in 256 colours, of which
/// each resource brings the first ECLIPSE_DRAGON_COLORS.
#define ECLIPSE_DRAGON_COLORS 240
#define ECLIPSE_BODY_PICTURES 31
#define ECLIPSE_DRAGON_SPRITES 32
/// GoldenDragonHeadTiles' other pictures, which as many of the dragon's sprites
/// take (gEclipse_TileObjects) as it opens its jaws and moves its wings.
#define ECLIPSE_OPEN_JAW_PICTURES 7
/// The sprite that shows a wing tip until the jaws open, then the tip of the
/// snout at (32, 64) about the dragon.
#define ECLIPSE_SNOUT_SPRITE 25

/// The dragon's moments: it bobs from ECLIPSE_BOB_START, opens its jaws and
/// gathers the orb at ECLIPSE_JAWS_OPEN, breathes at ECLIPSE_BREATH, and from
/// ECLIPSE_BREATH_FADE the breath narrows to nothing by ECLIPSE_DRAGON_FRAMES.
#define ECLIPSE_BOB_START 32
#define ECLIPSE_JAWS_OPEN 136
#define ECLIPSE_BREATH 172
#define ECLIPSE_BREATH_FADE 204
#define ECLIPSE_DRAGON_FRAMES 220
/// The canvas fills that flicker the sky while the dragon gathers its breath
/// and breathes, rows of gEclipse_FillFrames and gEclipse_FillLevels.
#define ECLIPSE_FILLS 17

/// The glowing orb the dragon gathers: its radius in variant, drawn as shaded
/// circles into the effect's pictures. It swells to ECLIPSE_ORB_GATHERED as
/// the jaws open and to ECLIPSE_ORB_BREATH as the dragon breathes.
#define ECLIPSE_ORB (work->particles[16])
#define ECLIPSE_ORB_GATHERED 32
#define ECLIPSE_ORB_BREATH 64
/// The head of the blast, as it falls from the top right, and the frames it
/// whites out the field.
#define ECLIPSE_BLAST (work->particles[63])
#define ECLIPSE_BLAST_FRAMES 48

/// Rays of light that spin out from a point, each a tapered beam that widens
/// and narrows over LIGHT_RAY_FRAMES: Eclipse and the form change each set up
/// LIGHT_RAYS, and Eclipse draws only the first ECLIPSE_LIGHT_RAYS.
#define LIGHT_RAYS 16
#define LIGHT_RAY_FRAMES 48
#define ECLIPSE_LIGHT_RAYS 8
/// The frame Eclipse's rays start, each 8 frames after the last.
#define ECLIPSE_RAYS_START 48

/// The swirl's scale, which shrinks from 4 this much a frame while the dragon
/// gathers its breath.
#define ECLIPSE_SWIRL_SHRINK (FIXED_ONE * 375 / 4096)
/// The brightest a band of light is shaded: a quarter of the way to
/// MODEL_SHADE_INVISIBLE.
#define BAND_SHADE_BRIGHTEST (MODEL_SHADE_INVISIBLE / 4)

/// The square ramp drawn as the blast begins, brightening from left to right.
#define ECLIPSE_RAMP_SIZE 120
/// The landing's buffers in the map cell buffer: the dome's vertices and its
/// grid mesh, and the band's vertices and mesh; its motes are
/// ECLIPSE_MOTE_COUNT of EXTRA_PARTICLES.
#define ECLIPSE_DOME_VERTICES (Ram_MapCellBuffer + 2048)
#define ECLIPSE_DOME_MESH (Ram_MapCellBuffer + 4096)
#define ECLIPSE_BAND_VERTICES (Ram_MapCellBuffer + 8192)
#define ECLIPSE_BAND_MESH (Ram_MapCellBuffer + 10240)
#define ECLIPSE_MOTE_COUNT 512
/// The landing's frames, and the motes it throws up each frame.
#define ECLIPSE_LANDING_FRAMES 26
#define ECLIPSE_MOTES_A_FRAME 16
/// The dome the blast lands as: its rings of vertices run from its top, its
/// height up, to its rim, its radius out.
#define ECLIPSE_DOME_RINGS 8
#define ECLIPSE_DOME_RING_VERTICES 16
#define ECLIPSE_DOME_HEIGHT 96
#define ECLIPSE_DOME_RADIUS 48
/// The band of the four rings that spread from the dome: two rings of
/// ECLIPSE_BAND_SEGMENTS vertices (Mesh_BuildCylinderBand).
#define ECLIPSE_BAND_SEGMENTS 32
/// A ring's life, how long it takes to fade in and when it starts to fade out,
/// and how far it leans.
#define ECLIPSE_RING_FRAMES 64
#define ECLIPSE_RING_FADE_IN 16
#define ECLIPSE_RING_FADE_OUT 40
#define ECLIPSE_RING_TILT 10000
/// The aerial view whitens as the blast lands and fades to this tint, its red
/// and green darkened and its blue brightened.
#define ECLIPSE_LANDING_RED (-10)
#define ECLIPSE_LANDING_GREEN (-15)
#define ECLIPSE_LANDING_BLUE 4

/// Eclipse: after a flash the golden dragon looms over the horizon while rays
/// of light spin out from its head; it opens its jaws, gathers an orb of light
/// and breathes it as a blast that whites out the field, and the blast lands
/// on the land far below as a dome that spreads in rings before a last white
/// flash.
///
/// Abilities: Eclipse.
void BattleEffect_RunEclipse(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  s32 frame;
  s32 savedScroll;
  struct BattleBackgroundWork *background;
  u8 *sprites;
  struct BattleWork *battle;
  s32 i;
  s32 j;
  s32 k;
  s32 bgX;
  FIXED baseX;
  FIXED baseY;
  s32 velocityX;
  s32 velocityY;
  FIXED cx;
  FIXED cy;
  u32 u;
  s32 count;
  s32 ring;
  s32 shade;
  s32 offset;
  u8 *cells;
  s32 fade;
  s16 list[UNIT_MAX];
  FIXED place[4];
  struct Scale req;
  struct ModelTexture texture;
  struct ModelTexture breathTexture;
  struct ModelTexture landingTexture;
  struct RenderModel *model;
  struct Particle *p;
  struct Sprite *obj;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  savedScroll = gBgScroll[1].x;
  background = Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  battle = Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  RagingHeat_FillGradientCanvas();
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // The battle flashes away; then the sky and the dragon's sprites load.
  bgX = -80;
  for (frame = 0; frame != SUMMON_OPENING_FRAMES; frame++) {
    if (frame == SUMMON_OPENING_LOAD_FRAME) {
      u8 *tiles;

      list[0] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForListFar(list, 0);
      VramCache_FreeEntry(battle->arrowTiles);
      ActorShadow_FreeTilesFar();
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_EARTH_HORIZON_BACKDROP, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_FRAME);
      background->busy = 1;
      cells = Resource_GetTableEntry(RESOURCE_GOLDEN_DRAGON_TILES);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, ECLIPSE_DRAGON_COLORS * 2);
      cells += ECLIPSE_DRAGON_COLORS * 2;
      Decompress_TaggedLz(cells, gMapGridCellBuffer);
      tiles = gMapGridCellBuffer;
      for (i = 0; i != ECLIPSE_BODY_PICTURES; i++) {
        obj = EffectSprite_CreateWithTiles(32, 32,
                                           OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        obj->image.priority = 2;
        work->sprites[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), tiles, OBJ_32X32_8BPP_BYTES);
        tiles += OBJ_32X32_8BPP_BYTES;
      }
      // The head's first picture; the rest stay decoded for its open jaws.
      cells = Resource_GetTableEntry(RESOURCE_GOLDEN_DRAGON_HEAD_TILES);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, ECLIPSE_DRAGON_COLORS * 2);
      cells += ECLIPSE_DRAGON_COLORS * 2;
      Decompress_TaggedLz(cells, gMapGridCellBuffer);
      obj = EffectSprite_CreateWithTiles(32, 32,
                                         OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
      work->sprites[ECLIPSE_BODY_PICTURES] = obj;
      obj->image.priority = 2;
      Iwram_CopyWords((void *)OBJ_TILES(obj), gMapGridCellBuffer,
                      OBJ_32X32_8BPP_BYTES);
    }
    bgX += 4;
    REG_BG2X = bgX << 8;
    EFFECT_PRESENT_CANVAS(work);
  }
  Iwram_FillWords(canvas, CANVAS_VIEW_BYTES, 0);
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  REG_BG2X = 0;
  baseX = 0;
  baseY = 0;
  velocityX = 0;
  velocityY = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = Ram_HeapSlots->ramCode;
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                  SHEET_PALETTE_BYTES);
  {
    const u8 *texturePixels = (const u8 *)FLASHES;

    // The dragon looms over the horizon while rays of light spin out from its
    // head, then breathes its blast; A or B cuts it short.
    for (frame = 0;
         frame != ECLIPSE_DRAGON_FRAMES && !(gInput.repeat & (KEY_A | KEY_B));
         frame++) {
      if (frame == 0) {
        baseX = -toFIXED(48);
        baseY = toFIXED(212);
        velocityX = toFIXED(4);
        velocityY = -toFIXED(10);
        work->fadeFrames = 1;
        work->fadeStep = frame;
        ECLIPSE_ORB.variant = frame;
        Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE, FLASHES, 0, 0);
      }
      if (frame == ECLIPSE_JAWS_OPEN) {
        // The head's other pictures, still decoded after its first.
        u8 *dst = Ram_MapCellBuffer + OBJ_32X32_8BPP_BYTES;
        for (i = 0; i != ECLIPSE_OPEN_JAW_PICTURES; i++) {

          void *o = (void *)OBJ_TILES(
              (struct Sprite *)work->sprites[gEclipse_TileObjects[i]]);
          s32 size = 0;
          size = OBJ_32X32_8BPP_BYTES;
          Iwram_CopyWordsEntry(o, &dst[i * OBJ_32X32_8BPP_BYTES], size);
        }
      }
      if (frame == ECLIPSE_RAYS_START) {
        Sound_PlayFar(SE_DJINNI_SET);
      }
      if (frame == ECLIPSE_JAWS_OPEN) {
        Sound_PlayFar(SE_SPIN);
      }
      if (frame == ECLIPSE_BREATH) {
        Sound_PlayFar(SE_CLATTER);
      }
      for (i = 0; i != ECLIPSE_FILLS; i++) {
        if (frame == gEclipse_FillFrames[i]) {
          Iwram_FillWords(canvas, CANVAS_VIEW_BYTES, gEclipse_FillLevels[i]);
        }
      }
      req = gEclipse_Scales;
      place[3] = 0;
      place[1] = toFIXED(255);
      baseX += velocityX;
      baseY += velocityY;
      if (frame >= ECLIPSE_BREATH) {
        if (frame > 183) {
          velocityX += -toFIXED(1) / 128;
          velocityY += toFIXED(1) / 64;
        }
        if (frame <= 205) {
          velocityX = velocityX * 54 / DAMPING_SCALE;
          velocityY = velocityY * 54 / DAMPING_SCALE;
        }
      } else {
        if (velocityY < 0) {
          velocityY += toFIXED(1) / 4;
        }
        if (velocityX > 0) {
          velocityX += -toFIXED(1) / 8;
        }
        if (frame >= ECLIPSE_BOB_START) {
          baseY += Trig_Sin((frame - ECLIPSE_BOB_START) << 9) << 2 >> 4;
          if (frame <= 159) {
            baseX += toFIXED(1) / 4;
          }
        }
      }
      for (i = 0; i != ECLIPSE_DRAGON_SPRITES; i++) {
        if (i == ECLIPSE_SNOUT_SPRITE && frame >= ECLIPSE_JAWS_OPEN) {
          place[0] = baseX + toFIXED(32);
          place[2] = baseY + toFIXED(64);
        } else {
          place[0] = baseX + INTtoFIXED(gEclipse_ObjectX[i]);
          place[2] = baseY + INTtoFIXED(gEclipse_ObjectZ[i]);
        }
        if (place[2] <= toFIXED(200) - 1) {
          Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                            (s32 *)&req, 0);
        }
      }
      if (frame == ECLIPSE_JAWS_OPEN) {
        work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
        work->fadeStep = 0;
      }
      // The orb's place, which shifts left and down as it swells.
      cx = baseX / 2 - ECLIPSE_ORB.variant * (toFIXED(1) / 8) + toFIXED(28);
      cy = baseY + ECLIPSE_ORB.variant * (toFIXED(1) / 4) + toFIXED(56);
      u = frame - ECLIPSE_JAWS_OPEN;
      if (u <= ECLIPSE_DRAGON_FRAMES - ECLIPSE_JAWS_OPEN) {
        if (frame == ECLIPSE_JAWS_OPEN) {
          ECLIPSE_ORB.variant = 4;
          ECLIPSE_ORB.x = 0;
          ECLIPSE_ORB.z = 0;
          BattleEffect_DrawShadedCircles(work->sheet, ECLIPSE_ORB.variant * 2,
                                         FIXED_ONE);
        }
        if (frame >= ECLIPSE_BREATH) {
          if (frame == ECLIPSE_BREATH) {
            ECLIPSE_ORB.variant = ECLIPSE_ORB_BREATH;
            BattleEffect_DrawShadedCircles(work->sheet, ECLIPSE_ORB_BREATH * 2,
                                           FIXED_ONE);
            velocityX = toFIXED(3);
            velocityY = -toFIXED(3);
          }
          if (frame >= ECLIPSE_BREATH_FADE && ECLIPSE_ORB.variant > 1) {
            ECLIPSE_ORB.variant -= 4;
            if (ECLIPSE_ORB.variant <= 1) {
              ECLIPSE_ORB.variant = 2;
            }
            BattleEffect_DrawShadedCircles(work->sheet, ECLIPSE_ORB.variant * 2,
                                           FIXED_ONE);
          }
        } else if (frame >= ECLIPSE_JAWS_OPEN) {
          velocityX += -toFIXED(1) / 32;
          velocityY += toFIXED(1) / 32;
          if (++ECLIPSE_ORB.variant <= ECLIPSE_ORB_GATHERED) {
            BattleEffect_DrawShadedCircles(work->sheet, ECLIPSE_ORB.variant * 2,
                                           FIXED_ONE);
          } else {
            ECLIPSE_ORB.variant = ECLIPSE_ORB_GATHERED;
          }
        }
        {
          s32 size = ECLIPSE_ORB.variant;

          Canvas_BlitAtFoot(work->sheet, FIXEDtoINT(cx), FIXEDtoINT(cy),
                            size * 2);
        }
      }
      if (frame >= ECLIPSE_RAYS_START) {
        struct RenderModel *model;
        void *verts;
        s32 modelShade;

        // Each ray has an angle (x), a spin (y) and an age (variant).
        if (frame == ECLIPSE_RAYS_START) {
          for (i = 0; i != LIGHT_RAYS; i++) {
            work->particles[i].x = Random16() & TRIG_ANGLE_MASK;
            work->particles[i].y = (Random16() & 255) + 128;
            if (Random16() & 1) {
              work->particles[i].y = -work->particles[i].y;
            }
            work->particles[i].variant = i * -8;
          }
          Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                                     work->sheet + ECLIPSE_TORNADO_PICTURES, 1,
                                     0);
          Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                                     work->sheet + ECLIPSE_GLOW_BAND_A, 0, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        modelShade = 0;
        model->shade = modelShade;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + ECLIPSE_GLOW_BAND_A;
        model->kind = MODEL_SHADED_MAX;
        model->mesh = gBattleEffect_SmallSquareMesh;
        model->texture = &texture;
        model->vertices = verts;
        for (i = 0; i != ECLIPSE_LIGHT_RAYS; i++) {
          work->particles[i].variant++;
          work->particles[i].x += work->particles[i].y;
          if (work->particles[i].variant >= 0 &&
              work->particles[i].variant < LIGHT_RAY_FRAMES) {
            Matrix_Initialize();
            {
              s32 x = INTtoFIXED((i & 3) * 2) + toFIXED(2);
              s32 o = -toFIXED(64);
              Matrix_Translate(cx + x + o, cy - INTtoFIXED((i & 3) + 2) + o, 0);
            }
            Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
            Matrix_RotateZ(work->particles[i].x);
            Matrix_Scale(Trig_Sin(work->particles[i].variant *
                                  (TRIG_TURN / 2 / LIGHT_RAY_FRAMES)) +
                             FIXED_ONE / 4,
                         FIXED_ONE * 2, FIXED_ONE);
            Render_TransformVertices(gBattleEffect_TaperedBeamCorners, verts,
                                     4);
            Render_DrawModelList(model);
          }
        }
        // As the jaws open a swirl turns in on the orb, shrinking and
        // brightening.
        if (frame >= ECLIPSE_JAWS_OPEN && frame < ECLIPSE_BREATH) {
          s32 s = toFIXED(4) - u * ECLIPSE_SWIRL_SHRINK;

          modelShade = u * 4 + MODEL_SHADE_INVISIBLE;
          if (modelShade > BAND_SHADE_BRIGHTEST) {
            modelShade = BAND_SHADE_BRIGHTEST;
          }
          texture.logWidth = 6;
          model->shade = modelShade;
          texture.logHeight = 6;
          model->kind = MODEL_SHADED_ADDITIVE;
          texture.pixels = texturePixels;
          model->texture = &texture;
          model->mesh = gBattleEffect_SquareMesh;
          model->vertices = verts;
          Matrix_Initialize();
          Matrix_Translate(cx - toFIXED(62), cy - toFIXED(66), 0);
          Matrix_Scale(s >> 1, s, s);
          Matrix_RotateX(DEGtoANG(45));
          Matrix_RotateZ(TRIG_TURN * 5 / 32);
          Matrix_RotateY(-frame * (TRIG_TURN / 64));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
        // The breath: a beam from the orb, narrowing to nothing at the end.
        if (frame >= ECLIPSE_BREATH && frame <= ECLIPSE_DRAGON_FRAMES) {
          Matrix_Initialize();
          Matrix_Translate(cx - toFIXED(62), cy - toFIXED(66), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(TRIG_TURN * 5 / 32);
          if (frame >= ECLIPSE_BREATH_FADE) {
            s32 fadeFrame = frame - ECLIPSE_BREATH_FADE;

            // Half a unit narrower a frame.
            Matrix_Scale(FIXED_ONE * 8 - fadeFrame * (FIXED_ONE / 2),
                         FIXED_ONE * 2, FIXED_ONE);
          } else {
            Matrix_Scale(FIXED_ONE * 8, FIXED_ONE * 2, FIXED_ONE);
          }
          Render_TransformVertices(gBattleEffect_BeamCorners, verts, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      // Glints sparkle about the orb, and then along the breath.
      if (frame >= 148 && frame < ECLIPSE_DRAGON_FRAMES) {
        for (j = 0; j != 4; j++) {
          s32 t = (frame + j) & 3;
          s32 x = 0;
          s32 y = 0;

          if (frame < ECLIPSE_BREATH) {
            s32 size = ECLIPSE_ORB.variant * 2;

            x = Random16() % size + (FIXEDtoINT(cx)) - (size >> 1) -
                (gBattleEffect_GlintPictureWidths[t] >> 1);
            y = Random16() % size + (FIXEDtoINT(cy)) - (size >> 1) -
                (gBattleEffect_GlintPictureHeights[t] >> 1);
          } else {
            u32 a = Random16() & 63;
            s32 r = ECLIPSE_ORB.variant;

            x = (FIXEDtoINT(cx)) - (a >> 1) -
                (gBattleEffect_GlintPictureWidths[t] >> 1) + Random16() % r -
                r / 2 - 8;
            y = a + (FIXEDtoINT(cy)) -
                (gBattleEffect_GlintPictureHeights[t] >> 1);
          }
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              gEclipse_GlintFlips[Random16() & 3] |
                                  EFFECT_CLIP_BOUNDS,
                              BLITTER_MAX);
          ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
              canvas,
              work->sheet + ECLIPSE_TORNADO_PICTURES +
                  gBattleEffect_GlintPictures[t],
              x, y, gBattleEffect_GlintPictureWidths[t],
              gBattleEffect_GlintPictureHeights[t]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_PRESENT_CANVAS(work);
    }
  }
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  gBgScroll[1].x = savedScroll;
  background->busy = 0;
  for (i = 0; i != ECLIPSE_DRAGON_SPRITES; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  ActorShadow_LoadTilesFar();
  battle->arrowTiles = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->args);
  for (i = 0; i != args->targetCount; i++) {
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_GRB, TINT_KEEP,
                             TINT_KEEP, 0);
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_NORMAL, TINT_KEEP,
                             TINT_KEEP, 0);
  }
  Palette_LoadBasicSpriteColors();
  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background,
                           BACKGROUND_DARKENED_LEVEL);
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
  wait(1);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
  Iwram_FillWords(canvas, CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  args->targets[args->targetCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(args->targets, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                             work->sheet + ECLIPSE_GLOW_BAND_B, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  for (i = 0; i != ECLIPSE_RAMP_SIZE; i++) {
    s32 v = i / 2;

    if (v > CANVAS_LEVEL_BRIGHTEST) {
      v = CANVAS_LEVEL_BRIGHTEST;
    }
    for (j = 0; j != ECLIPSE_RAMP_SIZE; j++) {
      work->sheet[j * ECLIPSE_RAMP_SIZE + i] = v;
    }
  }
  // The blast whites out the battlefield.
  fade = 0;
  for (frame = 0; frame != ECLIPSE_BLAST_FRAMES; frame++) {
    if (frame > 39) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(4));
    }
    if (frame == 16) {
      Sound_PlayFar(SE_SPIN);
    }
    if (frame == 18) {
      work->shakeFrames = 90;
      Sound_PlayFar(SE_CRASH);
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 4);
      }
    }
    // DEAD: the blast ends before frame 110, so this test is always true; the
    // ROM keeps its compare.
    if (frame <= 109) {
      struct RenderModel *model = 0;
      void *verts = 0;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      breathTexture.logWidth = 5;
      breathTexture.logHeight = 2;
      breathTexture.pixels = work->sheet + ECLIPSE_GLOW_BAND_B;
      model->mesh = gBattleEffect_SmallSquareMesh;
      model->shade = fade;
      model->texture = &breathTexture;
      model->vertices = verts;
      model->kind = MODEL_SHADED_MAX;
      // The blast's beam, its head sweeping down from the top right over four
      // frames.
      if (frame > 15) {
        struct Particle *q = &ECLIPSE_BLAST;

        if (frame == 16) {
          q->x = toFIXED(128);
          q->y = -toFIXED(40);
        }
        if (frame <= 19) {
          q->x += -toFIXED(32);
          q->y += toFIXED(64);
        }
        if (frame <= 27) {
          cells = work->sheet + ECLIPSE_BLAST_GLOW;
          BattleEffect_DrawShadedCircles(cells, 96, FIXED_ONE);
          Canvas_BlitAtFoot(cells, FIXEDtoINT(q->x), FIXEDtoINT(q->y), 96);
        }
        Matrix_Initialize();
        Matrix_Translate(q->x - toFIXED(63), q->y - toFIXED(64), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateZ(TRIG_TURN * 19 / 32);
        Matrix_Scale(FIXED_ONE * 3525 / 256, FIXED_ONE * 2673 / 1024,
                     FIXED_ONE / 2);
        Render_TransformVertices(gBattleEffect_BeamCorners, verts, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
    }
    {
      s32 r = 8;

      // DEAD: the blast ends before frame 110, so the shake stays at 8; the ROM
      // keeps the three compares.
      if (frame > 130) {
        r = 2;
      } else if (frame > 120) {
        r = 4;
      } else if (frame > 110) {
        r = 6;
      }
      BattleCamera_Shake(r, r);
    }
    EFFECT_END_FRAME(work);
  }
  // The blast lands on the land far below and spreads in rings of light.
  count = -1;
  background->busy = 1;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  Palette_BrightenSavedBattleBg(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX);
  gBgScroll[1].x = 0;
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE,
                             work->sheet + ECLIPSE_FLAME_BAND, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                             work->sheet + ECLIPSE_GLOW_BAND_B, 0, 0);
  // Every mote starts free.
  {
    s32 *v = &EXTRA_PARTICLES[0].variant;
    s32 c = -1;

    for (i = 0; i != ECLIPSE_MOTE_COUNT; i++) {
      *v = c;
      v += sizeof(struct Particle) / sizeof(*v);
    }
  }
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  {
    const void *mesh = ECLIPSE_DOME_MESH;
    void *vertices = gMapGridCellBuffer;
    const void *bandMesh = ECLIPSE_BAND_MESH;

    for (frame = 0; frame != ECLIPSE_LANDING_FRAMES; frame++) {
      // A or B ends the landing eight frames later, the view whitening.
      if ((gInput.repeat & (KEY_A | KEY_B)) && count == -1) {
        count = 8;
      }
      if (frame <= 6 && count == -1) {
        s32 b = RGB_LEVEL_MAX - frame * 8;
        s32 a = b;
        s32 c = b;

        if (c < ECLIPSE_LANDING_RED) {
          a = ECLIPSE_LANDING_RED;
        }
        if (c < ECLIPSE_LANDING_GREEN) {
          b = ECLIPSE_LANDING_GREEN;
        }
        if (c < ECLIPSE_LANDING_BLUE) {
          c = ECLIPSE_LANDING_BLUE;
        }
        Palette_BrightenSavedBattleBg(a, b, c);
      }
      if (frame > 17 || count > 0) {
        Palette_BrightenBattleBg(4, 4, 4);
        if (--count == 0) {
          break;
        }
      }
      // Motes spring up about where the blast landed, farther out each frame.
      for (i = 0; i != ECLIPSE_MOTES_A_FRAME; i++) {
        struct Particle *q =
            EXTRA_PARTICLES +
            ((frame * ECLIPSE_MOTES_A_FRAME + i) & (ECLIPSE_MOTE_COUNT - 1));
        s32 a = 0;
        s32 r = 0;

        a = Random16();
        r = (Random16() & 15) + frame * 2;
        q->x = Trig_Sin(a) * r + toFIXED(60);
        q->y = Trig_Cos(a) * r + toFIXED(60);
        q->velocityX = 0;
        q->velocityY = 0;
        q->variant = (Random16() & 15) + 16;
      }
      for (i = 0; i != ECLIPSE_MOTE_COUNT; i++) {
        p = EXTRA_PARTICLES + i;
        if (p->variant >= 0) {
          s32 n = (p->variant >> 3) + 1;

          blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                  (FIXEDtoINT(p->x)) - n / 2, (FIXEDtoINT(p->y)) - n, n, n * 2);
          Particle_AdvanceWithGravity2D(p, 60, -toFIXED(1) / 4);
          p->variant--;
        }
      }
      model = Render_AllocateModelList(1);
      shade = 0;
      // The blast's last beam, which narrows from frame 16.
      // DEAD: the landing ends before frame 32, so this test is always true;
      // the ROM keeps its compare.
      if (frame <= 31) {
        s32 s = FIXED_ONE;

        if (frame > 15) {
          s += FIXED_ONE - frame * (FIXED_ONE / 16);
        }
        landingTexture.logWidth = 5;
        landingTexture.logHeight = 2;
        landingTexture.pixels = work->sheet + ECLIPSE_GLOW_BAND_B;
        model->mesh = gBattleEffect_SmallSquareMesh;
        model->shade = shade;
        model->vertices = gMapGridCellBuffer;
        model->kind = MODEL_SHADED_MAX;
        model->texture = &landingTexture;
        Matrix_Initialize();
        Matrix_Translate(-toFIXED(2), toFIXED(16), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateZ(TRIG_TURN * 19 / 32);
        Matrix_Scale(s, FIXED_ONE * 2, FIXED_ONE / 2);
        Render_TransformVertices(gBattleEffect_BeamCorners, gMapGridCellBuffer,
                                 4);
        Render_DrawModelList(model);
      }
      // The dome's rings of vertices, from its top to its rim, and the rings'
      // band.
      if (frame == 0) {
        Sound_PlayFar(SE_BANE_ESCAPE);
        Mesh_BuildGrid(ECLIPSE_DOME_MESH, ECLIPSE_DOME_RING_VERTICES,
                       ECLIPSE_DOME_RINGS, 8, 8);
        {
          s32 ringAngle = 0;
          for (ring = 0, offset = 0; ring != ECLIPSE_DOME_RINGS;
               ringAngle += DEGtoANG(12),
              offset += ECLIPSE_DOME_RING_VERTICES * sizeof(struct ModelVertex),
              ring++) {
            s32 y = FIXEDtoINT(-Trig_Cos(ringAngle) * ECLIPSE_DOME_HEIGHT);
            s32 rr = FIXEDtoINT(Trig_Sin(ringAngle) * ECLIPSE_DOME_RADIUS);
            struct ModelVertex *v = (struct ModelVertex *)ECLIPSE_DOME_VERTICES;

            k = 0;
            v = (struct ModelVertex *)((u8 *)v + offset);
            for (; k != ECLIPSE_DOME_RING_VERTICES; k++) {
              SET_POSITION(
                  v,
                  FIXEDtoINT(
                      Trig_Sin(k * (TRIG_TURN / ECLIPSE_DOME_RING_VERTICES)) *
                      rr),
                  y,
                  FIXEDtoINT(
                      Trig_Cos(k * (TRIG_TURN / ECLIPSE_DOME_RING_VERTICES)) *
                      rr));
              v++;
            }
          }
        }
        Mesh_BuildCylinderBand(ECLIPSE_BAND_VERTICES, ECLIPSE_BAND_MESH,
                               ECLIPSE_BAND_SEGMENTS);
      }
      landingTexture.logWidth = 5;
      landingTexture.logHeight = 5;
      landingTexture.pixels = work->sheet;
      {
        model->kind = MODEL_SHADED_MAX;
        model->mesh = mesh;
        model->vertices = vertices;
        model->texture = &landingTexture;
        model->shade = shade;
        model->flags = 3;
      }
      // The dome swells from half its size, turning.
      {
        s32 s = (frame << 9) + FIXED_ONE / 2;

        if (s > FIXED_ONE) {
          s = FIXED_ONE;
        }
        Matrix_Initialize();
        Matrix_Translate(-toFIXED(2), toFIXED(12), 0);
        Matrix_RotateX(DEGtoANG(45));
        Matrix_RotateY(-frame * (TRIG_TURN / 64));
        Matrix_ScaleUniform(s);
        Render_TransformVertices(ECLIPSE_DOME_VERTICES, vertices,
                                 ECLIPSE_DOME_RINGS *
                                     ECLIPSE_DOME_RING_VERTICES);
        Render_DrawModelList(model);
      }
      landingTexture.logHeight = 5;
      landingTexture.logWidth = 7;
      landingTexture.pixels = work->sheet + ECLIPSE_FLAME_BAND;
      model->texture = &landingTexture;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = bandMesh;
      model->flags = shade;
      model->vertices = vertices;
      model->scrollU = shade;
      model->scrollV = shade;
      // Four rings of flame spread from the dome one after another, fading
      // in and out.
      {
        for (i = 0; i != 4; i++) {
          s32 t = frame + 24 - i * 6;

          if (t >= 0 && t < ECLIPSE_RING_FRAMES) {
            s32 s =
                gEclipse_RingGrowth[i] * t * (FIXED_ONE / 64) + toFIXED(0.25);

            shade = 0;
            if (t < ECLIPSE_RING_FADE_IN) {
              shade = t * 4 + MODEL_SHADE_INVISIBLE;
            }
            if (t > ECLIPSE_RING_FADE_OUT) {
              shade = (ECLIPSE_RING_FADE_OUT - t) * 4;
            }
            model->shade = shade;
            Matrix_Initialize();
            Matrix_Translate(-toFIXED(2), toFIXED(32), 0);
            Matrix_Scale(s / 4, s / 4, s / 4);
            Matrix_RotateX(ECLIPSE_RING_TILT);
            Matrix_RotateY((frame + i * 8) * (TRIG_TURN / 64));
            Matrix_Scale(FIXED_ONE, FIXED_ONE * 6, FIXED_ONE);
            Matrix_Translate(0, INTtoFIXED(gEclipse_RingHeights[i]), 0);
            Render_TransformVertices(ECLIPSE_BAND_VERTICES, vertices,
                                     ECLIPSE_BAND_SEGMENTS * 2);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      EFFECT_PRESENT_CANVAS(work);
    }
  }
  // A last white flash over the battle, as the damage shows.
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  gBgScroll[1].x = savedScroll;
  background->busy = 0;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background,
                           BACKGROUND_DARKENED_LEVEL);
  wait(1);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
  Graphics_LoadGreyRamp();
  Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  for (frame = 0; frame != 32; frame++) {
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Unused: does nothing; nothing calls it.
UNUSED void BattleEffect_ReservedNoOpA(void) {}

/// Unused: does nothing; nothing calls it.
UNUSED void BattleEffect_ReservedNoOpB(void) {}

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define FORM_CHANGE_GLOW_BAND 12288
/// Where BillowTexture, the columns' texture, is decoded in the map cell
/// buffer.
#define FORM_CHANGE_BILLOW (Ram_MapCellBuffer + 8192)
/// The frame the change bursts: the screen shakes, flashes burst and the
/// columns start to rise. At FORM_CHANGE_SWAP the battle swaps in the next
/// form.
#define FORM_CHANGE_BURST 48
#define FORM_CHANGE_SWAP 62
/// The flashes: FORM_CHANGE_FLASHES of them, two a frame from the burst, each
/// thrown at FORM_CHANGE_FLASH_SPEED eighths of a pixel a frame and up to as
/// much again at random, and each showing FlashBurstSheet's
/// FLASH_BURST_PICTURE_COUNT pictures for FLASH_PICTURE_FRAMES apiece. The
/// burst first frees FORM_CHANGE_FLASH_SLOTS.
#define FORM_CHANGE_FLASHES 16
#define FORM_CHANGE_FLASH_SLOTS 64
#define FORM_CHANGE_FLASH_SPEED 32
#define FLASH_PICTURE_FRAMES 3
/// The glow's largest radius: it widens a pixel a frame from 8.
#define FORM_CHANGE_GLOW_RADIUS 64
/// The level the last form's change fills the canvas with after each copy,
/// as the screen whitens.
#define FORM_CHANGE_FINALE_LEVEL 16
/// How far the columns lean.
#define FORM_CHANGE_COLUMN_TILT 1000

/// The Doom Dragon's change of form, played over the dragon once a form is
/// beaten. A glow widens over it while rays of light spin out from it, and it
/// flashes white; at FORM_CHANGE_BURST the screen shakes, flashes burst
/// upward and four columns of light rise from the ground one after another,
/// spinning. After its first and second forms (FORMATION_DOOM_DRAGON_A and B)
/// the glow stands to its right and to its left, and the battle swaps in the
/// next form at FORM_CHANGE_SWAP; after its last (FORMATION_DOOM_DRAGON_C) the
/// glow stands over it, the screen whitens, and it puts the battle's view
/// back itself.
void BattleEffect_RunFormChange(struct BattleEffectParams *args, s32 kind) {
  struct ScreenPoint center;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 index;
  s32 i;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  work->vblanksSinceTransfer = 1;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                             work->sheet + FORM_CHANGE_GLOW_BAND, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_BILLOW_TEXTURE, FORM_CHANGE_BILLOW, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;

  // The last form's change runs on while the screen whitens.
  frames = 100;
  if (kind != FORMATION_DOOM_DRAGON_C)
    frames = 76;
  for (frame = 0; frame != frames; frame++) {
    index = (frame - FORM_CHANGE_BURST) * 2;
    // The effect's centre, beside or above the dragon.
    EffectPosition_ProjectBattler(args->user, &center);
    if (kind == FORMATION_DOOM_DRAGON_A) {
      center.x += 40;
      center.y -= 24;
    } else if (kind == FORMATION_DOOM_DRAGON_B) {
      center.x -= 16;
      center.y -= 24;
    } else {
      /* FAKEMATCH(dead): the third arm steps x by nothing, as the two arms
       * above step x and y. */
      center.x += 0;
      center.y -= 48;
    }

    if (frame == 0) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      if (kind == FORMATION_DOOM_DRAGON_B)
        Iwram_CopyWordsEntry((void *)BG_PLTT,
                             Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                             SHEET_PALETTE_BYTES);
      else
        Iwram_CopyWordsEntry((void *)BG_PLTT,
                             Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_C),
                             SHEET_PALETTE_BYTES);
      // DEAD: inside frame 0's block this test is always true; the ROM keeps
      // its compare.
      if (frame == 0)
        Sound_PlayFar(SE_SPIN);
    }
    if (frame == FORM_CHANGE_BURST)
      Sound_PlayFar(SE_CLATTER);
    // The dragon roars after the burst, and shows white from frame 32 to 68.
    if (frame == 56)
      Sound_PlayFar(SE_BOSS_ROAR);
    if (frame == 32)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, -1);
    if (frame == 68)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, -1);
    if (kind == FORMATION_DOOM_DRAGON_C) {
      // After the last form the screen whitens.
      if (frame >= FORM_CHANGE_SWAP) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                            CANVAS_FILL_WORD(FORM_CHANGE_FINALE_LEVEL));
        Palette_BrightenBgEntries(2, 2, 2);
      }
    } else if (frame == FORM_CHANGE_SWAP) {
      Battle_ChangeEnemyFormFar(kind);
    }

    // The burst: shake, flash the canvas, and burst two flashes a frame from
    // a point near the centre.
    if (frame >= FORM_CHANGE_BURST) {
      struct Particle *flashes;

      if (frame == FORM_CHANGE_BURST) {
        {
          s32 *variant = &FLASHES[0].variant;

          for (i = 0; i != FORM_CHANGE_FLASH_SLOTS; i++) {
            *variant = -1;
            variant += sizeof(struct Particle) / sizeof(*variant);
          }
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      flashes = FLASHES;
      if (frame >= FORM_CHANGE_BURST &&
          frame < FORM_CHANGE_BURST + FORM_CHANGE_FLASHES / 2) {
        s32 x = INTtoFIXED((Random16() & 63) + center.x / 2 - 32);
        s32 y = INTtoFIXED((Random16() & 31) + center.y);
        s32 k = 0;
        // Each flash starts within 8 pixels of the point.
        s32 spread = 15;
        struct Particle *burst = &flashes[index];

        for (; k != 2; k++) {
          struct Particle *flash = &burst[k];
          s32 speed = FORM_CHANGE_FLASH_SPEED - 1;
          s32 angle;

          speed &= Random16();
          angle = Random16();

          flash->x = x + INTtoFIXED((Random16() & spread) - 8);
          flash->y = y + INTtoFIXED((Random16() & spread) - 8);
          flash->velocityX =
              Trig_Sin(angle) * (speed + FORM_CHANGE_FLASH_SPEED) >> 3;
          flash->velocityY =
              Trig_Cos(angle) * (speed + FORM_CHANGE_FLASH_SPEED) >> 3;
          flash->variant = 0;
        }
      }
      {
        struct Particle *flash = flashes;

        for (i = 0; i != FORM_CHANGE_FLASHES; i++, flash++) {
          if (flash->variant >= 0 &&
              flash->variant <
                  FLASH_BURST_PICTURE_COUNT * FLASH_PICTURE_FRAMES) {
            draw[0](canvas,
                    work->sheet + flash->variant / FLASH_PICTURE_FRAMES *
                                      PICTURE_BYTES(FLASH_BURST),
                    FIXED_INTEGER_PART(flash->x) - 16,
                    FIXED_INTEGER_PART(flash->y) - 48, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
            Particle_AdvanceWithGravity2D(flash, 62, -toFIXED(1) / 32);
            flash->variant++;
          }
        }
      }
    }

    if (frame >= 0) {
      FIXED cx = INTtoFIXED(center.x / 2);
      FIXED cy = INTtoFIXED(center.y);
      struct RenderModel *model;
      void *corners;
      s32 bias;

      // The glow widens until it reaches FORM_CHANGE_GLOW_RADIUS.
      if (frame <= 63) {
        s32 size = FORM_CHANGE_GLOW_RADIUS * 2;
        s32 radius = frame + 8;

        if (radius <= FORM_CHANGE_GLOW_RADIUS) {
          size = radius * 2;
          BattleEffect_DrawShadedCircles(Ram_MapCellBuffer, size, FIXED_ONE);
        }
        Canvas_BlitAtFoot(Ram_MapCellBuffer, FIXEDtoINT(cx), FIXEDtoINT(cy),
                          size);
      }

      // Each ray has an angle (x), a spin (y) and an age (variant), each
      // starting 2 frames after the last.
      if (frame == 0) {
        for (i = 0; i != LIGHT_RAYS; i++) {
          struct Particle *ray = &work->particles[i];

          ray->x = Random16() & TRIG_ANGLE_MASK;
          ray->y = (Random16() & 255) + 128;
          if (Random16() & 1)
            ray->y = -ray->y;
          ray->variant = -4 * i;
        }
      }

      corners = Heap_AllocEwram(512);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      bias = 0;
      model->shade = bias;
      texture.logHeight = 2;
      texture.pixels = work->sheet + FORM_CHANGE_GLOW_BAND;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMesh;
      model->vertices = corners;

      // The rays spin twice as fast as Eclipse's and widen and narrow in 24
      // frames.
      for (i = 0; i != LIGHT_RAYS; i++) {
        struct Particle *ray = &work->particles[i];

        ray->variant += 2;
        ray->x += ray->y * 2;
        if (ray->variant >= 0 && ray->variant < LIGHT_RAY_FRAMES) {
          Matrix_Initialize();
          {
            s32 column = i & 3;

            Matrix_Translate(cx + INTtoFIXED(column * 2 + 2) - toFIXED(64),
                             cy - INTtoFIXED(column + 2) - toFIXED(64), 0);
          }
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(ray->x);
          Matrix_Scale(
              Trig_Sin(ray->variant * (TRIG_TURN / 2 / LIGHT_RAY_FRAMES)) +
                  FIXED_ONE,
              FIXED_ONE * 2, FIXED_ONE);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, corners,
                                   4);
          Render_DrawModelList(model);
        }
      }

      // The four columns rise from the ground after the burst.
      model->texture = &texture;
      model->vertices = corners;
      texture.logWidth = 6;
      texture.pixels = FORM_CHANGE_BILLOW;
      model->kind = MODEL_SHADED_ADDITIVE;
      texture.logHeight = 6;
      model->mesh = gBattleEffect_RingMesh;
      for (i = 0; i != 4; i++) {
        s32 start = gFormChange_WaveStarts[i] + FORM_CHANGE_BURST;

        model->scrollV = (frame * 8 + i * 32) & TEXTURE_SCROLL_MASK;
        if (frame > start) {
          bias = (start - frame) * 8 + 56;
          if (bias > BAND_SHADE_BRIGHTEST)
            bias = BAND_SHADE_BRIGHTEST;
          if (bias > MODEL_SHADE_INVISIBLE) {
            s32 growth = (frame - start) * gFormChange_WaveGrowth[i] * 2000 +
                         FIXED_ONE / 4;

            model->shade = bias;
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(center.x / 2 - 60), toFIXED(32), 0);
            Matrix_Scale(growth * 2,
                         gFormChange_WaveStretch[i] * growth + FIXED_ONE * 4,
                         growth / 4);
            Matrix_RotateX(FORM_CHANGE_COLUMN_TILT);
            if (i & 1)
              Matrix_RotateY((i * 8 + frame) * (TRIG_TURN / 64));
            else
              Matrix_RotateY((i * 8 - frame) * (TRIG_TURN / 64));
            Render_TransformVertices(gBattleEffect_RingCorners, corners, 32);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (kind == FORMATION_DOOM_DRAGON_C) {
    // Put the battle's view back by hand.
    Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
    Task_Remove(Palette_StepFadeTransfer);
  } else {
    BattleEffect_EndCanvasLayer();
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Copies the canvas to VRAM once the effect has finished a frame, then clears,
/// fades, darkens or brightens it as transferMode says.
void BattleEffect_ProcessPendingGraphicsTransfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *source = Ram_HeapSlots->battleCanvas;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      Iwram_FillWords(source, CANVAS_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == CANVAS_FADE_HALF)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(1),
                                       CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(1),
                         CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_BRIGHTEN:
      ColorBuffer_Brighten(source, work->transferValue,
                           (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; the ROM keeps its load.
    (void)work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// Copies the stamp's picture to its sprite's tiles once the effect has drawn
/// it, then clears the picture.
void BattleEffect_ProcessPendingStampTransfer(void) {
  u8 *work = Ram_HeapSlots->battleEffect;
  u32 ime;
  s32 *pending;

  IRQ_LOCK(ime);
  pending = &((struct BattleEffectWork *)work)->stampPending;
  if (*pending == 1) {
    *pending = 0;
    IRQ_UNLOCK(ime);
    Iwram_CopyWords((void *)STAMP_TILES(work), STAMP_CANVAS(work),
                    OBJ_64X64_8BPP_BYTES);
    Iwram_FillWords(STAMP_CANVAS(work), OBJ_64X64_8BPP_BYTES, 0);
  } else {
    IRQ_UNLOCK(ime);
  }
}

/// Copies a picture the size of the battle background from the map cell buffer
/// to BG character block 2 once the effect has finished a frame, as
/// transferMode says.
void BattleEffect_FlushPendingGraphicsTransfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *source = Ram_MapCellBuffer;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      Iwram_FillWords(source, BATTLE_BACKGROUND_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == CANVAS_FADE_HALF)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(2),
                          BATTLE_BACKGROUND_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                       BATTLE_BACKGROUND_BYTES);
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(2),
                         BATTLE_BACKGROUND_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; the ROM keeps its load.
    (void)work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// The same for a picture drawn in the canvas's heap slot: a plain copy takes
/// all of BATTLE_BACKGROUND_BYTES, while the clear, the fade and the darkening
/// take only its first CANVAS_VIEW_BYTES.
void BattleEffect_ProcessPendingBlock2Transfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *source;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    source = Ram_HeapSlots->battleCanvas;
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source,
                      BATTLE_BACKGROUND_BYTES);
      Iwram_FillWords(source, CANVAS_VIEW_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == CANVAS_FADE_HALF)
        ColorBuffer_Halve(source, (void *)BG_CHAR_BLOCK(2), CANVAS_VIEW_BYTES);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                       CANVAS_VIEW_BYTES);
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_Darken(source, work->transferValue, (void *)BG_CHAR_BLOCK(2),
                         CANVAS_VIEW_BYTES);
      break;
    }
    // DEAD: nothing uses the counter read here; the ROM keeps its load.
    (void)work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// Row `row` of BG1's map.
#define BG1_MAP_ROW(row)                                                       \
  (BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK) + (row) * MAP_ROW_BYTES)

/// Writes BG1's map to show a picture CANVAS_SIZE wide from BG character block
/// 2, as BattleTransition_BuildTilemap does for the battle background: four
/// rows of blank entries, two of BG1_CLEAR_TILE, then the picture's tiles in
/// order, CANVAS_TILES rows of them, each row closed with BG1_CLEAR_TILE.
void BattleEffect_BuildCanvasTilemap(void) {
  u32 *map = (u32 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK);
  u32 tiles;
  s32 row;
  s32 column;

  Iwram_FillWords((void *)map, 4 * MAP_ROW_BYTES, -1);
  map += 4 * MAP_ROW_WORDS;
  Iwram_FillWords((void *)map, 2 * MAP_ROW_BYTES,
                  MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE));
  map += 2 * MAP_ROW_WORDS;
  tiles = MAP_ENTRY_PAIR(BG1_BLOCK2_FIRST_TILE, BG1_BLOCK2_FIRST_TILE + 1);
  for (row = 0; row != CANVAS_TILES; row++) {
    for (column = 0; column != 8; column++) {
      *map++ = tiles;
      tiles += MAP_ENTRY_PAIR(2, 2);
    }
    for (column = 0; column != 8; column++)
      *map++ = MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE);
  }
}

/// Writes the window, display and blend registers the effect keeps in its
/// work, all at once.
void BattleEffect_ApplyWindowBlendRegisters(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  REG_WIN0H = work->win0h;
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

/// Writes the effect's BG2 scale to BG2PA and BG2PD and its reference point to
/// BG2X and BG2Y.
void Display_ApplyBg2Affine(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  REG_BG2PA = work->bg2pa;
  REG_BG2PD = work->bg2pd;
  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Writes the effect's BG2 reference point to BG2X and BG2Y.
void Display_ApplyBg2Reference(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Darkens the battle background a step each frame while the canvas is open, so
/// the effect stands out, for as many frames as fadeFrames says.
void Palette_StepFadeTransfer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  s32 *framesLeft;
  struct BattleWork *battle;

  framesLeft = &work->fadeFrames;
  battle = Ram_HeapSlots->battleWork;

  if (*framesLeft > 0) {
    s32 *step = &work->fadeStep;

    (*step)++;
    ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                         FIXED_ONE - *step * BACKGROUND_LEVEL_STEP,
                         BATTLE_BACKGROUND_COLORS);
    (*framesLeft)--;
  }
}

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  Iwram_ClearWords(work->actorTimers, sizeof work->actorTimers);
}

/// Counts down each target's flash timer and gives it its own palette back when
/// one runs out.
void BattleEffect_CountDownBattlerTints(void) {
  struct BattleEffectWork *work;
  s32 actorIndex;

  work = Ram_HeapSlots->battleEffect;
  for (actorIndex = 0; actorIndex != 8; actorIndex += 1) {
    if (work->actorTimers[actorIndex] != 0) {
      if ((work->actorTimers[actorIndex] = work->actorTimers[actorIndex] - 1) ==
          0) {
        BattleEffect_TintBattler(work->args->targets[actorIndex],
                                 SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
      }
    }
  }
}

/// Shows the canvas on BG2 stretched to twice its width across the battle
/// view, blended over the battle, with windows 0 and 1 over the battle view.
void BattleEffect_SetupCanvasWindows(void) {
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  REG_BG2X = 0;
  REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = Q_8_8(1);
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
}

/// Opens the canvas layer over the battle, darkening the battle background
/// behind it.
void BattleEffect_StartCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleBackgroundWork *display = Ram_HeapSlots->battleBackground;
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);

  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_FRAME);

  // Change BG2's settings while it is hidden.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);

  BattleEffect_SetupCanvasWindows();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);

  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  work->shakeFrames = 0;
  work->transferPending = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  wait(1);
}

/// Unused: opens the canvas layer as BattleEffect_StartCanvasLayer does, with
/// the windows off and the battle background's saved colours put back before
/// it darkens; nothing calls it.
UNUSED void BattleEffect_OpenCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleBackgroundWork *display = Ram_HeapSlots->battleBackground;
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background,
                              BATTLE_BACKGROUND_FADE_RESTORE);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_FRAME);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);

  BattleEffect_SetupCanvasWindows();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  wait(1);

  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  work->shakeFrames = 0;
  work->transferPending = 0;
  wait(1);
}

/// Opens the canvas layer with the canvas repeated across a wrapping map.
void BattleEffect_StartTiledCanvas(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_StartCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_SIZE(1);
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
    }
  }
}

/// Two entries of BG2's affine map, a byte each, both showing canvas tile
/// `tile`.
#define AFFINE_TILE_PAIR(tile) ((tile) << 8 | (tile))

/// Opens the canvas layer as BattleEffect_StartTiledCanvas does, with the
/// canvas's top right tile in the rest of each row in place of its top left.
void BattleEffect_StartTiledCanvasFilled(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_StartCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_SIZE(1);
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) =
          AFFINE_TILE_PAIR(CANVAS_TILES - 1);
    }
  }
}

/// The levels the battle background brightens by each frame as the canvas
/// closes, back to its own colours.
#define BACKGROUND_RESTORE_STEP 3

/// Closes the canvas layer and brings the battle background's colours back.
void BattleEffect_EndCanvasLayer(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Task_Remove(Palette_StepFadeTransfer);
  // This overwrites the saved y restored above, so BG1 is always left at
  // BATTLE_BACKGROUND_SCROLL_Y.
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  REG_BLDCNT = 0;
  wait(1);

  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 7);
  wait(1);
  // The background brightens from the darkened level back to its own colours
  // over eight frames.
  for (i = 0; i != BACKGROUND_DARKENED_LEVEL / BACKGROUND_RESTORE_STEP; i++) {
    BattleBackground_SetPaletteLevelFar(battle->background,
                                        BACKGROUND_DARKENED_LEVEL -
                                            BACKGROUND_RESTORE_STEP -
                                            i * BACKGROUND_RESTORE_STEP);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
}

/// Hides BG1 and the windows, starts bringing the battle background's colours
/// back, and gives BG1 back its own map.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &(Ram_HeapSlots->battleBackground)->secondMode;
  battle = Ram_HeapSlots->battleWork;
  *flag = BATTLE_BACKGROUND_FIXED;
  IoWriteQueue_Store16((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG2_ON |
                                              DISPCNT_OBJ_ON);
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
                          BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK),
                      IO_WRITE_HALFWORD);
  wait(1);
}

/// The tile, cleared, that BG1 shows around the canvas.
#define CANVAS_BG1_BLANK_TILE 255
/// The canvas's first tile counted from BG1's character base: the canvas is
/// character block 1.
#define CANVAS_BG1_FIRST_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)
/// The map row where the canvas starts on BG1, 96 pixels down: the effects that
/// show it there scroll BG1 to y 80 to show it from the battle view's top line.
#define CANVAS_BG1_TOP_ROW 12

/// Shows the canvas on BG1 instead of BG2, at its own size.
void BattleEffect_SetupCanvasTileMap(void) {
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundWork *layer = Ram_HeapSlots->battleBackground;
  s32 row;
  s32 column;
  s32 byteOffset;

  BattleEffect_ClearActorTimers();
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, screen->background, 0);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  layer->secondMode = BATTLE_BACKGROUND_NONE;
  // A blank tile, then the rows above the canvas, from the first one BG1
  // shows, which show nothing.
  Iwram_ClearWords(BG_CHAR_BLOCK(0) + CANVAS_BG1_BLANK_TILE * TILE_SIZE_8BPP,
                   TILE_SIZE_8BPP);
  Iwram_FillWords(BG1_MAP_ROW(BATTLE_BACKGROUND_SCROLL_Y / 8),
                  (CANVAS_BG1_TOP_ROW - BATTLE_BACKGROUND_SCROLL_Y / 8) *
                      MAP_ROW_BYTES,
                  -1);
  // The canvas's tiles, then the blank tile across the rest of each row.
  byteOffset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (column = 0; column != BG_SCREEN_WIDTH; column++, byteOffset += 2) {
      if (column >= CANVAS_TILES)
        *(volatile u16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) + byteOffset) =
            CANVAS_BG1_BLANK_TILE;
      else
        *(volatile u16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) + byteOffset) =
            row * CANVAS_TILES + CANVAS_BG1_FIRST_TILE + column;
    }
  }
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  wait(1);
}

/// A line follows its slower axis in fixed point with LINE_FRACTION_BITS of
/// fraction, kept at the top of a word: a pixel's worth carries into the sign
/// bit, LINE_FRACTION_SHIFT up.
#define LINE_FRACTION_BITS 8
#define LINE_PIXEL (1 << LINE_FRACTION_BITS)
#define LINE_FRACTION_SHIFT (31 - LINE_FRACTION_BITS)

/// Draws a line on the canvas, keeping the brighter colour where it crosses
/// what is drawn.
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = LINE_PIXEL / 2 << LINE_FRACTION_SHIFT;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  CLAMP(y0, 0, CANVAS_SIZE - 1);
  CLAMP(y1, 0, CANVAS_SIZE - 1);

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << LINE_FRACTION_BITS) / ABS(y1 - y0);
    step <<= LINE_FRACTION_SHIFT;
    x = x0;
    // dx now steps x a pixel toward the line's end.
    if (dx < 0)
      dx = -1;
    else
      dx = 1;
    for (i = y0; i != y1; i++) {
      offset = CANVAS_PIXEL_OFFSET((u32)x, (u32)i);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        x += dx;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  } else {
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << LINE_FRACTION_BITS) / ABS(x1 - x0);
    step <<= LINE_FRACTION_SHIFT;
    y = y0;
    if (dy < 0)
      dy = -1;
    else
      dy = 1;
    for (i = x0; i != x1; i++) {
      offset = CANVAS_PIXEL_OFFSET((u32)i, (u32)y);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        y += dy;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  }
}

#undef FLASHES
#undef GLOW_BAND_BYTES
#undef LIGHT_RAYS
#undef LIGHT_RAY_FRAMES
#undef BAND_SHADE_BRIGHTEST
#undef FLASH_PICTURE_FRAMES
#undef BG1_MAP_ROW
#undef AFFINE_TILE_PAIR
#undef BACKGROUND_RESTORE_STEP
#undef CANVAS_BG1_BLANK_TILE
#undef CANVAS_BG1_FIRST_TILE
#undef CANVAS_BG1_TOP_ROW
#undef LINE_FRACTION_BITS
#undef LINE_PIXEL
#undef LINE_FRACTION_SHIFT
