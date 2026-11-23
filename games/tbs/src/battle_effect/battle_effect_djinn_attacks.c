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

/// The frame each Djinni's attack shows its damage.
#define DJINN_ATTACK_DAMAGE_FRAME 56

#define MERCURY_FRAMES 96
/// The blue flame sprite sheet's flame, after its shaft; Mercury turns both on
/// their side, and Venus draws the flame as its rising columns.
#define BLUE_FLAME_SHEET_FLAME PICTURE_BYTES(BLUE_FLAME_COLUMN)

/// Mercury's sprite sheet: its Djinni, then the blue flame sprite sheet's two
/// pictures turned on their side.
enum MercurySheetPicture {
  /// The shaft turned; never drawn.
  MERCURY_TURNED_SHAFT = SMALL_DJINNI_WIDTH * MERCURY_SMALL_DJINNI_HEIGHT,
  /// The flame turned to fly left.
  MERCURY_TURNED_FLAME = MERCURY_TURNED_SHAFT +
      BLUE_FLAME_COLUMN_HEIGHT * BLUE_FLAME_COLUMN_WIDTH
};

#define MERCURY_FLAMES 5       ///< flames thrown
#define MERCURY_MOTE_SLOTS 256 ///< slots of PARTICLES the motes may take
/// In each summon what bursts throws its motes into its own slots of PARTICLES;
/// a mote falls, slows and shrinks a size for each
/// DJINN_ATTACKS_MOTE_SHRINK_FRAMES of its life it loses.
#define DJINN_ATTACKS_MOTES_PER_FLAME 32 ///< slots for each flame's motes
#define DJINN_ATTACKS_MOTE_SLOTS 512
#define DJINN_ATTACKS_MOTE_SHRINK_FRAMES 16

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
  BattleEffect_TransposeCopy(cells, work->sheet + MERCURY_TURNED_SHAFT,
                             BLUE_FLAME_COLUMN_WIDTH, BLUE_FLAME_COLUMN_HEIGHT);
  BattleEffect_TransposeCopy(cells + BLUE_FLAME_SHEET_FLAME,
                             work->sheet + MERCURY_TURNED_FLAME,
                             BLUE_FLAME_SPLASH_WIDTH, BLUE_FLAME_SPLASH_HEIGHT);
  // The canvas fades by a quarter each frame, so each flame trails like a
  // comet.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_FREE_PARTICLES(i, work->particles, 8);
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_WAVE);
  for (frame = 0; frame != MERCURY_FRAMES; frame++) {
    if (frame == DJINN_ATTACK_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Flames fly left and burst on their 5th frame.
    for (i = 0; i != MERCURY_FLAMES; i++) {
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
          for (j = 0; j != DJINN_ATTACKS_MOTES_PER_FLAME; j++) {
            s32 angle;
            s32 speed;

            mote = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
            angle = Random16() & TRIG_ANGLE_MASK;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = INTtoFIXED(flame->x);
            mote->y = INTtoFIXED(flame->y);
            mote->velocityX = (Trig_Sin(angle) * speed) >> 8;
            mote->velocityY = (Trig_Cos(angle) * speed) >> 7;
            mote->variant = (Random16() & 15) + 32; // lives 32 to 47 frames
          }
        }
      }
    }

    if (frame <= 95) {
      s32 x =
          (FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) * (64 - frame * 2)) >>
           1) +
          96;
      s32 y =
          FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * (64 - frame * 2)) +
          60;

      blit[1](canvas, work->sheet, x - SMALL_DJINNI_WIDTH / 2,
              y - MERCURY_SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
              MERCURY_SMALL_DJINNI_HEIGHT);
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
        s32 size = mote->variant / DJINN_ATTACKS_MOTE_SHRINK_FRAMES + 2;

        EFFECT_DRAW_MOTE(blit[1], canvas, sheet, FIXEDtoINT(mote->x),
                         FIXEDtoINT(mote->y), size);
        Particle_AdvanceWithGravity2D(mote, 62, toFIXED(1) / 8);
        mote->variant--;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define JUPITER_FRAMES 96
