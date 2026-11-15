/// @file
/// The Moloch and Iris summons, and the model renderers and projections the
/// effects draw with.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct RenderModel;

/// Moloch's frames, and the frame A or B skips to.
#define MOLOCH_FRAMES 296
#define MOLOCH_SKIP_TO 232
/// The motes, in the map cell buffer; the flame columns, four pixels apart
/// from x = 44 (sixteen are set up, the first twelve drawn); the embers, from
/// work->particles[24]; and the summoned sprites.
#define MOLOCH_MOTES 256
#define MOLOCH_FLAMES 12
#define MOLOCH_FLAME_LEFT 44
#define MOLOCH_FIRST_EMBER 24
#define MOLOCH_EMBERS 16
#define MOLOCH_SPRITES 12
/// A flame or ember lives four frames on each of FlashBurstSheet's six
/// pictures; the ring texture follows them.
#define MOLOCH_FLAME_LIFE (6 * 4)
#define MOLOCH_RING_TEXTURE (6 * PICTURE_BYTES(FLASH_BURST))

/// Moloch. A shower of 256 motes drifts down the screen and, from frame
/// 91, gathers toward the middle; from frame 97 flames rise in twelve columns
/// and from frame 136 four textured rings spread out from the summoned
/// objects. At frames 212 to 268 the targets are struck in turn, and the
/// scene closes with a glow of the whole palette. A or B skips to frame 232.
///
/// The work's particles 0-11 are the flames (velocityZ picks the blitter, 2
/// retires the column), particles 24-39 the embers of the closing glow; the
/// map cell buffer holds the motes (variant counts down the frames before a
/// mote appears, 0 is a live mote).
void BattleEffect_RunMoloch(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  struct ModelTexture texture;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;
  struct Particle *mote;
  struct Particle *flame;
  struct Particle *motes;
  struct RenderModel *model;
  void *projected;
  s32 i;
  s32 k;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  graphics = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameBandTexture,
                             work->sheet + MOLOCH_RING_TEXTURE, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Graphics_LoadGreyRamp();
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  {
    struct Point2D focus;
    s32 frame;

    focus.x = toFIXED(128);
    focus.y = toFIXED(64);
    BattleEffect_SelectLivingTargets(work->args);
    wait(1);
    BattleEffect_SpawnObjects(MOLOCH_SPRITES, SPRITE_MOLOCH, 2);

    frame = 0;
    do {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame <= 231 && frame > 15) {
        Sound_PlayFar(SE_CLATTER);
        frame = MOLOCH_SKIP_TO;
      }

      if (frame == 0) {
        EFFECT_FREE_PARTICLES(i, work->particles, 64);

        // The motes start along the top, each after its own delay.
        motes = gMapGridCellBuffer;
        for (i = 0; i != MOLOCH_MOTES; i++) {
          mote = &motes[i];
          Random16();
          mote->variant = i;
          mote->x = INTtoFIXED((Random16() & 255) - 64);
          mote->y = 0;
          mote->velocityX = 0;
          mote->velocityY = 0;
        }

        // The flame columns rise from y = 72.
        for (i = 0; i != 16; i++) {
          flame = &work->particles[i];
          flame->x = INTtoFIXED(i * 3 + i + MOLOCH_FLAME_LEFT);
          flame->y = toFIXED(72);
          flame->velocityX = 0;
          flame->velocityY = INTtoFIXED(-((Random16() & 3) + 2));
          flame->velocityZ = 0;
          flame->variant = -i - (Random16() & 15);
        }

        // The embers of the closing glow start at (54, 56).
        for (i = 0; i != MOLOCH_EMBERS; i++) {
          flame = &work->particles[MOLOCH_FIRST_EMBER + i];
          flame->x = toFIXED(54);
          flame->y = toFIXED(56);
          flame->variant = i * -2;
          flame->velocityX = -(INTtoFIXED(Random16() & 3) + toFIXED(2));
          flame->velocityY = -(Random16() & 3) << 15;
        }
      }

      if (frame <= 271) {
        motes = gMapGridCellBuffer;
        for (i = 0; i != MOLOCH_MOTES; i++) {
          mote = &motes[i];
          if (mote->variant == 0) {
            s32 size = gMoloch_MoteSizes[i & 7];

            draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                    FIXED_INTEGER_PART(mote->x) - (size >> 1),
                    FIXED_INTEGER_PART(mote->y) - size, size, size * 2);

            // Frames 91-119: motes still at rest head for a random point of
            // the middle, timed to arrive as they reach the ground.
            if ((u32)(frame - 91) <= 28 && mote->velocityX == 0 &&
                FIXED_INTEGER_PART(mote->y) <= 113) {
              s32 dx;

              mote->velocityX = INTtoFIXED(Random16() & 63) + toFIXED(56);
              dx = mote->velocityX - mote->x;
              mote->velocityX = dx / ((toFIXED(114) - mote->y) / (size << 14));
            }

            // Frames 142-161: they swirl about the centre line.
            if ((u32)(frame - 142) <= 19) {
              FIXED distance = mote->x - toFIXED(96);
              FIXED pull;

              if (distance < 0)
                distance = -distance;
              pull = (mote->y / 2 - distance +
                      (s32)INTtoFIXED(Random16() & 31) + toFIXED(96)) /
                     32;
              if (mote->x < toFIXED(96) && pull > 0)
                pull = -pull;
              if (mote->x > toFIXED(96) && pull < 0)
                pull = -pull;
              mote->velocityX += pull / 8;
            }

            mote->x += mote->velocityX + Trig_Sin(mote->y >> 5) / 5;
            mote->y += mote->velocityY + (size << 14);
            if (mote->y > toFIXED(114)) {
              if (frame > 215) {
                // Late motes fly back in from the right.
                mote->variant = 0;
                mote->x = toFIXED(120);
                mote->y = INTtoFIXED((Random16() & 127) - 32);
                mote->velocityX = INTtoFIXED((Random16() & 3) - 8);
                mote->velocityY = 0;
              } else {
                mote->variant = Random16() & 255;
                mote->x = INTtoFIXED((Random16() & 255) - 64);
                mote->y = -toFIXED(8);
                mote->velocityX = 0;
                mote->velocityY = 0;
              }
            }
          } else {
            mote->variant--;
          }

          // Frames 217-245: the wind turns.
          if ((u32)(frame - 217) <= 28) {
            mote->velocityX -= toFIXED(1) / 8;
            mote->velocityY += toFIXED(1) / 8;
          }
        }
      }

      // Frames 91-255: the flame columns, a 32x64 cell for every four frames.
      if ((u32)(frame - 91) <= 164) {
        for (i = 0; i != MOLOCH_FLAMES; i++) {
          flame = &work->particles[i];
          if (flame->velocityZ != 2) {
            if ((u32)flame->variant < MOLOCH_FLAME_LIFE) {
              draw[flame->velocityZ](
                  canvas,
                  work->sheet +
                      (flame->variant >> 2) * PICTURE_BYTES(FLASH_BURST),
                  FIXED_INTEGER_PART(flame->x), FIXED_INTEGER_PART(flame->y),
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
              flame->x += flame->velocityX;
              flame->y += flame->velocityY;
            }
            if (++flame->variant == MOLOCH_FLAME_LIFE) {
              flame->x = INTtoFIXED(i * 3 + i + MOLOCH_FLAME_LEFT);
              flame->y = toFIXED(72);
              flame->variant = 0;
              flame->velocityY = INTtoFIXED(-((Random16() & 3) + 2));
              flame->velocityZ = 0;
              if ((u32)(frame - 97) <= 42)
                flame->velocityZ = 1;
              else if (frame > 140)
                flame->velocityZ = 2;
            }
            if (frame == 136) {
              if (flame->x < toFIXED(80))
                flame->velocityX -= toFIXED(4);
              else
                flame->velocityX += toFIXED(4);
            }
          }
        }
      }

      // Frames 201-271: the embers of the closing glow.
      if ((u32)(frame - 200) <= 71) {
        if (frame > 200) {
          for (i = 0; i != MOLOCH_EMBERS; i++) {
            flame = &work->particles[MOLOCH_FIRST_EMBER + i];
            if ((u32)flame->variant < MOLOCH_FLAME_LIFE) {
              draw[0](canvas,
                      work->sheet +
                          (flame->variant >> 2) * PICTURE_BYTES(FLASH_BURST),
                      FIXED_INTEGER_PART(flame->x),
                      FIXED_INTEGER_PART(flame->y), FLASH_BURST_WIDTH,
                      FLASH_BURST_HEIGHT);
              flame->x += flame->velocityX;
              flame->y += flame->velocityY;
            }
            if (++flame->variant == MOLOCH_FLAME_LIFE) {
              flame->x = toFIXED(54);
              flame->y = toFIXED(56);
              flame->velocityX = -(INTtoFIXED(Random16() & 3) + toFIXED(2));
              flame->variant = 0;
            }
          }
        }
      }

      // From frame 136: four rings, each starting at its own frame, grow and
      // fade out over eight frames.
      if (frame > 135) {
        projected = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        texture.logWidth = 7;
        texture.logHeight = 5;
        texture.pixels = work->sheet + MOLOCH_RING_TEXTURE;
        model->mesh = gBattleEffect_CylinderMesh;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &texture;
        model->vertices = projected;
        model->scrollU = 0;
        model->scrollV = 0;
        for (i = 0; i != ARRAY_COUNT(gMoloch_RingStarts); i++) {
          s32 start = gMoloch_RingStarts[i] + 136;
          FIXED scale;
          s32 shade;

          if (frame <= start)
            continue;
          scale =
              (frame - start) * gMoloch_RingGrowth[i] * 2000 + FIXED_ONE / 2;
          shade = (start - frame) * 8 + 56;
          if (shade > 0)
            shade = 0;
          if (shade <= MODEL_SHADE_INVISIBLE)
            continue;
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Translate(toFIXED(16), INTtoFIXED(gMoloch_RingHeights[i]), 0);
          Matrix_Scale(scale, scale, scale);
          Matrix_RotateX(0xbb8);
          model->scrollU = ((frame + i * 5) & 7) << 4;
          Render_TransformVertices(gBattleEffect_CylinderCorners, projected,
                                   32);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(projected);
      }

      if (frame > 139)
        BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_MOLOCH, focus.x,
                                           focus.y);

      // Frames 212-268, every eight: the targets flash and are thrown back.
      for (i = 0; i != 8; i++) {
        if (frame == i * 8 + 212) {
          for (k = 0; k != work->args->targetCount; k++) {
            BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, k, 8);
            BattleEffect_StartActorMotion(args->targets[k], 1, toFIXED(1),
                                          toFIXED(4), toFIXED(1), i * 16 + 100);
          }
        }
      }
      if ((u32)(frame - 201) <= 54)
        work->shakeFrames = 2;

      if (frame == 32)
        Sound_PlayFar(SE_GLITTER);
      if (frame == 90)
        Sound_PlayFar(SE_225);
      if (frame == 140)
        Sound_PlayFar(SE_CAST);
      if (frame == 200)
        Sound_PlayFar(SE_BANE_ESCAPE);
      if (frame == 232)
        Sound_PlayFar(SE_CLATTER);

      // Frames 256-271 brighten the whole screen to white; at 272 the battle
      // phase resumes and the canvas fades back.
      if (frame == 272) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        Graphics_LoadGreyRamp();
      } else if ((u32)(frame - 256) <= 15) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
        if (frame > 269)
          work->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
        Palette_BrightenBgEntries(2, 2, 2);
      }

      BattleCamera_Shake(4, 8);
      BattleEffect_CountDownBattlerTints();
      frame++;
      EFFECT_PRESENT_CANVAS(work);
    } while (frame != MOLOCH_FRAMES);

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_MOLOCH, focus.x, focus.y);
    for (i = 0; i != MOLOCH_SPRITES; i++)
      Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

/// Iris's backdrop change: clears the targets' ground motion and drops
/// each a little above the ground at a random height, empties the actor
/// list, loads the violet sky and recolours its 128 BG palette entries from
/// 96 on: each channel is darkened and the three are rotated (red to green,
/// green to blue, blue to red).
void Iris_ChangeBackdrop(void) {
  struct BattleBackgroundWork *transition = Ram_HeapSlots->battleBackground;
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  s16 list[UNIT_MAX];
  u16 *palette;
  s32 i;

  REG_BG1HOFS = 0;
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;

  // The targets fall in from just above the ground, halfway closer in depth.
  for (i = 0; i != work->args->targetCount; i++) {
    struct MotionObject *object =
        BattleObject_GetFar(work->args->targets[i])->object;

    object->x = 0;
    object->y = INTtoFIXED(16 - (Random16() & 15));
    object->z /= 2;
    object->verticalMotionStrength = 0;
  }

  // No actor objects over the new backdrop.
  list[0] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(list, 0);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_VioletSkyBackdrop, 0);

  palette = (u16 *)&BG_PLTT_COLOR(6, 0);
  for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
    s32 color = *palette;
    s32 blue = RGB_BLUE((u16)color);
    s32 green = RGB_GREEN((u16)color);
    s32 red = RGB_LEVEL_MAX;

    red &= color;
    red -= 16;
    green -= 24;
    blue -= 20;

    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *palette++ = RGB(blue, red, green);
  }
  transition->active = 1;
  gBgScroll[1].x = 0;
}

/// Iris's sheet, work->sheet: the mote set and its fading copies, then the
/// drops' pictures (eight of each size, each a shaded circle mirrored and
/// dimmer than the last), later overwritten by the strips' and the glow's
/// 32x64 textures.
#define IRIS_MOTE_FADES 8
#define IRIS_DROPS 0x1b12
#define IRIS_DROP_WIDTH 20
#define IRIS_DROP_HEIGHT 40
#define IRIS_SMALL_DROPS 0x4c12
#define IRIS_SMALL_DROP_WIDTH 12
#define IRIS_SMALL_DROP_HEIGHT 24
#define IRIS_DROP_PICTURES 8
#define IRIS_TEXTURES (IRIS_MOTE_FADES * MOTE_SET_BYTES)
#define IRIS_TEXTURE_WIDTH 32
#define IRIS_TEXTURE_HEIGHT 64
#define IRIS_TEXTURE_BYTES PICTURE_BYTES(IRIS_TEXTURE)
/// The pillars' 16x16 texture follows the first mote set; its shaded circle
/// is drawn first at IRIS_PILLAR_CIRCLE and mirrored into it.
#define IRIS_PILLAR_TEXTURE MOTE_SET_BYTES
#define IRIS_PILLAR_TEXTURE_SIZE 16
#define IRIS_PILLAR_CIRCLE 0x3e8

