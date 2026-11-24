/// @file
/// The comets and feathers of Balm, Tinder and Phoenix, the Zagan summon, and
/// the camera turn Fire and Froth share.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "gamepak/far_call/field.h"
#include "resource_ids.h"

#define TUMBLING_QUADS_FRAMES 64
/// The quads given a place at frame 0, of which the first TUMBLING_QUADS are
/// drawn, each TUMBLING_QUADS_DELAY frames after the one before.
#define TUMBLING_QUADS_PLACED 32
#define TUMBLING_QUADS 16
#define TUMBLING_QUADS_DELAY 3
/// The frame every target flashes and the quads start to fade, 8 levels a
/// frame.
#define TUMBLING_QUADS_FLASH_FRAME 56
/// The quads' circle is seen from a little above, tilted by this angle.
#define TUMBLING_QUADS_TILT 3000

/// Where the quads' texture (CometTexture, or FeatherSheet's feather) and the
/// Djinni sit in the sprite sheet; the light line is at its start.
#define TUMBLING_QUADS_TEXTURE 10000
#define TUMBLING_QUADS_DJINNI 26048
/// How far the Djinni sways to either side, in screen pixels.
#define TUMBLING_QUADS_DJINNI_SWAY 20
/// The ring of light lines: TUMBLING_QUADS_LINES lines a pixel wide and
/// TUMBLING_QUADS_LINE_HEIGHT tall, at level TUMBLING_QUADS_LINE_LEVEL, one
/// level dimmer every two frames.
#define TUMBLING_QUADS_LINES 16
#define TUMBLING_QUADS_LINE_HEIGHT 128
#define TUMBLING_QUADS_LINE_LEVEL 40

