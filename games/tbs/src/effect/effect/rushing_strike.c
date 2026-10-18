// Part of effect.c, the battle effect object, compiled only through it.
// Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt).

// Packs 288 rows of 40 bytes to half width, keeping every second byte
// (the bytes of the odd columns win).
//
// src: the rows to pack.
// dst: receives the packed rows, half the stride apart.
// stride: the bytes in one row of src.
// rowCount: ignored; 288 rows are always packed.
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount) {
  s32 row = 0;
  s32 rows = 288;
  s32 offset = 0;

  do {
    s32 x;

    for (x = 0; x != 40; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
    row++;
  } while (row != rows);
}

#define RUSHING_STRIKE_SPARK_COUNT 64   // sparks
#define RUSHING_STRIKE_RING_BYTES 0x3c0 // one 20x48 ring picture

// Abilities: Bone Charge (kind 311).
//
// Rush the target and scatter sparks in the fire pillar palette.
DEFINE_EFFECT_KIND(BattleFx_RunBoneCharge, BattleFx_RunRushingStrike, 0)

// Abilities: Brute Force (kind 314).
//
// Rush the target and scatter sparks in the ice-block palette.
DEFINE_EFFECT_KIND(BattleFx_RunBruteForce, BattleFx_RunRushingStrike, 1)

// Abilities: Onslaught (kind 326).
//
// Rush the target and scatter sparks in the pink-burst palette.
DEFINE_EFFECT_KIND(BattleFx_RunOnslaught, BattleFx_RunRushingStrike, 2)

// Abilities: Headbutt (kind 339).
//
// Rush the target and scatter sparks in the blast palette.
DEFINE_EFFECT_KIND(BattleFx_RunHeadbutt, BattleFx_RunRushingStrike, 3)

#define RUSHING_STRIKE_FRAMES 60 // frames in the effect
#define RUSHING_STRIKE_FRAME 10  // the user strikes

// Rushing strike: the user rushes at the first target and strikes it on
// frame 10; a ring flares where it hits and 64 sparks scatter, over 60
// frames in all.
//
// effect: the user and the targets.
// variant: the palette: 0 fire pillar, 1 ice block, 2 pink burst, 3 blast.
void BattleFx_RunRushingStrike(struct BattleEffectArgument *effect,
                               s32 variant) {
  /* FAKEMATCH: the blitter/camera declarations and independent
   * store order preserve spill slots and loop preheader order 2026-10-11: the
   * camera read is the plain slot now; Ram_MapCellBuffer at the two uses of
   * cells differs 2 (its load moves ahead of lsls r3 at the packing call). */
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  struct EffectStep *spark;
  struct EffectStep *newSpark;
  struct EffectPosition origin;
  struct EffectPosition screen;
  struct EffectPosition sparkScreen;
  struct MotionObject *target;
  s32 frame;
  s32 i;
  s32 size;
  s32 ringFrame;
  s32 paletteId;
  u8 *cells;

  // The work, the canvas and the camera.
  cells = Ram_MapCellBuffer;
  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // The top half, and the bottom half flipped; mirrored on side 1.
  if (work->effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_MAX);
  }
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

  // Pictures and palette: the ring is the yellow orb packed to half width.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  Graphics_PackTileRows(work->sheet, (u8 *)cells, 40, 288);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBurstSheet, work->sheet, 1,
                             1);
  switch (variant) {
  case 0:
    paletteId = (s32)&ResourceId_FirePillarSheetB;
    break;
  case 1:
    paletteId = (s32)&ResourceId_IceBlockSheet;
    break;
  case 2:
    paletteId = (s32)&ResourceId_PinkBurstSheet;
    break;
  default:
    paletteId = (s32)&ResourceId_BlastSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Send the user in and ready the sparks at the target (they fly towards
  // the middle of the field).
  BattleFx_SetApproachMotion(work->effect->actor, work->effect->actors[0], 10);
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
    newSpark = &work->particles[i];
    newSpark->x = target->x;
    newSpark->y = target->y + 0xa0000; // 10 pixels up
    newSpark->z = target->z;
    SET_VELOCITY(newSpark, (Random16() & 0x1ff) << 11,
                 ((Random16() & 255) - 64) << 11,
                 ((Random16() & 255) - 128) << 11);
    if (newSpark->x > 0)
      newSpark->velocityX = -newSpark->velocityX;
    newSpark->variant = i / 2 + 16; // frames left
  }
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &origin);

  for (frame = 0; frame != RUSHING_STRIKE_FRAMES; frame++) {
    // For 15 frames, draw the burst at the user in two halves.
    if (frame <= 14) {
      EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &screen);
      draw[0](canvas, work->sheet, screen.x / 2 - 16, screen.y - 48, 40, 32);
      draw[1](canvas, work->sheet, screen.x / 2 - 16, screen.y - 16, 40, 32);
    }
    // The strike.
    if (frame == RUSHING_STRIKE_FRAME) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
      BattleEventRuntime_BeginPhaseFar(134);
      work->shakeFrames = 8;
    }
    // The ring: six pictures, 2 frames each, from frame 8, at the target's x
    // and the height where the user was last drawn.
    ringFrame = frame - 8;
    if (ringFrame >= 0 && ringFrame <= 11) {
      s32 ring = ringFrame / 2;
      draw[0](canvas, cells + ring * RUSHING_STRIKE_RING_BYTES,
              origin.x / 2 - 16, screen.y - 40, 20, 48);
    }
    // The sparks shrink as their lives run out and sink slightly.
    if (ringFrame >= 0 && ringFrame <= 55) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
        spark = &work->particles[i];
        size = spark->variant;
        if (size > 0) {
          EffectPosition_ApplyBaseAndYOffset(&spark->x, &sparkScreen);
          size >>= 4;
          size += 2;
          sparkScreen.x >>= 1;
          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, sparkScreen.x, sparkScreen.y,
                           size);
          EffectStep_AdvanceWithGravity3D(spark, 60, -0x200);
          spark->variant--;
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}
