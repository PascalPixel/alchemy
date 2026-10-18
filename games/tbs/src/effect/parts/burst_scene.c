// Part of effect.c, the battle effect object, compiled only through it.
// Gust, Flying Attack and the rush bursts.

// Abilities: Gust (kind 219); Rapid Smash, Sonic Smash, Slice (kind 310).
//
// The flurry's entries: each passes its kind of flurry to
// BattleFx_RunBurstScene. The dispatch table supplies actual effect
// arguments; their variant selects the flurry when an entry covers several
// modes.
void BattleFx_RunGust(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunBurstScene(effect, 6);
    return;
  }
  if (effect->variant == 1) {
    BattleFx_RunBurstScene(effect, 7);
    return;
  }
  BattleFx_RunBurstScene(effect, 8);
}

// Abilities: Bear Claw (kind 317); Counterstrike (kind 342).
//
// Play the first flurry style selected by the effect table.
DEFINE_EFFECT_KIND(BattleFx_RunBearClaw, BattleFx_RunBurstScene, 0)

// Abilities: Ransack (kind 321); Mad Dash (kind 343).
//
// Use the same first flurry style for this alternate effect-table entry.
DEFINE_EFFECT_KIND(BattleFx_RunRansack, BattleFx_RunBurstScene, 0)

// Abilities: Bacteria Rush (kind 328).
//
// Play the second flurry style selected by the effect table.
DEFINE_EFFECT_KIND(BattleFx_RunBacteriaRush, BattleFx_RunBurstScene, 1)

// Abilities: Freebite Rush (kind 334).
//
// Play the third flurry style selected by the effect table.
DEFINE_EFFECT_KIND(BattleFx_RunFreebiteRush, BattleFx_RunBurstScene, 2)

// Abilities: Flying Attack (kind 319); Swift Strike (kind 329).
//
// Choose flurry styles 3, 4 or 5 from the effect variant.
void BattleFx_RunFlyingAttack(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunBurstScene(effect, 3);
    return;
  }
  if (effect->variant == 1) {
    BattleFx_RunBurstScene(effect, 4);
    return;
  }
  BattleFx_RunBurstScene(effect, 5);
}

// Flurries of strikes, and the rising rock wall.
// The bytes of a gBurstScene_Records row.
enum BurstField {
  BURST_ARC_SHEET, // 0: blue arc A, else B; 1 also draws every strike as an arc
  BURST_PALETTE,   // the palette's resource (0 to 3)
  BURST_STRIKES,   // the strikes
  BURST_MOTES,     // motes thrown by each strike
  BURST_INTERVAL,  // frames between strikes
  BURST_FRAMES,    // frames in the whole effect
  BURST_FLASH      // 1 fills the canvas on each hit
};

// The value of one field of this flurry's gBurstScene_Records row.
#define BURST_RECORD(field) gBurstScene_Records[(variant)][(field)]

// The packed flash pictures and the waves.
#define BURST_FLASH_CELLS 0x5100
#define BURST_WAVE_CELLS (Ram_MapCellBuffer + 0x5e00)

