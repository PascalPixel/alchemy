/// @file
/// Vengeance and Life Shear, a geyser, swarms of stars and a column of bubbles,
/// and the Coatlicue summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The star swarm's modes (BattleEffect_RunStarSwarm).
enum StarSwarmMode { STAR_SWARM_DJINN_STUN, STAR_SWARM_MYSTIC_CALL };

/// The geyser's modes (BattleEffect_RunGeyser), each named for the effect that
/// plays it.
enum GeyserMode { GEYSER_PURGATORY, GEYSER_DJINN_STORM, GEYSER_DJINN_BLAST };

// Vengeance: the archer's flaming arrows.

struct StripQuad;

#define VENGEANCE_FRAMES 124
#define VENGEANCE_ARROWS 7 ///< arrows shot, one every twelve frames
/// Vengeance's sheet: the archer twice, her bow at rest and drawn, then the
/// flame at an arrow's tail, the flaming arrow and a glint.
#define VENGEANCE_ARCHER_WIDTH 88
#define VENGEANCE_ARCHER_HEIGHT 96
#define VENGEANCE_ARCHER_DRAWN PICTURE_BYTES(VENGEANCE_ARCHER)
#define VENGEANCE_FLAME_TIP (2 * PICTURE_BYTES(VENGEANCE_ARCHER))
#define VENGEANCE_FLAME_TIP_WIDTH 17
#define VENGEANCE_FLAME_TIP_HEIGHT 12
#define VENGEANCE_ARROW                                                        \
  (VENGEANCE_FLAME_TIP + PICTURE_BYTES(VENGEANCE_FLAME_TIP))
#define VENGEANCE_ARROW_WIDTH 51
#define VENGEANCE_ARROW_HEIGHT 12
#define VENGEANCE_GLINT (VENGEANCE_ARROW + PICTURE_BYTES(VENGEANCE_ARROW))
#define VENGEANCE_GLINT_WIDTH 14
#define VENGEANCE_GLINT_HEIGHT 13

/// An archer slides in from the far side on a window of her own and from frame
/// 22 looses a flaming arrow every twelve frames; seven fly across, and those
/// the level picks strike the target, the last one hardest. Everything is
/// mirrored on the right side.
///
/// Abilities: Vengeance.
void BattleEffect_RunVengeance(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  // The plain and the flipped blitter, as one pair: as two locals the
  // arrow loop no longer hoists them from memory.
  DrawRectangle draw[2];
  struct ScreenPoint pos;
  struct Particle *arrow;
  FIXED backgroundX;
  s32 speed;
  s32 drawn;
  s32 row;
  s32 column;
  s32 i;
  s32 strikes;
  s32 mask;
  s32 frames;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  mask = 7;
  BattleEffect_StartTiledCanvas(0);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 4);

  // The archer's tile map: her body, and her arms in rows 3 to 6.
  i = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 16; column++, i += 2) {
      s32 tileX;
      s32 tileY;
      s32 tile;
      s16 entry;

      // The arms take the left half on the enemies' side and the right half on
      // the party's; the body takes the other half.
      if (args->side == EFFECT_SIDE_ENEMIES && column <= 7) {
        if ((u32)(row - 3) > 3) {
          tileX = 0;
          tileY = 0;
        } else {
          tileX = column;
          tileY = row + 9;
        }
      } else if (args->side == EFFECT_SIDE_ENEMIES || column <= 7) {
        if (row > 11) {
          tileX = 0;
          tileY = 0;
        } else {
          tileX = column & mask;
          tileY = row;
        }
      } else if ((u32)(row - 3) <= 3) {
        tileX = column & mask;
        tileY = row + 9;
      } else {
        tileX = 0;
        tileY = 0;
      }
      tile = (tileY * 8 + tileX) * 2;
      entry = ((tile + 1) << 8) | tile;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + i) = entry;
    }
  }
  REG_BG2Y = -(40 << 8);
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(40, 136);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(40, 136);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_OBJ |
               WINOUT_WIN01_CLR | WINOUT_WINOBJ_CLR;
  Resource_LoadAndDecompress(RESOURCE_ARCHER_SHEET, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  backgroundX = toFIXED(86);
  speed = toFIXED(16);
  frames = VENGEANCE_FRAMES;

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    EffectPosition_ProjectScaledBattler(args->targets[0], &pos);
    if (frame <= 31) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        camera->yaw = (s16)camera->yaw - 128;
      } else {
        camera->yaw = (s16)camera->yaw + 128;
      }
    }
    if (frame >= frames - 16) {
      REG_BLDALPHA = BLEND_ALPHA(frames - frame - 1, BLEND_WEIGHT_MAX);
    }
    if (frame == 0) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        backgroundX = toFIXED(86);
        speed = toFIXED(16);
      } else {
        backgroundX = 0xffaa0000;
        speed = 0xfff00000;
      }
      for (i = 0; i != VENGEANCE_ARROWS; i++) {
        arrow = &work->particles[i];
        if (args->side == EFFECT_SIDE_ENEMIES) {
          arrow->x = toFIXED(160);
          arrow->velocityX = toFIXED(12);
        } else {
          arrow->x = toFIXED(32);
          arrow->velocityX = -toFIXED(12);
        }
        arrow->y = toFIXED(12);
        strikes = gVengeance_BladeStrikes[args->variant][i];
        if (strikes != 0) {
          strikes = 1;
        }
        arrow->z = strikes;
        arrow->velocityY = (Random16() & TRIG_ANGLE_MASK) - 0x8000;
        arrow->velocityZ = 0;
        arrow->variant = 0;
      }
    }

    // The archer slides in, her window following.
    if (frame <= 24) {
      REG_BG2X = -(FIXEDtoINT(backgroundX)) << 8;
      if (args->side == EFFECT_SIDE_ENEMIES) {
        REG_WIN0H = WIN_RANGE(FIXEDtoINT(backgroundX) + 120, DISPLAY_WIDTH);
        REG_WIN1H = WIN_RANGE(FIXEDtoINT(backgroundX) + 120, DISPLAY_WIDTH);
      } else {
        REG_WIN0H = FIXEDtoINT(backgroundX) + DISPLAY_WIDTH;
        REG_WIN1H = FIXEDtoINT(backgroundX) + DISPLAY_WIDTH;
      }
    } else {
      REG_BG2X = -(FIXEDtoINT(backgroundX)) << 8;
      REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
      REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
    }
    backgroundX -= speed;
    speed = speed * 52 / DAMPING_SCALE;

    drawn = 0;
    if (frame > 105) {
      drawn = 1;
    } else if (frame > 21 && (frame - 22) % 12 > 3) {
      drawn = 1;
    }
    if (args->side == EFFECT_SIDE_ENEMIES) {
      if (drawn == 0) {
        draw[0](canvas, work->sheet, 32, 0, VENGEANCE_ARCHER_WIDTH,
                VENGEANCE_ARCHER_HEIGHT);
      } else {
        draw[0](canvas, work->sheet + VENGEANCE_ARCHER_DRAWN, 32, 0,
                VENGEANCE_ARCHER_WIDTH, VENGEANCE_ARCHER_HEIGHT);
      }
    } else if (drawn == 0) {
      draw[1](canvas, work->sheet, 8, 0, VENGEANCE_ARCHER_WIDTH,
              VENGEANCE_ARCHER_HEIGHT);
    } else {
      draw[1](canvas, work->sheet + VENGEANCE_ARCHER_DRAWN, 8, 0,
              VENGEANCE_ARCHER_WIDTH, VENGEANCE_ARCHER_HEIGHT);
    }

    // The arrows.
    for (i = 0; i != VENGEANCE_ARROWS; i++) {
      arrow = &work->particles[i];
      if (frame < i * 12 + 22 && (frame > 21 || i != 0)) {
        continue;
      }
      if (frame >= i * 12 + 26 && arrow->variant == 0) {
        arrow->x -= arrow->velocityX;
        arrow->y += arrow->velocityY;
      }
      if (arrow->variant > 4) {
        continue;
      }
      if (arrow->variant > 0) {
        arrow->variant++;
      }
      if (arrow->z == 1 && arrow->variant == 0) {
        s32 hit = 0;

        if (args->side == EFFECT_SIDE_ENEMIES) {
          if ((FIXEDtoINT(arrow->x)) < pos.x / 2 + 16) {
            hit = 1;
          }
        } else if ((FIXEDtoINT(arrow->x)) > pos.x / 2 + 48) {
          hit = 1;
        }
        if (hit == 1) {
          work->shakeFrames = 4;
          if (i == 6) {
            BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2),
                                          toFIXED(8), toFIXED(2), i * 10 + 110);
            Sound_PlayFar(SE_IMPACT);
          }
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 4);
          arrow->variant = 1;
          if (args->side == EFFECT_SIDE_ENEMIES) {
            arrow->x = pos.x / 2 - FIXED_INTEGER_PART(arrow->x);
          } else {
            arrow->x = pos.x / 2 - FIXED_INTEGER_PART(arrow->x) + 128;
          }
        }
      }
      if (args->side == EFFECT_SIDE_ENEMIES) {
        if (arrow->variant == 0) {
          draw[0](canvas, work->sheet + VENGEANCE_ARROW,
                  (FIXEDtoINT(arrow->x)) - 111,
                  FIXED_INTEGER_PART(arrow->y) + 24, VENGEANCE_ARROW_WIDTH,
                  VENGEANCE_ARROW_HEIGHT);
          draw[0](canvas, work->sheet + VENGEANCE_ARROW,
                  FIXED_INTEGER_PART(arrow->x) + 17,
                  FIXED_INTEGER_PART(arrow->y) + 96, VENGEANCE_ARROW_WIDTH,
                  VENGEANCE_ARROW_HEIGHT);
          draw[0](canvas, work->sheet + VENGEANCE_FLAME_TIP,
                  FIXED_INTEGER_PART(arrow->x) - 128,
                  FIXED_INTEGER_PART(arrow->y) + 24, VENGEANCE_FLAME_TIP_WIDTH,
                  VENGEANCE_FLAME_TIP_HEIGHT);
          draw[0](canvas, work->sheet + VENGEANCE_FLAME_TIP,
                  FIXED_INTEGER_PART(arrow->x),
                  FIXED_INTEGER_PART(arrow->y) + 96, VENGEANCE_FLAME_TIP_WIDTH,
                  VENGEANCE_FLAME_TIP_HEIGHT);
        } else {
          draw[0](canvas, work->sheet + VENGEANCE_ARROW,
                  pos.x / 2 + arrow->x - 111, FIXED_INTEGER_PART(arrow->y) + 24,
                  VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
          draw[0](canvas, work->sheet + VENGEANCE_ARROW,
                  pos.x / 2 + arrow->x + 17, FIXED_INTEGER_PART(arrow->y) + 96,
                  VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
        }
      } else if (arrow->variant == 0) {
        draw[1](canvas, work->sheet + VENGEANCE_ARROW, FIXEDtoINT(arrow->x),
                FIXED_INTEGER_PART(arrow->y) + 24, VENGEANCE_ARROW_WIDTH,
                VENGEANCE_ARROW_HEIGHT);
        draw[1](canvas, work->sheet + VENGEANCE_ARROW,
                FIXED_INTEGER_PART(arrow->x) - 128,
                FIXED_INTEGER_PART(arrow->y) + 96, VENGEANCE_ARROW_WIDTH,
                VENGEANCE_ARROW_HEIGHT);
        draw[1](canvas, work->sheet + VENGEANCE_FLAME_TIP,
                FIXED_INTEGER_PART(arrow->x) + 51,
                FIXED_INTEGER_PART(arrow->y) + 24, VENGEANCE_FLAME_TIP_WIDTH,
                VENGEANCE_FLAME_TIP_HEIGHT);
        draw[1](canvas, work->sheet + VENGEANCE_FLAME_TIP,
                FIXED_INTEGER_PART(arrow->x) - 77,
                FIXED_INTEGER_PART(arrow->y) + 96, VENGEANCE_FLAME_TIP_WIDTH,
                VENGEANCE_FLAME_TIP_HEIGHT);
      } else {
        draw[1](canvas, work->sheet + VENGEANCE_ARROW, pos.x / 2 + arrow->x,
                FIXED_INTEGER_PART(arrow->y) + 24, VENGEANCE_ARROW_WIDTH,
                VENGEANCE_ARROW_HEIGHT);
        draw[1](canvas, work->sheet + VENGEANCE_ARROW,
                pos.x / 2 + arrow->x - 128, FIXED_INTEGER_PART(arrow->y) + 96,
                VENGEANCE_ARROW_WIDTH, VENGEANCE_ARROW_HEIGHT);
      }
    }

    // The glint, while the bow is at rest.
    if (drawn == 0) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet, VENGEANCE_GLINT, 88,
                            29);
      } else {
        EFFECT_DRAW_PICTURE(draw[1], canvas, work->sheet, VENGEANCE_GLINT, 26,
                            29);
      }
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(1, 4);
    EFFECT_PRESENT_CANVAS(work);
  }
  REG_WIN0V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
  REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
  EFFECT_FINISH_CANVAS();
}

