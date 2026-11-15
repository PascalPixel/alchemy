/// @file
/// The summons that spend one Djinni: Mercury, Jupiter, Venus and Mars.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Rotates w-by-h src clockwise into h-by-w dst.
/// The top row of src becomes the rightmost column of dst.
void BattleEffect_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
  s32 y;
  s32 x;
  s32 col;
  s32 offset;
  u8 *d;
  const u8 *s;

  y = 0;
  if (h != 0) {
    offset = 0;
    col = h;
    do {
      x = 0;
      if (w != 0) {
        d = (u8 *)(col + (u32)dst - 1);
        s = (const u8 *)(offset + (u32)src);
        do {
          x++;
          *d = *s;
          s++;
          d += h;
        } while (x != w);
      }
      y++;
      offset += w;
      col--;
    } while (y != h);
  }
}

// The Mercury Djinn's flames.

/// The dots live in the map cell buffer: 32 for each of the five flames.
#define DOTS ((struct Particle *)Ram_MapCellBuffer)
#define DOTS_PER_FLAME 32
#define FLAME_COUNT 5

/// The frame each Djinni's attack shows its damage.
#define DJINN_ATTACK_DAMAGE_FRAME 56
#define MERCURY_MOTE_SLOTS 256

/// The blue flame sheet's flame, after its shaft; Mercury turns both on their
/// side, and Venus draws the flame as its rising columns.
#define BLUE_FLAME_SHEET_FLAME PICTURE_BYTES(BLUE_FLAME_COLUMN)

/// Mercury's sheet: its Djinni, then the blue flame sheet's two pictures turned
/// on their side.
enum MercurySheetPicture {
  /// The shaft turned; never drawn.
  MERCURY_TURNED_SHAFT = SMALL_DJINNI_WIDTH * MERCURY_SMALL_DJINNI_HEIGHT,
  /// The flame turned to fly left.
  MERCURY_TURNED_FLAME = MERCURY_TURNED_SHAFT +
      BLUE_FLAME_COLUMN_HEIGHT * BLUE_FLAME_COLUMN_WIDTH
};

/// A small Mercury Djinn circles over the targets, one turn every 32 frames, on
/// a radius of 64 - 2 x frame pixels (half that across) that shrinks through
/// the middle at frame 32 and opens out on the other side. At frames 4, 7, 10,
/// 21 and 34 it throws a blue flame that shoots left 12 pixels a frame and
/// shatters on its fifth frame into 32 dots, with a shake; six frames after
/// each throw every target flashes and recoils. The dots shrink with age and
/// fall back under gravity. The effect runs 96 frames; at frame 56 the battle
/// resumes and resolves the targets (hit cue 133).
void BattleEffect_RunMercury(struct BattleEffectParams *args) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct Particle *flame;
  struct Particle *dot;
  u8 *cells;
  s32 i;
  s32 j;

  // Setup: the canvas, the blitter pair, the sheets.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleEffect;
  canvas = slots->battleCanvas;
  sheet = slots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);

  // The flame picture is the blue flame sheet turned on its side.
  cells = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
  BattleEffect_TransposeCopy(cells, work->sheet + MERCURY_TURNED_SHAFT,
                             BLUE_FLAME_COLUMN_WIDTH, BLUE_FLAME_COLUMN_HEIGHT);
  BattleEffect_TransposeCopy(cells + BLUE_FLAME_SHEET_FLAME,
                             work->sheet + MERCURY_TURNED_FLAME,
                             BLUE_FLAME_SPLASH_WIDTH, BLUE_FLAME_SPLASH_HEIGHT);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // No flame and no dot is alive yet.
  EFFECT_FREE_PARTICLES(i, work->particles, 8);
  for (i = 0; i != 512; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(SE_WAVE);
  frame = 0;
  do {
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == DJINN_ATTACK_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Flames fly 12 pixels left a frame and shatter on their fifth frame.
    for (i = 0; i != FLAME_COUNT; i++) {
      flame = &work->particles[i];
      if (flame->variant != -1) {
        blit[0](canvas, work->sheet + MERCURY_TURNED_FLAME, flame->x - 16,
                flame->y - BLUE_FLAME_SPLASH_WIDTH / 2,
                BLUE_FLAME_SPLASH_HEIGHT, BLUE_FLAME_SPLASH_WIDTH);
        flame->x -= 12;
        flame->variant++;
        if (flame->variant == 5) {
          Sound_PlayFar(SE_IMPACT);
          work->shakeFrames = 4;
          for (j = 0; j != DOTS_PER_FLAME; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_FLAME + j];
            direction = Random16() & TRIG_ANGLE_MASK;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = INTtoFIXED(flame->x);
            dot->y = INTtoFIXED(flame->y);
            dot->velocityX = (Trig_Sin(direction) * speed) >> 8;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 7;
            // Each dot lives 32 to 47 frames.
            dot->variant = (Random16() & 15) + 32;
          }
        }
      }
    }

    // The Djinn circles on a radius that shrinks through the middle.
    if (frame <= 95) {
      s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
      s32 y = (FIXEDtoINT(Trig_Cos(frame << 11) * (64 - frame * 2))) + 60;

      draw[1](canvas, work->sheet, x - 10, y - 17, SMALL_DJINNI_WIDTH, 34);
      for (i = 0; i != FLAME_COUNT; i++) {
        // On its launch frame a free flame leaves the Djinn's hand.
        if (frame == gDjinnAttacks_LaunchFrames[i]) {
          struct Particle *shot = &work->particles[i];

          if (shot->variant == -1) {
            shot->x = x - 8;
            shot->velocityX = x;
            shot->y = y;
            shot->variant = 0;
            break;
          }
        }
        // Six frames later every target flashes and recoils.
        if (frame == gDjinnAttacks_LaunchFrames[i] + 6) {
          for (j = 0; j != args->targetCount; j++) {
            EFFECT_STRIKE_TARGET(args, j, 6, KNOCKBACK_BOUNCE);
          }
        }
      }
    }

    // Each live dot shrinks from 4 to 2 pixels and falls.
    for (i = 0; i != MERCURY_MOTE_SLOTS; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 2;

        draw[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                Q_16_16_UPPER_HALF(dot->x) - size / 2,
                Q_16_16_UPPER_HALF(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, toFIXED(1) / 8);
        dot->variant--;
      }
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != 96);

  // Teardown.
  EFFECT_FINISH_CANVAS();
}

