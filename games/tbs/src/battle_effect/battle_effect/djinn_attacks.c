/// @file
/// The summons that spend one Djinni: Mercury, Jupiter, Venus and Mars. Each
/// shows its Djinni washed bright, Mercury's white and the others in their
/// element's colour, while what it throws bursts over every target.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Copies a w x h byte picture into an h x w one turned 90 degrees
/// clockwise: the top row of src becomes the right column of dst.
///
/// Called twice by BattleEffect_RunMercury, to lay the blue flame sheet's two
/// pictures on their side. Writes only dst.
void BattleEffect_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
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

#define MERCURY_FRAMES 96
/// The blue flame sheet's 34x65 flame, after its 17x104 shaft; Mercury turns
/// both on their side, and Venus draws the flame as its rising columns.
#define BLUE_FLAME_SHEET_FLAME (17 * 104)

/// Mercury's sheet: its 20x40 Djinni, then the blue flame sheet's two pictures
/// turned on their side, written over the Djinni's last 6 rows, which Mercury
/// never draws.
enum MercurySheetPicture {
  /// 104x17, the shaft turned; never drawn.
  MERCURY_TURNED_SHAFT = 20 * 34,
  /// 65x34, the flame turned to fly left.
  MERCURY_TURNED_FLAME = MERCURY_TURNED_SHAFT + 104 * 17
};

#define MERCURY_FLAMES 5 ///< flames thrown
/// The mote slots Mercury walks: room for more than MERCURY_FLAMES *
/// DJINN_ATTACKS_MOTES_PER_FLAME.
#define MERCURY_MOTE_SLOTS 256
// The summons share these: each flame, ball or column that bursts throws its
// motes into its own DJINN_ATTACKS_MOTES_PER_FLAME slots of PARTICLES (Venus's
// columns fill 16 of them), and the motes move through all
// DJINN_ATTACKS_MOTE_SLOTS. A mote's variant is its life left, -1 when free;
// at life 0 it still draws once before its slot is freed. Each frame a mote
// falls under a gravity of 1/8 pixel a frame per frame (px(1) / 8), its speed
// damped to 62/64, and it shrinks a size every 16 frames of life it loses.
#define DJINN_ATTACKS_MOTES_PER_FLAME 32 ///< slots for each flame's motes
#define DJINN_ATTACKS_MOTE_SLOTS 512