/// The scythe, in the work's first particle: swing is its velocityZ and timer
/// its variant.
struct Scythe {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 swing;
  s32 timer;
};

#define LIFE_SHEAR_FRAMES 90
/// Life Shear's pictures in the map cell buffer: the reaper's raised pose, and
/// the textures of the scythe's blade, its handle and its flash.
#define LIFE_SHEAR_REAPER_RAISED (Ram_MapCellBuffer + 0x12e8)
#define LIFE_SHEAR_BLADE_TEXTURE (Ram_MapCellBuffer + 0x276b)
#define LIFE_SHEAR_HANDLE_TEXTURE (Ram_MapCellBuffer + 0x476b)
#define LIFE_SHEAR_FLASH_TEXTURE (Ram_MapCellBuffer + 0x576b)

/// The reaper (a tiled background picture) glides in from the far side and
/// swings its scythe (a two-part model) through the target at frame 50, with
/// a four-blade flash; on the right side everything is mirrored. From frame
/// 62 the reaper drifts off and the scythe falls away.
///
/// Abilities: Life Shear.
void BattleEffect_RunLifeShear(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit[2];
  struct ScreenPoint target;
  struct ModelTexture texture;
  FIXED reaperX;
  FIXED reaperY;
  s32 speed;
  struct ScreenPoint *position;
  s32 shift;
  s32 frame;
  void *vertices;
  struct RenderModel *model;
  struct Scythe *scythe;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  if (args->side == EFFECT_SIDE_ENEMIES) {
    BattleEffect_StartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
  } else {
    BattleEffect_StartTiledCanvasFilled(BGCNT_WRAP | BGCNT_PRIORITY(1));
  }
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 6);
  Resource_LoadAndDecompress(RESOURCE_LIFE_SHEAR_SHEET, gMapGridCellBuffer, 1,
                             1);
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_TRANSPARENT);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_TRANSPARENT);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  reaperX = 0;
  reaperY = 0;
  speed = 0;
  EffectPosition_ProjectScaledBattler(args->targets[0], position = &target);

  for (frame = 0; frame != LIFE_SHEAR_FRAMES; frame++) {
    s32 x;
    s32 phase;
    s32 scale;
    s32 timer;
    s32 zero;
    s32 still;
    s32 width;

    shift = 0;
    if (frame == 0) {
      Sound_PlayFar(SE_CURSE_FALL);
      if (args->side == EFFECT_SIDE_ENEMIES) {
        work->particles[0].x = toFIXED(32);
        work->particles[0].y = toFIXED(65472);
        work->particles[0].z = 0xffffe3d0;
      } else {
        work->particles[0].x = toFIXED(65504);
        work->particles[0].y = toFIXED(65472);
        work->particles[0].z = 0xffffe3d0;
      }
      work->particles[0].velocityZ = 0;
      work->particles[0].variant = 0;
      reaperY = toFIXED(24);
      if (args->side == EFFECT_SIDE_ENEMIES) {
        reaperX = INTtoFIXED(position->x + 176);
        speed = toFIXED(15);
      } else {
        reaperX = INTtoFIXED(position->x - 304);
        speed = 0xfff10000;
      }
    }
    if (frame > 61) {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        reaperX += toFIXED(2);
      } else {
        reaperX -= toFIXED(2);
      }
      reaperY -= 0x4aaaa;
    }
    reaperX -= speed;
    speed = speed * 60 / DAMPING_SCALE;
    x = FIXEDtoINT(reaperX);
    if (x > 120) {
      shift = x - 120;
      x = 120;
    } else if (x < 0) {
      shift = x;
      x = 0;
    }
    REG_BG2X = -x << 8;
    if ((u32)(frame - 24) <= 39) {
      x = args->side == EFFECT_SIDE_ENEMIES ? 64 : 5;
      blit[0](canvas, LIFE_SHEAR_REAPER_RAISED, x + shift, FIXEDtoINT(reaperY),
              59, 89);
    } else {
      if (args->side == EFFECT_SIDE_ENEMIES) {
        x = frame * 2 + 16;
        if (x > 64) {
          x = 64;
        }
      } else {
        x = 57 - frame * 2;
        if (x <= 8) {
          x = 9;
        }
      }
      blit[0](canvas, gMapGridCellBuffer, x + shift, FIXEDtoINT(reaperY), 55,
              88);
    }

    // The scythe's path: still, a backswing at 24, the strike at 33.
    i = 0;
    still = 0;
    scythe = (struct Scythe *)work->particles;
    for (; i != 1; i++, scythe++) {
      if (++scythe->timer <= 0) {
        continue;
      }
      if (scythe->timer == 1) {
        scythe->velocityX = still;
        scythe->velocityY = still;
      }
      if (scythe->timer == 24) {
        scythe->velocityX = -toFIXED(4);
        scythe->velocityY = toFIXED(8);
      }
      if (scythe->timer == 33) {
        scythe->velocityX = toFIXED(4);
        scythe->velocityY = -toFIXED(8);
        work->shakeFrames = 4;
        speed = 0;
        Sound_PlayFar(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
      }
      if (scythe->timer == 35) {
        scythe->velocityX = still;
        scythe->velocityY = still;
      }
      if (frame > 63) {
        s32 fall = (frame << 7) - 0x2000;

        scythe->velocityY -= toFIXED(1) / 4;
        if (fall > 0x400) {
          fall = 0x400;
        }
        scythe->z -= fall;
      }
      if (scythe->timer > 8) {
        if (args->side == EFFECT_SIDE_ENEMIES) {
          scythe->x += scythe->velocityX;
        } else {
          scythe->x -= scythe->velocityX;
        }
        scythe->y += scythe->velocityY;
      }
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    zero = 0;
    phase = frame - 50;
    if (phase == -4) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (phase == 0) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 12;
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
    }
    timer = work->particles[0].variant;
    if ((u32)(frame - 44) <= 13) {
      work->particles[0].velocityZ++;
    }
    if (timer >= 0) {
      scale = 0xb1e0;
      if (frame > 63) {
        scale = scale - (frame << 9) + FIXED_ONE / 2;
      }
      width = 7;
      if (scale > 0) {
        model->shade = zero;
        model->kind = MODEL_SHADED;
        model->vertices = vertices;
        model->texture = &texture;
        Matrix_Initialize();
        Matrix_Translate(work->particles[0].x + INTtoFIXED(shift),
                         work->particles[0].y, 0);
        if (args->side == EFFECT_SIDE_PARTY) {
          Matrix_RotateY(DEGtoANG(180));
        }
        Matrix_RotateZ(work->particles[0].z);
        Matrix_ScaleUniform(scale);
        texture.logHeight = 5;
        texture.pixels = LIFE_SHEAR_HANDLE_TEXTURE;
        texture.logWidth = width;
        model->mesh = gLifeShear_HandleMesh;
        Matrix_Push();
        Matrix_RotateZ((Trig_Sin(work->particles[0].velocityZ << 11) >> 3) -
                       DEGtoANG(45));
        Render_TransformVertices(gLifeShear_ScytheCorners, vertices, 4);
        Render_DrawModelList(model);
        Matrix_Pop();
        model->mesh = gLifeShear_BladeMesh;
        texture.logWidth = width;
        texture.logHeight = 6;
        texture.pixels = LIFE_SHEAR_BLADE_TEXTURE;
        Matrix_RotateZ(DEGtoANG(45) -
                       (Trig_Sin(work->particles[0].velocityZ << 11) >> 3));
        Matrix_Scale(FIXED_ONE, 0x15f90, FIXED_ONE);
        Render_TransformVertices(gLifeShear_ScytheCorners, vertices, 4);
        Render_DrawModelList(model);
      }

      // The flash: four blades, shrinking over eight frames.
      if ((u32)phase <= 7) {
        scale = 0x78e20 - (frame << 13);
        Matrix_Initialize();
        if (args->side == EFFECT_SIDE_ENEMIES) {
          Matrix_Translate(0xfff00000, 0, 0);
        } else {
          Matrix_Translate(toFIXED(16), 0, 0);
        }
        Matrix_Scale(scale, scale, scale);
        if (args->side == EFFECT_SIDE_PARTY) {
          Matrix_RotateY(DEGtoANG(180));
        }
        model->kind = MODEL_SHADED_MAX;
        model->mesh = gBattleEffect_SmallSquareMeshReversed;
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = LIFE_SHEAR_FLASH_TEXTURE;
        for (i = 0; i != 4; i++) {
          Matrix_Push();
          Matrix_RotateZ(i * (DEGtoANG(90)) + 0x5650);
          Matrix_Scale(FIXED_ONE / 4, ((i & 1) << 15) + FIXED_ONE * 3 / 4,
                       FIXED_ONE / 2);
          Matrix_ScaleUniform(FIXED_ONE);
          Render_TransformVertices(gBattleEffect_StandingSquareCorners,
                                   vertices, 4);
          Render_DrawModelList(model);
          Matrix_Pop();
        }
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Abilities: Purgatory.
void BattleEffect_RunPurgatory(struct BattleEffectParams *args) {
  BattleEffect_RunGeyser(args, GEYSER_PURGATORY);
}

/// Abilities: Djinn Blast.
void BattleEffect_RunDjinnBlast(struct BattleEffectParams *args) {
  BattleEffect_RunGeyser(args, GEYSER_DJINN_BLAST);
}

/// Abilities: Djinn Storm.
void BattleEffect_RunDjinnStorm(struct BattleEffectParams *args) {
  BattleEffect_RunGeyser(args, GEYSER_DJINN_STORM);
}

#define GEYSER_FRAMES 54
#define GEYSER_DRAW_FRAMES 40
#define GEYSER_PEAK_FRAME 20 ///< the result phase starts
#define DROP_COUNT 512       ///< drops in the map cell buffer, used in a ring

/// Copies a palette over the background's 256 colours.
static inline void CopyPalette(const void *palette) {
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
}

/// A geyser between the targets. A textured column, its texture scrolling
/// upward, stands on the floor at the targets' midpoint, and drops burst
/// from its foot, fall back under gravity and bounce once on the floor.
/// Mode 0: the cross texture in orange; the column rises and
/// falls on a sine and 200 drops burst at once. Mode 1: the
/// column keeps widening and fades from frame 32, with 200 drops and 16 more
/// each frame to frame 23. Mode 2: a thin column with 100 drops
/// and 2 more each frame. In modes 1 and 2 the palette also steps through
/// the Mercury, Mars and Jupiter Djinn's. At frame 20 the target is knocked
/// back and the result phase starts.
void BattleEffect_RunGeyser(struct BattleEffectParams *args, s32 mode) {
  struct ModelTexture texture;
  DrawRectangle draw[2];
  struct ScreenPoint midpoint;
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  u8 *sparkCels = Ram_HeapSlots->battleSheet;
  struct Particle *drop;
  struct RenderModel *column;
  void *vertices;
  s32 frame;
  s32 i;
  s32 count;
  s32 scale;
  s32 brightness;

  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (mode == GEYSER_PURGATORY)
    REG_BLDALPHA = BLEND_ALPHA(16, 12);
  else
    REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    s32 spawned;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
    spawned = 0;
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sparkCels, 0, 0);
    if (mode == GEYSER_PURGATORY) {
      Resource_LoadAndDecompress(RESOURCE_CROSS_TEXTURE_SHEET, work->sheet, 1,
                                 1);
      CopyPalette(Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B));
    } else {
      Resource_LoadAndDecompress(RESOURCE_EMBER_TEXTURE, work->sheet, 0, 0);
      CopyPalette(Resource_GetTableEntry(RESOURCE_BLAST_SHEET));
    }

    for (frame = 0; frame != GEYSER_FRAMES; frame++) {
      // The geyser follows the targets' midpoint while it rises.
      if (frame < GEYSER_PEAK_FRAME)
        BattleEffect_GetTargetsMidpoint(&midpoint);
      if (frame == 0) {
        Sound_PlayFar(SE_CLATTER);
        /* FAKEMATCH(dead): an empty once-only loop after the call. */
        do {
        } while (0);
        work->shakeFrames = 16;
      }
      if (frame == GEYSER_PEAK_FRAME) {
        work->shakeFrames = 8;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      }

      // The first frame clears every drop.
      if (frame == 0) {
        s32 *life;

        spawned = 0;
        i = 0;
        life = ((s32 *)(Ram_MapCellBuffer + 0x18));
        for (; i != DROP_COUNT; i++) {
          *life = -1;
          life += 7;
        }
      }

      // Modes 1 and 2 step the palette three times a frame from frame 8:
      // toward the Mercury Djinn's, from frame 20 the Mars Djinn's and from
      // frame 32 the Jupiter Djinn's.
      if (mode == GEYSER_DJINN_STORM || mode == GEYSER_DJINN_BLAST) {
        for (i = 0; i != 3; i++) {
          if (frame >= 8) {
            if (frame < 20)
              BattleEffect_StepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
            else if (frame < 32)
              BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
            else
              BattleEffect_StepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
          }
        }
      }

      // How many drops burst this frame.
      count = 0;
      if (mode == GEYSER_DJINN_BLAST) {
        if (frame == 0)
          count = 100;
        if (frame >= 1 && frame < 24)
          count = 2;
      } else {
        if (frame == 0)
          count = 200;
        if (mode == GEYSER_DJINN_STORM || mode == GEYSER_DJINN_BLAST) {
          if (frame >= 1 && frame < 24)
            count = 16;
        }
      }
      for (i = 0; i != count; i++, spawned++) {
        s32 speed;
        s32 angle;

        drop = &((struct Particle *)Ram_MapCellBuffer)[spawned & 511];
        speed = (Random16() & 1023) + 32;
        angle = (Random16() & 0x7fff) - DEGtoANG(90);
        drop->x = INTtoFIXED(midpoint.x / 2);
        drop->y = toFIXED(108);
        if (mode == GEYSER_DJINN_BLAST)
          drop->velocityX = (Trig_Sin(angle) * speed) >> 8;
        else
          drop->velocityX = (Trig_Sin(angle) * speed) >> 6;
        drop->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        drop->variant = (Random16() & 7) + 48;
      }

      // Each drop falls, bounces off the floor at y 120, and is drawn while
      // it is on the canvas, its cel shrinking as it burns down.
      drop = (struct Particle *)Ram_MapCellBuffer;
      for (i = 0; i != DROP_COUNT; i++, drop++) {
        if (drop->variant > 0) {
          s32 x;
          s32 y;

          drop->variant--;
          BattleEffectKernels_IntegrateVector2(drop, 60,
                                               gGeyser_DropGravity[mode]);
          y = FIXEDtoINT(drop->y);
          if (y > 120) {
            drop->velocityY = -drop->velocityY / 2;
          } else if (drop->x >= 0 && (x = FIXEDtoINT(drop->x)) <= 126 &&
                     drop->y >= 0) {
            s32 size = (drop->variant - 16) / 8;

            if (size < gGeyser_DropMinSizes[mode])
              size = gGeyser_DropMinSizes[mode];
            draw[i & 1](canvas,
                        sparkCels + gBattleEffect_MotePictures[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }

      // The column, for the first 40 frames.
      if (frame < GEYSER_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(512);
        column = Render_AllocateModelList(1);
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = work->sheet;
        column->scrollU = (frame << 2) & 31;
        column->scrollV = (frame << 1) & 31;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(midpoint.x / 2 - 64), -toFIXED(16), 0);
        Matrix_RotateX(2000);
        if (mode == GEYSER_PURGATORY) {
          // Up on a sine in 16 frames, held, and down again from frame 24.
          if (frame < 16)
            scale = Trig_Sin(frame << 10);
          else if (frame < 24)
            scale = FIXED_ONE;
          else
            scale = Trig_Sin((frame << 10) - DEGtoANG(45));
          brightness = -32;
        } else if (mode == GEYSER_DJINN_STORM) {
          // Wider every frame, fading from frame 32.
          scale = frame << 12;
          brightness = -32;
          if (frame > 31)
            brightness = 224 - (frame << 3);
        } else {
          scale = FIXED_ONE / 4;
          brightness = -32;
          if (frame > 31)
            brightness = 224 - (frame << 3);
        }
        column->kind = MODEL_SHADED_ADDITIVE;
        column->shade = brightness;
        column->mesh = gGeyser_Mesh;
        column->texture = &texture;
        column->vertices = vertices;
        Matrix_Scale(scale * 2, FIXED_ONE * 7, scale * 2);
        Render_TransformVertices(gBattleEffect_OctagonBandCorners, vertices,
                                 16);
        Render_DrawModelList(column);
        Heap_Release(column);
        Heap_Release(vertices);
      }

      BattleCamera_Shake(4, 16);
      EFFECT_END_FRAME(work);
    }
  }
  EFFECT_RELEASE_BLITTERS();
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Djinn Stun.
void BattleEffect_RunDjinnStun(struct BattleEffectParams *args) {
  BattleEffect_RunStarSwarm(args, STAR_SWARM_DJINN_STUN);
}

/// Abilities: Mystic Call.
void BattleEffect_RunMysticCall(struct BattleEffectParams *args) {
  BattleEffect_RunStarSwarm(args, STAR_SWARM_MYSTIC_CALL);
}

#define STARS_PER_TARGET 64 ///< each target's slice of the star buffer
#define STAR_TOTAL 384

/// Swarms of stars over the targets. Each target gets a swarm, eight frames
/// after the one before: its stars burst from a point that rises out of it,
/// each turning about the point on one of four axes and drawn smaller the
/// deeper it is, and from frame 24 they are pulled back into the point,
/// each one gone once it has arrived. The palette steps through the
/// Mercury, Mars and Jupiter Djinn's. Mode 0: 16 stars a target, the
/// target flashes as its swarm starts, and the effect lasts eight frames
/// a target more than 38. Mode 1: 48 stars, a flash at the start and 55
/// frames later, 72 frames in all.
void BattleEffect_RunStarSwarm(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  DrawRectangle draw[2];
  s32 member;
  u8 *sparkCels = Ram_HeapSlots->battleSheet;
  s32 frames;
  struct BattleCamera *camera;
  s32 frame;
  s32 i;
  struct ScreenPoint screen;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sparkCels, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  if (mode == STAR_SWARM_DJINN_STUN)
    frames = args->targetCount * 8 + 38;
  else
    frames = 72;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != frames; frame++) {
    camera = Ram_HeapSlots->cameraWork;

    // The first frame sets each target's point on it, rising, and every
    // star at its point with a random push across the floor.
    if (frame == 0) {
      for (i = 0; i != args->targetCount; i++) {
        struct MotionObject *object =
            BattleObject_GetFar(args->targets[i])->object;

        work->particles[i].x = object->x;
        work->particles[i].y = 0;
        work->particles[i].z = object->z;
        if (mode == STAR_SWARM_MYSTIC_CALL) {
          work->particles[i].velocityX = 0;
          work->particles[i].velocityY = toFIXED(1) / 2;
        } else {
          work->particles[i].velocityX = -object->x / 40;
          work->particles[i].velocityY = toFIXED(3);
        }
        work->particles[i].velocityZ = 0;
      }
      for (i = 0; i != STAR_TOTAL; i++) {
        struct Particle *star = &((struct Particle *)Ram_MapCellBuffer)[i];
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 speed = 255; // the mask starts in speed's own register

        speed &= Random16();

        SET_POSITION(star, 0, INTtoFIXED((Random16() & 31) + 20), 0);
        speed += 128;
        star->velocityX = (Trig_Sin(angle) * speed) >> 5;
        star->velocityY = 0;
        star->velocityZ = (Trig_Cos(angle) * speed) >> 5;
        star->variant = 0;
      }
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_BLAST_SHEET),
                           SHEET_PALETTE_BYTES);
    }

    // Two palette steps a frame from frame 8.
    for (i = 0; i != 2; i++) {
      if (frame >= 8) {
        if (frame < 20)
          BattleEffect_StepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
        else if (frame < 32)
          BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
        else
          BattleEffect_StepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
      }
    }

    for (member = 0; member != args->targetCount; member++) {
      struct Particle *point = &work->particles[member];
      s32 start = member;
      s32 starCount;

      // Set in two steps: as one giv the loop pass would turn it into a walker.
      start *= 8;
      starCount = 16;
      if (mode != STAR_SWARM_DJINN_STUN)
        starCount = 48;
      if (frame == start) {
        if (mode == STAR_SWARM_DJINN_STUN)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, member, 16);
        else
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 16);
      }
      if (frame == start + 55 && mode != STAR_SWARM_DJINN_STUN)
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, member, 16);

      // The swarm turns about its point, which rises from frame 4.
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      if (frame >= start + 4 && start < 44)
        BattleEffectKernels_IntegrateVector3(point, PARTICLE_DAMPING_NONE, 0);
      Matrix_TranslateVector(&point->x);

      for (i = 0; i != starCount; i++) {
        struct Particle *star = &((
            struct Particle *)Ram_MapCellBuffer)[member * STARS_PER_TARGET + i];

        if (frame >= i / 8 + start && star->variant == 0) {
          s32 size;

          Matrix_Push();
          switch ((member + i) & 3) {
          case STAR_SWARM_DJINN_STUN:
            Matrix_RotateY(frame * (i + 8) * 32);
            break;
          case STAR_SWARM_MYSTIC_CALL:
            Matrix_RotateX(-frame * (i + 8) * 32);
            break;
          case 2:
            Matrix_RotateZ(-frame * (i + 8) * 32);
            break;
          case 3:
            Matrix_RotateX(-frame * (i + 8) * 32);
            Matrix_RotateZ(-frame * (i + 8) * 32);
            break;
          }
          EffectPosition_ProjectPoint(&star->x, &screen);
          screen.x >>= 1;
          Matrix_Pop();
          if (screen.depth < 250)
            screen.depth = 250;
          if (screen.depth > 0x27a)
            screen.depth = 0x27a;
          size = 9 - (screen.depth - 250) / 64;
          draw[0](canvas, sparkCels + gBattleEffect_MotePictures[size - 1],
                  screen.x - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(star, 60, 0);

          // The pull back into the point; a star within 2047 of it across
          // the floor is gone.
          if (frame >= i / 8 + 24) {
            s32 dx = -star->x >> 7;
            s32 dy = -star->y >> 7;
            s32 dz = -star->z >> 7;

            star->velocityX += dx;
            star->velocityY += dy;
            star->velocityZ += dz;
            star->velocityX = star->velocityX * 62 / DAMPING_SCALE;
            star->velocityY = star->velocityY * 62 / DAMPING_SCALE;
            star->velocityZ = star->velocityZ * 62 / DAMPING_SCALE;
            if ((dx >= -2047 && dx <= 2047) && (dz >= -2047 && dz <= 2047))
              star->variant = -1;
          }
        }
      }
    }

    BattleCamera_Shake(4, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

/// Unused: nothing calls it. Opens the canvas layer, sets alpha blending and a
/// 24-frame palette fade counter (read by Palette_StepFadeTransfer), then runs
/// the effect body BattleEffect_RunBubbleColumn.
void BattleEffect_StartBubbleColumn(s32 arg) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  work->fadeFrames = 24;
  work->fadeStep = 0;
  BattleEffect_RunBubbleColumn(arg);
}

// The Lost Age numbers its blitter modes from the other end of The Broken
// Seal's: additive 0, max 1, transparent 2, copy 3.

#define BUBBLE_COUNT 256
#define BUBBLE_FRAMES 60
/// The bubbles past the map cells: each keeps its picture in z and its life in
/// variant, -1 once it has left the top of the canvas.
#define BUBBLES ((struct Particle *)(Ram_MapCellBuffer + 0x5000))

/// A column of bubbles. On the first frame 256 glowing ovals of random size
/// appear over the bottom left of the canvas; for 60 frames they float up
/// and drift sideways, drawn with the max blitter in the blue beam's palette,
/// and each one is gone once it has risen past the top. From frame 18 the
/// targets flash one by one, eight frames apart, each with the hit sound.
void BattleEffect_RunBubbleColumn(struct BattleEffectParams *args) {
  void *canvas = Ram_HeapSlots->battleCanvas;
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 frame;
  s32 i;
  struct Particle *bubble;

  // Each frame the transfer task copies the canvas and clears it.
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  // The blue beam's palette colours the bubble cels.
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_BLUE_BEAM_SHEET),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_BUBBLE_ANIMATION_SHEET, work->sheet, 0,
                             0);

  for (frame = 0; frame != BUBBLE_FRAMES; frame++) {
    // The first frame scatters every bubble below the bottom left.
    if (frame == 0) {
      struct Particle *spawn = BUBBLES;

      for (i = 0; i != BUBBLE_COUNT; i++) {
        spawn->x = INTtoFIXED(Random16() & 127);
        spawn->y = INTtoFIXED((Random16() & 127) + 120);
        spawn->velocityX = ((Random16() & 31) - 16) << 13;
        spawn->velocityY = (8 - (Random16() & 31)) << 12;
        spawn->z = (Random16() & 7) + 5;
        spawn->variant = 32;
        spawn++;
      }
    }

    // Draw each bubble centred on its place, let it rise, and drop it once
    // it is wholly above the canvas.
    bubble = BUBBLES;
    for (i = 0; i != BUBBLE_COUNT; i++) {
      if (bubble->variant >= 0) {
        s32 cel = bubble->z;
        u32 width;
        u32 height;

        draw[0](canvas, work->sheet + gBubbleColumn_Pictures[cel],
                FIXED_INTEGER_PART(bubble->x) -
                    ((width = gBubbleColumn_PictureWidths[cel]) >> 1),
                FIXED_INTEGER_PART(bubble->y) -
                    ((height = gBubbleColumn_PictureHeights[cel]) >> 1),
                width, height);
        BattleEffectKernels_IntegrateVector2(bubble, 63, -toFIXED(1) / 4);
        if (bubble->y < -INTtoFIXED(gBubbleColumn_PictureHeights[cel]))
          bubble->variant = -1;
      }
      bubble++;
    }

    // The targets flash in turn from frame 18; the first hit lets the
    // battle show the damage.
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 16);
          EFFECT_SHOW_DAMAGE(SE_RECOVERY);
          Sound_PlayFar(SE_RECOVERY);
        } else {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 16);
        }
      }
    }

    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Coatlicue's summon.

