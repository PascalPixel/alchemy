/// @file
/// Froth and Whirlwind.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define FROTH_MOTE_CELL 0xc00 ///< the mote picture in the sheet
#define FROTH_FIRST_TARGET 32 ///< the first target seat in work->particles
/// The motes set up in work->particles, before the target seats: no row of
/// gVortices_Counts may ask for more.
#define FROTH_MOTES 30

/// The tornado pictures follow the six glint cells (gBattleEffect_GlintCells)
/// in the sheet: 40x32 sections, the top and bottom ones' three frames
/// interleaved, then the middle one's.
#define WHIRLWIND_TORNADO_OFFSETS 0xc56
#define WHIRLWIND_SECTION_BYTES (40 * 32)

/// Motes fly out of the top of the user, hang, and from 48 frames after each
/// appears home in on the targets in turn; each that reaches the ground strikes
/// in a burst, flashing and knocking back its target, while the camera swings
/// round over frames 17 to 63.
///
/// Abilities: Froth, Froth Sphere, Froth Spiral (BATTLE_EFFECT_FROTH).
///
/// args->variant picks the volley from gVortices_Counts (motes and frames)
/// and how hard the screen shakes.
///
/// work->particles 0 to 29 are the motes, battle positions in 16.16 until a
/// mote strikes and then its burst's canvas pixels; from 32, one seat for
/// each target. The first 30 bytes of Ram_MapGridCellBuffer mark the motes that
/// have struck. Loads the vortex palette into the first 64 background
/// colours, sets REG_BLDALPHA and work->shakeFrames, and turns the camera
/// through BattleCamera_ApplyPhasedDelta (work->cameraDelta and cameraPhase),
/// which it removes before it returns.
void BattleEffect_RunFroth(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *palette;
  struct Actor *source;
  struct Actor *actor;
  struct Particle *point;
  struct Particle *seat;
  struct Particle *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *flags;

  // Nonzero once a mote has struck.
  flags = (s8 *)Ram_MapGridCellBuffer;
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = (DrawRectangle) * (gHeapSlots + HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      COMPOSITE_LIGHTEN);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle) * (gHeapSlots + HEAP_SLOT_RAM_CODE_2);
  source = GetBattleObjectSlotFar(work->args->user)->actor;
  y = source->y.fixed + BattleUnit_GetEffectHeightFar(work->args->user);
  // The motes start at the top of the user, flying out up to 4 pixels a
  // frame across and up to 2 up; variant -1 means no burst yet.
  for (i = 0; i != FROTH_MOTES; i++) {
    point = &work->particles[i];
    SET_POSITION(point, source->x.fixed, y, source->z.fixed);
    SET_VELOCITY(point, ((s32)((Random16() & 255) - 127) << 16) >> 5,
                 ((s32)((Random16() & 127) - 16) << 16) >> 6,
                 ((s32)((Random16() & 255) - 127) << 16) >> 5);
    point->variant = -1;
    flags[i] = 0;
  }
  // Where each target stands, on the ground.
  EFFECT_FOR_EACH_TARGET(i, work->args) {
    seat = &work->particles[FROTH_FIRST_TARGET + i];
    actor = GetBattleObjectSlotFar(work->args->targets[i])->actor;
    SET_POSITION(seat, actor->x.fixed, 0, actor->z.fixed);
  }
  work->cameraDelta = 0;
  work->cameraPhase = CAMERA_PHASE_IDLE;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_VBLANK);
  // Trails keep 75% of each pixel a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlaySongFar(SE_SCATTER);

  for (frame = 0; frame != gVortices_Counts[work->args->variant][FROTH_FRAMES];
       frame++) {
    // The camera swings 0x180 of yaw a frame over frames 17 to 63.
    camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    // 16 frames before the end the battle shows the damage, with sound 132
    // on each hit.
    if (frame == gVortices_Counts[work->args->variant][FROTH_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_WATER);
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != gVortices_Counts[work->args->variant][FROTH_MOTE_COUNT];
         i++) {
      point = &work->particles[i];
      if (frame > i * 2 && flags[i] == 0) {
        Projection_Point(point, &screenPosition);
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
        target =
            &work->particles[FROTH_FIRST_TARGET + i % work->args->targetCount];
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
          BattleEffect_TintBattler(
              work->args->targets[i % work->args->targetCount],
              SPRITE_COLOR_WHITE, POSE_DAMAGE, i % work->args->targetCount, 4);
          BattleMotion_ApplyKnockbackFar(
              work->args->targets[i % work->args->targetCount],
              KNOCKBACK_NUDGE);
          work->shakeFrames = 4;
          Sound_PlaySongFar(SE_WATER);
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
    BattleCamera_Shake(work->args->variant * 2 + 2,
                       work->args->variant * 2 + 2);
    // Tell BattleCamera_ApplyPhasedDelta a frame has ended.
    if (work->cameraPhase == CAMERA_PHASE_IDLE)
      work->cameraPhase = CAMERA_PHASE_FRAME_DONE;
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleCamera_ApplyPhasedDelta);
  EFFECT_FINISH_CANVAS();
}

/// Tornadoes sweep across the targets for 80 frames, sparkling with glints; the
/// canvas flashes and the targets reel seven times, 8 frames apart, from frame
/// 16, and the battle shows the damage on frame 32.
///
/// Abilities: Whirlwind, Tornado, Tempest, Gale, Typhoon, Hurricane
/// (BATTLE_EFFECT_WHIRLWIND).
///
/// args->variant picks the shape from gVortices_Shapes: how many tornadoes,
/// how far they sway and how hard the screen shakes.
///
/// work->particles 0 to 15 are the glints, four for each tornado (so at most
/// four tornadoes), offsets in pixels from its top left. Loads the tornado
/// palette into the first 64 background colours, fades the canvas in and out
/// through REG_BLDALPHA and keeps work->shakeFrames at 1 so the screen shakes
/// every frame.
void BattleEffect_RunWhirlwind(struct BattleEffectParams *args) {
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  u8 *palette;
  struct Particle *point;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  palette = Resource_GetTableEntry((s32)&ResourceId_TornadoSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE_2);

  // The glints: each waits up to 15 frames, then plays at a random spot on
  // its tornado.
  for (j = 0; j != 16; j++) {
    point = &work->particles[j];
    point->x = Random16() & 31;
    point->y = (Random16() & 63) + 16;
    point->variant = -(Random16() & 15);
  }
  // Trails keep half a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = BLEND_ALPHA(0, 16); // invisible to start with
  WaitFrames(1);
  Sound_PlaySongFar(SE_RUMBLE);

  for (frame = 0; frame != 80; frame++) {
    wave = (Trig_Sin(frame << 10) * 16);
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // Seven flashes of the canvas, 8 frames apart: it is filled with level 8.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(8));
    }
    // The sweep sways 16 pixels each way, once every 64 frames, about a
    // point 32 pixels right of the canvas's x 40 when the targets are the
    // party, left otherwise.
    if (work->args->side == EFFECT_SIDE_PARTY)
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
    for (i = 0; i != gVortices_Shapes[work->args->variant][WHIRLWIND_TORNADOES];
         i++) {
      x = ((gVortices_Shapes[work->args->variant][WHIRLWIND_SWAY] *
                Trig_Sin((frame << 11) + i * 0x4000) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  WHIRLWIND_TORNADO_OFFSETS,
              x, y + 16, 40, 32);
      draw[0](canvas,
              work->sheet + segment * WHIRLWIND_SECTION_BYTES +
                  (WHIRLWIND_TORNADO_OFFSETS + 6 * WHIRLWIND_SECTION_BYTES),
              x, y + 48, 40, 32);
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  (WHIRLWIND_TORNADO_OFFSETS + WHIRLWIND_SECTION_BYTES),
              x, y + 80, 40, 32);
      for (j = 0; j != 4; j++) {
        point = &work->particles[i * 4 + j];
        // A glint plays three cells, two frames each: glints 0 and 1 the
        // first three, glints 2 and 3 the last three; 1 and 3 are mirrored.
        if (point->variant >= 0) {
          cell = point->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gBattleEffect_GlintCells[cell],
                      point->x + x, point->y + y,
                      gBattleEffect_GlintCellWidths[cell],
                      gBattleEffect_GlintCellHeights[cell]);
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
    EFFECT_FOR_EACH_TARGET(j, work->args) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          BattleEffect_TintBattler(work->args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 4);
          BattleMotion_ApplyKnockbackFar(work->args->targets[j],
                                         KNOCKBACK_BOUNCE);
        }
      }
    }
    work->shakeFrames = 1;
    BattleCamera_Shake(gVortices_Shapes[work->args->variant][WHIRLWIND_SHAKE],
                       gVortices_Shapes[work->args->variant][WHIRLWIND_SHAKE] *
                           2);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef FROTH_MOTE_CELL
#undef FROTH_FIRST_TARGET
#undef FROTH_MOTES
#undef WHIRLWIND_TORNADO_OFFSETS
#undef WHIRLWIND_SECTION_BYTES