/// The splashes' pictures, in the particle sheet (Ram_HeapSlots->battleSheet):
/// three fades each of a large and a small ring mote, eight fades of a dot and
/// the spray's column.
#define IRIS_LARGE_RING_WIDTH 7
#define IRIS_LARGE_RING_HEIGHT 14
#define IRIS_SMALL_RING_WIDTH 5
#define IRIS_SMALL_RING_HEIGHT 10
#define IRIS_DOT_WIDTH 2
#define IRIS_DOT_HEIGHT 4
#define IRIS_SPRAY_WIDTH 8
#define IRIS_SPRAY_HEIGHT 32
enum IrisSplashPicture {
  IRIS_LARGE_RINGS = 0,
  IRIS_SMALL_RINGS = IRIS_LARGE_RINGS + 3 * PICTURE_BYTES(IRIS_LARGE_RING),
  IRIS_DOTS = IRIS_SMALL_RINGS + 3 * PICTURE_BYTES(IRIS_SMALL_RING),
  IRIS_SPRAY = IRIS_DOTS + 8 * PICTURE_BYTES(IRIS_DOT)
};

/// Iris: draws one falling drop at (x, y), large (20x40) when small is 0
/// and small (12x24) otherwise, its animation picture chosen by frame / 2, and
/// under it a trail of particles every half size down to row 112.
void Iris_DrawDrop(u32 frame, s32 x, s32 y, s32 small) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  u32 size;
  u32 half;
  s32 i;
  s32 trailY;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

  // The drop: one picture of its eight, large or small.
  if (small == 0)
    draw[1](canvas,
            work->sheet + IRIS_DROPS + frame / 2 * PICTURE_BYTES(IRIS_DROP),
            x - IRIS_DROP_WIDTH / 2, y - IRIS_DROP_HEIGHT / 2, IRIS_DROP_WIDTH,
            IRIS_DROP_HEIGHT);
  else
    draw[1](canvas,
            work->sheet + IRIS_SMALL_DROPS +
                frame / 2 * PICTURE_BYTES(IRIS_SMALL_DROP),
            x - IRIS_SMALL_DROP_WIDTH / 2, y - IRIS_SMALL_DROP_HEIGHT / 2,
            IRIS_SMALL_DROP_WIDTH, IRIS_SMALL_DROP_HEIGHT);

  // The trail: particles of size 8 (6 for a small drop), half a size apart.
  size = 8 - small * 2;
  half = size / 2;
  i = 1;
  trailY = y + i * half;
  if (trailY > 112)
    return;
next:
  draw[0](canvas,
          work->sheet + (gBattleEffect_MotePictures[size - 1] +
                         frame / 2 * MOTE_SET_BYTES),
          x - size / 2, trailY, size, size * 2);
  i++;
  trailY = y + i * half;
  if (trailY <= 112)
    goto next;
}

extern const struct ModelVertex gIris_PillarVertices[6];

/// Iris: draws one rising pillar, a textured strip of six vertices whose
/// two lower rows come from gIris_PillarVertices and whose top row is raised to
/// height. x is the pillar's screen column (63 is the centre); the base sits
/// height pixels below row 63, so the pillar grows upward from there.
void Iris_DrawPillar(s32 halfWidth, s32 x, s32 height) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *projected;
  struct RenderModel *model;
  struct ModelVertex vertices[6];
  struct ModelTexture texture;
  struct Vector3 position;

  projected = Heap_AllocEwram(48);
  model = Render_AllocateModelList(1);
  memcpy(vertices, gIris_PillarVertices, sizeof(vertices));

  texture.logWidth = 4;
  texture.logHeight = 4;
  texture.pixels = work->sheet + IRIS_PILLAR_TEXTURE;

  // Each row is halfWidth to either side; the top row reaches height.
  vertices[0].x = -halfWidth;
  vertices[1].x = halfWidth;
  vertices[2].x = -halfWidth;
  vertices[3].x = halfWidth;
  vertices[4].x = -halfWidth;
  vertices[5].x = halfWidth;
  vertices[5].y = height;
  vertices[4].y = height;

  Matrix_Initialize();
  SET_POSITION(&position, INTtoFIXED(x) - toFIXED(63), INTtoFIXED(63 - height),
               0);
  Matrix_TranslateVector(&position);

  model->kind = MODEL_TEXTURED_MAX;
  model->texture = &texture;
  model->mesh = gIris_PillarMesh;
  model->vertices = projected;
  Render_TransformVertices(vertices, projected, 6);
  Render_DrawModelList(model);
  Heap_Release(model);
  Heap_Release(projected);
}

/// Draws a 45-degree canvas line from (x + w/2, y - w/2) down-left toward
/// (x, y), at most 48 pixels long, in the given colour.
void BattleEffect_DrawDiagonalCanvasLine(s32 x, s32 y, s32 w, u8 color) {
  s32 h = w >> 1;
  s32 x1;
  s32 y1;

  x += h;
  y -= h;
  if (h > 48)
    h = 48;
  y1 = y + h;
  x1 = x - h;
  BattleEffect_DrawClippedCanvasLine(x, y, x1, y1, color);
}

struct StripQuad;

#define HALF(p, n) (((s16 *)(p))[n])

/// Iris's runs: the pink petals (PinkPetalsTiles, fifteen pictures of a petal
/// tumbling), the motes in work->particles, and the opening's streaks, each a
/// head and the points that trail it.
#define IRIS_PETALS 36
#define IRIS_PETAL_FRAMES 15
#define IRIS_MOTES 32
#define IRIS_STREAKS 15
#define IRIS_STREAK_LENGTH 10
/// Iris herself, RobedGoddessTiles as a 4 x 4 grid of 32x32 sprites.
#define IRIS_GODDESS_COLUMNS 4
#define IRIS_GODDESS_ROWS 4
#define IRIS_GODDESS_SPRITES (IRIS_GODDESS_COLUMNS * IRIS_GODDESS_ROWS)
#define IRIS_GODDESS_TILE 32
/// The 128 background colours the scene fades (entries 96 to 223).
#define IRIS_FADE_COLORS 128

/// The petal objects' work, in the map cell buffer at 0x1f40.
struct IrisPetals {
  s16 life[IRIS_PETALS]; // high byte: frames left; 0 frees the slot
  u16 tiles[IRIS_PETAL_FRAMES];
  u16 frames[IRIS_PETALS];
  s32 x[IRIS_PETALS];
  s32 y[IRIS_PETALS];
  s32 vx[IRIS_PETALS];
  s32 vy[IRIS_PETALS];
};

/// A point of a streak: the head moves, the other nine trail it.
struct IrisPoint {
  s32 x;
  s32 y;
  s32 angle;
  s32 speed;
  s32 unused; ///< cleared as the opening starts; nothing reads it
};

/// The opening's circle of light and its fifteen streaks, at 0x2ee0.
struct IrisScene {
  union {
    s32 x;
    struct {
      u16 xFraction;
      s16 xPixels;
    };
  };
  union {
    s32 y;
    struct {
      u16 yFraction;
      s16 yPixels;
    };
  };
  s32 unused[2]; ///< nothing reads or writes them
  union {
    s32 radius;
    struct {
      u16 radiusFraction;
      s16 radiusPixels;
    };
  };
  struct IrisPoint streaks[IRIS_STREAKS][IRIS_STREAK_LENGTH];
};

/// The five widths a rising pillar takes.
struct FiveWidths {
  s32 w[5];
};

/// The seven widths a widening pillar takes.
struct SevenWidths {
  s32 w[7];
};

/// Where a strip's petals start across, by quarter of its rise: the left edge
/// and the width they spread over.
struct FourSpans {
  struct {
    s32 left;
    s32 width;
  } p[4];
};

#define IRIS_PETAL_WORK (*(struct IrisPetals *)(Ram_MapCellBuffer + 0x1f40))
#define IRIS_SCENE (*(struct IrisScene *)(Ram_MapCellBuffer + 0x2ee0))
#define IRIS_STREAK_POINTS                                                     \
  ((struct IrisPoint(*)[IRIS_STREAK_LENGTH])(Ram_MapCellBuffer + 0x2ef4))

extern const struct FiveWidths gIris_RisingPillarWidths;
extern const struct SevenWidths gIris_WideningPillarWidths;
extern const struct SevenWidths gIris_NarrowingPillarWidths;
extern const struct FourSpans gIris_LeftStripSpans;
extern const struct FourSpans gIris_RightStripSpans;

#define SCENE (&IRIS_SCENE)
#define STREAK(i, j)                                                           \
  (((struct IrisPoint *)IRIS_STREAK_POINTS)[(i) * IRIS_STREAK_LENGTH + (j)])
#define FADE_PLTT ((u16 *)&BG_PLTT_COLOR(6, 0))

#define Iwram_FillWords(dest, size, value)                                     \
  do {                                                                         \
    Iwram_FillWords(dest, size, value);                                        \
  } while (0)

#define DECOMPRESS_IN_BLOCK(src, dest)                                         \
  do {                                                                         \
    Decompress_TaggedLz(src, dest);                                            \
  } while (0)

/// A splash where a drop lands: rings spread from (x, y) for twice the
/// splash's duration, then a smaller pair of rings and a column of spray
/// rises while the screen shakes; the drop itself is drawn last.
static inline void Iris_DrawSplash(s32 t, s32 x, s32 y, s32 small) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  DrawRectangle drawAlt;
  DrawRectangle draw;
  u8 *sprites;
  s32 r;
  s32 cel;
  s32 cel8;
  s32 sheetOffset;
  s32 point;
  s32 segment;
  s32 column;
  s32 dead;

  sprites = Ram_HeapSlots->battleSheet;
  draw = Ram_HeapSlots->ramCode;
  drawAlt = Ram_HeapSlots->ramCodePaired;
  if (t < *(gIris_SplashRadii + small) * 2) {
    r = *(gIris_SplashRadii + small) * 2 - t;
    cel = r / 6;
    cel8 = cel * 8;
    sheetOffset = cel * MOTE_SET_BYTES;
    for (point = 0; point != 64; point++) {
      s32 px;
      s32 a;
      s32 py;
      a = point << 10;
      px = (FIXEDtoINT(r * Trig_Sin(a) / 2)) + x;
      py = (FIXEDtoINT(r * Trig_Cos(a))) + y;
      draw(canvas, sprites + cel8 + IRIS_DOTS, px, py - 2, IRIS_DOT_WIDTH,
           IRIS_DOT_HEIGHT);
    }
    if (small == 0) {
      s32 offset = 0;
      u8 *ring;
      DrawRectangle blit;
      s32 spoke;

      if (t <= 11)
        offset = IRIS_LARGE_RINGS + 2 * PICTURE_BYTES(IRIS_LARGE_RING);
      else if (t <= 23)
        offset = IRIS_LARGE_RINGS + PICTURE_BYTES(IRIS_LARGE_RING);
      /* FAKEMATCH(dead): t * 4 / 3 into dead. */
      dead = t * 4;
      dead /= 3;
      ring = sprites + offset;
      /* FAKEMATCH(temporary): draw copied into blit for the ring's draws. */
      blit = draw;
      for (spoke = 0; spoke != 8; spoke++) {
        s32 px;
        s32 a;
        s32 py;
        a = (spoke << 13) + (t * 5 << 9);
        px = (FIXEDtoINT(r * Trig_Sin(a))) + x;
        py = (FIXEDtoINT(r * Trig_Cos(a) * 2)) + y;
        blit(canvas, ring, px - 3, py - 7, IRIS_LARGE_RING_WIDTH,
             IRIS_LARGE_RING_HEIGHT);
        a += a >> 1;
        px = (FIXEDtoINT(Iwram_MulFX(0x28000, r * Trig_Sin(a) / 2))) + x;
        py = (FIXEDtoINT(Iwram_MulFX(0x50000, r * Trig_Cos(a) / 2))) + y;
        blit(canvas, ring, px - 3, py - 7, IRIS_LARGE_RING_WIDTH,
             IRIS_LARGE_RING_HEIGHT);
      }
      draw(canvas, work->sheet + (gBattleEffect_MotePictures[8] + sheetOffset),
           x - 4, y - 9, 9, 18);
    } else {
      s32 offset = IRIS_SMALL_RINGS;
      u8 *ring;
      DrawRectangle blit;
      s32 spoke;

      if (t <= 11)
        offset = IRIS_SMALL_RINGS + 2 * PICTURE_BYTES(IRIS_SMALL_RING);
      else if (t <= 23)
        offset = IRIS_SMALL_RINGS + PICTURE_BYTES(IRIS_SMALL_RING);
      /* FAKEMATCH(dead): t * 4 / 3 into dead, as in the large splash's arm. */
      dead = t * 4;
      dead /= 3;
      ring = sprites + offset;
      /* FAKEMATCH(temporary): draw copied into blit for the ring's draws. */
      blit = draw;
      for (spoke = 0; spoke != 8; spoke++) {
        s32 px;
        s32 a;
        s32 py;
        a = (spoke << 13) + (t * 5 << 9);
        px = (FIXEDtoINT(r * Trig_Sin(a))) + x;
        py = (FIXEDtoINT(r * Trig_Cos(a) * 2)) + y;
        blit(canvas, ring, px - 2, py - 5, IRIS_SMALL_RING_WIDTH,
             IRIS_SMALL_RING_HEIGHT);
        a += a >> 1;
        px = (FIXEDtoINT(Iwram_MulFX(0x28000, r * Trig_Sin(a) / 2))) + x;
        py = (FIXEDtoINT(Iwram_MulFX(0x50000, r * Trig_Cos(a) / 2))) + y;
        blit(canvas, ring, px - 2, py - 5, IRIS_SMALL_RING_WIDTH,
             IRIS_SMALL_RING_HEIGHT);
      }
      draw(canvas, work->sheet + (gBattleEffect_MotePictures[6] + sheetOffset),
           x - 3, y - 7, 7, 14);
    }
  } else {
    if (t < *(gIris_SplashRadii + small) * 2 + *(gIris_SplashRadii + small)) {
      s32 ringCel;

      r = t - *(gIris_SplashRadii + small) * 2;
      ringCel = r / 3 * 8;
      if (ringCel == 64)
        ringCel = 56;
      ringCel += IRIS_DOTS;
      for (point = 0; point != 64; point++) {
        s32 px;
        s32 a;
        s32 py;
        a = point << 10;
        px = (FIXEDtoINT(r * Trig_Sin(a))) + x;
        py = (FIXEDtoINT(r * Trig_Cos(a) * 2)) + y;
        draw(canvas, sprites + ringCel, px - 1, py - 2, IRIS_DOT_WIDTH,
             IRIS_DOT_HEIGHT);
      }
      for (point = 0; point != 64; point++) {
        s32 px;
        s32 a;
        s32 py;
        a = point << 10;
        px = (FIXEDtoINT(r * Trig_Sin(a) * 2)) + x;
        py =
            (FIXEDtoINT(r * Trig_Cos(a))) + y + gIris_SplashOffsetsY[small] * 2;
        draw(canvas, sprites + ringCel, px - 1, py - 2, IRIS_DOT_WIDTH,
             IRIS_DOT_HEIGHT);
      }
    }
    for (segment = 0; segment < t - *(gIris_SplashRadii + small) * 2 &&
                      y + segment * IRIS_SPRAY_HEIGHT <= 112;
         segment++)
      draw(canvas, sprites + IRIS_SPRAY, x - IRIS_SPRAY_WIDTH / 2,
           y + segment * IRIS_SPRAY_HEIGHT, IRIS_SPRAY_WIDTH,
           IRIS_SPRAY_HEIGHT);
    if (t < *(gIris_SplashRadii + small) * 2 + 8) {
      s32 shake = (*(gIris_SplashRadii + small) * 2 - t + 8) >> 1;
      typeof(gBgScroll[0]) *scroll = gBgScroll;
      s16 base = 32;

      // DEAD: both arms below set it again; the ROM keeps the store.
      scroll[1].y = base;
      if (t & 1)
        scroll[1].y = shake + 32;
      else
        scroll[1].y = base - shake;
    }
    if (small == 0)
      drawAlt(canvas, work->sheet + IRIS_DROPS, x - IRIS_DROP_WIDTH / 2,
              y - IRIS_DROP_HEIGHT / 2, IRIS_DROP_WIDTH, IRIS_DROP_HEIGHT);
    else
      drawAlt(canvas, work->sheet + IRIS_SMALL_DROPS,
              x - IRIS_SMALL_DROP_WIDTH / 2, y - IRIS_SMALL_DROP_HEIGHT / 2,
              IRIS_SMALL_DROP_WIDTH, IRIS_SMALL_DROP_HEIGHT);
  }
}

