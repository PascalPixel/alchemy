/// @file
/// Froth and Whirlwind.
// Part of effect.c.

#include "../effect_internal.h"

#define FROTH_MOTE_CELL 0xc00 ///< the mote picture in the sheet
#define FROTH_FIRST_TARGET 32 ///< the first target seat in work->particles
/// The motes set up in work->particles, before the target seats: no row of
/// gVortices_Counts may ask for more.
#define FROTH_MOTES 30

/// The tornado pictures follow the six glint cells (gBattleFx_GlintCells) in
/// the sheet: 40x32 sections, the top and bottom ones' three frames
/// interleaved, then the middle one's.
#define WHIRLWIND_TORNADO_CELLS 0xc56
#define WHIRLWIND_SECTION_BYTES (40 * 32)

/// Motes fly out of the top of the user, hang, and from 48 frames after each
/// appears home in on the targets in turn; each that reaches the ground strikes
/// in a burst, flashing and knocking back its target, while the camera swings
/// round over frames 17 to 63.
///
/// Abilities: Froth, Froth Sphere, Froth Spiral (BATTLE_FX_FROTH).
///
/// effect->variant picks the volley from gVortices_Counts (motes and frames)
/// and how hard the screen shakes.
///
/// work->particles 0 to 29 are the motes, battle positions in 16.16 until a
/// mote strikes and then its burst's canvas pixels; from 32, one seat for
/// each target. The first 30 bytes of Ram_MapCellBuffer mark the motes that
/// have struck. Loads the vortex palette into the first 64 background
/// colours, sets REG_BLDALPHA and work->shakeFrames, and turns the camera
/// through Camera_ApplyPhasedDelta (work->cameraDelta and cameraPhase),
/// which it removes before it returns.
void BattleFx_RunFroth(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *palette;
  struct MotionObject *source;
  struct MotionObject *object;
  struct EffectStep *point;
  struct EffectStep *seat;
  struct EffectStep *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *flags;

  // Nonzero once a mote has struck.
  flags = (s8 *)Ram_MapCellBuffer;
  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_MAX);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER_ALTERNATE);
  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  y = source->y + BattleUnit_GetEffectHeightFar(work->effect->actor);
  // The motes start at the top of the user, flying out up to 4 pixels a
  // frame across and up to 2 up; variant -1 means no burst yet.
  for (i = 0; i != FROTH_MOTES; i++) {
    point = &work->particles[i];
    SET_POSITION(point, source->x, y, source->z);
    SET_VELOCITY(point, ((s32)((Random16() & 255) - 127) << 16) >> 5,
                 ((s32)((Random16() & 127) - 16) << 16) >> 6,
                 ((s32)((Random16() & 255) - 127) << 16) >> 5);
    point->variant = -1;
    flags[i] = 0;
  }
  // Where each target stands, on the ground.
  EFFECT_FOR_EACH_TARGET(i, work->effect) {
    seat = &work->particles[FROTH_FIRST_TARGET + i];
    object = GetBattleObjectSlotFar(work->effect->actors[i])->object;
    SET_POSITION(seat, object->x, 0, object->z);
  }
  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, EFFECT_CALLBACK_ORDER);
  // Trails keep 75% of each pixel a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  AudioCommand_PlayFar(SE_164);

  for (frame = 0;
       frame != gVortices_Counts[work->effect->variant][FROTH_FRAMES];
       frame++) {
    // The camera swings 0x180 of yaw a frame over frames 17 to 63.
    camera = WORK_SLOT(HEAP_SLOT_CAMERA);
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    // 16 frames before the end the battle shows the damage, with sound 132
    // on each hit.
    if (frame == gVortices_Counts[work->effect->variant][FROTH_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_132);
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != gVortices_Counts[work->effect->variant][FROTH_MOTE_COUNT];
         i++) {
      point = &work->particles[i];
      if (frame > i * 2 && flags[i] == 0) {
        EffectPosition_ProjectPoint(point, &screenPosition);
        screenPosition.x >>= 1;
        CLAMP(screenPosition.depth, 160, 0x31f);
        draw[0](canvas, work->sheet + FROTH_MOTE_CELL, screenPosition.x - 6,
                screenPosition.y - 12, 12, 24);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }
      // Mote i appears after frame 2i; 48 frames later it starts to home in
      // on target i modulo the target count, gaining 1/512 of the distance
      // to it a frame and damped to 60/64 until frame 2i + 85. Seats are on
      // the ground (y 0), so a mote strikes when it falls below it.
      if (frame > i * 2 + 48 && flags[i] == 0) {
        target = &work->particles[FROTH_FIRST_TARGET + i % work->effect->count];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          SET_VELOCITY(point, point->velocityX * 60 / 64,
                       point->velocityY * 60 / 64, point->velocityZ * 60 / 64);
        }
        // On the ground it strikes: its burst starts where it was last
        // drawn, a little up or down at random, and the target flashes and
        // recoils.
        if (point->y < 0) {
          flags[i] = 1;
          point->variant = 0;
          point->x = screenPosition.x;
          point->y = screenPosition.y + (Random16() & 31) - 16;
          ObjectGroup_UpdateMembers(
              work->effect->actors[i % work->effect->count], SPRITE_COLOR_WHITE,
              POSE_DAMAGE, i % work->effect->count, 4);
          BattleMotion_ApplyVariantMotionFar(
              work->effect->actors[i % work->effect->count], 0);
          work->shakeFrames = 4;
          AudioCommand_PlayFar(SE_132);
        }
      }
      // The burst where a mote struck: two mirrored halves cycling three
      // images, for 16 frames.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - 16, point->y - 56, 16, 64);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x, point->y - 56, 16, 64);
        point->variant++;
      }
    }
    Camera_ApplyShake(work->effect->variant * 2 + 2,
                      work->effect->variant * 2 + 2);
    // Tell Camera_ApplyPhasedDelta a frame has ended.
    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
  EFFECT_FINISH_CANVAS();
}

