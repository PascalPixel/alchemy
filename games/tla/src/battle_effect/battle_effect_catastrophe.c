/// @file
/// The Catastrophe summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// A corner of a strip as an effect builds it, in signed bytes: the smoke band
/// of Catastrophe's last act, Haures's bands.
struct MeshVertex {
  s8 x;
  s8 y;
  s8 z;
};

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define CATASTROPHE_FLASH_BURST_PICTURES 6144

/// Where Catastrophe decodes its pictures: JaggedStreakSheet in act one;
/// PentagonSealTexture after TriangleSealTexture, in the pictures in act one
/// and the map cell buffer in act two; SwirlTexture and then FireRingTexture in
/// the map cell buffer; and TornadoSheet (act two) or ClawSlashSheet (act
/// three), whose glints and slash the last acts sweep across.
#define CATASTROPHE_JAGGED_STREAKS 8192
#define CATASTROPHE_PENTAGON_SEAL 4096
#define CATASTROPHE_RING 8192
#define CATASTROPHE_SWEEP_PICTURES 2048

/// LightningTexture's two 32x48 forks, which flash in act one's sky.
#define LIGHTNING_FORK_WIDTH 32
#define LIGHTNING_FORK_HEIGHT 48
/// JaggedStreakSheet's three streaks, and the four 32x128 textures act two
/// copies LightningBoltSheet's bolts into, each bolt in the middle of its own,
/// after TornadoSheet's glints.
#define JAGGED_STREAK_WIDTH 32
#define JAGGED_STREAK_HEIGHT 128
#define BOLT_TEXTURE_WIDTH 32
#define BOLT_TEXTURE_HEIGHT 128
#define CATASTROPHE_BOLT_TEXTURES (CATASTROPHE_SWEEP_PICTURES + GLINT_SET_BYTES)
#define CATASTROPHE_BOLT_TEXTURE_COUNT 4

/// Catastrophe's sprites, all 32x32: the chained demon (ChainedDemonTiles),
/// its mirrored copy's six, the golden shield, and the eleven of the sword (its
/// hilt's five, then its white blade's six). The demon's lowest four are copies
/// of its top row, sharing their tiles: those show the sheet's last four
/// pictures, and the top row's own from frame 339 to 450.
#define CATASTROPHE_DEMON 0
#define CATASTROPHE_DEMON_TILED 24
#define CATASTROPHE_DEMON_SWAPPED 24
#define CATASTROPHE_DEMON_SWAPPED_COUNT 4
#define CATASTROPHE_DEMON_COUNT 28
#define CATASTROPHE_MIRRORED_DEMON 28
#define CATASTROPHE_MIRRORED_DEMON_COUNT 6
#define CATASTROPHE_SHIELD 34
#define CATASTROPHE_SHIELD_COUNT 5
#define CATASTROPHE_SWORD 39
#define CATASTROPHE_HILT_COUNT 5
#define CATASTROPHE_BLADE 44
#define CATASTROPHE_BLADE_COUNT 6
#define CATASTROPHE_SWORD_COUNT                                                \
  (CATASTROPHE_HILT_COUNT + CATASTROPHE_BLADE_COUNT)
#define CATASTROPHE_SPRITES 50
/// ChainedDemonTiles' colours, OBJ palette 0 to 191.
#define CATASTROPHE_DEMON_COLORS 192
/// Where the decoded ChainedDemonTiles keeps its last four pictures in the map
/// cell buffer, and where they are kept again, after the top row's.
#define CATASTROPHE_DEMON_LOWEST_PICTURES                                      \
  (CATASTROPHE_DEMON_TILED * OBJ_32X32_8BPP_BYTES)
#define CATASTROPHE_DEMON_KEPT_PICTURES                                        \
  (CATASTROPHE_DEMON_SWAPPED_COUNT * OBJ_32X32_8BPP_BYTES)
/// The OBJ palettes of the shield, the sword's hilt and its blade.
#define CATASTROPHE_SHIELD_PALETTE 12
#define CATASTROPHE_HILT_PALETTE 13
#define CATASTROPHE_BLADE_PALETTE 14
/// The frames of act one, unless A or B skips it.
#define CATASTROPHE_ACT_ONE_FRAMES 637
/// As the demon swings its sword (frames 472 to 485), the shield moves right
/// and the sword left by CATASTROPHE_SWING_DRIFT a frame, less a sixteenth of a
/// pixel more each frame, and both sink CATASTROPHE_SWING_SINK a frame.
#define CATASTROPHE_SWING_DRIFT 120000
#define CATASTROPHE_SWING_SINK 75536
/// From frame 506 the view dives after the sword's blade, the demon rising out
/// of sight, at a speed that grows to twice this a frame.
#define CATASTROPHE_DIVE_SPEED_MAX 120000
/// Where the objects' first 240 colours are kept in the effect's pictures,
/// which the objects fade from.
#define CATASTROPHE_SAVED_OBJ_PALETTE 20480
#define CATASTROPHE_SAVED_OBJ_COLORS 240
/// Act one's claps of thunder, each flashing the backdrop blue, and the frames
/// after 242 on which the canvas flashes white.
#define CATASTROPHE_FLASHES 18
#define CATASTROPHE_CANVAS_FLASHES 17
/// The level the canvas flashes to: on those frames, every other frame from 537
/// to 592, and at 630 and 634.
#define CATASTROPHE_FLASH_LEVEL 31
/// Act one's three strikes, each of up to CATASTROPHE_STRIKE_STEPS steps (their
/// frames' rows one longer, for the 65535 that ends them), and the sparks each
/// step throws.
#define CATASTROPHE_STRIKES 3
#define CATASTROPHE_STRIKE_STEPS 12
#define CATASTROPHE_STRIKE_SPARKS 10
/// The orb's disc is drawn this much brighter than full, its middle held at
/// the brightest level.
#define CATASTROPHE_ORB_BRIGHTNESS 95536
/// Act two's orb is centred at this x on the canvas, and at this y as it
/// starts, before it rises and swells; its sparks fly out from (x, y).
#define CATASTROPHE_ORB_CENTER 60
/// The seal: the pentagon laid back by CATASTROPHE_SEAL_TILT, its three
/// triangles turned up from its edges by as much as CATASTROPHE_SEAL_SIDE_TILT,
/// and its largest scale.
#define CATASTROPHE_SEAL_TILT 18768
#define CATASTROPHE_SEAL_SIDE_TILT 19712
#define CATASTROPHE_SEAL_SCALE_MAX 140600
/// The swirl over the seal rises this much a frame for its first
/// CATASTROPHE_SWIRL_RISE_FRAMES frames.
#define CATASTROPHE_SWIRL_RISE 120000
#define CATASTROPHE_SWIRL_RISE_FRAMES 7
/// The seal's scale as act two opens, under the orb, before it swells.
#define CATASTROPHE_AERIAL_SEAL_SCALE 85536
/// How fast the rings of fire grow, a frame.
#define CATASTROPHE_RING_GROWTH 12192
/// Act two's bolts, and the sparks after them in work->particles.
#define CATASTROPHE_BOLTS 16
#define CATASTROPHE_SPARKS 48
/// Late in act two the canvas is cleared each frame to this pale level, the
/// land washing out, and then to the brightest, white.
#define CATASTROPHE_WASH_LEVEL 16
/// Act three's buffers in the map cell buffer: the smoke band's corners at its
/// start, their transformed vertices, its strip mesh, LightningBoltSheet's four
/// bolts, and the particles: the flash bursts thrown off the band, then the
/// debris; and the frames each lives.
#define CATASTROPHE_SWEEP_VERTICES 4096
#define CATASTROPHE_SWEEP_STRIP 8192
#define CATASTROPHE_BOLT_PICTURES 16384
#define CATASTROPHE_PARTICLES                                                  \
  ((struct Particle *)(Ram_MapCellBuffer + CATASTROPHE_BOLT_PICTURES +         \
                       4 * LIGHTNING_BOLT_PICTURE_BYTES))
#define CATASTROPHE_BURSTS 64
#define CATASTROPHE_DEBRIS 100
#define CATASTROPHE_BURST_LIFE 12
#define CATASTROPHE_DEBRIS_LIFE 16
/// The debris each target throws when struck, and each bolt where it lands.
#define CATASTROPHE_TARGET_DEBRIS 16
#define CATASTROPHE_BOLT_DEBRIS 12
/// The smoke band's rows of two corners, and its wave's phase a frame.
#define CATASTROPHE_SWEEP_ROWS 12
#define CATASTROPHE_SWEEP_WAVE 6096
/// The sweep's scale, the smoke band's and the slash's, and the band's own
/// further scale, turned upright.
#define CATASTROPHE_SWEEP_SCALE 55536
#define CATASTROPHE_BAND_SCALE 274144
/// Act three opens CATASTROPHE_PAN pixels right of the battle and pans back at
/// CATASTROPHE_PAN_SPEED pixels a frame, slowing to a stop from frame 62.
#define CATASTROPHE_PAN 1088
#define CATASTROPHE_PAN_SPEED 16