/// Iris: a circle of light opens over a dark field while fifteen
/// streaks swirl up out of it; Iris herself appears, a 4 x 4 grid of
/// sprites, amid two bursts of pink petals and later rises away; drops fall and
/// splash in rings; pillars of light climb, the scene fills with rising motes,
/// rays turn about the centre, and the screen whitens as she heals the party.
void BattleEffect_RunIris(struct BattleEffectParams *args) {
  typeof(SCENE) scene = SCENE;
  struct IrisPoint(*streaks)[IRIS_STREAK_LENGTH] = IRIS_STREAK_POINTS;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct Sprite **objects = (struct Sprite **)gMapGridCellBuffer;
  u8 *sprites = Ram_HeapSlots->battleSheet;
  struct IrisPetals *petals = &IRIS_PETAL_WORK;
  u8 *battle = (u8 *)Ram_HeapSlots->battleWork;
  s32 savedScroll = gBgScroll[1].x;
  s16 *camera = Ram_HeapSlots->cameraWork;
  s32 savedYaw;
  s32 frame;
  void *draw[2];

  // The first petal slot with no life left, or -1.
  s32 FindFreePetal(void) {
    s32 i;
    s32 slot = IRIS_PETALS;

    for (i = 0; i != IRIS_PETALS; i++) {
      if (petals->life[i] == 0) {
        slot = i;
        break;
      }
    }
    if (slot == IRIS_PETALS)
      slot = -1;
    return slot;
  }

  // Each live petal: placed at its size, moved, dropped when it leaves the
  // screen, otherwise animated and aged.
  void UpdatePetals(void) {
    struct Scale scale;
    s32 place[4];
    s32 i;

    for (i = 0; i != IRIS_PETALS; i++) {
      if (petals->life[i] != 0) {
        scale.x = scale.y = petals->life[i] * 2;
        place[3] = 0;
        place[1] = toFIXED(255);
        place[0] = petals->x[i];
        place[2] = petals->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], place, (s32 *)&scale, 0);
        petals->x[i] += petals->vx[i];
        petals->y[i] += petals->vy[i];
        if (petals->x[i] < -toFIXED(16) || petals->x[i] > toFIXED(240) ||
            petals->y[i] < 0 || petals->y[i] > toFIXED(160)) {
          petals->life[i] = 0;
          continue;
        }
        if (!(frame & 3))
          petals->frames[i]++;
        if (petals->frames[i] >= IRIS_PETAL_FRAMES)
          petals->frames[i] = 0;
        objects[i]->image.tile = petals->tiles[petals->frames[i]];
        petals->life[i] -= 0x100;
      }
    }
  }

  // The first free mote of IRIS_MOTES, or -1.
  s32 FindFreeParticle(void) {
    s32 i;
    s32 slot = IRIS_MOTES;

    for (i = 0; i != IRIS_MOTES; i++) {
      if (work->particles[i].variant == 0) {
        slot = i;
        break;
      }
    }
    if (slot == IRIS_MOTES)
      slot = -1;
    return slot;
  }

  // The motes: a sheet cell, fading after 32 frames with bit 4, or a
  // textured quad spinning up with bit 8.
  void DrawParticles(void) {
    struct ModelTexture texture;
    s32 place[3];
    s32 i;

    for (i = 0; i != IRIS_MOTES; i++) {
      if (work->particles[i].variant != 0) {
        s32 n = work->particles[i].variant & 15;
        s32 px = FIXEDtoINT(work->particles[i].x);
        s32 py = FIXEDtoINT(work->particles[i].y);
        s32 offset = 0;

        if (work->particles[i].x < 0 || work->particles[i].x > toFIXED(127) ||
            work->particles[i].y < 0 || work->particles[i].y > toFIXED(127)) {
          work->particles[i].variant = 0;
          continue;
        }
        if (work->particles[i].variant & 16) {
          s32 age = frame - work->particles[i].z;

          if (age > 32)
            offset = (age - 32) * MOTE_SET_BYTES;
          if (offset == IRIS_MOTE_FADES * MOTE_SET_BYTES) {
            work->particles[i].variant = 0;
            continue;
          }
        }
        if (work->particles[i].variant & 256) {
          void *verts = Heap_AllocEwram(32);
          struct RenderModel *model = Render_AllocateModelList(1);
          s32 age;

          offset = 0;
          age = frame - work->particles[i].z;
          if (age > 24)
            offset = (age - 24) * IRIS_TEXTURE_BYTES;
          texture.logWidth = 5;
          texture.logHeight = 6;
          texture.pixels = work->sheet + IRIS_TEXTURES + offset;
          Matrix_ResetUnit();
          place[0] = work->particles[i].x - toFIXED(64);
          place[1] = work->particles[i].y - toFIXED(64);
          place[2] = 0;
          Matrix_TranslateVector(place);
          place[1] = place[0] = work->particles[i].velocityZ;
          place[2] = 0;
          Matrix_ScaleVector(place);
          work->particles[i].velocityZ += toFIXED(1) / 8;
          model->kind = MODEL_TEXTURED_MAX;
          model->texture = &texture;
          model->mesh = gIris_ParticleMesh;
          model->vertices = verts;
          Render_TransformVertices(gIris_ParticleCorners, verts, 4);
          Render_DrawModelList(model);
          Heap_Release(model);
          Heap_Release(verts);
          if (offset == 7 * IRIS_TEXTURE_BYTES)
            work->particles[i].variant = 0;
        } else {
          ((DrawRectangle)draw[0])(
              canvas,
              work->sheet + (gBattleEffect_MotePictures[n - 1] + offset),
              px - n / 2, py - n, n, n * 2);
        }
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
      }
    }
  }

  s32 motesMask;
  u8 *tileBuffer;
  FIXED pos[4];
  struct Scale scale;
  struct Scale shrink;
  struct ModelTexture texture;
  struct ModelTexture coneTexture;
  s32 lift;
  s32 i;
  s32 j;

  s32 column;
  s32 m;
  s32 off;
  u8 *dst;
  u8 *tiles;
  /* FAKEMATCH(dead): 274 locals nothing uses (any count from 264 to 283 does).
   */
  UNUSED s32 z0, z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11, z12, z13, z14,
      z15, z16, z17, z18, z19, z20, z21, z22, z23, z24, z25, z26, z27, z28, z29,
      z30, z31, z32, z33, z34, z35, z36, z37, z38, z39, z40, z41, z42, z43, z44,
      z45, z46, z47, z48, z49, z50, z51, z52, z53, z54, z55, z56, z57, z58, z59,
      z60, z61, z62, z63, z64, z65, z66, z67, z68, z69, z70, z71, z72, z73, z74,
      z75, z76, z77, z78, z79, z80, z81, z82, z83, z84, z85, z86, z87, z88, z89,
      z90, z91, z92, z93, z94, z95, z96, z97, z98, z99, z100, z101, z102, z103,
      z104, z105, z106, z107, z108, z109, z110, z111, z112, z113, z114, z115,
      z116, z117, z118, z119, z120, z121, z122, z123, z124, z125, z126, z127,
      z128, z129, z130, z131, z132, z133, z134, z135, z136, z137, z138, z139,
      z140, z141, z142, z143, z144, z145, z146, z147, z148, z149, z150, z151,
      z152, z153, z154, z155, z156, z157, z158, z159, z160, z161, z162, z163,
      z164, z165, z166, z167, z168, z169, z170, z171, z172, z173, z174, z175,
      z176, z177, z178, z179, z180, z181, z182, z183, z184, z185, z186, z187,
      z188, z189, z190, z191, z192, z193, z194, z195, z196, z197, z198, z199,
      z200, z201, z202, z203, z204, z205, z206, z207, z208, z209, z210, z211,
      z212, z213, z214, z215, z216, z217, z218, z219, z220, z221, z222, z223,
      z224, z225, z226, z227, z228, z229, z230, z231, z232, z233, z234, z235,
      z236, z237, z238, z239, z240, z241, z242, z243, z244, z245, z246, z247,
      z248, z249, z250, z251, z252, z253, z254, z255, z256, z257, z258, z259,
      z260, z261, z262, z263, z264, z265, z266, z267, z268, z269, z270, z271,
      z272, z273;

  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  EFFECT_START_TRANSFER(work, CANVAS_COPY, 75);
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  Iris_WipeScreen(0, 4, Iris_ChangeBackdrop);
  Iwram_FillWords(canvas, CANVAS_BYTES, 0);
  while (((volatile struct DmaRegisters *)REG_DMA3)->control &
         (DMA_ENABLE << 16)) {
  }
  EFFECT_PRESENT_CANVAS(work);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  savedYaw = camera[27];
  if (args->side == EFFECT_SIDE_ENEMIES) {
    s32 yaw = DEGtoANG(90);

    camera[27] = yaw;
  } else {
    s32 yaw = TRIG_TURN * 3 / 4;

    camera[27] = yaw;
  }
  tileBuffer = gIris_TileBuffer;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_ADDITIVE);
  draw[0] = Ram_HeapSlots->ramCode;
  draw[1] = Ram_HeapSlots->ramCodePaired;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, tileBuffer, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  for (i = 0; i != MOTE_SET_BYTES; i++)
    sprites[i] >>= 1;
  BattleEffect_DrawShadedCircles(work->sheet, 32, FIXED_ONE);
  Graphics_LoadGreyRamp();
  frame = 0;
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  Matrix_Initialize();

  for (;;) {
    motesMask = 31;
    // A or B: skips the opening, or the rest of the scene.
    if (gInput.repeat & (KEY_A | KEY_B)) {
      if (frame <= 175) {
        frame = 176;
      } else if ((u32)(frame - 177) <= 268) {
        REG_BLDALPHA = EFFECT_BLEND_ADD;
        Iwram_FillWords(canvas, CANVAS_BYTES,
                        CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        EFFECT_PRESENT_CANVAS(work);
        if (frame <= 386) {
          for (i = 0; i != IRIS_GODDESS_SPRITES; i++)
            Sprite_DestroyFar(work->sprites[i]);
          BattleEffect_SelectLivingTargets(work->args);
          Palette_LoadBasicSpriteColors();
          for (i = 0; i != work->args->targetCount; i++)
            BattleEffect_TintBattler(work->args->targets[i], TINT_KEEP,
                                     POSE_DAMAGE, TINT_KEEP, 0);
        }
        frame = 446;
      } else {
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        EFFECT_PRESENT_CANVAS(work);
        break;
      }
    }
    if (frame == 850)
      break;

    if (frame == 0)
      Sound_PlayFar(SE_CURSE_FALL);
    for (i = 0; i != 11; i++)
      if (frame == gIris_PsynergySoundFrames[i])
        Sound_PlayFar(SE_PSYNERGY_START);
    if (frame == 175)
      Sound_PlayFar(SE_POWER_UP);
    for (i = 0; i != 5; i++)
      if (frame == gIris_DoorSoundFrames[i])
        Sound_PlayFar(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
    for (i = 0; i != 6; i++)
      if (frame == gIris_PowerUpSoundFrames[i])
        Sound_PlayFar(SE_POWER_UP);
    if (frame == 369)
      Sound_PlayFar(SE_CURSE_FALL);
    for (i = 0; i != 6; i++)
      if (frame == 644 + i * 12)
        Sound_PlayFar(SE_LAUNCH);
    if (frame == 755)
      Sound_PlayFar(SE_POWER_UP);
    for (i = 0; i != 6; i++)
      if (frame == 759 + i * 9)
        Sound_PlayFar(SE_CRASH);
    if (frame == 820)
      Sound_PlayFar(SE_BANE_ESCAPE);

    // The opening: the circle and the streaks.
    if (frame == 0) {
      scene->x = toFIXED(60);
      scene->y = toFIXED(128);
      scene->radius = toFIXED(12);
      for (i = 0; i != IRIS_STREAKS; i++) {
        struct IrisPoint *s = streaks[i];

        s->x = INTtoFIXED(gIris_StreakX[i]);
        s->y = INTtoFIXED(gIris_StreakY[i]);
        s->angle = gIris_StreakAngles[i];
        s->speed = gIris_StreakSpeeds[i];
        s->unused = 0;
        for (j = 1; j != IRIS_STREAK_LENGTH; j++) {
          STREAK(i, j).x = toFIXED(150);
          STREAK(i, j).y = toFIXED(150);
        }
      }
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame <= 175) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      draw[1] = Ram_HeapSlots->ramCodePaired;
      for (i = 0; i != IRIS_STREAKS; i++) {
        if (!(frame & 1)) {
          for (j = 9; j != 0; j--) {
            STREAK(i, j).x = STREAK(i, j - 1).x;
            STREAK(i, j).y = STREAK(i, j - 1).y;
          }
        }
        if (frame >= i * 8 + 24) {
          struct IrisPoint *s = streaks[i];
          if (HALF(s, 3) > 59) {
            s32 size;

            s->x += Trig_Sin(s->angle) * 3 / 2;
            s->y -= Trig_Cos(s->angle) * 3 / 2;
            s->angle += s->speed * 2 / 3;
            size = (i & 3) + 5;
            for (j = 0; j != IRIS_STREAK_LENGTH; j++) {
              u8 *to = canvas;
              u8 *cell = sprites + gBattleEffect_MotePictures[size - 1];
              struct IrisPoint *p = &STREAK(i, j);

              ((DrawRectangle)draw[1])(to, cell, HALF(p, 1) - (size >> 1),
                                       HALF(p, 3) - size, size, size * 2);
            }
          }
        }
      }
      if (frame <= 71) {
        scene->y -= toFIXED(1);
        scene->radius -= toFIXED(1) / 16;
        if (!(frame & 15)) {
          {
            u16 *fade = FADE_PLTT;

            for (i = 0; i != IRIS_FADE_COLORS; i++) {
              s32 c = *fade;
              s32 b = RGB_BLUE((u16)c);
              s32 g = RGB_GREEN((u16)c);
              s32 r = RGB_RED(c);

              r += 1;
              *fade++ = (b + 1) << 10 | (g + 1) << 5 | r;
            }
          }
        }
      } else {
        scene->radius += toFIXED(1) / 8;
        if (!(frame & 7)) {
          {
            u16 *fade = FADE_PLTT;

            for (i = 0; i != IRIS_FADE_COLORS; i++) {
              s32 c = *fade;
              s32 b = RGB_BLUE((u16)c);
              s32 g = RGB_GREEN((u16)c);
              s32 r = RGB_RED(c);

              if (r < 23)
                r++;
              if (g > 1)
                g--;
              if (b < 15)
                b++;
              {
                u16 v = RGB(r, g, b);

                *fade++ = v;
              }
            }
          }
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      {
        s32 size = scene->radiusPixels * 2;

        BattleEffect_DrawShadedCircles(work->sheet, size, FIXED_ONE);
        Canvas_BlitAtFoot(work->sheet, scene->xPixels, scene->yPixels, size);
      }
      if (frame == 175) {
        REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                     BGCNT_SCREEN_BASE(7);
        Iwram_FillWords(canvas, CANVAS_BYTES,
                        CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        Iwram_FillWords(FADE_PLTT, 0x100, COLOR_PAIR(RGB_WHITE));
      }
    }

    // Iris appears: her sixteen sprites, the petals and the first motes.
    if (frame == 176) {
      u8 *colors;
      struct VramCacheEntry *table;

      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      EFFECT_PRESENT_CANVAS(work);
      battle[0x867] = 1;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               (s32)&ResourceId_SunsetBackdrop, 0);
      colors = Resource_GetTableEntry((s32)&ResourceId_RobedGoddessTiles);
      Dma_Set(colors, (void *)OBJ_PLTT, DMA_COPY_WORDS(128), REG_DMA3);
      colors += 0x200;
      Decompress_TaggedLz(colors, tileBuffer);
      tiles = gIris_TileBuffer;
      for (i = 0; i != IRIS_GODDESS_SPRITES; i++) {
        struct Sprite *obj = EffectSprite_CreateWithTiles(
            32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

        work->sprites[i] = obj;
        obj->image.priority = 1;
        Dma_Set(tiles + i * 0x400, (void *)OBJ_TILES(obj), DMA_COPY_WORDS(256),
                REG_DMA3);
      }
      colors = Resource_GetTableEntry((s32)&ResourceId_PinkPetalsTiles);
      Iwram_FillWords((void *)&OBJ_PLTT_COLOR(15, 0), 32, -1);
      colors += 32;
      DECOMPRESS_IN_BLOCK(colors, work->sheet);
      table = gVramCacheEntries;
      for (i = 0; i != IRIS_PETAL_FRAMES; i++) {
        struct Sprite *obj = EffectSprite_CreateWithTiles(
            16, 16, OBJ_SIZE_16X16, OBJ_PALETTE(15));

        objects[i] = obj;
        obj->image.priority = 1;
        petals->tiles[i] = obj->image.tile;
        petals->frames[i] = i;
        Dma_Set(work->sheet + i * 128,
                (void *)(OBJ_VRAM0 + table[obj->slot].offset),
                DMA_COPY_WORDS(32), REG_DMA3);
      }
      for (; i != IRIS_PETALS; i++) {
        struct Sprite *obj =
            EffectSprite_Acquire(16, 16, OBJ_SIZE_16X16, OBJ_PALETTE(15));

        objects[i] = obj;
        Dma_Set(objects[0], obj, DMA_COPY_WORDS(6), REG_DMA3);
        petals->frames[i] = Random16() % IRIS_PETAL_FRAMES;
        objects[i]->image.tile = petals->tiles[petals->frames[i]];
      }
      for (i = 0; i != IRIS_PETALS / 2; i++) {
        petals->x[i] = INTtoFIXED(Random16() % 15) + toFIXED(57);
        petals->y[i] = INTtoFIXED(Random16() % 15) + toFIXED(79);
        petals->vx[i] = ((Random16() & 63) - 32) << 12;
        petals->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      for (; i != IRIS_PETALS; i++) {
        petals->x[i] = INTtoFIXED(Random16() % 15) + toFIXED(169);
        petals->y[i] = INTtoFIXED(Random16() % 15) + toFIXED(79);
        petals->vx[i] = ((Random16() & 63) - 32) << 12;
        petals->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, work->sheet,
                                 0, 0);
      for (i = 0; i != 6; i++) {
        work->particles[i].x = INTtoFIXED(Random16() % 15) + toFIXED(33);
        work->particles[i].y = INTtoFIXED(Random16() % 15) + toFIXED(57);
        work->particles[i].velocityX = -(Random16() & motesMask) << 12;
      }
      for (; i != 12; i++) {
        work->particles[i].x = INTtoFIXED(Random16() % 15) + toFIXED(81);
        work->particles[i].y = INTtoFIXED(Random16() % 15) + toFIXED(57);
        work->particles[i].velocityX = (Random16() & motesMask) << 12;
      }
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 70);
    }
    if ((u32)(frame - 177) <= 216) {
      scale = gIris_UnitScale;
      pos[3] = 0;
      pos[1] = toFIXED(255);
      lift = 0;
      if (frame > 350) {
        u16 *pal = (u16 *)OBJ_PLTT;

        for (i = 0; i != 255; i++) {
          s32 c = *pal;
          s32 r = RGB_LEVEL_MAX;
          s32 g;
          s32 b;

          r &= c;
          g = RGB_GREEN((u16)c);
          b = RGB_BLUE((u16)c);

          if (r != RGB_LEVEL_MAX)
            r++;
          if (g != RGB_LEVEL_MAX)
            g++;
          if (b != RGB_LEVEL_MAX)
            b++;
          *pal++ = RGB(r, g, b);
        }
      }
      if (frame <= 385) {
        if ((u32)(frame - 351) <= 14)
          lift = Trig_Sin((frame - 350) << 10) << 6;
        else if (frame > 365)
          lift = ((366 - frame) * 5 << 17) + 0x400000;
        if (frame <= 229)
          lift = (Trig_Sin((frame << 6) - 0x2c00) << 6) - 0x147200;
        for (j = 0; j != IRIS_GODDESS_ROWS; j++) {
          for (column = 0; column != IRIS_GODDESS_COLUMNS; column++) {
            pos[0] = toFIXED(72) + column * toFIXED(IRIS_GODDESS_TILE);
            pos[2] = lift + toFIXED(56) + j * toFIXED(IRIS_GODDESS_TILE);
            Render_ApplyProjectedPlacementFar(
                work->sprites[j * IRIS_GODDESS_COLUMNS + column], pos,
                (s32 *)&scale, 0);
          }
        }
      }
      if (frame == 386) {
        for (i = 0; i != IRIS_GODDESS_SPRITES; i++)
          Sprite_DestroyFar(work->sprites[i]);
        BattleEffect_SelectLivingTargets(work->args);
        Palette_LoadBasicSpriteColors();
        for (i = 0; i != work->args->targetCount; i++)
          BattleEffect_TintBattler(work->args->targets[i], TINT_KEEP,
                                   POSE_DAMAGE, TINT_KEEP, 0);
      }
      if (frame <= 191) {
        for (i = 0; i != 3; i++) {
          s32 y = i + 62;
          u8 v = frame + (i & 1) * 47 + 80;
          s32 off;
          s32 x;
          s32 m;

          off = CANVAS_ROW_OFFSET(y);
          for (x = 0; x != CANVAS_SIZE; x += 8) {
            for (m = x; m != x + 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += CANVAS_TILE_BYTES - 8;
          }
        }
      }
    }
    if ((u32)(frame - 177) <= 46) {
      for (i = 0; i != IRIS_PETALS; i++) {
        shrink.x = shrink.y = toFIXED(1) - ((frame - 177) >> 1 << 11);
        pos[3] = 0;
        pos[1] = toFIXED(255);
        pos[0] = petals->x[i];
        pos[2] = petals->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], pos, (s32 *)&shrink, 0);
        petals->x[i] += petals->vx[i];
        petals->y[i] += petals->vy[i];
        if (!(frame & 3))
          petals->frames[i]++;
        if (petals->frames[i] >= IRIS_PETAL_FRAMES)
          petals->frames[i] = 0;
        objects[i]->image.tile = petals->tiles[petals->frames[i]];
      }
    }
    if ((u32)(frame - 179) <= 38) {
      s32 radius = frame - 170;

      for (i = 0; i != 256; i++) {
        s32 a = i << 8;
        s32 px = Trig_Sin(a) * radius;
        s32 py = Trig_Cos(a) * radius;

        ((DrawRectangle)draw[0])(
            canvas, work->sheet + gBattleEffect_MotePictures[3],
            (FIXEDtoINT(px)) + 58, (FIXEDtoINT(py * 2)) + 48, 4, 8);
      }
      for (i = 0; i != 12; i++) {
        ((DrawRectangle)draw[0])(canvas,
                                 work->sheet + gBattleEffect_MotePictures[1],
                                 (FIXEDtoINT(work->particles[i].x)) - 2,
                                 (FIXEDtoINT(work->particles[i].y)) - 2, 2, 4);
        work->particles[i].x += work->particles[i].velocityX;
      }
    }
    if ((u32)(frame - 179) <= 31)
      REG_BLDALPHA = (210 - frame) >> 1 | 0x1000;

    // The drops: the motes' fading sets, the drops' pictures and the spray's
    // column.
    if (frame == 230) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      for (i = 1; i != IRIS_MOTE_FADES; i++) {
        s32 next;

        for (j = 0; j != MOTE_PICTURES; j = next) {
          u8 *src = work->sheet + gBattleEffect_MotePictures[j];
          u8 *out = work->sheet +
                    (gBattleEffect_MotePictures[j] + i * MOTE_SET_BYTES);
          s32 off = 0;

          next = j + 1;

          for (; off != (next << 1) * next; off++) {
            u8 v = *src - i * 8;

            if (v > CANVAS_LEVEL_BRIGHTEST)
              v = 0;
            *out = v;
            src++;
            out++;
          }
        }
      }
      for (i = 0; i != 8; i++) {
        BattleEffect_DrawShadedCirclesMirrored(
            work->sheet + IRIS_DROPS + i * PICTURE_BYTES(IRIS_DROP),
            IRIS_DROP_WIDTH, FIXED_ONE - i * (FIXED_ONE / IRIS_DROP_PICTURES));
        BattleEffect_DrawShadedCirclesMirrored(
            work->sheet + IRIS_SMALL_DROPS + i * PICTURE_BYTES(IRIS_SMALL_DROP),
            IRIS_SMALL_DROP_WIDTH,
            FIXED_ONE - i * (FIXED_ONE / IRIS_DROP_PICTURES));
      }
      for (i = 0; i != IRIS_PETALS; i++)
        petals->life[i] = 0;
      Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0,
                                 0);
      for (i = 0; i != 32; i++)
        for (j = 0; j != 8; j++)
          sprites[IRIS_SPRAY + i * IRIS_SPRAY_WIDTH + j] = gIris_LineShades[j];
      dst = sprites;
      for (i = 0; i != 2; i++) {
        for (j = 0; j != 3; j++) {
          u8 *src =
              work->sheet + gBattleEffect_MotePictures[gIris_MoteSizes[i] - 1];

          for (m = 0; m != *(gIris_MoteSizes + i) * *(gIris_MoteSizes + i) * 2;
               m++) {
            s32 v = *src - j * 16;

            if (v < 0)
              v = 0;
            *dst = v;
            dst++;
            src++;
          }
        }
      }
      for (j = 0; j != 8; j++) {
        u8 *src = work->sheet + gBattleEffect_MotePictures[1];

        for (m = 0; m != 8; m++) {
          s32 v = *src - j * 8;

          if (v < 0)
            v = 0;
          *dst = v;
          src++;
          dst++;
        }
      }
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    if ((u32)(frame - 240) <= 127 && !(frame & 15)) {
      s32 v = (240 - frame) / 16;

      Palette_BrightenSavedBattleBg(v, v, v);
    }
    for (j = 0; j != 6; j++) {
      if (frame > gIris_SplashStarts[j]) {
        if (frame < gIris_SplashEnds[j]) {
          Iris_DrawSplash(frame - gIris_SplashStarts[j], gIris_SplashX[j],
                          gIris_SplashY[j], gIris_SplashSmall[j]);
          if (gIris_SplashStarts[j] - gIris_SplashSmall[j] * 16 + 48 == frame) {
            for (i = 0; i != 6; i++) {
              s32 s = FindFreePetal();

              petals->life[s] = 0x7900;
              petals->x[s] = gIris_SplashX[j] << 17;
              petals->y[s] = INTtoFIXED(gIris_SplashY[j]) + toFIXED(32);
              petals->vx[s] = ((Random16() & 31) - 16) << 12;
              petals->vy[s] = -(Random16() & 31) << 12;
            }
          }
        } else if (frame < gIris_SplashEnds[j] + gIris_DropFrames[j]) {
          s32 t = frame - gIris_SplashEnds[j];

          Iris_DrawDrop(t / gIris_DropPictureSteps[j], gIris_SplashX[j],
                        gIris_SplashY[j] - gIris_DropSpeeds[j] * t / 2,
                        gIris_SplashSmall[j]);
        }
      }
    }
    if ((u32)(frame - 279) <= 132 || frame > 446)
      UpdatePetals();
    HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);

    // The pillars of light.
    if (frame == 370) {
      BattleEffect_DrawShadedCircles(work->sheet + IRIS_PILLAR_CIRCLE,
                                     IRIS_PILLAR_TEXTURE_SIZE, FIXED_ONE);
      for (i = 0; i != 16; i++) {
        Dma_Set(work->sheet + IRIS_PILLAR_CIRCLE + (i << 3),
                work->sheet + IRIS_PILLAR_TEXTURE + (i << 4),
                DMA_COPY_HALFWORDS(8), REG_DMA3);
        for (j = 0; j != 8; j++)
          work->sheet[IRIS_PILLAR_TEXTURE + 8 + i * 16 + j] =
              work->sheet[IRIS_PILLAR_CIRCLE + 7 + i * 8 - j];
      }
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(8);
    }
    if ((u32)(frame - 414) <= 7) {
      s32 alpha = frame - 406;
      REG_BLDALPHA = BLEND_ALPHA(alpha, BLEND_WEIGHT_MAX);
    }
    if (frame > 385)
      gProjection.centerY = 2556 - frame * 6;
    if (frame <= 422) {
      if (frame > 370) {
        s32 h = (frame - 370) << 3;

        if (h > CANVAS_SIZE - 1)
          h = CANVAS_SIZE - 1;
        Iris_DrawPillar(3, 34, h);
      }
      if (frame > 375) {
        struct FiveWidths widths = gIris_RisingPillarWidths;
        s32 h = (frame - 375) << 3;

        if (h > CANVAS_SIZE - 1)
          h = CANVAS_SIZE - 1;
        Iris_DrawPillar(widths.w[h >> 5], (h >> 5) * 2 + 94, h);
      }
      if (frame > 380) {
        struct SevenWidths widths = gIris_WideningPillarWidths;
        s32 h = (frame - 380) << 2;
        s32 q;

        if (h > CANVAS_SIZE - 1)
          h = CANVAS_SIZE - 1;
        q = h / 19;
        Iris_DrawPillar(widths.w[q], q + 12, h);
      }
      if (frame > 382) {
        struct SevenWidths widths = gIris_NarrowingPillarWidths;
        s32 h = (frame - 382) << 3;
        s32 q;

        if (h > CANVAS_SIZE - 1)
          h = CANVAS_SIZE - 1;
        q = h / 21;
        Iris_DrawPillar(widths.w[q], q * 2 + 70, h);
      }
      if (frame > 385) {
        s32 h = (frame - 385) << 2;

        if (h > CANVAS_SIZE - 1)
          h = CANVAS_SIZE - 1;
        Iris_DrawPillar(14, (h >> 3) + 48, h);
      }
      if (frame == 422) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY, 0);
        REG_BLDALPHA = EFFECT_BLEND_ADD;
      }
    }
    if ((u32)(frame - 423) <= 19) {
      s32 step = frame - 422;
      s32 first;
      s32 dead;
      s32 top;
      s32 off;
      s32 k;
      s32 m;
      u8 v;

      top = 120;
      first = top - step * 8;
      /* FAKEMATCH(dead): off set to 0. */
      off = 0;
      /* FAKEMATCH(temporary): the band's first row passed to i through first.
       */
      i = first;
      top = i;
      for (; i != top + 40 && (v = (i - top) * 2, i <= CANVAS_SIZE - 1); i++) {
        if (i >= 0) {
          /* FAKEMATCH(dead): i / 3 into dead, which flow deletes. */
          dead = i / 3;
          if (v > CANVAS_LEVEL_BRIGHTEST)
            v = CANVAS_LEVEL_BRIGHTEST;
          off = CANVAS_ROW_OFFSET(i);
          for (k = 0; k != CANVAS_SIZE; k += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += CANVAS_TILE_BYTES - 8;
          }
        }
      }
    }
    if ((u32)(frame - 401) <= 30 && (frame & 1)) {
      Resource_GetTableEntry((s32)&ResourceId_VioletSkyBackdrop);
      {
        u16 *fade = FADE_PLTT;

        for (i = 0; i != IRIS_FADE_COLORS; i++) {
          s32 c = *fade;
          s32 r = RGB_LEVEL_MAX;
          s32 g;
          s32 b;

          r &= c;
          g = RGB_GREEN((u16)c);
          b = RGB_BLUE((u16)c);

          if (r > 3)
            r -= 2;
          if (g > 3)
            g -= 2;
          if (b > 3)
            b -= 2;
          *fade++ = RGB(r, g, b);
        }
      }
    }

    // The fountain: the second backdrop, the motes and the light shafts.
    if (frame == 446) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Matrix_Initialize();
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               (s32)&ResourceId_StarfieldFlareBackdrop, 0);
      off = MOTE_SET_BYTES;
      {
        u16 *fade = FADE_PLTT;
        s32 red = RGB_LEVEL_MAX;

        for (i = 0; i != IRIS_FADE_COLORS; i++) {
          s32 c = *fade;
          s32 r = red & c;
          s32 g = RGB_GREEN((u16)c);
          s32 b = RGB_BLUE((u16)c);

          g += 1;
          *fade++ = (b + 1) << 10 | g << 5 | (r + 1);
        }
      }
      Graphics_LoadGreyRamp();
      for (i = 0; i != 6; i += 2) {
        s32 n = i + 14;
        s32 w = n * 2;

        BattleEffect_DrawShadedCircles(work->sheet + off, n, FIXED_ONE);
        off += n * w;
      }
      Iwram_FillWords(work->sheet + IRIS_TEXTURES, 2 * IRIS_TEXTURE_BYTES, 0);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + IRIS_TEXTURES +
                                               IRIS_TEXTURE_BYTES,
                                           16, IRIS_TEXTURE_HEIGHT, FIXED_ONE);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + IRIS_TEXTURES +
                                               2 * IRIS_TEXTURE_BYTES,
                                           16, IRIS_TEXTURE_HEIGHT, FIXED_ONE);
      {
        u8 *ramp = work->sheet + IRIS_TEXTURES + IRIS_TEXTURE_BYTES;

        for (i = 0; i != 64; i++) {
          for (j = 0; j != 32; j++) {
            if (*ramp > i)
              *ramp = i;
            ramp++;
          }
        }
      }
      for (i = 0; i != IRIS_PETALS; i++)
        petals->life[i] = 0;
      for (i = 0; i != IRIS_MOTES; i++)
        work->particles[i].variant = 0;
      for (i = 1; i != IRIS_MOTE_FADES; i++) {
        s32 next;

        for (j = 0; j != MOTE_PICTURES; j = next) {
          u8 *src = work->sheet + gBattleEffect_MotePictures[j];
          u8 *out = work->sheet +
                    (gBattleEffect_MotePictures[j] + i * MOTE_SET_BYTES);
          s32 off = 0;

          next = j + 1;

          for (; off != (next << 1) * next; off++) {
            u8 v = *src - i * 8;

            if (v > CANVAS_LEVEL_BRIGHTEST)
              v = 0;
            *out = v;
            src++;
            out++;
          }
        }
      }
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 70);
    }
    if ((u32)(frame - 447) <= 15) {
      s32 step = frame - 446;
      s32 top;
      s32 off;
      s32 x;
      s32 m;

      top = 136 - step * 8;
      for (i = top; i != 0; i--) {
        u8 v = (top - i) * 2;

        if (i <= CANVAS_SIZE - 1) {
          if (v > CANVAS_LEVEL_BRIGHTEST)
            v = CANVAS_LEVEL_BRIGHTEST;
          if (top - 8 < i)
            v = 0;
          off = CANVAS_ROW_OFFSET(i);
          for (x = 0; x != CANVAS_SIZE; x += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += CANVAS_TILE_BYTES - 8;
          }
        }
      }
    }
    if (frame > 446)
      DrawParticles();
    if ((u32)(frame - 447) <= 194) {
      void *verts = Heap_AllocEwram(80);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 scroll;
      struct FourSpans leftSpans;
      struct FourSpans rightSpans;

      texture.logWidth = 5;
      texture.logHeight = 6;
      Matrix_ResetUnit();
      model->kind = MODEL_TEXTURED_MAX;
      model->mesh = gIris_StripMesh;
      model->vertices = verts;
      if (frame <= 563) {
        scroll = 2 * IRIS_TEXTURE_BYTES;
        if (frame > 500) {
          scroll = (s16)(scroll - (frame - 500) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + IRIS_TEXTURES + scroll;
        if (!(frame & 7)) {
          s32 s;

          leftSpans = gIris_LeftStripSpans;
          s = FindFreePetal();

          if (s != -1) {
            s32 r = Random16() & 127;

            petals->life[s] = -0x7c00;
            petals->x[s] = (leftSpans.p[(r + 1) >> 5].left +
                            Random16() % leftSpans.p[(r + 1) >> 5].width)
                           << 16;
            petals->y[s] = INTtoFIXED(r) + toFIXED(16);
            petals->vx[s] = (Random16() & 15) * 3 << 11;
            petals->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        model->texture = &texture;
        Render_TransformVertices(gIris_LeftStripCorners, verts, 10);
        Render_DrawModelList(model);
      }
      if (frame <= 563) {
        scroll = 2 * IRIS_TEXTURE_BYTES;
        if (frame > 470) {
          scroll = (s16)(scroll - (frame - 470) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + IRIS_TEXTURES + scroll;
        if (!(frame & 7) && frame <= 501) {
          s32 s;

          rightSpans = gIris_RightStripSpans;
          s = FindFreePetal();

          if (s != -1) {
            s32 r = Random16() & 127;

            petals->life[s] = -0x7c00;
            petals->x[s] = (rightSpans.p[(r + 1) >> 5].left +
                            Random16() % rightSpans.p[(r + 1) >> 5].width)
                           << 16;
            petals->y[s] = INTtoFIXED(r) + toFIXED(16);
            petals->vx[s] = (Random16() & 15) * 3 << 11;
            petals->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        if (!(frame & 3)) {
          s32 n = FindFreeParticle();

          if (n != -1) {
            work->particles[n].x = toFIXED(32);
            if (frame <= 509)
              work->particles[n].x += INTtoFIXED(Random16() & 63);
            else
              work->particles[n].x += INTtoFIXED(Random16() & motesMask);
            work->particles[n].y = toFIXED(127);
            work->particles[n].velocityX = (Random16() & 15) << 12;
            work->particles[n].velocityY =
                -toFIXED(1) / 2 - ((Random16() & 31) * 7 << 11);
            work->particles[n].variant = (Random16() & 3) + 1;
          }
        }
        model->texture = &texture;
        Render_TransformVertices(gIris_RightStripCorners, verts, 10);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
    }

    // The five light bars sweeping in from the edges.
    if ((u32)(frame - 513) <= 209) {
      s16 shade = 723 - frame;

      if (shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
        shade = CANVAS_LEVEL_BRIGHTEST;
      BattleEffect_DrawDiagonalCanvasLine(0, 127, frame - 512, shade);
    }
    if ((u32)(frame - 522) <= 209) {
      s16 shade = 732 - frame;

      if (shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
        shade = CANVAS_LEVEL_BRIGHTEST;
      BattleEffect_DrawDiagonalCanvasLine(
          0, FIXEDtoINT(Iwram_MulFX((frame - 521) * 0x10000, 0x400) + 0x7b0000),
          frame - 521, shade);
    }
    if ((u32)(frame - 531) <= 209) {
      s32 x = toFIXED(4);
      s16 shade = 741 - frame;

      if (shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
        shade = CANVAS_LEVEL_BRIGHTEST;
      x -= Iwram_MulFX((frame - 530) * toFIXED(1), 0x400);
      BattleEffect_DrawDiagonalCanvasLine(FIXEDtoINT(x), 127, frame - 530,
                                          shade);
    }
    if ((u32)(frame - 535) <= 209) {
      s16 shade = 745 - frame;

      if (shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
        shade = CANVAS_LEVEL_BRIGHTEST;
      BattleEffect_DrawDiagonalCanvasLine(
          0, FIXEDtoINT(Iwram_MulFX((frame - 534) * 0x10000, 0x600) + 0x780000),
          frame - 534, shade);
    }
    if ((u32)(frame - 554) <= 209) {
      s32 x = toFIXED(6);
      s16 shade = 764 - frame;

      if (shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
        shade = CANVAS_LEVEL_BRIGHTEST;
      x -= Iwram_MulFX((frame - 553) * toFIXED(1), 0x600);
      BattleEffect_DrawDiagonalCanvasLine(FIXEDtoINT(x), 127, frame - 553,
                                          shade);
    }

    // The healing: rings turning about the centre, then the great glow.
    if ((u32)(frame - 639) <= 110) {
      s32 sinT = Trig_Sin(TRIG_TURN * 15 / 32);
      s32 cosT = Trig_Cos(TRIG_TURN * 15 / 32);

      for (j = 0; j != 80; j += 8) {
        if (638 + j < frame && frame < 670 + j) {
          s32 t = frame - j - 638;
          s32 ry = t >> 1;
          s32 tile = gBattleEffect_MotePictures[0] + t / 4 * MOTE_SET_BYTES;

          for (i = 0; i != 128; i++) {
            s32 a = i << 9;
            s32 sx = Trig_Sin(a) * (ry >> 1);
            s32 sy = Trig_Cos(a) * ry;
            s32 px = Iwram_MulFX(sx, cosT) - Iwram_MulFX(sy * 2, sinT);
            s32 py = Iwram_MulFX(sx, sinT) + Iwram_MulFX(sy * 2, cosT);
            s32 rowY;

            px = (FIXEDtoINT(px)) + (j >> 1) + 64;
            rowY = (FIXEDtoINT(py)) - (j >> 1) + 63;
            ((DrawRectangle)draw[0])(canvas, work->sheet + tile, px, rowY, 1,
                                     2);
          }
        }
      }
    }
    if ((u32)(frame - 755) <= 2 || frame == 781)
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    if (frame == 754) {
      s32 white;
      BattleEffect_BuildSteepShadeRampWave(work->sheet + IRIS_TEXTURES +
                                               8 * IRIS_TEXTURE_BYTES,
                                           16, 32, FIXED_ONE);
      BattleEffect_DrawShadedCircles(work->sheet + IRIS_TEXTURES +
                                         IRIS_TEXTURE_BYTES / 2,
                                     IRIS_TEXTURE_WIDTH, FIXED_ONE);
      for (i = 0; i != 32; i++) {
        Dma_Set(work->sheet + (i << 4) + IRIS_TEXTURES + IRIS_TEXTURE_BYTES / 2,
                work->sheet + (i << 5) + IRIS_TEXTURES, DMA_COPY_WORDS(8),
                REG_DMA3);
        for (j = 0; j != 16; j++)
          work->sheet[IRIS_TEXTURES + 16 + i * IRIS_TEXTURE_WIDTH + j] =
              work->sheet[IRIS_TEXTURES + 15 + i * IRIS_TEXTURE_WIDTH - j];
      }
      for (i = 0; i != 32; i++)
        Dma_Set(work->sheet + IRIS_TEXTURES + IRIS_TEXTURE_BYTES / 2 -
                    IRIS_TEXTURE_WIDTH - (i << 5),
                work->sheet + IRIS_TEXTURES + IRIS_TEXTURE_BYTES / 2 + (i << 5),
                DMA_COPY_WORDS(16), REG_DMA3);
      white = 64;
      for (i = 1; i != 8; i++) {
        u8 *src = work->sheet + IRIS_TEXTURES;
        u8 *out = work->sheet + IRIS_TEXTURES + i * IRIS_TEXTURE_BYTES;
        u8 v;

        j = 0;
        v = white - i * 8;
        for (; j != IRIS_TEXTURE_BYTES; j++) {
          if (v < *src)
            *out = v;
          else
            *out = *src;
          out++;
          src++;
        }
      }
    }
    if ((u32)(frame - 759) <= 30) {
      s32 sinT = Trig_Sin(TRIG_TURN * 11 / 32);
      s32 cosT = Trig_Cos(TRIG_TURN * 11 / 32);
      s32 t = frame - 758;
      s32 tile = gBattleEffect_MotePictures[5] + t / 4 * MOTE_SET_BYTES;

      for (i = 0; i != 96; i++) {
        s32 a = i * 384;
        s32 sx = Trig_Sin(a) * (t << 1);
        s32 sy = Trig_Cos(a) * t;
        s32 px = FIXEDtoINT(Iwram_MulFX(sx, cosT) - Iwram_MulFX(sy, sinT));
        s32 py = FIXEDtoINT(Iwram_MulFX(sx, sinT) + Iwram_MulFX(sy, cosT));

        ((DrawRectangle)draw[0])(canvas, work->sheet + tile, px + 101, py + 26,
                                 6, 12);
      }
    }
    if ((u32)(frame - 783) <= 46) {
      FIXED sinT = Trig_Sin(TRIG_TURN * 29 / 32);
      FIXED cosT = Trig_Cos(TRIG_TURN * 7 / 8);
      s32 t = frame - 782;
      s32 tile = gBattleEffect_MotePictures[5] + t / 6 * MOTE_SET_BYTES;

      for (i = 0; i != 197; i++) {
        s32 a = i * 384;
        FIXED sx = Trig_Sin(a) * (t >> 1);
        FIXED sy = Trig_Cos(a) * (t << 1);
        s32 px = FIXEDtoINT(Iwram_MulFX(sx, cosT) - Iwram_MulFX(sy, sinT));
        s32 py = FIXEDtoINT(Iwram_MulFX(sx, sinT) + Iwram_MulFX(sy, cosT));

        ((DrawRectangle)draw[0])(canvas, work->sheet + tile, px + 93, py + 32,
                                 6, 12);
      }
    }
    if (frame > 758) {
      void *verts = Heap_AllocEwram(64);
      struct RenderModel *model = Render_AllocateModelList(1);
      FIXED place[3];
      s32 grow[3];
      s32 s;

      coneTexture.logWidth = 5;
      coneTexture.logHeight = 5;
      coneTexture.pixels = work->sheet + IRIS_TEXTURES + 8 * IRIS_TEXTURE_BYTES;
      Matrix_ResetUnit();
      place[0] = toFIXED(32);
      place[1] = -toFIXED(24);
      place[2] = 0;
      Matrix_TranslateVector(place);
      Matrix_RotateZ(TRIG_TURN * 19 / 32);
      grow[0] = 0x10000;
      grow[1] = (frame - 758) << 10;
      if (grow[1] > 0x10000)
        grow[1] = 0x10000;
      grow[2] = 0;
      Matrix_ScaleVector(grow);
      model->kind = MODEL_TEXTURED_MAX;
      model->mesh = gIris_ConeMesh;
      model->texture = &coneTexture;
      model->vertices = verts;
      Render_TransformVertices(gIris_ConeCorners, verts, 8);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
      s = FindFreePetal();
      if (s != -1) {
        petals->life[s] = (Random16() & 15) << 15;
        petals->x[s] = toFIXED(239);
        petals->y[s] = INTtoFIXED(Random16() & 255);
        petals->vx[s] = -(Random16() & 31) << 12;
        petals->vy[s] = ((Random16() & 31) - 16) << 12;
      }
      if (!(frame & 3)) {
        s32 n = FindFreeParticle();

        if (n != -1) {
          s32 spin = Random16() & 3;

          work->particles[n].x = toFIXED(96);
          work->particles[n].y = INTtoFIXED(Random16() & 7) + toFIXED(36);
          work->particles[n].z = frame;
          work->particles[n].velocityX =
              Iwram_MulFX((spin + 2) << 15, Trig_Sin(TRIG_TURN * 19 / 32));
          work->particles[n].velocityY =
              -Iwram_MulFX((spin + 2) << 15, Trig_Cos(TRIG_TURN * 19 / 32));
          work->particles[n].variant = (Random16() & 3) | 16;
        }
        n = FindFreeParticle();
        if (n != -1) {
          work->particles[n].x = INTtoFIXED(Random16() & 31) + toFIXED(96);
          work->particles[n].y = INTtoFIXED(Random16() & 31);
          work->particles[n].z = frame;
          work->particles[n].velocityX = -((Random16() & 15) << 13);
          work->particles[n].velocityY = (Random16() & 15) << 14;
          work->particles[n].velocityZ = toFIXED(1) / 2;
          work->particles[n].variant = ((Random16() & 3) + 1) | 0x110;
        }
      }
    }
    if (frame > 782) {
      if (frame & 2)
        gBgScroll[1].y = gBgScroll[1].y + (frame & 1) + 2;
      else
        gBgScroll[1].y = gBgScroll[1].y - (frame & 1) - 2;
    }
    if (frame > 816) {
      {
        u16 *fade = FADE_PLTT;

        for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
          s32 c = *fade;
          s32 b = RGB_BLUE((u16)c);
          s32 g = RGB_GREEN((u16)c);
          s32 r = RGB_RED(c);

          if (r <= 30)
            r++;
          if (g <= 30)
            g++;
          if (b <= 30)
            b++;
          {
            u16 v = RGB(r, g, b);

            *fade++ = v;
          }
        }
      }
    }
    frame++;
    EFFECT_PRESENT_CANVAS(work);
  }

  camera[27] = savedYaw;
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Iris_RestoreBattle();
  Iwram_FillWords(canvas, CANVAS_BYTES, 0);
  while (((volatile struct DmaRegisters *)REG_DMA3)->control &
         (DMA_ENABLE << 16)) {
  }
  EFFECT_PRESENT_CANVAS(work);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  gBgScroll[1].x = savedScroll;
  for (i = 0; i != 36; i++)
    Sprite_DestroyFar(objects[i]);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Swaps two integers without a temporary.
#define XOR_SWAP(a, b)                                                         \
  {                                                                            \
    (a) ^= (b);                                                                \
    (b) ^= (a);                                                                \
    (a) ^= (b);                                                                \
  }

/// A triangle of the model: where its three corners start in the vertex
/// buffer (in words) and its colour.
struct SolidFace {
  s32 corner[3];
  u8 unread[12]; ///< the renderer skips them
  u8 color;
  u8 padding[3];
};

/// Fills one scanline of the canvas (8x8 tiles, 16 to a row) from xa to xb,
/// keeping the brighter of each pixel's colour and color.
#define FILL_SPAN(row, xa, xb)                                                 \
  {                                                                            \
    s32 left = FIXEDtoINT(xa);                                                 \
    s32 right = FIXEDtoINT(xb);                                                \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = (((row) & 7) << 3) + (((row) >> 3) << 10);                     \
    for (x = left; x != right; x++) {                                          \
      s32 pixel = ((x >> 3) << 6) + (x & 7) + rowOffset;                       \
                                                                               \
      if (canvas[pixel] < color)                                               \
        canvas[pixel] = color;                                                 \
    }                                                                          \
  }

/// An unused solid renderer, the filled twin of Render_DrawMeshWireframe.
/// Transforms the vertexCount points in the sheet block by the current matrix,
/// then fills every face of the effect work's list that faces the camera into
/// the canvas, centred at (64, 64), keeping the brighter colour where faces
/// overlap. Each triangle is sorted by y and filled in two halves, top to
/// middle and middle to bottom, along 16.16 edge slopes.
void Render_FillMesh(s32 vertexCount, s32 faceCount) {
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 *vertices = Ram_HeapSlots->battleSheet;
  struct SolidFace *faces = (struct SolidFace *)Ram_HeapSlots->battleEffect;
  s32 i;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 facing;
  s32 (*mul)(s32, s32);
  FIXED slopeA, slopeB;
  s32 top, middle, bottom;
  u8 color;
  FIXED edgeA, edgeB;
  s32 row;

  // From here on, the count of words: three to a point.
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // The three corners of each face are copied after the vertices.
    ax = (s32 *)(vertexCount * 4 + (s32)vertices);
    bx = ax + 3;
    cx = ax + 6;
    cy = ax + 7;
    ay = ax + 1;
    by = ax + 4;
    // A local holding the multiply routine's address (one value throughout).
    mul = Iwram_MulFX;
    Dma_Set(&vertices[faces[i].corner[0]], ax, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], bx, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], cx, DMA_COPY_WORDS(3), REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing = mul(*bx - *ax, *cy - *ay) - mul(*cx - *ax, *by - *ay);
    if (facing >= 0) {
      color = faces[i].color;
      *ax += toFIXED(64);
      *ay += toFIXED(64);
      *bx += toFIXED(64);
      *by += toFIXED(64);
      *cx += toFIXED(64);
      *cy += toFIXED(64);

      if (*ay > *by) {
        XOR_SWAP(*ax, *bx);
        *ay ^= *by;
        bx[1] = *by ^ *ay;
        *ay ^= bx[1];
      }
      if (*by > *cy) {
        XOR_SWAP(*bx, *cx);
        bx[1] = *by ^ *cy;
        cx[1] = *cy ^ bx[1];
        *by ^= cx[1];
        if (*ay > *by) {
          XOR_SWAP(*ax, *bx);
          *ay ^= *by;
          bx[1] = *by ^ *ay;
          *ay ^= bx[1];
        }
      }

      top = FIXEDtoINT(ax[1]);
      middle = FIXEDtoINT(ax[4]);
      bottom = FIXED_INTEGER_PART(ax[7]);
      if (top != middle)
        slopeA = Iwram_RatioMulQ14(ax[4] - ax[1], ax[3] - ax[0]);
      if (top != bottom)
        slopeB = Iwram_RatioMulQ14(ax[7] - ax[1], ax[6] - ax[0]);

      // The upper half, between the short edge and the long one.
      edgeB = ax[0];
      edgeA = ax[0];
      for (row = top; row != middle; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }

      // The lower half, from the middle corner.
      if (middle != bottom)
        slopeA = Iwram_RatioMulQ14(ax[7] - ax[4], ax[6] - ax[3]);
      edgeA = ax[3];
      for (row = middle; row != bottom; row++) {
        FILL_SPAN(row, edgeA, edgeB);
        edgeA += slopeA;
        edgeB += slopeB;
      }
    }
  }
}

/// One mesh face: its three corners as word offsets into the vertex buffer
/// (vertex index * 3), padded to an effect step's 28 bytes.
struct MeshFace {
  s32 vertex[3];
  s32 padding[4];
};

#define DIFF_ABS(v) ((v) >= 0 ? (v) : -(v))

/// One textured scanline at canvas row y, from xA to xB, keeping the brighter
/// of the canvas and texture colour.
#define DRAW_SPAN(y)                                                           \
  {                                                                            \
    u8 *row = work->sheet;                                                     \
    s32 left, right, u, v;                                                     \
    s32 rowOffset;                                                             \
    s32 x;                                                                     \
                                                                               \
    row += ((FIXEDtoINT(vA)) << textureShift) + (FIXEDtoINT(uA));              \
    left = FIXEDtoINT(xA);                                                     \
    right = FIXEDtoINT(xB);                                                    \
    u = 0;                                                                     \
    v = 0;                                                                     \
    if (left > right)                                                          \
      XOR_SWAP(left, right);                                                   \
    rowOffset = (((y) & 7) << 3) + (((y) >> 3) << 10);                         \
    for (x = left; x != right; x++) {                                          \
      s32 texel = (FIXEDtoINT(u)) + ((FIXEDtoINT(v)) << textureShift);         \
      s32 at = ((x >> 3) << 6) + (x & 7) + rowOffset;                          \
                                                                               \
      if (canvas[at] < row[texel])                                             \
        canvas[at] = row[texel];                                               \
      u += dudx;                                                               \
      v += dvdx;                                                               \
    }                                                                          \
  }

/// The texture steps along a scanline, from the middle corner's row.
#define STEP_RIGHTWARD()                                                       \
  {                                                                            \
    width = DIFF_ABS(ax[3] - ax[0] - Iwram_MulFX(dxB, ax[4] - ax[1]));         \
    dudx = Iwram_RatioMulQ14(width, uv[1][0] - uv[0][0] -                      \
                                        Iwram_MulFX(duB, ax[4] - ax[1]));      \
    dvdx = Iwram_RatioMulQ14(width, uv[1][1] - uv[0][1] -                      \
                                        Iwram_MulFX(dvB, ax[4] - ax[1]));      \
    XOR_SWAP(duA, duB);                                                        \
    XOR_SWAP(dvA, dvB);                                                        \
  }
#define STEP_LEFTWARD()                                                        \
  {                                                                            \
    width = DIFF_ABS(Iwram_MulFX(dxB, ax[4] - ax[1]) - (ax[3] - ax[0]));       \
    dudx = Iwram_RatioMulQ14(width, Iwram_MulFX(duB, ax[4] - ax[1]) -          \
                                        (uv[1][0] - uv[0][0]));                \
    dvdx = Iwram_RatioMulQ14(width, Iwram_MulFX(dvB, ax[4] - ax[1]) -          \
                                        (uv[1][1] - uv[0][1]));                \
  }

/// Transforms vertexCount vertices (16.16 x, y, z in the heap's vertex
/// buffer) in place, then draws each front-facing face of the mesh at the
/// start of the effect work as an affine-textured triangle: uvs gives each
/// face's three texture corners (16.16), the texture is the effect sheet,
/// textureShift its row length as a power of two. Coordinates are offset by
/// 64 pixels into the canvas.
void Render_DrawTexturedMesh(s32 vertexCount, s32 faceCount, s32 (*uvs)[3][2],
                             s32 textureShift) {
  s32 i;
  s32 *vertices;
  u8 *canvas;
  struct MeshFace *faces;
  s32 *ax, *bx, *cx, *cy, *ay, *by;
  s32 uv[3][2];
  s32 width;
  FIXED dxA, dxB, duA, dvA, duB, dvB;
  FIXED xA, xB, uA, vA, uB, vB;
  s32 top, middle, bottom;
  s32 dudx, dvdx;
  struct BattleEffectWork *work;
  u32 y;

  vertices = Ram_HeapSlots->battleSheet;
  faces = (struct MeshFace *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  for (i = 0; i != faceCount; i++) {
    // As an integer sum, so the scaled count is added first.
    ax = (s32 *)(vertexCount * 4 + (s32)vertices);
    bx = &vertices[vertexCount + 3];
    cx = &vertices[vertexCount + 6];
    cy = &vertices[vertexCount + 7];
    ay = &vertices[vertexCount + 1];
    by = &vertices[vertexCount + 4];
    Dma_Set(&vertices[faces[i].vertex[0]], ax,
            toFIXED(DMA_ENABLE | DMA_32BIT) | 3, REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[1]], bx,
            toFIXED(DMA_ENABLE | DMA_32BIT) | 3, REG_DMA3);
    Dma_Set(&vertices[faces[i].vertex[2]], cx,
            toFIXED(DMA_ENABLE | DMA_32BIT) | 3, REG_DMA3);

    // Back faces are skipped.
    if (Iwram_MulFX(*bx - *ax, *cy - *ay) - Iwram_MulFX(*cx - *ax, *by - *ay) >=
        0) {
      work = Ram_HeapSlots->battleEffect;
      Dma_Set(uvs[i], uv, toFIXED(DMA_ENABLE | DMA_32BIT) | 6, REG_DMA3);
      *ax += toFIXED(64);
      *ay += toFIXED(64);
      *bx += toFIXED(64);
      *by += toFIXED(64);
      *cx += toFIXED(64);
      *cy += toFIXED(64);

      // Sort the corners from top to bottom. bx[1] is *by and cx[1] is *cy.
      if (*ay > *by) {
        XOR_SWAP(*ax, *bx);
        *ay ^= *by;
        bx[1] = *by ^ *ay;
        *ay ^= bx[1];
        XOR_SWAP(uv[0][0], uv[1][0]);
        XOR_SWAP(uv[0][1], uv[1][1]);
      }
      if (*by > *cy) {
        XOR_SWAP(*bx, *cx);
        bx[1] = *by ^ *cy;
        cx[1] = *cy ^ bx[1];
        *by ^= cx[1];
        XOR_SWAP(uv[1][0], uv[2][0]);
        XOR_SWAP(uv[1][1], uv[2][1]);
        if (*ay > *by) {
          XOR_SWAP(*ax, *bx);
          *ay ^= *by;
          bx[1] = *by ^ *ay;
          *ay ^= bx[1];
          XOR_SWAP(uv[0][0], uv[1][0]);
          XOR_SWAP(uv[0][1], uv[1][1]);
        }
      }

      top = FIXEDtoINT(ax[1]);
      middle = FIXEDtoINT(ax[4]);
      bottom = FIXEDtoINT(ax[7]);
      if (top != middle) {
        s32 height = ax[4] - ax[1];

        dxA = Iwram_RatioMulQ14(height, ax[3] - ax[0]);
        duA = Iwram_RatioMulQ14(height, uv[1][0] - uv[0][0]);
        dvA = Iwram_RatioMulQ14(height, uv[1][1] - uv[0][1]);
      }
      if (top != bottom) {
        s32 height = ax[7] - ax[1];

        dxB = Iwram_RatioMulQ14(height, ax[6] - ax[0]);
        duB = Iwram_RatioMulQ14(height, uv[2][0] - uv[0][0]);
        dvB = Iwram_RatioMulQ14(height, uv[2][1] - uv[0][1]);
      }
      xB = xA = ax[0];
      uA = uB = uv[0][0];
      vA = vB = uv[0][1];

      if (ax[3] >= ax[6])
        STEP_RIGHTWARD()
      else
        STEP_LEFTWARD()

      // The upper half.
      for (y = top; y != middle; y++) {
        DRAW_SPAN(y);
        xA += dxA;
        xB += dxB;
        uA += duA;
        vA += dvA;
        uB += duB;
        vB += dvB;
      }

      // The lower half: edge A now runs from the middle corner to the bottom.
      if (middle != bottom) {
        s32 height = ax[7] - ax[4];

        dxA = Iwram_RatioMulQ14(height, ax[6] - ax[3]);
        if (ax[3] >= ax[6]) {
          duB = duA;
          dvB = dvA;
          uB = uA;
          vB = vA;
        }
        duA = Iwram_RatioMulQ14(height, uv[2][0] - uv[1][0]);
        dvA = Iwram_RatioMulQ14(height, uv[2][1] - uv[1][1]);
        xA = ax[3];
        uA = uv[1][0];
        vA = uv[1][1];
        if (ax[0] <= xA) {
          STEP_RIGHTWARD();
          uA = uB;
          vA = vB;
        } else
          STEP_LEFTWARD()
        for (y = middle; y != bottom; y++) {
          DRAW_SPAN(y);
          xA += dxA;
          xB += dxB;
          uA += duA;
          vA += dvA;
          /* FAKEMATCH(dead): edge B's values stepped on though nothing reads
           * them again; without them, 165 of 966 instructions differ in
           * tla-en. */
          uB += duB;
          vB += dvB;
        }
      }
    }
  }
}

/// A triangle of the wireframe model: where its three corners start in the
/// vertex buffer (in words) and the colour its edges are drawn in.
struct WireFace {
  s32 corner[3];
  u8 unread[12]; ///< the renderer skips them
  u8 color;
  u8 padding[3];
};

/// An unused wireframe renderer. Transforms the vertexCount points in the
/// sheet block by the current matrix, then draws the three edges of every
/// face of the effect work's list that faces the camera (its corners run
/// counter-clockwise on screen) as clipped lines into the canvas, centred at
/// (64, 64).
void Render_DrawMeshWireframe(s32 vertexCount, s32 faceCount) {
  s32 *vertices = Ram_HeapSlots->battleSheet;
  struct WireFace *faces = (struct WireFace *)Ram_HeapSlots->battleEffect;
  s32 i;
  s32 facing;
  s32 color;

  vertexCount *= 3;
  for (i = 0; i != vertexCount; i += 3)
    Iwram_TransformVector(&vertices[i], &vertices[i]);

  // The three corners of each face are copied after the vertices.
  for (i = 0; i != faceCount; i++) {
    // Spelled through integers: the index comes first in the addition.
    s32 *x0 = (s32 *)(vertexCount * 4 + (s32)vertices);
    s32 *x1 = x0 + 3;
    s32 *x2 = x0 + 6;
    s32 *y2 = x0 + 7;
    s32 *y0 = x0 + 1;
    s32 *y1 = x0 + 4;

    Dma_Set(&vertices[faces[i].corner[0]], x0, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[1]], x1, DMA_COPY_WORDS(3), REG_DMA3);
    Dma_Set(&vertices[faces[i].corner[2]], x2, DMA_COPY_WORDS(3), REG_DMA3);

    // The z of the edges' cross product: negative for a face turned away.
    facing =
        Iwram_MulFX(*x1 - *x0, *y2 - *y0) - Iwram_MulFX(*x2 - *x0, *y1 - *y0);
    if (facing >= 0) {
      color = faces[i].color;
      *x0 += toFIXED(64);
      *y0 += toFIXED(64);
      *x1 += toFIXED(64);
      *y1 += toFIXED(64);
      *x2 += toFIXED(64);
      *y2 += toFIXED(64);
      BattleEffect_DrawClippedCanvasLine(
          FIXED_INTEGER_PART(*x0), FIXED_INTEGER_PART(*y0),
          FIXED_INTEGER_PART(*x1), FIXED_INTEGER_PART(*y1), color);
      BattleEffect_DrawClippedCanvasLine(
          FIXED_INTEGER_PART(*x1), FIXED_INTEGER_PART(*y1),
          FIXED_INTEGER_PART(*x2), FIXED_INTEGER_PART(*y2), color);
      BattleEffect_DrawClippedCanvasLine(
          FIXED_INTEGER_PART(*x0), FIXED_INTEGER_PART(*y0),
          FIXED_INTEGER_PART(*x2), FIXED_INTEGER_PART(*y2), color);
    }
  }
}

/// BattleEffect_BuildBlitter(slot, 7, 7, flags, mode) for a 128 by 128 canvas,
/// with the flags and the mode packed into one word: mode << 4 | flags.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags) {
  return BattleEffect_BuildBlitter(slot, 7, 7, modeAndFlags & 0xf,
                                   (u32)modeAndFlags >> 4);
}

// BattleEffect_BuildBlitter walks The Lost Age's blitter template once and
// writes the blitter's ARM code into a heap slot: a list of steps with short
// copies as block moves and longer ones by DMA, modes 0 additive, 1 max, 2
// transparent, default copy, and the second run copied from the first by its
// measured length.

CAMELOT_ASM("ldmia and stmia of two registers from r0 to r1 with scratch from "
            "r2 up, in the shape of Dma_Set: agscc's block moves leave r0 and "
            "r1 to the allocator")
static __inline__ void CopyWords2(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3}\n\tstmia %1!, {r2, r3}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "memory");
}

CAMELOT_ASM("ldmia and stmia of three registers from r0 to r1 with scratch "
            "from r2 up, in the shape of Dma_Set: agscc's block moves leave r0 "
            "and r1 to the allocator")
/// Copies three words from source to destination with one block load and store.
static __inline__ void CopyWords3(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4}\n\tstmia %1!, {r2, r3, r4}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "memory");
}

CAMELOT_ASM("ldmia and stmia of four registers: agscc's block moves stop at "
            "three, so a 16-byte struct copy compiles to 12 and 4")
/// Copies four words from source to destination with one block load and store.
static __inline__ void CopyWords4(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}

#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

#define PUT(word)                                                              \
  do {                                                                         \
    *dst++ = (word);                                                           \
  } while (0)
#define COPY_FROM(from, n)                                                     \
  CopyWords##n(from, dst);                                                     \
  dst += (n)
#define COPY(n)                                                                \
  do {                                                                         \
    COPY_FROM(src, n);                                                         \
    src += (n);                                                                \
  } while (0)
#define COPY_LONG(n)                                                           \
  do {                                                                         \
    Dma_Set(src, dst, control | (n), REG_DMA3);                                \
    dst += (n);                                                                \
    src += (n);                                                                \
  } while (0)
#define COPY_DMA(from, n)                                                      \
  Dma_Set(from, dst, DMA_COPY_WORDS(n), REG_DMA3);                             \
  dst += (n)
#define COPY_TWICE(from, n)                                                    \
  {                                                                            \
    const u32 *part = (from);                                                  \
    u32 control = DMA_COPY_WORDS(n);                                           \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
  }
#define PUT_BRANCH(target)                                                     \
  *dst = *src++ + BRANCH_FIELD(dst, target);                                   \
  dst++
#define PATCH(site)                                                            \
  do {                                                                         \
    *(site) |= BRANCH_FIELD(site, dst);                                        \
  } while (0)

/// Writes a drawing routine for the canvas into heap slot `slot`, built from
/// the template gBlitterTemplate for one clip, mirroring and blend mode.
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode) {
  const u32 *src;
  u32 *dst;
  u32 *noRows;
  u32 *noColumns;
  u32 *row;
  u32 *skip;
  u32 *run;
  s32 runSize;
  u32 *runCopy;
  u32 control;
  // Unused: gives the stack frame its 136 bytes.
  u32 frame[34];

  src = BattleEffect_PatchTemplateCode;
  dst = (u32 *)436;
  dst = HeapSlot_Alloc(slot, (s32)dst);

  COPY(4);
  if (mode == 0) {
    COPY_FROM(BattleEffect_Mask, 3);
  }

  if ((flags & 12) == 4) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & 12) == 8) {
    COPY_FROM(src, 4);
  }
  src += 4;
  if ((flags & 12) == 12) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & 12) == 0) {
    PUT(src[0]);
  }
  src += 1;

  if ((flags & 2) != 0) {
    COPY(2);
    if ((flags & 8) != 0) {
      PUT(src[0]);
    } else {
      PUT(src[1]);
    }
    src += 2;
    COPY(2);
    PUT(src[0] + (1 << heightShift));
    PUT(src[1] + (1 << heightShift));
    src -= 6;
  }
  src += 8;

  PUT(*src++);
  noRows = dst;
  PUT(*src++);

  if ((flags & 1) == 0) {
    s32 width = 1 << widthShift;

    PUT(src[0] + width - 1);
  }
  src += 1;
  if ((flags & 1) != 0) {
    PUT(src[0]);
    PUT(src[1]);
    if ((flags & 4) != 0) {
      PUT(src[2]);
      PUT(src[3]);
    } else {
      PUT(src[4]);
      PUT(src[5]);
    }
    PUT(src[6]);
    PUT(src[7]);
    PUT(src[8] + Data_081973f0[widthShift]);
    if ((flags & 4) != 0) {
      PUT(src[9]);
    } else {
      PUT(src[10]);
    }
    PUT(src[11] + Data_081973f0[widthShift]);
  }
  src += 12;

  PUT(*src++);
  noColumns = dst;
  PUT(*src++);

  control = DMA_COPY_WORDS(0);
  COPY_LONG(6);

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  COPY_LONG(5);

  row = dst;
  PUT(src[0]);
  PUT(src[1] + (1 << (heightShift - 3)) - 1);
  PUT(src[2] + ((widthShift - 3) << 7));
  PUT(src[3]);
  src += 4;

  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  skip = dst + 1;
  COPY(2);
  if ((flags & 4) != 0) {
    PUT(src[0]);
  } else {
    PUT(src[1]);
  }
  src += 2;

  run = dst;
  runCopy = dst;
  switch (mode) {
  default: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_CopyReverse;
    } else {
      part = BattleEffect_CopyForward;
    }
    COPY_FROM(part, 2);
    break;
  }
  case 2: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_TransparentReverse;
    } else {
      part = BattleEffect_TransparentForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case 1: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_MaxReverse;
    } else {
      part = BattleEffect_MaxForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case 0:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_AdditiveReverse, 6);
    } else {
      COPY_DMA(BattleEffect_AdditiveForward, 6);
    }
    break;
  }

  PUT(*src++);
  PUT_BRANCH(run);
  runSize = (u8 *)dst - (u8 *)runCopy;
  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;
  PATCH(skip);

  skip = dst + 1;
  COPY(2);

  run = dst;
  switch (mode) {
  default:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Copy8Reverse, 16);
    } else {
      COPY_DMA(BattleEffect_Copy8Forward, 16);
    }
    break;
  case 2:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Transparent8Reverse, 25);
    } else {
      COPY_DMA(BattleEffect_Transparent8Forward, 25);
    }
    break;
  case 1:
    if ((flags & 4) != 0) {
      COPY_TWICE(BattleEffect_Max4Reverse, 16);
    } else {
      COPY_TWICE(BattleEffect_Max4Forward, 16);
    }
    break;
  case 0:
    if ((flags & 4) != 0) {
      COPY_TWICE(BattleEffect_ColorReverse, 14);
    } else {
      COPY_TWICE(BattleEffect_ColorForward, 14);
    }
    break;
  }

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;

  PUT(*src++);
  PUT_BRANCH(run);
  PATCH(skip);

  PUT(*src++);
  skip = dst;
  PUT(*src++);
  if ((flags & 4) != 0) {
    PUT(src[0]);
  } else {
    PUT(src[1]);
  }
  src += 2;
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);

  {
    u32 words = (u32)runSize / 4;
    /* FAKEMATCH(pin): count fixed in r2, where the rounded size is built. */
    register s32 count __asm__("r2") = words * 4;
    s32 size = count;

    /* FAKEMATCH(barrier): an empty asm. */
    __asm__("" : : "r"(words));
    Dma_Set(runCopy, dst, DMA_COPY_WORDS(count / 4), REG_DMA3);
    dst = (u32 *)((u8 *)dst + size);
  }
  PATCH(skip);

  COPY(3);

  PUT_BRANCH(row);
  PATCH(noRows);
  PATCH(noColumns);

  COPY(2);
  {
    const u32 *end = (const u32 *)Data_081973f0;

    return src == end;
  }
}