struct MeshTriangle;
struct MeshVertex;

#define EFFECT_STEPS ((struct Particle *)(Ram_MapCellBuffer + 0x5000))

/// Fills the scanline gradient behind the serpent: dark rows above and below,
/// a ramp shaded by level in between.
#define FILL_GRADIENT(table, i, level, c)                                      \
  do {                                                                         \
    for (; i != 135; i++) {                                                    \
      s32 t_ = i - 15;                                                         \
      s32 b_ = (level) - t_ / 15 + 32;                                         \
      s32 g_ = (level) - t_ / 16 + 16;                                         \
      s32 r_ = (level) - t_ / 17;                                              \
                                                                               \
      if (b_ < 0)                                                              \
        b_ = 0;                                                                \
      if (b_ > 31)                                                             \
        b_ = 31;                                                               \
      if (g_ < 0)                                                              \
        g_ = 0;                                                                \
      if (g_ > 31)                                                             \
        g_ = 31;                                                               \
      if (r_ < 0)                                                              \
        r_ = 0;                                                                \
      if (r_ > 31)                                                             \
        r_ = 31;                                                               \
      c = RGB(r_, g_, b_);                                                     \
      *table++ = c;                                                            \
    }                                                                          \
    for (; i != DISPLAY_HEIGHT; i++) {                                         \
      *table++ = 0;                                                            \
    }                                                                          \
  } while (0)

