/// @file
/// Effect positions, projection and id dispatch helpers: a unit projected onto
/// the screen, the dispatch of the second range of effect kinds, the plain
/// attack's slash and the critical hit's gathering light and crescent, and a
/// unit pushed back by an effect.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "enemy_ids.h"
#include "sounds.h"
#include "battle/battle_summon.h"

/// Projects a unit onto the screen, raised by half its height, from the battle
/// camera.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  struct BattleCamera *camera;
  struct MotionObject *object;
  struct Sprite *info;
  FIXED scale;

  camera = Ram_HeapSlots->cameraState;
  object = BattleObject_GetFar(unit)->object;
  info = Actor_GetSpriteFar(object, 0);
  EFFECT_VIEW_FROM_CAMERA(camera);
  scale = Iwram_MulFX(Matrix_ProjectPoint(&object->x, screen), info->scale);
  screen[1] -=
      Iwram_MulFX(scale, FIXEDtoINT(BattleUnit_GetEffectHeightFar(unit)) >> 1);
  return 0;
}

/// Unused: reads the held keys when the first target is an enemy and does
/// nothing with them.
UNUSED void BattleEffect_ReadKeysForEnemyTarget(void) {
  struct BattleEffectState *state;
  struct BattleEffectParams *args;

  state = Ram_HeapSlots->battleEffect;
  args = state->args;

  // DEAD: nothing uses the held keys read here; the ROM keeps the test and the
  // load.
  if (args->targets[0] >= FIRST_ENEMY_UNIT)
    (void)gInput.held;
}

/// Where the casting impacts' effect kinds start, CASTING_IMPACT_KINDS of
/// them. Below them are the classes' plain attacks (enum AttackEffect), and
/// from EFFECT_ID_CRITICAL_OFFSET on the same attacks landing as critical hits.
#define EFFECT_RANGE_CASTING_IMPACT 100
#define CASTING_IMPACT_KINDS 101

