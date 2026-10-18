// Part of effect.c, the battle effect object, compiled only through it.
// The Mercury, Jupiter, Venus and Mars Djinn attacks.

// Copies a w x h byte picture into an h x w one turned 90 degrees
// clockwise: the top row of src becomes the right column of dst.
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
  s32 y;
  s32 x;
  s32 column;
  s32 offset;
  u8 *to;
  const u8 *from;

  y = 0;
  if (h != 0) {
    offset = 0;
    column = h;
    do {
      x = 0;
      if (w != 0) {
        to = (u8 *)(column + (u32)dst - 1);
        from = (const u8 *)(offset + (u32)src);
        do {
          x++;
          *to = *from;
          from++;
          to += h;
        } while (x != w);
      }
      y++;
      offset += w;
      column--;
    } while (y != h);
  }
}

// Djinni attacks, reveal particles and camera shake.
//
// Each attack draws one of the small Djinn on the effect canvas and throws its
// element at the targets:
//
// - The Mercury Djinni spirals through the centre of the field and fires five
//   blue flames to the left, each shattering into motes.
// - The Jupiter Djinni spirals the other way while eight flameballs fall from
//   the top right and shatter on the ground.
// - The Venus Djinni sways while ten flame columns rise from the ground, each
//   throwing motes upwards.
// - The Mars Djinni flares from a glowing orb, dives to the ground and sets
//   nine flames erupting in an arc while embers rise.
// - The Venus Djinni of the rock toss (Ground) throws six rocks from the
//   user, shedding chips, which swirl and home in on the first target.
//
// Every attack sets up the canvas, its blitters and its particles, then runs
// a fixed number of frames, each a list of steps run in a fixed order and
// ending with a canvas present, which waits for the next frame. The order
// matters: later steps draw over earlier ones and several consume random
// numbers.
//
// The motes the flames shatter into live in PARTICLES, 32 slots per flame;
// the flames, rocks and chips themselves live in work->particles. The
// appearance prelude leaves the canvas ready for the calling attack; each
// attack removes its callbacks before returning. Sheet cell layouts select
// the images.

#define MERCURY_FRAMES 96 // frames in the effect
#define MERCURY_FLAMES 5  // flames thrown
// The Djinn attacks share these: each flame, ball or column that lands
// throws its motes into its own DJINN_MOTES_PER_FLAME slots of PARTICLES
// (Venus's columns fill 16 of them), and the motes move through all
// DJINN_MOTE_SLOTS.
#define DJINN_MOTES_PER_FLAME 32 // slots for each flame's motes
#define DJINN_MOTE_SLOTS 512

// Abilities: Mercury (kind 251).
//
// Mercury djinni flames: the small Mercury djinni spirals around (96, 60)
// and fires five blue flames to the left; each flame shatters into 32
// motes after 5 frames, hitting every target.
//
// effect: the targets.
void BattleFx_RunMercury(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  BlitterPair blit;
  DrawRectangle *draw;
  struct EffectStep *flame;
  struct EffectStep *mote;
  u8 *cells;
  s32 i;
  s32 j;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  draw = blit;
  BattleFx_FetchRectangleBlitters(0, draw);

  // The flame pictures are the blue flame sheet turned on its side.
  cells = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
  Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
  Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        75); // each transfer fades the canvas to 3/4
  EFFECT_FREE_PARTICLES(i, work->particles, 8);
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_MOTE_SLOTS);
  AudioCommand_PlayFar(SE_162);
  frame = 0;
  do {
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(SE_133);

    // Flames move 12 pixels left a frame and shatter on their 5th frame.
    for (i = 0; i != MERCURY_FLAMES; i++) {
      flame = &work->particles[i];
      if (flame->variant != -1) {
        blit[0](canvas, work->sheet + 0x990, flame->x - 16, flame->y - 17, 65,
                34);
        flame->x -= 12;
        flame->variant++;
        if (flame->variant == 5) {
          AudioCommand_PlayFar(SE_133);
          work->shakeFrames = 4;
          // Each mote flies off in a random direction at a speed of 256 to
          // 767; its vertical speed is doubled, so the burst is twice as tall
          // as it is wide.
          for (j = 0; j != DJINN_MOTES_PER_FLAME; j++) {
            s32 angle;
            s32 speed;

            mote = &PARTICLES[i * DJINN_MOTES_PER_FLAME + j];
            angle = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = flame->x << 16;
            mote->y = flame->y << 16;
            mote->velocityX = (Trig_Sin(angle) * speed) >> 8;
            mote->velocityY = (Trig_Cos(angle) * speed) >> 7;
            mote->variant = (Random16() & 15) + 32; // lives 32 to 47 frames
          }
        }
      }
    }

    // The djinni turns once every 32 frames. Its radius, 64 - 2 * frame
    // (half that across), reaches 0 at frame 32, passing through the
    // centre, and then grows on the other side.
    if (frame <= 95) {
      s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
      s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

      draw[1](canvas, work->sheet, x - 10, y - 17, 20, 34);
      for (i = 0; i != MERCURY_FLAMES; i++) {
        // Fire a flame on each launch frame; 6 frames later the targets flash
        // white in their damage pose for 6 frames, and motion 6.
        if (frame == gMercury_LaunchFrames[i]) {
          struct EffectStep *shot = &work->particles[i];

          if (shot->variant == -1) {
            shot->x = x - 8;
            shot->velocityX = x;
            shot->y = y;
            shot->variant = 0;
            break;
          }
        }
        if (frame == gMercury_LaunchFrames[i] + 6) {
          EFFECT_FOR_EACH_TARGET(j, work->effect) {
            EFFECT_STRIKE_TARGET(work->effect, j, 6, 6);
          }
        }
      }
    }

    // Motes shrink as they age and slow as they fall. A mote's variant is
    // its remaining life and -1 marks a free slot; at life 0 it still draws
    // once before the slot is freed.
    for (i = 0; i != 256; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 2;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, HI(mote->x), HI(mote->y),
                         size);
        EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
        mote->variant--;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != MERCURY_FRAMES);
  EFFECT_FINISH_CANVAS();
}

