// Part of effect.c, the battle effect object, compiled only through it.
// The casting impact scene and its image helpers.

// Casting impacts.
//
// A casting impact (enum CastingImpact, the ability's effect kind less 100)
// runs in four phases:
//
// - Gathering: for 32 frames 64 sparks close in on the caster, four more each
//   frame, each in its own random rotation. Djinni impacts (Bane to Sap, 0 to
//   7) show their Djinni 80 pixels over the caster once the first sparks have
//   gone out.
// - Background: BG2 is placed so the impact sits over the target. Impacts 15,
//   17, 24, 26 and 35 (Severe Blow) stop here and hand the canvas to another
//   effect.
// - Impact: for 20 to 80 frames, depending on the impact, the caster's
//   overlay, an orb, sparks and the impact's own animation are drawn while
//   the background palette steps toward the impact's palette. A Djinni flies
//   to the target over the first six frames, and the target is hit on
//   frame 6.
// - Finish: the canvas and its callbacks are released, or impact 21 (Meltdown)
//   hands them to the stat reduction.
//
// The particles are used twice: work->particles holds the gathering sparks
// and then, from the impact, 32 scattering sparks (variant is their
// lifetime); the 64 records at gMapCellBuffer + 0x4000 belong to the impact's
// own animation.
//
// A casting impact may hand off to another runner after removing its
// prelude callbacks and releasing the blitters that runner will replace.

// Moves each RGB555 channel of background colours 0 to 63 one level towards
// the resource palette. Colour 0 always goes towards black, whatever the
// resource holds.
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

static inline void CastingImpact_LoadResource(const void *id, void *dest,
                                              s32 skip, s32 copy) {
  /* FAKEMATCH: inline scope keeps each resource call's
   * arguments local; only BattleFx_RunCastingImpact loads through it: with its
   * 24 loads called directly it differs throughout and is 2 instructions longer
   * (tbs-en, 2026-10-14), and the same inline as a layer for the whole effect
   * family breaks BattleFx_RunProjectileVolley and BattleFx_RunBurstScene
   * (measured 2026-10-14) */
  Resource_LoadAndDecompress((s32)id, dest, skip, copy);
}

// Packs a casting impact's picture rows (Graphics_PackTileRows).
static inline void CastingImpact_PackRows(void *src, void *dest, s32 width,
                                          s32 height) {
  /* FAKEMATCH: keep packing arguments within their own inline
   * scope */
  Graphics_PackTileRows(src, dest, width, height);
}

// Draws one casting impact image with the given blitter.
static inline void CastingImpact_DrawImage(void *canvas, const void *pixels,
                                           s32 x, s32 y, s32 width, s32 height,
                                           DrawRectangle *draw) {
  /* FAKEMATCH: inline scope reloads each image address at its
   * drawing call */
  (*draw)(canvas, pixels, x, y, width, height);
}

// Draw a casting-impact image whose height is twice its width.
static inline void CastingImpact_DrawTallImage(void *canvas, const void *pixels,
                                               s32 x, s32 y, s32 width,
                                               DrawRectangle *draw) {
  /* FAKEMATCH: derive height inside the inline scope so the
   * dimension stores reuse one register instead of keeping two argument
   * temporaries alive */
  (*draw)(canvas, pixels, x, y, width, width * 2);
}

// Draw only the unscrolled height of a casting-impact image.
static inline void CastingImpact_DrawScrolledImage(void *canvas,
                                                   const void *pixels, s32 x,
                                                   s32 y, s32 width, s32 height,
                                                   s32 scroll,
                                                   DrawRectangle *draw) {
  /* FAKEMATCH: subtract the scrolled part inside the drawing
   * scope */
  (*draw)(canvas, pixels, x, y, width, height - scroll);
}

// Draw a casting-impact image cropped to 91 pixels high.
static inline void CastingImpact_DrawCroppedImage(void *canvas,
                                                  const void *pixels, s32 x,
                                                  s32 y, s32 width,
                                                  DrawRectangle *draw) {
  /* FAKEMATCH: forwarding inline; the cropped height stays inside its scope.
   * Both calls made through the blitter directly with 91 differ 16 on tbs-en:
   * the stack arguments 128 and 91 go through r0 instead of r1 and the pixels
   * pool load moves ahead of them. */
  (*draw)(canvas, pixels, x, y, width, 91);
}

// Clear the prelude image, stop its callbacks and release its blitters before
// the next effect takes over.
static inline void CastingImpact_EndPrelude(struct BattleEffectWork *work,
                                            void *canvas) {
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Iwram_ClearWords(canvas, 0x4000);
  work->effect->prelude = 0;
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
}

