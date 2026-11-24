/// @file
/// Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt, Stun Muscle,
/// Living Armor), and the handlers that play the mirrored wall.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define RUSHING_STRIKE_FRAMES 60
#define RUSHING_STRIKE_FRAME 8 ///< the user strikes; the ring and sparks start
#define RUSHING_STRIKE_BURST_FRAMES 15 ///< the burst hides the user rushing in
#define RUSHING_STRIKE_RING_FRAMES 12  ///< the ring where it hits swells
#define RUSHING_STRIKE_SPARK_COUNT 64  ///< in EXTRA_PARTICLES
/// The first two sparks' frames to live; each pair after lives one more.
#define RUSHING_STRIKE_SPARK_LIFE 16
#define RUSHING_STRIKE_EXPLOSIONS 16 ///< Living Armor's
/// The explosions set up at the hit, of which RUSHING_STRIKE_EXPLOSIONS are
/// drawn.
#define RUSHING_STRIKE_EXPLOSION_SLOTS 64
#define RUSHING_STRIKE_EXPLOSION_PICTURE_FRAMES 4
/// FlashBurstSheet's six pictures, four frames apiece.
#define RUSHING_STRIKE_EXPLOSION_FRAMES 24

/// The burst: FlamePlumeSheet's 32x32 plume above the same plume upside down.
#define RUSHING_STRIKE_BURST_WIDTH 32
#define RUSHING_STRIKE_BURST_HEIGHT 64
#define RUSHING_STRIKE_PLUME_SIZE 32
/// Where FlashBurstSheet is decoded in the effect's pictures, after the burst.
#define RUSHING_STRIKE_EXPLOSION_PICTURES PICTURE_BYTES(RUSHING_STRIKE_BURST)

