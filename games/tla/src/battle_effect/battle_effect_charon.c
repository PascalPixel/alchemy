/// @file
/// Charon, and the routine that builds the quad strips other effects map their
/// textures on.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The attack's particles in the effect work: the drops, each target's curse
/// (where on the screen it caught the target, and the frames since, -1
/// before) and its home, then the four columns of light; the last particle's
/// variant holds the shockwave's radius.
#define DROPS (work->particles)
#define CURSES (work->particles + 16)
#define HOMES (work->particles + 32)
#define COLUMNS (work->particles + 48)
#define WAVE_RADIUS (work->particles[63].variant)
/// The drops' particles, how many of them fall, and their size: shaded
/// circles of this radius, in the act's pictures.
#define CHARON_DROP_SLOTS 16
#define CHARON_DROPS 12
#define CHARON_DROP_RADIUS 16
/// The columns of light: each this tall and the next this much lower, its left
/// end rounded by a shaded quarter circle of size CHARON_COLUMN_END.
#define CHARON_COLUMNS 4
#define CHARON_COLUMN_HEIGHT 30
#define CHARON_COLUMN_END 56
/// The summon's sprites in the effect work: the red dome's until frame 132,
/// then Charon's.
#define OBJECTS ((struct Sprite **)work->sprites)
/// Charon (CrimsonBeastTiles) as a grid of sprites four across and three down,
/// its wisp (WhiteWispsTiles, in a sprite palette of its own) and the 64x64
/// sprite the red orb is revealed in.
#define CHARON_BEAST_SPRITES 12
#define CHARON_BEAST_COLUMNS 4
#define CHARON_BEAST_SPRITE_SIZE 32
#define CHARON_WISP 12
#define CHARON_REVEAL 13
#define CHARON_OBJECTS 14
#define CHARON_WISP_PALETTE 14
/// The dark shards (TumblingShardTiles, in a sprite palette of their own): the
/// pictures of an 8x8 shard tumbling, and the sprites that swarm with them. A
/// shard's tumble counts CHARON_SHARD_TURN to a turn of its pictures.
#define CHARON_SHARD_FRAMES 16
#define CHARON_SHARDS 48
#define CHARON_SHARD_PALETTE 15
#define CHARON_SHARD_TURN 256

/// The map cell buffer: the shards' sprites from its start, where the attack
/// later decodes FlashBurstSheet (SPRITE_BUFFER). At 16384, a list of
/// CHARON_HIDDEN_SPRITES sprites built at frame 0, which the summon never
/// reads; from frame 243 the red orb's 64x64 picture (RedOrbTiles) over it
/// (ORB_BUFFER, read back as REVEAL_BUFFER) and the order it is revealed
/// in after it. Then the particles.
#define SPRITES ((struct Sprite **)Ram_MapCellBuffer)
#define SPRITE_BUFFER gMapGridCellBuffer
#define OAM_BUFFER ((u32 *)(gMapGridCellBuffer + 16384))
#define CHARON_HIDDEN_SPRITES 64
#define ORB_BUFFER (gMapGridCellBuffer + 16384)
#define REVEAL_BUFFER (Ram_MapCellBuffer + 16384)
#define CHARON_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 16896))
/// The swarm: CHARON_SWARM particles fly up from frame 132, the shards ride
/// those from CHARON_SWARM_SHARDS on, the first CHARON_LARGE_SHARDS a quarter
/// larger, and each CHARON_SWARM_GROUP particles tumble a step faster than the
/// group before.
#define CHARON_SWARM 128
#define CHARON_SWARM_SHARDS (CHARON_PARTICLES + 64)
#define CHARON_LARGE_SHARDS 10
#define CHARON_SWARM_GROUP 16
/// The attack's particles: the motes of each target's curse, the flash bursts
/// (FlashBurstSheet's six pictures, four frames each) and the sparks among
/// them.
#define CHARON_CURSE_MOTES 16 ///< for each target
#define CHARON_MOTE_COUNT (ENEMY_BATTLE_SIZE * CHARON_CURSE_MOTES)
#define CHARON_BURSTS (CHARON_PARTICLES + CHARON_MOTE_COUNT)
#define CHARON_BURST_COUNT 32
#define CHARON_BURSTS_DRAWN 16
#define CHARON_BURST_FRAMES 24
#define CHARON_SPARKS (CHARON_BURSTS + CHARON_BURST_COUNT)
#define CHARON_SPARK_COUNT 128
/// The colours of the 256-colour sprites: the red dome's (RedDomeTiles) from
/// CHARON_DOME_FIRST_COLOR, Charon's from the first, and the red orb's
/// (RedOrbTiles) in the two palettes from CHARON_ORB_PALETTE.
#define CHARON_DOME_FIRST_COLOR 32
#define CHARON_DOME_COLORS 160
#define CHARON_BEAST_COLORS 192
#define CHARON_ORB_PALETTE 12
#define CHARON_ORB_FIRST_COLOR (CHARON_ORB_PALETTE * PALETTE_COLORS)
#define CHARON_ORB_COLORS 32
/// The red orb is revealed in its sprite two pixels at a time, the same
/// pair in each of its tiles, in a shuffled order of the tile's pairs.
#define CHARON_REVEAL_TILES (OBJ_64X64_8BPP_BYTES / TILE_SIZE_8BPP)
#define CHARON_DISSOLVE_PAIRS (TILE_SIZE_8BPP / 2)
/// The battle background's tiles from column 4 of row 5, where the rune seal
/// is stamped.
#define CHARON_SEAL_TILES (BG_CHAR_BLOCK(2) + (5 * 32 + 4) * 64)
/// The rune seal's colours take a background palette of their own: a seal
/// pixel of colour c is stamped as the byte c + CHARON_SEAL_COLOR, which wraps
/// to colour c of that palette.
#define CHARON_SEAL_PALETTE 14
#define CHARON_SEAL_COLOR                                                      \
  (CHARON_SEAL_PALETTE * PALETTE_COLORS - BG_PLTT_COLORS)

extern const struct Scale gCharon_SwarmScale;
extern const struct Scale gCharon_PulseScale;
extern const struct Scale gCharon_RevealScale;
extern const struct Scale gCharon_FinaleScale;

/// Where the pictures that load once are decoded in the effect's sheet: the
/// rune seal as it is stamped (RuneSealSheet, 184 by 80), then at frame 132
/// Charon's, its wisp's and ParticleSpritesD's in turn.
#define CHARON_BEAST_PICTURES 9216
#define CHARON_RUNE_SEAL 9216
#define RUNE_SEAL_WIDTH 184
#define RUNE_SEAL_HEIGHT 80
/// Where each act decodes its own sheet in the effect's pictures: SwirlTexture
/// with CharmRingSheet, CurseSealSheet, FireRingTexture or shaded circles.
#define CHARON_ACT_PICTURES 4096
/// The finale's brightest block, which the four columns of light are drawn
/// from, and GlowRingTexture made all bright.
#define CHARON_BRIGHT_BLOCK 8192
#define CHARON_GLOW_RING 12288
/// The bytes of either, a 64x64 texture.
#define CHARON_TEXTURE_BYTES (64 * 64)

/// The ring of motes the seal sends out: CHARON_RING_MOTES motes of mote
/// picture CHARON_RING_MOTE (4 pixels wide, 8 tall), drawn from
/// CHARON_RING_SHADES copies in the battle sheet, each 7 levels darker than
/// the last.
#define CHARON_RING_MOTES 256
#define CHARON_RING_MOTE 4
#define CHARON_RING_SHADES 10
#define CHARON_RING_MOTE_BYTES (CHARON_RING_MOTE * CHARON_RING_MOTE * 2)

/// The opening's swirl: it shrinks from CHARON_SWIRL_SCALE by
/// CHARON_SCALE_STEP a frame until the seal is stamped, then grows again from
/// half its size. A second swirl, mirrored, turns the other way inside it, and
/// from the seal on the charm ring turns with a smaller mirrored one inside it.
/// The swirls are drawn only while brighter than CHARON_SWIRL_SHADE_MIN.
#define CHARON_SWIRL_SCALE 115536
#define CHARON_SCALE_STEP 1524
#define CHARON_INNER_SWIRL_SCALE 42768
#define CHARON_CHARM_RING_SCALE 22768
#define CHARON_INNER_RING_SCALE 12768
#define CHARON_SWIRL_SHADE_MIN (-48)
/// The curse seal falls at frame 200 and the swirl under it at 232, both from
/// CHARON_FALL_SCALE: the seal shrinks CHARON_SCALE_STEP a frame down to
/// CHARON_SEAL_SCALE_MIN, the swirl CHARON_SWIRL_SHRINK.
#define CHARON_FALL_SCALE 100000
#define CHARON_SEAL_SCALE_MIN 13800
#define CHARON_SWIRL_SHRINK 1200
/// The glow ring that rises at each cursed target grows this much a step.
#define CHARON_GLOW_RING_GROWTH 1016