/// The revival of Balm, Tinder and Phoenix: TUMBLING_QUADS comets or
/// feathers, each a textured quad, tumble down round the targets in a widening
/// circle, or for Phoenix rise from them, TUMBLING_QUADS_DELAY frames apart;
/// with a prelude the Djinni circles above, then climbs away, and a ring of
/// light lines spreads out. At TUMBLING_QUADS_FLASH_FRAME every target flashes
/// and the quads fade.
///
/// Balm, Tinder and Phoenix run it in their modes.
void BattleEffect_RunTumblingQuads(struct BattleEffectParams *args, s32 mode) {
  s32 anchor[3];
  s32 size[3];
  struct ScreenPoint pos;
  struct ModelTexture texture;
  s32 y;
  struct BattleEffectState *state;
  struct Particle *quad;
  s32 x;
  s32 frame;
  struct RenderModel *model;
  void *vertices;
  void *canvas;
  /* LEFTOVER(frame): draws[1], an idle second blitter, which the ROM's frame
   * reserves and never touches; as one DrawRectangle. */
  DrawRectangle draws[2];
  s32 i;
  FIXED height;
  s32 fade;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);

  // The mode's canvas, sprite sheets and palette.
  if (mode == TUMBLING_QUADS_BALM) {
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_TARGET_X),
                                   anchor, size);
    Resource_LoadAndDecompress(RESOURCE_COMET_TEXTURE,
                               state->sheet + TUMBLING_QUADS_TEXTURE, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
                               state->sheet + TUMBLING_QUADS_DJINNI, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                    SHEET_PALETTE_BYTES);
  } else if (mode == TUMBLING_QUADS_TINDER) {
    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MARS, args->side ^ 1,
                                                   DJINNI_ANCHOR_OVER_TARGET),
                                   anchor, size);
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SHEET,
                               state->sheet + TUMBLING_QUADS_DJINNI, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_FEATHER_SHEET,
                               state->sheet + TUMBLING_QUADS_TEXTURE, 1, 1);
  } else {
    EffectPosition_ProjectBattler(args->targets[0], &pos);
    anchor[0] = pos.x;
    Resource_LoadAndDecompress(RESOURCE_FEATHER_SHEET,
                               state->sheet + TUMBLING_QUADS_TEXTURE, 1, 1);
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Centre the canvas on the anchor; the blitter mirrors for side 0.
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (CANVAS_SIZE / 2 - anchor[0]) << 8;
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  draws[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  for (frame = 0; frame != TUMBLING_QUADS_FRAMES; frame++) {
    if (frame == 8)
      Sound_PlayFar(SE_GLITTER);

    // The quads start above the screen, each higher than the last (or, for
    // Phoenix, low by the targets), each with its own heading, spin and size.
    if (frame == 0) {
      height = -toFIXED(32);
      for (i = 0; i != TUMBLING_QUADS_PLACED; i++) {
        quad = &state->particles[i];
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
        quad->variant = (Random16() & 0x3fff) + FIXED_ONE / 8;
        height += -toFIXED(4);
      }
    }

    // The heal shows, and every target flashes.
    if (frame == TUMBLING_QUADS_FLASH_FRAME) {
      Sound_PlayFar(SE_POWER_UP);
      EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
      for (i = 0; i != args->targetCount; i++)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
    }

    // With a prelude the Djinni circles over the anchor and climbs away
    // after frame 32, and a ring of light lines spreads out from the canvas's
    // middle.
    if (args->prelude == 1) {
      x = (FIXEDtoINT(-Trig_Sin(frame * (TRIG_TURN / 32)) *
                      TUMBLING_QUADS_DJINNI_SWAY)) +
          CANVAS_SIZE / 2 - DJINNI_WIDTH / 2;
      y = (FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4)) + anchor[1] - 24;
      if (frame > 32)
        y = y - frame * 4 + 128;
      draws[0](canvas, state->sheet + TUMBLING_QUADS_DJINNI, x, y, DJINNI_WIDTH,
               DJINNI_HEIGHT);
      if (frame <= 3)
        draws[0](canvas, state->sheet + TUMBLING_QUADS_DJINNI, x, y,
                 DJINNI_WIDTH, DJINNI_HEIGHT);
      // DEAD: frame never passes 63 here; the ROM keeps the compare and branch.
      if (frame <= 79) {
        s32 angle;
        s32 lineX;
        s32 lineY;

        Iwram_FillWordsEntry(state->sheet, TUMBLING_QUADS_LINE_HEIGHT,
                             CANVAS_FILL_WORD(TUMBLING_QUADS_LINE_LEVEL) -
                                 frame / 2 * CANVAS_FILL_WORD(1));
        i = 0;
        angle = frame * (TRIG_TURN / 128);
        for (; i != TUMBLING_QUADS_LINES; i++) {
          lineX = (FIXEDtoINT(Trig_Sin(angle) * frame)) + CANVAS_SIZE / 2;
          lineY = (FIXEDtoINT((Trig_Cos(angle) * frame) / 8)) - 16;
          draws[0](canvas, state->sheet, lineX, lineY, 1,
                   TUMBLING_QUADS_LINE_HEIGHT);
          angle += TRIG_TURN / TUMBLING_QUADS_LINES;
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
      if (frame >= TUMBLING_QUADS_FLASH_FRAME)
        fade = (TUMBLING_QUADS_FLASH_FRAME - frame) * 8;
      model->shade = fade;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = state->sheet + TUMBLING_QUADS_TEXTURE;
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = vertices;

      for (i = 0; i != TUMBLING_QUADS; i++) {
        FIXED scale;
        s32 delay;

        quad = &state->particles[i];
        delay = i * TUMBLING_QUADS_DELAY;
        scale = quad->variant;

        if (frame >= delay) {
          FIXED across;
          FIXED deep;

          Matrix_Initialize();
          Matrix_RotateX(TUMBLING_QUADS_TILT);
          if (mode == TUMBLING_QUADS_PHOENIX)
            quad->y -= quad->velocityY;
          else
            quad->y += quad->velocityY;
          if (quad->velocityY < -toFIXED(2))
            quad->z += toFIXED(1);
          else
            quad->z += toFIXED(3) / 4;
          across = Trig_Sin(quad->x) * FIXED_INTEGER_PART(quad->z);
          deep = Trig_Cos(quad->x) * FIXED_INTEGER_PART(quad->z);
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
    EFFECT_END_FRAME(state);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

/// Zagan is drawn by ZAGAN_PARTS sprites, each showing its own picture of
/// zagan.png: part i shows ZAGAN_GALLOP_PICTURES + i as Zagan gallops and,
/// but for the last part, ZAGAN_SWING_PICTURES + i as it swings its axe.
#define ZAGAN_PARTS 12
#define ZAGAN_GALLOP_PICTURES 12
#define ZAGAN_SWING_PICTURES 24

#define ZAGAN_FRAMES 200 ///< frames of the gallop, the swing and the rings
/// The frame Zagan's axe falls: the screen flashes white the frame before, and
/// the targets are struck four times, ZAGAN_STRIKE_INTERVAL frames apart.
#define ZAGAN_BLOW_FRAME 157
#define ZAGAN_STRIKE_INTERVAL 4
/// Zagan and its rings of fire are seen from a little above, tilted by this
/// angle.
#define ZAGAN_RING_TILT 3000
/// Where FlameBandTexture, the rings' texture, is decoded in the map cell
/// buffer.
#define ZAGAN_FLAME_BAND 16384

/// PetalSheet's four 60x100 pictures, a glowing half sphere, drawn in the
/// orange palette as the blaze before Zagan as it gallops.
#define ZAGAN_BLAZE_WIDTH 60
#define ZAGAN_BLAZE_HEIGHT 100
/// The streaks that blow across the field, ZAGAN_STREAKS of them: three rows
/// of a ramp ZAGAN_STREAK_WIDTH long from level ZAGAN_STREAK_LEVEL, after
/// the blaze's pictures.
#define ZAGAN_STREAK (4 * PICTURE_BYTES(ZAGAN_BLAZE))
#define ZAGAN_STREAKS 20
#define ZAGAN_STREAK_WIDTH 32
#define ZAGAN_STREAK_LEVEL 16
/// The mote copies made in the battle sprite sheet, each 7 levels fainter.
#define ZAGAN_FAINT_MOTES 10
/// As the field darkens, each of its colours steps toward the battle
/// background's own, less these levels of red, green and blue.
#define ZAGAN_DARKEN_RED 6
#define ZAGAN_DARKEN_GREEN 10
#define ZAGAN_DARKEN_BLUE 2

/// "Earth's might enflamed." The battle flashes away to a white field and
/// streaks blow across it; Zagan gallops in from the right in an orange blaze
/// as the field darkens to the battle's own backdrop, and rears. At
/// ZAGAN_BLOW_FRAME the screen flashes white, a crescent of light sweeps the
/// targets and Zagan's axe falls, striking them four times while the screen
/// shakes and rings of fire spread. Holding A or B draws only every other
/// frame.
///
/// Abilities: Zagan.
void BattleEffect_RunZagan(struct BattleEffectParams *args) {
  struct BattleState *battle;
  void *canvas;
  struct BattleEffectState *state;
  u8 *fadedMotes;
  struct BattleBackgroundState *backdrop;
  s32 savedScroll;
  DrawRectangle *blit;
  struct Point2D position;
  struct Point2D velocity;
  s32 skip;
  DrawRectangle blitters[2];
  struct Scale zaganScale;
  FIXED placement[4];
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct Particle *p = NULL;
  u8 *ringPixels;
  s32 size;
  s32 bgX;
  s32 frame;
  s32 i;
  s32 j;

  ringPixels = Ram_MapCellBuffer + ZAGAN_FLAME_BAND;
  battle = Ram_HeapSlots->battleState;
  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  fadedMotes = Ram_HeapSlots->battleSheet;
  backdrop = Ram_HeapSlots->battleBackground;
  savedScroll = gBgScroll[1].x;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FillWhiteBand();
  state->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);

  // The battle flashes away as the summons' openings do, stopping while the
  // white covers the screen.
  for (bgX = -80, frame = 0; frame != SUMMON_OPENING_LOAD_FRAME + 1; frame++) {
    if (frame == SUMMON_OPENING_LOAD_FRAME) {
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      BattleEffect_ShowLivingTargets(state->args);
      state->scrollStepX = -8;
      backdrop->busy = 1;
      Iwram_FillWordsEntry((void *)&BG_PLTT_COLOR(6, 0),
                           BATTLE_BACKGROUND_COLORS * 2, COLOR_PAIR(RGB_WHITE));
      Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, state->sheet, 0,
                                 0);
      // DEAD: ever-fainter copies of one mote, which Zagan never draws; the
      // ROM keeps the copies.
      for (j = 0; j != ZAGAN_FAINT_MOTES; j++) {
        size = 4;
        for (i = 0; i != size * size * 2; i++) {
          s32 offset = gBattleEffect_MotePictures[size - 1] + i;
          s32 value = state->sheet[offset];

          if (value != 0) {
            value -= j * 7;
            if (value <= 0) {
              value = 1;
            }
          }
          fadedMotes[size * size * j * 2 + i] = value;
        }
      }
      Resource_LoadAndDecompress(RESOURCE_PETAL_SHEET, state->sheet, 1, 1);
      for (i = 0; i != ZAGAN_STREAK_WIDTH; i++) {
        state->sheet[ZAGAN_STREAK + i] = i + ZAGAN_STREAK_LEVEL;
        state->sheet[ZAGAN_STREAK + ZAGAN_STREAK_WIDTH + i] =
            i + ZAGAN_STREAK_LEVEL;
        state->sheet[ZAGAN_STREAK + ZAGAN_STREAK_WIDTH * 2 + i] =
            i + ZAGAN_STREAK_LEVEL;
      }
    }
    bgX += 4;
    REG_BG2X = bgX << 8;
    EFFECT_PRESENT_CANVAS(state);
  }
  blit = blitters;
  BattleEffect_BuildCanvasBlitters(0, (void **)blit);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  skip = 0;
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                       SHEET_PALETTE_BYTES);
  BattleEffect_ShowLivingTargets(state->args);
  wait(1);
  BattleEffect_CreateSprites(ZAGAN_PARTS, SPRITE_ZAGAN, 1);
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
      Sound_PlayFar(SE_TABLET_RISE_HIGH);
    }
    if ((u32)(frame - 78) <= 31) {
      REG_BLDALPHA = BLEND_ALPHA(16 - (frame - 78) / 2, BLEND_WEIGHT_MAX);
    }
    if (frame == 140) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
    }
    if (frame == ZAGAN_BLOW_FRAME)
      EFFECT_SHOW_DAMAGE(SE_CRASH);
    if (frame == ZAGAN_BLOW_FRAME ||
        frame == ZAGAN_BLOW_FRAME + ZAGAN_STRIKE_INTERVAL ||
        frame == ZAGAN_BLOW_FRAME + ZAGAN_STRIKE_INTERVAL * 2 ||
        frame == ZAGAN_BLOW_FRAME + ZAGAN_STRIKE_INTERVAL * 3) {
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
        Sprite_StartAnimationFar(state->sprites[i], i + ZAGAN_GALLOP_PICTURES);
      }
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      Iwram_FillWordsEntry((void *)&BG_PLTT_COLOR(6, 0),
                           BATTLE_BACKGROUND_COLORS * 2, COLOR_PAIR(RGB_WHITE));
      state->scrollStepX = -8;
      gProjection.centerX = -968;
      gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
      gBgScroll[1].x = savedScroll + 1128;
      Resource_LoadAndDecompress(RESOURCE_PETAL_SHEET, state->sheet, 1, 1);
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                           SHEET_PALETTE_BYTES);
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    gProjection.centerX -= state->scrollStepX;
    gBgScroll[1].x += state->scrollStepX;

    // The field darkens toward the battle background's colours.
    if (!(frame & 1) && (u32)(frame - 32) <= 111) {
      u16 *from = Resource_GetTableEntry(battle->background);
      u16 *to = (u16 *)&BG_PLTT_COLOR(6, 0);

      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, from++, to++) {
        s32 b = RGB_BLUE(*to);
        s32 g = RGB_GREEN(*to);
        s32 r = RGB_RED(*to);
        s32 blue = RGB_BLUE(*from) - ZAGAN_DARKEN_BLUE;
        s32 green = RGB_GREEN(*from) - ZAGAN_DARKEN_GREEN;
        s32 red = RGB_RED(*from) - ZAGAN_DARKEN_RED;

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
      velocity.x += -toFIXED(0.61036);
      velocity.y += toFIXED(1) / 8;
    } else if (frame > 99) {
      velocity.x += -toFIXED(1) / 8;
      velocity.y += toFIXED(1) / 32;
    }
    if (frame == ZAGAN_BLOW_FRAME - 1) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == ZAGAN_BLOW_FRAME) {
      Resource_LoadAndDecompress(RESOURCE_CRESCENT_ROW_SHEET, state->sheet, 1,
                                 0);
      state->shakeFrames = 16;
      for (i = 0; i != ZAGAN_PARTS - 1; i++) {
        Sprite_StartAnimationFar(state->sprites[i], i + ZAGAN_SWING_PICTURES);
      }
      EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    }
    // The crescent of light, for eight frames from the flash.
    if ((u32)(frame - (ZAGAN_BLOW_FRAME - 1)) <= 7) {
      blitters[0](canvas, state->sheet, 12, 0, 58, 120);
    }
    for (i = 0; i != 8; i++) {
      if (frame == i * 6 + 114) {
        state->scrollStepX++;
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

    // The streaks, a row of the field apart, each one or two lines thick.
    if (frame == 0) {
      s32 row;

      for (i = 0, row = 3, p = state->particles; i != ZAGAN_STREAKS;
           i++, p++, row += 6) {
        p->x = (Random16() & 127) - 160;
        p->y = row;
        p->variant = Random16() & 1;
        p->velocityX = (Random16() & 3) + p->variant * 8 + 8;
      }
    }
    if ((u32)frame <= 119) {
      s32 odd;

      for (i = 0, odd = frame & 1, p = state->particles; i != ZAGAN_STREAKS;
           i++, p++) {
        if (!odd || !skip) {
          blitters[0](canvas, state->sheet + ZAGAN_STREAK, p->x, p->y,
                      ZAGAN_STREAK_WIDTH, p->variant + 1);
        }
        p->x += p->velocityX;
        if (p->x > CANVAS_SIZE) {
          p->x -= 160;
        }
      }
    }
    if ((u32)frame <= 195) {
      // Zagan.
      zaganScale = gZagan_UnitScale;
      placement[3] = 0;
      placement[1] = 0;
      if (!(frame & 1) || !skip) {
        if (frame >= ZAGAN_BLOW_FRAME) {
          for (i = 0; i != ZAGAN_PARTS - 1; i++) {
            placement[0] = INTtoFIXED(gZagan_ChargePoints[i][0]) + position.x -
                           toFIXED(32);
            placement[2] = INTtoFIXED(gZagan_ChargePoints[i][1]) + position.y +
                           toFIXED(16);
            Render_ApplyProjectedPlacementFar(state->sprites[i], placement,
                                              (s32 *)&zaganScale, 0);
          }
        } else {
          for (i = 0; i != ZAGAN_PARTS; i++) {
            placement[0] = INTtoFIXED(gZagan_GallopPoints[i][0]) + position.x;
            placement[2] = INTtoFIXED(gZagan_GallopPoints[i][1]) + position.y -
                           toFIXED(16);
            Render_ApplyProjectedPlacementFar(state->sprites[i], placement,
                                              (s32 *)&zaganScale, 0);
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
      // The blaze, a new picture every three frames.
      if ((!(frame & 1) || !skip) && frame <= 119) {
        blit[1](canvas,
                state->sheet + ((frame / 3) & 3) * PICTURE_BYTES(ZAGAN_BLAZE),
                (FIXEDtoINT(position.x / 2)) - 12,
                (FIXEDtoINT(position.y)) - 58, ZAGAN_BLAZE_WIDTH,
                ZAGAN_BLAZE_HEIGHT);
      }
      if (frame == ZAGAN_BLOW_FRAME) {
        Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, ringPixels, 0,
                                   0);
      }
      if (!(frame & 1) || !skip) {
        // Rings of fire from the blow.
        if (frame >= ZAGAN_BLOW_FRAME) {
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
            s32 start = gZagan_RingStarts[k] + ZAGAN_BLOW_FRAME;
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
            Matrix_RotateX(ZAGAN_RING_TILT);
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
    if (frame >= ZAGAN_BLOW_FRAME) {
      if (state->shakeFrames > 0) {
        s32 offsetX = (Random16() & 3) - 8;
        s32 randomY = Random16() & 15;
        s32 offsetY = randomY - 8;

        gBgScroll[1].y = randomY + 24;
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X - offsetX;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - offsetY;
        state->shakeFrames--;
      } else {
        gBgScroll[1].y = state->savedBg1Y;
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
      }
    }
    BattleEffect_CountDownBattlerTints();
    if (!(frame & 1) || !skip) {
      EFFECT_PRESENT_CANVAS(state);
    }
  }
  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  backdrop->busy = 0;
  for (i = 0; i != ZAGAN_PARTS; i++) {
    Sprite_DestroyFar(state->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

/// Turns the battle camera's yaw by the effect's delta: every VBlank turns it
/// at least half the delta, so it keeps turning on VBlanks that end no frame
/// of the effect.
void BattleCamera_ApplyPhasedDelta(void) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  struct BattleCamera *camera = Ram_HeapSlots->cameraState;

  if (state->cameraPhase == CAMERA_PHASE_FRAME_DONE) {
    camera->yaw += state->cameraDelta;
    state->cameraPhase = CAMERA_PHASE_IDLE;
  } else {
    camera->yaw += state->cameraDelta / 2;
    if (state->cameraPhase == CAMERA_PHASE_HALF_TURNED)
      state->cameraPhase = CAMERA_PHASE_IDLE;
    else
      state->cameraPhase = CAMERA_PHASE_HALF_TURNED;
  }
}
#undef TUMBLING_QUADS_FRAMES
#undef TUMBLING_QUADS_PLACED
#undef TUMBLING_QUADS
#undef TUMBLING_QUADS_DELAY
#undef TUMBLING_QUADS_FLASH_FRAME
#undef TUMBLING_QUADS_TILT
#undef TUMBLING_QUADS_TEXTURE
#undef TUMBLING_QUADS_DJINNI
#undef TUMBLING_QUADS_DJINNI_SWAY
#undef TUMBLING_QUADS_LINES
#undef TUMBLING_QUADS_LINE_HEIGHT
#undef TUMBLING_QUADS_LINE_LEVEL
#undef ZAGAN_PARTS
#undef ZAGAN_GALLOP_PICTURES
#undef ZAGAN_SWING_PICTURES
#undef ZAGAN_FRAMES
#undef ZAGAN_BLOW_FRAME
#undef ZAGAN_STRIKE_INTERVAL
#undef ZAGAN_RING_TILT
#undef ZAGAN_FLAME_BAND
#undef ZAGAN_BLAZE_WIDTH
#undef ZAGAN_BLAZE_HEIGHT
#undef ZAGAN_STREAK
#undef ZAGAN_STREAKS
#undef ZAGAN_STREAK_WIDTH
#undef ZAGAN_STREAK_LEVEL
#undef ZAGAN_FAINT_MOTES
#undef ZAGAN_DARKEN_RED
#undef ZAGAN_DARKEN_GREEN
#undef ZAGAN_DARKEN_BLUE