// Flurry: the user closes on the first target and strikes it again and
// again; each strike shows an arc (or, in rows with arc sheet 0, a wave
// for the third and fourth of every four), flashes and throws motes. A
// strike lands 2 frames after it starts: an odd one jolts the target, and
// the last knocks it back, shakes the screen harder and ends the attack.
// The motes wait above the target until their strike releases them, then
// bounce off the ground and die out.
//
// effect: the user and the targets; prelude 1 brings in a Jupiter djinni
//         first.
// variant: the row of gBurstScene_Records.
void BattleFx_RunBurstScene(struct BattleEffectArgument *effect, s32 variant) {
  struct EffectPosition actorPos;
  struct EffectPosition targetPos;
  struct EffectPosition screen;
  s32 screenX;
  s32 screenY;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sprites;
  struct BattleCamera *camera;
  struct MotionObject *target;
  u8 *palette;
  s32 paletteId;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  sprites = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (work->effect->prelude == 1)
    BattleFx_PrepareCanvasEffect(effect, 7, work->effect->side, 2, &screenX,
                                 &screenY);

  // Pictures and palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // Pack the flash (40x288) to half width at BURST_FLASH_CELLS, as the
  // canvas is shown at double width.
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  for (row = 0; row != 288; row++) {
    for (col = 0; col != 40; col++) {
      s32 src = row * 40 + col;
      s32 dst = row * 20 + col / 2 + BURST_FLASH_CELLS;

      work->sheet[dst] = work->sheet[src];
    }
  }
  if (BURST_RECORD(BURST_ARC_SHEET) == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1,
                               1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1,
                               1);
  Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, BURST_WAVE_CELLS, 1,
                             0);
  switch (BURST_RECORD(BURST_PALETTE)) {
  case 0:
    paletteId = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 1:
    paletteId = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    paletteId = (s32)&ResourceId_BlueArcSheetB;
    break;
  default:
    paletteId = (s32)&ResourceId_EmberStreakSheet;
    break;
  }
  palette = Resource_GetTableEntry(paletteId);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  WaitFrames(1);

  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &targetPos);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails, fading to 3/4
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  // Motes wait 25 pixels above the target, set to fly towards the middle
  // of the field.
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != 768; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    SET_POSITION(mote, target->x, target->y + 0x190000, target->z);
    SET_VELOCITY(mote, (Random16() & 255) << 12,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1; // frames left (negative: not out yet)
  }
  BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0],
                                 4, 0);

  for (frame = 0; frame != BURST_RECORD(BURST_FRAMES); frame++) {
    s32 bursts = BURST_RECORD(BURST_STRIKES);

    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &actorPos);
    actorPos.x /= 2;

    // Arcs and waves; odd strikes are drawn flipped.
    if (work->effect->side == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP,
                          COMPOSITE_MAX);
    }
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    for (i = 0; i != bursts; i++) {
      s32 start = i * BURST_RECORD(BURST_INTERVAL);

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 || BURST_RECORD(BURST_ARC_SHEET) == 1) {
          // The arc: six 48x72 frames.
          if (work->effect->side == 0)
            draw[i & 1](canvas, work->sheet + step * 3456, targetPos.x / 2 - 16,
                        targetPos.y - 40, 48, 72);
          else
            draw[i & 1](canvas, work->sheet + step * 3456, targetPos.x / 2 - 32,
                        targetPos.y - 40, 48, 72);
        } else {
          // The wave: six 48x16 frames.
          if (work->effect->side == 0)
            draw[i & 1](canvas, BURST_WAVE_CELLS + step * 768,
                        targetPos.x / 2 - 16, actorPos.y - 8, 48, 16);
          else
            draw[i & 1](canvas, BURST_WAVE_CELLS + step * 768,
                        targetPos.x / 2 - 32, actorPos.y - 8, 48, 16);
        }
      }
      if (frame == start + 2) {
        // The hit; the last strike ends the attack.
        if (BURST_RECORD(BURST_FLASH) == 1)
          Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 4);
        if (i == bursts - 1) {
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
          work->shakeFrames = 8;
          BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
        } else {
          if (i & 1)
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 7);
          work->shakeFrames = 4;
          AudioCommand_PlayFar(SE_HEAVY_IMPACT);
        }
        for (j = 0; j != BURST_RECORD(BURST_MOTES); j++)
          PARTICLES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The flash: six 20x48 frames, 2 frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas,
                work->sheet + ((frame - start - 2) / 2) * 960 +
                    BURST_FLASH_CELLS,
                targetPos.x / 2 - 10, targetPos.y - 24, 20, 48);
    }
    EFFECT_RELEASE_BLITTERS();

    // Motes: drawn until they die out, bouncing off the ground.
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    for (i = 0; i != 512; i++) {
      struct EffectStep *mote = &PARTICLES[i];

      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ApplyBaseAndYOffset(&mote->x, &screen);
        screen.x /= 2;
        size >>= 3;
        size++;
        draw[(i / 2) & 1](canvas, sprites + gBattleFx_MoteCells[size - 1],
                          screen.x - size / 2, screen.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(mote, 60, -0x400);
        if (mote->y < 0x80000)
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    EFFECT_RELEASE_BLITTERS();
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
