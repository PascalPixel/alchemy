/// @file
/// Demon Night, Rumble and Haunting.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Sixteen pink lights burst out of the user and are pulled into each target,
/// with the heavy impact sound.
///
/// Abilities: Demon Night.
void BattleEffect_RunDemonNight(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_DEMON_NIGHT);
}
/// Eight lights in Mars Djinn colours burst out of the user and are pulled into
/// each target.
///
/// Abilities: Rumble.
void BattleEffect_RunRumble(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_RUMBLE);
}
/// Twelve lights in Mercury Djinn colours burst out of the user and are pulled
/// into each target.
///
/// Abilities: Haunting.
void BattleEffect_RunHaunting(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_HAUNTING);
}
#define GATHERING_LIGHTS_SLOTS 128 ///< particles reserved for each target
/// The targets' turns, this many frames apart: each is struck by its lights
/// from GATHERING_LIGHTS_HIT_FRAME.
#define GATHERING_LIGHTS_TARGET_FRAMES 20
#define GATHERING_LIGHTS_HIT_FRAME 70
/// MemberBurstImage's three lights.
#define MEMBER_BURST_WIDTH 20
#define MEMBER_BURST_HEIGHT 32
/// How far the canvas's ripple sways each line either side of its middle, in
/// CANVAS_WAVE_BG2X's quarter canvas pixels (4 canvas pixels, 8 on screen); the
/// wave moves a 64th of a turn a line and a frame.
#define GATHERING_LIGHTS_RIPPLE 16

/// For each target in turn, lights burst out of the user and are pulled into
/// the middle of the target's body, which is struck as they arrive; the effect
/// layer ripples throughout.
void BattleEffect_RunGatheringLights(struct BattleEffectParams *args,
                                     s32 mode) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 k;
  struct BattleCamera *camera;
  s32 paletteId;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_MEMBER_BURST_IMAGE),
                      state);
  if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
    paletteId = RESOURCE_PINK_BURST_SHEET;
  else if (mode == GATHERING_LIGHTS_RUMBLE)
    paletteId = RESOURCE_MARS_DJINN_SHEET;
  else
    paletteId = RESOURCE_MERCURY_DJINN_SHEET;
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(paletteId),
                  SHEET_PALETTE_BYTES);
  BattleEffect_FetchRectangleBlitters(state->args->side, draw);

  // The lights fly off the user in every direction.
  EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
  EFFECT_FOR_EACH_TARGET(k, state->args) {
    struct Actor *user = GetBattleObjectSlotFar(state->args->user)->actor;
    s32 height = BattleUnit_GetEffectHeightFar(state->args->user);

    for (i = 0; i != GATHERING_LIGHTS_SLOTS; i++) {
      struct Particle *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

      SET_POSITION(mote, user->x.fixed, height, user->z.fixed);
      SET_VELOCITY(mote, ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10);
      mote->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ENEMY_CRY);

  for (frame = 0;
       frame != gGatheringLights_Counts[mode][GATHERING_LIGHTS_FRAMES] +
                    state->args->targetCount * GATHERING_LIGHTS_TARGET_FRAMES;
       frame++) {
    s32 *row;

    if (frame == 80) {
      if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);

    // Ripple the canvas line by line.
    row = state->bg2xLines;
    for (i = 0; i != DISPLAY_HEIGHT; i++)
      *row++ = CANVAS_WAVE_BG2X(toFIXED(GATHERING_LIGHTS_RIPPLE) -
                                Trig_Sin((frame + i) * (TRIG_TURN / 64)) *
                                    GATHERING_LIGHTS_RIPPLE);

    EFFECT_FOR_EACH_TARGET(k, state->args) {
      struct Actor *target =
          GetBattleObjectSlotFar(state->args->targets[k])->actor;
      s32 half = BattleUnit_GetEffectHeightFar(state->args->targets[k]) / 2;

      if (frame ==
          k * GATHERING_LIGHTS_TARGET_FRAMES + GATHERING_LIGHTS_HIT_FRAME + 1) {
        if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
          Sound_PlayFar(SE_HEAVY_IMPACT);
        else
          Sound_PlayFar(SE_IMPACT);
      }
      if (frame ==
          k * GATHERING_LIGHTS_TARGET_FRAMES + GATHERING_LIGHTS_HIT_FRAME)
        BattleEffect_TintBattler(state->args->targets[k], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, k, 26);
      if (frame > k * GATHERING_LIGHTS_TARGET_FRAMES) {
        for (i = 0; i != gGatheringLights_Counts[mode][GATHERING_LIGHTS_COUNT];
             i++) {
          struct Particle *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

          // Each light leaves two frames after the one before it.
          if (frame > (k * (GATHERING_LIGHTS_TARGET_FRAMES / 2) + i) * 2 &&
              mote->variant >= 0) {
            Projection_Point(mote, &screenPosition);
            screenPosition.x >>= 1;
            draw[0](canvas, state->sheet + i % 3 * PICTURE_BYTES(MEMBER_BURST),
                    screenPosition.x - MEMBER_BURST_WIDTH / 2,
                    screenPosition.y - MEMBER_BURST_HEIGHT / 2,
                    MEMBER_BURST_WIDTH, MEMBER_BURST_HEIGHT);
            Particle_AdvanceWithGravity3D(mote, 62, 0);
            // Thirty frames on, the light is pulled toward the middle of its
            // target's body, its speed gaining a 512th of the distance a frame,
            // and is gone once within 32 pixels of it across and in depth.
            if (frame > k * GATHERING_LIGHTS_TARGET_FRAMES + i + 30) {
              s32 dx = (target->x.fixed - mote->x) >> 9;
              s32 dy = (target->y.fixed + half - mote->y) >> 9;
              s32 dz = (target->z.fixed - mote->z) >> 9;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx >= -4095 && dx <= 4095) && (dz >= -4095 && dz <= 4095))
                mote->variant = -1;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef GATHERING_LIGHTS_SLOTS
#undef GATHERING_LIGHTS_RIPPLE
#undef GATHERING_LIGHTS_TARGET_FRAMES
#undef GATHERING_LIGHTS_HIT_FRAME
#undef MEMBER_BURST_WIDTH
#undef MEMBER_BURST_HEIGHT