#define CHARON_FRAMES 394 ///< frames of Charon's coming, unless A or B skips it
#define CHARON_ATTACK_FRAMES 180 ///< frames of the attack that follows
#define CHARON_FLASHES 3 ///< the backdrop's flashes, at gCharon_FlashFrames
/// The frame the rune seal is stamped on the ground, after the canvas flashes.
#define CHARON_SEAL_FRAME 56
/// The rune seal glows this many levels brighter than its own colours, pulsing
/// by CHARON_SEAL_GLOW_PULSE either way.
#define CHARON_SEAL_GLOW 16
#define CHARON_SEAL_GLOW_PULSE 12
/// The red dusk the backdrop fades toward: its own colours this many levels
/// darker in green and blue and CHARON_DUSK_REDDENING redder, and after the
/// flash that brings the rings of fire, CHARON_DEEP_DUSK_DIMMING darker.
#define CHARON_DUSK_DIMMING 16
#define CHARON_DEEP_DUSK_DIMMING 20
#define CHARON_DUSK_REDDENING 8
/// The deep red the wisp darkens to from white.
#define CHARON_WISP_RED_MIN 12
#define CHARON_WISP_GREEN_MIN 4
#define CHARON_WISP_BLUE_MIN 4
/// The dark disc rimmed in magenta swells at this x on the canvas, and from
/// CHARON_DISC_SWAY_FRAME sways as far either side of it.
#define CHARON_DISC_X 54
#define CHARON_DISC_SWAY_FRAME 350
/// The top of the attack's ramp, a pink violet: full red with these levels of
/// green and blue.
#define CHARON_RAMP_GREEN 8
#define CHARON_RAMP_BLUE 27
/// Each flash as the targets are drawn in fades the background back to this
/// many levels darker in green, and half that in red and blue.
#define CHARON_FLASH_DIMMING 20