/// Draws cell n of the stone sheet at offset base, centred on (x, y).
#define BLIT_CELL(base, n, x, y)                                               \
  do {                                                                         \
    u8 *src_ = work->sheet + (base) + gCoatlicue_Pictures[n];                  \
    u8 w_ = gCoatlicue_PictureWidths[n];                                       \
    s32 x_ = (x) - (w_ >> 1);                                                  \
    u8 h_ = gCoatlicue_PictureHeights[n];                                      \
                                                                               \
    blit[0](canvas, src_, x_, (y) - (h_ >> 1), w_, h_);                        \
  } while (0)

/// Draws one spark of the falling stone sheet and drops it when it leaves the
/// top of the screen.
#define DRAW_SPARK(p)                                                          \
  do {                                                                         \
    s32 n_ = (p)->z;                                                           \
                                                                               \
    {                                                                          \
      u8 *src_ = work->sheet + COATLICUE_BUBBLES + gCoatlicue_Pictures[n_];    \
      s32 x0_ = FIXED_INTEGER_PART((p)->x);                                    \
      u8 w_ = gCoatlicue_PictureWidths[n_];                                    \
      s32 x_ = x0_ - (w_ >> 1);                                                \
      s32 y_ = FIXED_INTEGER_PART((p)->y);                                     \
      u8 h_ = gCoatlicue_PictureHeights[n_];                                   \
                                                                               \
      blit[0](canvas, src_, x_, y_ - (h_ >> 1), w_, h_);                       \
    }                                                                          \
    BattleEffectKernels_IntegrateVector2(p, 63, -toFIXED(1) / 4);              \
    if ((p)->y < -INTtoFIXED(gCoatlicue_PictureHeights[n_])) {                 \
      (p)->variant = -1;                                                       \
    }                                                                          \
  } while (0)