// The casting impacts, by kind - 100: each is named for the first ability
// whose gBattle_ActionFlags entry selects it; the four no ability selects
// keep their numbers.
//
// The trailing comments give each impact's own animation, drawn over the
// target after the orb and sparks. Most animations use the 64 particles at
// gMapCellBuffer + 0x4000, which start at the target; their pictures are in
// gMapCellBuffer. "Smoke" is puffs of smoke rising from the target; "hands
// off" impacts stop after gathering and hand the canvas to another effect.
enum CastingImpact {
  CASTING_IMPACT_BANE = 0,       // smoke
  CASTING_IMPACT_SMOG,           // smoke
  CASTING_IMPACT_MIST,           // smoke
  CASTING_IMPACT_FEVER,          // smoke
  CASTING_IMPACT_FLINT,          // nothing
  CASTING_IMPACT_SCORCH,         // flash bursts rise from the target
  CASTING_IMPACT_SQUALL,         // two bolts strike among glints
  CASTING_IMPACT_SAP,            // motes fly back to the caster
  CASTING_IMPACT_TERRA_STRIKE,   // a rock spire grows and sinks
  CASTING_IMPACT_POISON_CLOUD,   // smoke
  CASTING_IMPACT_DEADLY_POISON,  // smoke
  CASTING_IMPACT_MORTAL_DANGER,  // also Death Size; a large picture rises and
                                 // breaks
  CASTING_IMPACT_BAD_OMEN,       // skulls rise from the target
  CASTING_IMPACT_LIFE_NOURISH,   // motes fly back to the caster
  CASTING_IMPACT_AQUA_SOCK,      // a blue flame column falls onto the target
  CASTING_IMPACT_15,             // hands off
  CASTING_IMPACT_FROST_BITE,     // ice chips converge on the target
  CASTING_IMPACT_17,             // hands off
  CASTING_IMPACT_LIFE_LEECH,     // motes fly back to the caster
  CASTING_IMPACT_PSY_LEECH,      // motes fly back to the caster
  CASTING_IMPACT_BROIL,          // ember streaks spread sideways
  CASTING_IMPACT_MELTDOWN,       // nothing; hands to the stat reduction
  CASTING_IMPACT_HEAT_MIRAGE,    // also Heat Flash; smoke
  CASTING_IMPACT_BARRAGE,        // flash bursts rise from the target
  CASTING_IMPACT_24,             // hands off
  CASTING_IMPACT_ACID_BATH,      // smoke
  CASTING_IMPACT_26,             // hands off
  CASTING_IMPACT_STUN_VOLTAGE,   // two bolts strike among glints
  CASTING_IMPACT_BLINDING_SMOG,  // smoke
  CASTING_IMPACT_MURK,           // smoke
  CASTING_IMPACT_CYCLONE_SLASH,  // a tornado stands over the target
  CASTING_IMPACT_PSYPHON_SEAL,   // a rune opens over the target
  CASTING_IMPACT_ASURA,          // a beast scrolls across and breaks
  CASTING_IMPACT_FORCIBLE_ARM,   // a flame falls diagonally
  CASTING_IMPACT_TRUNCHEON_FIST, // a flame falls diagonally
  CASTING_IMPACT_SEVERE_BLOW,    // hands off
};

#define CASTING_ORB_CELLS 0x5100    // the impact orb, packed at half width
#define CASTING_DJINNI_CELLS 0x3c56 // the Djinni pictures, in gMapCellBuffer
#define CASTING_GATHER_FRAMES 32    // frames the sparks gather
#define CASTING_SPARKS 64           // sparks in work->particles
// The impact pictures' particles, in gMapCellBuffer after the Djinni.
#define CASTING_IMPACT_PARTICLES                                               \
  ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define CASTING_IMPACT_PARTICLE_COUNT 64

// gCastingImpact_ImageX has seven pictures for each side.
#define CASTING_IMAGE_X(side, image) gCastingImpact_ImageX[(side) * 7 + (image)]