#define JUPITER_FRAMES 96    // frames in the effect
#define JUPITER_FLAMEBALLS 8 // flameballs that fall
#define JUPITER_GROUND_Y 95  // where the flameballs land

// Abilities: Jupiter (kind 253).
//
// Jupiter djinni fall: the small Jupiter djinni spirals while eight
// flameballs fall from the top right to the bottom left, 4 frames apart;
// each shatters into 32 motes below line 95, hitting every target.
//
// effect: the targets.
void BattleFx_RunJupiter(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  BlitterPair blit;
  DrawRectangle *draw;
  struct EffectStep *ball;
  struct EffectStep *mote;
  s32 frame;
  s32 i;
  s32 j;
  s32 angle;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  draw = blit;
  BattleFx_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                             work->sheet + 0x320, 1, 0);
  // Each transfer fades the canvas to 3/4.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // The flameballs start at x 64 to 127, y -80 to -17.
  for (i = 0; i != 32; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_MOTE_SLOTS);
  AudioCommand_PlayFar(SE_171);
  frame = 0;
  angle = 0x8000; // 180 degrees
  do {
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(SE_133);

    // The djinni turns 11.25 degrees a frame. Its radius, 64 - 2 * frame
    // (half that across), reaches 0 at frame 32, passing through the
    // centre, and then grows on the other side.
    if (frame <= 95) {
      s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      blit[0](canvas, work->sheet, x + 86, y + 28, 20, 40);
    }

    // Flameballs: one every 4 frames, moving 6 pixels left and 12 down a
    // frame.
    for (i = 0; i != JUPITER_FLAMEBALLS; i++) {
      ball = &work->particles[i];
      if (frame >= i * 4 + 8 && ball->y <= JUPITER_GROUND_Y) {
        blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40,
                64);
        ball->x -= 6;
        ball->y += 12;
        if (ball->y > JUPITER_GROUND_Y) {
          for (j = 0; j != DJINN_MOTES_PER_FLAME; j++) {
            s32 direction;
            s32 speed;

            mote = &PARTICLES[i * DJINN_MOTES_PER_FLAME + j];
            direction = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = ball->x << 16;
            mote->y = ball->y << 16;
            mote->velocityX = (Trig_Sin(direction) * speed) >> 7;
            mote->velocityY = (Trig_Cos(direction) * speed) >> 6;
            mote->variant = (Random16() & 15) + 32; // lives 32 to 47 frames
          }
          AudioCommand_PlayFar(SE_133);
          work->shakeFrames = 4;
          EFFECT_FOR_EACH_TARGET(j, work->effect) {
            EFFECT_STRIKE_TARGET(work->effect, j, 6, 6);
          }
        }
      }
    }

    // Motes shrink as they age and slow as they fall.
    for (i = 0; i != DJINN_MOTE_SLOTS; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 1;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, HI(mote->x), HI(mote->y),
                         size);
        EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
        mote->variant--;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= 0x800; // 11.25 degrees
    frame++;
  } while (frame != JUPITER_FRAMES);
  EFFECT_FINISH_CANVAS();
}

