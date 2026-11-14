/// @file
/// The summons that spend one Djinni: Mercury, Jupiter, Venus and Mars. Each
/// shows its Djinni washed bright, Mercury's white and the others in their
/// element's colour, while what it throws bursts over every target.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Copies a byte picture turned 90 degrees clockwise.
void BattleEffect_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
  s32 x;
  s32 y;

  for (y = 0; y != h; y++) {
    for (x = 0; x != w; x++)
      dst[x * h + (h - 1 - y)] = src[y * w + x];
  }
}

#define MERCURY_FRAMES 96
/// The blue flame sheet's 34x65 flame, after its 17x104 shaft; Mercury turns
/// both on their side, and Venus draws the flame as its rising columns.
#define BLUE_FLAME_SHEET_FLAME (17 * 104)

/// Mercury's sheet: its Djinni, then the blue flame sheet's two pictures turned
/// on their side.
enum MercurySheetPicture {
  /// 104x17, the shaft turned; never drawn.
  MERCURY_TURNED_SHAFT = 20 * 34,
  /// 65x34, the flame turned to fly left.
  MERCURY_TURNED_FLAME = MERCURY_TURNED_SHAFT + 104 * 17
};

#define MERCURY_FLAMES 5 ///< flames thrown
#define MERCURY_MOTE_SLOTS 256
// In each summon what bursts throws its motes into its own slots of PARTICLES;
// a mote falls, slows and shrinks as its life runs out.
#define DJINN_ATTACKS_MOTES_PER_FLAME 32 ///< slots for each flame's motes
#define DJINN_ATTACKS_MOTE_SLOTS 512

/// The Mercury Djinni rises from below on the caster's side and whirls there,
/// washed white, firing five blue flames to the left; trailing like comets,
/// each bursts into a spray of motes over the targets and hits them all.
///
/// Abilities: Mercury.
void BattleEffect_RunMercury(struct BattleEffectParams *args) {
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, blit);

  cells = Ram_MapGridCellBuffer;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, cells, 1, 1);
  BattleEffect_TransposeCopy(cells, work->sheet + MERCURY_TURNED_SHAFT, 17,
                             104);
  BattleEffect_TransposeCopy(cells + BLUE_FLAME_SHEET_FLAME,
                             work->sheet + MERCURY_TURNED_FLAME, 34, 65);
  // The canvas fades by a quarter each frame, so each flame trails like a
  // comet.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_FREE_PARTICLES(i, work->particles, 8);
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_WAVE);
  for (frame = 0; frame != MERCURY_FRAMES; frame++) {
    if (frame == 56)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Flames fly left and burst on their 5th frame.
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

    if (frame <= 95) {
      s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
      s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

      blit[1](canvas, work->sheet, x - 10, y - 17, 20, 34);
      for (i = 0; i != MERCURY_FLAMES; i++) {
        if (frame == gDjinnAttacks_LaunchFrames[i]) {
          struct Particle *shot = &work->particles[i];

          if (shot->variant == -1) {
            shot->x = x - 8;
            // DEAD: velocityX keeps the launch x, which nothing reads; the
            // ROM keeps the store.
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
  }
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
/// Abilities: Jupiter.
void BattleEffect_RunJupiter(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  BlitterPair blit;
  struct Particle *ball;
  struct Particle *mote;
  s32 frame;
  s32 i;
  s32 j;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(0, blit);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_FLAMEBALL_SHEET,
                             work->sheet + JUPITER_FLAMEBALL, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (i = 0; i != 32; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_THUNDER);
  frame = 0;
  {
    s32 angle = TRIG_TURN / 2;

    do {
      if (frame == 56)
        EFFECT_SHOW_DAMAGE(SE_IMPACT);

      if (frame <= 95) {
        s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
        s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

        blit[0](canvas, work->sheet, x + 86, y + 28, 20, 40);
      }

      // Flameballs fall from the top right, one every 4 frames.
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

      for (i = 0; i != DJINN_ATTACKS_MOTE_SLOTS; i++) {
        mote = &PARTICLES[i];
        if (mote->variant != -1) {
          s32 size = mote->variant / 16 + 1;

          EFFECT_DRAW_MOTE(blit[1], canvas, sheet, Q_16_16_TO_INT(mote->x),
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
  }
  EFFECT_FINISH_CANVAS();
}

/// Decompresses a resource that may open with a palette.
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

/// Venus's sheet: its Djinni, then the blue flame sheet.
enum VenusSheetPicture {
  VENUS_BLUE_FLAME_SHEET = 20 * 38,
  /// 34x65, the flame, drawn upside down as a rising column.
  VENUS_COLUMN = VENUS_BLUE_FLAME_SHEET + BLUE_FLAME_SHEET_FLAME
};

/// The Venus Djinni drops in from above the targets, swaying and trailing
/// yellow, while ten columns of yellow fire shoot up from the ground beneath
/// them one after another, each bursting into motes and hitting every target.
///
/// Abilities: Venus.
void BattleEffect_RunVenus(struct BattleEffectParams *args) {
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                      COMPOSITE_LIGHTEN);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET,
                             work->sheet + VENUS_BLUE_FLAME_SHEET, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
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
    // The Djinni sways from side to side as it drops.
    if (frame <= 79) {
      s32 x = (Trig_Sin(angle) * 24) >> 16;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      draw[1](canvas, work->sheet, x + 22, y + 29, 20, 38);
    }
    if (frame == 56)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // Ten flame columns rise, one every 4 frames.
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
#define MARS_FLAMES 9 ///< flames along the arc
#define MARS_EMBERS_PER_FLAME 16
#define MARS_GLOW_ORB (20 * 40) ///< six 40x40 pictures, after the 20x40 Djinni

/// A fireball blooms high over the targets and burns out, the Mars Djinni
/// appears where it was and dives onto them, nine flames erupt in a row along
/// the ground beneath them and embers stream up.
///
/// Abilities: Mars.
void BattleEffect_RunMars(struct BattleEffectParams *args) {
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, blitters);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET,
                             work->sheet + MARS_GLOW_ORB, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Nine flames spread out along the ground, alternately left and right.
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
      // DEAD: nothing reads velocityZ; the ROM keeps the store.
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
    // A glowing orb flares, then the Djinni shows in it and dives down to the
    // left.
    if (frame <= 23)
      blitters[0](canvas, work->sheet + MARS_GLOW_ORB + (frame / 4) * (40 * 40),
                  40, 20, 40, 40);
    if (frame == 20)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
                      128);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        blitters[1](canvas, work->sheet, 146 - frame * 4, diveY, 20, 40);
      else
        blitters[0](canvas, work->sheet, 50, 20, 20, 40);
    }
    // It strikes: the screen shakes and the flames erupt.
    if (frame == 32) {
      Sound_PlayFar(SE_CRASH);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, work->sheet, 1,
                                 1);
    }
    if (frame > 31) {
      index = 0;
      step = work->particles;
      do {
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
    // Embers rise from the flames, swaying.
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
        if (step->z > TRIG_ANGLE_MASK)
          step->z -= TRIG_TURN - 1;
      }
      index++;
      step++;
    } while (index != MARS_FLAMES * MARS_EMBERS_PER_FLAME);

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