/// The Mercury Djinni rises from below on the caster's side and whirls there,
/// washed white, firing five blue flames to the left; trailing like comets,
/// each bursts into a spray of motes over the targets and hits them all.
///
/// Abilities: Mercury (BATTLE_EFFECT_MERCURY).
///
/// The Djinni circles (96, 60) on a radius that shrinks through the centre at
/// frame 32. A flame bursts on its 5th frame, 60 pixels on, into 32 motes.
/// work->particles 0 to 4 hold the flames, variant -1 until launched and then
/// counting the frames since; Ram_MapGridCellBuffer takes the blue flame sheet
/// before it is turned. The palette copied is the blue flame sheet's, not the
/// Djinni's, which is what washes the Djinni white. Sets work->shakeFrames as
/// each flame bursts.
///
/// args: the targets.
void BattleEffect_RunMercury(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  BlitterPair blit;
  struct Particle *flame;
  struct Particle *mote;
  u8 *cells;
  s32 i;
  s32 j;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, blit);

  // The flame pictures are the blue flame sheet turned on its side.
  cells = Ram_MapGridCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
  BattleEffect_TransposeCopy(cells, work->sheet + MERCURY_TURNED_SHAFT, 17,
                             104);
  BattleEffect_TransposeCopy(cells + BLUE_FLAME_SHEET_FLAME,
                             work->sheet + MERCURY_TURNED_FLAME, 34, 65);
  // The canvas fades by a quarter each frame, so every flame trails like a
  // comet.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_FREE_PARTICLES(i, work->particles, 8);
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_WAVE);
  frame = 0;
  do {
    if (frame == 56)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Flames move 12 pixels left a frame and burst on their 5th frame; a
    // burst flame is still drawn, flying on off the canvas.
    for (i = 0; i != MERCURY_FLAMES; i++) {
      flame = &work->particles[i];
      if (flame->variant != -1) {
        blit[0](canvas, work->sheet + MERCURY_TURNED_FLAME, flame->x - 16,
                flame->y - 17, 65, 34);
        flame->x -= 12;
        flame->variant++;
        if (flame->variant == 5) {
          Sound_PlayFar(SE_IMPACT);
          work->shakeFrames = 4;
          // Each mote flies off in a random direction at 1 to 3 pixels a
          // frame, the vertical part doubled, so the burst is twice as tall
          // as it is wide.
          for (j = 0; j != DJINN_ATTACKS_MOTES_PER_FLAME; j++) {
            s32 angle;
            s32 speed;

            mote = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
            angle = Random16() & TRIG_ANGLE_MASK;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = px(flame->x);
            mote->y = px(flame->y);
            mote->velocityX = (Trig_Sin(angle) * speed) >> 8;
            mote->velocityY = (Trig_Cos(angle) * speed) >> 7;
            mote->variant = (Random16() & 15) + 32; // lives 32 to 47 frames
          }
        }
      }
    }

    // The Djinni turns once every 32 frames. Its radius, 64 - 2 * frame
    // (half that across), reaches 0 at frame 32, passing through the
    // centre, and then grows on the other side.
    // Always true: the loop ends at frame 95.
    if (frame <= 95) {
      s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
      s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

      blit[1](canvas, work->sheet, x - 10, y - 17, 20, 34);
      for (i = 0; i != MERCURY_FLAMES; i++) {
        // Fire a flame from the Djinni on each launch frame; 6 frames later,
        // a frame after it bursts, the targets flash white in their damage
        // pose for 6 frames and take KNOCKBACK_BOUNCE.
        if (frame == gDjinnAttacks_LaunchFrames[i]) {
          struct Particle *shot = &work->particles[i];

          if (shot->variant == -1) {
            shot->x = x - 8;
            /* FAKEMATCH(dead): velocityX keeps the launch x, which nothing
             * reads; without the store, 247 of 413 instructions differ in
             * tbs-en. */
            shot->velocityX = x;
            shot->y = y;
            shot->variant = 0;
            break;
          }
        }
        if (frame == gDjinnAttacks_LaunchFrames[i] + 6) {
          EFFECT_FOR_EACH_TARGET(j, work->args) {
            EFFECT_STRIKE_TARGET(work->args, j, 6, KNOCKBACK_BOUNCE);
          }
        }
      }
    }

    // The motes, sizes 2 to 4 (see DJINN_ATTACKS_MOTE_SLOTS).
    for (i = 0; i != MERCURY_MOTE_SLOTS; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 2;

        EFFECT_DRAW_MOTE(blit[1], canvas, sheet, Q_16_16_TO_INT(mote->x),
                         Q_16_16_TO_INT(mote->y), size);
        Particle_AdvanceWithGravity2D(mote, 62, px(1) / 8);
        mote->variant--;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != MERCURY_FRAMES);
  EFFECT_FINISH_CANVAS();
}

#define JUPITER_FRAMES 96
#define JUPITER_FLAMEBALLS 8        ///< flameballs that fall
#define JUPITER_BURST_Y 95          ///< a flameball bursts once below this line
#define JUPITER_FLAMEBALL (20 * 40) ///< 40x64, after the 20x40 Djinni