/// Catastrophe: over a red sky the chained demon with its golden shield spreads
/// its wings and drives down a sword of light; the blade's orb falls on the
/// land far below and bursts in a ring and a white flash; then violet lightning
/// and smoke sweep in from the right over the foes. Holding A or B shortens
/// each act.
///
/// Abilities: Catastrophe.
void BattleEffect_RunCatastrophe(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle blit[2];
  s32 savedScroll;
  struct BattleBackgroundWork *background;
  u8 *sprites;
  struct BattleWork *battle;
  FIXED camZ;
  s32 swapped;
  s32 drift;
  s32 sway;
  s32 flashTimer;
  s32 flash;
  void *verts;
  s32 shade;
  struct RenderModel *model;
  struct RenderModel *aerialModel;
  struct RenderModel *sweepModel;
  s32 frame;
  s32 i;
  s32 j;
  struct Particle *p;
  struct Particle *q;
  struct Sprite *obj;
  struct ModelTexture aerialTexture;
  s16 list[UNIT_MAX];
  FIXED place[4];
  struct ScreenPoint pos;
  struct Scale spriteScale;
  struct ModelTexture texture;
  struct ModelTexture sweepTexture;
  struct Particle *bolt;
  struct Particle *spark;
  struct Particle *burst;
  u16 *backdropColor;
  u8 *resource;
  u8 *cellBuffer = Ram_MapCellBuffer;
  s32 span = toFIXED(8);
  s32 rowStep = 8;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  savedScroll = gBgScroll[1].x;
  background = Ram_HeapSlots->battleBackground;
  sprites = Ram_HeapSlots->battleSheet;
  battle = Ram_HeapSlots->battleWork;
  VramCache_FreeEntry(battle->arrowTiles);
  ActorShadow_FreeTilesFar();
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = Ram_HeapSlots->ramCode;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  list[0] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(list, 0);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_RED_HAZE_BACKDROP,
                           0);
  Palette_BrightenSavedBattleBg(-RGB_LEVEL_MAX, -RGB_LEVEL_MAX, -RGB_LEVEL_MAX);
  background->busy = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // BG1 shows the red haze as one picture 128 wide and 240 tall, each row
  // twice across: its upper 15 rows are the battle background's left half
  // (from tile 512, at BG character block 2), its lower 15 the right half.
  {
    u16 *map = (u16 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK);

    {
      s32 row;

      for (row = 0; row != 30; row++) {
        if (row > 14) {
          {
            s32 column;

            for (column = 0; column != 32; column++) {
              *map++ = row * 32 + (column & 15) + 48;
            }
          }
        } else {
          {
            s32 column;

            for (column = 0; column != 32; column++) {
              *map++ = row * 32 + (column & 15) + 512;
            }
          }
        }
      }
    }
  }
  {
    u8 *cells;

    resource = Resource_GetTableEntry(RESOURCE_CHAINED_DEMON_TILES);
    Iwram_CopyWords((void *)OBJ_PLTT, resource, CATASTROPHE_DEMON_COLORS * 2);
    resource += CATASTROPHE_DEMON_COLORS * 2;
    Decompress_TaggedLz(resource, cellBuffer);
    cells = Ram_MapCellBuffer;
    for (i = 0; i != CATASTROPHE_DEMON_TILED;
         cells += OBJ_32X32_8BPP_BYTES, i++) {
      obj = EffectSprite_CreateWithTiles(32, 32,
                                         OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
      obj->image.priority = 2;
      work->sprites[i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_8BPP_BYTES);
    }
    {
      u8 *cells = Ram_MapCellBuffer + CATASTROPHE_DEMON_LOWEST_PICTURES;

      for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++) {
        obj = EffectSprite_Acquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        work->sprites[CATASTROPHE_DEMON_SWAPPED + i] = obj;
        Iwram_CopyWords(obj, work->sprites[i], offsetof(struct Sprite, lastNo));
        Iwram_CopyWords(
            (void *)OBJ_TILES(work->sprites[CATASTROPHE_DEMON_SWAPPED + i]),
            cells + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
      }
    }
    Iwram_CopyWords(cellBuffer + CATASTROPHE_DEMON_KEPT_PICTURES,
                    Ram_MapCellBuffer + CATASTROPHE_DEMON_LOWEST_PICTURES,
                    CATASTROPHE_DEMON_SWAPPED_COUNT * OBJ_32X32_8BPP_BYTES);
  }
  for (i = 0; i != CATASTROPHE_MIRRORED_DEMON_COUNT; i++) {
    obj = EffectSprite_Acquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
    work->sprites[CATASTROPHE_MIRRORED_DEMON + i] = obj;
    Iwram_CopyWords(obj, work->sprites[i], offsetof(struct Sprite, lastNo));
  }
  {
    u8 *cells;

    resource = Resource_GetTableEntry(RESOURCE_GOLDEN_SCALES_TILES);
    Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(CATASTROPHE_SHIELD_PALETTE, 0),
                    resource, PALETTE_BANK_BYTES);
    resource += PALETTE_BANK_BYTES;
    Decompress_TaggedLz(resource, work->sheet);
    cells = work->sheet;
    for (i = 0; i != CATASTROPHE_SHIELD_COUNT;
         cells += OBJ_32X32_4BPP_BYTES, i++) {
      obj = EffectSprite_CreateWithTiles(
          32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(CATASTROPHE_SHIELD_PALETTE));
      obj->image.priority = 1;
      work->sprites[CATASTROPHE_SHIELD + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_4BPP_BYTES);
    }
  }
  {
    u8 *cells;

    resource = Resource_GetTableEntry(RESOURCE_PURPLE_ARMOUR_TILES);
    Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(CATASTROPHE_HILT_PALETTE, 0),
                    resource, PALETTE_BANK_BYTES);
    resource += PALETTE_BANK_BYTES;
    Decompress_TaggedLz(resource, work->sheet);
    cells = work->sheet;
    for (i = 0; i != CATASTROPHE_HILT_COUNT;
         cells += OBJ_32X32_4BPP_BYTES, i++) {
      obj = EffectSprite_CreateWithTiles(32, 32, OBJ_SIZE_32X32,
                                         OBJ_PALETTE(CATASTROPHE_HILT_PALETTE));
      obj->image.priority = 1;
      work->sprites[CATASTROPHE_SWORD + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_4BPP_BYTES);
    }
  }
  {
    u8 *cells;

    resource = Resource_GetTableEntry(RESOURCE_WHITE_SPIKES_TILES);
    Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(CATASTROPHE_BLADE_PALETTE, 0),
                    resource, PALETTE_BANK_BYTES);
    resource += PALETTE_BANK_BYTES;
    Decompress_TaggedLz(resource, work->sheet);
    cells = work->sheet;
    for (i = 0; i != CATASTROPHE_BLADE_COUNT;
         cells += OBJ_32X32_4BPP_BYTES, i++) {
      obj = EffectSprite_CreateWithTiles(
          32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(CATASTROPHE_BLADE_PALETTE));
      obj->image.priority = 1;
      work->sprites[CATASTROPHE_BLADE + i] = obj;
      Iwram_CopyWords((void *)OBJ_TILES(obj), cells, OBJ_32X32_4BPP_BYTES);
    }
  }
  Iwram_CopyWords(work->sheet + CATASTROPHE_SAVED_OBJ_PALETTE, (void *)OBJ_PLTT,
                  CATASTROPHE_SAVED_OBJ_COLORS * 2);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_TEXTURE, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_JAGGED_STREAK_SHEET,
                             work->sheet + CATASTROPHE_JAGGED_STREAKS, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                  SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
                             Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
  camZ = 0;
  swapped = 0;
  drift = 0;
  sway = 0;
  flashTimer = 0;
  flash = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = Ram_HeapSlots->ramCode;
  // Act one: the demon breaks out of its chains and drives down its sword.
  frame = 0;
  while (frame != CATASTROPHE_ACT_ONE_FRAMES) {
    s32 level;
    s32 d;

    // The backdrop fades in over the first 16 frames; A or B skips the act.
    if (frame <= 15) {
      s32 v = frame * 2 - 30;

      Palette_BrightenSavedBattleBg(v, v, v);
    }
    if (gInput.repeat & (KEY_A | KEY_B)) {
      break;
    }
    if (frame == 0) {
      REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                   BGCNT_SCREEN_BASE(7);
      for (i = CATASTROPHE_STRIKES; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame == 440) {
      REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    }
    if (frame <= 369) {
      camZ = INTtoFIXED(frame * 334 / 379) + -toFIXED(282);
    } else if ((u32)(d = frame - 370) <= 19) {
      camZ = Trig_Sin(d * (TRIG_TURN / 40)) * 9 + toFIXED(43);
    } else if ((u32)(frame - 390) <= 95) {
      camZ += -toFIXED(1) / 2;
    } else if ((u32)(frame - 486) <= 19) {
      camZ += -toFIXED(1);
    } else if ((u32)(frame - 506) <= 47) {
      s32 v = (frame - 506) * 7296 + 50768;

      if (v > CATASTROPHE_DIVE_SPEED_MAX) {
        v = CATASTROPHE_DIVE_SPEED_MAX;
      }
      camZ -= v << 1;
    }
    if (frame <= 379) {
      gBgScroll[1].y = 104 - frame * 104 / 379;
    } else {
      s32 v;

      gBgScroll[1].y = v = (frame - 380) / 4;
      if ((u32)INTtoFIXED(v) > toFIXED(104)) {
        gBgScroll[1].y = 104;
      }
    }
    {
      s32 swayFrame = frame - 370;
      if ((u32)swayFrame <= 59) {
        sway = Trig_Sin(swayFrame * (TRIG_TURN / 240) / 2) << 4;
      }
    }
    if ((u32)(frame - 420) <= 51) {
      drift = (420 - frame) * (toFIXED(1) / 16);
      sway += -toFIXED(1) / 4;
    } else if ((u32)(frame - 472) <= 13) {
      s32 swingFrame = frame - 472;

      drift = drift - swingFrame * (toFIXED(1) / 16) + CATASTROPHE_SWING_DRIFT;
      sway += CATASTROPHE_SWING_SINK;
    } else if ((u32)(frame - 486) <= 8) {
      drift += toFIXED(0.125);
      sway -= Trig_Sin((frame - 486) * (TRIG_TURN / 512));
    } else if ((u32)(frame - 554) <= 29) {
      sway += -toFIXED(1);
    } else if ((u32)(frame - 584) <= 9) {
      sway += -toFIXED(8);
    }
    if (frame == 0) {
      flashTimer = -1;
      flash = 0;
    }
    // Each clap of thunder flashes the backdrop blue, fading over its length.
    if (flash < CATASTROPHE_FLASHES) {
      level = -1;
      if (frame == gCatastrophe_FlashFrames[flash]) {
        flashTimer = gCatastrophe_FlashLengths[flash];
        switch (flash & 3) {
        case 0:
          Sound_PlayFar(SE_TOPPLE);
          break;
        case 1:
          Sound_PlayFar(SE_THUNDER);
          break;
        case 2:
          Sound_PlayFar(SE_BANE_ESCAPE);
          break;
        case 3:
          Sound_PlayFar(SE_THUNDER_LOUD);
          break;
        }
      }
      if (flashTimer >= 0) {
        level = flashTimer * RGB_LEVEL_MAX / gCatastrophe_FlashLengths[flash];
        if (--flashTimer == -1) {
          flash++;
        }
      }
      if (level >= 0) {
        u16 *dst = (u16 *)&BG_PLTT_COLOR(6, 0);
        backdropColor = battle->palette;

        for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
          s32 r = RGB_RED(*backdropColor);
          s32 g = RGB_GREEN(*backdropColor);
          s32 b = RGB_BLUE(*backdropColor) + level;

          if (r > RGB_LEVEL_MAX) {
            r = RGB_LEVEL_MAX;
          }
          if (g > RGB_LEVEL_MAX) {
            g = RGB_LEVEL_MAX;
          }
          if (b > RGB_LEVEL_MAX) {
            b = RGB_LEVEL_MAX;
          }
          backdropColor++;
          *dst++ = RGB(r, g, b);
        }
      }
    }
    // The sprites' colours step through these levels from their saved ones,
    // and the backdrop's too from frame 324, when both flash white.
    {
      s32 fadeRed;
      s32 fadeGreen;
      s32 fadeBlue;
      s32 apply[2] = {0, 0};

      fadeRed = 0;
      fadeGreen = 0;
      fadeBlue = 0;
      if (frame == 0) {
        fadeGreen = -RGB_LEVEL_MAX;
        fadeRed = fadeGreen;
        fadeBlue = fadeGreen;
        apply[0] = 1;
      } else if (frame == 106) {
        fadeGreen = -RGB_LEVEL_MAX;
        fadeRed = fadeGreen;
        fadeBlue = -15;
        apply[0] = 1;
      } else if ((u32)(frame - 154) <= 59) {
        s32 h = (frame - 154) / 2;

        fadeGreen = 5 - h * 2;
        fadeRed = fadeGreen;
        fadeBlue = 5 - h;
        apply[0] = 1;
      } else if ((u32)(frame - 226) <= 15) {
        fadeGreen = (frame - 226) / 2 - RGB_LEVEL_MAX;
        fadeRed = fadeGreen;
        fadeBlue = frame - 257;
        apply[0] = 1;
      } else if (frame == 242) {
        fadeRed = -24;
        fadeGreen = -RGB_LEVEL_MAX;
        apply[0] = 1;
      } else if ((u32)(frame - 256) <= 21) {
        s32 d = frame - 256;

        fadeRed = -24 - d * 2;
        fadeGreen = -RGB_LEVEL_MAX - d * 2;
        fadeBlue = -d;
        apply[0] = 1;
      } else if ((u32)(frame - 282) <= 40) {
        s32 d = frame - 282;

        fadeGreen = -d;
        fadeRed = fadeGreen;
        fadeBlue = 16 - d;
        apply[0] = 1;
      } else if (frame == 324) {
        fadeRed = RGB_LEVEL_MAX;
        fadeGreen = RGB_LEVEL_MAX;
        fadeBlue = RGB_LEVEL_MAX;
        apply[0] = 1;
        apply[1] = 1;
      } else if ((u32)(frame - 332) <= 59) {
        fadeGreen = 30 - (frame - 332) / 2;
        apply[0] = 1;
        fadeRed = fadeGreen;
        fadeBlue = fadeGreen;
        apply[1] = 1;
      } else if ((u32)(frame - 489) <= 42) {
        s32 d = frame - 489;

        fadeRed = d / 4;
        fadeGreen = d / 6;
        fadeBlue = d / 16;
        apply[0] = 1;
      }
      for (j = 0; j != 2; j++) {
        if (apply[j] != 0) {
          u16 *src;
          u16 *dst;
          s32 n;

          if (j == 0) {
            src = (u16 *)(work->sheet + CATASTROPHE_SAVED_OBJ_PALETTE + 2);
            n = CATASTROPHE_SAVED_OBJ_COLORS;
            dst = (u16 *)&OBJ_PLTT_COLOR(0, 1);
          } else {
            dst = (u16 *)&BG_PLTT_COLOR(6, 0);
            src = battle->palette;
            n = BATTLE_BACKGROUND_COLORS;
          }
          for (i = 1; i != n; i++) {
            s32 r = RGB_RED(*src) + fadeRed;
            s32 g = RGB_GREEN(*src) + fadeGreen;
            s32 b = RGB_BLUE(*src) + fadeBlue;

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
            *dst++ = RGB(r, g, b);
            src++;
          }
        }
      }
    }
    // The red haze dims by a 64th every other frame, to about half.
    if ((u32)(frame - 554) <= 31 && !((frame - 554) & 1)) {
      s32 dimFrame = frame - 554;

      ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                           FIXED_ONE - dimFrame * (FIXED_ONE / 64),
                           BATTLE_BACKGROUND_COLORS);
    }
    // The top row's pictures stand in for the lowest four's from frame 339 to
    // 450.
    if (swapped == 0) {
      if (frame == 339) {
        swapped = 1;
        for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++) {
          void *tiles =
              (void *)OBJ_TILES(work->sprites[CATASTROPHE_DEMON_SWAPPED + i]);

          Iwram_CopyWords(tiles, cellBuffer + i * OBJ_32X32_8BPP_BYTES,
                          OBJ_32X32_8BPP_BYTES);
        }
      }
    } else if (frame == 450) {
      u8 *cells;

      swapped = 0;
      cells = Ram_MapCellBuffer + CATASTROPHE_DEMON_KEPT_PICTURES;
      for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++) {
        Iwram_CopyWords(
            (void *)OBJ_TILES(work->sprites[CATASTROPHE_DEMON_SWAPPED + i]),
            cells + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
      }
    }
    if (frame <= 593) {
      spriteScale = gCatastrophe_UnitScale;
      place[3] = 0;
      place[1] = toFIXED(255);
      {

        for (i = 0; i != CATASTROPHE_DEMON_COUNT; i++) {
          place[0] = INTtoFIXED(gCatastrophe_DemonX[i]) + toFIXED(8);
          place[2] = INTtoFIXED(gCatastrophe_DemonZ[i]) + camZ;
          if (place[2] > toFIXED(16) - 1) {
            if (place[2] <= toFIXED(168)) {
              Render_ApplyProjectedPlacementFar(work->sprites[i], place,
                                                (s32 *)&spriteScale, 0);
            }
          }
        }
      }
      for (i = 0; i != CATASTROPHE_SHIELD_COUNT; i++) {
        s32 row = i / 2;

        place[0] = INTtoFIXED((i - row * 2) * 32) + drift + toFIXED(72);
        place[2] = INTtoFIXED(row * 32) + camZ + sway + toFIXED(60);
        if (place[2] > toFIXED(16) - 1) {
          if (place[2] <= toFIXED(168)) {
            Render_ApplyProjectedPlacementFar(
                work->sprites[CATASTROPHE_SHIELD + i], place,
                (s32 *)&spriteScale, 0);
          }
        }
      }
      for (i = 0; i != CATASTROPHE_SWORD_COUNT; i++) {
        place[0] = INTtoFIXED(gCatastrophe_ArmourX[i]) - drift + toFIXED(132);
        place[2] =
            INTtoFIXED(gCatastrophe_ArmourZ[i]) + camZ + sway + toFIXED(29);
        if (place[2] > toFIXED(16) - 1) {
          if (place[2] <= toFIXED(168)) {
            Render_ApplyProjectedPlacementFar(
                work->sprites[CATASTROPHE_SWORD + i], place,
                (s32 *)&spriteScale, 0);
          }
        }
      }
      spriteScale.x = -(FIXED_ONE + 1);
      for (i = 0; i != CATASTROPHE_MIRRORED_DEMON_COUNT; i++) {
        place[0] = INTtoFIXED(gCatastrophe_MirroredDemonX[i]) + toFIXED(8);
        place[2] = INTtoFIXED(gCatastrophe_MirroredDemonZ[i]) + camZ;
        if (place[2] > toFIXED(16) - 1) {
          if (place[2] <= toFIXED(168)) {
            Render_ApplyProjectedPlacementFar(
                work->sprites[CATASTROPHE_MIRRORED_DEMON + i], place,
                (s32 *)&spriteScale, 0);
          }
        }
      }
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    shade = 0;
    texture.logHeight = 6;
    model->mesh = gCatastrophe_ForkMesh;
    model->texture = &texture;
    model->kind = MODEL_SHADED_MAX;
    model->vertices = verts;
    // Forks of lightning flash in the sky, each fading over eight frames.
    for (i = 0; i != 7; i++) {
      s32 st = gCatastrophe_ForkStarts[i];

      if (frame >= st && frame < st + 8) {
        s32 c = gCatastrophe_ForkPictures[i];

        shade = (st - frame) << 3;
        model->shade = shade;
        texture.pixels = work->sheet + c * PICTURE_BYTES(LIGHTNING_FORK);
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(gCatastrophe_ForkX[i] - 48),
                         INTtoFIXED(gCatastrophe_ForkY[i] - 44), 0);
        Matrix_RotateZ(0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 489) {
      for (i = 0; i != CATASTROPHE_STRIKES; i++) {
        work->particles[i].variant = 0;
      }
    }
    for (j = CATASTROPHE_STRIKES; j != ARRAY_COUNT(work->particles); j++) {
      p = &work->particles[j];
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                FIXED_INTEGER_PART(p->x) - n / 2, FIXED_INTEGER_PART(p->y) - n,
                n, n * 2);
        Particle_AdvanceWithGravity2D(p, 63, 0);
        p->variant--;
      }
    }
    {
      for (i = 0; i != CATASTROPHE_CANVAS_FLASHES; i++) {
        u16 t = gCatastrophe_CanvasFlashFrames[i];

        if (frame == t + 242) {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                              CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
        }
        if (frame == t + 243) {
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        }
      }
    }
    if ((u32)(frame - 537) <= 55) {
      if (frame & 1) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                            CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
      } else {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      }
    }
    if (frame == 324) {
      Sound_PlayFar(SE_CURSE_FALL);
    }
    if (frame == 560) {
      Sound_PlayFar(SE_SHIMMER);
    }
    if (frame == 602) {
      Sound_PlayFar(SE_CRASH);
    }
    texture.logWidth = 5;
    texture.logHeight = 7;
    model->shade = shade;
    model->kind = MODEL_SHADED_MAX;
    model->mesh = gBattleEffect_ColumnMesh;
    model->texture = &texture;
    model->vertices = verts;
    // From frame 489 three strikes step through their tables at half speed:
    // each step turns its jagged streak, plays its sound and throws sparks.
    if ((u32)(frame - 489) <= 130) {
      shade = 0;
      for (i = 0; i != 3; i++) {
        s32 off;
        s32 idx;

        p = &work->particles[i];
        off = i * CATASTROPHE_STRIKE_STEPS;
        idx = p->variant;

        if (frame ==
            (gCatastrophe_StrikeFrames[i * (CATASTROPHE_STRIKE_STEPS + 1) +
                                       idx] -
             492) / 2 +
                489) {
          s32 count = 0;
          const s16 *table = gCatastrophe_StrikeRolls;
          s32 n = off + idx;
          s32 k = n * 2;
          s32 a = *(const s16 *)((s32)table + k);
          u8 *src;
          s32 bx;
          s32 height;
          s32 width;
          s32 x;
          s32 x0;
          s32 y0;
          s32 y;

          if (a != 0) {
            SET_POSITION(p, gCatastrophe_StrikeX[n] << 3,
                         gCatastrophe_StrikeY[n] << 3, a << 8);
          }
          p->velocityY = gCatastrophe_StrikeSpeedsY[p->variant + off];
          p->velocityZ = gCatastrophe_StrikeTurns[p->variant + off];
          p->velocityX = gCatastrophe_StrikePictures[p->variant + off];
          p->variant++;
          if (p->velocityX == -1) {
            continue;
          }
          width = 10;
          height = 20;
          src = sprites + gBattleEffect_MotePictures[MOTE_PICTURES - 1];
          x0 = p->x / 8 - 5;
          bx = span - drift;
          x = x0 + (FIXEDtoINT(bx / 2));
          y0 = p->y / 8 - 10;
          y = y0 + (FIXEDtoINT(camZ + sway));
          blit[0](canvas, src, x, y, width, height);
          switch (i) {
          case 0:
            Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
            break;
          case 1:
            Sound_PlayFar(SE_FALLING_LIGHT);
            break;
          case 2:
            Sound_PlayFar(SE_WHIRLWIND);
            break;
          }
          {
            s32 half;
            s32 j;

            j = CATASTROPHE_STRIKES;
            q = &work->particles[CATASTROPHE_STRIKES];
            half = (bx + (s32)((u32)bx >> 31)) >> 1;
            for (; j != ARRAY_COUNT(work->particles); j++, q++) {
              if (q->variant < 0) {
                s32 angle;
                s32 speed;

                q->x = p->x * (FIXED_ONE / 8) + half;
                q->y = p->y * (FIXED_ONE / 8) + camZ + sway;
                angle = Random16();
                speed = (Random16() & 255) + 127;
                q->velocityX = Trig_Sin(angle) * speed >> 6;
                q->velocityY = Trig_Cos(angle) * speed >> 6;
                q->variant = (Random16() & 7) + 16;
                if (++count == CATASTROPHE_STRIKE_SPARKS) {
                  break;
                }
              }
            }
          }
        }
        if (p->velocityX != -1) {
          s32 cell = p->velocityX;

          model->shade = shade;
          texture.pixels = work->sheet + CATASTROPHE_JAGGED_STREAKS +
                           cell * PICTURE_BYTES(JAGGED_STREAK);
          Matrix_Initialize();
          {
            s32 t = p->x / 8;
            s32 b = span - drift;

            Matrix_Translate(
                INTtoFIXED(t - CANVAS_SIZE / 2) + b / 2,
                INTtoFIXED(p->y / 8 - CANVAS_SIZE / 2) + camZ + sway, 0);
          }
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(p->z);
          Matrix_Scale(FIXED_ONE * 4, 85536, FIXED_ONE / 2);
          Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
          Render_DrawModelList(model);
          p->y += p->velocityY * 2;
          p->z += p->velocityZ * 2;
        }
      }
    }
    if (frame == 602) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == 630) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
    }
    if (frame == 634) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
    }
    if ((u32)(frame - 552) <= 43) {
      p = work->particles;
      if (!(frame & 1)) {
        p->y = (Random16() & 0x3fff) + toFIXED(0.375);
        p->z = Random16() % 3 * PICTURE_BYTES(JAGGED_STREAK) +
               CATASTROPHE_JAGGED_STREAKS;
      }
      texture.pixels = work->sheet + p->z;
      Matrix_Initialize();
      Matrix_Translate((span - drift) / 2 + toFIXED(4), camZ + toFIXED(204), 0);
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
      Matrix_RotateZ(p->y);
      Matrix_Scale(FIXED_ONE * 4, 85536, FIXED_ONE / 2);
      Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
      Render_DrawModelList(model);
    }
    if ((u32)(frame - 602) <= 15 || (u32)(frame - 630) <= 3) {
      s32 n = 1;
      s32 sx = FIXED_ONE * 5;
      s32 base = Random16() % 3;
      s32 spread = 8;

      if (frame > 611) {
        sx -= (frame - 612) * 61845;
      }
      if (sx < FIXED_ONE) {
        sx = FIXED_ONE;
      }
      if (frame <= 613) {
        if (frame > 605) {
          n = 3;
        } else if (frame > 603) {
          n = 2;
        }
      }
      model->kind = MODEL_SHADED_MAX;
      model->shade = shade;
      for (i = 0; i != n; i++) {
        struct Particle *p4 = &work->particles[i];

        SET_POSITION(p4, INTtoFIXED(Random16() % spread - spread / 2),
                     camZ + toFIXED(200),
                     (base + i) % 3 * PICTURE_BYTES(JAGGED_STREAK) +
                         CATASTROPHE_JAGGED_STREAKS);
        texture.pixels = work->sheet + p4->z;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(1), p4->y, 0);
        Matrix_Scale(sx, FIXED_ONE, FIXED_ONE / 2);
        Matrix_Translate(p4->x, 0, 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE * 2, FIXED_ONE);
        Matrix_RotateZ(DEGtoANG(180));
        Render_TransformVertices(gBattleEffect_LongBeamCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 552) {
      Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
                                 Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
    }
    if (frame == 551) {
      Resource_LoadAndDecompress(RESOURCE_TRIANGLE_SEAL_TEXTURE, work->sheet, 0,
                                 0);
      Resource_LoadAndDecompress(RESOURCE_PENTAGON_SEAL_TEXTURE,
                                 work->sheet + CATASTROPHE_PENTAGON_SEAL, 0, 0);
    }
    if (frame == 602) {
      Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                                 Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
    }
    if ((u32)(frame - 602) <= 6) {
      camZ += toFIXED(16);
    }
    // From frame 552 the seal: the pentagon spins as it swells, its triangles
    // fold up from its edges from frame 580, and the swirl spins over it.
    {
      s32 sealFrame = frame - 552;

      if ((u32)sealFrame <= 58) {
        s32 s = sealFrame * 4424;
        s32 a;
        s32 b;

        shade = -16;
        if (s > CATASTROPHE_SEAL_SCALE_MAX) {
          s = CATASTROPHE_SEAL_SCALE_MAX;
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->shade = shade;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &texture;
        model->mesh = gBattleEffect_SquareMesh;
        model->vertices = verts;
        texture.pixels = work->sheet + CATASTROPHE_PENTAGON_SEAL;
        Matrix_Initialize();
        b = span - drift;
        Matrix_Translate(b / 2 + toFIXED(5), camZ + toFIXED(230), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateX(CATASTROPHE_SEAL_TILT);
        a = -frame * (TRIG_TURN / 64);
        Matrix_RotateZ(a);
        Matrix_ScaleUniform(s);
        Matrix_Translate(0, -toFIXED(15), 0);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Matrix_Translate(0, toFIXED(15), 0);
        Render_DrawModelList(model);
        texture.pixels = work->sheet;
        for (i = 0; i != 3; i++) {
          Matrix_Push();
          Matrix_RotateZ(i * (DEGtoANG(120)) + DEGtoANG(60));
          {
            s32 phase = i * (DEGtoANG(120)) + DEGtoANG(60);
            model->shade = -24 - (FIXEDtoINT(Trig_Cos(phase + a) << 3));
          }
          Matrix_Translate(0, -toFIXED(16), 0);
          if (frame > 579) {
            s32 v = (frame - 580) * 1096;

            if (v > CATASTROPHE_SEAL_SIDE_TILT) {
              v = CATASTROPHE_SEAL_SIDE_TILT;
            }
            Matrix_RotateX(-v);
          }
          Render_TransformVertices(gBattleEffect_StandingSquareCorners, verts,
                                   4);
          Render_DrawModelList(model);
          Matrix_Pop();
        }
        if (frame > 589) {
          s += (frame - 590) * 11060;
        }
        if (s > 0 && frame <= 601) {
          shade = -16;
          if (frame > 589) {
            shade -= (frame - 590) * 2;
          }
          if (shade > MODEL_SHADE_INVISIBLE) {
            model->shade = shade;
            texture.pixels = Ram_MapCellBuffer + CATASTROPHE_RING;
            Matrix_Initialize();
            if (frame > 553) {
              s32 v = (554 - frame) * CATASTROPHE_SWIRL_RISE;

              if (v < -CATASTROPHE_SWIRL_RISE_FRAMES * CATASTROPHE_SWIRL_RISE) {
                v = -CATASTROPHE_SWIRL_RISE_FRAMES * CATASTROPHE_SWIRL_RISE;
              }
              Matrix_Translate(0, v, 0);
            }
            Matrix_Translate(((b + (s32)((u32)b >> 31)) >> 1) + toFIXED(5),
                             camZ + toFIXED(220), 0);
            Matrix_Scale(FIXED_ONE, 32000, FIXED_ONE);
            Matrix_RotateZ(frame * (TRIG_TURN / 32));
            Matrix_ScaleUniform(s);
            Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
    }
    // From frame 602 a ring of fire spreads, fading from frame 612.
    if (frame > 601) {
      s32 s = (frame - 602) * CATASTROPHE_RING_GROWTH + toFIXED(0.125);

      shade = 0;
      if (frame > 611) {
        shade -= (frame - 612) * 8;
      }
      if (shade > MODEL_SHADE_INVISIBLE) {
        model->shade = shade;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->kind = MODEL_SHADED_ADDITIVE;
        texture.pixels = Ram_MapCellBuffer + CATASTROPHE_RING;
        texture.logWidth = 7;
        texture.logHeight = 7;
        Matrix_Initialize();
        Matrix_Translate(toFIXED(1), -toFIXED(16), 0);
        Matrix_Scale(FIXED_ONE, 32000, FIXED_ONE);
        Matrix_RotateZ(frame * (TRIG_TURN / 64));
        Matrix_ScaleUniform(s);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    // The orb swells, rising a little, and from frame 601 it falls.
    if ((u32)(frame - 554) <= 51) {
      s32 radius = frame - 553;
      s32 y = 90 - (frame - 554) / 2;

      if (frame > 600) {
        y += (frame - 601) * 16;
      }
      if (y <= 63) {
        y = 64;
      }
      if (radius <= 38) {
        BattleEffect_DrawShadedCircles(cellBuffer, radius,
                                       CATASTROPHE_ORB_BRIGHTNESS);
      } else {
        radius = 38;
      }
      Canvas_BlitAtFoot(cellBuffer, 62, y, radius);
    }
    EFFECT_PRESENT_CANVAS(work);
    frame++;
  }
  for (i = 0; i != CATASTROPHE_SPRITES; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  ActorShadow_LoadTilesFar();
  battle->arrowTiles = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->args);
  for (i = 0; i != args->targetCount; i++) {
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_GRB, TINT_KEEP,
                             TINT_KEEP, 0);
    BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_NORMAL, TINT_KEEP,
                             TINT_KEEP, 0);
  }
  Palette_LoadBasicSpriteColors();
  BattleEffect_SetupBlendedDisplay();
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  work->fadeFrames = 0;
  work->fadeStep = 0;
  // Act two: the land seen from the sky, where the orb falls and bursts.
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  Palette_BrightenSavedBattleBg(0, -16, -16);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, cellBuffer, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                             work->sheet + CATASTROPHE_SWEEP_PICTURES, 1, 0);
  Iwram_ClearWordsEntry(work->sheet + CATASTROPHE_BOLT_TEXTURES,
                        CATASTROPHE_BOLT_TEXTURE_COUNT *
                            PICTURE_BYTES(BOLT_TEXTURE));
  for (i = 0; i != CATASTROPHE_BOLT_TEXTURE_COUNT; i++) {
    {
      s32 y;
      s32 boltTexture = i * PICTURE_BYTES(BOLT_TEXTURE);
      u8 *cells = cellBuffer;

      for (y = 0; y != LIGHTNING_BOLT_HEIGHT; y++) {
        u8 *line = (u8 *)work + (boltTexture + y * BOLT_TEXTURE_WIDTH);
        s32 offset =
            i * LIGHTNING_BOLT_PICTURE_BYTES + y * LIGHTNING_BOLT_WIDTH;
        u8 *dst = line + (offsetof(struct BattleEffectWork, sheet) +
                          CATASTROPHE_BOLT_TEXTURES +
                          (BOLT_TEXTURE_WIDTH - LIGHTNING_BOLT_WIDTH) / 2);
        u8 *src = cells + offset;
        s32 x;

        for (x = 0; x != LIGHTNING_BOLT_WIDTH; x++) {
          *dst++ = *src++;
        }
      }
    }
  }
  Resource_LoadAndDecompress(RESOURCE_TRIANGLE_SEAL_TEXTURE, cellBuffer, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PENTAGON_SEAL_TEXTURE,
                             Ram_MapCellBuffer + CATASTROPHE_PENTAGON_SEAL, 0,
                             0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                             Ram_MapCellBuffer + CATASTROPHE_RING, 0, 0);
  {
    FIXED rise;
    s32 top;
    void *aerialVertices;
    s32 aerialShade;
    s32 radius;
    s32 ry;

    rise = 0;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    frame = 0;
    if (!(gInput.repeat & (KEY_A | KEY_B))) {
      do {
        if (frame == 0) {
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES, 0);
          rise = -toFIXED(64);
          Palette_BrightenSavedBattleBg(0, -16, -16);
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
          Iwram_CopyWords((void *)BG_PLTT,
                          Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_F),
                          SHEET_PALETTE_BYTES);
        }
        if (rise <= toFIXED(16) - 1) {
          rise += toFIXED(1);
        }
        {
          s32 launchFrame = frame - 96;

          if ((u32)launchFrame <= 23) {
            Palette_BrightenSavedBattleBg(-launchFrame, -16, frame - 112);
            work->shakeFrames = 8;
          }
        }
        if (frame > 139) {
          work->transferMode = CANVAS_COPY_AND_CLEAR;
          if (frame > 153) {
            work->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
          } else {
            work->transferValue = CANVAS_FILL_WORD(CATASTROPHE_WASH_LEVEL);
          }
          Palette_BrightenBgEntries(1, 1, 1);
        }
        if (frame == 80 || frame == 83) {
          u16 *c = (u16 *)&BG_PLTT_COLOR(6, 0);

          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++) {
            *c ^= RGB_WHITE;
          }
        }
        if (frame == 82) {
          work->shakeFrames = 8;
          Sound_PlayFar(SE_BANE_ESCAPE);
          IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        }
        if (frame == 96) {
          Sound_PlayFar(SE_LAUNCH);
        }
        if (frame == 128) {
          u16 *c;

          work->shakeFrames = 16;
          c = (u16 *)&BG_PLTT_COLOR(6, 0);
          for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++) {
            *c ^= RGB_WHITE;
          }
        }
        if (frame > 128) {
          s32 d = (frame - 128) << 1;

          Palette_BrightenSavedBattleBg(d - 11, frame - 136, 3 - d);
        }
        radius = 16;
        ry = FIXEDtoINT(rise);
        if (frame > 92) {
          radius = frame - 75;
        }
        if (frame <= 79) {
          radius += FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) * 2);
        }
        if (radius <= 64) {
          BattleEffect_DrawShadedCircles(work->sheet, radius,
                                         CATASTROPHE_ORB_BRIGHTNESS);
        } else {
          radius = 64;
        }
        top = ry - radius / 2 + CATASTROPHE_ORB_CENTER;
        Canvas_BlitAtFoot(work->sheet, CATASTROPHE_ORB_CENTER, top, radius);
        // From frame 96 glints sparkle around the orb.
        if (frame > 95) {
          {
            for (i = 0; i != 4; i++) {
              s32 n = i & 3;
              s32 angle = Random16();
              s32 x = (FIXEDtoINT(Trig_Sin(angle) * radius) >> 1) +
                      CATASTROPHE_ORB_CENTER;
              s32 y;
              u32 flip;

              x -= gBattleEffect_GlintPictureWidths[n] >> 1;
              y = top + (FIXEDtoINT(Trig_Cos(angle) * radius) >> 1) -
                  (gBattleEffect_GlintPictureHeights[n] >> 1);
              flip = Random16();
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  gCatastrophe_GlintFlips[flip & 3] |
                                      EFFECT_CLIP_BOUNDS,
                                  BLITTER_ADDITIVE);
              ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
                  canvas,
                  work->sheet + CATASTROPHE_SWEEP_PICTURES +
                      gBattleEffect_GlintPictures[n],
                  x, y, gBattleEffect_GlintPictureWidths[n],
                  gBattleEffect_GlintPictureHeights[n]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
          }
        }
        aerialVertices = Heap_AllocEwram(512);
        aerialModel = Render_AllocateModelList(1);
        // Until frame 127 the seal turns under the orb, swelling from frame 72,
        // its triangles folding down from frame 64.
        if (frame <= 127) {
          s32 s = CATASTROPHE_AERIAL_SEAL_SCALE;
          s32 a;

          if (frame > 71) {
            s = (frame - 72) * (FIXED_ONE / 32) + CATASTROPHE_AERIAL_SEAL_SCALE;
          }
          aerialShade = -16;
          if (frame > 119) {
            aerialShade = 944 - frame * 8;
          }
          if (s > CATASTROPHE_SEAL_SCALE_MAX) {
            s = CATASTROPHE_SEAL_SCALE_MAX;
          }
          aerialTexture.logWidth = 6;
          aerialTexture.logHeight = 6;
          aerialModel->shade = aerialShade;
          aerialModel->kind = MODEL_SHADED_MAX;
          aerialModel->texture = &aerialTexture;
          aerialModel->mesh = gBattleEffect_SquareMesh;
          aerialModel->vertices = aerialVertices;
          aerialTexture.pixels = Ram_MapCellBuffer + CATASTROPHE_PENTAGON_SEAL;
          Matrix_Initialize();
          Matrix_Translate(-toFIXED(2), rise, 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_RotateX(CATASTROPHE_SEAL_TILT);
          a = frame * (TRIG_TURN / 128);
          Matrix_RotateZ(a);
          Matrix_ScaleUniform(s);
          Matrix_Translate(0, -toFIXED(15), 0);
          Render_TransformVertices(gBattleEffect_SquareCorners, aerialVertices,
                                   4);
          Matrix_Translate(0, toFIXED(15), 0);
          Render_DrawModelList(aerialModel);
          aerialTexture.pixels = cellBuffer;
          for (i = 0; i != 3; i++) {
            s32 k = i * (DEGtoANG(120)) + DEGtoANG(60);
            s32 sideShade =
                aerialShade + (FIXEDtoINT(Trig_Cos(a + k) << 3)) - 16;
            s32 v;

            Matrix_Push();
            Matrix_RotateZ(i * (DEGtoANG(120)) + DEGtoANG(60));
            aerialModel->shade = sideShade;
            Matrix_Translate(0, -toFIXED(16), 0);
            v = CATASTROPHE_SEAL_SIDE_TILT;
            if (frame > 63) {
              v -= (frame - 64) * (TRIG_TURN / 64);
            }
            if (v < 0) {
              v = 0;
            }
            Matrix_RotateX(-v);
            Render_TransformVertices(gBattleEffect_StandingSquareCorners,
                                     aerialVertices, 4);
            Render_DrawModelList(aerialModel);
            Matrix_Pop();
          }
        }
        // Two rings of fire spread from frames 80 and 88, the second at half
        // the speed.
        for (i = 0; i != 2; i++) {
          s32 delay = i * 8;
          s32 start = delay + 80;
          s32 s;

          if (frame < start) {
            continue;
          }
          aerialShade = 0;
          if (i == 0) {
            s = (frame - start) * CATASTROPHE_RING_GROWTH + toFIXED(0.125);
            if (frame >= delay + 96) {
              aerialShade = (start - frame) * 8 + 128;
            }
          } else {
            s = (frame - start) * (CATASTROPHE_RING_GROWTH / 2) +
                toFIXED(0.125);
            if (frame >= delay + 112) {
              aerialShade = (start - frame) * 8 + 256;
            }
          }
          if (aerialShade > MODEL_SHADE_INVISIBLE) {
            aerialModel->shade = aerialShade;
            aerialModel->mesh = gBattleEffect_LargeSquareMesh;
            aerialModel->kind = MODEL_SHADED_MAX;
            aerialTexture.pixels = Ram_MapCellBuffer + CATASTROPHE_RING;
            aerialTexture.logWidth = 7;
            aerialTexture.logHeight = 7;
            Matrix_Initialize();
            Matrix_Translate(0, toFIXED(16), 0);
            Matrix_Scale(FIXED_ONE, 32000, FIXED_ONE);
            Matrix_RotateZ(frame * (TRIG_TURN / 64));
            Matrix_ScaleUniform(s);
            Render_TransformVertices(gBattleEffect_SquareCorners,
                                     aerialVertices, 4);
            Render_DrawModelList(aerialModel);
          }
        }
        aerialTexture.logWidth = 5;
        aerialTexture.logHeight = 7;
        aerialModel->kind = MODEL_SHADED_MAX;
        aerialModel->mesh = gBattleEffect_ColumnMesh;
        shade = 0;
        aerialModel->shade = shade;
        if (frame == 0) {
          p = work->particles;
          for (i = 0; i != CATASTROPHE_BOLTS; p++, i++) {
            p->z = gCatastrophe_BoltRolls[i];
            p->velocityZ = (Random16() & 255) + 128;
            if (Random16() & 1) {
              p->velocityZ = -p->velocityZ;
            }
            p->variant = Random16() & 3;
          }
        }
        for (i = 0; i != 8; i++) {
          if (frame == 128 + i * 4) {
            Sound_PlayFar(SE_CLATTER);
          }
        }
        // The bolts shoot out from the middle, one a frame from frame 128, each
        // turning as it grows.
        p = work->particles;
        for (i = 0; i != CATASTROPHE_BOLTS; i++, p++) {
          s32 startFrame = i + 128;

          if (frame >= startFrame && frame < i + 144) {
            s32 s = (frame - startFrame) * (FIXED_ONE / 8);

            if (s > FIXED_ONE) {
              s = FIXED_ONE;
            }
            aerialTexture.pixels = work->sheet + CATASTROPHE_BOLT_TEXTURES +
                                   p->variant * PICTURE_BYTES(BOLT_TEXTURE);
            Matrix_Initialize();
            Matrix_RotateZ(p->z);
            p->z += p->velocityZ;
            Matrix_Scale(s << 2, s, FIXED_ONE);
            Render_TransformVertices(gBattleEffect_TaperedBeamCorners,
                                     aerialVertices, 4);
            Render_DrawModelList(aerialModel);
          }
        }
        Heap_Release(aerialModel);
        Heap_Release(aerialVertices);
        // At frame 134 sparks ring the orb and twinkle out.
        if (frame > 133) {
          if (frame == 134) {
            struct Particle *p;

            i = 0;
            p = &work->particles[CATASTROPHE_BOLTS];
            for (; i != CATASTROPHE_SPARKS; i++, p++) {
              s32 random = Random16();
              s32 angle = i * (TRIG_TURN / CATASTROPHE_SPARKS);
              s32 distance = (random & 31) + 32;

              SET_POSITION(p,
                           (FIXEDtoINT(Trig_Sin(angle) * distance)) +
                               CATASTROPHE_ORB_CENTER,
                           (FIXEDtoINT(Trig_Cos(angle) * distance)) +
                               CATASTROPHE_ORB_CENTER,
                           (Random16() & 3) + 5);
              p->variant = -(Random16() & 15);
            }
          }
          i = 0;
          spark = &work->particles[CATASTROPHE_BOLTS];
          for (; i != CATASTROPHE_SPARKS; i++, spark++) {
            if ((u32)spark->variant <= 7) {
              s32 n = (FIXEDtoINT(Trig_Sin(spark->variant * (TRIG_TURN / 16)) *
                                  spark->z)) +
                      1;

              blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                      spark->x - n / 2, spark->y - n, n, n * 2);
            }
            spark->variant++;
          }
        }
        if (work->shakeFrames > 0) {
          s32 m = 7;
          s32 a = Random16();
          s32 b = Random16();

          a &= m;
          b &= m;
          a -= 4;
          b += BATTLE_BACKGROUND_SCROLL_Y - 4;
          gBgScroll[1].x = a;
          gBgScroll[1].y = b;
          work->shakeFrames--;
        } else {
          gBgScroll[1].x = 0;
          gBgScroll[1].y = work->savedBg1Y;
        }
        EFFECT_PRESENT_CANVAS(work);
        frame++;
      } while (frame != 160 && !(gInput.repeat & (KEY_A | KEY_B)));
    }
  }
  {
    FIXED slide;
    s32 slideStep;
    u8 *bolts;
    void *quads = Ram_MapCellBuffer + CATASTROPHE_SWEEP_VERTICES;
    void *strip = Ram_MapCellBuffer + CATASTROPHE_SWEEP_STRIP;
    struct Particle *bursts = CATASTROPHE_PARTICLES;
    struct Particle *debris = &CATASTROPHE_PARTICLES[CATASTROPHE_BURSTS];

    // Act three: back on the battlefield, lightning and smoke sweep the foes.
    bolts = Ram_MapCellBuffer + CATASTROPHE_BOLT_PICTURES;
    slide = 0;
    slideStep = 0;
    Iwram_FillWords(canvas, CANVAS_BYTES,
                    CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    EFFECT_PRESENT_CANVAS(work);
    gBgScroll[1].x = savedScroll;
    BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background,
                             BACKGROUND_DARKENED_LEVEL);
    Palette_BrightenSavedBattleBg(-16, -16, -4);
    Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, work->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
                               work->sheet + CATASTROPHE_SWEEP_PICTURES, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               work->sheet + CATASTROPHE_FLASH_BURST_PICTURES,
                               1, 0);
    Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, bolts, 1, 0);
    Render_BuildQuadStrip(strip, 4, 32, 11);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    frame = 0;
    do {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 2 && frame <= 87) {
        frame = 88;
      }
      if (frame == 88) {
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        gBgScroll[1].x = savedScroll;
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
        work->scrollStepX = 0;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      if (frame == 0) {
        IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
                         CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        work->scrollStepX = -CATASTROPHE_PAN_SPEED;
        gProjection.centerX = BATTLE_PROJECTION_CENTER_X - CATASTROPHE_PAN;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
        gBgScroll[1].x = savedScroll + CATASTROPHE_PAN;
        Graphics_LoadGreyRamp();
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        for (i = 0; i != CATASTROPHE_BURSTS + CATASTROPHE_DEBRIS; i++) {
          CATASTROPHE_PARTICLES[i].variant = -1;
        }
        slide = toFIXED(128);
        slideStep = 0;
      }
      if (frame == 16) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                            CANVAS_FADE_THREE_QUARTERS);
      }
      if ((u32)(frame - 16) <= 47) {
        BattleEffect_StepPaletteToResource(RESOURCE_VIOLET_PALETTE_F);
      }
      // At frame 66 the targets are struck: thrown back, flashing white, debris
      // flying from each.
      if (frame == 66) {
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(4),
                                        toFIXED(8), toFIXED(4), 150);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
          {
            s32 a = args->targets[i];
            s32 k = CATASTROPHE_TARGET_DEBRIS * sizeof(struct Particle);
            EffectPosition_ProjectBattler(a, &pos);
            j = 0;
            q = (struct Particle *)(i * k + (s32)debris);
          }
          for (; j != CATASTROPHE_TARGET_DEBRIS; j++, q++) {
            s32 angle;
            s32 speed;

            q->x = INTtoFIXED(pos.x / 2);
            q->y = toFIXED(80);
            angle = (Random16() & 0x7fff) + DEGtoANG(90);
            speed = (Random16() & 127) + 128;
            q->velocityX = Trig_Sin(angle) * speed >> 5;
            q->velocityY = Trig_Cos(angle) * speed >> 4;
            q->variant = CATASTROPHE_DEBRIS_LIFE;
          }
        }
        work->shakeFrames = 16;
        Sound_PlayFar(SE_CRASH);
      }
      if ((u32)(frame - 72) <= 15) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN,
                            CANVAS_FILL_WORD(1));
        Palette_BrightenBgEntries(1, 1, 1);
      }
      if (frame >= 0) {
        s32 x = 0;

        if (frame > 71) {
          x -= (frame - 72) * 8;
        }
        for (i = 0; i != 2; i++) {
          s32 sum = x + 116;
          struct Particle *base =
              (struct Particle *)((frame & 31) * (2 * sizeof(struct Particle)) +
                                  (s32)bursts);
          struct Particle *q = &base[i];
          s32 angle;
          s32 speed;

          q->x = INTtoFIXED(sum);
          q->y = INTtoFIXED(Random16() % 112 + 4);
          angle = (Random16() & 0x7fff) + DEGtoANG(180);
          speed = (Random16() & 127) + 128;
          q->velocityX = Trig_Sin(angle) * speed >> 6;
          q->velocityY = Trig_Cos(angle) * speed >> 6;
          q->variant = 0;
        }
        burst = CATASTROPHE_PARTICLES;
        for (i = 0; i != CATASTROPHE_BURSTS; i++, burst++) {
          if ((u32)burst->variant < CATASTROPHE_BURST_LIFE) {
            blit[0](canvas,
                    work->sheet + CATASTROPHE_FLASH_BURST_PICTURES +
                        burst->variant / 2 * PICTURE_BYTES(FLASH_BURST),
                    FIXED_INTEGER_PART(burst->x) - FLASH_BURST_WIDTH / 2,
                    FIXED_INTEGER_PART(burst->y) - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
            Particle_AdvanceWithGravity2D(burst, 63, 0);
            burst->variant++;
          }
        }
      }
      if (frame <= 87) {
        s32 off;
        s32 jitter = 0;
        s32 z;
        s32 y;
        s32 a;
        s32 b;
        s32 c;

        j = 0;
        z = 0;
        y = 0;
        for (; j != CATASTROPHE_SWEEP_ROWS; j++) {
          struct MeshVertex *v;
          s32 x;

          off = j * rowStep;

          for (i = 0, x = 0, v = (struct MeshVertex *)(off + (s32)cellBuffer);
               i != 2; i++, x += 10, v++) {
            SET_POSITION(
                v,
                x +
                    (FIXEDtoINT(Trig_Sin((frame + j * 2) * (TRIG_TURN / 64)) *
                                2)) +
                    jitter - 5,
                y, z);
          }
          jitter = jitter + (Random16() & 7) - 4;
          y -= 6;
        }
        sweepModel = Render_AllocateModelList(1);
        sweepModel->kind = MODEL_SHADED_MAX;
        sweepModel->texture = &sweepTexture;
        sweepModel->shade = 0;
        sweepModel->scrollV = 0;
        sweepModel->vertices = quads;
        if (frame == 16) {
          slideStep = -toFIXED(8);
        }
        if ((u32)(frame - 48) <= 5) {
          slideStep = toFIXED(8);
        }
        if (frame > 59) {
          slideStep = -toFIXED(16);
        }
        if (frame > 31) {
          slideStep = slideStep * 48 / DAMPING_SCALE;
        }
        slide += slideStep;
        sweepTexture.logWidth = 6;
        sweepTexture.logHeight = 5;
        sweepTexture.pixels = work->sheet;
        sweepModel->mesh = strip;
        sweepModel->scrollU = (frame << 3) & TEXTURE_SCROLL_MASK;
        Matrix_Initialize();
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        a = frame * CATASTROPHE_SWEEP_WAVE;
        b = frame * (TRIG_TURN / 8);
        c = Trig_Sin(a);
        Matrix_Translate(slide, (c << 5) + Trig_Cos(b) * 24, 0);
        Matrix_ScaleUniform(CATASTROPHE_SWEEP_SCALE);
        Matrix_Push();
        Matrix_RotateZ(DEGtoANG(90));
        Matrix_ScaleUniform(CATASTROPHE_BAND_SCALE);
        Render_TransformVertices(cellBuffer, quads, 24);
        Render_DrawModelList(sweepModel);
        Matrix_Pop();
        sweepTexture.logWidth = 6;
        sweepModel->mesh = gBattleEffect_SquareMesh;
        sweepTexture.logHeight = 6;
        sweepModel->scrollU = 0;
        sweepTexture.pixels = work->sheet + CATASTROPHE_SWEEP_PICTURES;
        Matrix_Translate(-toFIXED(2),
                         (Trig_Sin(frame * (TRIG_TURN / 64)) << 2) - toFIXED(6),
                         0);
        Matrix_RotateZ(-TRIG_TURN / 32);
        {
          FIXED s0 = Trig_Sin(a);
          s32 y0 = (s0 << 5) + Trig_Cos(b) * 24;
          FIXED s1;
          FIXED y1;

          a += CATASTROPHE_SWEEP_WAVE;
          s1 = Trig_Sin(a);
          b += DEGtoANG(45);
          y1 = (s1 << 5) + Trig_Cos(b) * 24;
          Matrix_RotateZ((y0 - y1) / 512 - TRIG_TURN / 64);
        }
        Matrix_Scale(135536, 105536, FIXED_ONE);
        Render_TransformVertices(gBattleEffect_SquareCorners, quads, 4);
        Render_DrawModelList(sweepModel);
        Heap_Release(sweepModel);
        if (frame <= 87) {

          // Six bolts strike, one every eight frames from frame 16, each with
          // a clap, a shake and debris thrown up where it lands.
          i = 0;
          bolt = work->particles;
          {

            for (; i != 6; bolt++, i++) {
              s32 strikeFrame = i * 8 + 16;

              if (frame == strikeFrame) {
                bolt->x = (Random16() & 31) + 64;
                bolt->variant = Random16();
                work->shakeFrames = 4;
                switch (i & 3) {
                case 0:
                  Sound_PlayFar(SE_TOPPLE);
                  break;
                case 1:
                  Sound_PlayFar(SE_THUNDER);
                  break;
                case 2:
                  Sound_PlayFar(SE_BANE_ESCAPE);
                  break;
                case 3:
                  Sound_PlayFar(SE_THUNDER_LOUD);
                  break;
                }
                j = 0;
                q = (struct Particle *)((i & 7) * (CATASTROPHE_BOLT_DEBRIS *
                                                   sizeof(struct Particle)) +
                                        (s32)debris);
                for (; j != CATASTROPHE_BOLT_DEBRIS; j++, q++) {
                  s32 angle;
                  s32 speed;

                  q->x = INTtoFIXED(bolt->x);
                  q->y = toFIXED(104);
                  angle = (Random16() & 0x7fff) + DEGtoANG(90);
                  speed = (Random16() & 127) + 128;
                  q->velocityX = Trig_Sin(angle) * speed >> 5;
                  q->velocityY = Trig_Cos(angle) * speed >> 4;
                  q->variant = CATASTROPHE_DEBRIS_LIFE;
                }
              }
              if (frame >= strikeFrame && frame < strikeFrame + 8) {
                u8 *src = Ram_MapCellBuffer + CATASTROPHE_BOLT_PICTURES;

                blit[0](canvas,
                        ((bolt->variant / 2) & 3) *
                                PICTURE_BYTES(LIGHTNING_BOLT) +
                            src,
                        bolt->x - LIGHTNING_BOLT_WIDTH / 2, 0,
                        LIGHTNING_BOLT_WIDTH, LIGHTNING_BOLT_HEIGHT);
                bolt->variant++;
              }
            }
          }
          {
            struct Particle *d = debris;

            for (i = 0; i != CATASTROPHE_DEBRIS; i++, d++) {
              if (d->variant >= 0) {
                s32 n = (d->variant >> 2) + 2;

                s32 h = n * 2;

                blit[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                        FIXED_INTEGER_PART(d->x) - n / 2,
                        FIXED_INTEGER_PART(d->y) - n, n, h);
                Particle_AdvanceWithGravity2D(d, 56, toFIXED(1) / 8);
                d->velocityX -= work->scrollStepX * (FIXED_ONE / 16);
                d->variant--;
              }
            }
          }
        }
      }
      gProjection.centerX -= work->scrollStepX;
      gBgScroll[1].x += work->scrollStepX;
      for (i = 0; i != CATASTROPHE_PAN_SPEED; i++) {
        if (frame == i + 62) {
          work->scrollStepX++;
        }
      }
      Camera_ApplyVerticalShake(8);
      BattleEffect_CountDownBattlerTints();
      {
        s32 *pending = &work->transferPending;

        frame++;
        *pending = 1;
      }
      wait(1);
    } while (frame != 104);
    background->busy = 0;
    EFFECT_FINISH_CANVAS_ONE_BLITTER();
  }
}
