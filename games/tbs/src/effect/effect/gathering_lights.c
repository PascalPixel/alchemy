// Part of effect.c, the battle effect object, compiled only through it.
// Demon Night, Rumble and Haunting.

// Lights that scatter from the user and gather into each target.

// Abilities: Demon Night (kind 50).
//
// Battle effect 50: sixteen pink lights burst out of the user and are pulled
// into each target, with the heavy impact sound.
DEFINE_EFFECT_KIND(BattleFx_RunDemonNight, BattleFx_RunGatheringLights, 0)

// Abilities: Rumble (kind 308).
//
// Battle effect 308: eight lights in Mars Djinn colours burst out of the user
// and are pulled into each target.
DEFINE_EFFECT_KIND(BattleFx_RunRumble, BattleFx_RunGatheringLights, 1)

// Abilities: Haunting (kind 351).
//
// Battle effect 351: twelve lights in Mercury Djinn colours burst out of the
// user and are pulled into each target.
DEFINE_EFFECT_KIND(BattleFx_RunHaunting, BattleFx_RunGatheringLights, 2)

#define GATHERING_LIGHTS_SLOTS 128 // particles reserved for each target

// For each target in turn, 20 frames apart, lights burst out of the user;
// 30 frames after each one appears it is pulled towards the middle of the
// target's body and goes out when it is within 32 pixels across and in
// depth. Each light takes one of three images. Each target is struck 70
// frames after its turn starts, and the battle shows the damage at frame 80.
// The effect layer ripples throughout.
// mode : the colours (0 pink, 1 Mars Djinn, 2 Mercury Djinn) and the row of
//        gMemberBurst_Counts: lights per target and frames.
void BattleFx_RunGatheringLights(struct BattleEffectArgument *effect,
                                 s32 mode) {
  struct EffectPosition screenPosition;
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
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_MemberBurstImage), work);
  if (mode == 0)
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    paletteId = (s32)&ResourceId_MarsDjinnSheet;
  else
    paletteId = (s32)&ResourceId_MercuryDjinnSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);

  // The lights start at the top of the user's body and fly off in any
  // direction at up to 2 pixels a frame.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  EFFECT_FOR_EACH_TARGET(k, work->effect) {
    struct MotionObject *src =
        GetBattleObjectSlotFar(work->effect->actor)->object;
    s32 height = Battle_GetObjectTableValueFar(work->effect->actor);

    for (i = 0; i != GATHERING_LIGHTS_SLOTS; i++) {
      struct EffectStep *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

      SET_POSITION(mote, src->x, height, src->z);
      SET_VELOCITY(mote, ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10,
                   ((Random16() & 255) - 128) << 10);
      mote->variant = 0;
    }
  }

  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  AudioCommand_PlayFar(146);

  for (frame = 0; frame != gMemberBurst_Counts[mode][GATHERING_LIGHTS_FRAMES] +
                               work->effect->count * 20;
       frame++) {
    s32 *row;

    if (frame == 80) {
      if (mode == 0)
        BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
      else
        BattleEventRuntime_BeginPhaseFar(133);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);

    // Ripple the canvas sideways, line by line.
    row = work->bg2X;
    for (i = 0; i != 160; i++)
      *row++ = (0x100000 - Trig_Sin((frame + i) << 10) * 16) >> 10;

    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[k])->object;
      s32 half = Battle_GetObjectTableValueFar(work->effect->actors[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == 0)
          AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
        else
          AudioCommand_PlayFar(133);
      }
      if (frame == k * 20 + 70)
        ObjectGroup_UpdateMembers(work->effect->actors[k], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != gMemberBurst_Counts[mode][GATHERING_LIGHTS_COUNT];
             i++) {
          struct EffectStep *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ApplyBaseAndYOffset(mote, &screenPosition);
            screenPosition.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * 640, screenPosition.x - 10,
                    screenPosition.y - 16, 20, 32);
            EffectStep_AdvanceWithGravity3D(mote, 62, 0);
            if (frame > k * 20 + i + 30) {
              s32 dx = (target->x - mote->x) >> 9;
              s32 dy = (target->y + half - mote->y) >> 9;
              s32 dz = (target->z - mote->z) >> 9;

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

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  BattleFx_EndCanvasLayer();
}