/// Tornadoes sweep across the targets for 80 frames, sparkling with glints; the
/// canvas flashes and the targets reel seven times, 8 frames apart, from frame
/// 16, and the battle shows the damage on frame 32.
///
/// Abilities: Whirlwind, Tornado, Tempest, Gale, Typhoon, Hurricane
/// (BATTLE_FX_WHIRLWIND).
///
/// effect->variant picks the shape from gVortices_Shapes: how many tornadoes,
/// how far they sway and how hard the screen shakes.
///
/// work->particles 0 to 15 are the glints, four for each tornado (so at most
/// four tornadoes), offsets in pixels from its top left. Loads the tornado
/// palette into the first 64 background colours, fades the canvas in and out
/// through REG_BLDALPHA and keeps work->shakeFrames at 1 so the screen shakes
/// every frame.
void BattleFx_RunWhirlwind(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  u8 *palette;
  struct EffectStep *point;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  palette = Resource_GetTableEntry((s32)&ResourceId_TornadoSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  // The glints: each waits up to 15 frames, then plays at a random spot on
  // its tornado.
  for (j = 0; j != 16; j++) {
    point = &work->particles[j];
    point->x = Random16() & 31;
    point->y = (Random16() & 63) + 16;
    point->variant = -(Random16() & 15);
  }
  // Trails keep half a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BLDALPHA = BLEND_ALPHA(0, 16); // invisible to start with
  WaitFrames(1);
  AudioCommand_PlayFar(SE_141);

  for (frame = 0; frame != 80; frame++) {
    wave = (Trig_Sin(frame << 10) * 16);
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_133);
    // Seven flashes of the canvas, 8 frames apart: it is filled with level 8.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, CANVAS_BYTES, 0x08080808);
    }
    // The sweep sways 16 pixels each way, once every 64 frames, about a
    // point 32 pixels right of the canvas's x 40 when the targets are the
    // party, left otherwise.
    if (work->effect->side == 1)
      wave += 0x200000;
    else
      wave -= 0x200000;
    // Fade in over 16 frames and out over the last 16.
    if (frame <= 16)
      REG_BLDALPHA = BLEND_ALPHA(frame, 16);
    if (frame > 63)
      REG_BLDALPHA = BLEND_ALPHA(0x4f - frame, 16);
    // Each tornado: three sections stacked, swaying about the sweep by its
    // row's WHIRLWIND_SWAY pixels once every 32 frames, a quarter turn apart
    // and bobbing 2 pixels, with four glints; the sections change picture
    // every 2 frames.
    for (i = 0;
         i != gVortices_Shapes[work->effect->variant][WHIRLWIND_TORNADOES];
         i++) {
      x = ((gVortices_Shapes[work->effect->variant][WHIRLWIND_SWAY] *
                Trig_Sin((frame << 11) + i * 0x4000) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  WHIRLWIND_TORNADO_CELLS,
              x, y + 16, 40, 32);
      draw[0](canvas,
              work->sheet + segment * WHIRLWIND_SECTION_BYTES +
                  (WHIRLWIND_TORNADO_CELLS + 6 * WHIRLWIND_SECTION_BYTES),
              x, y + 48, 40, 32);
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  (WHIRLWIND_TORNADO_CELLS + WHIRLWIND_SECTION_BYTES),
              x, y + 80, 40, 32);
      for (j = 0; j != 4; j++) {
        point = &work->particles[i * 4 + j];
        // A glint plays three cells, two frames each: glints 0 and 1 the
        // first three, glints 2 and 3 the last three; 1 and 3 are mirrored.
        if (point->variant >= 0) {
          cell = point->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gBattleFx_GlintCells[cell],
                      point->x + x, point->y + y,
                      gBattleFx_GlintCellWidths[cell],
                      gBattleFx_GlintCellHeights[cell]);
        }
        point->variant++;
        if (point->variant == 6) {
          point->x = Random16() & 31;
          point->y = (Random16() & 63) + 16;
          point->variant = 0;
        }
      }
    }
    // Target j reels 3j frames after each flash.
    EFFECT_FOR_EACH_TARGET(j, work->effect) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 4);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
        }
      }
    }
    work->shakeFrames = 1;
    Camera_ApplyShake(gVortices_Shapes[work->effect->variant][WHIRLWIND_SHAKE],
                      gVortices_Shapes[work->effect->variant][WHIRLWIND_SHAKE] *
                          2);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

#undef FROTH_MOTE_CELL
#undef FROTH_FIRST_TARGET
#undef FROTH_MOTES
#undef WHIRLWIND_TORNADO_CELLS
#undef WHIRLWIND_SECTION_BYTES
