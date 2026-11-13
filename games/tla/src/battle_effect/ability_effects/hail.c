/// @file
/// Hail and Ground, and the battle camera's shakes.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// The canvas preparer's packed arguments.
#define PREPARE(kind, side, anchor) ((kind) | (side) << 4 | (anchor) << 5)

// The ice chips live in the map cell buffer.
#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)
#define CHIP_COUNT 64

// The ice chip's four cels: ☀️'s gProjectileVolley_PelletOffsets,
// gHail_CellWidths and gHail_CellHeights.
extern u16 Data_0819851c[];
extern u8 Data_08198513[];
extern u8 Data_08198517[];

void BattleEffect_RunHail(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screen[3];
  s32 otherPoint[3];
  s32 spawn[3];
  DrawRectangle blit[2];
  struct EffectStep *chip;
  s32 i;
  s32 frame;
  s32 rise;

  // Setup: the canvas, the blitter pair for the caster's side, the water.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(effect, PREPARE(1, effect->side, 2), screen, otherPoint);
  BattleEffect_FetchRectangleBlitters(effect->side, blit);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The chips start in a 16-pixel spread at the first target and fly up
  // at random, each to live 32 to 47 frames.
  EffectPosition_ProjectBattler(effect->actors[0], spawn);
  for (i = 0; i != CHIP_COUNT; i++) {
    s32 angle;
    s32 speed;

    angle = (Random16() & 0x7fff) + 0x4000;
    speed = (Random16() & 0x1ff) + 0x80;
    CHIPS[i].x = ((spawn[0] / 2 + (Random16() & 0xf)) - 8) << 16;
    CHIPS[i].y = (spawn[1] + 8) << 16;
    CHIPS[i].velocityX = (Trig_Sin(angle) * speed) >> 9;
    CHIPS[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
    CHIPS[i].z = Random16() & 0x7f;
    CHIPS[i].velocityZ = Random16() & 0x7f;
    CHIPS[i].variant = (Random16() & 0xf) + 32;
  }

  for (frame = 0; frame != 64; frame++) {
    // The last sixteen frames fade the layer out.
    if (frame > 47)
      REG_BLDALPHA = (64 - frame) | 0x1000;
    if (frame == 1) {
      Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet,
                                 work->sheet + 0x400, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                                 work->sheet + 0x65c0, 1, 0);
    }

    // With its Djinn prelude, the Djinn hovers over the anchor.
    if (effect->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame << 11;
      x = ((-Trig_Sin(orbit) << 2) >> 16) + screen[0] / 2 - 10;
      y = ((Trig_Cos(orbit) << 1) >> 16) + screen[1] - 22;
      // Never true in 64 frames; ☀️ has the same test.
      if (frame > 69)
        y = (y - frame * 2) + 138;
      blit[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
      if (frame <= 3)
        blit[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
    }

    // Chip i flies from frame 4 + i / 4, through the two blitters in turn.
    for (i = 0, chip = CHIPS; i != CHIP_COUNT; i++, chip++) {
      if (frame >= i / 4 + 4) {
        s32 cel;
        u32 width;
        u32 height;

        cel = (chip->z / 128) & 3;
        blit[i & 1](canvas, work->sheet + 0x400 + Data_0819851c[cel],
                    HI(chip->x) - (width = Data_08198513[cel]) / 2,
                    HI(chip->y) - (height = Data_08198517[cel]) / 2, width,
                    height);
        BattleEffectKernels_IntegrateVector3(chip, 63, 0x1000);
      }
    }

    // The hit: an eight-frame shake, the battle resumes, the first target
    // flashes for sixteen frames and recoils.
    if (frame == 8) {
      work->shakeFrames = frame;
      BattlePlayback_StartFar(0x86);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 16);
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 3);
    }

    // The water rises 4 pixels a frame to 32 and drifts away from the
    // caster's side.
    rise = frame * 4;
    if (rise > 32)
      rise = 32;
    if (effect->side == 0) {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                32, 32);
    } else {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet, ((i << 5) + (frame / 4 & 31)) - 32,
                120 - rise, 32, 32);
    }

    BattleCamera_Shake(4, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// BattleEffect_RunGround, effect kind 203: the Venus Djinn's rock toss.

// On screen: the background slowly takes on the portal's colours from
// frame 17. Six rocks leave the caster one after another, every two
// frames, flung out sideways with a random lift and drift; sixteen frames
// after each leaves, it is pulled towards the first target and slowed,
// until it drops below y 20 there: then it vanishes with cue 134, an
// eight-frame shake, and the target flashing and recoiling. Each rock is
// drawn with a rotating highlight and four chips that change shape and
// flip at random every other frame. If the effect has its Djinn prelude,
// a small Venus Djinn hovers over the anchor point, drawn twice as bright
// for the first four frames, and rises off from frame 17.

// Setup: the battle effect canvas and the canvas preparer (kind 0, anchor
// 2, for the caster's side), a max blitter for the side, the portal sheet
// and the Djinn sheet in the effect work, the transfer callback, and the
// eight rocks' starting points and velocities at the caster.
// Frame loop, 96 frames, as above, with a heavy shake throughout.
// Teardown: the callback and the blitter are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleEffect_RunGround (kind 203 there
// too), with The Lost Age's heap slots, blitter modes (3 minus ☀️'s) and
// task priority, no work->effect store, the canvas preparer's packed
// arguments, and the chips' blitter built by BattleEffect_BuildBlitter
// directly. Other editions place a differently built version; only the
// English listing was compared.

#define ROCK_COUNT 6
#define CHIPS_PER_ROCK 4
// The chips' shapes and flips live in particles[32..63].
#define FIRST_CHIP 32

typedef DrawRectangle BlitterPair[2];

extern char ResourceId_PortalSheet;

// The chips: ☀️'s gHail_ChipFlips (a blitter's flags), and, by shape,
// gHail_ChipCells, _ChipX, _ChipY, _ChipWidths and _ChipHeights.
extern const u8 Data_08198524[];
extern const u16 Data_0819853a[];
extern const u8 Data_0819854c[];
extern const u8 Data_08198555[];
extern const u8 Data_08198528[];
extern const u8 Data_08198531[];

// Steps the background colours one shade toward a resource's palette.
void BattleEffect_StepPaletteToResource(s32 resource);

void BattleEffect_RunGround(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  BlitterPair draw;
  s32 *camera;
  struct MotionObject *actor;
  s32 screen[3];
  s32 otherPoint[3];
  struct EffectPosition position;
  s32 j;

  // Setup: the canvas, a max blitter for the side, the two sheets.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(effect, PREPARE(0, effect->side, 2), screen, otherPoint);
  REG_BLDALPHA = 0x1010;
  if (effect->side == 1)
    BattleEffect_LoadBlitter(
        HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X));
  else
    BattleEffect_LoadBlitter(
        HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + 0x65c0, 1, 1);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The rocks start at the caster, 66 high, flung out to the side away
  // from the caster with a random lift and drift.
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  {
    struct MotionObject *target =
        GetBattleObjectSlotFar(effect->actors[0])->object;

    for (i = 0; i != 8; i++) {
      struct EffectStep *rock = &work->particles[i];

      rock->x = actor->x;
      rock->y = 0x420000;
      rock->z = actor->z;
      rock->velocityX = (i * 0x500000) >> 5;
      rock->velocityY = (s32)(((Random16() & 127) - 64) << 16) >> 6;
      rock->velocityZ = (s32)(((Random16() & 255) - 127) << 16) >> 5;
      if (rock->x > 0)
        rock->velocityX = -rock->velocityX;
      rock->variant = 1;
    }

    for (frame = 0; frame != 96; frame++) {
      // The background takes on the portal's colours from frame 17.
      if (frame > 16)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_PortalSheet);

      // With its Djinn prelude, the Djinn hovers over the anchor and rises
      // off from frame 17; the first four frames add a bright copy.
      if (effect->prelude == 1) {
        s32 angle = frame << 11;
        s32 x = ((-Trig_Sin(angle) << 2) >> 16) + screen[0] / 2 - 10;
        s32 y = ((Trig_Cos(angle) << 1) >> 16) + screen[1] - 22;

        if (frame > 16)
          y = y - frame * 2 + 32;
        if (effect->side == 1)
          BattleEffect_LoadBlitter(
              HEAP_BLITTER_ALTERNATE,
              BLITTER(BLIT_ADDITIVE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
        else
          BattleEffect_LoadBlitter(
              HEAP_BLITTER_ALTERNATE,
              BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
        if (frame <= 3)
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, work->sheet + 0x65c0,
                                                   x, y, 20, 40);
        HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        draw[0](canvas, work->sheet + 0x65c0, x, y, 20, 40);
      }

      // Every other frame each chip takes a new shape (3 to 8) and flip.
      if ((frame & 1) == 0) {
        for (i = 0; i != 32; i++) {
          struct EffectStep *chip = &work->particles[FIRST_CHIP + i];

          chip->velocityX = Random16() % 6 + 3;
          chip->velocityY = Data_08198524[Random16() & 3];
        }
      }

      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      for (i = 0; i != ROCK_COUNT; i++) {
        struct EffectStep *rock = &work->particles[i];

        if (rock->variant == 1) {
          // Rock i flies from frame 2i: the rock, its highlight (two shapes
          // in turn, two frames each) and its four chips.
          if (frame > i * 2) {
            s32 x;
            s32 y;

            EffectPosition_ProjectPoint(&rock->x, &position);
            position.x >>= 1;
            x = position.x - 12;
            y = position.y - 24;
            draw[0](canvas, work->sheet, x, y, 24, 48);
            if ((frame & 3) <= 1)
              draw[0](canvas, work->sheet + Data_0819853a[1],
                      x + Data_0819854c[1], y + Data_08198555[1],
                      Data_08198528[1], Data_08198531[1]);
            else
              draw[0](canvas, work->sheet + Data_0819853a[2],
                      x + Data_0819854c[2], y + Data_08198555[2],
                      Data_08198528[2], Data_08198531[2]);
            for (j = 0; j != CHIPS_PER_ROCK; j++) {
              struct EffectStep *chip =
                  &work->particles[FIRST_CHIP + i * CHIPS_PER_ROCK + j];
              s32 chipX;
              s32 chipY;

              // A max blitter with the chip's own flip, for this chip only.
              {
                s32 mode = BLIT_MAX;
                s32 flags = chip->velocityY;
                /* FAKEMATCH(barrier): an empty asm after the mode and the flags
                 * are set; without it, 2 of 571 instructions differ in
                 * tla-en. */
                __asm__("");
                Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7, flags, mode);
              }
              draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
              if (chip->velocityY & COMPOSITE_FLIP_X)
                chipX = x - Data_08198528[chip->velocityX] -
                        Data_0819854c[chip->velocityX] + 24;
              else
                chipX = x + Data_0819854c[chip->velocityX];
              if (chip->velocityY & COMPOSITE_FLIP_Y)
                chipY = y - Data_08198531[chip->velocityX] -
                        Data_08198555[chip->velocityX] + 48;
              else
                chipY = y + Data_08198555[chip->velocityX];
              draw[1](canvas, work->sheet + Data_0819853a[chip->velocityX],
                      chipX, chipY, Data_08198528[chip->velocityX],
                      Data_08198531[chip->velocityX]);
              HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
            }
            rock->x += rock->velocityX;
            rock->y += rock->velocityY;
            rock->z += rock->velocityZ;
          }
          // Sixteen frames later it is pulled towards the target, 20 high,
          // and slowed until frame 2i + 85; below y 20 it hits.
          if (frame > i * 2 + 16) {
            rock->velocityX += (target->x - rock->x) >> 8;
            rock->velocityY += (0x140000 - rock->y) >> 8;
            rock->velocityZ += (target->z - rock->z) >> 8;
            if (frame < i * 2 + 85) {
              rock->velocityX = rock->velocityX * 60 / 64;
              rock->velocityY = rock->velocityY * 60 / 64;
              rock->velocityZ = rock->velocityZ * 60 / 64;
            }
            if (rock->y < 0x140000) {
              work->shakeFrames = 8;
              rock->variant = 0;
              Sound_PlayFar(134);
              BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 4);
              BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
            }
          }
        }
      }
      BattleCamera_Shake(16, 16);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

/// Shakes the battle view while the effect work's shakeFrames lasts, moving
/// BG1 and gCameraSceneParameters[3] and [4] by a random offset; once the
/// shake runs out, BG1 returns to its saved position and both parameters to
/// 120. It takes two random numbers each shaking frame, x first.
///
/// Called by nearly every effect. The x offset is centred on
/// half of rangeY too, so with rangeX smaller the x offset is never positive.
///
/// rangeX: the horizontal range, a power of two.
/// rangeY: the vertical range, a power of two; both offsets are centred on
///         half of it.
void BattleCamera_Shake(s32 rangeX, u32 rangeY) {
  struct BattleEffectWork *work;
  s32 *remainingFrames;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    s32 randomX = (rangeX - 1) & Random16();
    s32 halfRange = (s32)rangeY / 2;
    s32 offsetY = ((rangeY - 1) & Random16()) - halfRange;
    s32 offsetX = randomX - halfRange;

    gBgScroll[1].x = offsetX;
    gBgScroll[1].y = offsetY + 32;
    gCameraSceneParameters[3] = 120 - offsetX;
    gCameraSceneParameters[4] = 120 - offsetY;
    (*remainingFrames)--;
    return;
  }
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
}

/// While shakeFrames lasts, moves BG1 and the projection centre up or down
/// by a random offset within range; then restores them.
void Camera_ApplyVerticalShake(s32 range) {
  u8 *fx = Ram_HeapSlots->battleFxWork[0];
  s32 *timer;

  timer = (s32 *)(fx + 0x77a8);
  if (*timer > 0) {
    s32 shake = ((range - 1) & Random16()) - range / 2;

    gBgScroll[1].y = shake + 32;
    gCameraSceneParameters[4] = 120 - shake;
    (*timer)--;
  } else {
    gBgScroll[1].y = *(s32 *)(fx + 0x77a4);
    gCameraSceneParameters[4] = 120;
  }
}

#undef BLITTER

// Thor: a beam of light falls from a dusk sky, then a pillar of lightning and
// violet bolts strike the foes.

struct FxPartyList;

void BattleEffect_AdvanceScrollOnInterval(void);
void BattleEffect_ConfigureEffectDisplay(void);

void BattleEffect_WipeCanvas(s32 page, s32 value);

void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects

void BattleEffect_PlaceFormationObjects(s32 group, s32 x, s32 y);
void BattleEffect_RunImpactBurst(s32 group, s32 x, s32 y);

void Func_0818caa8(void *source, s32 x, s32 y, s32 size); // four motes
void SceneTransform_ApplyScale(s32 *scale);

void Projection_Set(s32 a, s32 b, s32 c);
void BattleBackground_LoadFar(s32 layer, s32 resource, s32 mode);

// The Broken Seal's iwram_call.h forwards these through static inlines; here
// that form keeps a constant from being carried across the call.

static inline s32 RatioMulQ14(s32 divisor, s32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_RatioMulQ14, 10 of 1676 instructions of BattleEffect_RunThor differ
   * in tla-en. */
  return Iwram_RatioMulQ14(divisor, value);
}
