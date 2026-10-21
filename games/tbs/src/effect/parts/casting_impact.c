/// @file
/// The casting impact scene and its image helpers.
// Part of effect.c.

#include "../effect_internal.h"

// The casting impacts: their phases and particle use are described in the
// folder README, under "Effect families".

/// Moves each RGB555 channel of background colours 0 to 63 one level towards
/// the resource palette. Colour 0 always goes towards black, whatever the
/// resource holds.
///
/// Called once a frame, so a palette arrives in at most 31 frames: by the
/// casting impacts, Ragnarok, the Titan Blade and Ground (hail.c). Reads
/// and rewrites background palette RAM.
void BattleFx_StepPaletteToResource(s32 resourceId) {
  u16 target[64];
  u16 *color = (u16 *)BG_PLTT;
  s32 i;

  Iwram_CopyWords(target, Resource_GetTableEntry(resourceId), 128);
  target[0] = 0;
  for (i = 0; i != 64; i++, color++) {
    s32 red = *color & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 blue = (*color >> 10) & 0x1f;
    s32 targetRed = target[i] & 0x1f;
    s32 targetGreen = (target[i] >> 5) & 0x1f;
    s32 targetBlue = (target[i] >> 10) & 0x1f;

    if (red < targetRed)
      red++;
    else if (red > targetRed)
      red--;
    if (green < targetGreen)
      green++;
    else if (green > targetGreen)
      green--;
    if (blue < targetBlue)
      blue++;
    else if (blue > targetBlue)
      blue--;
    target[i] = RGB(red, green, blue);
  }
  Iwram_CopyWords((u16 *)BG_PLTT, target, 128);
}

/// Loads the impact orb's picture (Resource_LoadAndDecompress).
static inline void CastingImpact_LoadResource(const void *id, void *dest,
                                              s32 skip, s32 copy) {
  /* FAKEMATCH(wrapper): the orb's load through this helper, so the packing
   * loads Ram_MapCellBuffer again; as a direct call in
   * BattleFx_RunCastingImpact, 75 of 3070 instructions differ in tbs-en. */
  Resource_LoadAndDecompress((s32)id, dest, skip, copy);
}

/// Packs a casting impact's picture rows (Graphics_PackTileRows).
static inline void CastingImpact_PackRows(void *src, void *dest, s32 width,
                                          s32 height) {
  /* FAKEMATCH(wrapper): the orb's packing through this helper, which loads
   * the buffer's address last; as a direct call in BattleFx_RunCastingImpact,
   * 72 of 3070 instructions differ in tbs-en. */
  Graphics_PackTileRows(src, dest, width, height);
}

/// Draws one casting impact image with the given blitter.
static inline void CastingImpact_DrawImage(void *canvas, const void *pixels,
                                           s32 x, s32 y, s32 width, s32 height,
                                           DrawRectangle *draw) {
  /* FAKEMATCH(wrapper): four draws through this helper, which keeps the
   * frame at 184 bytes and loads the image's address late; as direct calls in
   * BattleFx_RunCastingImpact, 468 of 3070 instructions differ in tbs-en. */
  (*draw)(canvas, pixels, x, y, width, height);
}

/// Draws a casting-impact image whose height is twice its width.
static inline void CastingImpact_DrawTallImage(void *canvas, const void *pixels,
                                               s32 x, s32 y, s32 width,
                                               DrawRectangle *draw) {
  /* FAKEMATCH(wrapper): the flame's draw through this helper, a call of its
   * own; as a direct call with 64 in BattleFx_RunCastingImpact, merged into
   * another draw's call, 61 of 3070 instructions differ in tbs-en. */
  (*draw)(canvas, pixels, x, y, width, width * 2);
}

/// Draws only the unscrolled height of a casting-impact image.
static inline void CastingImpact_DrawScrolledImage(void *canvas,
                                                   const void *pixels, s32 x,
                                                   s32 y, s32 width, s32 height,
                                                   s32 scroll,
                                                   DrawRectangle *draw) {
  /* FAKEMATCH(wrapper): the column's lower tile through this helper, which
   * subtracts the scroll inside it; as a direct call in
   * BattleFx_RunCastingImpact, 29 of 3070 instructions differ in tbs-en. */
  (*draw)(canvas, pixels, x, y, width, height - scroll);
}

/// Draws a casting-impact image cropped to 91 pixels high.
static inline void CastingImpact_DrawCroppedImage(void *canvas,
                                                  const void *pixels, s32 x,
                                                  s32 y, s32 width,
                                                  DrawRectangle *draw) {
  /* FAKEMATCH(wrapper): two draws through this helper, which loads the
   * image's address last; as direct blitter calls with 91 in
   * BattleFx_RunCastingImpact, 8 of 3070 instructions differ in tbs-en. */
  (*draw)(canvas, pixels, x, y, width, 91);
}

/// Clears the prelude image, stops its callbacks and releases its blitters
/// before the next effect takes over.
///
/// Called by BattleFx_RunCastingImpact for the impacts that hand off after
/// the gathering (15, 17, 24, 26 and Severe Blow); Meltdown's end does the
/// same inline. Clears the canvas and its VRAM (character block 1) and
/// work->effect->prelude, removes Palette_StepFadeTransfer and the canvas
/// transfer, and frees both blitters.
static inline void CastingImpact_EndPrelude(struct BattleEffectWork *work,
                                            void *canvas) {
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  work->effect->prelude = 0;
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
}

/// The casting impacts, by effect kind less 100: each is named for the first
/// ability whose gBattle_ActionFlags entry selects it; the four no ability
/// selects keep their numbers.
///
/// The trailing comments give each impact's own animation, drawn over the
/// target after the orb and sparks. Most animations use the 64 particles at
/// Ram_MapCellBuffer + 0x4000, which start at the target; their pictures are in
/// Ram_MapCellBuffer. "Smoke" is puffs of smoke rising from the target; "hands
/// off" impacts stop after gathering and hand the canvas to another effect.
enum CastingImpact {
  CASTING_IMPACT_IMPACT_BANE = 0,      ///< smoke
  CASTING_IMPACT_IMPACT_SMOG,          ///< smoke
  CASTING_IMPACT_IMPACT_MIST,          ///< smoke
  CASTING_IMPACT_IMPACT_FEVER,         ///< smoke
  CASTING_IMPACT_IMPACT_FLINT,         ///< nothing
  CASTING_IMPACT_IMPACT_SCORCH,        ///< flash bursts rise from the target
  CASTING_IMPACT_IMPACT_SQUALL,        ///< two bolts strike among glints
  CASTING_IMPACT_IMPACT_SAP,           ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_TERRA_STRIKE,  ///< a rock spire grows and sinks
  CASTING_IMPACT_IMPACT_POISON_CLOUD,  ///< smoke
  CASTING_IMPACT_IMPACT_DEADLY_POISON, ///< smoke
  CASTING_IMPACT_IMPACT_MORTAL_DANGER, ///< also Death Size; a large picture
                                       ///< rises and breaks
  CASTING_IMPACT_IMPACT_BAD_OMEN,      ///< skulls rise from the target
  CASTING_IMPACT_IMPACT_LIFE_NOURISH,  ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_AQUA_SOCK,     ///< a blue flame column falls onto the
                                       ///< target
  CASTING_IMPACT_IMPACT_15,            ///< hands off
  CASTING_IMPACT_IMPACT_FROST_BITE,    ///< ice chips converge on the target
  CASTING_IMPACT_IMPACT_17,            ///< hands off
  CASTING_IMPACT_IMPACT_LIFE_LEECH,    ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_PSY_LEECH,     ///< motes fly back to the caster
  CASTING_IMPACT_IMPACT_BROIL,         ///< ember streaks spread sideways
  CASTING_IMPACT_IMPACT_MELTDOWN,      ///< nothing; hands to the stat reduction
  CASTING_IMPACT_IMPACT_HEAT_MIRAGE,   ///< also Heat Flash; smoke
  CASTING_IMPACT_IMPACT_BARRAGE,       ///< flash bursts rise from the target
  CASTING_IMPACT_IMPACT_24,            ///< hands off
  CASTING_IMPACT_IMPACT_ACID_BATH,     ///< smoke
  CASTING_IMPACT_IMPACT_26,            ///< hands off
  CASTING_IMPACT_IMPACT_STUN_VOLTAGE,  ///< two bolts strike among glints
  CASTING_IMPACT_IMPACT_BLINDING_SMOG, ///< smoke
  CASTING_IMPACT_IMPACT_MURK,          ///< smoke
  CASTING_IMPACT_IMPACT_CYCLONE_SLASH, ///< a tornado stands over the target
  CASTING_IMPACT_IMPACT_PSYPHON_SEAL,  ///< a rune opens over the target
  CASTING_IMPACT_IMPACT_ASURA,         ///< a beast scrolls across and breaks
  CASTING_IMPACT_IMPACT_FORCIBLE_ARM,  ///< a flame falls diagonally
  CASTING_IMPACT_IMPACT_TRUNCHEON_FIST, ///< a flame falls diagonally
  CASTING_IMPACT_IMPACT_SEVERE_BLOW,    ///< hands off
};