/// Runs a battle effect from the second range of kinds: the casting impacts,
/// the plain attacks' beams and their critical hits' ring and sparks, and the
/// canvas burst.
void BattleEffect_RunByIdRange(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  s32 kind;
  s32 impact;

  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SHEET, 1550);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  state = Ram_HeapSlots->battleEffect;
  state->args = args;
  state->vblanksSinceTransfer = 1;
  kind = args->kind;
  impact = kind - EFFECT_RANGE_CASTING_IMPACT;
  if ((u32)impact < CASTING_IMPACT_KINDS) {
    BattleEffect_RunCastingImpact(args, impact);
  } else if (kind == EFFECT_ID_CRITICAL_OFFSET + ATTACK_EFFECT_BURST ||
             kind == ATTACK_EFFECT_BURST) {
    BattleEffect_RunBallAttack(args);
  } else if (kind >= EFFECT_ID_CRITICAL_OFFSET) {
    BattleEffect_RunCriticalHit(args);
  } else {
    BattleEffect_RunAttackSlash(args);
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

#define BEAM_FRAMES 32 ///< the slash's and the critical hit's length
#define BEAM_SPARKS 64 ///< sparks in state->particles, drawn in pairs
/// The frames the first spark lives, shrinking as it goes; each later one
/// lives a frame longer.
#define BEAM_SPARK_LIFE 16
/// BG1's scroll that shows the canvas, 96 pixels down BG1's map
/// (BattleEffect_SetupCanvasTileMap), from the battle view's top line.
#define BEAM_CANVAS_SCROLL_Y (96 - BATTLE_VIEW_TOP)
/// The critical hit's camera zoom, in percent, held while it gathers.
#define CRITICAL_HIT_ZOOM 130
/// The motes that close in on the attacker, and how far either side of the
/// centre their depth is clamped.
#define CRITICAL_HIT_MOTES 64
#define CRITICAL_HIT_MOTE_DEPTH 60

/// A plain attack: a slash or streak sweeps from the user over its first
/// target, a yellow orb flashes over the target and sparks fly off.
void BattleEffect_RunAttackSlash(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  struct ScreenPoint anchor;
  struct ScreenPoint screen;
  struct BattleEffectState *state;
  void *canvas;
  s32 scrollX;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 kind;
  struct Unit *unit;
  void *draw[2];
  struct MotionObject *target;
  s32 half;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  kind = args->kind;
  unit = Unit_Get(args->user);
  wait(1);
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  wait(1);
  if (kind == ATTACK_EFFECT_RISING_SLASH ||
      kind == ATTACK_EFFECT_RISING_SLASH_LOW) {
    if (args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
    }
  } else {
    if (args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    }
  }
  draw[0] = Ram_HeapSlots->ramCode;
  draw[1] = Ram_HeapSlots->ramCodePaired;
  wait(1);
  if (kind == ATTACK_EFFECT_WAVE) {
    Resource_LoadAndDecompress(RESOURCE_WAVE_SHEET, state->sheet, 1, 1);
  } else if (kind == ATTACK_EFFECT_THRUST) {
    Resource_LoadAndDecompress(RESOURCE_BEAM_IMAGE, state->sheet, 0, 0);
  } else {
    switch (kind) {
    case 0: // a class with no attack effect slashes
    case ATTACK_EFFECT_SLASH:
    case ATTACK_EFFECT_RISING_SLASH:
    case ATTACK_EFFECT_RISING_SLASH_LOW:
      Resource_LoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_A, state->sheet, 1, 1);
      break;
    case ATTACK_EFFECT_CLAW:
      Resource_LoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_B, state->sheet, 1, 1);
      break;
    }
  }
  // An enemy's sparks take the Mars Djinn's colours, a party member's the
  // star crescent's.
  Dma_Set(Resource_GetTableEntry(args->user > ALLY_MAX
                                     ? RESOURCE_MARS_DJINN_SMALL_SHEET
                                     : RESOURCE_STAR_CRESCENT_SHEET),
          (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
  wait(1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, Ram_MapCellBuffer, 1,
                             0);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  // Scroll BG1 so the first target falls on the canvas.
  EffectPosition_ProjectSprite(args->targets[0], &anchor);
  if (args->side == EFFECT_SIDE_ENEMIES)
    scrollX = 96 - anchor.x;
  else
    scrollX = 32 - anchor.x;
  if (scrollX > 0)
    scrollX = 0;
  if (scrollX < -128)
    scrollX = -128;
  anchor.x += scrollX;
  gBgScroll[1].x = scrollX;
  gBgScroll[1].y = BEAM_CANVAS_SCROLL_Y;
  wait(1);
  // Sparks fly from the target, mostly rising.
  target = BattleObject_GetFar(args->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
  for (i = 0; i != BEAM_SPARKS; i++) {
    struct Particle *spark = &state->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
    spark->velocityX = (Random16() & 255) << 10;
    spark->velocityY = ((Random16() & 255) - 32) << 10;
    spark->velocityZ = ((Random16() & 255) - 127) << 10;
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->velocityX = -spark->velocityX;
    spark->variant = i + BEAM_SPARK_LIFE;
  }

  for (frame = 0; frame != BEAM_FRAMES; frame++) {
    if (frame == 5) {
      if (BattleSprite_HasHeavyImpactFar(unit->enemyId))
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    // The target recoils: hurled, hopping, shoved or nudged by the kind.
    if (frame == 4) {
      if (kind == ATTACK_EFFECT_HURL)
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      else if (kind == ATTACK_EFFECT_RISING_SLASH ||
               kind == ATTACK_EFFECT_RISING_SLASH_LOW)
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      else if (kind == ATTACK_EFFECT_SHOVE)
        BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(1),
                                      toFIXED(4), toFIXED(3), 150);
      else
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
    }
    // The slash or streak plays from the user.
    EffectPosition_ProjectScaledBattler(args->user, &screenPosition);
    screenPosition.y += 16;
    if (kind == ATTACK_EFFECT_RISING_SLASH_LOW)
      screenPosition.y += 16;
    if (kind == ATTACK_EFFECT_WAVE) {
      // Six pictures, two frames each, played backwards.
      if (frame <= 11) {
        if (args->side == EFFECT_SIDE_ENEMIES)
          ((DrawRectangle)draw[0])(
              canvas, state->sheet + (5 - frame / 2) * PICTURE_BYTES(WAVE),
              screenPosition.x + scrollX - WAVE_WIDTH,
              screenPosition.y - WAVE_HEIGHT / 2, WAVE_WIDTH, WAVE_HEIGHT);
        else
          ((DrawRectangle)draw[0])(
              canvas, state->sheet + (5 - frame / 2) * PICTURE_BYTES(WAVE),
              screenPosition.x + scrollX, screenPosition.y - WAVE_HEIGHT / 2,
              WAVE_WIDTH, WAVE_HEIGHT);
      }
    } else if ((u32)kind <= ATTACK_EFFECT_CLAW ||
               kind == ATTACK_EFFECT_RISING_SLASH ||
               kind == ATTACK_EFFECT_RISING_SLASH_LOW) {
      // Six pictures, two frames each.
      if (frame <= 11) {
        if (args->side == EFFECT_SIDE_ENEMIES)
          ((DrawRectangle)draw[0])(
              canvas, state->sheet + frame / 2 * PICTURE_BYTES(BLUE_ARC),
              screenPosition.x + scrollX - BLUE_ARC_WIDTH,
              screenPosition.y - 40, BLUE_ARC_WIDTH, BLUE_ARC_HEIGHT);
        else
          ((DrawRectangle)draw[0])(
              canvas, state->sheet + frame / 2 * PICTURE_BYTES(BLUE_ARC),
              screenPosition.x + scrollX, screenPosition.y - 40, BLUE_ARC_WIDTH,
              BLUE_ARC_HEIGHT);
      }
    } else if (kind == ATTACK_EFFECT_THRUST) {
      // Six pictures, three frames each, each reaching further.
      if (frame <= 17) {
        s32 picture = frame / 3;

        if (args->side == EFFECT_SIDE_ENEMIES)
          ((DrawRectangle)draw[0])(
              canvas, state->sheet + gBeam_Pictures[picture],
              screenPosition.x + gBeam_PictureReaches[picture] + scrollX - 58,
              screenPosition.y - gBeam_PictureHeights[picture] / 2,
              gBeam_PictureWidths[picture], gBeam_PictureHeights[picture]);
        else
          ((DrawRectangle)draw[0])(
              canvas, state->sheet + gBeam_Pictures[picture],
              screenPosition.x - gBeam_PictureReaches[picture] + scrollX -
                  gBeam_PictureWidths[picture] + 58,
              screenPosition.y - gBeam_PictureHeights[picture] / 2,
              gBeam_PictureWidths[picture], gBeam_PictureHeights[picture]);
      }
    }
    // The yellow orb flashes over the target, six pictures, two frames each.
    if (frame >= 4 && frame <= 15)
      ((DrawRectangle)draw[0])(canvas,
                               Ram_MapCellBuffer +
                                   (frame - 4) / 2 * PICTURE_BYTES(YELLOW_ORB),
                               anchor.x - 16, anchor.y - YELLOW_ORB_HEIGHT / 2,
                               YELLOW_ORB_WIDTH, YELLOW_ORB_HEIGHT);
    EFFECT_VIEW_FROM_CAMERA(camera);
    // The sparks fly, two to each particle, through the two blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != BEAM_SPARKS; i++) {
        s32 pair = i / 2;
        struct Particle *spark = &state->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Matrix_ProjectPoint(&spark->x, &screen.x);
          screen.x += scrollX;
          size >>= 3;
          size += 2;
          EFFECT_DRAW_FLARE((DrawRectangle)draw[pair & 1], canvas, sheet,
                            screen.x, screen.y, size);
          Particle_AdvanceWithGravity3D(spark, 60, -toFIXED(1) / 64);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleEffect_HideBg1AndRestoreBackdrop();
}

/// A critical hit: motes of light close in on the attacker as it hangs in its
/// leap, then a crescent slash sweeps across the first target and sparks fly
/// from it.
void BattleEffect_RunCriticalHit(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  struct ScreenPoint anchor;
  struct ScreenPoint screen;
  struct ScreenPoint spot;
  struct BattleEffectState *state;
  void *canvas;
  s32 scrollX;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 acceleration;
  s32 strength;
  void *draw[2];
  struct HeapSlots *heap;
  struct MotionObject *object;
  struct MotionObject *target;
  u16 *background;
  s32 bytes = CANVAS_VIEW_BYTES;
  s32 big;
  s32 half;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  // DEAD: never true, as only kinds from EFFECT_ID_CRITICAL_OFFSET reach this
  // runner (BattleEffect_RunByIdRange), so big stays 1; the ROM keeps the
  // compare and the store.
  big = 1;
  if (args->kind < EFFECT_ID_CRITICAL_OFFSET)
    big = 0;
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget,
                                        CRITICAL_HIT_ZOOM);
  wait(1);
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  heap = Ram_HeapSlots;
  draw[0] = heap->ramCode;
  draw[1] = heap->ramCodePaired;
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget,
                                        CRITICAL_HIT_ZOOM);
  wait(1);
  Resource_LoadAndDecompress(RESOURCE_STAR_CRESCENT_SHEET, state->sheet, 1, 0);
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget,
                                        CRITICAL_HIT_ZOOM);
  wait(1);
  Resource_LoadAndDecompress(RESOURCE_GREY_CRESCENT_SHEET, Ram_MapCellBuffer, 1,
                             1);
  // An enemy's sparks take the Mars Djinn's colours.
  if (args->user > ALLY_MAX)
    Dma_Set(Resource_GetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
            (void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget,
                                        CRITICAL_HIT_ZOOM);
  wait(1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget,
                                        CRITICAL_HIT_ZOOM);
  wait(1);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

  // The gathering.
  if (big == 1) {
    object = BattleObject_GetFar(args->user)->object;
    for (i = 0; i != CRITICAL_HIT_MOTES; i++) {
      struct Particle *mote = &state->particles[i];

      SET_POSITION(mote, (Random16() & 63) + 16, 0, 0);
      mote->velocityX = Random16() & TRIG_ANGLE_MASK;
      mote->velocityY = Random16() & TRIG_ANGLE_MASK;
      mote->velocityZ = Random16() & TRIG_ANGLE_MASK;
    }
    Actor_SetAnimationSpeedFar(object, 0);
    velocityX = object->velocityX;
    velocityY = object->velocityY;
    velocityZ = object->velocityZ;
    strength = object->verticalMotionStrength;
    acceleration = object->acceleration;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    EffectPosition_ProjectScaledBattler(args->user, &screenPosition);
    scrollX = 64 - screenPosition.x;
    gBgScroll[1].x = scrollX;
    gBgScroll[1].y = BEAM_CANVAS_SCROLL_Y;
    state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    state->fadeStep = 0;
    Task_Add(BattleEffect_DarkenBackgroundStep, TASK_PASS_FRAME);
    Sound_PlayFar(SE_POWER_UP);
    for (frame = 0; frame != BEAM_FRAMES; frame++) {
      BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget,
                                            CRITICAL_HIT_ZOOM);
      // Mote i starts at frame i / 4, 16 to 79 pixels out on its own axes, and
      // moves in 4 pixels a frame until it reaches the attacker.
      for (i = 0; i != CRITICAL_HIT_MOTES; i++) {
        struct Particle *mote = &state->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Matrix_Initialize();
          Matrix_RotateZ(mote->velocityZ);
          Matrix_RotateX(mote->velocityX);
          Matrix_RotateY(mote->velocityY);
          EffectPosition_ProjectPoint(&mote->x, &screen);
          screen.x += 64;
          screen.y = screen.y + screenPosition.y + 24;
          // DEAD: the depth is clamped and moved to start at 0 but never read;
          // the ROM keeps the clamp and the move.
          CLAMP(screen.depth, -CRITICAL_HIT_MOTE_DEPTH,
                CRITICAL_HIT_MOTE_DEPTH);
          screen.depth += CRITICAL_HIT_MOTE_DEPTH;
          EFFECT_DRAW_FLARE((DrawRectangle)draw[0], canvas, sheet, screen.x,
                            screen.y, size);
          mote->x -= 4;
        }
      }
      EFFECT_PRESENT_CANVAS(state);
    }
    Task_Remove(BattleEffect_DarkenBackgroundStep);
    Actor_SetAnimationSpeedFar(object, ANIM_STEP_NORMAL);
    object->velocityX = velocityX;
    object->velocityY = velocityY;
    object->velocityZ = velocityZ;
    object->acceleration = acceleration;
    object->verticalMotionStrength = strength;
  }

  // The strike.
  Iwram_ClearWordsEntry(canvas, bytes);
  Iwram_ClearWordsEntry((void *)BG_CHAR_BLOCK(1), bytes);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  EffectPosition_ProjectScaledBattler(args->targets[0], &anchor);
  if (args->side == EFFECT_SIDE_ENEMIES)
    scrollX = 32 - anchor.x;
  else
    scrollX = 96 - anchor.x;
  if (scrollX > 0)
    scrollX = 0;
  if (scrollX < -128)
    scrollX = -128;
  anchor.x += scrollX;
  gBgScroll[1].x = scrollX;
  gBgScroll[1].y = BEAM_CANVAS_SCROLL_Y;
  target = BattleObject_GetFar(args->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
  for (i = 0; i != BEAM_SPARKS; i++) {
    struct Particle *spark = &state->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
    spark->velocityX = (Random16() & 255) << 10;
    spark->velocityY = (Random16() & 255) << 10;
    spark->velocityZ = ((Random16() & 255) - 127) << 10;
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->variant = i + BEAM_SPARK_LIFE;
  }

  for (frame = 0; frame != BEAM_FRAMES; frame++) {
    if (frame == 5)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 4)
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
    EffectPosition_ProjectScaledBattler(args->user, &screenPosition);
    screenPosition.y += 16;
    // The slash: a star, an arc, then two crescents.
    if (frame <= 1)
      ((DrawRectangle)draw[0])(canvas, state->sheet, 0, 0, SLASH_PICTURE_SIZE,
                               SLASH_PICTURE_SIZE);
    else if (frame <= 3)
      ((DrawRectangle)draw[0])(canvas, Ram_MapCellBuffer, -4, 0, CANVAS_SIZE,
                               CANVAS_SIZE);
    else if (frame <= 5)
      ((DrawRectangle)draw[0])(canvas, Ram_MapCellBuffer + CANVAS_BYTES, -4, 0,
                               CANVAS_SIZE, CANVAS_SIZE);
    else if (frame <= 7)
      ((DrawRectangle)draw[0])(
          canvas, state->sheet + SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE, 0, 0,
          SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
    EFFECT_VIEW_FROM_CAMERA(camera);
    // The sparks fly, two to each particle, through the two blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != BEAM_SPARKS; i++) {
        s32 pair = i / 2;
        struct Particle *spark = &state->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          EffectPosition_ProjectPoint(&spark->x, &spot);
          spot.x += scrollX;
          size >>= 3;
          size += 2;
          spot.y += 16;
          EFFECT_DRAW_FLARE((DrawRectangle)draw[pair & 1], canvas, sheet,
                            spot.x, spot.y, size);
          Particle_AdvanceWithGravity3D(spark, 60, -toFIXED(1) / 64);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }

  // The background's palette steps back over seven frames.
  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  for (frame = 0, background = &Ram_HeapSlots->battleState->background;
       frame != 7; frame++) {
    BattleBackground_SetPaletteLevelFar(*background, 6 - frame);
    wait(1);
  }
  BattleEffect_HideBg1AndRestoreBackdrop();
}

