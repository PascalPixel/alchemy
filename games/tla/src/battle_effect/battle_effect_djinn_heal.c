/// @file
/// Sixteen tumbling textured quads, the Zagan summon, and the camera turned by
/// phases.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "gamepak/far_call/field.h"
#include "resource_ids.h"

#define QUAD_FRAMES 64
#define QUAD_COUNT 16

/// Where the quads' texture and the Djinn sit in the sheet.
#define SHEET_TEXTURE 10000
#define SHEET_DJINN 26048

/// Sixteen textured quads tumble down over the targets (or, from the first
/// target, rise), three frames apart, for 64 frames; with a prelude a Djinn
/// circles over the canvas and a fan of light lines spreads from it. At
/// frame 56 every target flashes.
/// Balm, Tinder and Phoenix run it in their modes.
void BattleEffect_RunTumblingQuads(struct BattleEffectParams *args, s32 mode) {
  s32 anchor[3];
  s32 size[3];
  struct ScreenPoint pos;
  struct ModelTexture texture;
  s32 y;
  struct BattleEffectWork *work;
  struct Particle *quad;
  s32 x;
  s32 frame;
  struct RenderModel *model;
  void *vertices;
  void *canvas;
  // Unused: draws[1] gives the stack frame its 4 bytes.
  DrawRectangle draws[2];
  s32 i;
  FIXED height;
  s32 fade;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);

  // The mode's canvas, sheets and palette.
  if (mode == TUMBLING_QUADS_BALM) {
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_TARGET_X),
                                   anchor, size);
    Resource_LoadAndDecompress(RESOURCE_COMET_TEXTURE,
                               work->sheet + SHEET_TEXTURE, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
                               work->sheet + SHEET_DJINN, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                    SHEET_PALETTE_BYTES);
  } else if (mode == TUMBLING_QUADS_TINDER) {
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MARS, args->side ^ 1,
                                                   DJINNI_ANCHOR_OVER_TARGET),
                                   anchor, size);
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SHEET,
                               work->sheet + SHEET_DJINN, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_FEATHER_SHEET,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  } else {
    EffectPosition_ProjectBattler(args->targets[0], &pos);
    anchor[0] = pos.x;
    Resource_LoadAndDecompress(RESOURCE_FEATHER_SHEET,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Centre the canvas on the anchor; the blitter mirrors for side 0.
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (64 - anchor[0]) << 8;
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  draws[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  for (frame = 0; frame != QUAD_FRAMES; frame++) {
    if (frame == 8)
      Sound_PlayFar(SE_GLITTER);

    // The quads start above the screen (or at the target for mode 2), each
    // with its own heading, spin and size.
    if (frame == 0) {
      height = -toFIXED(32);
      for (i = 0; i != 32; i++) {
        quad = &work->particles[i];
        quad->x = Random16() & TRIG_ANGLE_MASK;
        Random16();
        quad->z = toFIXED(1);
        if (mode == TUMBLING_QUADS_PHOENIX)
          quad->y = toFIXED(32);
        else
          quad->y = height;
        quad->velocityY = ((Random16() & TRIG_ANGLE_MASK) << 1) + toFIXED(1);
        quad->velocityX = Random16() & TRIG_ANGLE_MASK;
        quad->velocityZ = Random16() & TRIG_ANGLE_MASK;
        quad->variant = (Random16() & 0x3fff) + 8192;
        height += -toFIXED(4);
      }
    }

    // Frame 56: the next phase, and every target flashes.
    if (frame == 56) {
      Sound_PlayFar(SE_POWER_UP);
      EFFECT_SHOW_DAMAGE(-1);
      for (i = 0; i != args->targetCount; i++)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
    }

    // With a prelude the Djinn circles over the anchor and climbs away
    // after frame 32, and a fan of light lines spreads from the canvas's
    // middle while the canvas fades.
    if (args->prelude == 1) {
      x = (FIXEDtoINT(-Trig_Sin(frame << 11) * 20)) + 44;
      y = (FIXEDtoINT(Trig_Cos(frame << 11) * 4)) + anchor[1] - 24;
      if (frame > 32)
        y = y - frame * 4 + 128;
      draws[0](canvas, work->sheet + SHEET_DJINN, x, y, DJINNI_WIDTH,
               DJINNI_HEIGHT);
      if (frame <= 3)
        draws[0](canvas, work->sheet + SHEET_DJINN, x, y, DJINNI_WIDTH,
                 DJINNI_HEIGHT);
      // DEAD: frame never passes 63 here; the ROM keeps the compare and branch.
      if (frame <= 79) {
        s32 angle;
        s32 lineX;
        s32 lineY;

        Iwram_FillWordsEntry(work->sheet, 128,
                             CANVAS_FILL_WORD(40) -
                                 frame / 2 * CANVAS_FILL_WORD(1));
        i = 0;
        angle = frame << 9;
        for (; i != 16; i++) {
          lineX = (FIXEDtoINT(Trig_Sin(angle) * frame)) + 64;
          lineY = (FIXEDtoINT((Trig_Cos(angle) * frame) / 8)) - 16;
          draws[0](canvas, work->sheet, lineX, lineY, 1, 128);
          angle += TRIG_TURN / 16;
        }
      }
    }

    // The quads: each a render model of one textured face, placed by its own
    // translation, scale, pitch and roll.
    Matrix_Initialize();
    gProjection.centerY = 104;
    // DEAD: frame never passes 63 here; the ROM keeps the compare and branch.
    if (frame <= 63) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      fade = 0;
      if (frame > 55)
        fade = (56 - frame) * 8;
      model->shade = fade;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + SHEET_TEXTURE;
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = vertices;

      for (i = 0; i != QUAD_COUNT; i++) {
        FIXED scale;
        s32 delay;

        quad = &work->particles[i];
        delay = i * 3;
        scale = quad->variant;

        if (frame >= delay) {
          FIXED across;
          FIXED deep;

          Matrix_Initialize();
          Matrix_RotateX(3000);
          if (mode == TUMBLING_QUADS_PHOENIX)
            quad->y -= quad->velocityY;
          else
            quad->y += quad->velocityY;
          if (quad->velocityY < -toFIXED(2))
            quad->z += toFIXED(1);
          else
            quad->z += toFIXED(3) / 4;
          across = Trig_Sin(quad->x) * ((s16 *)&quad->z)[1];
          deep = Trig_Cos(quad->x) * ((s16 *)&quad->z)[1];
          quad->x += TRIG_TURN / 128;
          Matrix_Translate(across, quad->y, deep);
          Matrix_Scale(scale, scale, scale);
          Matrix_RotateX(quad->velocityX);
          Matrix_RotateZ(quad->velocityZ);
          quad->velocityX += toFIXED(1) / 64;
          quad->velocityZ += toFIXED(1) / 64;
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#define ZAGAN_PARTS 12

#define ZAGAN_FRAMES 200 ///< frames of the gallop, the swing and the dust
/// PetalSheet's four 60x100 pictures, drawn at Zagan's position as it gallops.
#define ZAGAN_PETALS_WIDTH 60
#define ZAGAN_PETALS_HEIGHT 100
/// Three rows of a 32-shade ramp after PetalSheet's pictures, drawn as the
/// streaks that blow across the field.
#define ZAGAN_BAND (4 * PICTURE_BYTES(ZAGAN_PETALS))

/// The screen slides over to a white field and petals blow across it; Zagan
/// (twelve objects) gallops in from the right as the backdrop darkens, rears,
/// and at frame 157 swings its axe across the targets, striking four times
/// while the screen shakes and rings of dust spread. Holding A or B draws
/// only every other frame.
void BattleEffect_RunZagan(struct BattleEffectParams *args) {
  struct BattleWork *battle;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  struct BattleBackgroundWork *backdrop;
  s32 savedScroll;
  DrawRectangle *blit;
  struct Point2D position;
  struct Point2D velocity;
  s32 skip;
  DrawRectangle blitters[2];
  struct Scale request;
  FIXED placement[4];
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct Particle *p;
  s32 none;
  u8 *ringPixels;
  s32 size;
  s32 scroll;
  s32 frame;
  s32 i;
  s32 j;

  none = 0;
  ringPixels =
      Ram_MapCellBuffer + 16384; // the rings' texture, for the whole function
  battle = Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  backdrop = Ram_HeapSlots->battleBackground;
  savedScroll = gBgScroll[1].x;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  RagingHeat_FillGradientCanvas();
  work->transferMode = none;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The screen slides over to the field.
  for (scroll = -80, frame = 0; frame != 28; frame++) {
    if (frame == 27) {
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      BattleEffect_SelectLivingTargets(work->args);
      work->scrollStepX = -8;
      backdrop->busy = 1;
      Iwram_FillWordsEntry((void *)&BG_PLTT_COLOR(6, 0), 256,
                           COLOR_PAIR(RGB_WHITE));
      Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, work->sheet, 0,
                                 0);
      // Ten ever-fainter copies of one mote.
      for (j = 0; j != 10; j++) {
        size = 4;
        for (i = 0; i != size * size * 2; i++) {
          s32 offset = gBattleEffect_MotePictures[size - 1] + i;
          s32 value = work->sheet[offset];

          if (value != 0) {
            value -= j * 7;
            if (value <= 0) {
              value = 1;
            }
          }
          sprites[size * size * j * 2 + i] = value;
        }
      }
      Resource_LoadAndDecompress(RESOURCE_PETAL_SHEET, work->sheet, 1, 1);
      for (i = 0; i != 32; i++) {
        work->sheet[ZAGAN_BAND + i] = i + 16;
        work->sheet[ZAGAN_BAND + 32 + i] = i + 16;
        work->sheet[ZAGAN_BAND + 64 + i] = i + 16;
      }
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    EFFECT_PRESENT_CANVAS(work);
  }
  blit = blitters;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  skip = 0;
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                       SHEET_PALETTE_BYTES);
  BattleEffect_SelectLivingTargets(work->args);
  wait(1);
  BattleEffect_SpawnObjects(ZAGAN_PARTS, SPRITE_ZAGAN, 1);
  REG_BG2X = skip;
  REG_BG2PA = Q_8_8(0.5);

  for (frame = 0; frame != ZAGAN_FRAMES; frame++) {
    if (gInput.repeat & (KEY_A | KEY_B)) {
      skip = 1;
    }
    if (frame == 0) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 32) {
      Sound_PlayFar(SE_191);
    }
    if ((u32)(frame - 78) <= 31) {
      REG_BLDALPHA = BLEND_ALPHA(16 - (frame - 78) / 2, BLEND_WEIGHT_MAX);
    }
    if (frame == 140) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
    }
    if (frame == 157)
      EFFECT_SHOW_DAMAGE(SE_CRASH);
    if (frame == 157 || frame == 161 || frame == 165 || frame == 169) {
      for (i = 0; i != args->targetCount; i++) {
        BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 4);
      }
    }
    if (frame == 0) {
      backdrop->busy = 1;
      position.x = toFIXED(240);
      position.y = toFIXED(64);
      velocity.x = -toFIXED(20);
      velocity.y = 0;
      for (i = 0; i != ZAGAN_PARTS; i++) {
        Sprite_StartAnimationFar(work->sprites[i], i + 12);
      }
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      Iwram_FillWordsEntry((void *)&BG_PLTT_COLOR(6, 0), 256,
                           COLOR_PAIR(RGB_WHITE));
      work->scrollStepX = -8;
      gProjection.centerX = -968;
      gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
      gBgScroll[1].x = savedScroll + 1128;
      Resource_LoadAndDecompress(RESOURCE_PETAL_SHEET, work->sheet, 1, 1);
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                           SHEET_PALETTE_BYTES);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    gProjection.centerX -= work->scrollStepX;
    gBgScroll[1].x += work->scrollStepX;

    // The backdrop darkens toward the battle's palette.
    if (!(frame & 1) && (u32)(frame - 32) <= 111) {
      u16 *from = Resource_GetTableEntry(battle->background);
      u16 *to = (u16 *)&BG_PLTT_COLOR(6, 0);

      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, from++, to++) {
        s32 b = RGB_BLUE(*to);
        s32 g = RGB_GREEN(*to);
        s32 r = RGB_RED(*to);
        s32 blue = RGB_BLUE(*from) - 2;
        s32 green = RGB_GREEN(*from) - 10;
        s32 red = RGB_RED(*from) - 6;

        if (blue < 0) {
          blue = 0;
        }
        if (green < 0) {
          green = 0;
        }
        if (red < 0) {
          red = 0;
        }
        if (b > blue) {
          b--;
        }
        if (g > green) {
          g--;
        }
        if (r > red) {
          r--;
        }
        *to = RGB(r, g, b);
      }
    }
    if ((u32)(frame - 64) <= 35) {
      velocity.x += toFIXED(1) / 2;
      velocity.y += -toFIXED(1) / 16;
    }
    if (frame > 177) {
      velocity.x += -toFIXED(2);
      velocity.y += -toFIXED(1);
    } else if (frame > 139) {
      velocity.x += -40000;
      velocity.y += toFIXED(1) / 8;
    } else if (frame > 99) {
      velocity.x += -toFIXED(1) / 8;
      velocity.y += toFIXED(1) / 32;
    }
    if (frame == 156) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == 157) {
      Resource_LoadAndDecompress(RESOURCE_CRESCENT_ROW_SHEET, work->sheet, 1,
                                 0);
      work->shakeFrames = 16;
      for (i = 0; i != ZAGAN_PARTS - 1; i++) {
        Sprite_StartAnimationFar(work->sprites[i], i + 24);
      }
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    if ((u32)(frame - 156) <= 7) {
      blitters[0](canvas, work->sheet, 12, 0, 58, 120);
    }
    for (i = 0; i != 8; i++) {
      if (frame == i * 6 + 114) {
        work->scrollStepX++;
      }
    }
    if (frame == 163) {
      gBgScroll[1].x += -20;
    }
    if (frame == 167) {
      gBgScroll[1].x += -10;
    }
    if (frame == 171) {
      gBgScroll[1].x += -10;
    }

    // Petals on the wind.
    if (frame == 0) {
      s32 row;

      for (i = 0, row = 3, p = work->particles; i != 20; i++, p++, row += 6) {
        p->x = (Random16() & 127) - 160;
        p->y = row;
        p->variant = Random16() & 1;
        p->velocityX = (Random16() & 3) + p->variant * 8 + 8;
      }
    }
    if ((u32)frame <= 119) {
      s32 odd;

      for (i = 0, odd = frame & 1, p = work->particles; i != 20; i++, p++) {
        if (!odd || !skip) {
          blitters[0](canvas, work->sheet + ZAGAN_BAND, p->x, p->y, 32,
                      p->variant + 1);
        }
        p->x += p->velocityX;
        if (p->x > 128) {
          p->x -= 160;
        }
      }
    }
    if ((u32)frame <= 195) {
      // Zagan.
      request = gZagan_UnitScale;
      placement[3] = 0;
      placement[1] = 0;
      if (!(frame & 1) || !skip) {
        if (frame > 156) {
          for (i = 0; i != ZAGAN_PARTS - 1; i++) {
            placement[0] = INTtoFIXED(gZagan_ChargePoints[i][0]) + position.x -
                           toFIXED(32);
            placement[2] = INTtoFIXED(gZagan_ChargePoints[i][1]) + position.y +
                           toFIXED(16);
            Render_ApplyProjectedPlacementFar(work->sprites[i], placement,
                                              (s32 *)&request, 0);
          }
        } else {
          for (i = 0; i != ZAGAN_PARTS; i++) {
            placement[0] = INTtoFIXED(gZagan_GallopPoints[i][0]) + position.x;
            placement[2] = INTtoFIXED(gZagan_GallopPoints[i][1]) + position.y -
                           toFIXED(16);
            Render_ApplyProjectedPlacementFar(work->sprites[i], placement,
                                              (s32 *)&request, 0);
          }
        }
      }
      if (frame > 177) {
        position.x += velocity.x;
        position.y += velocity.y;
        velocity.x = velocity.x * 63 / DAMPING_SCALE;
        velocity.y = velocity.y * 63 / DAMPING_SCALE;
      } else if (frame > 149) {
        if (position.y >= toFIXED(56)) {
          velocity.x = 0;
          velocity.y = 0;
        }
        position.x += velocity.x;
        position.y += velocity.y;
      } else if (frame <= 59) {
        position.x += velocity.x;
        position.y += velocity.y;
        velocity.x = velocity.x * 58 / DAMPING_SCALE;
        velocity.y = velocity.y * 62 / DAMPING_SCALE;
      } else {
        position.x += velocity.x;
        position.y += velocity.y;
        velocity.x = velocity.x * 58 / DAMPING_SCALE;
        velocity.y = velocity.y * 61 / DAMPING_SCALE;
      }
      if ((!(frame & 1) || !skip) && frame <= 119) {
        blit[1](canvas,
                work->sheet + ((frame / 3) & 3) * PICTURE_BYTES(ZAGAN_PETALS),
                (FIXEDtoINT(position.x / 2)) - 12,
                (FIXEDtoINT(position.y)) - 58, ZAGAN_PETALS_WIDTH,
                ZAGAN_PETALS_HEIGHT);
      }
      if (frame == 157) {
        Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, ringPixels, 0,
                                   0);
      }
      if (!(frame & 1) || !skip) {
        // Rings of dust from the blow.
        if (frame > 156) {
          s32 k;

          vertices = Heap_AllocEwram(1024);
          model = Render_AllocateModelList(1);
          texture.logWidth = 7;
          texture.logHeight = 7;
          texture.pixels = ringPixels;
          model->texture = &texture;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->mesh = gBattleEffect_CylinderMesh;
          model->vertices = vertices;
          model->scrollU = 0;
          model->scrollV = 0;
          for (k = 0; k != 4; k++) {
            s32 start = gZagan_RingStarts[k] + 157;
            FIXED scale;
            s32 shade;

            if (frame <= start) {
              continue;
            }
            scale = (((frame - start) * gZagan_RingGrowth[k] * 125) << 4) +
                    FIXED_ONE / 2;
            shade = (start - frame) * 8 + 56;
            if (shade > 0) {
              shade = 0;
            }
            if (shade <= MODEL_SHADE_INVISIBLE) {
              continue;
            }
            model->shade = shade;
            Matrix_Initialize();
            Matrix_Translate(-toFIXED(16), INTtoFIXED(gZagan_RingHeights[k]),
                             0);
            Matrix_Scale(scale, scale * 4 / 3, scale);
            Matrix_RotateX(3000);
            model->scrollU = ((frame + k * 5) & 7) * 16;
            Render_TransformVertices(gBattleEffect_CylinderCorners, vertices,
                                     32);
            Render_DrawModelList(model);
          }
          Heap_Release(model);
          Heap_Release(vertices);
        }
      }
    }
    if (frame > 156) {
      if (work->shakeFrames > 0) {
        s32 offsetX = (Random16() & 3) - 8;
        s32 randomY = Random16() & 15;
        s32 offsetY = randomY - 8;

        gBgScroll[1].y = randomY + 24;
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X - offsetX;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - offsetY;
        work->shakeFrames--;
      } else {
        gBgScroll[1].y = work->savedBg1Y;
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
      }
    }
    BattleEffect_CountDownBattlerTints();
    if (!(frame & 1) || !skip) {
      EFFECT_PRESENT_CANVAS(work);
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  backdrop->busy = 0;
  for (i = 0; i != ZAGAN_PARTS; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

/// A task: turns the camera by the effect's delta, the whole
/// delta on phase 1 and half of it on the frames in between.
void BattleCamera_ApplyPhasedDelta(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

  if (work->cameraPhase == 1) {
    camera->yaw += work->cameraDelta;
    work->cameraPhase = 0;
  } else {
    camera->yaw += work->cameraDelta / 2;
    if (work->cameraPhase == 2)
      work->cameraPhase = 0;
    else
      work->cameraPhase = 2;
  }
}