/// The user rushes at the first target in a burst of light; a ring flares where
/// it hits and sparks scatter, and Living Armor's hit raises explosions.
void BattleEffect_RunRushingStrike(struct BattleEffectParams *args, s32 mode) {
  struct ModelTexture texture;
  DrawRectangle draw[2];
  u8 *ringPixels = Ram_MapCellBuffer;
  s32 height;
  struct ScreenPoint target;
  struct ScreenPoint caster;
  struct ScreenPoint screen;
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  s32 frame;
  struct BattleCamera *camera = Ram_HeapSlots->cameraState;
  u8 *motePictures = Ram_HeapSlots->battleSheet;
  struct MotionObject *targetObject;
  struct RenderModel *quad;
  void *vertices;
  s32 i;
  s32 row;
  s32 column;
  s32 scale;
  s32 ringFrame;
  s32 brightness;
  s32 resource;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motePictures, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, Ram_MapCellBuffer, 0,
                             0);

  // The burst: the plume, then the same plume upside down below it.
  Resource_LoadAndDecompress(RESOURCE_FLAME_PLUME_SHEET, state->sheet, 0, 0);
  for (row = 0; row != RUSHING_STRIKE_PLUME_SIZE; row++)
    for (column = 0; column != RUSHING_STRIKE_PLUME_SIZE; column++)
      state->sheet[RUSHING_STRIKE_PLUME_SIZE * RUSHING_STRIKE_PLUME_SIZE +
                   (RUSHING_STRIKE_PLUME_SIZE - 1 - row) *
                       RUSHING_STRIKE_PLUME_SIZE +
                   column] =
          state->sheet[row * RUSHING_STRIKE_PLUME_SIZE + column];
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + RUSHING_STRIKE_EXPLOSION_PICTURES,
                             1, 0);
  switch (mode) {
  case RUSHING_STRIKE_UNUSED:
    resource = RESOURCE_FIRE_PILLAR_SHEET_B;
    break;
  case RUSHING_STRIKE_BRUTE_FORCE:
    resource = RESOURCE_ICE_BLOCK_SHEET;
    break;
  case RUSHING_STRIKE_ONSLAUGHT:
    resource = RESOURCE_PINK_BURST_SHEET;
    break;
  case RUSHING_STRIKE_HEADBUTT:
  case RUSHING_STRIKE_LIVING_ARMOR:
    resource = RESOURCE_BLAST_SHEET;
    break;
  default:
    resource = RESOURCE_ORANGE_PALETTE_B;
    break;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(resource),
                       SHEET_PALETTE_BYTES);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // The user rushes at the target.
  BattleEffect_SetApproachMotion(args->user, args->targets[0], 10, 80);
  Actor_SetAnimationSpeedFar(BattleObject_GetFar(args->user)->object,
                             ANIM_STEP_NORMAL * 3);
  targetObject = BattleObject_GetFar(args->targets[0])->object;
  EffectPosition_ProjectBattler(args->targets[0], &target);

  for (frame = 0; frame != RUSHING_STRIKE_FRAMES; frame++) {
    // The first frame readies the sparks and Living Armor's explosions at the
    // target, 8 pixels out from it, flying back the way the user came.
    if (frame == 0) {
      struct Particle *spark = EXTRA_PARTICLES;

      for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++, spark++) {
        spark->x = targetObject->x;
        if (spark->x < 0)
          spark->x += -toFIXED(8);
        else
          spark->x += toFIXED(8);
        spark->y = targetObject->y + toFIXED(14);
        spark->z = targetObject->z;
        spark->velocityX = (Random16() & 511) << 11;
        spark->velocityY = ((Random16() & 255) - 64) << 11;
        spark->velocityZ = ((Random16() & 255) - 128) << 11;
        if (spark->x > 0)
          spark->velocityX = -spark->velocityX;
        spark->variant = i / 2 + RUSHING_STRIKE_SPARK_LIFE;
      }
      for (i = 0; i != RUSHING_STRIKE_EXPLOSION_SLOTS; i++) {
        struct Particle *explosion = &state->particles[i];
        explosion->x = targetObject->x;
        if (explosion->x < 0)
          explosion->x += -toFIXED(8);
        else
          explosion->x += toFIXED(8);
        explosion->y = targetObject->y + toFIXED(14);
        explosion->z = targetObject->z;
        explosion->velocityX = ((Random16() & 511) - 256) << 10;
        explosion->velocityY = ((Random16() & 255) - 64) << 10;
        explosion->velocityZ = ((Random16() & 255) - 128) << 10;
        if (explosion->x > 0)
          explosion->velocityX = -explosion->velocityX;
        explosion->variant = 0;
      }
    }

    // The burst at the user as it rushes in.
    EffectPosition_ProjectBattler(args->user, &caster);
    vertices = Heap_AllocEwram(32);
    quad = Render_AllocateModelList(1);
    texture.logWidth = 5;
    brightness = 0;
    texture.logHeight = 6;
    texture.pixels = state->sheet;
    quad->texture = &texture;
    quad->mesh = gBattleEffect_TallMesh;
    quad->kind = MODEL_SHADED_MAX;
    quad->vertices = vertices;
    quad->shade = brightness;
    if (frame < RUSHING_STRIKE_BURST_FRAMES) {
      Matrix_Initialize();
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
      Matrix_Translate(INTtoFIXED(caster.x - 128), INTtoFIXED(caster.y - 88),
                       0);
      Matrix_Scale(gRushingStrike_Scales[mode] << 1,
                   gRushingStrike_Scales[mode] << 1,
                   gRushingStrike_Scales[mode]);
      if (args->side == EFFECT_SIDE_PARTY)
        Matrix_RotateY(DEGtoANG(180));
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(quad);
    }

    // The ring where it hits swells, four times as tall as it is wide, and
    // darkens after its fifth frame.
    ringFrame = frame - RUSHING_STRIKE_FRAME;
    if (frame >= RUSHING_STRIKE_FRAME &&
        frame < RUSHING_STRIKE_FRAME + RUSHING_STRIKE_RING_FRAMES) {
      if (mode == RUSHING_STRIKE_STUN_MUSCLE)
        scale = ringFrame * (FIXED_ONE / 16) + FIXED_ONE / 16;
      else
        scale = ringFrame * 3048 + 3048;
      brightness = 32 - ringFrame * 8;
      if (brightness > 0)
        brightness = 0;
      quad->shade = brightness;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = ringPixels;
      quad->kind = MODEL_SHADED_ADDITIVE;
      quad->mesh = gBattleEffect_LargeSquareMesh;
      Matrix_Initialize();
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
      if (args->side == EFFECT_SIDE_ENEMIES)
        Matrix_Translate(INTtoFIXED(target.x - 144), INTtoFIXED(target.y - 84),
                         0);
      else
        Matrix_Translate(INTtoFIXED(target.x - 112), INTtoFIXED(target.y - 84),
                         0);
      Matrix_Scale(scale, scale << 2, scale);
      Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
      Render_DrawModelList(quad);
    }
    Heap_Release(quad);
    Heap_Release(vertices);

    if (frame == RUSHING_STRIKE_FRAME) {
      EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_HURL);
      if (mode == RUSHING_STRIKE_LIVING_ARMOR)
        EFFECT_SHOW_DAMAGE(SE_CRASH);
      else
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      state->shakeFrames = 8;
    }

    // From the hit on, the sparks fly and burn down, drawn by turns with
    // the two blitters; their sprites shrink as they burn.
    // DEAD: frame < 64 is always true, as the loop ends at
    // RUSHING_STRIKE_FRAMES; the ROM keeps the compare.
    if (frame >= RUSHING_STRIKE_FRAME && frame < 64) {
      struct Particle *spark;

      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      spark = EXTRA_PARTICLES;
      for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++, spark++) {
        s32 life = spark->variant;

        if (life >= 0) {
          s32 size = (life >> 4) + 2;

          EffectPosition_ProjectPoint(&spark->x, &screen);
          screen.x >>= 1;
          height = size * 2;
          draw[i & 1](canvas,
                      motePictures + gBattleEffect_MotePictures[size - 1],
                      screen.x - size / 2, screen.y - size, size, height);
          Particle_AdvanceWithGravity3D(spark, 60, -toFIXED(1) / 128);
          spark->variant--;
        }
      }

      // Living Armor's explosions rise, flashing and burning out.
      if (mode == RUSHING_STRIKE_LIVING_ARMOR) {
        for (i = 0; i != RUSHING_STRIKE_EXPLOSIONS; i++) {
          struct Particle *explosion = &state->particles[i];
          s32 life = explosion->variant;

          if (life >= 0 && life < RUSHING_STRIKE_EXPLOSION_FRAMES) {
            s32 picture = life / RUSHING_STRIKE_EXPLOSION_PICTURE_FRAMES;

            EffectPosition_ProjectPoint(&explosion->x, &screen);
            screen.x >>= 1;
            draw[0](canvas,
                    state->sheet + RUSHING_STRIKE_EXPLOSION_PICTURES +
                        picture * PICTURE_BYTES(FLASH_BURST),
                    screen.x - FLASH_BURST_WIDTH / 2,
                    screen.y - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
                    FLASH_BURST_HEIGHT);
            Particle_AdvanceWithGravity3D(explosion, 60, toFIXED(1) / 8);
            explosion->variant++;
          }
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Spasm.
void BattleEffect_RunSpasm(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_SPASM);
}

/// Abilities: Berserk.
void BattleEffect_RunBerserk(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_BERSERK);
}

/// Abilities: Recovery.
void BattleEffect_RunRecovery(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_RECOVERY);
}

/// Abilities: Sack.
void BattleEffect_RunSack(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_SACK);
}
#undef RUSHING_STRIKE_FRAMES
#undef RUSHING_STRIKE_FRAME
#undef RUSHING_STRIKE_BURST_FRAMES
#undef RUSHING_STRIKE_RING_FRAMES
#undef RUSHING_STRIKE_SPARK_COUNT
#undef RUSHING_STRIKE_SPARK_LIFE
#undef RUSHING_STRIKE_EXPLOSIONS
#undef RUSHING_STRIKE_EXPLOSION_SLOTS
#undef RUSHING_STRIKE_EXPLOSION_PICTURE_FRAMES
#undef RUSHING_STRIKE_EXPLOSION_FRAMES
#undef RUSHING_STRIKE_BURST_WIDTH
#undef RUSHING_STRIKE_BURST_HEIGHT
#undef RUSHING_STRIKE_PLUME_SIZE
#undef RUSHING_STRIKE_EXPLOSION_PICTURES