// The dots live in the map cell buffer: 32 for each of the eight balls.

#define DOT_COUNT 512
#define DOTS_PER_BALL 32
#define BALL_COUNT 8

/// Where FlameballSheet is decoded in the effect's pictures.
#define JUPITER_FLAMEBALL_PICTURES 0x320

#define DJINN_ATTACKS_MOTES_PER_FLAME 32 ///< slots for each flame's motes
#define JUPITER_FRAMES 96
#define JUPITER_BURST_Y 95 ///< a flameball bursts once below this line

/// The Jupiter Djinni whirls by the caster, trailing pink, while eight pink
/// flameballs fall from the top right onto the targets, 4 frames apart; each
/// bursts into 32 motes and hits every target.
///
/// Abilities: Jupiter.
void BattleEffect_RunJupiter(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct Particle *ball;
  struct Particle *dot;
  s32 i;
  s32 j;
  s32 angle;

  // Setup: the canvas, the blitter pair, the sheets.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                             work->sheet + JUPITER_FLAMEBALL_PICTURES, 1, 0);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The balls start above the canvas to the right; no dot is alive yet.
  for (i = 0; i != DJINN_ATTACKS_MOTES_PER_FLAME; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  for (i = 0; i != DOT_COUNT; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(SE_THUNDER);
  frame = 0;
  angle = DEGtoANG(180);
  do {
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == DJINN_ATTACK_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The Djinn circles on a radius that shrinks through the middle.
    if (frame <= 95) {
      s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
      s32 y = FIXEDtoINT(Trig_Cos(angle) * (64 - frame * 2));

      blit[0](canvas, work->sheet, x + 86, y + 28, SMALL_DJINNI_WIDTH,
              SMALL_DJINNI_HEIGHT);
    }

    // Ball i drops from frame 8 + 4i until it passes y 95 and bursts.
    for (i = 0; i != BALL_COUNT; i++) {
      ball = &work->particles[i];
      if (frame >= i * 4 + 8 && ball->y <= JUPITER_BURST_Y) {
        blit[0](canvas, work->sheet + JUPITER_FLAMEBALL_PICTURES, ball->x - 20,
                ball->y - 32, FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
        ball->x -= 6;
        ball->y += 12;
        if (ball->y > JUPITER_BURST_Y) {
          for (j = 0; j != DOTS_PER_BALL; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_BALL + j];
            direction = Random16() & TRIG_ANGLE_MASK;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = INTtoFIXED(ball->x);
            dot->y = INTtoFIXED(ball->y);
            dot->velocityX = (Trig_Sin(direction) * speed) >> 7;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 6;
            // Each dot lives 32 to 47 frames.
            dot->variant = (Random16() & 15) + 32;
          }
          Sound_PlayFar(SE_IMPACT);
          work->shakeFrames = 4;
          // Every target flashes for six frames and recoils.
          for (j = 0; j != args->targetCount; j++) {
            EFFECT_STRIKE_TARGET(args, j, 6, KNOCKBACK_BOUNCE);
          }
        }
      }
    }

    // Each live dot shrinks from 3 to 1 pixel and falls.
    for (i = 0; i != DOT_COUNT; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 1;

        draw[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                Q_16_16_UPPER_HALF(dot->x) - size / 2,
                Q_16_16_UPPER_HALF(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, toFIXED(1) / 8);
        dot->variant--;
      }
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= TRIG_TURN / 32;
    frame++;
  } while (frame != JUPITER_FRAMES);

  // Teardown.
  EFFECT_FINISH_CANVAS();
}

/// Decompress resource id into dst. If the first 128 bytes are a palette,
/// copy_pal copies it to the start of BG palettes; skip_pal skips it on
/// decoding.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal) {
  u8 *res = Resource_GetTableEntry(id);

  if (copy_pal != 0) {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, res, SHEET_PALETTE_BYTES);
  }
  if (skip_pal != 0)
    res += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(res, dst);
}

#define DOTS_PER_COLUMN 32
#define COLUMN_COUNT 10

/// Where BlueFlameSheet is decoded in the effect's pictures.
#define VENUS_BLUE_FLAME_PICTURES 0x2f8

#define VENUS_FRAMES 96

/// The Venus Djinni drops in from above the targets, swaying and trailing
/// yellow, while ten columns of yellow fire shoot up from the ground beneath
/// them one after another, each bursting into motes and hitting every target.
///
/// Abilities: Venus.
void BattleEffect_RunVenus(struct BattleEffectParams *args) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *sheet;
  struct Particle *column;
  struct Particle *dot;
  s32 i;
  s32 j;
  s32 start;
  s32 angle;

  // Setup: the canvas, the two blitters, the three sheets.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleEffect;
  canvas = slots->battleCanvas;
  sheet = slots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                      BLITTER_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)slots->ramCode;
  draw[1] = (DrawRectangle)slots->ramCodePaired;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                             work->sheet + VENUS_BLUE_FLAME_PICTURES, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Each column starts somewhere in the canvas's 64 pixels, on the ground.
  for (i = 0; i != 32; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  // No dot is alive yet.
  for (i = 0; i != DOT_COUNT; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(SE_RUMBLE);
  frame = 0;
  angle = DEGtoANG(180);
  do {
    // The Djinn swings on an arc that narrows by two pixels a frame.
    if (frame <= 79) {
      s32 x = FIXEDtoINT(Trig_Sin(angle) * 24);
      s32 y = FIXEDtoINT(Trig_Cos(angle) * (64 - frame * 2));

      draw[1](canvas, work->sheet, x + 22, y + 29, 20, 38);
    }
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == DJINN_ATTACK_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Column i starts at frame 16 + 4i and climbs 12 pixels a frame.
    for (i = 0, start = 16; i != COLUMN_COUNT; start += 4, i++) {
      column = &work->particles[i];
      if (frame >= start) {
        draw[0](canvas, work->sheet + 0x9e0, column->x - 17, column->y - 32,
                BLUE_FLAME_SPLASH_WIDTH, BLUE_FLAME_SPLASH_HEIGHT);
        if (frame == start) {
          // The burst: sixteen dots sprayed upward from the column's foot.
          for (j = 0; j != 16; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_COLUMN + j];
            direction = (Random16() & 0x7fff) + 0x4000;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = INTtoFIXED(column->x);
            dot->y = INTtoFIXED(column->y + 16);
            dot->velocityX = (Trig_Sin(direction) * speed) >> 7;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 6;
            dot->variant = (Random16() & 15) + 32;
          }
          if (i & 1)
            Sound_PlayFar(SE_IMPACT);
          work->shakeFrames = 4;
          // Every target flashes for six frames and recoils.
          for (j = 0; j != args->targetCount; j++) {
            EFFECT_STRIKE_TARGET(args, j, 6, KNOCKBACK_BOUNCE);
          }
        }
        column->y -= 12;
      }
    }

    // Each live dot shrinks from 4 to 2 pixels as its life runs out.
    for (i = 0; i != DOT_COUNT; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 2;

        draw[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                Q_16_16_UPPER_HALF(dot->x) - size / 2,
                Q_16_16_UPPER_HALF(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, toFIXED(1) / 8);
        dot->variant--;
      }
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= TRIG_TURN / 32;
    frame++;
  } while (frame != VENUS_FRAMES);

  // Teardown.
  EFFECT_FINISH_CANVAS();
}

/// The sparks live in the map cell buffer: sixteen under each of nine puffs.
#define SPARKS ((struct Particle *)Ram_MapCellBuffer)
#define PUFF_COUNT 9
#define SPARKS_PER_PUFF 16

/// Where GlowOrbSheet is decoded in the effect's pictures.
#define MARS_GLOW_ORB_PICTURES 0x320

/// A fireball blooms high over the targets and burns out, the Mars Djinni
/// appears where it was and dives onto them, nine flames erupt in a row along
/// the ground beneath them and embers stream up.
///
/// Abilities: Mars.
void BattleEffect_RunMars(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct Particle *puff;
  struct Particle *spark;
  s32 member;
  s32 frame;
  s32 i;
  s32 diveY;

  // Setup: the canvas, the blitter pair, the sheets.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + MARS_GLOW_ORB_PICTURES, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The puffs sit on an oval, alternately left and right of the middle,
  // each two frames behind the last; sixteen sparks wait under each one.
  member = 0;
  puff = work->particles;
  do {
    s32 angle = member << 11;

    puff->x = FIXEDtoINT(Trig_Sin(angle) * 24);
    puff->y = (FIXEDtoINT(Trig_Cos(angle) * 4)) + 52;
    if (member & 1)
      puff->x = 32 - puff->x;
    else
      puff->x += 32;
    puff->variant = -(member * 2);
    for (i = 0; i != SPARKS_PER_PUFF; i++) {
      spark = &SPARKS[member * SPARKS_PER_PUFF + i];
      spark->x = INTtoFIXED(((Random16() & 15) + puff->x) - 8);
      spark->y = INTtoFIXED((Random16() & 7) + 96);
      spark->velocityX = ((Random16() & 127) - 64) << 11;
      spark->velocityY = ((Random16() & 127) - 64) << 10;
      spark->z = Random16() & TRIG_ANGLE_MASK;
      spark->velocityZ = Random16() & TRIG_ANGLE_MASK;
    }
    puff++;
    member++;
  } while (member != PUFF_COUNT);

  Sound_PlayFar(SE_WHOOSH);
  frame = 0;
  diveY = -172;
  do {
    // The battle resumes and resolves the targets, hit cue 0x85.
    if (frame == DJINN_ATTACK_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The orb swells, a cel every four frames.
    if (frame <= 23)
      blit[0](canvas,
              work->sheet + MARS_GLOW_ORB_PICTURES +
                  (frame / 4) * PICTURE_BYTES(GLOW_ORB),
              40, 20, GLOW_ORB_WIDTH, GLOW_ORB_HEIGHT);
    // The Djinn appears in the orb, then dives down to the left.
    if (frame == 20)
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
          SHEET_PALETTE_BYTES);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        draw[1](canvas, work->sheet, 146 - frame * 4, diveY, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
      else
        blit[0](canvas, work->sheet, 50, 20, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
    }
    // It lands: the ember sheet replaces the Djinn's.
    if (frame == 32) {
      Sound_PlayFar(SE_CRASH);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet,
                                 1, 1);
    }
    // Each puff plays its six cels, eight frames each, once its delay ends.
    if (frame > 31) {
      member = 0;
      puff = work->particles;
      do {
        if (puff->variant >= 0 && puff->variant <= 47) {
          s32 cel = puff->variant / 8;
          u32 width;

          blit[0](canvas, work->sheet + gBattleEffect_PuffArcPictures[cel],
                  puff->x -
                      ((width = gBattleEffect_PuffArcPictureWidths[cel]) >> 1),
                  puff->y + gBattleEffect_PuffArcPictureBiasY[cel], width,
                  gBattleEffect_PuffArcPictureHeights[cel]);
        }
        member++;
        puff->variant++;
        puff++;
      } while (member != PUFF_COUNT);
    }

    // The sparks rise from frame 40, a puff's column two frames after the
    // last, wobbling four pixels either way.
    spark = SPARKS;
    member = 0;
    do {
      if (frame >= (member / SPARKS_PER_PUFF) * 2 + 40) {
        s32 size;
        s32 x;
        s32 wobble;

        size = (member & 1) + 3;
        x = Q_16_16_UPPER_HALF(spark->x) + (FIXEDtoINT(Trig_Sin(spark->z) * 4));
        draw[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                x - ((u32)size >> 1), Q_16_16_UPPER_HALF(spark->y) - size, size,
                size * 2);
        BattleEffectKernels_IntegrateVector2(spark, PARTICLE_DAMPING_NONE,
                                             -toFIXED(1) / 8);
        wobble = spark->z;
        spark->z = wobble + 0x800;
        if (spark->z > 0xffff)
          spark->z = wobble + 0x800 - 0xffff;
      }
      member++;
      spark++;
    } while (member != PUFF_COUNT * SPARKS_PER_PUFF);

    // Every target flashes for sixteen frames and recoils.
    if (frame == 38) {
      for (member = 0; member != args->targetCount; member++) {
        EFFECT_STRIKE_TARGET(args, member, 16, KNOCKBACK_BOUNCE);
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
    diveY += 8;
    frame++;
  } while (frame != 112);

  // Teardown.
  EFFECT_FINISH_CANVAS();
}