// Decompresses a resource whose first 128 bytes may be a palette.
//
// id: the resource.
// dst: receives the decompressed data.
// skipPalette: the resource starts with a palette to skip.
// copyPalette: copy that palette to the start of background palette RAM.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette) {
  u8 *data = Resource_GetTableEntry(id);

  if (copyPalette != 0)
    Iwram_CopyWords((void *)BG_PLTT, data, 128);
  if (skipPalette != 0)
    data += 128;
  Resource_DecodeType01(data, dst);
}

#define VENUS_FRAMES 96  // frames in the effect
#define VENUS_COLUMNS 10 // columns of flame that rise

// Abilities: Venus (kind 250).
//
// Venus djinni columns: the small Venus djinni sways while ten columns of
// flame in its colours rise from the ground one after another, each
// bursting into motes and hitting every target.
//
// effect: the targets.
void BattleFx_RunVenus(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sheet;
  struct EffectStep *column;
  struct EffectStep *mote;
  s32 i;
  s32 j;
  s32 angle;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_Y,
                      COMPOSITE_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                             work->sheet + 0x2f8, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  // The flame columns start at the bottom, at random x.
  for (i = 0; i != 32; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_MOTE_SLOTS);
  AudioCommand_PlayFar(SE_141);
  frame = 0;
  angle = 0x8000;
  do {
    // The djinni sways from side to side as it rises and falls.
    if (frame <= 79) {
      s32 x = (Trig_Sin(angle) * 24) >> 16;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      draw[1](canvas, work->sheet, x + 22, y + 29, 20, 38);
    }
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(SE_133);
    // Ten flame columns rise 12 pixels a frame, one every 4 frames from
    // frame 16; each throws 16 motes and hits every target as it appears.
    for (i = 0; i != VENUS_COLUMNS; i++) {
      column = &work->particles[i];
      if (frame >= i * 4 + 16) {
        draw[0](canvas, work->sheet + 0x9e0, column->x - 17, column->y - 32, 34,
                65);
        if (frame == i * 4 + 16) {
          for (j = 0; j != 16; j++) {
            s32 direction;
            s32 speed;

            mote = &PARTICLES[i * DJINN_MOTES_PER_FLAME + j];
            direction = (Random16() & 0x7fff) + 0x4000;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = column->x << 16;
            mote->y = (column->y + 16) << 16;
            mote->velocityX = (Trig_Sin(direction) * speed) >> 7;
            mote->velocityY = (Trig_Cos(direction) * speed) >> 6;
            mote->variant = (Random16() & 15) + 32;
          }
          if (i & 1)
            AudioCommand_PlayFar(SE_133);
          work->shakeFrames = 4;
          EFFECT_FOR_EACH_TARGET(j, work->effect) {
            EFFECT_STRIKE_TARGET(work->effect, j, 6, 6);
          }
        }
        column->y -= 12;
      }
    }
    // Motes shrink as they age and slow as they fall.
    for (i = 0; i != DJINN_MOTE_SLOTS; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 2;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, HI(mote->x), HI(mote->y),
                         size);
        EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
        mote->variant--;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= 0x800;
    frame++;
  } while (frame != VENUS_FRAMES);
  EFFECT_FINISH_CANVAS();
}

#define MARS_FRAMES 112          // frames in the effect
#define MARS_STRIKE_FRAME 32     // the Djinni strikes the ground
#define MARS_FLAMES 9            // flames along the arc
#define MARS_EMBERS_PER_FLAME 16 // embers in each flame