#define JUPITER_FLAMEBALLS 8 ///< flameballs that fall
/// The flameballs given a place to start from, of which only
/// JUPITER_FLAMEBALLS fall.
#define JUPITER_FLAMEBALL_SLOTS 32
#define JUPITER_BURST_Y 95 ///< a flameball bursts once below this line
#define JUPITER_FLAMEBALL PICTURE_BYTES(SMALL_DJINNI) ///< after the Djinni

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
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, blit);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_FLAMEBALL_SHEET,
                             work->sheet + JUPITER_FLAMEBALL, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (i = 0; i != JUPITER_FLAMEBALL_SLOTS; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_THUNDER);
  frame = 0;
  {
    s32 angle = DEGtoANG(180);

    for (; frame != JUPITER_FRAMES; frame++) {
      if (frame == DJINN_ATTACK_DAMAGE_FRAME)
        EFFECT_SHOW_DAMAGE(SE_IMPACT);

      if (frame <= 95) {
        s32 x = FIXEDtoINT(Trig_Sin(angle) * (64 - frame * 2)) >> 1;
        s32 y = FIXEDtoINT(Trig_Cos(angle) * (64 - frame * 2));

        blit[0](canvas, work->sheet, x + 86, y + 28, SMALL_DJINNI_WIDTH,
                SMALL_DJINNI_HEIGHT);
      }

      // Flameballs fall from the top right, one every 4 frames.
      for (i = 0; i != JUPITER_FLAMEBALLS; i++) {
        ball = &work->particles[i];
        if (frame >= i * 4 + 8 && ball->y <= JUPITER_BURST_Y) {
          blit[0](canvas, work->sheet + JUPITER_FLAMEBALL,
                  ball->x - FLAMEBALL_WIDTH / 2, ball->y - FLAMEBALL_HEIGHT / 2,
                  FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
          ball->x -= 6;
          ball->y += 12;
          if (ball->y > JUPITER_BURST_Y) {
            for (j = 0; j != DJINN_ATTACKS_MOTES_PER_FLAME; j++) {
              s32 direction;
              s32 speed;

              mote = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
              direction = Random16() & TRIG_ANGLE_MASK;
              speed = (Random16() & 0x1ff) + 256;
              mote->x = INTtoFIXED(ball->x);
              mote->y = INTtoFIXED(ball->y);
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
          s32 size = mote->variant / DJINN_ATTACKS_MOTE_SHRINK_FRAMES + 1;

          EFFECT_DRAW_MOTE(blit[1], canvas, sheet, FIXEDtoINT(mote->x),
                           FIXEDtoINT(mote->y), size);
          Particle_AdvanceWithGravity2D(mote, 62, toFIXED(1) / 8);
          mote->variant--;
        }
      }
      BattleCamera_Shake(4, 4);
      EFFECT_END_FRAME(work);
      angle -= TRIG_TURN / 32;
    }
  }
  EFFECT_FINISH_CANVAS();
}

/// Decompresses a resource that may open with a palette.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette) {
  u8 *data = Resource_GetTableEntry(id);

  if (copyPalette != 0)
    Iwram_CopyWords(BG_PLTT, data, SHEET_PALETTE_BYTES);
  if (skipPalette != 0)
    data += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(data, dst);
}

#define VENUS_FRAMES 96
#define VENUS_COLUMNS 10 ///< columns of flame that rise
/// The columns given a place to rise from, of which only VENUS_COLUMNS rise.
#define VENUS_COLUMN_SLOTS 32
#define VENUS_MOTES_PER_COLUMN 16
/// How far the Djinni sways to either side as it drops.
#define VENUS_DJINNI_SWAY 24

/// Venus's sprite sheet: its Djinni, then the blue flame sprite sheet.
enum VenusSheetPicture {
  VENUS_BLUE_FLAME_SHEET = SMALL_DJINNI_WIDTH * VENUS_SMALL_DJINNI_HEIGHT,
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
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                      COMPOSITE_LIGHTEN);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET,
                             work->sheet + VENUS_BLUE_FLAME_SHEET, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  for (i = 0; i != VENUS_COLUMN_SLOTS; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
  Sound_PlayFar(SE_RUMBLE);
  frame = 0;
  angle = DEGtoANG(180);
  for (; frame != VENUS_FRAMES; frame++) {
    // The Djinni sways from side to side as it drops.
    if (frame <= 79) {
      s32 x = FIXEDtoINT(Trig_Sin(angle) * VENUS_DJINNI_SWAY);
      s32 y = FIXEDtoINT(Trig_Cos(angle) * (64 - frame * 2));

      draw[1](canvas, work->sheet, x + 22, y + 29, SMALL_DJINNI_WIDTH,
              VENUS_SMALL_DJINNI_HEIGHT);
    }
    if (frame == DJINN_ATTACK_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // Ten flame columns rise, one every 4 frames.
    for (i = 0; i != VENUS_COLUMNS; i++) {
      column = &work->particles[i];
      if (frame >= i * 4 + 16) {
        draw[0](canvas, work->sheet + VENUS_COLUMN,
                column->x - BLUE_FLAME_SPLASH_WIDTH / 2,
                column->y - BLUE_FLAME_SPLASH_HEIGHT / 2,
                BLUE_FLAME_SPLASH_WIDTH, BLUE_FLAME_SPLASH_HEIGHT);
        if (frame == i * 4 + 16) {
          for (j = 0; j != VENUS_MOTES_PER_COLUMN; j++) {
            s32 direction;
            s32 speed;

            mote = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
            direction = (Random16() & 0x7fff) + DEGtoANG(90);
            speed = (Random16() & 0x1ff) + 256;
            mote->x = INTtoFIXED(column->x);
            mote->y = INTtoFIXED(column->y + 16);
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
        s32 size = mote->variant / DJINN_ATTACKS_MOTE_SHRINK_FRAMES + 2;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, FIXEDtoINT(mote->x),
                         FIXEDtoINT(mote->y), size);
        Particle_AdvanceWithGravity2D(mote, 62, toFIXED(1) / 8);
        mote->variant--;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= TRIG_TURN / 32;
  }
  EFFECT_FINISH_CANVAS();
}

#define MARS_FRAMES 112
#define MARS_FLAMES 9 ///< flames along the arc
#define MARS_EMBERS_PER_FLAME 16
/// The row of flames: from the canvas's x MARS_ROW_X out to MARS_ROW_REACH to
/// either side, along line MARS_ROW_Y bowed four pixels.
#define MARS_ROW_X 32
#define MARS_ROW_Y 52
#define MARS_ROW_REACH 24
/// The frames a flame burns: eight to each of its six pictures.
#define MARS_FLAME_LIFE 48
#define MARS_GLOW_ORB                                                          \
  PICTURE_BYTES(SMALL_DJINNI) ///< six pictures, after the Djinni

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
  for (; index != MARS_FLAMES; index++) {
    s32 angle = index * (TRIG_TURN / 32);

    step->x = FIXEDtoINT(Trig_Sin(angle) * MARS_ROW_REACH);
    step->y = FIXEDtoINT(Trig_Cos(angle) * 4) + MARS_ROW_Y;
    if (index & 1)
      step->x = MARS_ROW_X - step->x;
    else
      step->x += MARS_ROW_X;
    step->variant = -(index * 2);
    for (i = 0; i != MARS_EMBERS_PER_FLAME; i++) {
      mote = &PARTICLES[index * MARS_EMBERS_PER_FLAME + i];
      mote->x = INTtoFIXED(((Random16() & 15) + step->x) - 8);
      mote->y = INTtoFIXED((Random16() & 7) + 96);
      mote->velocityX = ((Random16() & 127) - 64) << 11;
      mote->velocityY = ((Random16() & 127) - 64) << 10;
      mote->z = Random16() & TRIG_ANGLE_MASK;
      // DEAD: nothing reads velocityZ; the ROM keeps the store.
      mote->velocityZ = Random16() & 0xffff;
    }
    step++;
  }

  Sound_PlayFar(SE_WHOOSH);
  for (frame = 0; frame != MARS_FRAMES; frame++) {
    if (frame == DJINN_ATTACK_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // A glowing orb flares, then the Djinni shows in it and dives down to the
    // left.
    if (frame <= 23)
      blitters[0](canvas,
                  work->sheet + MARS_GLOW_ORB +
                      (frame / 4) * PICTURE_BYTES(GLOW_ORB),
                  40, 20, GLOW_ORB_WIDTH, GLOW_ORB_HEIGHT);
    if (frame == 20)
      Iwram_CopyWords(BG_PLTT,
                      Resource_GetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
                      SHEET_PALETTE_BYTES);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        blitters[1](canvas, work->sheet, 146 - frame * 4, frame * 8 - 172,
                    SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      else
        blitters[0](canvas, work->sheet, 50, 20, SMALL_DJINNI_WIDTH,
                    SMALL_DJINNI_HEIGHT);
    }
    // It strikes: the screen shakes and the flames erupt.
    if (frame == 32) {
      Sound_PlayFar(SE_CRASH);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, work->sheet, 1,
                                 1);
    }
    if (frame > 31) {
      for (index = 0; index != MARS_FLAMES; index++) {
        step = &work->particles[index];
        if (step->variant >= 0 && step->variant < MARS_FLAME_LIFE) {
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
        step->variant++;
      }
    }
    // Embers rise from the flames, swaying.
    for (index = 0; index != MARS_FLAMES * MARS_EMBERS_PER_FLAME; index++) {
      step = &PARTICLES[index];
      if (frame >= (index / MARS_EMBERS_PER_FLAME) * 2 + 40) {
        s32 size;
        s32 x;

        size = (index & 1) + 3;
        x = FIXEDtoINT(step->x) + FIXEDtoINT(Trig_Sin(step->z) * 4);
        EFFECT_DRAW_MOTE(blitters[1], canvas, sheet, x, FIXEDtoINT(step->y),
                         size);
        Particle_AdvanceWithGravity2D(step, PARTICLE_DAMPING_NONE,
                                      -toFIXED(1) / 8);
        step->z += TRIG_TURN / 32;
        if (step->z > TRIG_ANGLE_MASK)
          step->z -= TRIG_TURN - 1;
      }
    }

    if (frame == 38) {
      EFFECT_FOR_EACH_TARGET(index, work->args) {
        EFFECT_STRIKE_TARGET(work->args, index, 16, KNOCKBACK_BOUNCE);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#undef MERCURY_FRAMES
#undef MERCURY_FLAMES
#undef DJINN_ATTACKS_MOTES_PER_FLAME
#undef DJINN_ATTACKS_MOTE_SLOTS
#undef DJINN_ATTACKS_MOTE_SHRINK_FRAMES
#undef JUPITER_FLAMEBALL_SLOTS
#undef VENUS_COLUMN_SLOTS
#undef VENUS_MOTES_PER_COLUMN
#undef VENUS_DJINNI_SWAY
#undef MARS_ROW_X
#undef MARS_ROW_Y
#undef MARS_ROW_REACH
#undef MARS_FLAME_LIFE
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