// The same wrapper as Render_TransformVertices (below) and Message_Find, with
// two arguments. Nothing in the image calls it.

/// Copies Render_FillMeshKernel (render_fill_mesh_kernels.s), an ARM build of
/// Render_FillMesh: the same transform of vertexCount points centred at
/// (64, 64), back faces dropped and each triangle sorted by y with XOR swaps,
/// into a heap block (DMA 3, words), runs it there and frees the block.
void Render_RunFillMeshKernel(s32 vertexCount, s32 faceCount) {
  void (*routine)(s32, s32);

  RamCode_Load(routine, Render_FillMeshKernel, Render_FillMeshKernelCodeSize);
  routine(vertexCount, faceCount);
  Heap_Release(routine);
}

/// Copies the ARM vertex kernel into a heap block and runs it: count packed
/// s8 vertices through the current transform into screen coordinates.
void Render_TransformVertices(const void *corners, void *vertices, s32 count) {
  void (*routine)(const void *, void *, s32);

  RamCode_Load(routine, Render_TransformKernel, Render_TransformKernelCodeSize);
  routine(corners, vertices, count);
  Heap_Release(routine);
}

/// Projects count model vertices through Render_ProjectPoint into screen
/// coordinates.
void Render_ProjectVertices(struct ModelVertex *vertices,
                            struct ProjectedVertex *screen, s32 count) {
  s32 point[3];

  for (; count != 0; count--) {
    SET_VECTOR(point, INTtoFIXED(vertices->x), INTtoFIXED(vertices->y),
               INTtoFIXED(vertices->z));
    Render_ProjectPoint(point, point);
    screen->x = point[0];
    screen->y = point[1];
    screen->depth = point[2];
    vertices++;
    screen++;
  }
}