/// The Jupiter Djinni whirls by the caster, trailing pink, while eight pink
/// flameballs fall from the top right onto the targets, 4 frames apart; each
/// bursts into 32 motes and hits every target.
///
/// Abilities: Jupiter (BATTLE_EFFECT_JUPITER).
///
/// work->particles holds the flameballs. Copies the Djinni's palette over the
/// first 128 bytes of BG palette RAM, which colours the flameballs too, writes
/// REG_BLDALPHA, sets work->shakeFrames as each ball bursts.
///
/// args: the targets.
void BattleEffect_RunJupiter(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  BlitterPair blit;
  DrawRectangle *draw;
  struct Particle *ball;
  struct Particle *mote;
  s32 frame;
  s32 i;
  s32 j;
  s32 angle;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  /* FAKEMATCH(temporary): draw stands in for blit; as blit used directly, 14
   * of 348 instructions differ in tbs-en. */
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                             work->sheet + JUPITER_FLAMEBALL, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The flameballs start at x 64 to 127, y -80 to -17; 32 are placed, the
  // first 8 fall. A ball is done once it has burst.
  for (i = 0; i != 32; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_THUNDER);
  frame = 0;
  angle = TRIG_TURN / 2;
  do {
    if (frame == 56)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The Djinni turns 11.25 degrees a frame. Its radius, 64 - 2 * frame
    // (half that across), reaches 0 at frame 32, passing through the
    // centre, and then grows on the other side.
    // Always true: the loop ends at frame 95.
    if (frame <= 95) {
      s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      blit[0](canvas, work->sheet, x + 86, y + 28, 20, 40);
    }

    // Flameballs: one every 4 frames from frame 8, moving 6 pixels left and
    // 12 down a frame. Bursting throws its motes in random directions at 2 to
    // 6 pixels a frame, the vertical part doubled, and hits every target.
    for (i = 0; i != JUPITER_FLAMEBALLS; i++) {
      ball = &work->particles[i];
      if (frame >= i * 4 + 8 && ball->y <= JUPITER_BURST_Y) {
        blit[0](canvas, work->sheet + JUPITER_FLAMEBALL, ball->x - 20,
                ball->y - 32, 40, 64);
        ball->x -= 6;
        ball->y += 12;
        if (ball->y > JUPITER_BURST_Y) {
          for (j = 0; j != DJINN_ATTACKS_MOTES_PER_FLAME; j++) {
            s32 direction;
            s32 speed;

            mote = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
            direction = Random16() & TRIG_ANGLE_MASK;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = px(ball->x);
            mote->y = px(ball->y);
            mote->velocityX = (Trig_Sin(direction) * speed) >> 7;
            mote->velocityY = (Trig_Cos(direction) * speed) >> 6;
            mote->variant = (Random16() & 15) + 32; // lives 32 to 47 frames
          }
          Sound_PlayFar(SE_IMPACT);
          work->shakeFrames = 4;
          EFFECT_FOR_EACH_TARGET(j, work->args) {
            EFFECT_STRIKE_TARGET(work->args, j, 6, KNOCKBACK_BOUNCE);
          }
        }
      }
    }

    // The motes, sizes 1 to 3 (see DJINN_ATTACKS_MOTE_SLOTS).
    for (i = 0; i != DJINN_ATTACKS_MOTE_SLOTS; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 1;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, Q_16_16_TO_INT(mote->x),
                         Q_16_16_TO_INT(mote->y), size);
        Particle_AdvanceWithGravity2D(mote, 62, px(1) / 8);
        mote->variant--;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= TRIG_TURN / 32;
    frame++;
  } while (frame != JUPITER_FRAMES);
  EFFECT_FINISH_CANVAS();
}

/// Decompresses a resource whose first 128 bytes may be a palette.
///
/// Called by nearly every effect to load its pictures. Writes dst and, with
/// copyPalette, the first 128 bytes of BG palette RAM.
///
/// id: the resource.
/// dst: receives the decompressed data.
/// skipPalette: the resource starts with a palette to skip.
/// copyPalette: copy that palette to the start of background palette RAM.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette) {
  u8 *data = Resource_GetTableEntry(id);

  if (copyPalette != 0)
    Iwram_CopyWords((void *)BG_PLTT, data, 128);
  if (skipPalette != 0)
    data += 128;
  Decompress_TaggedLz(data, dst);
}

#define VENUS_FRAMES 96
#define VENUS_COLUMNS 10 ///< columns of flame that rise

/// Venus's sheet: its 20x40 Djinni, then the blue flame sheet, written over
/// the Djinni's last 2 rows, which Venus never draws.
enum VenusSheetPicture {
  VENUS_BLUE_FLAME_SHEET = 20 * 38,
  /// 34x65, the flame, drawn upside down as a rising column.
  VENUS_COLUMN = VENUS_BLUE_FLAME_SHEET + BLUE_FLAME_SHEET_FLAME
};