// Abilities: Mars (kind 252).
//
// Mars Djinni flames: a glowing orb flares, the small Mars Djinni dives to
// the ground, and nine flames erupt in a row while embers rise. The scene
// runs for 112 frames:
//
//   0 to 23    the orb flares
//   20 to 31   the Djinni appears, then dives from frame 24
//   32         it strikes: the screen shakes and the flames start erupting
//   38         every target is hit
//   40 on      the embers rise, 16 more every 2 frames
//
// effect: the targets.
void BattleFx_RunMars(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  BlitterPair blitters;
  DrawRectangle *draw;
  struct EffectStep *step;
  struct EffectStep *mote;
  s32 index;
  s32 frame;
  s32 i;
  s32 diveY;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  draw = blitters;
  BattleFx_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + 0x320,
                             1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Nine flames along a flattened arc, 2 frames apart, each with 16
  // embers that rise wobbling from the ground beneath it.
  index = 0;
  step = work->particles;
  do {
    s32 angle = index << 11;

    step->x = (Trig_Sin(angle) * 24) >> 16;
    step->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
    if (index & 1)
      step->x = 32 - step->x;
    else
      step->x += 32;
    step->variant = -(index * 2);
    for (i = 0; i != MARS_EMBERS_PER_FLAME; i++) {
      mote = &PARTICLES[index * MARS_EMBERS_PER_FLAME + i];
      mote->x = (((Random16() & 15) + step->x) - 8) << 16;
      mote->y = ((Random16() & 7) + 96) << 16;
      mote->velocityX = ((Random16() & 127) - 64) << 11;
      mote->velocityY = ((Random16() & 127) - 64) << 10;
      mote->z = Random16() & 0xffff;
      mote->velocityZ = Random16() & 0xffff;
    }
    step++;
    index++;
  } while (index != MARS_FLAMES);

  AudioCommand_PlayFar(SE_136);
  frame = 0;
  diveY = -172;
  do {
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(SE_133);
    // A glowing orb flares, then the djinni dives down to the left.
    if (frame <= 23)
      blitters[0](canvas, work->sheet + 0x320 + (frame / 4) * 0x640, 40, 20, 40,
                  40);
    // On frame 20 the small Djinni's palette replaces the start of the
    // background palette.
    if (frame == 20)
      Iwram_CopyWords(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        draw[1](canvas, work->sheet, 146 - frame * 4, diveY, 20, 40);
      else
        blitters[0](canvas, work->sheet, 50, 20, 20, 40);
    }
    // At frame 32 it strikes: the screen shakes and the flames erupt.
    if (frame == MARS_STRIKE_FRAME) {
      AudioCommand_PlayFar(SE_145);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet,
                                 1, 1);
    }
    if (frame > 31) {
      index = 0;
      step = work->particles;
      do {
        // A flame plays the six puff cells, 8 frames each; its variant counts
        // its frames, starting below 0 so the flames erupt 2 frames apart.
        if (step->variant >= 0 && step->variant <= 47) {
          s32 cell = step->variant / 8;
          u32 width;

          // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
          width = gBattleFx_PuffArcCellWidths[cell];
          blitters[0](canvas,
                      work->sheet + gBattleFx_PuffArcCells[cell],
                      step->x - (width >> 1),
                      step->y + gBattleFx_PuffArcCellBiasY[cell], width,
                      gBattleFx_PuffArcCellHeights[cell]);
#else
          blitters[0](
              canvas, work->sheet + gBattleFx_PuffArcCells[cell],
              step->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
              step->y + gBattleFx_PuffArcCellBiasY[cell], width,
              gBattleFx_PuffArcCellHeights[cell]);
#endif
        }
        index++;
        step->variant++;
        step++;
      } while (index != MARS_FLAMES);
    }
    // The embers rise from frame 40, each flame's 16 two frames after the
    // last's, swaying 4 pixels either way; an ember's z is its sway phase.
    step = PARTICLES;
    index = 0;
    do {
      if (frame >= (index / MARS_EMBERS_PER_FLAME) * 2 + 40) {
        s32 size;
        s32 x;
        s32 angle;

        size = (index & 1) + 3;
        x = HI(step->x) + ((Trig_Sin(step->z) * 4) >> 16);
        draw[1](canvas, sheet + gBattleFx_MoteCells[size - 1],
                x - ((u32)size >> 1), HI(step->y) - size, size, size * 2);
        EffectStep_AdvanceWithGravity2D(step, 64, -0x2000);
        angle = step->z;
        step->z = angle + 0x800;
        if (step->z > 0xffff)
          step->z = angle + 0x800 - 0xffff;
      }
      index++;
      step++;
    } while (index != 144);

    if (frame == 38) {
      EFFECT_FOR_EACH_TARGET(index, work->effect) {
        EFFECT_STRIKE_TARGET(work->effect, index, 16, 6);
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
    diveY += 8;
    frame++;
  } while (frame != MARS_FRAMES);
  EFFECT_FINISH_CANVAS();
}