/// Charon: over a red wasteland a violet whirlpool turns about a red dome that
/// rises out of it; in a white flash among dark shards Charon appears and
/// slashes across the vortex, and a black shadow sweeps from the left over the
/// foes.
///
/// Abilities: Charon.
void BattleEffect_RunCharon(struct BattleEffectParams *arg) {
  u16 tiles[CHARON_SHARD_FRAMES];
  s16 emptyList[UNIT_MAX];
  FIXED swarmPlace[4];
  FIXED pulsePlace[4];
  FIXED objectPlace[4];
  FIXED revealPlace[4];
  FIXED finalePlace[4];
  DrawRectangleFn blitters[2];
  struct ModelTexture sealTexture;
  struct ModelTexture fireRingTexture;
  struct ModelTexture swirlTexture;
  struct ModelTexture glowRingTexture;
  u8 *canvas;
  struct BattleEffectWork *work;
  struct BattleBackgroundWork *backgroundView;
  s32 j;
  s32 frame;
  u32 savedScrollX;
  u8 *motePictures;
  struct BattleWork *battle;
  DrawRectangleFn *blit;
  FIXED beastX;
  FIXED beastZ;
  s32 sealFrame;
  s32 red;
  s32 green;
  s32 blue;
  struct BattleCamera *camera;
  s32 shake;
  s16 savedYaw;
  s32 i;
  s32 tinted;
  u16 *palette;
  u8 *source;
  s32 darkenFrame;
  struct Sprite **sprites;
  struct VramCacheEntry *slots;
  struct VramCacheEntry *objectSlots;
  s32 moteSize;
  s32 phase;
  s32 halfWidth;
  s32 dimming;
  s32 width;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  backgroundView = Ram_HeapSlots->battleBackground;
  savedScrollX = gBgScroll[1].x;
  motePictures = Ram_HeapSlots->battleSheet;
  sprites = (struct Sprite **)gMapGridCellBuffer;
  battle = Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  {
    s32 order = TASK_PASS_VBLANK;

    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, order);
  }
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  backgroundView->busy = 1;
  gBgScroll[1].x = 0;
  emptyList[0] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(emptyList, 0);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  wait(1);
  Palette_LoadBasicSpriteColors();
  battle->keepBackdropPalette = 1;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  Palette_BrightenSavedBattleBg(0, -20, -20);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  blit = blitters;
  BattleEffect_FetchRectangleBlitters(0, blit);

  source = (u8 *)Resource_GetTableEntry(RESOURCE_RED_DOME_TILES);
  Iwram_CopyWords((u16 *)&OBJ_PLTT_COLOR(2, 0), source, CHARON_DOME_COLORS * 2);
  source += CHARON_DOME_COLORS * 2;
  Decompress_TaggedLz(source, work->sheet);
  for (i = 0; i != OBJ_64X64_8BPP_BYTES; i++) {
    if (work->sheet[i] != 0) {
      work->sheet[i] += CHARON_DOME_FIRST_COLOR;
    }
  }
  {
    struct Sprite *object = EffectSprite_CreateWithTiles(
        64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);

    OBJECTS[0] = object;
    object->image.priority = 3;
    slots = gVramCacheEntries;
    Iwram_CopyWords(OBJ_VRAM + slots[object->slot].offset, work->sheet,
                    OBJ_64X64_8BPP_BYTES);
  }
  OBJECTS[0]->image.objectMode = 0;
  OBJECTS[0]->offsetY = 32;

  source = (u8 *)Resource_GetTableEntry(RESOURCE_TUMBLING_SHARD_TILES);
  Iwram_CopyWords((u16 *)&OBJ_PLTT_COLOR(CHARON_SHARD_PALETTE, 0), source,
                  PALETTE_BANK_BYTES);
  source += PALETTE_BANK_BYTES;
  Decompress_TaggedLz(source, work->sheet);
  for (i = 0; i != CHARON_SHARD_FRAMES; i++) {
    struct Sprite *object = EffectSprite_CreateWithTiles(
        16, 16, 0, OBJ_PALETTE(CHARON_SHARD_PALETTE));
    SPRITES[i] = object;
    object->image.priority = 1;
    Iwram_CopyWords(OBJ_VRAM + slots[object->slot].offset,
                    work->sheet + i * TILE_SIZE_4BPP, TILE_SIZE_4BPP);
    tiles[i] = SPRITES[i]->image.tile;
  }
  for (i = 0; i != CHARON_SHARDS - CHARON_SHARD_FRAMES; i++) {
    struct Sprite *object =
        EffectSprite_Acquire(8, 8, 0, OBJ_PALETTE(CHARON_SHARD_PALETTE));
    SPRITES[CHARON_SHARD_FRAMES + i] = object;
    Iwram_CopyWords(object, sprites[0], offsetof(struct Sprite, lastNo));
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motePictures, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, work->sheet, 0, 0);
  for (j = 0; j != CHARON_RING_SHADES; j++) {
    moteSize = CHARON_RING_MOTE;
    for (i = 0; i != moteSize * moteSize * 2; i++) {
      s32 value = work->sheet[gBattleEffect_MotePictures[moteSize - 1] + i];

      if (value != 0) {
        value -= j * 7;
        if (value <= 0) {
          value = 1;
        }
      }
      motePictures[moteSize * moteSize * j * 2 + i] = value;
    }
  }
  Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_CHARM_RING_SHEET,
                             work->sheet + CHARON_ACT_PICTURES, 1, 1);
  source = (u8 *)Resource_GetTableEntry(RESOURCE_BOLT_SHEET);
  Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  REG_BG2X = Q_8_8(8);
  beastX = 0;
  beastZ = 0;

  for (frame = 0; frame != CHARON_FRAMES && !(gInput.repeat & (KEY_A | KEY_B));
       frame++) {
    u8 *order = REVEAL_BUFFER + (frame - 244) * 2;

    // From frame 32 the backdrop's red drains away and its blue comes back.
    darkenFrame = frame - 32;
    if (darkenFrame >= 0 && darkenFrame < 22) {
      Palette_BrightenSavedBattleBg(-darkenFrame, -20, frame - 52);
    }
    if (frame == 55) {
      Palette_BrightenSavedBattleBg(0, -10, -20);
    }
    if (frame == 4) {
      Sound_PlayFar(SE_TREMOR);
    }
    if (frame == 55) {
      Sound_PlayFar(SE_BANE_ESCAPE);
    }
    if (frame == 78) {
      Sound_PlayFar(SE_GUSH);
    }
    if (frame == 127) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 131) {
      Sound_PlayFar(SE_MONSTER_ROAR);
    }
    if (frame == 206) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 244) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 302) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 308) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 312) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 350) {
      Sound_PlayFar(SE_LAUNCH);
    }
    if (frame == 0) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      // A list of sprites off the screen's corner, each showing the shard
      // picture of a particle's tumble; the particles hold nothing yet.
      {
        u32 hidden;
        /* FAKEMATCH(temporary): the mask held in a halfword struct set in the
         * for clause. */
        struct {
          u16 value;
        } mask;
        /* FAKEMATCH(temporary): tumble, a pointer to each particle's variant
         * stepped a particle at a time; as a particle pointer walked. */
        s32 *tumble;
        u32 *b;
        u32 *a;

        for (i = 0, hidden = OBJ_X(DISPLAY_WIDTH) + DISPLAY_HEIGHT,
            mask.value = OBJ_PALETTE(CHARON_SHARD_PALETTE),
            tumble = &CHARON_PARTICLES[0].variant,
            b = (u32 *)(REVEAL_BUFFER + 4), a = OAM_BUFFER;
             i != CHARON_HIDDEN_SPRITES; i++) {
          s32 t;

          *a = hidden;
          t = *tumble;
          tumble += sizeof(struct Particle) / sizeof(*tumble);
          *b =
              tiles[t / (CHARON_SHARD_TURN / CHARON_SHARD_FRAMES)] | mask.value;
          b += 2;
          a += 2;
        }
      }
    }
    if (frame >= 132 && frame < 240) {
      struct Scale swarmScale = gCharon_SwarmScale;

      swarmScale.y = FIXED_ONE * 5 / 4;
      swarmScale.x = FIXED_ONE * 5 / 4;
      if (frame == 132) {
        for (i = 0; i != CHARON_SWARM; i++) {
          CHARON_PARTICLES[i].x = INTtoFIXED(Random16() % 160 + 40);
          CHARON_PARTICLES[i].y = INTtoFIXED((Random16() & 63) + 32);
          CHARON_PARTICLES[i].variant = Random16() & (CHARON_SHARD_TURN - 1);
          CHARON_PARTICLES[i].velocityX = ((Random16() & 255) - 127) << 9;
          CHARON_PARTICLES[i].velocityY = (-(Random16() & 255) - 128) << 8;
        }
      }
      for (i = 0; i != CHARON_SHARDS; i++) {
        if (i == CHARON_LARGE_SHARDS) {
          swarmScale.y = FIXED_ONE;
          swarmScale.x = FIXED_ONE;
        }
        swarmPlace[3] = 0;
        swarmPlace[1] = toFIXED(255);
        swarmPlace[0] = CHARON_SWARM_SHARDS[i].x;
        swarmPlace[2] = CHARON_SWARM_SHARDS[i].y + toFIXED(32);
        Render_ApplyProjectedPlacementFar(SPRITES[i], swarmPlace, &swarmScale.x,
                                          0);
        SPRITES[i]->image.tile =
            tiles[CHARON_SWARM_SHARDS[i].variant /
                  (CHARON_SHARD_TURN / CHARON_SHARD_FRAMES)];
      }
      for (i = 0; i != CHARON_SWARM; i++) {
        if (CHARON_PARTICLES[i].y > -toFIXED(16) &&
            CHARON_PARTICLES[i].x > -toFIXED(16) &&
            CHARON_PARTICLES[i].x < toFIXED(256)) {
          CHARON_PARTICLES[i].x += CHARON_PARTICLES[i].velocityX;
          CHARON_PARTICLES[i].y += CHARON_PARTICLES[i].velocityY;
        }
        CHARON_PARTICLES[i].variant += i / CHARON_SWARM_GROUP + 2;
        if (CHARON_PARTICLES[i].variant >= CHARON_SHARD_TURN) {
          CHARON_PARTICLES[i].variant -= CHARON_SHARD_TURN;
        }
      }
    }
    sealFrame = frame - CHARON_SEAL_FRAME;
    if (sealFrame >= 0 && sealFrame < 76) {
      struct Scale pulseScale = gCharon_PulseScale;

      pulseScale.x = pulseScale.y =
          Trig_Sin(sealFrame << 7) * 2 + FIXED_ONE / 4;
      pulsePlace[3] = 0;
      pulsePlace[1] = toFIXED(255);
      pulsePlace[0] = toFIXED(114);
      // DEAD: sealFrame is never negative here; the ROM keeps the test and its
      // other height.
      pulsePlace[2] = sealFrame >= 0 ? toFIXED(78) : toFIXED(100);
      Render_ApplyProjectedPlacementFar(OBJECTS[0], pulsePlace, &pulseScale.x,
                                        0);
    }
    if (frame == 131) {
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_AERIAL_VIEW_BACKDROP, 0);
      Iwram_FillWords((u16 *)&BG_PLTT_COLOR(6, 0), BATTLE_BACKGROUND_COLORS * 2,
                      COLOR_PAIR(RGB_WHITE));
      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame >= 132 && frame < 147) {
      Iwram_FillWords(
          canvas, CANVAS_VIEW_BYTES,
          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST - (frame - 132) * 4));
    }
    // Every fourth frame the backdrop fades a step from the flash's white to
    // its own colours, and from frame 168 on toward a red dusk; the wisp
    // darkens from white to a deep red.
    if ((frame & 3) == 0 && (frame >= 132 && frame < 240)) {
      u16 *target = Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
      u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, target++, color++) {
        s32 b = RGB_BLUE(*color);
        s32 g = RGB_GREEN(*color);
        s32 r = RGB_RED(*color);
        s32 tb;
        s32 tg;
        s32 tr;

        if (frame < 168) {
          tb = RGB_BLUE(*target);
          tg = RGB_GREEN(*target);
          tr = RGB_RED(*target);
        } else {
          tb = RGB_BLUE(*target) - CHARON_DUSK_DIMMING;
          tg = RGB_GREEN(*target) - CHARON_DUSK_DIMMING;
          tr = RGB_RED(*target) + CHARON_DUSK_REDDENING;
        }
        if (tb < 0) {
          tb = 0;
        }
        if (tg < 0) {
          tg = 0;
        }
        if (tr > RGB_LEVEL_MAX) {
          tr = RGB_LEVEL_MAX;
        }
        if (b > tb) {
          b--;
        }
        if (g > tg) {
          g--;
        }
        if (r > tr) {
          r--;
        }
        *color = RGB(r, g, b);
      }
      for (i = 0; i != PALETTE_COLORS - 1; i++) {
        s32 r = RGB_LEVEL_MAX - (frame - 132) / 4;
        s32 g = r;
        s32 b = r;

        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        if (r < CHARON_WISP_RED_MIN) {
          r = CHARON_WISP_RED_MIN;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        }
        if (g < CHARON_WISP_GREEN_MIN) {
          g = CHARON_WISP_GREEN_MIN;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
        if (b < CHARON_WISP_BLUE_MIN) {
          b = CHARON_WISP_BLUE_MIN;
        }
        OBJ_PLTT_COLOR(CHARON_WISP_PALETTE, 1 + i) = RGB(r, g, b);
      }
    }
    if (frame >= 132 && frame < 240) {
      u16 *target = Resource_GetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);
      u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 0);

      for (i = 0; i != CHARON_BEAST_COLORS; i++, target++, color++) {
        s32 b = RGB_BLUE(*color);
        s32 g = RGB_GREEN(*color);
        s32 r = RGB_RED(*color);
        s32 tb = RGB_BLUE(*target);
        s32 tg = RGB_GREEN(*target);
        s32 tr = RGB_RED(*target);

        if (b > tb) {
          b--;
        }
        if (g > tg) {
          g--;
        }
        if (r > tr) {
          r--;
        }
        *color = RGB(r, g, b);
      }
    }
    // Charon's own colours are tinted toward violet from frame 214, back from
    // 251, and toward red from 348.
    tinted = 0;
    if (frame >= 214 && frame < 246) {
      s32 t = (frame - 214) / 4;

      red = t / 2;
      green = 0;
      blue = t;
      tinted = 1;
    }
    if (frame >= 251 && frame < 283) {
      s32 t = (frame - 251) / 2;

      red = 4 - t / 4;
      green = -(t / 2);
      blue = 8 - t / 2;
      tinted = 1;
    }
    if (frame >= 348 && frame < 412) {
      red = (frame - 348) / 4;
      green = red / 2 - 8;
      blue = red / 8 - 4;
      tinted = 1;
    }
    if (tinted == 1) {
      u16 *color = Resource_GetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);

      for (i = 0; i != CHARON_BEAST_COLORS; i++, color++) {
        s32 r = RGB_RED(*color) + red;
        s32 g = RGB_GREEN(*color) + green;
        s32 b = RGB_BLUE(*color) + blue;

        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
        if (r < 0) {
          r = 0;
        }
        if (g < 0) {
          g = 0;
        }
        if (b < 0) {
          b = 0;
        }
        OBJ_PLTT_COLOR(0, i) = RGB(r, g, b);
      }
    }
    if (frame >= 132 && frame < 860) {
      if (frame == 132) {
        Sprite_DestroyFar((struct Sprite *)OBJECTS[0]);
        source = (u8 *)Resource_GetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(0, 0), CHARON_BEAST_COLORS * 2,
                        COLOR_PAIR(RGB_WHITE));
        source += CHARON_BEAST_COLORS * 2;
        Decompress_TaggedLz(source, work->sheet + CHARON_BEAST_PICTURES);
        for (i = 0, objectSlots = gVramCacheEntries; i != CHARON_BEAST_SPRITES;
             i++) {
          struct Sprite *object = EffectSprite_CreateWithTiles(
              CHARON_BEAST_SPRITE_SIZE, CHARON_BEAST_SPRITE_SIZE,
              OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

          object->image.priority = 3;
          OBJECTS[i] = object;
          Iwram_CopyWords(OBJ_VRAM + objectSlots[object->slot].offset,
                          work->sheet + CHARON_BEAST_PICTURES +
                              i * OBJ_32X32_8BPP_BYTES,
                          OBJ_32X32_8BPP_BYTES);
        }
        source = (u8 *)Resource_GetTableEntry(RESOURCE_WHITE_WISPS_TILES);
        Iwram_CopyWords((u16 *)&OBJ_PLTT_COLOR(CHARON_WISP_PALETTE, 0), source,
                        PALETTE_BANK_BYTES);
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(CHARON_WISP_PALETTE, 0),
                        PALETTE_BANK_BYTES, COLOR_PAIR(RGB_WHITE));
        source += PALETTE_BANK_BYTES;
        Decompress_TaggedLz(source, work->sheet + CHARON_BEAST_PICTURES);
        {
          struct Sprite *object = EffectSprite_CreateWithTiles(
              64, 64, OBJ_SIZE_64X64, OBJ_PALETTE(CHARON_WISP_PALETTE));
          struct VramCacheEntry *tileSlots;

          OBJECTS[CHARON_WISP] = object;
          object->image.priority = 3;
          tileSlots = gVramCacheEntries;
          {
            u8 *vram = OBJ_VRAM;

            vram += tileSlots[object->slot].offset;
            Iwram_CopyWords(vram, work->sheet + CHARON_BEAST_PICTURES,
                            OBJ_64X64_4BPP_BYTES);
          }
          object = EffectSprite_CreateWithTiles(
              64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);
          OBJECTS[CHARON_REVEAL] = object;
          object->image.priority = 2;
          object->offsetY = 32;
          Iwram_FillWords(OBJ_VRAM + tileSlots[object->slot].offset,
                          OBJ_64X64_8BPP_BYTES, 0);
        }
        source = (u8 *)Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_E);
        Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
        Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D,
                                   work->sheet + CHARON_BEAST_PICTURES, 0, 0);
        Iwram_CopyWords(motePictures, work->sheet + CHARON_BEAST_PICTURES,
                        MOTE_SET_BYTES);
        Resource_LoadAndDecompress(RESOURCE_CURSE_SEAL_SHEET,
                                   work->sheet + CHARON_ACT_PICTURES, 1, 1);
        beastX = toFIXED(72);
        beastZ = toFIXED(56);
      }
      if (frame == 243) {
        source = (u8 *)Resource_GetTableEntry(RESOURCE_RED_ORB_TILES);
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0),
                        CHARON_ORB_COLORS * 2, 0);
        source += CHARON_ORB_COLORS * 2;
        Decompress_TaggedLz(source, ORB_BUFFER);
        for (i = 0; i != OBJ_64X64_8BPP_BYTES; i++) {
          if (ORB_BUFFER[i] != 0) {
            ORB_BUFFER[i] += CHARON_ORB_FIRST_COLOR;
          }
        }
        // The order the pixel pairs are revealed in, after the picture: each
        // pair is swapped with one picked at random.
        for (i = 0; i != CHARON_DISSOLVE_PAIRS; i++) {
          REVEAL_BUFFER[OBJ_64X64_8BPP_BYTES + i] = i;
        }
        {
          u8 *slot;
          s32 orderOffset = OBJ_64X64_8BPP_BYTES;

          for (i = 0, slot = REVEAL_BUFFER + OBJ_64X64_8BPP_BYTES;
               i != CHARON_DISSOLVE_PAIRS;) {
            u8 *other;
            s32 swap;
            s32 value;
            s32 mask;

            mask = CHARON_DISSOLVE_PAIRS - 1;
            orderOffset = OBJ_64X64_8BPP_BYTES;
            other = (Random16() & mask) + ORB_BUFFER;
            other += orderOffset;
            swap = *slot;
            value = *other;
            /* FAKEMATCH(dead): swap and value cast to the bytes they already
             * are. */
            swap = (u8)swap;
            value = (u8)value;
            i++;
            *slot = value;
            *other = swap;
            slot++;
          }
        }
      }
      {
        FIXED objectScale[2];

        objectPlace[3] = 0;
        objectPlace[1] = toFIXED(255);
        if (frame < 160) {
          beastX += toFIXED(1) / 8;
          beastZ -= toFIXED(1) / 4;
        } else if (frame < 332) {
          beastX += toFIXED(1) / 8;
          beastZ += toFIXED(1) / 16;
        } else {
          beastX += toFIXED(1) / 8;
        }
        objectScale[1] = FIXED_ONE * 5 / 4;
        objectScale[0] = FIXED_ONE * 5 / 4;
        objectScale[0] = objectScale[1] = beastZ / 48 + FIXED_ONE / 4;
        objectPlace[0] = beastX + toFIXED(64);
        objectPlace[2] = toFIXED(144);
        Render_ApplyProjectedPlacementFar(OBJECTS[CHARON_WISP], objectPlace,
                                          objectScale, 0);
        objectScale[1] = FIXED_ONE;
        objectScale[0] = FIXED_ONE;
        for (i = 0; i != CHARON_BEAST_SPRITES; i++) {
          objectPlace[0] = INTtoFIXED((i % CHARON_BEAST_COLUMNS) *
                                      CHARON_BEAST_SPRITE_SIZE) +
                           beastX;
          objectPlace[2] = INTtoFIXED((i / CHARON_BEAST_COLUMNS) *
                                      CHARON_BEAST_SPRITE_SIZE) +
                           beastZ;
          Render_ApplyProjectedPlacementFar(OBJECTS[i], objectPlace,
                                            objectScale, 0);
        }
      }
      if (frame == 200) {
        source = (u8 *)Resource_GetTableEntry(RESOURCE_BOLT_SHEET);
        Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
      }
      // The curse seal falls turning onto the ground and fades from frame
      // 254; from frame 232 the swirl turns under it.
      if (frame >= 200 && frame < 302) {
        void *texture = Heap_AllocEwram(32);
        struct RenderModel *model = Render_AllocateModelList(1);
        s32 t = frame - 200;
        s32 shade;
        s32 scale;

        shade = t * 4 + MODEL_SHADE_INVISIBLE;
        scale = CHARON_FALL_SCALE - t * CHARON_SCALE_STEP;
        if (frame > 253) {
          shade = (254 - frame) * 8;
        }
        if (scale < CHARON_SEAL_SCALE_MIN) {
          scale = CHARON_SEAL_SCALE_MIN;
        }
        if (shade > 0) {
          shade = 0;
        }
        model->shade = shade;
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                            CANVAS_FADE_THREE_QUARTERS);
        sealTexture.logWidth = 7;
        sealTexture.logHeight = 7;
        sealTexture.pixels = work->sheet + CHARON_ACT_PICTURES;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &sealTexture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = texture;
        if (frame <= 282) {
          Matrix_Initialize();
          Matrix_Scale(scale * 2, scale * 2, scale * 2);
          Matrix_Translate(-toFIXED(16), -toFIXED(8), 0);
          Matrix_RotateX(TRIG_TURN * 7 / 8);
          Matrix_RotateZ(DEGtoANG(90) - t * (TRIG_TURN / 256));
          Matrix_RotateY(-t * (TRIG_TURN / 64));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                   4);
          Render_DrawModelList(model);
        }
        if (frame > 231) {
          t = frame - 232;
          shade = t * 2 + MODEL_SHADE_INVISIBLE;
          scale = CHARON_FALL_SCALE - t * CHARON_SWIRL_SHRINK;
          if (shade < MODEL_SHADE_INVISIBLE) {
            shade = MODEL_SHADE_INVISIBLE;
          }
          if (shade > 0) {
            shade = 0;
          }
          sealTexture.logWidth = 6;
          sealTexture.logHeight = 6;
          model->shade = shade;
          sealTexture.pixels = work->sheet;
          model->texture = &sealTexture;
          model->mesh = gBattleEffect_SquareMesh;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->vertices = texture;
          Matrix_Initialize();
          Matrix_Translate(-toFIXED(8), -toFIXED(8), 0);
          Matrix_Scale(scale, scale, scale);
          Matrix_RotateX(TRIG_TURN * 3 / 4);
          Matrix_RotateY(-t * (TRIG_TURN / 64));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                   4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(texture);
      }
      // From frame 244 the red orb is revealed in its sprite, two pixel
      // pairs of every tile a frame, as its colours rise from black, and it
      // shrinks away to nothing at frame 310.
      if (frame >= 244 && frame < 310) {
        struct Scale revealScale = gCharon_RevealScale;

        if (frame < 260) {
          u8 *low = REVEAL_BUFFER;
          u8 *high = REVEAL_BUFFER + 1;
          /* FAKEMATCH(temporary): orderCopy, a copy of order set again in the
           * loop. */
          u8 *orderCopy = order;

          for (i = 0; i != CHARON_REVEAL_TILES; i++) {
            s32 pair = (orderCopy = order)[OBJ_64X64_8BPP_BYTES] * 2;
            s32 tile = i * TILE_SIZE_8BPP;
            s32 pixel = tile + pair;
            u8 *lowByte = low + pixel;
            u8 *highByte = high + pixel;
            s32 texel = *lowByte | (*highByte << 8);

            *(u16 *)(OBJ_VRAM +
                     (tile +
                      gVramCacheEntries[OBJECTS[CHARON_REVEAL]->slot].offset) +
                     pair) = texel;
            pair = orderCopy[OBJ_64X64_8BPP_BYTES + 1] * 2;
            pixel = tile + pair;
            lowByte = low + pixel;
            highByte = high + pixel;
            texel = *lowByte | (*highByte << 8);
            *(u16 *)(OBJ_VRAM +
                     (tile +
                      gVramCacheEntries[OBJECTS[CHARON_REVEAL]->slot].offset) +
                     pair) = texel;
          }
        }
        revealScale.y = revealScale.x =
            310 * (FIXED_ONE / 32) - frame * (FIXED_ONE / 32);
        revealPlace[3] = 0;
        revealPlace[1] = toFIXED(255);
        revealPlace[0] = toFIXED(92);
        revealPlace[2] = toFIXED(72);
        Render_ApplyProjectedPlacementFar(OBJECTS[CHARON_REVEAL], revealPlace,
                                          &revealScale.x, 0);
        {
          u16 *target = Resource_GetTableEntry(RESOURCE_RED_ORB_TILES);
          u16 *color = (u16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0);

          for (i = 0; i != CHARON_ORB_COLORS; i++, target++, color++) {
            s32 b = RGB_BLUE(*color);
            s32 g = RGB_GREEN(*color);
            s32 r = RGB_RED(*color);
            s32 tb = RGB_BLUE(*target);
            s32 tg = RGB_GREEN(*target);
            s32 tr = RGB_RED(*target);

            if (b < tb) {
              b++;
            }
            if (g < tg) {
              g++;
            }
            if (r < tr) {
              r++;
            }
            *color = RGB(r, g, b);
          }
        }
      }
      // A white flash at frame 302; the backdrop comes back and fades toward a
      // deeper dusk, and three rings of fire spread, eight frames apart.
      if (frame > 301) {
        if (frame == 302) {
          Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0),
                          CHARON_ORB_COLORS * 2, COLOR_PAIR(RGB_WHITE));
          Iwram_CopyWords((u16 *)&BG_PLTT_COLOR(6, 0),
                          Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP),
                          BATTLE_BACKGROUND_COLORS * 2);
          source = (u8 *)Resource_GetTableEntry(RESOURCE_BOLT_SHEET);
          Iwram_CopyWords(BG_PLTT, source, SHEET_PALETTE_BYTES);
          Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                                     work->sheet + CHARON_ACT_PICTURES, 0, 0);
          Graphics_LoadGreyRamp();
        }
        if (frame > 303) {
          u16 *target = Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
          u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, target++, color++) {
            s32 b = RGB_BLUE(*color);
            s32 g = RGB_GREEN(*color);
            s32 r = RGB_RED(*color);
            s32 tb = RGB_BLUE(*target) - CHARON_DEEP_DUSK_DIMMING;
            s32 tg = RGB_GREEN(*target) - CHARON_DEEP_DUSK_DIMMING;
            s32 tr = RGB_RED(*target) + CHARON_DUSK_REDDENING;

            if (tb < 0) {
              tb = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tr > RGB_LEVEL_MAX) {
              tr = RGB_LEVEL_MAX;
            }
            if (b > tb) {
              b--;
            }
            if (g > tg) {
              g--;
            }
            if (r > tr) {
              r--;
            }
            *color = RGB(r, g, b);
          }
        }
        if (frame > 301) {
          s32 k;
          s32 x;
          s32 stripShade;
          s32 size;
          void *texture = Heap_AllocEwram(32);
          struct RenderModel *model = Render_AllocateModelList(1);

          fireRingTexture.logWidth = 7;
          fireRingTexture.logHeight = 7;
          fireRingTexture.pixels = work->sheet + CHARON_ACT_PICTURES;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->texture = &fireRingTexture;
          model->mesh = gBattleEffect_LargeSquareMesh;
          model->vertices = texture;
          for (k = 0; k != 3; k++) {
            x = frame - 302 - k * 8;
            if (x >= 0) {
              stripShade = x * 4 - 16;
              size = 140000 - x * 6192;
              if (size < 0) {
                size = 0;
              }
              if (stripShade > 0) {
                stripShade = 0;
              }
              model->shade = stripShade;
              Matrix_Initialize();
              Matrix_Translate(-toFIXED(8), -toFIXED(8), 0);
              Matrix_Scale(size / 2, size, size / 2);
              if (k == 0) {
                Matrix_RotateZ(TRIG_TURN * 15 / 16);
              } else if (k == 1) {
                Matrix_RotateZ(DEGtoANG(45));
              } else {
                Matrix_RotateZ(TRIG_TURN * 7 / 8);
              }
              Matrix_RotateX(TRIG_TURN / 32);
              Render_TransformVertices(gBattleEffect_GroundSquareCorners,
                                       texture, 4);
              Render_DrawModelList(model);
            }
          }
          Heap_Release(model);
          Heap_Release(texture);
        }
      }
      // At frame 330 the orb turns magenta and swells from nothing,
      // shaking from frame 347.
      if (frame == 330) {
        Iwram_FillWords((u16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0),
                        CHARON_ORB_COLORS * 2,
                        COLOR_PAIR(RGB(RGB_LEVEL_MAX, 0, RGB_LEVEL_MAX)));
      }
      if (frame >= 330 && frame < 351) {
        struct Scale finaleScale = gCharon_FinaleScale;
        s32 dx = 0;
        s32 dz = 0;

        if (frame > 346) {
          dx = 347 - frame;
          dz = (frame - 347) / 4;
        }
        finaleScale.x = finaleScale.y =
            frame * (FIXED_ONE / 32) - 330 * (FIXED_ONE / 32);
        finalePlace[3] = 0;
        finalePlace[1] = toFIXED(255);
        finalePlace[0] = INTtoFIXED(dx) + toFIXED(92);
        finalePlace[2] = INTtoFIXED(dz) + toFIXED(72);
        Render_ApplyProjectedPlacementFar(OBJECTS[CHARON_REVEAL], finalePlace,
                                          &finaleScale.x, 0);
      }
      // From frame 347 the canvas is drawn opaque in magenta: a dark disc
      // rimmed in magenta swells, and from CHARON_DISC_SWAY_FRAME sways across;
      // the backdrop flashes white at frames 357 and 376.
      if (frame == 347) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        REG_BLDCNT = 0;
        {
          u16 *entry = (u16 *)&BG_PLTT_COLOR(0, 1);
          for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++, entry++) {
            s32 r = RGB_LEVEL_MAX - i;
            s32 b = r;

            if (r < 0) {
              r = 0;
            }
            if (b < 0) {
              b = 0;
            }
            *entry = RGB(r, 0, b);
          }
        }
      }
      if (frame > 346) {
        s32 radius = (frame - 345) * 4;
        s32 centerX = CHARON_DISC_X;

        if (frame >= CHARON_DISC_SWAY_FRAME) {
          centerX = (FIXEDtoINT(-(Trig_Sin((frame - CHARON_DISC_SWAY_FRAME) *
                                           (TRIG_TURN / 64)) *
                                  CHARON_DISC_X))) +
                    CHARON_DISC_X;
        }
        if (frame == 360) {
          work->transferMode = CANVAS_COPY;
        }
        if (frame <= 371) {
          BattleEffect_DrawShadedCircles(work->sheet + CHARON_ACT_PICTURES,
                                         radius, FIXED_ONE);
        } else {
          radius = 104;
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        Canvas_BlitAtFoot(work->sheet + CHARON_ACT_PICTURES, centerX, 58,
                          radius);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
        blit[1] = Ram_HeapSlots->ramCodePaired;
      }
      for (j = 0; j != 2; j++) {
        s32 flashFrame = frame - 357 - j * 19;

        if (flashFrame == 0) {
          Iwram_FillWords((u16 *)&BG_PLTT_COLOR(6, 0),
                          BATTLE_BACKGROUND_COLORS * 2, COLOR_PAIR(RGB_WHITE));
        }
        if (flashFrame >= 0 && flashFrame < 160) {
          u16 *target = Resource_GetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
          u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, target++, color++) {
            s32 b = RGB_BLUE(*color);
            s32 g = RGB_GREEN(*color);
            s32 r = RGB_RED(*color);
            s32 tb;
            s32 tg;
            s32 tr;

            if (flashFrame < 8) {
              tb = RGB_BLUE(*target);
              tg = RGB_GREEN(*target);
              tr = RGB_RED(*target);
            } else {
              tb = RGB_BLUE(*target) - CHARON_DUSK_DIMMING;
              tg = RGB_GREEN(*target) - CHARON_DUSK_DIMMING;
              tr = RGB_RED(*target) + CHARON_DUSK_REDDENING;
            }
            if (tb < 0) {
              tb = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tr > RGB_LEVEL_MAX) {
              tr = RGB_LEVEL_MAX;
            }
            if (b > tb + 1) {
              b -= 2;
            }
            if (g > tg + 1) {
              g -= 2;
            }
            if (r > tr + 1) {
              r -= 2;
            }
            *color = RGB(r, g, b);
          }
        }
      }
    }
    // As the seal is stamped a ring of motes spreads from it, darker as it
    // grows.
    if (sealFrame >= 0 && sealFrame < 24) {
      s32 radius = frame * 2 - 104;
      s32 shade = (radius - 10) / 6;
      s32 x;
      s32 y;
      s32 w;
      s32 h;
      FIXED cosine;

      if (shade < 0) {
        shade = 0;
      }
      w = CHARON_RING_MOTE;
      for (i = 0, h = CHARON_RING_MOTE * 2; i != CHARON_RING_MOTES; i++) {
        x = FIXEDtoINT(Trig_Sin(i * (TRIG_TURN / CHARON_RING_MOTES)) * radius *
                       2);
        cosine = Trig_Cos(i * (TRIG_TURN / CHARON_RING_MOTES));
        y = (FIXEDtoINT(cosine * radius)) + 68;
        x += 66;
        blitters[0](canvas, motePictures + shade * CHARON_RING_MOTE_BYTES,
                    x - w / 2, y, w, h);
      }
    }
    // Before Charon comes the backdrop's green drains away, then its red.
    if (frame >= 108 && frame < 132) {
      u16 *color;

      for (i = 0, color = (u16 *)&BG_PLTT_COLOR(6, 0);
           i != BATTLE_BACKGROUND_COLORS; i++, color++) {
        s32 r = RGB_RED(*color);
        s32 g = RGB_GREEN(*color);
        s32 b = RGB_BLUE(*color);

        g--;
        if (g < 0) {
          r--;
        }
        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        } else if (r < 0) {
          r = 0;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        } else if (g < 0) {
          g = 0;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        } else if (b < 0) {
          b = 0;
        }
        *color = RGB(r, g, b);
      }
    }
    // The rune seal is stamped into the battle background's own tiles, on the
    // ground, and its colours pulse until frame 88.
    if (sealFrame >= 0 && sealFrame < 36) {
      if (frame == CHARON_SEAL_FRAME) {
        s32 x;
        s32 y;

        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
        source = (u8 *)Resource_GetTableEntry(RESOURCE_RUNE_SEAL_SHEET);
        source += PALETTE_BANK_BYTES;
        Decompress_TaggedLz(source, work->sheet + CHARON_RUNE_SEAL);
        for (y = 0; y != RUNE_SEAL_HEIGHT; y++) {
          for (x = 0; x != RUNE_SEAL_WIDTH; x += 2) {
            u16 *dst =
                (u16 *)(CHARON_SEAL_TILES +
                        ((((y / 8) * 32 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)));
            u8 hi = *dst >> 8;
            u8 lo = *dst;

            if (work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x + 1] !=
                0) {
              hi = work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x + 1] +
                   CHARON_SEAL_COLOR;
            }
            if (work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x] != 0) {
              lo = work->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH + x] +
                   CHARON_SEAL_COLOR;
            }
            *dst = (hi << 8) | lo;
          }
        }
      }
      if (frame < 88) {
        u16 *color = Resource_GetTableEntry(RESOURCE_RUNE_SEAL_SHEET);
        u16 *entry = (u16 *)&BG_PLTT_COLOR(CHARON_SEAL_PALETTE, 0);

        for (i = 0; i != PALETTE_COLORS; i++, color++, entry++) {
          s32 add =
              (FIXEDtoINT(Trig_Cos(sealFrame << 9) * CHARON_SEAL_GLOW_PULSE)) +
              CHARON_SEAL_GLOW;
          s32 r = RGB_RED(*color) + add;
          s32 g = RGB_GREEN(*color) + add;
          s32 b = RGB_BLUE(*color) + add;

          if (r > RGB_LEVEL_MAX) {
            r = RGB_LEVEL_MAX;
          }
          if (g > RGB_LEVEL_MAX) {
            g = RGB_LEVEL_MAX;
          }
          if (b > RGB_LEVEL_MAX) {
            b = RGB_LEVEL_MAX;
          }
          *entry = RGB(r, g, b);
        }
      }
    }
    // The canvas flashes as the seal comes: half bright, then white.
    if (frame == 54) {
      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                      CANVAS_FILL_WORD((CANVAS_LEVEL_BRIGHTEST + 1) / 2));
    }
    if (frame == 55) {
      Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    // Until Charon comes the swirl turns on the ground, a second inside it
    // the other way; from the seal on, the charm ring and a smaller one inside
    // it turn with them.
    if (frame < 132) {
      s32 risingScale;
      s32 shade;
      s32 rise = FIXED_ONE / 2 - CHARON_SEAL_FRAME * CHARON_SCALE_STEP;
      s32 base;
      s32 quarter;
      s32 twice;
      void *texture = Heap_AllocEwram(32);
      struct RenderModel *model = Render_AllocateModelList(1);

      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      twice = frame * 2;
      quarter = frame * (CHARON_SCALE_STEP / 4);
      shade = twice + MODEL_SHADE_INVISIBLE;
      base = quarter * 4;
      risingScale = CHARON_SWIRL_SCALE - base;
      if (frame >= CHARON_SEAL_FRAME) {
        risingScale = base + rise;
      }
      if (shade >= -8) {
        shade = -8;
      }
      if (frame > 95) {
        shade = 184 - frame * 2;
        model->shade = shade;
      }
      swirlTexture.logWidth = 6;
      swirlTexture.logHeight = 6;
      swirlTexture.pixels = work->sheet;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &swirlTexture;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = texture;
      if (shade > CHARON_SWIRL_SHADE_MIN) {
        model->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(4), 0, 0);
        Matrix_Scale(risingScale, risingScale, risingScale);
        Matrix_RotateX(DEGtoANG(45));
        if (frame >= CHARON_SEAL_FRAME) {
          Matrix_RotateY(frame * (TRIG_TURN / 64));
        } else {
          Matrix_RotateY(-frame * (TRIG_TURN / 64));
        }
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture, 4);
        Render_DrawModelList(model);
      }
      if (frame > 95) {
        shade = 568 - frame * 6;
        model->shade = shade;
      }
      if (shade > CHARON_SWIRL_SHADE_MIN) {
        Matrix_Initialize();
        Matrix_Translate(toFIXED(4), -toFIXED(16), 0);
        Matrix_ScaleUniform(-CHARON_INNER_SWIRL_SCALE);
        Matrix_RotateX(DEGtoANG(45));
        Matrix_RotateY(-frame * (TRIG_TURN / 128));
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture, 4);
        Render_DrawModelList(model);
      }
      if (frame >= CHARON_SEAL_FRAME) {
        s32 ringShade = frame * 2 - 208;

        swirlTexture.pixels = work->sheet + CHARON_ACT_PICTURES;
        if (ringShade >= MODEL_SHADE_INVISIBLE / 2) {
          ringShade = MODEL_SHADE_INVISIBLE / 2;
        }
        model->shade = ringShade;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(2), -toFIXED(32), 0);
        Matrix_ScaleUniform(CHARON_CHARM_RING_SCALE);
        Matrix_RotateX(DEGtoANG(45));
        Matrix_RotateY(frame * (TRIG_TURN / 64));
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture, 4);
        Render_DrawModelList(model);
        /* FAKEMATCH(dead): twice raised to frame though nothing reads it again.
         */
        if (frame > twice) {
          twice = frame;
        }
        // DEAD: the swirls' test of their shade, made of frame, which is never
        // that low; the ROM keeps the test.
        if (frame > CHARON_SWIRL_SHADE_MIN) {
          Matrix_Initialize();
          Matrix_Translate(toFIXED(2), -toFIXED(48), 0);
          Matrix_ScaleUniform(-CHARON_INNER_RING_SCALE);
          Matrix_RotateX(DEGtoANG(45));
          Matrix_RotateY(-frame * (TRIG_TURN / 64));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(texture);
      // DEAD: each test reads the count twice and drops the first read; the ROM
      // keeps both loads.
      while (work->vblanksSinceTransfer, work->vblanksSinceTransfer <= 1) {
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Sound_PlayFar(SE_WAVE);
  if (frame < 132) {
    Sprite_DestroyFar((struct Sprite *)OBJECTS[0]);
  } else {
    for (i = 0; i != CHARON_OBJECTS; i++) {
      Sprite_DestroyFar((struct Sprite *)OBJECTS[i]);
    }
  }
  for (i = 0; i != CHARON_SHARDS; i++) {
    Sprite_DestroyFar((struct Sprite *)SPRITES[i]);
  }
  EFFECT_RELEASE_BLITTERS();
  Palette_LoadBasicSpriteColors();
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON;
  wait(1);
  battle->keepBackdropPalette = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background,
                           BACKGROUND_DARKENED_LEVEL);
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  camera = Ram_HeapSlots->cameraWork;
  shake = 0;
  savedYaw = camera->yaw;
  camera->yaw = shake;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 4);
  REG_BG2X = 0;
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // The attack's ramp runs from a pink violet down to black.
  for (i = 1; i != SHEET_PALETTE_COLORS; i++) {
    s32 q = i * 2 / 3;
    s32 r = RGB_LEVEL_MAX - q;
    s32 g = CHARON_RAMP_GREEN - i / 2;
    s32 b = CHARON_RAMP_BLUE - q;

    if (r < 0) {
      r = 0;
    }
    if (g < 0) {
      g = 0;
    }
    if (b < 0) {
      b = 0;
    }
    BG_PLTT[i] = RGB(r, g, b);
  }
  arg->targets[arg->targetCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(arg->targets, 0);
  BattleEffect_DrawShadedCircles(work->sheet, CHARON_COLUMN_END, FIXED_ONE);
  BattleEffect_DrawShadedCircles(work->sheet + CHARON_ACT_PICTURES,
                                 CHARON_DROP_RADIUS, FIXED_ONE);
  Iwram_FillWords(work->sheet + CHARON_BRIGHT_BLOCK, CHARON_TEXTURE_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  Resource_LoadAndDecompress(RESOURCE_GLOW_RING_TEXTURE,
                             work->sheet + CHARON_GLOW_RING, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motePictures, 0, 0);
  // DEAD: i counts to MOTE_SET_BYTES by 35, and nothing reads it; the ROM
  // keeps the add and the test.
  {
    s32 limit;

    i = 0;
    limit = MOTE_SET_BYTES;
  empty:
    i += 35;
    if (i != limit) {
      goto empty;
    }
  }
  for (i = 0; i != CHARON_TEXTURE_BYTES; i++) {
    if (work->sheet[CHARON_GLOW_RING + i] != 0) {
      work->sheet[CHARON_GLOW_RING + i] = CANVAS_LEVEL_BRIGHTEST;
    }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  for (i = 0; i != CHARON_DROP_SLOTS; i++) {
    DROPS[i].variant = -i;
  }
  EFFECT_FREE_PARTICLES(i, CHARON_PARTICLES,
                        CHARON_MOTE_COUNT + CHARON_BURST_COUNT);
  for (i = 0; i != arg->targetCount; i++) {
    struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

    HOMES[i].x = actor->x.fixed;
    HOMES[i].y = actor->y.fixed;
    HOMES[i].z = actor->z.fixed;
  }

  width = CHARON_COLUMN_END;
  dimming = CHARON_FLASH_DIMMING;
  frame = 0;
  do {
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame < 120) {
      frame = 120;
    }
    if (frame == 0) {
      Sound_PlayFar(SE_WAVE);
      WAVE_RADIUS = 160;
      shake = 0;
      for (i = 0; i != arg->targetCount; i++) {
        CURSES[i].variant = -1;
      }
      for (i = 0; i != arg->targetCount; i++) {
        struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

        BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_NORMAL,
                                 POSE_DAMAGE, TINT_KEEP, -1);
        actor->x.fixed = -(i + 4) * toFIXED(128);
        actor->z.fixed = 0;
        actor->y.fixed = toFIXED(20);
        actor->gravity = 0;
      }
    }
    if (frame == 120) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
      // The battle's own background comes back as dark as a canvas effect's
      // fade leaves it.
      ColorBuffer_ScaleFar(battle->palette, (u16 *)&BG_PLTT_COLOR(6, 0),
                           FIXED_ONE -
                               BACKGROUND_DARKEN_FRAMES * BACKGROUND_LEVEL_STEP,
                           BATTLE_BACKGROUND_COLORS);
      for (i = 0; i != arg->targetCount; i++) {
        struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

        SET_ACTOR_POSITION(actor, 0, toFIXED(124), 0);
        actor->gravity = 0;
      }
    }
    if (frame == 160) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    // At frame 140 the curse breaks in a white flash: flash bursts and sparks
    // fly from the middle, and the targets are thrown up.
    if (frame >= 140) {
      if (frame == 140) {
        for (i = 0; i != CHARON_BURST_COUNT; i++) {
          s32 speed = (Random16() & 511) + 32;
          s32 angle = Random16();

          CHARON_BURSTS[i].x = toFIXED(60);
          CHARON_BURSTS[i].y = toFIXED(60);
          CHARON_BURSTS[i].velocityX = (Trig_Sin(angle) * speed) >> 5;
          CHARON_BURSTS[i].velocityY = (Trig_Cos(angle) * speed) >> 5;
          CHARON_BURSTS[i].variant = 0;
        }
        for (i = 0; i != CHARON_SPARK_COUNT; i++) {
          s32 speed = (Random16() & 255) + 32;
          s32 angle = Random16();

          CHARON_SPARKS[i].x = toFIXED(60);
          CHARON_SPARKS[i].y = toFIXED(60);
          CHARON_SPARKS[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
          CHARON_SPARKS[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
          CHARON_SPARKS[i].variant = (Random16() & 15) + 16;
        }
        Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, SPRITE_BUFFER, 1,
                                   0);
        Sound_PlayFar(SE_CLATTER);
        Iwram_FillWords(canvas, CANVAS_VIEW_BYTES,
                        CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        for (i = 0; i != arg->targetCount; i++) {
          struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;

          SET_ACTOR_POSITION(actor, 0, toFIXED(32), HOMES[i].z);
        }
        for (i = 0; i != arg->targetCount; i++) {
          s32 roll = Random16();
          s32 percent = (Random16() & 31) + 120;
          s32 velocityY = roll * 2 + toFIXED(2);

          BattleEffect_StartActorMotion(arg->targets[i], 1, toFIXED(4),
                                        toFIXED(8), velocityY, percent);
          BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 32);
        }
        work->shakeFrames = 3;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
      blitters[0] = ((DrawRectangle)Ram_HeapSlots->ramCode);
      for (i = 0; i != CHARON_BURSTS_DRAWN; i++) {
        struct Particle *p = &CHARON_BURSTS[i];

        if (p->variant >= 0 && p->variant < CHARON_BURST_FRAMES) {
          s32 row = (p->variant / 4) * PICTURE_BYTES(FLASH_BURST);
          u8 *buffer = SPRITE_BUFFER;
          s32 x = (FIXEDtoINT(p->x)) - FLASH_BURST_WIDTH / 2;

          row += (s32)buffer;
          buffer = (u8 *)row;
          blitters[0](canvas, buffer, x,
                      (FIXEDtoINT(p->y)) - FLASH_BURST_HEIGHT / 2,
                      FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity2D(p, 60, -toFIXED(1) / 8);
        }
        p->variant++;
      }
      for (i = 0; i != CHARON_SPARK_COUNT; i++) {
        struct Particle *p = &CHARON_SPARKS[i];

        if (p->variant >= 0) {
          s32 size = (p->variant >> 3) + 2;
          s32 k;

          for (k = 0; k != 4; k++) {
            blitters[0](canvas,
                        motePictures + gBattleEffect_MotePictures[size - 1],
                        (FIXEDtoINT(p->x)) - size / 2,
                        (FIXEDtoINT(p->y)) - size, size, size * 2);
            Particle_AdvanceWithGravity2D(p, 63, 0);
            if (p->y > toFIXED(112)) {
              p->velocityY = -p->velocityY;
            }
          }
          p->variant--;
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    // The backdrop flashes white three times as the targets are drawn in.
    if (frame < 120) {
      for (i = 0; i != CHARON_FLASHES; i++) {
        if (frame == gCharon_FlashFrames[i]) {
          Iwram_FillWords((u16 *)&BG_PLTT_COLOR(6, 0),
                          BATTLE_BACKGROUND_COLORS * 2, COLOR_PAIR(RGB_WHITE));
        }
        if (frame >= gCharon_FlashFrames[i] + 1 &&
            frame < gCharon_FlashFrames[i] + 16) {
          u16 *target = Ram_HeapSlots->battleWork->palette;
          u16 *color = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (j = 0; j != BATTLE_BACKGROUND_COLORS; j++) {
            s32 tr = RGB_RED(target[j]) - dimming / 2;
            s32 tg = RGB_GREEN(target[j]) - CHARON_FLASH_DIMMING;
            s32 tb = RGB_BLUE(target[j]) - dimming / 2;
            s32 r = RGB_RED(color[j]);
            s32 g = RGB_GREEN(color[j]);
            s32 b = RGB_BLUE(color[j]);

            if (tr < 0) {
              tr = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tb < 0) {
              tb = 0;
            }
            if (tr < r - 1) {
              r -= 2;
            }
            if (tg < g - 1) {
              g -= 2;
            }
            if (tb < b - 1) {
              b -= 2;
            }
            color[j] = RGB(r, g, b);
          }
        }
      }
    }
    if (frame > 120) {
      if (frame == 121) {
        gBgScroll[1].x = savedScrollX;
        backgroundView->busy = 0;
        camera->yaw = savedYaw;
      }
      if (frame < 148) {
        s32 step = 8 - (frame - 120) / 6;
        s32 wave = WAVE_RADIUS;

        WAVE_RADIUS = wave - step;
        if (wave > 0) {
          BattleEffect_DrawShadedCircles(work->sheet, wave, FIXED_ONE);
          Canvas_BlitAtFoot(work->sheet, 60, 60, wave);
        }
      }
    } else {
      if (frame >= 4 && frame < 26) {
        shake += 4;
      }
      if (frame > 107) {
        shake -= 8;
        if (shake < 0) {
          shake = 0;
        }
      }
      for (i = 0; i != arg->targetCount; i++) {
        struct Actor *actor = BattleObject_GetFar(arg->targets[i])->object;
        struct Particle *curse = &CURSES[i];
        s32 landed = 0;

        if (actor->y.fixed < toFIXED(100)) {
          if (curse->variant == -1) {
            actor->x.fixed += toFIXED(10);
          } else {
            actor->x.fixed += toFIXED(2);
          }
          if (curse->variant == -1) {
            if (actor->x.fixed > toFIXED(48)) {
              landed = 1;
            }
            if (landed == 1) {
              curse->variant = 0;
              EffectPosition_ProjectScaledBattler(arg->targets[i], curse);
              Sound_PlayFar(SE_CURSE);
              for (j = 0; j != CHARON_CURSE_MOTES; j++) {
                s32 angle = Random16() & (TRIG_TURN / 2 - 1);
                s32 speed;
                struct Particle *p =
                    &CHARON_PARTICLES[i * CHARON_CURSE_MOTES + j];

                speed = (Random16() & 255) + 256;

                // The canvas is half the screen's width.
                p->x = curse->x * (FIXED_ONE / 2);
                p->y = INTtoFIXED(curse->y);
                p->velocityX = -(Trig_Sin(angle) * speed) >> 7;
                p->velocityY = (Trig_Cos(angle) * speed) >> 6;
                p->variant = (Random16() & 15) + 16;
              }
            }
          }
        }
        if (curse->variant == 2 || curse->variant == 7) {
          BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 32);
        }
        /* FAKEMATCH(dead): landed set to 2 though nothing reads it again. */
        if (actor == 0) {
          landed = 2;
        }
        if (curse->variant == 6) {
          BattleEffect_TintBattler(arg->targets[i], SPRITE_COLOR_HIDDEN,
                                   TINT_KEEP, i, 32);
        }
        if (curse->variant == 8) {
          SET_ACTOR_POSITION(actor, 0, toFIXED(148), 0);
        }
      }
      if (frame >= 0) {
        gBgScroll[1].x -= 12;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
        {
          struct Particle *column;
          s32 x;
          s32 y;
          s32 y2;
          for (i = 0, y2 = -26, y = 0, x = shake,
              phase = frame * (TRIG_TURN / 64), column = COLUMNS,
              halfWidth = width / 2;
               i != CHARON_COLUMNS;
               y2 += CHARON_COLUMN_HEIGHT, y += CHARON_COLUMN_HEIGHT, x += 8,
              phase += DEGtoANG(45), column++, i++) {
            column->x = x + (FIXEDtoINT(Trig_Sin(phase) * 4));
            column->y = y;
            if (i == 3) {
              column->x -= 8;
              column->y = y2;
              HeapSlot_Release(HEAP_SLOT_RAM_CODE);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                                  BLITTER_MAX);
            }
            ((DrawRectangle)Ram_HeapSlots->ramCode)(
                canvas, work->sheet, column->x - halfWidth, column->y,
                halfWidth, width);
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        for (i = 0; i != CHARON_DROPS; i++) {
          struct Particle *drop = &DROPS[i];

          if (++drop->variant == 4) {
            drop->variant = 0;
          }
          if (drop->variant == 0) {
            drop->x = INTtoFIXED(Random16() % 24 + shake - i * 2);
            drop->y = toFIXED(80) + i * toFIXED(3);
            drop->velocityX = -(Random16() & TRIG_ANGLE_MASK) - toFIXED(0.25);
            drop->velocityY = -toFIXED(1);
          }
          if (drop->variant >= 0) {
            Canvas_BlitAtFoot(work->sheet + CHARON_ACT_PICTURES,
                              FIXEDtoINT(drop->x), FIXEDtoINT(drop->y),
                              CHARON_DROP_RADIUS);
            drop->x += drop->velocityX;
            drop->y += drop->velocityY;
          }
        }
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
        blitters[0] = ((DrawRectangle)Ram_HeapSlots->ramCode);
        for (i = 0; i != CHARON_COLUMNS; i++) {
          s32 width = 120 - COLUMNS[i].x;

          if (width > 0) {
            blitters[0](canvas, work->sheet + CHARON_BRIGHT_BLOCK, COLUMNS[i].x,
                        i * CHARON_COLUMN_HEIGHT, width, CHARON_COLUMN_HEIGHT);
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                            BLITTER_TRANSPARENT);
        blitters[0] = ((DrawRectangle)Ram_HeapSlots->ramCode);
        for (i = 0; i != CHARON_MOTE_COUNT; i++) {
          struct Particle *p = &CHARON_PARTICLES[i];

          if (p->variant >= 0) {
            s32 size = (p->variant >> 3) + 2;

            blitters[0](canvas,
                        motePictures + gBattleEffect_MotePictures[size - 1],
                        (FIXEDtoINT(p->x)) - size / 2,
                        (FIXEDtoINT(p->y)) - size, size, size * 2);
            Particle_AdvanceWithGravity2D(p, 60, 0);
            p->variant--;
          }
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      }
      {
        void *texture = Heap_AllocEwram(32);
        struct RenderModel *model = Render_AllocateModelList(1);

        glowRingTexture.logWidth = 6;
        glowRingTexture.logHeight = 6;
        glowRingTexture.pixels = work->sheet + CHARON_GLOW_RING;
        model->kind = MODEL_SHADED;
        model->mesh = gBattleEffect_SquareMesh;
        model->texture = &glowRingTexture;
        model->vertices = texture;
        for (i = 0; i != arg->targetCount; i++) {
          struct Particle *curse = &CURSES[i];
          s32 timer = curse->variant;

          if (timer >= 0 && timer < 18) {
            s32 step = timer * 3;
            s32 shade = step + MODEL_SHADE_INVISIBLE;
            s32 size = step * CHARON_GLOW_RING_GROWTH + toFIXED(1) / 32;

            curse->variant = timer + 1;
            model->shade = shade;
            Matrix_Initialize();
            Matrix_Translate(INTtoFIXED(curse->x / 2 - 56),
                             INTtoFIXED(curse->y - 56), 0);
            Matrix_Scale(size / 3, size, size);
            Matrix_RotateX(TRIG_TURN * 3 / 4);
            Render_TransformVertices(gBattleEffect_GroundSquareCorners, texture,
                                     4);
            Render_DrawModelList(model);
          }
          // A release only sets its pool back to the block, so the next
          // target still draws from the same model.
          Heap_Release(model);
          Heap_Release(texture);
        }
      }
    }
    Camera_ApplyVerticalShake(8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != CHARON_ATTACK_FRAMES);
  // DEAD: an empty loop over the targets; the ROM keeps its count and test.
  for (i = 0; i != arg->targetCount; i++) {
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// One quad of a vertex strip: its upper and lower triangles.
struct StripQuad {
  struct MeshTriangle upper;
  struct MeshTriangle lower;
};

/// Builds count quads of a vertex strip two vertices apart, each width texels
/// wide and height texels tall, and ends the list with an empty triangle.
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count) {
  s32 i;

  for (i = 0; i != count; i++) {
    quad[i].upper.uv[0][1] = 1;
    quad[i].upper.uv[2][1] = 1;
    quad[i].lower.uv[2][1] = 1;
    quad[i].upper.vertex[0] = i * 2;
    quad[i].upper.uv[0][0] = i * width;
    quad[i].upper.uv[1][0] = i * width;
    quad[i].upper.uv[2][0] = i * width + width - 1;
    quad[i].lower.uv[0][0] = i * width;
    quad[i].lower.uv[1][0] = i * width + width - 1;
    quad[i].lower.uv[2][0] = i * width + width - 1;
    quad[i].upper.vertex[1] = i * 2 + 1;
    quad[i].upper.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[0] = i * 2 + 1;
    quad[i].lower.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[1] = i * 2 + 3;
    quad[i].upper.uv[1][1] = height - 1;
    quad[i].lower.uv[0][1] = height - 1;
    quad[i].lower.uv[1][1] = height - 1;
  }
  SET_VECTOR(quad[count].upper.vertex, 0, 0, 0);
}
#undef SPRITES
#undef SPRITE_BUFFER
#undef OAM_BUFFER
#undef REVEAL_BUFFER
#undef ORB_BUFFER
#undef CHARON_SEAL_TILES
#undef DROPS
#undef CURSES
#undef HOMES
#undef COLUMNS
#undef WAVE_RADIUS
#undef OBJECTS