/// The Venus Djinni drops in from above the targets, swaying and trailing
/// yellow, while ten columns of yellow fire shoot up from the ground beneath
/// them one after another, each bursting into motes and hitting every target.
///
/// Abilities: Venus (BATTLE_EFFECT_VENUS).
///
/// work->particles holds the columns. Copies the Djinni's palette over the
/// first 128 bytes of BG palette RAM, writes REG_BLDALPHA, sets
/// work->shakeFrames as each column appears.
///
/// args: the targets.
void BattleEffect_RunVenus(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sheet;
  struct Particle *column;
  struct Particle *mote;
  s32 i;
  s32 j;
  s32 angle;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                      COMPOSITE_LIGHTEN);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                             work->sheet + VENUS_BLUE_FLAME_SHEET, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  // The flame columns start at the bottom, at random x from 0 to 63; 32
  // are placed, the first 10 rise. The columns draw flipped upside down.
  for (i = 0; i != 32; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_RUMBLE);
  frame = 0;
  angle = TRIG_TURN / 2;
  do {
    // Until frame 79 the Djinni sways 24 pixels from side to side, a turn
    // every 32 frames, while its height swings on a radius that shrinks
    // through 0 at frame 32.
    if (frame <= 79) {
      s32 x = (Trig_Sin(angle) * 24) >> 16;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      draw[1](canvas, work->sheet, x + 22, y + 29, 20, 38);
    }
    if (frame == 56)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // Ten flame columns rise 12 pixels a frame, one every 4 frames from
    // frame 16; each throws 16 motes upwards (in the upper half turn of
    // directions at 2 to 6 pixels a frame, the vertical part doubled) and
    // hits every target as it appears, with a sound for odd ones.
    for (i = 0; i != VENUS_COLUMNS; i++) {
      column = &work->particles[i];
      if (frame >= i * 4 + 16) {
        draw[0](canvas, work->sheet + VENUS_COLUMN, column->x - 17,
                column->y - 32, 34, 65);
        if (frame == i * 4 + 16) {
          for (j = 0; j != 16; j++) {
            s32 direction;
            s32 speed;

            mote = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
            direction = (Random16() & 0x7fff) + TRIG_TURN / 4;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = px(column->x);
            mote->y = px(column->y + 16);
            mote->velocityX = (Trig_Sin(direction) * speed) >> 7;
            mote->velocityY = (Trig_Cos(direction) * speed) >> 6;
            mote->variant = (Random16() & 15) + 32;
          }
          if (i & 1)
            Sound_PlayFar(SE_IMPACT);
          work->shakeFrames = 4;
          EFFECT_FOR_EACH_TARGET(j, work->args) {
            EFFECT_STRIKE_TARGET(work->args, j, 6, KNOCKBACK_BOUNCE);
          }
        }
        column->y -= 12;
      }
    }
    // The motes, sizes 2 to 4 (see DJINN_ATTACKS_MOTE_SLOTS).
    for (i = 0; i != DJINN_ATTACKS_MOTE_SLOTS; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 2;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, Q_16_16_TO_INT(mote->x),
                         Q_16_16_TO_INT(mote->y), size);
        Particle_AdvanceWithGravity2D(mote, 62, px(1) / 8);
        mote->variant--;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= TRIG_TURN / 32;
    frame++;
  } while (frame != VENUS_FRAMES);
  EFFECT_FINISH_CANVAS();
}

#define MARS_FRAMES 112
#define MARS_FLAMES 9            ///< flames along the arc
#define MARS_EMBERS_PER_FLAME 16 ///< embers in each flame
#define MARS_GLOW_ORB (20 * 40)  ///< six 40x40 pictures, after the 20x40 Djinni