/// The impact orb, 20x48 pictures packed at half width.
#define CASTING_IMPACT_ORB_CELLS 0x5100
/// The Djinni's 20x40 picture, in Ram_MapCellBuffer after the impact's own.
#define CASTING_IMPACT_DJINNI_CELLS 0x3c56
#define CASTING_IMPACT_GATHER_FRAMES 32 ///< frames the sparks gather
#define CASTING_IMPACT_SPARKS 64        ///< sparks in work->particles
/// The impact pictures' particles, in Ram_MapCellBuffer after the Djinni.
#define CASTING_IMPACT_IMPACT_PARTICLES                                        \
  ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define CASTING_IMPACT_IMPACT_PARTICLE_COUNT 64

/// gCastingImpact_ImageX has seven pictures for each side.
#define CASTING_IMPACT_IMAGE_X(side, image)                                    \
  gCastingImpact_ImageX[(side)][(image)]

/// Gathers sparks around the caster, launches the Djinni and draws the
/// impact on the first target.
///
/// Abilities: Bane (CASTING_IMPACT_IMPACT_BANE); Smog
/// (CASTING_IMPACT_IMPACT_SMOG); Mist (CASTING_IMPACT_IMPACT_MIST); Fever
/// (CASTING_IMPACT_IMPACT_FEVER); Flint (CASTING_IMPACT_IMPACT_FLINT); Scorch
/// (CASTING_IMPACT_IMPACT_SCORCH); Squall (CASTING_IMPACT_IMPACT_SQUALL); Sap
/// (CASTING_IMPACT_IMPACT_SAP); Terra Strike
/// (CASTING_IMPACT_IMPACT_TERRA_STRIKE); Poison Cloud
/// (CASTING_IMPACT_IMPACT_POISON_CLOUD); Deadly Poison
/// (CASTING_IMPACT_IMPACT_DEADLY_POISON); Mortal Danger, Death Size
/// (CASTING_IMPACT_IMPACT_MORTAL_DANGER); Bad Omen
/// (CASTING_IMPACT_IMPACT_BAD_OMEN); Life Nourish
/// (CASTING_IMPACT_IMPACT_LIFE_NOURISH); Aqua Sock
/// (CASTING_IMPACT_IMPACT_AQUA_SOCK); Frost Bite
/// (CASTING_IMPACT_IMPACT_FROST_BITE); Life Leech
/// (CASTING_IMPACT_IMPACT_LIFE_LEECH); Psy Leech
/// (CASTING_IMPACT_IMPACT_PSY_LEECH); Broil (CASTING_IMPACT_IMPACT_BROIL);
/// Meltdown (CASTING_IMPACT_IMPACT_MELTDOWN); Heat Mirage, Heat Flash
/// (CASTING_IMPACT_IMPACT_HEAT_MIRAGE); Barrage
/// (CASTING_IMPACT_IMPACT_BARRAGE); Acid Bath
/// (CASTING_IMPACT_IMPACT_ACID_BATH); Stun Voltage
/// (CASTING_IMPACT_IMPACT_STUN_VOLTAGE); Blinding Smog
/// (CASTING_IMPACT_IMPACT_BLINDING_SMOG); Murk (CASTING_IMPACT_IMPACT_MURK);
/// Cyclone Slash (CASTING_IMPACT_IMPACT_CYCLONE_SLASH); Psyphon Seal
/// (CASTING_IMPACT_IMPACT_PSYPHON_SEAL); Asura (CASTING_IMPACT_IMPACT_ASURA);
/// Forcible Arm (CASTING_IMPACT_IMPACT_FORCIBLE_ARM); Truncheon Fist
/// (CASTING_IMPACT_IMPACT_TRUNCHEON_FIST); Severe Blow
/// (CASTING_IMPACT_IMPACT_SEVERE_BLOW); none (CASTING_IMPACT_IMPACT_15); none
/// (CASTING_IMPACT_IMPACT_17); none (CASTING_IMPACT_IMPACT_24); none
/// (CASTING_IMPACT_IMPACT_26).
///
/// Called only by BattleFx_DispatchByIdRange (positions.c), for effect kinds
/// 100 to 135. The phases, the hand-offs and the particle ranges are in the
/// folder README under "Effect families". Opens the canvas layer (tiled for
/// Terra Strike, Mortal Danger and Asura), writes the background palette,
/// BG2's scale and reference and REG_BLDALPHA, holds the caster still while
/// the sparks gather, and adds BattleFx_ArmBg2AffineHBlankDma for the waving
/// impacts; it releases all of them before it returns, or hands them on.
///
/// mode: the impact, an enum CastingImpact (the effect kind less 100). The
///       tests on mode 100 never hold, as the dispatcher passes 0 to 35.
void BattleFx_RunCastingImpact(struct BattleEffectArgument *effect, s32 mode) {
  void **slots;
  void **slot;
  s32 alive;
  s32 cellOffset;
  struct EffectPosition casterPos;
  struct EffectPosition targetPos;
  struct FixedPointPosition djinnPos;
  s32 phase;
  s32 i;
  s32 *scanline;
  s32 n;
  s32 djinnX;
  DrawRectangle *slotDraw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sprites;
  s32 scrollPos;
  s32 scrollSpeed;
  s32 duration;
  struct EffectPosition *targetScreen;
  struct EffectPosition *casterScreen;
  DrawRectangle *drawPair;
  s32 savedVelocityX;
  s32 savedVelocityY;
  s32 savedVelocityZ;
  s32 savedAcceleration;
  s32 savedVerticalStrength;
  struct MotionObject *targetActor;
  struct FixedPointPosition *motion;
  s32 kindFromTwo;
  s32 kindFromFour;
  struct FixedPointPosition *position;
  struct MotionObject *casterActor;
  struct EffectStep *particle;
  s32 pairX;
  s32 stripX;
  struct FixedPointPosition velocity;
  struct EffectPosition sparkScreen;
  BlitterPair blitters;
  struct EffectPosition projected;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  slot = slots;
  work = (struct BattleEffectWork *)*slot++;
  canvas = *slot;
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  sprites = (u8 *)EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
      mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
      mode == CASTING_IMPACT_IMPACT_ASURA) {
    BattleFx_BeginTiledCanvas(0);
  } else {
    BattleFx_BeginCanvasLayer(0);
  }
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // Setup: the pictures.
  //   work->sheet: six 48x72 frames over the caster
  //   work->sheet + CASTING_IMPACT_ORB_CELLS: the impact orb
  //   Ram_MapCellBuffer: the impact's own pictures
  //   Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS: the element's Djinni
  // The orb is decoded into the map buffer first and packed from there,
  // before the impact's own pictures replace it.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work, 1, 0);
  CastingImpact_LoadResource(&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1,
                             0);
  /* FAKEMATCH(temporary): the orb's dimensions named, which orders the
   * packing's argument setup; as literal 40 and 288, 2 of 3070 instructions
   * differ in tbs-en. */
  {
    s32 width = 40;
    s32 height = 288;
    CastingImpact_PackRows(Ram_MapCellBuffer,
                           work->sheet + CASTING_IMPACT_ORB_CELLS, width,
                           height);
  }
  if (mode == CASTING_IMPACT_IMPACT_SCORCH ||
      mode == CASTING_IMPACT_IMPACT_BARRAGE) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               Ram_MapCellBuffer, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
    Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, Ram_MapCellBuffer,
                               1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_SQUALL ||
             mode == CASTING_IMPACT_IMPACT_STUN_VOLTAGE) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                               Ram_MapCellBuffer + 0xc56, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, Ram_MapCellBuffer, 1,
                               1);
  } else if (mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
    Resource_LoadAndDecompress((s32)&ResourceId_RockSpireSheet,
                               Ram_MapCellBuffer, 1, 1);
  } else if (mode == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
    Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                               Ram_MapCellBuffer, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer,
                               1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_FROST_BITE) {
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, Ram_MapCellBuffer,
                               1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_BROIL) {
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                               Ram_MapCellBuffer, 1, 0);
  } else if (mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
             mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetA, Ram_MapCellBuffer,
                               1, 0);
  } else if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
             mode != CASTING_IMPACT_IMPACT_ASURA) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, Ram_MapCellBuffer,
                               1, 0);
  }
  // The Djinni of the impact's element.
  switch (mode) {
  case CASTING_IMPACT_IMPACT_BANE:
  case CASTING_IMPACT_IMPACT_FLINT:
  case CASTING_IMPACT_IMPACT_SAP:
  case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
  case CASTING_IMPACT_IMPACT_POISON_CLOUD:
  case CASTING_IMPACT_IMPACT_DEADLY_POISON:
  case CASTING_IMPACT_IMPACT_MORTAL_DANGER:
  case CASTING_IMPACT_IMPACT_BAD_OMEN:
  case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
  case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS,
                               1, 1);
    break;
  case CASTING_IMPACT_IMPACT_MIST:
  case CASTING_IMPACT_IMPACT_AQUA_SOCK:
  case CASTING_IMPACT_IMPACT_15:
  case CASTING_IMPACT_IMPACT_FROST_BITE:
  case CASTING_IMPACT_IMPACT_17:
  case CASTING_IMPACT_IMPACT_LIFE_LEECH:
  case CASTING_IMPACT_IMPACT_PSY_LEECH:
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS,
                               1, 1);
    break;
  case CASTING_IMPACT_IMPACT_FEVER:
  case CASTING_IMPACT_IMPACT_SCORCH:
  case CASTING_IMPACT_IMPACT_BROIL:
  case CASTING_IMPACT_IMPACT_MELTDOWN:
  case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
  case CASTING_IMPACT_IMPACT_BARRAGE:
  case CASTING_IMPACT_IMPACT_24:
  case CASTING_IMPACT_IMPACT_ACID_BATH:
  case CASTING_IMPACT_IMPACT_TRUNCHEON_FIST:
  case CASTING_IMPACT_IMPACT_SEVERE_BLOW:
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS,
                               1, 1);
    break;
  case CASTING_IMPACT_IMPACT_SMOG:
  case CASTING_IMPACT_IMPACT_SQUALL:
  case CASTING_IMPACT_IMPACT_26:
  case CASTING_IMPACT_IMPACT_STUN_VOLTAGE:
  case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
  case CASTING_IMPACT_IMPACT_MURK:
  case CASTING_IMPACT_IMPACT_CYCLONE_SLASH:
  case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
  case CASTING_IMPACT_IMPACT_ASURA:
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS,
                               1, 1);
    break;
  case 100:
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS,
                               1, 1);
    break;
  }
  // Trails: each transfer keeps 3/4 of every canvas level for Bad Omen,
  // half for the others.
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
    work->transferValue = 75;
  } else {
    work->transferValue = 50;
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  /* FAKEMATCH(temporary): the results named by pointers set inside their
   * calls; as &targetPos, &casterPos and blitters used directly, 69 of 3070
   * instructions differ in tbs-en. */
  EffectPosition_ProjectScaledBattler(work->effect->actors[0],
                                      targetScreen = &targetPos);
  EffectPosition_ProjectScaledBattler(work->effect->actor,
                                      casterScreen = &casterPos);
  BattleFx_FetchRectangleBlitters(work->effect->side, drawPair = blitters);
  // The background palette fade over 24 frames (Palette_StepFadeTransfer,
  // added by BattleFx_BeginCanvasLayer).
  work->fadeFrames = 24;
  work->fadeStep = 0;
  casterActor = GetBattleObjectSlotFar(work->effect->actor)->object;

  // Gathering sparks: x is the radius (32 to 95), and the three velocities
  // are the spark's roll, pitch and yaw.
  {
    for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
      particle = &work->particles[i];
      SET_POSITION(particle, (Random16() & 63) + 32, 0, 0);
      SET_VELOCITY(particle, Random16() & 0xffff, Random16() & 0xffff,
                   Random16() & 0xffff);
    }
  }
  // Stop the acting unit, saving its motion for after the gathering, and
  // place the Djinni 80 pixels overhead.
  ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)casterActor, 0);
  position = &djinnPos;
  SET_POSITION(position, casterActor->x, casterActor->y + 0x500000,
               casterActor->z);
  savedVelocityX = casterActor->velocityX;
  savedVelocityY = casterActor->velocityY;
  savedVelocityZ = casterActor->velocityZ;
  savedAcceleration = casterActor->acceleration;
  savedVerticalStrength = casterActor->verticalMotionStrength;
  SET_VELOCITY(casterActor, 0, 0, 0);
  casterActor->acceleration = 0;
  casterActor->verticalMotionStrength = 0;
  EffectPosition_ProjectScaledBattler(work->effect->actor, casterScreen);
  casterScreen->x = casterScreen->x / 2;
  AudioCommand_PlayFar(SE_212);

  // Phase 1, the gathering (CASTING_IMPACT_GATHER_FRAMES): four more sparks
  // each frame, their radii shrinking by 4 towards the caster; a spark goes
  // out once its radius passes 0. The Djinni impacts (0 to 7) show the
  // Djinni once fewer than 64 sparks are left.
  frame = 0;
  do {
    n = 0;
    for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
      struct EffectStep *step = &work->particles[i];
      if (step->x >= 0) {
        if (frame >= i / 4) {
          {
            s32 size = 5;
            Render_ResetTransformState();
            SceneTransform_ApplyRoll(step->velocityZ);
            SceneTransform_ApplyPitch(step->velocityX);
            SceneTransform_ApplyYaw(step->velocityY);
            EffectPosition_ProjectPoint(step, &sparkScreen);
            sparkScreen.x = sparkScreen.x / 2 + casterScreen->x;
            // Djinni kinds circle the caster's chest; the rest circle lower,
            // Severe Blow lowest.
            if (mode <= 7)
              sparkScreen.y = sparkScreen.y + casterScreen->y - 8;
            else if (mode == CASTING_IMPACT_IMPACT_SEVERE_BLOW)
              sparkScreen.y = sparkScreen.y + casterScreen->y + 44;
            else
              sparkScreen.y = sparkScreen.y + casterScreen->y + 12;
            // DEAD: the depth is clamped to -60..60 and moved to 0..120 but
            // never read; the ROM keeps the clamp.
            CLAMP(sparkScreen.depth, -60, 60);
            sparkScreen.depth += 60;
            EFFECT_DRAW_MOTE(blitters[1], canvas, sprites, sparkScreen.x,
                             sparkScreen.y, size);
          }
          step->x -= 4;
        }
        n++;
      }
    }
    if (mode <= 7) {
      alive = n;
      if (alive <= 63) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        EffectPosition_ProjectPoint(&djinnPos, &sparkScreen);
        sparkScreen.x /= 2;
        djinnX = sparkScreen.x;
        blitters[0](canvas, Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS,
                    djinnX - 10, sparkScreen.y - 4, 20, 40);
      }
    }
    EFFECT_PRESENT_FRAME(work);
    frame++;
  } while (frame != CASTING_IMPACT_GATHER_FRAMES);
  /* FAKEMATCH(dead): position set again to the &djinnPos it holds, so the
   * gathering's projection takes the address from sp, not a spill; as one
   * assignment, 53 of 3070 instructions differ in tbs-en. */
  position = &djinnPos;

  // Phase 2: place BG2 for the impacts that need it, at its unstretched
  // scale with the impact over the target: canvas x 32 (96 on side 1), or
  // 64 for Terra Strike, falls on the target's screen x. Asura's BG2X
  // starts at -128 (8 on side 1) instead and starts scrolling 12 pixels a
  // frame, slowing from frame 7.
  if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
    REG_BG2PA = Q_8_8(1);
    if (work->effect->side == 0) {
      // frame is 32 here, the gathering's length.
      REG_BG2X = (frame - targetScreen->x) << 8;
    } else {
      REG_BG2X = (96 - targetScreen->x) << 8;
    }
  } else {
    if (mode == CASTING_IMPACT_IMPACT_ASURA) {
      // Scroll the image horizontally; scrollPos is in 16.16 pixels.
      REG_BG2PA = Q_8_8(1);
      if (work->effect->side == 0) {
        scrollPos = -0x800000;
        scrollSpeed = 0xc0000;
      } else {
        scrollPos = 0x80000;
        scrollSpeed = -0xc0000;
      }
      REG_BG2X = (scrollPos >> 16) << 8;
    }
  }
  if (mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
    REG_BG2PA = Q_8_8(1);
    REG_BG2X = (64 - targetScreen->x) << 8;
    work->transferMode = CANVAS_COPY_AND_CLEAR;
    work->transferValue = 0;
    Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    Iwram_ClearWords(canvas, CANVAS_BYTES);
    REG_BLDCNT = 0;
  }
  if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
    REG_BG2PA = Q_8_8(1);
    if (work->effect->side == 0) {
      REG_BG2X = (32 - targetScreen->x) << 8;
    } else {
      REG_BG2X = (96 - targetScreen->x) << 8;
    }
  }
  // Impacts 15, 17, 24 and 26 clear the canvas and hand off to another
  // effect without giving the caster its motion back: 15 to projectile
  // volley 9, 24 to Mystic Flame's orbs and 26 to projectile volley 8; 17
  // ends here.
  if (mode == CASTING_IMPACT_IMPACT_15 || mode == CASTING_IMPACT_IMPACT_17 ||
      mode == CASTING_IMPACT_IMPACT_24 || mode == CASTING_IMPACT_IMPACT_26) {
    CastingImpact_EndPrelude(work, canvas);
    ObjectDispatch_SelectAnimationFar(casterActor, 3);
    if (mode == CASTING_IMPACT_IMPACT_15) {
      BattleFx_RunProjectileVolley(effect, 9);
    }
    if (mode == CASTING_IMPACT_IMPACT_24) {
      BattleFx_RunMysticFlame(effect);
    }
    if (mode != CASTING_IMPACT_IMPACT_26) {
      return;
    }
    BattleFx_RunProjectileVolley(effect, 8);
    return;
  }
  ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)casterActor, 16);
  SET_VELOCITY(casterActor, savedVelocityX, savedVelocityY, savedVelocityZ);
  casterActor->acceleration = savedAcceleration;
  casterActor->verticalMotionStrength = savedVerticalStrength;
  // Impact 35 (Severe Blow), with the caster's motion back, clears the
  // canvas and hands off to BattleFx_RunSparkGroups, mode 2, with variant 3.
  if (mode == CASTING_IMPACT_IMPACT_SEVERE_BLOW) {
    CastingImpact_EndPrelude(work, canvas);
    effect->variant = 3;
    BattleFx_RunSparkGroups(effect, 2);
    return;
  }

  // The Djinni's step a frame, to reach 30 pixels above the target in six
  // frames.
  targetActor = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  motion = &velocity;
  SET_POSITION(motion, (targetActor->x - position->x) / 6,
               (targetActor->y - position->y + 0x1e0000) / 6,
               (targetActor->z - position->z) / 6);
  for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
    particle = &work->particles[i];
    particle->variant = 0;
  }
  // 32 sparks scatter from half the target's height, mostly upwards; variant
  // is a spark's life in visits, 32 to 47, two visits a frame (below). Aqua
  // Sock throws its own later, and Psyphon Seal's rise more slowly.
  if (mode != CASTING_IMPACT_IMPACT_AQUA_SOCK) {
    s32 height = BattleUnit_GetEffectHeightFar(work->effect->actors[0]) / 2;
    for (i = 0; i != 32; i++) {
      particle = &work->particles[i];
      SET_POSITION(particle, targetActor->x, height, targetActor->z);
      if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
        SET_VELOCITY(particle, ((Random16() & 255) - 127) << 12,
                     ((Random16() & 255) - 64) << 10,
                     ((Random16() & 255) - 127) << 12);
      } else {
        SET_VELOCITY(particle, ((Random16() & 255) - 127) << 12,
                     ((Random16() & 255) - 64) << 12,
                     ((Random16() & 255) - 127) << 12);
      }
      particle->variant = i / 2 + 32;
    }
  }
  // Mortal Danger and Asura replace the caster's overlay and the impact's
  // pictures.
  if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
    Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA,
                               Ram_MapCellBuffer, 1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 14);
  }
  if (mode == CASTING_IMPACT_IMPACT_ASURA) {
    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeastSheet,
                               work->imageStorage, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetB,
                               Ram_MapCellBuffer, 1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 14);
  }
  // The 64 particles of the impact's own animation start at the target, 20
  // pixels up (on the ground for Bad Omen), flying outwards. The returning
  // motes start theirs on frame 12; Mortal Danger and Asura use none.
  if (mode != CASTING_IMPACT_IMPACT_SAP &&
      mode != CASTING_IMPACT_IMPACT_LIFE_NOURISH &&
      mode != CASTING_IMPACT_IMPACT_LIFE_LEECH &&
      mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
      mode != CASTING_IMPACT_IMPACT_ASURA &&
      mode != CASTING_IMPACT_IMPACT_PSY_LEECH) {
    s32 height = 0;
    if (mode != CASTING_IMPACT_IMPACT_BAD_OMEN)
      height = 0x140000;
    for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
      particle = &CASTING_IMPACT_IMPACT_PARTICLES[i];
      SET_POSITION(particle, targetActor->x, height, targetActor->z);
      if (mode == CASTING_IMPACT_IMPACT_SCORCH ||
          mode == CASTING_IMPACT_IMPACT_BARRAGE) {
        SET_VELOCITY(particle, ((Random16() & 255) - 127) << 11,
                     (Random16() & 255) << 11,
                     ((Random16() & 255) - 127) << 11);
      } else if (mode == CASTING_IMPACT_IMPACT_ACID_BATH) {
        SET_VELOCITY(particle, ((Random16() & 255) - 127) << 11,
                     (Random16() & 127) << 10,
                     ((Random16() & 255) - 127) << 11);
      } else {
        SET_VELOCITY(particle, ((Random16() & 255) - 127) << 10,
                     (Random16() & 127) << 10,
                     ((Random16() & 255) - 127) << 10);
      }
      particle->variant = 0;
    }
  }
  // Mist, Fever, Bad Omen, Heat Mirage, Blinding Smog and Murk wave BG2 by
  // scanline.
  kindFromTwo = mode - 2;
  if ((kindFromTwo >= 0 && kindFromTwo <= 1) ||
      mode == CASTING_IMPACT_IMPACT_BAD_OMEN ||
      mode == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
      mode == CASTING_IMPACT_IMPACT_MURK ||
      mode == CASTING_IMPACT_IMPACT_BLINDING_SMOG) {
    Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma,
                                  EFFECT_CALLBACK_ORDER);
  }

  // The impact lasts 20, 32, 40, 48, 64 or 80 frames, by mode.
  kindFromFour = mode - 4;
  if ((kindFromFour >= 0 && kindFromFour <= 2) ||
      mode == CASTING_IMPACT_IMPACT_BARRAGE ||
      mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
      mode == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
      mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
      mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST || mode == 100) {
    duration = 32;
  } else if ((mode >= 0 && mode <= 3) ||
             mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
             mode == CASTING_IMPACT_IMPACT_POISON_CLOUD ||
             mode == CASTING_IMPACT_IMPACT_DEADLY_POISON ||
             mode == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
             mode == CASTING_IMPACT_IMPACT_ACID_BATH ||
             mode == CASTING_IMPACT_IMPACT_MURK ||
             mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL ||
             mode == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
    duration = 48;
  } else if (mode == CASTING_IMPACT_IMPACT_MELTDOWN) {
    duration = 20;
  } else if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
             mode == CASTING_IMPACT_IMPACT_ASURA ||
             mode == CASTING_IMPACT_IMPACT_BROIL) {
    duration = 40;
  } else if (mode == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
             mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
    duration = 64;
  } else {
    duration = 80;
  }
  // Phase 3, the impact, for duration frames. Each frame: the wave table,
  // the caster's overlay, the palette step, the orb and the scattering
  // sparks, the impact's own animation, the Djinni's flight, then the cues
  // for the target.
  frame = 0;
  while (frame != duration) {
    // BG2 wave table: BG2X for each scanline. It is built for every impact
    // but Mortal Danger and Asura, and shows only for the six waving impacts,
    // for which BattleFx_ArmBg2AffineHBlankDma was added above.
    if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      if (mode != CASTING_IMPACT_IMPACT_ASURA) {
        s32 base;
        scanline = work->bg2xLines;
        i = 0;
        base = 0x40000;
        phase = frame << 12;
        for (; i != 160; i++) {
          // 1 - sin pixels (0 to 2), in BG2X's 20.8 format: a wave 32 lines
          // long that moves up two lines a frame.
          s32 offset = (base - ((Trig_Sin(phase) * 4))) >> 10;
          *scanline++ = offset;
          phase += 0x800;
        }
      }
    }
    // Follow the caster for the first three frames, anchoring 16 pixels
    // lower.
    if (frame <= 2) {
      EffectPosition_ProjectScaledBattler(work->effect->actor, casterScreen);
      casterScreen->x = casterScreen->x / 2;
      casterScreen->y += 16;
    }
    // The caster's overlay: six pictures, two frames each.
    if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
        mode != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
        mode != CASTING_IMPACT_IMPACT_ASURA &&
        mode != CASTING_IMPACT_IMPACT_FORCIBLE_ARM &&
        mode != CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
      if (frame <= 11) {
        if (work->effect->side == 0) {
          blitters[0](canvas, work->sheet + frame / 2 * (48 * 72),
                      casterScreen->x - 32, casterScreen->y - 40, 48, 72);
        } else {
          blitters[0](canvas, work->sheet + frame / 2 * (48 * 72),
                      casterScreen->x, casterScreen->y - 40, 48, 72);
        }
      }
    }
    // Step the background palette towards the impact's palette; impacts not
    // listed keep the faded one.
    switch (mode) {
    case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
      BattleFx_StepPaletteToResource((s32)&ResourceId_FlameSheetA);
      break;
    case CASTING_IMPACT_IMPACT_AQUA_SOCK:
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
      break;
    case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
      BattleFx_StepPaletteToResource((s32)&ResourceId_RuneSheet);
      break;
    case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_RockSpireSheet);
      break;
    case CASTING_IMPACT_IMPACT_BANE:
    case CASTING_IMPACT_IMPACT_DEADLY_POISON:
      BattleFx_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
      break;
    case CASTING_IMPACT_IMPACT_BAD_OMEN:
    case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
    case CASTING_IMPACT_IMPACT_ACID_BATH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_PinkBurstSheet);
      break;
    case CASTING_IMPACT_IMPACT_LIFE_LEECH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_IceBlockSheet);
      break;
    case CASTING_IMPACT_IMPACT_PSY_LEECH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
      break;
    case CASTING_IMPACT_IMPACT_MIST:
    case CASTING_IMPACT_IMPACT_MURK:
      BattleFx_StepPaletteToResource((s32)&ResourceId_TanPalette);
      break;
    case CASTING_IMPACT_IMPACT_SMOG:
    case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
      BattleFx_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      break;
    case CASTING_IMPACT_IMPACT_FEVER:
    case CASTING_IMPACT_IMPACT_BROIL:
    case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_EmberStreakSheet);
      break;
    case CASTING_IMPACT_IMPACT_POISON_CLOUD:
      BattleFx_StepPaletteToResource((s32)&ResourceId_LimePalette);
      break;
    case CASTING_IMPACT_IMPACT_SCORCH:
    case CASTING_IMPACT_IMPACT_BARRAGE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_FlashBurstSheet);
      break;
    }
    // The orb on frames 4 to 15, at the target's x but the caster's height
    // (casterScreen->y), and the 32 scattering sparks from frame 4.
    if (mode != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
        mode != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
        mode != CASTING_IMPACT_IMPACT_ASURA) {
      if (frame >= 4 && frame <= 15) {
        drawPair[1](canvas,
                    work->sheet + (frame - 4) / 2 * (20 * 48) +
                        CASTING_IMPACT_ORB_CELLS,
                    targetScreen->x / 2 - 8, casterScreen->y - 24, 20, 48);
      }
      EFFECT_VIEW_FROM_CAMERA(camera);
      // Each spark is visited twice a frame, so it moves and ages twice; even
      // sparks draw brightest-wins, odd ones additively.
      if (frame > 3) {
        for (i = 0; i != 128; i++) {
          s32 index = i / 2;
          struct EffectStep *step = &work->particles[index];
          s32 size = step->variant;
          if (size > 0) {
            EffectPosition_ProjectPoint(step, &projected);
            size >>= 4;
            size++;
            projected.x /= 2;
            EFFECT_DRAW_MOTE(blitters[index & 1], canvas, sprites, projected.x,
                             projected.y, size);
            EffectStep_AdvanceWithGravity3D(step, 60, -0x1000);
            step->variant--;
          }
        }
      }
    }
    // The impact's own animation.
    if (mode == CASTING_IMPACT_IMPACT_SAP ||
        mode == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
        mode == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
        mode == CASTING_IMPACT_IMPACT_PSY_LEECH) {
      // Motes burst from the target on frame 12 and fly back into the caster,
      // who glows white from frame 50 to 79. Mote i steers towards the
      // caster's middle from frame i + 23, by a 256th of the distance more
      // each frame, and goes out once it is within 16 pixels of the caster
      // across and in depth (a 256th of the distance under 0x1000); only the
      // first 32 are drawn.
      if (frame == 50) {
        ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1,
                                  -1, 0);
      }
      if (frame == 79) {
        ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
      }
      if (frame == 12) {
        particle = CASTING_IMPACT_IMPACT_PARTICLES;
        for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT;
             i++, particle++) {
          SET_POSITION(particle, targetActor->x, 0x140000, targetActor->z);
          SET_VELOCITY(particle, ((Random16() & 255) - 128) << 10,
                       ((Random16() & 255) - 128) << 10,
                       ((Random16() & 255) - 128) << 10);
          particle->variant = 0;
        }
      }
      if (frame > 11) {
        struct EffectStep *step = CASTING_IMPACT_IMPACT_PARTICLES;
        struct MotionObject *caster;
        s32 height;
        caster = GetBattleObjectSlotFar(work->effect->actor)->object;
        height = BattleUnit_GetEffectHeightFar(work->effect->actor) / 2;
        for (i = 0; i != 32; i++, step++) {
          if (step->variant >= 0) {
            s32 size = (i & 1) + 6;
            EffectPosition_ProjectPoint(step, &projected);
            projected.x >>= 1;
            blitters[0](canvas, sprites + gBattleFx_MoteCells[size - 1],
                        projected.x - (u32)size / 2, projected.y - size, size,
                        size * 2);
            EffectStep_AdvanceWithGravity3D(step, 62, 0);
            if (frame > i + 22) {
              s32 dx = (caster->x - step->x) >> 8;
              s32 dy = (caster->y + height - step->y) >> 8;
              s32 dz = (caster->z - step->z) >> 8;
              step->velocityX += dx;
              step->velocityY += dy;
              step->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                step->variant = -1;
            }
          }
        }
      }
    } else if (mode == CASTING_IMPACT_IMPACT_MELTDOWN) {
      // Draw nothing; finish with BattleFx_RunImpair.
    } else if (mode == CASTING_IMPACT_IMPACT_SQUALL ||
               mode == CASTING_IMPACT_IMPACT_STUN_VOLTAGE) {
      // Two 24x104 bolts strike the target from frame 6 to 19, three frames
      // apart in their four-picture cycle; from frame 8 to 23 three glints a
      // frame flash at random points around it, each randomly flipped. Each
      // glint takes a blitter of its own, so the pair is fetched again after.
      if (frame >= 6 && frame <= 19) {
        phase = frame;
        i = 0;
        do {
          blitters[0](canvas,
                      ((phase / 2) & 3) * LIGHTNING_BOLT_CELL_BYTES +
                          (Ram_MapCellBuffer + 0xc56),
                      targetScreen->x / 2 - 8, 0, 24, 104);
          i++;
          phase += 3;
        } while (i != 2);
      }
      if (frame >= 8 && frame <= 23) {
        for (i = 0; i != 3; i++) {
          s32 image = i & 3;
          s32 angle = Random16() & 0xffff;
          s32 imageX;
          s32 imageY;
          s32 drawFlags;
          imageX = Trig_Sin(angle);
          imageX <<= 3;
          imageX >>= 16;
          imageX += targetScreen->x / 2;
          imageX -= gBattleFx_GlintCellWidths[image] / 2;
          imageY = Trig_Cos(angle);
          imageY <<= 5;
          imageY >>= 16;
          imageY -= gBattleFx_GlintCellHeights[image] / 2;
          EFFECT_RELEASE_BLITTERS();
          drawFlags = Random16();
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              gCastingImpact_GlintDrawFlags[drawFlags & 3] |
                                  EFFECT_CLIP_BOUNDS,
                              COMPOSITE_MAX);
          ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE](
              canvas, Ram_MapCellBuffer + gBattleFx_GlintCells[image], imageX,
              imageY + 56, gBattleFx_GlintCellWidths[image],
              gBattleFx_GlintCellHeights[image]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
          BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
        }
      }
    } else if (mode == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
      // Over the first 24 frames a flame column falls onto the target: a
      // 17x104 picture tiled down the screen, scrolling 16 pixels a frame,
      // its head falling 32 pixels a frame until it lands on frame 8, and the
      // 34x65 flame at its foot. The landing shakes the screen for 8 frames,
      // and from frame 2 up to four more sparks a frame rise from the target
      // into free slots.
      s32 fall;
      s32 scroll;
      EFFECT_RELEASE_BLITTERS();
      if (frame >= 0 && frame <= 23) {
        DrawRectangle *draw;
        s32 width;
        s32 tileHeight;
        s32 columnX;
        s32 columnY;
        s32 top;
        s32 originX;

        originX = targetScreen->x / 2;
        fall = frame * 32 - 232;
        scroll = frame * 16 - 48;
        if (fall > 0)
          fall = 0;
        while (scroll > 104)
          scroll -= 104;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_MAX);
        draw = (DrawRectangle *)gWorkSlot;
        draw += HEAP_SLOT_BLITTER_ALTERNATE;
        columnX = originX - 8;
        columnY = fall + scroll;
        top = columnY - 104;
        width = 17;
        tileHeight = 104;
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, columnX, top, width,
                                tileHeight, draw);
        CastingImpact_DrawScrolledImage(canvas, Ram_MapCellBuffer, columnX,
                                        columnY, width, tileHeight, scroll,
                                        draw);
        (*draw)(canvas, Ram_MapCellBuffer + (17 * 104), originX - width,
                fall + 47, width * 2, 65);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        if (frame == 8) {
          work->shakeFrames = frame;
        }
        if (frame > 1) {
          s32 emitted = 0;
          struct EffectStep *step;
          for (i = 0; i != CASTING_IMPACT_SPARKS; i++) {
            if ((step = &work->particles[i])->variant == 0) {
              SET_POSITION(step, targetActor->x, 0x140000, targetActor->z);
              SET_VELOCITY(step, ((Random16() & 255) - 127) << 12,
                           ((Random16() & 255) - 64) << 10,
                           ((Random16() & 255) - 127) << 12);
              step->variant = i / 2 + 32;
              emitted++;
              if (emitted == 4)
                break;
            }
          }
        }
      }
      BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
    } else if (mode == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
      s32 height;
      EFFECT_RELEASE_BLITTERS();
      // From frame 4 to 23 the rune's two halves are drawn brightest-wins,
      // the second mirrored, each with a blitter of its own.
      if (frame >= 4 && frame <= 23) {
        pairX = targetScreen->x / 2;
        height = 48;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_MAX);
        slotDraw = (DrawRectangle *)gWorkSlot;
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, pairX - 24, 48, 24,
                                height, slotDraw + HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            COMPOSITE_MAX);
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, pairX, 48, 24,
                                height, slotDraw + HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
      BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
    } else if (mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
      // From frame 6 a tornado of three stacked 40x32 strips stands over the
      // target, cycling three pictures of two frames each; from frame 16 it
      // fades. After the sheet's six glints (0xc56 bytes) come each
      // picture's top and bottom strips in pairs, then the three middles.
      s32 image;
      if (frame > 15) {
        REG_BLDALPHA = BLEND_ALPHA(32 - frame, 16);
      }
      if (frame > 5) {
        stripX = targetScreen->x / 2 - 20;
        image = frame / 2 % 3;
        cellOffset = image * 2560;
        blitters[0](canvas, Ram_MapCellBuffer + 0xc56 + cellOffset, stripX, 16,
                    40, 32);
        blitters[0](canvas, image * (40 * 32) + (Ram_MapCellBuffer + 0x2a56),
                    stripX, 48, 40, 32);
        blitters[0](canvas, cellOffset + (Ram_MapCellBuffer + 0x1156), stripX,
                    80, 40, 32);
      }
    } else if (mode == CASTING_IMPACT_IMPACT_SCORCH ||
               mode == CASTING_IMPACT_IMPACT_BARRAGE) {
      // Six 32x64 frames per particle, two ticks each.
      struct EffectStep *step = CASTING_IMPACT_IMPACT_PARTICLES;
      for (i = 0; i != 16; i++, step++) {
        // 16 flash bursts rise, two starting each frame from frame 4.
        if (frame >= i / 2 + 4) {
          s32 age = step->variant;
          if (age <= 11) {
            s32 image = age / 2;
            EffectPosition_ProjectPoint(step, &projected);
            projected.x /= 2;
            blitters[0](canvas, Ram_MapCellBuffer + (image << 11),
                        projected.x - 16, projected.y - 32, 32, 64);
            EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
            step->variant++;
          }
        }
      }
    } else if (mode == CASTING_IMPACT_IMPACT_FLINT) {
      // Draw nothing.
    } else if (mode == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
      s32 imageY;
      s32 sway;
      // The 57x98 picture shows until frame 7 and the 99x69 one from frame 4;
      // the canvas flashes on frames 4 and 5, and from frame 6 five pieces
      // follow, two frames each.
      // The sine is taken and dropped; only the cosine sways the pictures,
      // up to 4 pixels.
      Trig_Sin(frame << 9);
      sway = Trig_Cos(frame << 9);
      sway <<= 2;
      sway >>= 16;
      // targetScreen->y is in whole pixels, so the shift leaves 0 for any
      // target on the screen: the pictures stand at a fixed height.
      imageY = (targetScreen->y >> 16) + sway + 16;
      if (frame <= 3) {
        blitters[0](canvas, work->sheet,
                    CASTING_IMPACT_IMAGE_X(work->effect->side, 0),
                    imageY + gCastingImpact_ImageY[0], 57, 98);
      } else {
        if (frame <= 7) {
          blitters[0](canvas, work->sheet,
                      CASTING_IMPACT_IMAGE_X(work->effect->side, 0),
                      imageY + gCastingImpact_ImageY[0], 57, 98);
        }
        blitters[0](canvas, work->sheet + (57 * 98),
                    CASTING_IMPACT_IMAGE_X(work->effect->side, 1),
                    imageY + gCastingImpact_ImageY[1], 99, 69);
        if (frame == 4 || frame == 5) {
          Iwram_FillWords(canvas, CANVAS_BYTES, 0x3f3f3f3f);
        }
        if (frame == 6 || frame == 7) {
          blitters[0](canvas, work->sheet + (57 * 98 + 99 * 69),
                      CASTING_IMPACT_IMAGE_X(work->effect->side, 2),
                      imageY + gCastingImpact_ImageY[2], 128, 91);
        }
        if (frame == 8 || frame == 9) {
          CastingImpact_DrawCroppedImage(
              canvas, Ram_MapCellBuffer,
              CASTING_IMPACT_IMAGE_X(work->effect->side, 3),
              imageY + gCastingImpact_ImageY[3], 128, &blitters[0]);
        }
        if (frame == 10 || frame == 11) {
          blitters[0](canvas, Ram_MapCellBuffer + (128 * 91),
                      CASTING_IMPACT_IMAGE_X(work->effect->side, 4),
                      imageY + gCastingImpact_ImageY[4], 128, 59);
        }
        if (frame == 12 || frame == 13) {
          blitters[0](canvas, Ram_MapCellBuffer + (128 * 91 + 128 * 59),
                      CASTING_IMPACT_IMAGE_X(work->effect->side, 5),
                      imageY + gCastingImpact_ImageY[5], 122, 29);
        }
        if (frame == 14 || frame == 15) {
          blitters[0](canvas,
                      Ram_MapCellBuffer + (128 * 91 + 128 * 59 + 122 * 29),
                      CASTING_IMPACT_IMAGE_X(work->effect->side, 6),
                      imageY + gCastingImpact_ImageY[6], 76, 25);
        }
      }
    } else if (mode == CASTING_IMPACT_IMPACT_ASURA) {
      // The background scrolls, slowing from frame 7 until it stops, while an
      // 80x104 beast appears and breaks: its first picture shows until frame
      // 7 and its second from frame 4, the canvas flashes on frames 4 and 5,
      // and from frame 6 five pieces follow, two frames each. It fades from
      // frame 16.
      scrollPos += scrollSpeed;
      if (frame > 6) {
        scrollSpeed = (scrollSpeed * 48) / 64;
      }
      REG_BG2X = (scrollPos >> 16) << 8;
      {
        s32 fade;
        fade = frame - 16;
        if (fade >= 0 && fade <= 15) {
          REG_BLDALPHA = BLEND_ALPHA(16 - fade, 16);
        }
      }
      if (frame == 4 || frame == 5) {
        Iwram_FillWords(canvas, CANVAS_BYTES, 0x3f3f3f3f);
      }
      if (frame <= 3) {
        if (work->effect->side == 1) {
          blitters[0](canvas, work->imageStorage, 0, 24, 80, 104);
        } else {
          blitters[0](canvas, work->imageStorage, 48, 24, 80, 104);
        }
      } else {
        if (frame <= 7) {
          if (work->effect->side == 1) {
            blitters[0](canvas, work->imageStorage, 0, 24, 80, 104);
          } else {
            blitters[0](canvas, work->imageStorage, 48, 24, 80, 104);
          }
        }
        if (work->effect->side == 1) {
          blitters[0](canvas, work->imageStorage + 0x1e00, 16, 16, 80, 104);
        } else {
          blitters[0](canvas, work->imageStorage + 0x1e00, 32, 16, 80, 104);
        }
        if (frame == 6 || frame == 7) {
          blitters[0](canvas, work->imageStorage + 0x3e80, 0, 16, 128, 91);
        }
        if (frame == 8 || frame == 9) {
          CastingImpact_DrawCroppedImage(canvas, Ram_MapCellBuffer, 0, 16, 128,
                                         &blitters[0]);
        }
        if (frame == 10 || frame == 11) {
          blitters[0](canvas, Ram_MapCellBuffer + (128 * 91), 0, 16, 128, 59);
        }
        if (frame == 12 || frame == 13) {
          blitters[0](canvas, Ram_MapCellBuffer + (128 * 91 + 128 * 59), 0, 16,
                      128, 29);
        }
        if (frame == 14 || frame == 15) {
          blitters[0](canvas,
                      Ram_MapCellBuffer + (128 * 91 + 128 * 59 + 128 * 29), 0,
                      16, 128, 26);
        }
      }
    } else if (mode == CASTING_IMPACT_IMPACT_BROIL) {
      // Twelve ember streaks, one starting each frame from frame 6, each six
      // pictures of two frames. Odd streaks step right of the target and even
      // ones left, 3 pixels further each pair; the first, and each whose
      // (i - 1) % 4 is 2 or 3, draws additively.
      for (i = 0; i != 12; i++) {
        if (frame >= i + 6 && frame < i + 18) {
          s32 image = (frame - i - 6) / 2;
          s32 imageX =
              targetScreen->x / 2 - gBattleFx_PuffArcCellWidths[image] / 2;
          s32 mirrored;
          if (i & 1)
            imageX += (1 + i) / 2 * 3;
          else
            imageX -= (i + 1) / 2 * 3;
          mirrored = 1;
          if (i != 0) {
            mirrored = 0;
            if (((i - 1) & 3) > 1)
              mirrored = 1;
          }
          blitters[mirrored](canvas,
                             Ram_MapCellBuffer + gBattleFx_PuffArcCells[image],
                             imageX, gBattleFx_PuffArcCellBiasY[image] + 48,
                             gBattleFx_PuffArcCellWidths[image],
                             gBattleFx_PuffArcCellHeights[image]);
        }
      }
    } else if (mode == CASTING_IMPACT_IMPACT_FROST_BITE) {
      // 64 ice chips close in on the target in straight lines from 32 to 159
      // pixels out, two more each frame, 6 pixels a frame. The target
      // flinches, with a sound, as each eighth chip and the last arrive.
      if (frame == 0) {
        for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
          particle = &CASTING_IMPACT_IMPACT_PARTICLES[i];
          SET_POSITION(particle, (Random16() & 127) + 32, 0, 0);
          SET_VELOCITY(particle, Random16() & 0xffff, Random16() & 0xffff,
                       Random16() & 0xffff);
        }
        // Chip 63's y, not its radius, is set to 159, which sets it up to 159
        // pixels off its line. XXX: likely meant as its radius, because 159
        // is the largest the others draw (32 + 127) and chip 63 gives the
        // last flinch.
        CASTING_IMPACT_IMPACT_PARTICLES[63].y = 159;
      }
      {
        struct EffectStep *step = CASTING_IMPACT_IMPACT_PARTICLES;
        struct EffectPosition *origin;
        /* FAKEMATCH(dead): origin offset by a zero gcse finds late, keeping
         * targetScreen in a register for the loop; as targetScreen read
         * directly, 8 of 3070 instructions differ in tbs-en. */
        s32 bias = 0;
        for (i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++, step++) {
          if (step->x >= 0 && frame >= i / 2) {
            s32 image = i & 3;
            Render_ResetTransformState();
            SceneTransform_ApplyPitch(step->velocityX);
            SceneTransform_ApplyYaw(step->velocityY);
            EffectPosition_ProjectPoint(step, &projected);
            origin = (struct EffectPosition *)((u8 *)targetScreen + bias);
            projected.x = projected.x / 2 + origin->x / 2;
            projected.y += origin->y + 32;
            blitters[1](canvas,
                        Ram_MapCellBuffer + gCastingImpact_IceChipCells[image],
                        projected.x - 4, projected.y - 4, 8, 8);
            step->x -= 6;
            if (step->x < 0 && ((i & 7) == 0 || i == 63)) {
              AudioCommand_PlayFar(SE_133);
              ObjectGroup_UpdateMembers(work->effect->actors[0],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
            }
          }
        }
      }
    } else if (mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
      s32 height;
      // The rock spire rises 16 pixels a frame from frame 5 to its full 96,
      // and from frame 26 sinks 4 a frame.
      if (frame >= 5 && frame <= 49) {
        if (frame > 25) {
          height = 196 - (frame << 2);
        } else {
          height = (frame << 4) - 64;
        }
        if (height > 96) {
          height = 96;
        }
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, 48, 104 - height, 32,
                                height, &blitters[0]);
      }
    } else if (mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
               mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
      // A 32x64 flame falls diagonally onto the target over six frames.
      s32 offset;
      s32 imageX;
      s32 imageY;
      if (frame <= 5) {
        // targetScreen->x / 2 is written out, so that its last shift can
        // follow the offset.
        if (work->effect->side == 0) {
          imageX = targetScreen->x;
          imageX += (u32)imageX >> 31;
          imageX >>= 1;
          offset = (6 - frame) * 3;
          imageX += offset * 2;
          imageY = targetScreen->y - offset * 4 + 24;
        } else {
          imageX = targetScreen->x;
          imageX += (u32)imageX >> 31;
          imageX >>= 1;
          offset = (6 - frame) * 3;
          imageX -= offset * 2;
          imageY = targetScreen->y - offset * 4 + 24;
        }
        CastingImpact_DrawTallImage(canvas, Ram_MapCellBuffer, imageX - 16,
                                    imageY - 32, 32, &blitters[1]);
      }
    } else if (mode == CASTING_IMPACT_IMPACT_BAD_OMEN) {
      // 16 skulls of three 24x24 pictures rise from the target, each at one of
      // four rates by its number; from frame 48 they fade.
      if (frame > 47) {
        REG_BLDALPHA = BLEND_ALPHA(64 - frame, 16);
      }
      {
        struct EffectStep *step = CASTING_IMPACT_IMPACT_PARTICLES;
        for (i = 0; i != 16; i++, step++) {
          s32 image = i % 3;
          EffectPosition_ProjectPoint(step, &projected);
          projected.x /= 2;
          blitters[i & 1](canvas, Ram_MapCellBuffer + image * (24 * 24),
                          projected.x - 12, projected.y - 12, 24, 24);
          EffectStep_AdvanceWithGravity3D(step, 60, 1 << ((i & 3) + 11));
          step->variant++;
        }
      }
    } else if (mode != 100) {
      // The smoke impacts: six 24x48 pictures a puff, four frames each.
      struct EffectStep *step = CASTING_IMPACT_IMPACT_PARTICLES;
      for (i = 0; i != 16; i++, step++) {
        // One puff starts each frame from frame 4; Acid Bath's rise slower.
        if (frame >= i + 4) {
          s32 age = step->variant;
          if (age <= 23) {
            s32 image = age / 4;
            EffectPosition_ProjectPoint(step, &projected);
            projected.x /= 2;
            blitters[i & 1](canvas, Ram_MapCellBuffer + image * (24 * 48),
                            projected.x - 12, projected.y - 24, 24, 48);
            if (mode == CASTING_IMPACT_IMPACT_ACID_BATH)
              EffectStep_AdvanceWithGravity3D(step, 60, 0x400);
            else
              EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
            step->variant++;
          }
        }
      }
    }
    // Djinni impacts 0 to 7: fly to the target in six frames.
    if (mode <= 7) {
      if (frame <= 5) {
        EffectPosition_ProjectPoint(position, &projected);
        projected.x /= 2;
        djinnX = projected.x;
        drawPair[1](canvas, Ram_MapCellBuffer + CASTING_IMPACT_DJINNI_CELLS,
                    djinnX - 10, projected.y - 4, 20, 40);
        position->x += motion->x;
        position->y += motion->y;
        position->z += motion->z;
      }
    }
    // The cues: the damage shows on frame 3 with no sound of its own (a cue
    // of -1 is never played), the heavy impact sounds on frame 4, frame 6
    // gives some impacts a reaction and a shake, and the target flashes on
    // frames 6 and 14. Of the reactions (gBattleMotion_Variant*), motion 4
    // is the fastest knockback, 3 the highest hop and 1 a short knockback.
    if (frame == 3) {
      EFFECT_SHOW_DAMAGE(-1);
    }
    if (frame == 4) {
      AudioCommand_PlayFar(SE_HEAVY_IMPACT);
    }
    if (frame == 6) {
      if (mode == CASTING_IMPACT_IMPACT_FLINT ||
          mode == CASTING_IMPACT_IMPACT_SCORCH ||
          mode == CASTING_IMPACT_IMPACT_SAP ||
          mode == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
          mode == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
          mode == CASTING_IMPACT_IMPACT_PSY_LEECH ||
          mode == CASTING_IMPACT_IMPACT_BARRAGE ||
          mode == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST || mode == 100) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        work->shakeFrames = 8;
      } else if (mode == CASTING_IMPACT_IMPACT_BROIL ||
                 mode == CASTING_IMPACT_IMPACT_AQUA_SOCK ||
                 mode == CASTING_IMPACT_IMPACT_FORCIBLE_ARM) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
        work->shakeFrames = 2;
      } else if (mode == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
                 mode == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
        work->shakeFrames = 8;
      }
    }
    if (frame == 6) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 4);
    }
    if (frame == 14) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 4);
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
    frame++;
  }
  // Phase 4: Meltdown hands the cleared canvas to the stat reduction
  // (Impair), as CastingImpact_EndPrelude does; the others stop the wave
  // callback and release the canvas.
  if (mode == CASTING_IMPACT_IMPACT_MELTDOWN) {
    Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    Iwram_ClearWords(canvas, CANVAS_BYTES);
    work->effect->prelude = 0;
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleFx_RunImpair(effect);
  } else {
    if (mode == CASTING_IMPACT_IMPACT_MIST ||
        mode == CASTING_IMPACT_IMPACT_FEVER ||
        mode == CASTING_IMPACT_IMPACT_BAD_OMEN ||
        mode == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
        mode == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
        mode == CASTING_IMPACT_IMPACT_MURK) {
      Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
    }
    EFFECT_FINISH_CANVAS();
  }
}

#undef CASTING_IMPACT_ORB_CELLS
#undef CASTING_IMPACT_DJINNI_CELLS
#undef CASTING_IMPACT_GATHER_FRAMES
#undef CASTING_IMPACT_SPARKS
#undef CASTING_IMPACT_IMPACT_PARTICLES
#undef CASTING_IMPACT_IMPACT_PARTICLE_COUNT
#undef CASTING_IMPACT_IMAGE_X
