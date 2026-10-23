/// @file
/// Thrash, Quick Strike and Vorpal Slash.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// One 120x120 picture of a flashing strike; two fill work->imageStorage and
/// two more Ram_MapGridCellBuffer.
#define FLASHING_STRIKE_PICTURE_BYTES (120 * 120)

/// The flashing strike's modes (BattleAnim_RunFlashingStrike): which two
/// RevealMask resources it shows.
enum FlashingStrikeMode {
  FLASHING_STRIKE_UNUSED,       ///< RevealMaskE and F; nothing passes it
  FLASHING_STRIKE_QUICK_STRIKE, ///< RevealMaskC and D, with a hop
  FLASHING_STRIKE_VORPAL_SLASH  ///< RevealMaskA and B
};

/// The user leaps high at the first target, then a burst of 32 sparks rises
/// beneath it.
///
/// Abilities: Thrash (BATTLE_ANIM_THRASH).
///
/// The leap takes the user three quarters of the way in 24 steps, starting up
/// at 12.2 pixels a frame (16.16); after 29 frames the spark group plays with
/// effect->variant set to SPARK_GROUPS_THRASH, overriding the ability's own.
void BattleAnim_RunThrash(struct BattleAnimArgs *effect) {
  BattleMotion_ApproachTargetFar(effect->user, effect->targets[0], 24, 0xc3333);
  WaitFrames(29);
  effect->variant = SPARK_GROUPS_THRASH;
  BattleAnim_RunSparkGroups(effect, SPARK_GROUPS_RISING);
}

/// Unused: no row of actflags.tsv names its kind, and the handler table is
/// reached with no other kind (BattleAnim_RunTableEffect). The user would
/// leap at the first target, then a burst of 340 sparks would rise beneath it
/// and the canvas flash. It is Severe Blow's animation in The Lost Age, whose
/// row for Severe Blow names this kind; here Severe Blow plays a casting
/// impact.
///
/// Abilities: none (BATTLE_ANIM_UNUSED_SEVERE_BLOW_LEAP).
///
/// A lower leap than Thrash's (7.2 pixels a frame up), and the spark group
/// after 12 frames with effect->variant set to SPARK_GROUPS_SEVERE_BLOW,
/// overriding the ability's own.
void BattleAnim_RunUnusedSevereBlowLeap(struct BattleAnimArgs *effect) {
  BattleMotion_ApproachTargetFar(effect->user, effect->targets[0], 24, 0x73333);
  WaitFrames(12);
  effect->variant = SPARK_GROUPS_SEVERE_BLOW;
  BattleAnim_RunSparkGroups(effect, SPARK_GROUPS_RISING);
}

/// The user leaps at the first target, four pictures flash across the animation
/// layer, then it fills with one colour as the target is struck.
///
/// Abilities: Quick Strike (BATTLE_ANIM_QUICK_STRIKE).
void BattleAnim_RunQuickStrike(struct BattleAnimArgs *effect) {
  BattleAnim_RunFlashingStrike(effect, FLASHING_STRIKE_QUICK_STRIKE);
}

/// The user steps up to the first target, four pictures flash across the
/// animation layer, then it fills with one colour as the target is struck.
///
/// Abilities: Vorpal Slash (BATTLE_ANIM_VORPAL_SLASH).
void BattleAnim_RunVorpalSlash(struct BattleAnimArgs *effect) {
  BattleAnim_RunFlashingStrike(effect, FLASHING_STRIKE_VORPAL_SLASH);
}

/// Plays a flashing strike, the runner of BattleAnim_RunQuickStrike and
/// BattleAnim_RunVorpalSlash; nothing passes FLASHING_STRIKE_UNUSED.
///
/// The user steps up to the first target over 16 frames. Then four 120x120
/// pictures show for four frames each (mirrored when effect->side is 1), the
/// canvas fills with colour 0x3f on frames 16 to 19, the battle shows the
/// damage on frame 18, and on frame 20 the target reels and the screen
/// shakes. 21 frames.
///
/// Opens and closes the canvas layer; the pictures take work->imageStorage
/// and the start of Ram_MapGridCellBuffer.
///
/// mode: picks the pictures (enum FlashingStrikeMode); Quick Strike also
///       makes the step a hop.
void BattleAnim_RunFlashingStrike(struct BattleAnimArgs *effect, s32 mode) {
  void **slots;
  struct BattleAnimWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;

  slots = gBattleAnimWork;
  ANIM_GET_WORK_AND_CANVAS(slots, work, canvas);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  REG_BLDCNT = 0;

  // Each resource holds two pictures: the first fills work->imageStorage, up
  // to the particles, and the second the start of Ram_MapGridCellBuffer.
  if (mode == FLASHING_STRIKE_UNUSED) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF,
                               Ram_MapGridCellBuffer, 1, 1);
  } else if (mode == FLASHING_STRIKE_QUICK_STRIKE) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD,
                               Ram_MapGridCellBuffer, 1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB,
                               Ram_MapGridCellBuffer, 1, 1);
  }
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // The step: three quarters of the way in 16 steps, a hop of 8 pixels a
  // frame up for Quick Strike.
  if (mode == FLASHING_STRIKE_QUICK_STRIKE)
    BattleMotion_ApproachTargetFar(effect->user, effect->targets[0], 16,
                                   0x80000);
  else
    BattleMotion_ApproachTargetFar(effect->user, effect->targets[0], 16, 0);
  WaitFrames(16);

  if (work->effect->side == 1)
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_COPY);
  else
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_COPY);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  Sound_PlaySongFar(SE_212);
  for (frame = 0; frame != 21; frame++) {
    if (frame <= 3)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, work->imageStorage + FLASHING_STRIKE_PICTURE_BYTES, 0, 0,
              120, 120);
    else if (frame <= 11)
      draw[0](canvas, Ram_MapGridCellBuffer, 0, 0, 120, 120);
    else if (frame <= 15)
      draw[0](canvas, Ram_MapGridCellBuffer + FLASHING_STRIKE_PICTURE_BYTES, 0,
              0, 120, 120);
    if (frame >= 16 && frame <= 19)
      Iwram_FillWords(canvas, CANVAS_BYTES, 0x3f3f3f3f);
    if (frame == 18)
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 20) {
      work->shakeFrames = 8;
      BattleMotion_ApplyKnockbackFar(work->effect->targets[0], KNOCKBACK_HURL);
    }
    Camera_ApplyShake(16, 16);
    ANIM_END_FRAME(work);
  }
  // Releases the blitter before stopping the transfer, the other way round
  // from ANIM_FINISH_CANVAS_ONE_BLITTER.
  FreeHeapSlot(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
}

#undef FLASHING_STRIKE_PICTURE_BYTES