/// A fireball blooms high over the targets and burns out, the Mars Djinni
/// appears where it was and dives onto them, nine flames erupt in a row along
/// the ground beneath them and embers stream up.
///
/// Abilities: Mars (BATTLE_EFFECT_MARS).
///
/// The effect runs for 112 frames:
///
///   0 to 23    the glow orb blooms and burns out
///   20 to 31   the Djinni appears in it, then dives from frame 24
///   32         it strikes: the screen shakes and the flames start erupting
///   38         every target is hit
///   40 on      the embers rise, 16 more every 2 frames
///
/// work->particles holds the nine flames, PARTICLES their 144 embers. The BG
/// palette's first 128 bytes change three times: the glow orb's at the
/// start, the Djinni's on frame 20 and the ember streak's on frame 32, when
/// the streak sheet replaces the Djinni in work->sheet. Sets
/// work->shakeFrames on the strike.
///
/// args: the targets.
void BattleEffect_RunMars(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  BlitterPair blitters;
  struct Particle *step;
  struct Particle *mote;
  s32 index;
  s32 frame;
  s32 i;
  s32 diveY;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, blitters);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + MARS_GLOW_ORB, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Nine flames spread out from x 32 alternately left and right (odd flames
  // left), 2 frames apart: flame i stands sin(i * 11.25 degrees) * 24 pixels
  // out (0, 4, 9, 13, 16, 19, 22, 23, 24), on a flattened arc from y 56 at the
  // middle to 52 at the ends. Each has 16 embers that rise wobbling from rows
  // 96 to 103 beneath it, at up to 2 pixels a frame across and 1 up or down;
  // their z is a sway phase.
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
      mote->z = Random16() & TRIG_ANGLE_MASK;
      /* FAKEMATCH(dead): nothing reads velocityZ; as Random16() alone, 161
       * of 406 instructions differ in tbs-en. */
      mote->velocityZ = Random16() & 0xffff;
    }
    step++;
    index++;
  } while (index != MARS_FLAMES);

  Sound_PlayFar(SE_WHOOSH);
  frame = 0;
  diveY = -172;
  do {
    if (frame == 56)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // A glowing orb flares, six pictures of 4 frames, then the Djinni shows
    // in it and from frame 24 dives down to the left, 4 pixels across
    // and 8 down a frame. diveY is -172 + 8 * frame, so the dive starts where
    // the Djinni stood, (50, 20).
    if (frame <= 23)
      blitters[0](canvas, work->sheet + MARS_GLOW_ORB + (frame / 4) * (40 * 40),
                  40, 20, 40, 40);
    // On frame 20 the small Djinni's palette replaces the start of the
    // background palette.
    if (frame == 20)
      Iwram_CopyWords(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        blitters[1](canvas, work->sheet, 146 - frame * 4, diveY, 20, 40);
      else
        blitters[0](canvas, work->sheet, 50, 20, 20, 40);
    }
    // At frame 32 it strikes: the screen shakes and the flames erupt. The
    // Djinni is no longer drawn, so the ember streaks take its place in the
    // sheet, with their palette.
    if (frame == 32) {
      Sound_PlayFar(SE_CRASH);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet,
                                 1, 1);
    }
    if (frame > 31) {
      index = 0;
      step = work->particles;
      do {
        // A flame plays the six puff pictures, 8 frames each; its variant
        // counts its frames, starting below 0 so the flames erupt 2 frames
        // apart.
        if (step->variant >= 0 && step->variant <= 47) {
          s32 picture = step->variant / 8;
          u32 width;

          // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
          width = gBattleEffect_PuffArcPictureWidths[picture];
          blitters[0](canvas,
                      work->sheet + gBattleEffect_PuffArcPictures[picture],
                      step->x - (width >> 1),
                      step->y + gBattleEffect_PuffArcPictureBiasY[picture],
                      width, gBattleEffect_PuffArcPictureHeights[picture]);
#else
          blitters[0](
              canvas, work->sheet + gBattleEffect_PuffArcPictures[picture],
              step->x -
                  ((width = gBattleEffect_PuffArcPictureWidths[picture]) >> 1),
              step->y + gBattleEffect_PuffArcPictureBiasY[picture], width,
              gBattleEffect_PuffArcPictureHeights[picture]);
#endif
        }
        index++;
        step->variant++;
        step++;
      } while (index != MARS_FLAMES);
    }
    // The embers rise from frame 40, each flame's 16 two frames after the
    // last's, swaying 4 pixels either way; an ember's z is its sway phase,
    // stepped 1/32 of a turn a frame. Embers never die; they rise off the
    // canvas, pulled up at 1/8 pixel a frame per frame.
    step = PARTICLES;
    index = 0;
    do {
      if (frame >= (index / MARS_EMBERS_PER_FLAME) * 2 + 40) {
        s32 size;
        s32 x;

        size = (index & 1) + 3;
        x = Q_16_16_TO_INT(step->x) + ((Trig_Sin(step->z) * 4) >> 16);
        blitters[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    x - ((u32)size >> 1), Q_16_16_TO_INT(step->y) - size, size,
                    size * 2);
        Particle_AdvanceWithGravity2D(step, 64, -px(1) / 8);
        step->z += TRIG_TURN / 32;
        // The wrap takes off one less than a turn, so a wrapped phase lands a
        // 65536th of a turn on, which the sway never shows.
        if (step->z > TRIG_ANGLE_MASK)
          step->z -= TRIG_TURN - 1;
      }
      index++;
      step++;
    } while (index != MARS_FLAMES * MARS_EMBERS_PER_FLAME);

    // Every target is hit on frame 38, flashing for 16 frames.
    if (frame == 38) {
      EFFECT_FOR_EACH_TARGET(index, work->args) {
        EFFECT_STRIKE_TARGET(work->args, index, 16, KNOCKBACK_BOUNCE);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
    diveY += 8;
    frame++;
  } while (frame != MARS_FRAMES);
  EFFECT_FINISH_CANVAS();
}

#undef MERCURY_FRAMES
#undef MERCURY_FLAMES
#undef DJINN_ATTACKS_MOTES_PER_FLAME
#undef DJINN_ATTACKS_MOTE_SLOTS
#undef JUPITER_FRAMES
#undef JUPITER_FLAMEBALLS
#undef JUPITER_BURST_Y
#undef VENUS_FRAMES
#undef VENUS_COLUMNS
#undef MARS_FRAMES
#undef MARS_FLAMES
#undef MARS_EMBERS_PER_FLAME
#undef BLUE_FLAME_SHEET_FLAME
#undef JUPITER_FLAMEBALL
#undef MARS_GLOW_ORB
#undef MERCURY_MOTE_SLOTS