/// Knocks a unit back in its damage pose with the caller's acceleration, top
/// speed, hop and reach in percent, as BattleMotion_ApplyKnockback does from
/// its tables; a unit already moving up or down always takes the hop, a still
/// one only when `force` is set. The Fusion Dragon and the Doom Dragon's
/// three forms only take the pose.
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent) {
  struct BattleObject *slot;
  struct MotionObject *object;
  u32 enemy;

  slot = BattleObject_GetFar(id);
  object = slot->object;
  enemy = Unit_Get(id)->enemyId;
  if (enemy != ENEMY_FUSION_DRAGON && enemy != ENEMY_DOOM_DRAGON_A &&
      enemy != ENEMY_DOOM_DRAGON_E && enemy != ENEMY_DOOM_DRAGON_H) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
    if (force != 0 || object->velocityY != 0)
      object->velocityY = velocityY;
    Actor_StartIdleFar(object);
    Actor_SetMoveTargetFar(object, slot->anchorX * percent / 100, 0,
                           slot->anchorZ);
  }
  object->verticalMotionStrength = toFIXED(3) / 5;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SelectAnimationFar(object, POSE_DAMAGE);
}

#undef EFFECT_RANGE_CASTING_IMPACT
#undef CASTING_IMPACT_KINDS
#undef BEAM_FRAMES
#undef BEAM_SPARKS
#undef BEAM_SPARK_LIFE
#undef BEAM_CANVAS_SCROLL_Y
#undef CRITICAL_HIT_ZOOM
#undef CRITICAL_HIT_MOTES
#undef CRITICAL_HIT_MOTE_DEPTH