// Gather sparks around the acting unit, launch the Djinni and draw its impact.
// Impacts 15, 17, 24, 26 and 35 (Severe Blow) hand off to another effect after
// gathering. command: effect arguments kind: ability (effect number minus 100)
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind) {
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
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  sprites = (u8 *)EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = command;
  if (kind == CASTING_IMPACT_MORTAL_DANGER ||
      kind == CASTING_IMPACT_TERRA_STRIKE || kind == CASTING_IMPACT_ASURA) {
    BattleFx_BeginTiledCanvas(0);
  } else {
    BattleFx_BeginCanvasLayer(0);
  }
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  //------------------------------------------------------------------
  // Graphics
  //   work->sheet: six 48x72 frames over the acting unit
  //   work->sheet + 0x5100: 20x48 impact orb, packed at half width
  //   gMapCellBuffer: ability-specific impact graphics
  //   gMapCellBuffer + 0x3c56: 20x40 Djinni graphics for each element
  //------------------------------------------------------------------
  CastingImpact_LoadResource(&ResourceId_ParticleSpritesA, sprites, 0, 0);
  CastingImpact_LoadResource(&ResourceId_FireStreakSheet, work, 1, 0);
  CastingImpact_LoadResource(&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1,
                             0);
  /* FAKEMATCH: local dimensions preserve the packing argument
   * order 2026-10-11: literal 40 and 288 differ 4 (movs r2, #40 and the buffer
   * load move ahead of lsls r3). */
  {
    s32 width = 40;
    s32 height = 288;
    CastingImpact_PackRows(Ram_MapCellBuffer, work->sheet + CASTING_ORB_CELLS,
                           width, height);
  }
  if (kind == CASTING_IMPACT_SCORCH || kind == CASTING_IMPACT_BARRAGE) {
    CastingImpact_LoadResource(&ResourceId_FlashBurstSheet, Ram_MapCellBuffer,
                               1, 0);
  } else if (kind == CASTING_IMPACT_BAD_OMEN) {
    CastingImpact_LoadResource(&ResourceId_SkullSheet, Ram_MapCellBuffer, 1, 0);
  } else if (kind == CASTING_IMPACT_SQUALL ||
             kind == CASTING_IMPACT_STUN_VOLTAGE) {
    CastingImpact_LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1,
                               0);
    CastingImpact_LoadResource(&ResourceId_LightningBoltSheet,
                               Ram_MapCellBuffer + 0xc56, 1, 0);
  } else if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
    CastingImpact_LoadResource(&ResourceId_RuneSheet, Ram_MapCellBuffer, 1, 1);
  } else if (kind == CASTING_IMPACT_TERRA_STRIKE) {
    CastingImpact_LoadResource(&ResourceId_RockSpireSheet, Ram_MapCellBuffer, 1,
                               1);
  } else if (kind == CASTING_IMPACT_AQUA_SOCK) {
    CastingImpact_LoadResource(&ResourceId_BlueFlameSheet, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind == CASTING_IMPACT_CYCLONE_SLASH) {
    CastingImpact_LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind == CASTING_IMPACT_FROST_BITE) {
    CastingImpact_LoadResource(&ResourceId_IceChipSheet, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind == CASTING_IMPACT_BROIL) {
    CastingImpact_LoadResource(&ResourceId_EmberStreakSheet, Ram_MapCellBuffer,
                               1, 0);
  } else if (kind == CASTING_IMPACT_FORCIBLE_ARM ||
             kind == CASTING_IMPACT_TRUNCHEON_FIST) {
    CastingImpact_LoadResource(&ResourceId_FlameSheetA, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind != CASTING_IMPACT_MORTAL_DANGER &&
             kind != CASTING_IMPACT_ASURA) {
    CastingImpact_LoadResource(&ResourceId_SmokeSheet, Ram_MapCellBuffer, 1, 0);
  }
  switch (kind) // Djinni element
  {
  case CASTING_IMPACT_BANE: // Earth
  case CASTING_IMPACT_FLINT:
  case CASTING_IMPACT_SAP:
  case CASTING_IMPACT_TERRA_STRIKE:
  case CASTING_IMPACT_POISON_CLOUD:
  case CASTING_IMPACT_DEADLY_POISON:
  case CASTING_IMPACT_MORTAL_DANGER:
  case CASTING_IMPACT_BAD_OMEN:
  case CASTING_IMPACT_LIFE_NOURISH:
  case CASTING_IMPACT_FORCIBLE_ARM:
    CastingImpact_LoadResource(&ResourceId_VenusDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case CASTING_IMPACT_MIST: // Water
  case CASTING_IMPACT_AQUA_SOCK:
  case CASTING_IMPACT_15:
  case CASTING_IMPACT_FROST_BITE:
  case CASTING_IMPACT_17:
  case CASTING_IMPACT_LIFE_LEECH:
  case CASTING_IMPACT_PSY_LEECH:
    CastingImpact_LoadResource(&ResourceId_MercuryDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case CASTING_IMPACT_FEVER: // Fire
  case CASTING_IMPACT_SCORCH:
  case CASTING_IMPACT_BROIL:
  case CASTING_IMPACT_MELTDOWN:
  case CASTING_IMPACT_HEAT_MIRAGE:
  case CASTING_IMPACT_BARRAGE:
  case CASTING_IMPACT_24:
  case CASTING_IMPACT_ACID_BATH:
  case CASTING_IMPACT_TRUNCHEON_FIST:
  case CASTING_IMPACT_SEVERE_BLOW:
    CastingImpact_LoadResource(&ResourceId_MarsDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case CASTING_IMPACT_SMOG: // Wind
  case CASTING_IMPACT_SQUALL:
  case CASTING_IMPACT_26:
  case CASTING_IMPACT_STUN_VOLTAGE:
  case CASTING_IMPACT_BLINDING_SMOG:
  case CASTING_IMPACT_MURK:
  case CASTING_IMPACT_CYCLONE_SLASH:
  case CASTING_IMPACT_PSYPHON_SEAL:
  case CASTING_IMPACT_ASURA:
    CastingImpact_LoadResource(&ResourceId_JupiterDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case 100:
    CastingImpact_LoadResource(&ResourceId_MercuryDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  }
  work->transferMode =
      CANVAS_COPY_AND_FADE; // Afterimage: 3/4 for kind 12, otherwise 1/2.
  if (kind == CASTING_IMPACT_BAD_OMEN) {
    work->transferValue = 75;
  } else {
    work->transferValue = 50;
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  /* FAKEMATCH: retain each output pointer while preparing its
   * call 2026-10-11: the pointers set in statements before the calls differ 38,
   * after the calls 142. */
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0],
                                     targetScreen = &targetPos);
  EffectPosition_ApplyStepAndYOffset(work->effect->actor,
                                     casterScreen = &casterPos);
  BattleFx_FetchRectangleBlitters(work->effect->side, drawPair = blitters);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  casterActor = GetBattleObjectSlotFar(work->effect->actor)->object;

  // Gathering sparks: x is radius (32-95), velocity is the rotation angle.
  {
    for (i = 0; i != CASTING_SPARKS; i++) {
      particle = &work->particles[i];
      SET_POSITION(particle, (Random16() & 63) + 32, 0, 0);
      SET_VELOCITY(particle, Random16() & 0xffff, Random16() & 0xffff,
                   Random16() & 0xffff);
    }
  }
  // Stop the acting unit, saving its motion for after the gathering, and
  // place the Djinni 80 pixels overhead.
  ObjectDispatch_ApplyValueToChildrenFar(casterActor, 0);
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
  EffectPosition_ApplyStepAndYOffset(work->effect->actor, casterScreen);
  casterScreen->x = casterScreen->x / 2;
  AudioCommand_PlayFar(SE_212);

  //------------------------------------------------------------------
  // Gather for 32 frames.
  // Start four sparks each frame; shrink their radii by 4 toward the acting
  // unit. Kinds 0-7 reveal the Djinni as the sparks begin to disappear.
  //------------------------------------------------------------------
  frame = 0;
  do {
    n = 0;
    for (i = 0; i != CASTING_SPARKS; i++) {
      struct EffectStep *step = &work->particles[i];
      if (step->x >= 0) {
        if (frame >= i / 4) {
          {
            s32 size = 5;
            Render_ResetTransformState();
            SceneTransform_ApplyRoll(step->velocityZ);
            SceneTransform_ApplyPitch(step->velocityX);
            SceneTransform_ApplyYaw(step->velocityY);
            EffectPosition_ApplyBaseAndYOffset(step, &sparkScreen);
            sparkScreen.x = sparkScreen.x / 2 + casterScreen->x;
            // Djinni kinds circle the caster's chest; the rest circle lower,
            // Severe Blow lowest.
            if (kind <= 7)
              sparkScreen.y = sparkScreen.y + casterScreen->y - 8;
            else if (kind == CASTING_IMPACT_SEVERE_BLOW)
              sparkScreen.y = sparkScreen.y + casterScreen->y + 44;
            else
              sparkScreen.y = sparkScreen.y + casterScreen->y + 12;
            CLAMP(sparkScreen.depth, -60, 60);
            sparkScreen.depth += 60; // 0〜120
            EFFECT_DRAW_MOTE(blitters[1], canvas, sprites, sparkScreen.x,
                             sparkScreen.y, size);
          }
          step->x -= 4;
        }
        n++;
      }
    }
    if (kind <= 7) {
      alive = n;
      if (alive <= 63) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        EffectPosition_ApplyBaseAndYOffset(&djinnPos, &sparkScreen);
        sparkScreen.x /= 2;
        djinnX = sparkScreen.x;
        blitters[0](canvas, Ram_MapCellBuffer + CASTING_DJINNI_CELLS,
                    djinnX - 10, sparkScreen.y - 4, 20, 40);
      }
    }
    EFFECT_PRESENT_FRAME(work);
    frame++;
  } while (frame != CASTING_GATHER_FRAMES);
  /* FAKEMATCH: restart the pointer's lifetime after gathering
   * so the projection above uses the stack position directly 2026-10-11: direct
   * djinnPos stores above, with this as the only assignment, differ 114, as
   * does removing it. */
  position = &djinnPos;

  //------------------------------------------------------------------
  // Prepare BG2 for each ability: normal horizontal scale, impact at the
  // target.
  //------------------------------------------------------------------
  if (kind == CASTING_IMPACT_MORTAL_DANGER) {
    REG_BG2PA = Q_8_8(1);
    if (work->effect->side == 0) {
      REG_BG2X = (frame - targetScreen->x) << 8; // frame is 32 here.
    } else {
      REG_BG2X = (96 - targetScreen->x) << 8;
    }
  } else {
    if (kind == CASTING_IMPACT_ASURA) {
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
  if (kind == CASTING_IMPACT_TERRA_STRIKE) {
    REG_BG2PA = Q_8_8(1);
    REG_BG2X = (64 - targetScreen->x) << 8;
    work->transferMode = CANVAS_COPY_AND_CLEAR; // Clear to 0 each frame.
    work->transferValue = 0;
    Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
    Iwram_ClearWords(canvas, 0x4000);
    REG_BLDCNT = 0;
  }
  if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
    REG_BG2PA = Q_8_8(1);
    if (work->effect->side == 0) {
      REG_BG2X = (32 - targetScreen->x) << 8;
    } else {
      REG_BG2X = (96 - targetScreen->x) << 8;
    }
  }
  // Kinds 15, 17, 24 and 26 clear the canvas and hand off to another effect
  // without giving the caster its motion back: 15 to projectile volley 9,
  // 24 to Mystic Flame's orbs and 26 to projectile volley 8; 17 ends here.
  if (kind == CASTING_IMPACT_15 || kind == CASTING_IMPACT_17 ||
      kind == CASTING_IMPACT_24 || kind == CASTING_IMPACT_26) {
    CastingImpact_EndPrelude(work, canvas);
    ObjectDispatch_ApplyArgumentToChildrenFar(casterActor, 3);
    if (kind == CASTING_IMPACT_15) {
      BattleFx_RunProjectileVolley(command, 9);
    }
    if (kind == CASTING_IMPACT_24) {
      BattleFx_RunMysticFlame(command);
    }
    if (kind != CASTING_IMPACT_26) {
      return;
    }
    BattleFx_RunProjectileVolley(command, 8);
    return;
  }
  ObjectDispatch_ApplyValueToChildrenFar(casterActor, 16);
  SET_VELOCITY(casterActor, savedVelocityX, savedVelocityY, savedVelocityZ);
  casterActor->acceleration = savedAcceleration;
  casterActor->verticalMotionStrength = savedVerticalStrength;
  // Kind 35 (Severe Blow), with the caster's motion back, clears the canvas
  // and hands off to BattleFx_RunSparkGroups, mode 2, with variant 3.
  if (kind == CASTING_IMPACT_SEVERE_BLOW) {
    CastingImpact_EndPrelude(work, canvas);
    command->variant = 3;
    BattleFx_RunSparkGroups(command, 2);
    return;
  }

  // Djinni step: reach 30 pixels above the target in six frames.
  targetActor = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  motion = &velocity;
  SET_POSITION(motion, (targetActor->x - position->x) / 6,
               (targetActor->y - position->y + 0x1e0000) / 6,
               (targetActor->z - position->z) / 6);
  for (i = 0; i != CASTING_SPARKS; i++) {
    particle = &work->particles[i];
    particle->variant = 0;
  }
  // 32 sparks scatter from half the target's height, variant their lifetime;
  // Aqua Sock throws its own later, and Psyphon Seal's rise more slowly.
  if (kind != CASTING_IMPACT_AQUA_SOCK) {
    s32 height = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
    for (i = 0; i != 32; i++) {
      particle = &work->particles[i];
      SET_POSITION(particle, targetActor->x, height, targetActor->z);
      if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
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
  // Kinds 11 and 32 replace the acting-unit overlay and impact graphics.
  if (kind == CASTING_IMPACT_MORTAL_DANGER) {
    CastingImpact_LoadResource(&ResourceId_CounterRevealSheet, work, 1, 1);
    CastingImpact_LoadResource(&ResourceId_RedCrescentSheetA, Ram_MapCellBuffer,
                               1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 14);
  }
  if (kind == CASTING_IMPACT_ASURA) {
    CastingImpact_LoadResource(&ResourceId_BlueBeastSheet, work->imageStorage,
                               1, 1);
    CastingImpact_LoadResource(&ResourceId_RedCrescentSheetB, Ram_MapCellBuffer,
                               1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 14);
  }
  // The 64 particles of the impact's own animation start at the target, 20
  // pixels up (on the ground for Bad Omen), flying outwards. The returning
  // motes start theirs on frame 12; Mortal Danger and Asura use none.
  if (kind != CASTING_IMPACT_SAP && kind != CASTING_IMPACT_LIFE_NOURISH &&
      kind != CASTING_IMPACT_LIFE_LEECH &&
      kind != CASTING_IMPACT_MORTAL_DANGER && kind != CASTING_IMPACT_ASURA &&
      kind != CASTING_IMPACT_PSY_LEECH) {
    s32 height = 0;
    if (kind != CASTING_IMPACT_BAD_OMEN)
      height = 0x140000;
    for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
      particle = &CASTING_IMPACT_PARTICLES[i];
      SET_POSITION(particle, targetActor->x, height, targetActor->z);
      if (kind == CASTING_IMPACT_SCORCH || kind == CASTING_IMPACT_BARRAGE) {
        SET_VELOCITY(particle, ((Random16() & 255) - 127) << 11,
                     (Random16() & 255) << 11,
                     ((Random16() & 255) - 127) << 11);
      } else if (kind == CASTING_IMPACT_ACID_BATH) {
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
  // Kinds 2, 3, 12, 22, 28 and 29 wave BG2 by scanline.
  kindFromTwo = kind - 2;
  if ((kindFromTwo >= 0 && kindFromTwo <= 1) ||
      kind == CASTING_IMPACT_BAD_OMEN || kind == CASTING_IMPACT_HEAT_MIRAGE ||
      kind == CASTING_IMPACT_MURK || kind == CASTING_IMPACT_BLINDING_SMOG) {
    Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma,
                                  EFFECT_CALLBACK_ORDER);
  }

  // The impact lasts 20, 32, 40, 48, 64 or 80 frames, by kind.
  kindFromFour = kind - 4;
  if ((kindFromFour >= 0 && kindFromFour <= 2) ||
      kind == CASTING_IMPACT_BARRAGE || kind == CASTING_IMPACT_CYCLONE_SLASH ||
      kind == CASTING_IMPACT_STUN_VOLTAGE ||
      kind == CASTING_IMPACT_FORCIBLE_ARM ||
      kind == CASTING_IMPACT_TRUNCHEON_FIST || kind == 100) {
    duration = 32;
  } else if (kind >= 0 && kind <= 3 || kind == CASTING_IMPACT_TERRA_STRIKE ||
             kind == CASTING_IMPACT_POISON_CLOUD ||
             kind == CASTING_IMPACT_DEADLY_POISON ||
             kind == CASTING_IMPACT_HEAT_MIRAGE ||
             kind == CASTING_IMPACT_ACID_BATH || kind == CASTING_IMPACT_MURK ||
             kind == CASTING_IMPACT_PSYPHON_SEAL ||
             kind == CASTING_IMPACT_AQUA_SOCK) {
    duration = 48;
  } else if (kind == CASTING_IMPACT_MELTDOWN) {
    duration = 20;
  } else if (kind == CASTING_IMPACT_MORTAL_DANGER ||
             kind == CASTING_IMPACT_ASURA || kind == CASTING_IMPACT_BROIL) {
    duration = 40;
  } else if (kind == CASTING_IMPACT_BLINDING_SMOG ||
             kind == CASTING_IMPACT_BAD_OMEN) {
    duration = 64;
  } else {
    duration = 80;
  }
  //------------------------------------------------------------------
  // Impact
  //------------------------------------------------------------------
  frame = 0;
  while (frame != duration) {
    // BG2 wave table: BG2X for each scanline.
    if (kind != CASTING_IMPACT_MORTAL_DANGER) {
      if (kind != CASTING_IMPACT_ASURA) {
        s32 base;
        scanline = work->bg2X;
        i = 0;
        base = 0x40000;
        phase = frame << 12;
        for (; i != 160; i++) {
          s32 offset = (base - ((Trig_Sin(phase) * 4))) >>
                       10; // 1 - sin pixels, in 8.8 format
          *scanline++ = offset;
          phase += 0x800;
        }
      }
    }
    // Refresh the acting unit position for the first three frames, 16 pixels
    // below.
    if (frame <= 2) {
      EffectPosition_ApplyStepAndYOffset(work->effect->actor, casterScreen);
      casterScreen->x = casterScreen->x / 2;
      casterScreen->y += 16;
    }
    // Acting-unit overlay: six frames, two ticks each.
    if (kind != CASTING_IMPACT_MORTAL_DANGER &&
        kind != CASTING_IMPACT_TERRA_STRIKE && kind != CASTING_IMPACT_ASURA &&
        kind != CASTING_IMPACT_FORCIBLE_ARM &&
        kind != CASTING_IMPACT_TRUNCHEON_FIST) {
      if (frame <= 11) {
        if (work->effect->side == 0) {
          blitters[0](canvas, work->sheet + frame / 2 * 3456,
                      casterScreen->x - 32, casterScreen->y - 40, 48, 72);
        } else {
          blitters[0](canvas, work->sheet + frame / 2 * 3456, casterScreen->x,
                      casterScreen->y - 40, 48, 72);
        }
      }
    }
    // Move the BG palette one step toward the ability's resource palette.
    switch (kind) {
    case CASTING_IMPACT_FORCIBLE_ARM:
      BattleFx_StepPaletteToResource((s32)&ResourceId_FlameSheetA);
      break;
    case CASTING_IMPACT_AQUA_SOCK:
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
      break;
    case CASTING_IMPACT_PSYPHON_SEAL:
      BattleFx_StepPaletteToResource((s32)&ResourceId_RuneSheet);
      break;
    case CASTING_IMPACT_TERRA_STRIKE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_RockSpireSheet);
      break;
    case CASTING_IMPACT_BANE:
    case CASTING_IMPACT_DEADLY_POISON:
      BattleFx_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
      break;
    case CASTING_IMPACT_BAD_OMEN:
    case CASTING_IMPACT_LIFE_NOURISH:
    case CASTING_IMPACT_ACID_BATH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_PinkBurstSheet);
      break;
    case CASTING_IMPACT_LIFE_LEECH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_IceBlockSheet);
      break;
    case CASTING_IMPACT_PSY_LEECH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
      break;
    case CASTING_IMPACT_MIST:
    case CASTING_IMPACT_MURK:
      BattleFx_StepPaletteToResource((s32)&ResourceId_TanPalette);
      break;
    case CASTING_IMPACT_SMOG:
    case CASTING_IMPACT_BLINDING_SMOG:
      BattleFx_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      break;
    case CASTING_IMPACT_FEVER:
    case CASTING_IMPACT_BROIL:
    case CASTING_IMPACT_HEAT_MIRAGE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_EmberStreakSheet);
      break;
    case CASTING_IMPACT_POISON_CLOUD:
      BattleFx_StepPaletteToResource((s32)&ResourceId_LimePalette);
      break;
    case CASTING_IMPACT_SCORCH:
    case CASTING_IMPACT_BARRAGE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_FlashBurstSheet);
      break;
    }
    // Impact orb on frames 4-15; impact particles from frame 4.
    if (kind != CASTING_IMPACT_MORTAL_DANGER &&
        kind != CASTING_IMPACT_TERRA_STRIKE && kind != CASTING_IMPACT_ASURA) {
      if (frame >= 4 && frame <= 15) {
        drawPair[1](canvas,
                    work->sheet + (frame - 4) / 2 * 960 + CASTING_ORB_CELLS,
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
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
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
    //------------------------------------------------------------------
    // Ability-specific impact
    //------------------------------------------------------------------
    if (kind == CASTING_IMPACT_SAP || kind == CASTING_IMPACT_LIFE_NOURISH ||
        kind == CASTING_IMPACT_LIFE_LEECH || kind == CASTING_IMPACT_PSY_LEECH) {
      // Motes burst from the target on frame 12 and fly back into the caster,
      // who glows white from frame 50 to 79. Mote i steers towards the
      // caster's middle from frame i + 23, by a 256th of the distance more
      // each frame, and goes out once over the caster; only the first 32 are
      // drawn.
      if (frame == 50) {
        ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1,
                                  -1, 0);
      }
      if (frame == 79) {
        ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
      }
      if (frame == 12) {
        particle = CASTING_IMPACT_PARTICLES;
        for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++, particle++) {
          SET_POSITION(particle, targetActor->x, 0x140000, targetActor->z);
          SET_VELOCITY(particle, ((Random16() & 255) - 128) << 10,
                       ((Random16() & 255) - 128) << 10,
                       ((Random16() & 255) - 128) << 10);
          particle->variant = 0;
        }
      }
      if (frame > 11) {
        struct EffectStep *step = CASTING_IMPACT_PARTICLES;
        struct MotionObject *caster;
        s32 height;
        caster = GetBattleObjectSlotFar(work->effect->actor)->object;
        height = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
        for (i = 0; i != 32; i++, step++) {
          if (step->variant >= 0) {
            s32 size = (i & 1) + 6;
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
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
    } else if (kind == CASTING_IMPACT_MELTDOWN) {
      // Draw nothing; finish with BattleFx_RunImpair.
    } else if (kind == CASTING_IMPACT_SQUALL ||
               kind == CASTING_IMPACT_STUN_VOLTAGE) {
      // Two 24x104 bolts strike the target from frame 6 to 19, three frames
      // apart in their four-picture cycle; from frame 8 to 23 three glints a
      // frame flash at random points around it, each randomly flipped. Each
      // glint takes a blitter of its own, so the pair is fetched again after.
      if (frame >= 6 && frame <= 19) {
        phase = frame;
        i = 0;
        do {
          blitters[0](canvas,
                      ((phase / 2) & 3) * 2880 + (Ram_MapCellBuffer + 0xc56),
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
          drawFlags = Random16(); // Random flips
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              gCastingImpact_GlintDrawFlags[drawFlags & 3] |
                                  COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                              COMPOSITE_MAX);
          ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE](
              canvas, Ram_MapCellBuffer + gBattleFx_GlintCells[image],
              imageX, imageY + 56, gBattleFx_GlintCellWidths[image],
              gBattleFx_GlintCellHeights[image]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
          BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
        }
      }
    } else if (kind == CASTING_IMPACT_AQUA_SOCK) {
      // Over the first 24 frames a flame column falls onto the target: a
      // 17x104 picture tiled down the screen, scrolling 16 pixels a frame,
      // its head falling 32 pixels a frame until it lands, and the 34x65 flame
      // at its foot. The screen shakes from frame 8, and from frame 2 up to
      // four more sparks a frame rise from the target into free slots.
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
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
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
        (*draw)(canvas, Ram_MapCellBuffer + 0x6e8, originX - width, fall + 47,
                width * 2, 65);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        if (frame == 8) {
          work->shakeFrames = frame;
        }
        if (frame > 1) {
          s32 emitted = 0;
          struct EffectStep *step;
          for (i = 0; i != CASTING_SPARKS; i++) {
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
    } else if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
      // Mirrored 24x48 images
      s32 height;
      EFFECT_RELEASE_BLITTERS();
      // From frame 4 to 23 the rune's two halves are drawn brightest-wins,
      // the second mirrored, each with a blitter of its own.
      if (frame >= 4 && frame <= 23) {
        pairX = targetScreen->x / 2;
        height = 48;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
        slotDraw = (DrawRectangle *)gWorkSlot;
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, pairX - 24, 48, 24,
                                height, slotDraw + HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X,
                            COMPOSITE_MAX);
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, pairX, 48, 24,
                                height, slotDraw + HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
      BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
    } else if (kind == CASTING_IMPACT_CYCLONE_SLASH) {
      // From frame 6 a tornado of three stacked 40x32 strips stands over the
      // target, cycling three pictures of two frames each; from frame 16 it
      // fades.
      s32 image;
      if (frame > 15) {
        REG_BLDALPHA = (32 - frame) | 0x1000;
      }
      if (frame > 5) {
        stripX = targetScreen->x / 2 - 20;
        image = frame / 2 % 3;
        cellOffset = image * 2560;
        blitters[0](canvas, Ram_MapCellBuffer + 0xc56 + cellOffset, stripX, 16,
                    40, 32);
        blitters[0](canvas, image * 1280 + (Ram_MapCellBuffer + 0x2a56), stripX,
                    48, 40, 32);
        blitters[0](canvas, cellOffset + (Ram_MapCellBuffer + 0x1156), stripX,
                    80, 40, 32);
      }
    } else if (kind == CASTING_IMPACT_SCORCH ||
               kind == CASTING_IMPACT_BARRAGE) {
      // Six 32x64 frames per particle, two ticks each.
      struct EffectStep *step = CASTING_IMPACT_PARTICLES;
      for (i = 0; i != 16; i++, step++) {
        // 16 flash bursts rise, two starting each frame from frame 4.
        if (frame >= i / 2 + 4) {
          s32 age = step->variant;
          if (age <= 11) {
            s32 image = age / 2;
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            projected.x /= 2;
            blitters[0](canvas, Ram_MapCellBuffer + (image << 11),
                        projected.x - 16, projected.y - 32, 32, 64);
            EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
            step->variant++;
          }
        }
      }
    } else if (kind == CASTING_IMPACT_FLINT) {
      // Draw nothing.
    } else if (kind == CASTING_IMPACT_MORTAL_DANGER) {
      // Large target overlay: seven frames, two ticks each, bobbing vertically.
      // Fill the canvas on frames 4 and 5.
      s32 imageY;
      s32 sway;
      // The 57x98 picture shows until frame 7 and the 99x69 one from frame 4;
      // the canvas flashes on frames 4 and 5, and from frame 6 five pieces
      // follow, two frames each.
      Trig_Sin(frame << 9); // Result unused
      sway = Trig_Cos(frame << 9);
      sway <<= 2;
      sway >>= 16;
      imageY = (targetScreen->y >> 16) + sway + 16;
      if (frame <= 3) {
        blitters[0](canvas, work, CASTING_IMAGE_X(work->effect->side, 0),
                    imageY + gCastingImpact_ImageY[0], 57, 98);
      } else {
        if (frame <= 7) {
          blitters[0](canvas, work, CASTING_IMAGE_X(work->effect->side, 0),
                      imageY + gCastingImpact_ImageY[0], 57, 98);
        }
        blitters[0](canvas, work->sheet + 0x15d2,
                    CASTING_IMAGE_X(work->effect->side, 1),
                    imageY + gCastingImpact_ImageY[1], 99, 69);
        if (frame == 4 || frame == 5) {
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        }
        if (frame == 6 || frame == 7) {
          blitters[0](canvas, work->sheet + 0x3081,
                      CASTING_IMAGE_X(work->effect->side, 2),
                      imageY + gCastingImpact_ImageY[2], 128, 91);
        }
        if (frame == 8 || frame == 9) {
          CastingImpact_DrawCroppedImage(
              canvas, Ram_MapCellBuffer, CASTING_IMAGE_X(work->effect->side, 3),
              imageY + gCastingImpact_ImageY[3], 128, &blitters[0]);
        }
        if (frame == 10 || frame == 11) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x2d80,
                      CASTING_IMAGE_X(work->effect->side, 4),
                      imageY + gCastingImpact_ImageY[4], 128, 59);
        }
        if (frame == 12 || frame == 13) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x4b00,
                      CASTING_IMAGE_X(work->effect->side, 5),
                      imageY + gCastingImpact_ImageY[5], 122, 29);
        }
        if (frame == 14 || frame == 15) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x58d2,
                      CASTING_IMAGE_X(work->effect->side, 6),
                      imageY + gCastingImpact_ImageY[6], 76, 25);
        }
      }
    } else if (kind == CASTING_IMPACT_ASURA) {
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
          REG_BLDALPHA = (16 - fade) | 0x1000;
        }
      }
      if (frame == 4 || frame == 5) {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
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
          blitters[0](canvas, Ram_MapCellBuffer + 0x2d80, 0, 16, 128, 59);
        }
        if (frame == 12 || frame == 13) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x4b00, 0, 16, 128, 29);
        }
        if (frame == 14 || frame == 15) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x5980, 0, 16, 128, 26);
        }
      }
    } else if (kind == CASTING_IMPACT_BROIL) {
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
                             Ram_MapCellBuffer +
                                 gBattleFx_PuffArcCells[image],
                             imageX, gBattleFx_PuffArcCellBiasY[image] + 48,
                             gBattleFx_PuffArcCellWidths[image],
                             gBattleFx_PuffArcCellHeights[image]);
        }
      }
    } else if (kind == CASTING_IMPACT_FROST_BITE) {
      // 64 ice chips close in on the target in straight lines from 32 to 159
      // pixels out, two more each frame, 6 pixels a frame. The target
      // flinches, with a sound, as each eighth chip and the last arrive.
      if (frame == 0) {
        for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
          particle = &CASTING_IMPACT_PARTICLES[i];
          SET_POSITION(particle, (Random16() & 127) + 32, 0, 0);
          SET_VELOCITY(particle, Random16() & 0xffff, Random16() & 0xffff,
                       Random16() & 0xffff);
        }
        CASTING_IMPACT_PARTICLES[63].y = 159;
      }
      {
        struct EffectStep *step = CASTING_IMPACT_PARTICLES;
        struct EffectPosition *origin;
        /* FAKEMATCH: a zero offset known only after gcse keeps the origin copy,
         * hoisted after the projection address. origin = targetScreen, or
         * targetScreen->x and ->y read directly, differ 64 on tbs-en, two
         * instructions fewer. */
        s32 bias = 0;
        for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++, step++) {
          if (step->x >= 0 && frame >= i / 2) {
            s32 image = i & 3;
            Render_ResetTransformState();
            SceneTransform_ApplyPitch(step->velocityX);
            SceneTransform_ApplyYaw(step->velocityY);
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            /* FAKEMATCH: the zero bias above; see there. */
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
    } else if (kind == CASTING_IMPACT_TERRA_STRIKE) {
      // 32-pixel-wide image grows from the ground to 96 pixels, then shrinks.
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
    } else if (kind == CASTING_IMPACT_FORCIBLE_ARM ||
               kind == CASTING_IMPACT_TRUNCHEON_FIST) {
      // 32x64 image falls diagonally onto the target over six frames.
      s32 offset;
      s32 imageX;
      s32 imageY;
      if (frame <= 5) {
        /* FAKEMATCH: calculate each center before the shared
         * centered draw 2026-10-11: targetScreen->x / 2 in each arm differs
         * 147; with offset set first 129; hoisted out of the arms 146. */
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
    } else if (kind == CASTING_IMPACT_BAD_OMEN) {
      // 16 skulls of three 24x24 pictures rise from the target, each at one of
      // four rates by its number; from frame 48 they fade.
      if (frame > 47) {
        REG_BLDALPHA = (64 - frame) | 0x1000;
      }
      {
        struct EffectStep *step = CASTING_IMPACT_PARTICLES;
        for (i = 0; i != 16; i++, step++) {
          s32 image = i % 3;
          EffectPosition_ApplyBaseAndYOffset(step, &projected);
          projected.x /= 2;
          blitters[i & 1](canvas, Ram_MapCellBuffer + image * 576,
                          projected.x - 12, projected.y - 12, 24, 24);
          EffectStep_AdvanceWithGravity3D(step, 60, 1 << ((i & 3) + 11));
          step->variant++;
        }
      }
    } else if (kind != 100) {
      // Other kinds: six 24x48 frames per particle, four ticks each.
      struct EffectStep *step = CASTING_IMPACT_PARTICLES;
      for (i = 0; i != 16; i++, step++) {
        // One puff starts each frame from frame 4; Acid Bath's rise slower.
        if (frame >= i + 4) {
          s32 age = step->variant;
          if (age <= 23) {
            s32 image = age / 4;
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            projected.x /= 2;
            blitters[i & 1](canvas, Ram_MapCellBuffer + image * 1152,
                            projected.x - 12, projected.y - 24, 24, 48);
            if (kind == CASTING_IMPACT_ACID_BATH)
              EffectStep_AdvanceWithGravity3D(step, 60, 0x400);
            else
              EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
            step->variant++;
          }
        }
      }
    }
    // Djinni kinds 0-7: fly to the target in six frames.
    if (kind <= 7) {
      if (frame <= 5) {
        EffectPosition_ApplyBaseAndYOffset(position, &projected);
        projected.x /= 2;
        djinnX = projected.x;
        drawPair[1](canvas, Ram_MapCellBuffer + CASTING_DJINNI_CELLS,
                    djinnX - 10, projected.y - 4, 20, 40);
        position->x += motion->x;
        position->y += motion->y;
        position->z += motion->z;
      }
    }
    // Impact: ability-specific shake at frame 6; target reacts at frames 6
    // and 14.
    if (frame == 3) {
      BattleEventRuntime_BeginPhaseFar(-1);
    }
    if (frame == 4) {
      AudioCommand_PlayFar(SE_HEAVY_IMPACT);
    }
    if (frame == 6) {
      if (kind == CASTING_IMPACT_FLINT || kind == CASTING_IMPACT_SCORCH ||
          kind == CASTING_IMPACT_SAP || kind == CASTING_IMPACT_LIFE_NOURISH ||
          kind == CASTING_IMPACT_LIFE_LEECH ||
          kind == CASTING_IMPACT_PSY_LEECH || kind == CASTING_IMPACT_BARRAGE ||
          kind == CASTING_IMPACT_TRUNCHEON_FIST || kind == 100) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        work->shakeFrames = 8;
      } else if (kind == CASTING_IMPACT_BROIL ||
                 kind == CASTING_IMPACT_AQUA_SOCK ||
                 kind == CASTING_IMPACT_FORCIBLE_ARM) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
        work->shakeFrames = 2;
      } else if (kind == CASTING_IMPACT_CYCLONE_SLASH ||
                 kind == CASTING_IMPACT_TERRA_STRIKE) {
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
  // Meltdown hands the cleared canvas to the stat reduction (Impair); the
  // others stop the wave callback and release the canvas.
  if (kind == CASTING_IMPACT_MELTDOWN) {
    Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
    Iwram_ClearWords(canvas, 0x4000);
    work->effect->prelude = 0;
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleFx_RunImpair(command);
  } else {
    if (kind == CASTING_IMPACT_MIST || kind == CASTING_IMPACT_FEVER ||
        kind == CASTING_IMPACT_BAD_OMEN || kind == CASTING_IMPACT_HEAT_MIRAGE ||
        kind == CASTING_IMPACT_BLINDING_SMOG || kind == CASTING_IMPACT_MURK) {
      Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
    }
    EFFECT_FINISH_CANVAS();
  }
}
