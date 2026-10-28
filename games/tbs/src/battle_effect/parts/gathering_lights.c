/// @file
/// Demon Night, Rumble and Haunting.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// The lights take 128 slots of PARTICLES per target, and ripple the effect
// layer throughout through the HBlank DMA of BG2's affine origin.

// Lights that scatter from the user and gather into each target.

/// Sixteen pink lights burst out of the user and are pulled into each target,
/// with the heavy impact sound.
///
/// Abilities: Demon Night (BATTLE_EFFECT_DEMON_NIGHT).
void BattleEffect_RunDemonNight(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_DEMON_NIGHT);
}
/// Eight lights in Mars Djinn colours burst out of the user and are pulled into
/// each target.
///
/// Abilities: Rumble (BATTLE_EFFECT_RUMBLE).
void BattleEffect_RunRumble(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_RUMBLE);
}
/// Twelve lights in Mercury Djinn colours burst out of the user and are pulled
/// into each target.
///
/// Abilities: Haunting (BATTLE_EFFECT_HAUNTING).
void BattleEffect_RunHaunting(struct BattleEffectParams *args) {
  BattleEffect_RunGatheringLights(args, GATHERING_LIGHTS_HAUNTING);
}
#define GATHERING_LIGHTS_SLOTS 128 ///< particles reserved for each target

/// For each target in turn, 20 frames apart, lights burst out of the user,
/// one every 2 frames; from 30 frames into the turn, one more each frame is
/// pulled towards the middle of the target's body, and goes out when it is
/// within 32 pixels across and in depth. Each light takes one of three
/// images. Each target is struck 70 frames after its turn starts, and the
/// battle shows the damage at frame 80. The effect layer ripples throughout.
///
/// Called by BattleEffect_RunDemonNight, BattleEffect_RunRumble and
/// BattleEffect_RunHaunting. Loads its colours over the first 64 of the
/// background palette, uses PARTICLES 0 to 1023, writes work->bg2xLines each
/// frame and adds BattleEffect_ArmBg2AffineHBlankDma to feed it to BG2X;
/// removes both its tasks before it returns.
///
/// mode: the colours (enum GatheringLightsMode) and the row of
///       gGatheringLights_Counts: lights per target and frames, 16 and 88,
///       8 and 72, or 12 and 80, the effect running 20 frames more for each
///       target.
void BattleEffect_RunGatheringLights(struct BattleEffectParams *args,
                                     s32 mode) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  struct BattleCamera *camera;
  s32 paletteId;
  s32 i;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  // Setup. The sheet is the first field of the work block, so the pictures
  // decode to `work` itself: three 20x32 lights.
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_MemberBurstImage),
                      work);
  if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == GATHERING_LIGHTS_RUMBLE)
    paletteId = (s32)&ResourceId_MarsDjinnSheet;
  else
    paletteId = (s32)&ResourceId_MercuryDjinnSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);

  // The lights start at the top of the user's body and fly off in any
  // direction at up to 2 pixels a frame.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  EFFECT_FOR_EACH_TARGET(k, work->args) {
    struct Actor *src = GetBattleObjectSlotFar(work->args->user)->actor;
    s32 height = BattleUnit_GetEffectHeightFar(work->args->user);

    for (i = 0; i != GATHERING_LIGHTS_SLOTS; i++) {
      struct Particle *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

      SET_POSITION(mote, src->x.fixed, height, src->z.fixed);
      SET_VELOCITY(mote, ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10);
      mote->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  // Trails keep 3/4 a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ENEMY_CRY);

  for (frame = 0;
       frame != gGatheringLights_Counts[mode][GATHERING_LIGHTS_FRAMES] +
                    work->args->targetCount * 20;
       frame++) {
    s32 *row;

    // Frame loop. The damage shows on frame 80 whatever the number of
    // targets, with the mode's hit sound.
    if (frame == 80) {
      if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);

    // Ripple the canvas sideways, line by line: 4 texels either way about a
    // 4-texel offset (16.16 >> 10 is a quarter in BG2X's 20.8), in a wave 64
    // lines long that moves up a line each frame.
    row = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *row++ = (0x100000 - Trig_Sin((frame + i) << 10) * 16) >> 10;

    EFFECT_FOR_EACH_TARGET(k, work->args) {
      struct Actor *target =
          GetBattleObjectSlotFar(work->args->targets[k])->actor;
      s32 half = BattleUnit_GetEffectHeightFar(work->args->targets[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == GATHERING_LIGHTS_DEMON_NIGHT)
          Sound_PlayFar(SE_HEAVY_IMPACT);
        else
          Sound_PlayFar(SE_IMPACT);
      }
      if (frame == k * 20 + 70)
        BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != gGatheringLights_Counts[mode][GATHERING_LIGHTS_COUNT];
             i++) {
          struct Particle *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            Projection_Point(mote, &screenPosition);
            screenPosition.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * (20 * 32),
                    screenPosition.x - 10, screenPosition.y - 16, 20, 32);
            Particle_AdvanceWithGravity3D(mote, 62, 0);
            // The pull: 1/512 of the distance added to the speed each frame.
            if (frame > k * 20 + i + 30) {
              s32 dx = (target->x.fixed - mote->x) >> 9;
              s32 dy = (target->y.fixed + half - mote->y) >> 9;
              s32 dz = (target->z.fixed - mote->z) >> 9;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                mote->variant = -1;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  // Finish: the transfer goes before the blitters and the DMA task
  // before the layer closes.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef GATHERING_LIGHTS_SLOTS