/// Where ScaleTexture is decoded in the effect's pictures.
#define COATLICUE_SCALES 0x4000
/// BubbleAnimationSheet's fifteen pictures, then a copy of them at half
/// brightness.
#define COATLICUE_BUBBLES 0x4800
#define BUBBLE_SHEET_BYTES 3000
#define COATLICUE_DIM_BUBBLES (COATLICUE_BUBBLES + BUBBLE_SHEET_BYTES)

#define COATLICUE_FRAMES 602 ///< frames of the summon after its opening

/// Coatlicue: under water among rising bubbles a ring of light spins and
/// crosses a second; they settle into a ripple and burst in a froth of
/// bubbles, Coatlicue rises among them, and a wash of light heals the party.
///
/// Abilities: Coatlicue.
void BattleEffect_RunCoatlicue(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  u8 *cellBuffer = gMapGridCellBuffer;
  // Only blit[0] is used; the array takes an 8-byte slot in the frame.
  DrawRectangle blit[2];
  struct Point2D grid;
  s32 skip;
  u16 *table;
  s32 frame;
  s32 i;
  s32 k;
  s32 y;
  s32 level;
  s32 color;
  s32 s;
  s32 sh;
  s32 ty;
  s32 spin;
  s32 zero;
  u8 *coilMesh = Ram_MapCellBuffer + 0x4800;
  s16 list[UNIT_MAX];
  FIXED place[4];
  struct Scale req;
  struct ScreenPoint pos;
  struct ModelTexture texture;
  struct ModelTexture washTexture;
  struct Sprite *obj;
  struct Particle *p;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  RagingHeat_FillGradientCanvas();
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  y = -80;
  for (frame = 0; frame != 27; frame++) {
    y += 4;
    REG_BG2X = y << 8;
    EFFECT_PRESENT_CANVAS(work);
  }
  zero = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  table = work->backdropLines;
  level = -8;
  color = 0;
  for (i = 0; i != 15; i++) {
    *table++ = color;
  }
  FILL_GRADIENT(table, i, level, color);
  Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_FRAME + 126);
  skip = 0;
  Mesh_BuildCylinderBand(Ram_MapCellBuffer + 0x4400, coilMesh, 32);
  list[0] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(list, 0);
  {
    u8 *e = Resource_GetTableEntry(RESOURCE_BELL_MAIDEN_TILES);

    Iwram_CopyWords((void *)OBJ_PLTT, e, OBJ_PLTT_SIZE);
    Decompress_TaggedLz(e + 512, cellBuffer);
    {
      u8 *cells;

      i = 0;
      cells = gMapGridCellBuffer;
      for (; i != 12; cells += 1024, i++) {
        obj = EffectSprite_CreateWithTiles(32, 32,
                                           OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        obj->image.priority = 2;
        work->sprites[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), cells, 1024);
      }
    }
  }
  Graphics_LoadGreyRamp();
  Resource_LoadAndDecompress(RESOURCE_COG_TEXTURE, cellBuffer, 0, 0);
  Picture_MirrorIntoQuadrants(cellBuffer, work->sheet, 64, 64);
  Resource_LoadAndDecompress(RESOURCE_SCALE_TEXTURE,
                             work->sheet + COATLICUE_SCALES, 0, 0);
  {
    u8 *src = work->sheet + COATLICUE_BUBBLES;

    Resource_LoadAndDecompress(RESOURCE_BUBBLE_ANIMATION_SHEET, src, 0, 0);
    for (i = 0; i != BUBBLE_SHEET_BYTES; i++) {
      work->sheet[COATLICUE_DIM_BUBBLES + i] = src[i] >> 1;
    }
  }
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cellBuffer, 0, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  for (frame = 0; frame != COATLICUE_FRAMES; frame++) {
    spin = ((frame - 1) << 9) + 512;
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame <= 581 && skip == 0) {
      skip = 1;
    }
    if (skip > 0) {
      s32 *value = &work->transferValue;
      s32 v = skip * 0x101;

      v = (v + INTtoFIXED(v)) << 2;
      *value = v;
      if (v > 0x3f3f3f3f) {
        *value = 0x3f3f3f3f;
      }
      work->transferMode = CANVAS_COPY_AND_CLEAR;
      if (++skip == 19) {
        break;
      }
    }
    if (frame == 0) {
      for (i = 0; i != 384; i++) {
        EFFECT_STEPS[i].variant = -1;
      }
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = -1;
      }
      grid.x = toFIXED(88);
      grid.y = toFIXED(36);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if (frame == 82) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
      Sound_PlayFar(SE_PSYNERGY_START);
    }
    if (frame == 210) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
      Sound_PlayFar(SE_PSYNERGY_START);
    }
    if (frame == 274) {
      Sound_PlayFar(SE_DJINNI_SET);
    }
    if (frame == 310) {
      Sound_PlayFar(SE_CURSE_FALL);
    }
    if (frame == 316) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 466) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 498) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 503) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 508) {
      Sound_PlayFar(SE_CAST);
    }
    if (frame == 520) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 540) {
      Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
    }
    if (frame <= 1999999) {
      u16 *row = work->backdropLines;
      s32 shade = frame / 32 - 8;

      if (shade > 8) {
        shade = 8;
      }
      color = 0;
      for (i = 0; i != 15; i++) {
        *row++ = color;
      }
      if (frame > 539) {
        shade = (frame - 540) / 2 + 8;
      } else if (frame > 311) {
        shade = 14 - (frame - 312) / 4;
        if (shade <= 7) {
          shade = 8;
        }
      } else if (frame > 299) {
        shade += (frame - 300) / 2;
        if (shade > 14) {
          shade = 14;
        }
      }
      i = 15;
      FILL_GRADIENT(row, i, shade, color);
    }
    if ((u32)(frame - 282) <= 17) {
      s32 green = (frame - 282) / 2 + 14;
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 blue = frame - 253;
      u32 color;

      if (green > RGB_LEVEL_MAX) {
        green = RGB_LEVEL_MAX;
      }
      if (blue > RGB_LEVEL_MAX) {
        blue = RGB_LEVEL_MAX;
      }
      i = 0;
      color = RGB(0, green, blue);
      for (; i != 256; i++) {
        *dst++ = color;
      }
    }
    // From frame 300 every sprite colour steps two levels a frame toward the
    // maiden's own, lifted by redLift, greenLift and k in blue: first to white,
    // then less, then to her colours.
    if (frame > 299) {
      u16 *source = Resource_GetTableEntry(RESOURCE_BELL_MAIDEN_TILES);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 redLift = RGB_LEVEL_MAX;
      s32 greenLift = RGB_LEVEL_MAX;

      k = 15;

      if (frame > 315) {
        redLift = 4;
        greenLift = 12;
        k = 16;
      }
      if (frame > 337) {
        redLift = 0;
        greenLift = 0;
        k = 0;
      }
      for (i = 0; i != 256; i++, source++, dst++) {
        s32 color = *dst;
        s32 goal = *source;
        s32 blue = RGB_BLUE((u16)color);
        s32 green = RGB_GREEN((u16)color);
        s32 red = RGB_RED(color);
        s32 goalBlue = RGB_BLUE((u16)goal) + k;
        s32 goalGreen = RGB_GREEN((u16)goal) + greenLift;
        s32 goalRedBase = RGB_RED(goal);
        s32 goalRed;

        /* FAKEMATCH(barrier): goalRedBase held apart from goal through an
         * empty asm. */
        __asm__("" : "=r"(goalRedBase) : "0"(goalRedBase));
        goalRed = goalRedBase + redLift;
        if (blue < goalBlue - 1) {
          blue += 2;
        } else if (blue > goalBlue + 1) {
          blue -= 2;
        }
        if (green < goalGreen - 1) {
          green += 2;
        } else if (green > goalGreen + 1) {
          green -= 2;
        }
        if (red < goalRed - 1) {
          red += 2;
        } else if (red > goalRed + 1) {
          red -= 2;
        }
        if (blue > RGB_LEVEL_MAX) {
          blue = RGB_LEVEL_MAX;
        }
        if (green > RGB_LEVEL_MAX) {
          green = RGB_LEVEL_MAX;
        }
        if (red > RGB_LEVEL_MAX) {
          red = RGB_LEVEL_MAX;
        }
        if (blue < 0) {
          blue = 0;
        }
        if (green < 0) {
          green = 0;
        }
        if (red < 0) {
          red = 0;
        }
        *dst = RGB(red, green, blue);
      }
    }
    // From frame 520 the maiden's colours brighten, red fastest and blue
    // slowest.
    if (frame > 519) {
      s32 lift = frame - 520;
      u16 *source = Resource_GetTableEntry(RESOURCE_BELL_MAIDEN_TILES);
      u16 *dst = (u16 *)OBJ_PLTT;
      s32 half;

      i = 0;
      half = lift / 2;
      for (; i != 256; i++, source++, dst++) {
        s32 b;
        s32 g;
        s32 r;
        s32 color;
        s32 sourceRed;

        color = *source;
        /* FAKEMATCH(temporary): red through a temporary set twice. */
        sourceRed = 31;
        sourceRed &= color;
        r = sourceRed + lift;
        g = RGB_GREEN((u16)color) + half;
        b = RGB_BLUE((u16)color) + lift / 4;

        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
        *dst = RGB(r, g, b);
      }
    }
    if ((u32)(frame - 292) <= 253) {
      req = gCoatlicue_UnitScale;
      place[1] = toFIXED(255);
      place[3] = zero;
      if (frame == 360) {
        grid.y = toFIXED(36);
      }
      if ((u32)(frame - 396) <= 111) {
        grid.y += toFIXED(1) / 4;
      }
      if (frame > 539) {
        grid.y += toFIXED(16);
      }
      for (i = 0; i != 12; i++) {
        place[0] = INTtoFIXED((i % 3) * 32) + grid.x;
        place[2] = INTtoFIXED((i / 3) * 32) + grid.y;
        Render_ApplyProjectedPlacementFar(work->sprites[i], place, (s32 *)&req,
                                          0);
      }
    }
    {
      s32 d = frame - 274;
      if ((u32)d <= 23) {
        struct Particle *p = &(&EFFECT_STEPS[80])[d];
        {
          s32 x = INTtoFIXED((Random16() & 15) + 52);

          p->y = toFIXED(100);
          p->x = x;
        }
        p->velocityX = ((Random16() & 31) - 16) << 12;
        p->velocityY = -((Random16() & 15) + 8) << 12;
        p->z = (Random16() & 7) + 5;
        p->variant = 32;
      }
    }
    if (frame == 310) {
      struct Particle *p = EFFECT_STEPS;

      for (i = 0; i != 80; i++, p++) {
        p->x = INTtoFIXED((Random16() & 63) + 28);
        p->y = INTtoFIXED(Random16() % 80 + 56);
        p->velocityX = ((Random16() & 31) - 16) << 12;
        p->velocityY = (8 - (Random16() & 31)) << 12;
        p->z = (Random16() & 7) + 5;
        p->variant = 32;
      }
    }
    for (k = 0; k != 9; k++) {
      if (frame == gCoatlicue_BurstFrames[k]) {
        for (i = 0; i != 7; i++) {
          struct Particle *p = &work->particles[(k % 9) * 7 + i];

          p->x =
              (Random16() % gCoatlicue_BurstSpreadX[k] + gCoatlicue_BurstX[k])
              << 16;
          p->y = ((Random16() & 3) + i * gCoatlicue_BurstSpacingY[k] +
                  gCoatlicue_BurstY[k])
                 << 16;
          p->velocityX = gCoatlicue_BurstVelocityX[k] << 12;
          p->velocityY = gCoatlicue_BurstVelocityY[k] << 12;
          p->z = INTtoFIXED((Random16() & 3) + gCoatlicue_BurstZ[k]);
          p->velocityZ = gCoatlicue_BurstVelocityZ[k] << 10;
          p->variant = zero;
        }
      }
    }
    if (frame <= 359) {
      p = EFFECT_STEPS;

      for (i = 0; i != 384; i++, p++) {
        if (p->variant >= 0) {
          DRAW_SPARK(p);
        }
      }
    } else {
      Matrix_Initialize();
      Matrix_Translate(0, 0, toFIXED(256));
      if (frame > 539) {
        Matrix_Scale(frame * 1500 - 0xc4c10, FIXED_ONE, FIXED_ONE);
      } else if (frame > 517) {
        Matrix_Scale(0, 0, FIXED_ONE);
      } else if (frame > 485) {
        s32 s = 0x103000 - (frame << 11);

        Matrix_Scale(s, s, FIXED_ONE);
      }
      if (frame > 539) {
        Matrix_RotateY(frame << 10);
      } else if (frame > 517) {
        Matrix_RotateY(TRIG_TURN);
        Matrix_RotateX(DEGtoANG(180));
      } else if (frame > 453) {
        s32 a = (frame - 454) << 10;

        Matrix_RotateY(a);
        Matrix_RotateX(Trig_Sin(a) >> 3);
      }
      Matrix_Scale(55536, 45536, 27768);
      if (frame == 360) {
        struct Particle *p;

        if (skip == 0) {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                              CANVAS_FADE_THREE_QUARTERS);
        }
        p = EFFECT_STEPS;
        for (i = 0; i != 384; i++, p++) {
          p->variant = Random16() & 3;
          SET_POSITION(p, INTtoFIXED(Random16() % 160 - 80),
                       INTtoFIXED(Random16() % 160 + 48),
                       INTtoFIXED(Random16() % 160 - 80));
          p->velocityX = zero;
          p->velocityY =
              (-((Random16() & 63) + 32) << 10) - (1 << (16 - p->variant));
          p->velocityZ = Random16();
        }
      }
      if (frame == 540) {
        struct Particle *p = EFFECT_STEPS;
        s32 m;

        i = 0;
        m = 63;
        for (; i != 384; i++, p++) {
          p->y = zero;
          p->velocityY = -((Random16() & m) + 32) << 15;
        }
      }
      {
        p = EFFECT_STEPS;

        for (i = 0; i != 80; i++, p++) {
          s32 n;
          struct ScreenPoint *q;

          if (p->variant < 0) {
            continue;
          }
          q = &pos;
          EffectPosition_ProjectPoint(p, q);
          q->depth = FIXED_INTEGER_PART(q->depth) - 228;
          n = q->depth * 6 / 50 + gCoatlicue_PictureBias[p->variant];
          if (n < 0) {
            n = 0;
          }
          if (n > 14) {
            n = 14;
          }
          {
            s32 x = FIXED_INTEGER_PART(q->x);

            q->y = FIXED_INTEGER_PART(q->y);
            q->x = x + 60;
          }
          if (frame > 539) {
            q->y = q->y + (frame << 4) - 8608;
          } else if (frame > 517) {
            q->y = q->y + 16;
          } else if (frame <= 453) {
            q->y = q->y + 80;
          } else {
            q->y = q->y - frame + 534;
          }
          if (p->variant <= 1) {
            BLIT_CELL(COATLICUE_BUBBLES, n, q->x, q->y);
          } else {
            BLIT_CELL(COATLICUE_DIM_BUBBLES, n, q->x, q->y);
          }
          if (frame <= 453 || frame >= i / 16 + 540) {
            p->y += p->velocityY;
          }
        }
      }
    }
    for (k = 0; k != 9; k++) {
      FIXED top = 0;

      for (i = 0; i != 7; i++) {
        struct Particle *step = &work->particles[k * 7 + i];

        if (work->particles[k * 7 + 6].variant >= 0) {
          s32 n = 14 - FIXED_INTEGER_PART(step->z);
          s32 x = FIXEDtoINT(step->x +
                             (Trig_Sin((frame - (i % 7) * 8) << 10) << 2));

          if (i == 0) {
            top = step->y;
          }
          if (n < 0) {
            n = 0;
          }
          {
            u8 *src =
                work->sheet + COATLICUE_DIM_BUBBLES + gCoatlicue_Pictures[n];
            u8 w = gCoatlicue_PictureWidths[n];
            s32 x0 = x - (w >> 1);
            s32 y0 = FIXEDtoINT(top);
            u8 h = gCoatlicue_PictureHeights[n];

            blit[0](canvas, src, x0, y0 - (h >> 1), w, h);
          }
          BattleEffectKernels_IntegrateVector3(step, PARTICLE_DAMPING_NONE, 0);
          if (step->z < 0) {
            step->z = zero;
          }
          if (i == 6 && step->y < -INTtoFIXED(gCoatlicue_PictureHeights[n])) {
            step->variant = -1;
          }
          top = top + step->z + toFIXED(8);
        }
      }
    }
    {
      void *verts;
      s32 t;
      struct RenderModel *model;

      verts = Heap_AllocEwram(512);
      model = Render_AllocateModelList(1);
      model->texture = &texture;
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = verts;
      t = frame - 210;
      if ((u32)t <= 107) {
        s32 pitch;

        ty = (frame * 3 << 13) - 0x4ec000;
        pitch = 4000;
        if (ty > 0x180000) {
          ty = toFIXED(24);
        }
        s = frame * 611 - 111926;
        if (s > 55536) {
          s = 55536;
        }
        if (frame > 273) {
          ty = (frame << 15) - 0x710000;
          if (ty > 0x200000) {
            ty = toFIXED(32);
          }
          pitch = 21536 - (frame << 6);
          if (pitch <= 2999) {
            pitch = 3000;
          }
          s = s + frame * 1875 - 513750;
          if (s > 85536) {
            s = 85536;
          }
        }
        model->kind = MODEL_SHADED_MAX;
        texture.pixels = work->sheet;
        sh = (frame << 3) - 1744;
        if (sh > 0) {
          sh = 0;
        }
        if (frame > 309) {
          sh = (310 - frame) << 3;
        }
        model->shade = sh;
        Matrix_Initialize();
        Matrix_Translate(0, ty, 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_ScaleUniform(s);
        if (frame <= 273) {
          Matrix_RotateZ(spin - 0x12400);
        }
        Matrix_RotateX(pitch);
        Matrix_RotateY(spin);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 64);
        Render_DrawModelList(model);
      }
      if ((u32)(frame - 273) <= 7 && skip == 0) {
        s32 d = frame - 273;
        s32 *value;
        s32 v;

        work->transferMode = CANVAS_COPY_AND_CLEAR;
        value = &work->transferValue;
        v = d * 0x101;
        v = v + INTtoFIXED(v);
        *value = 0x3f3f3f3f - (v << 2);
      }
      if (frame == 281 && skip == 0) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      }
      if ((u32)(frame - 274) <= 27) {
        s = (frame << 11) - 0x85000;
        texture.pixels = cellBuffer;
        model->kind = MODEL_SHADED_ADDITIVE;
        sh = 0;
        if (frame > 293) {
          sh = (294 - frame) << 3;
        }
        model->shade = sh;
        Matrix_Initialize();
        Matrix_Translate(0, toFIXED(28), 0);
        Matrix_ScaleUniform(s);
        Matrix_RotateX(4000);
        Matrix_RotateY(frame << 9);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 64);
        Render_DrawModelList(model);
      }
      if (frame > 80) {
        for (i = 0; i != 4; i++) {
          s32 st = gCoatlicue_RingFrames[i];

          if (frame > st && frame < st + 10) {
            s32 d;

            texture.pixels = gMapGridCellBuffer;
            sh = -16;
            d = frame - st;
            model->kind = MODEL_SHADED_MAX;
            model->shade = sh;
            Matrix_Initialize();
            s = d << 13;
            if (i > 1) {
              s32 ty = -toFIXED(44);

              if (frame > 539) {
                ty += (frame - 539) << 20;
              }
              Matrix_Translate(-toFIXED(4), ty, 0);
              Matrix_Scale(s, s, s);
            } else {
              Matrix_Scale(s, d << 14, s);
            }
            Matrix_RotateX(DEGtoANG(90));
            Matrix_RotateY(frame << 9);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     64);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame > 80) {
        for (i = 0; i != 3; i++) {
          if (frame == gCoatlicue_FlashFrames[i]) {
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(47));
          }
          if (frame > gCoatlicue_FlashFrames[i] &&
              frame < gCoatlicue_FlashFrames[i] + 10) {
            s32 d = frame - gCoatlicue_FlashFrames[i];
            s = 0x14000 - (d << 13);
            texture.pixels = cellBuffer;
            model->kind = MODEL_SHADED_ADDITIVE;
            sh = d * 16 - 56;
            if (sh > 0) {
              sh = 0;
            }
            model->shade = sh;
            Matrix_Initialize();
            Matrix_Translate(-toFIXED(4), -toFIXED(44), 0);
            Matrix_Scale(s, s << 1, s);
            Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
            Matrix_RotateX(-TRIG_TURN / 16);
            Matrix_RotateY(frame << 9);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     64);
            Render_DrawModelList(model);
          }
        }
      }
      if (frame > 80) {
        for (i = 0; i != 3; i++) {
          s32 st = gCoatlicue_GlowFrames[i];

          if (frame > st && frame < st + 100) {
            ty = -toFIXED(44);
            s = (frame - st) * 2978;
            if (s > 65535) {
              s = 0x10000;
            }
            texture.pixels = work->sheet;
            sh = -16;
            model->kind = MODEL_SHADED_MAX;
            model->shade = sh;
            Matrix_Initialize();
            if (frame > 539) {
              ty = ((frame - 539) << 20) + -0x2c0000;
            }
            Matrix_Translate(-toFIXED(4), ty, 0);
            Matrix_Scale(s, s / 2, s);
            Matrix_RotateX(DEGtoANG(90));
            Matrix_RotateY(frame << 11);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts,
                                     64);
            Render_DrawModelList(model);
          }
        }
      }
      texture.logWidth = 7;
      texture.logHeight = 4;
      texture.pixels = work->sheet + COATLICUE_SCALES;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->mesh = coilMesh;
      model->vertices = verts;
      model->scrollU = zero;
      model->scrollV = zero;
      for (i = 0; i != 2; i++) {
        if (frame < gCoatlicue_CoilStarts[i]) {
          continue;
        }
        if (frame <= 293) {
          s = gCoatlicue_CoilScales[i] +
              (frame - gCoatlicue_CoilStarts[i]) * gCoatlicue_CoilGrowth[i];
          if (s > gCoatlicue_CoilMaxScales[i]) {
            s = gCoatlicue_CoilMaxScales[i];
          }
          if (frame > 209) {
            s -= t * gCoatlicue_CoilGrowth[i];
          }
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          if (frame > 273) {
            s = s + (frame << 13) + -0x224000;
          }
          Matrix_ScaleUniform(s);
          if (i == 0) {
            if (frame > 209) {
              s32 v = t;

              if (v > 48) {
                v = 48;
              }
              Matrix_Translate(0, INTtoFIXED(v), 0);
            }
            Matrix_RotateZ(gCoatlicue_CoilRolls[i] - (frame << 8));
            sh = -16;
          } else {
            s32 x = -48;

            if (frame > 127) {
              x = frame - 176;
            }
            if (x > -16) {
              x = -16;
            }
            Matrix_Translate(INTtoFIXED(x), 0, 0);
            Matrix_RotateZ(gCoatlicue_CoilRolls[i] + (frame << 8));
            if (frame > 209) {
              Matrix_RotateZ((frame << 8) + -0xd200);
            }
            sh = ((frame - gCoatlicue_CoilStarts[i]) << 2) - 64;
            if (sh > -16) {
              sh = -16;
            }
          }
          if (frame > 273) {
            sh = 1080 - (frame << 2);
            if (i == 0) {
              Matrix_RotateZ((274 - frame) << 10);
            } else {
              Matrix_RotateZ((frame << 9) + -0x22400);
            }
          }
        } else {
          sh = (frame << 2) - (i << 3) - 1240;
          if (sh > -16) {
            sh = -16;
          }
          s = i * 26384 - (frame << 12) + 0x138710;
          if (s <= 16383) {
            continue;
          }
          Matrix_Initialize();
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_Translate(0, toFIXED(32), 0);
          Matrix_ScaleUniform(s);
        }
        if (sh > -64) {
          model->shade = sh;
          Matrix_RotateX((frame - i * 82) * 22 - 3000);
          Matrix_RotateY(-frame << 10);
          Render_TransformVertices(Ram_MapCellBuffer + 0x4400, verts, 64);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
    }
    while (*(volatile u32 *)&work->vblanksSinceTransfer <= 1) {
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  for (i = 0; i != 12; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  BattleEffect_SelectLivingTargets(work->args);
  wait(1);
  Task_Remove(BattleEffect_ArmPaletteHBlankDma);
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  wait(1);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                       SHEET_PALETTE_BYTES);
  Iwram_FillWords(canvas, CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  Palette_BrightenSavedBattleBg(0, 0, 11);
  frame = 0;
  for (; frame != 60; frame++) {
    s32 s;
    s32 lift;
    struct RenderModel *washModel;
    void *washVertices;
    s32 d;

    if (frame == 16) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
    }
    if (frame > 52) {
      Palette_BrightenBgEntries(-4, -4, -4);
    }
    if (frame > 48) {
      s32 fade = frame - 48;

      Palette_BrightenSavedBattleBg(-fade, -fade, 11 - fade * 2);
    }
    if (frame == 0) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
      {
        struct Particle *p = EFFECT_STEPS;

        for (i = 0; i != 256; i++, p++) {
          p->x = INTtoFIXED(Random16() & 127);
          p->y = INTtoFIXED((Random16() & 255) + 120);
          p->velocityX = ((Random16() & 31) - 16) << 13;
          p->velocityY = (8 - (Random16() & 31)) << 12;
          p->z = (Random16() & 7) + 5;
          p->variant = 32;
        }
      }
    }
    for (i = 0; i != 256; i++) {
      struct Particle *p = &EFFECT_STEPS[i];

      if (p->variant >= 0) {
        DRAW_SPARK(p);
      }
    }
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, 0, 16);
          EFFECT_SHOW_DAMAGE(SE_RECOVERY);
          Sound_PlayFar(SE_RECOVERY);
        } else {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 16);
        }
      }
    }
    washVertices = Heap_AllocEwram(512);
    washModel = Render_AllocateModelList(1);
    washModel->texture = &washTexture;
    washTexture.logWidth = 7;
    washTexture.logHeight = 7;
    washModel->mesh = gBattleEffect_LargeSquareMesh;
    washModel->vertices = washVertices;
    washModel->kind = MODEL_SHADED_MAX;
    washTexture.pixels = work->sheet;
    i = 0;
    lift = frame * 8 - 128;
    s = (frame << 13) + -0x20000;
    d = frame - 16;
    for (; i != 3; lift -= 64, s += -0x10000, i++, d -= 8) {
      if (d >= 0) {
        s32 washShade = 0;

        if (d > 15) {
          washShade = (16 - d) << 3;
        }
        washModel->shade = washShade;
        if (washShade > MODEL_SHADE_INVISIBLE) {
          Matrix_Initialize();
          Matrix_Translate(0, INTtoFIXED(112 - lift), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_ScaleUniform(s);
          Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
          Matrix_RotateX(-TRIG_TURN / 16);
          Matrix_RotateY(frame << 10);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                   washVertices, 4);
          Render_DrawModelList(washModel);
        }
      }
    }
    Heap_Release(washModel);
    Heap_Release(washVertices);
    EFFECT_END_FRAME(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef VENGEANCE_FRAMES
#undef VENGEANCE_ARROWS
#undef LIFE_SHEAR_FRAMES
#undef GEYSER_FRAMES
#undef GEYSER_DRAW_FRAMES
#undef GEYSER_PEAK_FRAME
#undef DROP_COUNT
#undef STARS_PER_TARGET
#undef STAR_TOTAL
#undef BUBBLE_COUNT
#undef BUBBLE_FRAMES
#undef EFFECT_STEPS
#undef FILL_GRADIENT
#undef BLIT_CELL
#undef DRAW_SPARK