/// A block of the render pool, as Render_AllocateModelList hands it out.
struct PoolEntry {
  s32 head;
  s32 next;
  u8 unused[16]; ///< nothing reads or writes them
  s32 tail;
};

/// Allocates count model entries plus a cleared one that ends the list.
struct PoolEntry *Render_AllocateModelList(s32 count) {
  struct PoolEntry *entries =
      Heap_AllocEwram((count + 1) * sizeof(struct PoolEntry));
  s32 i;

  for (i = 0; i < count + 1; i++) {
    entries[i].head = 0;
    entries[i].next = 0;
    entries[i].tail = 0;
  }
  return entries;
}

/// Copies a texture's pixels into a heap block when IWRAM has room for them;
/// returns 1 with copy set up, 0 when there is no room.
s32 Render_CopyTextureToIwram(struct Texture *source, struct Texture *copy) {
  s32 size = (1 << source->logWidth) << source->logHeight;
  u32 pixels;
  s32 misalign;

  if (Heap_GetRemainingIwram() < size + 8)
    return 0;
  *(u16 *)copy = *(u16 *)source;
  copy->pixels = Heap_Alloc(size + 8);
  pixels = (u32)source->pixels;
  misalign = pixels & 3;
  Iwram_CopyWordsEntry(copy->pixels, (void *)(pixels - misalign),
                       (size + misalign + 3) & -4);
  copy->pixels = (u8 *)copy->pixels + misalign;
  return 1;
}

/// The texture stays where it is, even when IWRAM has room for a copy.
#define MODEL_TEXTURE_IN_PLACE 4

typedef void (*ModelKernel)(struct RenderModel *model);

/// Copies a kernel whose DMA control is a word of its own.
#define LOAD_KERNEL_WORDS(to, code, bytes, words)                              \
  {                                                                            \
    (to) = Heap_Alloc((u32)(bytes));                                           \
    Dma_Set((code), (to), DMA_COPY_WORDS(0) + (u32)(words), REG_DMA3);         \
  }

/// Copies the model's texture into IWRAM unless it is to stay in place.
#define COPY_TEXTURE()                                                         \
  {                                                                            \
    if (!(model->flags & MODEL_TEXTURE_IN_PLACE) &&                            \
        Render_CopyTextureToIwram(texture, &copy))                             \
      model->texture = &copy;                                                  \
  }

/// Draws a list of models. A run of models of one kind loads that kind's
/// rasterizer into IWRAM once, and it is freed when the kind changes. Before
/// a textured model is drawn, its texture is copied into IWRAM where there is
/// room, and the copy is freed after the model is drawn.
void Render_DrawModelList(struct RenderModel *model) {
  ModelKernel code = NULL;
  s32 lastKind = 0;
  struct Texture copy;
  struct Texture *texture;

  for (;;) {
    texture = model->texture;
    if (model->kind != lastKind && code != NULL)
      Heap_Release(code);
    if (model->kind == MODEL_END)
      break;
    switch (model->kind) {
    case MODEL_SOLID:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidKernel, Render_SolidKernelCodeSize);
      code(model);
      break;
    case MODEL_SOLID_NARROW:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidNarrowKernel,
                     Render_SolidNarrowKernelCodeSize);
      code(model);
      break;
    case MODEL_SOLID_MAX:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidMaxKernel,
                     Render_SolidMaxKernelCodeSize);
      code(model);
      break;
    case MODEL_SOLID_ADDITIVE:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_SolidAdditiveKernel,
                     Render_SolidAdditiveKernelCodeSize);
      code(model);
      break;
    case MODEL_TEXTURED:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Render_TexturedKernel,
                          Render_TexturedKernelCodeSize,
                          Render_TexturedKernelCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_SHADED:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Render_ShadedKernel,
                          Render_ShadedKernelCodeSize,
                          Render_ShadedKernelCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_TEXTURED_NARROW:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Render_TexturedNarrowKernel,
                          Render_TexturedNarrowKernelCodeSize,
                          Render_TexturedNarrowKernelCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_TEXTURED_MAX:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_TexturedMaxKernel,
                     Render_TexturedMaxKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_TEXTURED_ADDITIVE:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_TexturedAdditiveKernel,
                     Render_TexturedAdditiveKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_SHADED_MAX:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_ShadedMaxKernel,
                     Render_ShadedMaxKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case MODEL_SHADED_ADDITIVE:
      if (model->kind != lastKind)
        RamCode_Load(code, Render_ShadedAdditiveKernel,
                     Render_ShadedAdditiveKernelCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    }
    lastKind = model->kind;
    if (model->texture == &copy) {
      Heap_Release((void *)((u32)model->texture->pixels & ~3));
      model->texture = texture;
    }
    model++;
  }
}
#undef MOLOCH_FRAMES
#undef MOLOCH_SKIP_TO
#undef MOLOCH_MOTES
#undef MOLOCH_FLAMES
#undef MOLOCH_FLAME_LEFT
#undef MOLOCH_FIRST_EMBER
#undef MOLOCH_EMBERS
#undef MOLOCH_SPRITES
#undef MOLOCH_FLAME_LIFE
#undef MOLOCH_RING_TEXTURE
#undef IRIS_MOTE_FADES
#undef IRIS_DROPS
#undef IRIS_DROP_WIDTH
#undef IRIS_DROP_HEIGHT
#undef IRIS_SMALL_DROPS
#undef IRIS_SMALL_DROP_WIDTH
#undef IRIS_SMALL_DROP_HEIGHT
#undef IRIS_DROP_PICTURES
#undef IRIS_TEXTURES
#undef IRIS_TEXTURE_WIDTH
#undef IRIS_TEXTURE_HEIGHT
#undef IRIS_TEXTURE_BYTES
#undef IRIS_PILLAR_TEXTURE
#undef IRIS_PILLAR_TEXTURE_SIZE
#undef IRIS_PILLAR_CIRCLE
#undef IRIS_LARGE_RING_WIDTH
#undef IRIS_LARGE_RING_HEIGHT
#undef IRIS_SMALL_RING_WIDTH
#undef IRIS_SMALL_RING_HEIGHT
#undef IRIS_DOT_WIDTH
#undef IRIS_DOT_HEIGHT
#undef IRIS_SPRAY_WIDTH
#undef IRIS_SPRAY_HEIGHT
#undef BLITTER
#undef HALF
#undef IRIS_PETALS
#undef IRIS_PETAL_FRAMES
#undef IRIS_MOTES
#undef IRIS_STREAKS
#undef IRIS_STREAK_LENGTH
#undef IRIS_GODDESS_COLUMNS
#undef IRIS_GODDESS_ROWS
#undef IRIS_GODDESS_SPRITES
#undef IRIS_GODDESS_TILE
#undef IRIS_FADE_COLORS
#undef IRIS_PETAL_WORK
#undef IRIS_SCENE
#undef IRIS_STREAK_POINTS
#undef SCENE
#undef STREAK
#undef FADE_PLTT
#undef DECOMPRESS_IN_BLOCK
#undef XOR_SWAP
#undef FILL_SPAN
#undef DIFF_ABS
#undef DRAW_SPAN
#undef STEP_RIGHTWARD
#undef STEP_LEFTWARD
#undef BRANCH_FIELD
#undef PUT
#undef COPY_FROM
#undef COPY
#undef COPY_LONG
#undef COPY_DMA
#undef COPY_TWICE
#undef PUT_BRANCH
#undef PATCH
#undef MODEL_TEXTURE_IN_PLACE
#undef LOAD_KERNEL_WORDS
#undef COPY_TEXTURE
