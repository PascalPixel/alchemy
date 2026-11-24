/// @file
/// The Flora summon, Sabre Dance and Backstab, with the routine that sets a
/// battler's sprite priority and an unused one that sends a unit outward.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "enemy_ids.h"
#include "sounds.h"

// Flora's summon.

/// Flora is twelve 32x32 sprites in 256 colours, three across and four down,
/// the first sprites of the effect state; then come her thirty-two 16x16
/// petals, the first fifteen each holding one of the tumbling petal's pictures
/// (PinkPetalsTiles).
#define FLORA_FIGURE_PIECES 12
#define FLORA_FIGURE_COLUMNS 3
#define FLORA_PETAL_SPRITES FLORA_FIGURE_PIECES
#define FLORA_PETAL_PICTURE_COUNT 15
#define FLORA_PETAL_COUNT 32
#define FLORA_PETAL_SIZE 16
#define FLORA_PETAL_PALETTE 15
#define FLORA_COLORS 240
/// A petal's phase counts its tumbling pictures in 256ths: its high byte is
/// the picture it shows.
#define FLORA_PHASE_ONE 256
/// Petals 0 to 5 blow across the sky on paths of their own before Flora
/// comes; 6 to 15 stream from her hand towards the foes from
/// FLORA_LAUNCH_FRAME; 16 to 31 burst out around her as she comes and again
/// as she leaves.
#define FLORA_PATH_PETALS 6
#define FLORA_BURST_PETAL 16
/// The storm: petals set out above the foes, of which one more falls each
/// frame, sweeping down over them, until FLORA_STORM_DRAWN have fallen. Each
/// foe is struck every FLORA_HIT_INTERVAL frames, the first time at a random
/// moment.
#define FLORA_STORM_FRAMES 128
#define FLORA_STORM_PETALS 128
#define FLORA_STORM_DRAWN 64
#define FLORA_HIT_INTERVAL 15
/// Where the storm's fifteen petal pictures are unpacked to a byte a pixel in
/// the effect's pictures.
#define FLORA_STORM_PICTURES 4096

/// Flora's petals in gEffectScratch: the tile number of each of the petal's
/// pictures, each petal's phase, position and velocity, and the object
/// palette as it fades.
struct FloraState {
  u16 tiles[FLORA_PETAL_PICTURE_COUNT];
  u16 phase[FLORA_PETAL_COUNT];
  u16 padding;
  s32 x[FLORA_PETAL_COUNT];
  s32 y[FLORA_PETAL_COUNT];
  s32 vx[FLORA_PETAL_COUNT];
  s32 vy[FLORA_PETAL_COUNT];
  u16 palette[FLORA_COLORS];
};

/// The petals' state, kept in the particles' memory.
#define FLORA_STATE ((struct FloraState *)petals)
/// Petal i's phase, read at gFlora_PetalTiles, FloraState's tile numbers from
/// the second on.
#define FLORA_PETAL_PHASE(i)                                                   \
  gFlora_PetalTiles[FLORA_PETAL_PICTURE_COUNT - 1 + (i)]

/// After the battle flashes away (SUMMON_OPENING_FRAMES), the sky scene plays
/// for FLORA_FRAMES; A or B cuts it short after its first sixteen.
#define FLORA_FRAMES 300
/// The canvas, shown at its own width FLORA_CANVAS_X pixels right of the
/// screen's left edge until then, widens to twice it.
#define FLORA_WIDE_FRAME 108
#define FLORA_CANVAS_X 60
/// Flora appears in a flash of FLORA_FLASH_FRAMES and fades in from white over
/// FLORA_FADE_IN_FRAMES.
#define FLORA_APPEAR_FRAME 112
#define FLORA_FLASH_FRAMES 8
#define FLORA_FADE_IN_FRAMES 32
/// Petals stream from her hand from FLORA_LAUNCH_FRAME, two frames apart, as
/// glows flare there; at FLORA_LEAVE_FRAME she flies away.
#define FLORA_LAUNCH_FRAME 223
#define FLORA_LEAVE_FRAME 252
/// The glows' pictures (gFlora_PetalPictures): shaded discs `height` pixels
/// tall and half as wide, one after another.
#define FLORA_GLOW_BYTES(height) ((height) / 2 * (height))
/// Where the glows flare on the canvas, by Flora's hand.
#define FLORA_GLOW_X 44
#define FLORA_GLOW_Y 78
/// Ten copies of mote picture 3, each 7 levels dimmer than the one before,
/// laid over the tenth mote picture from its 21st byte. Flora never draws
/// either.
#define FLORA_DIMMED_MOTE 3
#define FLORA_DIMMED_MOTE_BYTES (FLORA_DIMMED_MOTE * FLORA_DIMMED_MOTE * 2)
#define FLORA_DIMMED_MOTE_COPIES 10
#define FLORA_DIMMED_MOTES 590
/// The ring of motes: eight arms of four motes, spinning faster and faster
/// from FLORA_RING_FRAME, its spin at the start a 128th of a turn a frame.
#define FLORA_RING_MOTES 32
#define FLORA_RING_ARMS 8
#define FLORA_ARM_MOTES (FLORA_RING_MOTES / FLORA_RING_ARMS)
#define FLORA_RING_SPIN (TRIG_TURN / 128)
#define FLORA_RING_FRAME 156
#define FLORA_RING_FRAMES 75
/// The four rings of light (FireRingTexture): three as Flora appears and one
/// as she leaves.
#define FLORA_LIGHT_RINGS 4
/// The centre of the ring of motes and of the wheel, on the canvas.
#define FLORA_RING_X 48
#define FLORA_RING_Y 92
/// The wheel: a ring of dots and one three quarters its size, joined by four
/// spokes, which from FLORA_WHEEL_FRAME turns, tilts flat and slides to the
/// rings' centre, narrowing as it brightens to FLORA_WHEEL_GLOW; as Flora
/// leaves it widens and fades.
#define FLORA_WHEEL_DOTS 128
#define FLORA_WHEEL_SPOKES 4
#define FLORA_WHEEL_GLOW 36
#define FLORA_WHEEL_FRAME 186
#define FLORA_WHEEL_FRAMES 80

/// The effect state's sprite sheet.
static inline u8 *SheetOf(struct BattleEffectState *owner) {
  /* FAKEMATCH(wrapper): the sprite sheet's address through this inline. */
  return owner->sheet;
}

/// Flora: the battle flashes away to a violet sky where petals blow by. Flora
/// appears in a flash amid rings of light and a burst of petals and circles;
/// she drifts left as a ring of motes spins and a wheel of light turns by her
/// hand, where glows flare and petals stream out, and flies away as a last
/// ring of light flies out. A or B skips the sky scene. Then a storm of petals
/// sweeps over the foes.
///
/// Abilities: Flora.
void BattleEffect_RunFlora(struct BattleEffectParams *args) {
  u8 *canvas;
  struct BattleEffectState *state;
  DrawRectangle draw[2];
  s32 frame;
  s32 savedScroll;
  struct BattleBackgroundState *backdrop;
  u8 *motes;
  s32 bgX;
  FIXED posX;
  FIXED posY;
  s32 angle;
  s32 spin;
  s32 spread;
  s32 twist;
  s32 petals; // gEffectScratch, as FLORA_STATE reads it
  s32 i;
  s32 j;
  s32 order;
  s32 size;
  s32 dx;
  s32 dy;
  u32 shown;
  struct SpritePlacement place;
  struct Scale floraScale;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *verts;
  struct Particle *p;
  u8 *cells;
  u16 *phase;
  u8 *source;
  u8 *resource;
  struct ScreenPoint out;
  struct ScreenPoint pos;
  u8 dot[2];
  struct ScreenPoint ringScreen;
  struct ScreenPoint ringPoint;
  struct ScreenPoint fadeRingScreen;
  struct ScreenPoint fadeRingPoint;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  savedScroll = gBgScroll[1].x;
  backdrop = Ram_HeapSlots->battleBackground;
  petals = (s32)gEffectScratch;
  source = Ram_MapCellBuffer;
  motes = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  RagingHeat_FillGradientCanvas();
  state->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // The battle flashes away; then the sky, the petals and Flora load.
  bgX = -80;
  for (frame = 0; frame != SUMMON_OPENING_FRAMES; frame++) {
    if (frame == SUMMON_OPENING_LOAD_FRAME) {
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      BattleEffect_ShowLivingTargets(state->args);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_VIOLET_SKY_BACKDROP, 0);
      state->scrollTimer = 0;
      state->scrollInterval = 1;
      state->scrollStepX = -3;
      state->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
      backdrop->busy = 1;
      resource = Resource_GetTableEntry(RESOURCE_PINK_PETALS_TILES);
      Iwram_CopyWordsEntry((void *)&OBJ_PLTT_COLOR(FLORA_PETAL_PALETTE, 0),
                           resource, PALETTE_BANK_BYTES);
      resource += PALETTE_BANK_BYTES;
      Decompress_TaggedLz(resource, source);
      cells = Ram_MapCellBuffer;
      for (i = 0; i != FLORA_PETAL_PICTURE_COUNT; i++) {
        struct Sprite *obj = EffectSprite_CreateWithTiles(
            16, 16, OBJ_SIZE_16X16, OBJ_PALETTE(FLORA_PETAL_PALETTE));
        state->sprites[FLORA_PETAL_SPRITES + i] = obj;
        obj->image.priority = 2;
        Iwram_CopyWordsEntry((void *)OBJ_TILES(obj),
                             cells + i * OBJ_16X16_4BPP_BYTES,
                             OBJ_16X16_4BPP_BYTES);
        FLORA_STATE->tiles[i] =
            ((struct Sprite *)state->sprites[FLORA_PETAL_SPRITES + i])
                ->image.tile;
      }
      for (i = 0; i != FLORA_PETAL_COUNT - FLORA_PETAL_PICTURE_COUNT; i++) {
        struct Sprite *obj = EffectSprite_Acquire(
            16, 16, OBJ_SIZE_16X16, OBJ_PALETTE(FLORA_PETAL_PALETTE));
        state->sprites[FLORA_PETAL_SPRITES + FLORA_PETAL_PICTURE_COUNT + i] =
            obj;
        Iwram_CopyWordsEntry(obj, state->sprites[FLORA_PETAL_SPRITES],
                             offsetof(struct Sprite, lastNo));
      }
      // Each petal starts at a picture of its own.
      i = 0;
      phase = &FLORA_PETAL_PHASE(0);
      for (; i != FLORA_PETAL_COUNT; i++, phase++) {
        *phase = (Random16() % FLORA_PETAL_PICTURE_COUNT) << 8;
        ((struct Sprite *)state->sprites[FLORA_PETAL_SPRITES + i])->image.tile =
            FLORA_STATE->tiles[*phase / FLORA_PHASE_ONE];
      }
      resource = Resource_GetTableEntry(RESOURCE_GREEN_HAIRED_MAIDEN_TILES);
      resource += FLORA_COLORS * 2;
      Decompress_TaggedLz(resource, source);
      cells = Ram_MapCellBuffer;
      for (i = 0; i != FLORA_FIGURE_PIECES; i++) {
        struct Sprite *obj = EffectSprite_CreateWithTiles(
            32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        obj->image.priority = 2;
        state->sprites[i] = obj;
        Iwram_CopyWordsEntry((void *)OBJ_TILES(obj),
                             cells + i * OBJ_32X32_8BPP_BYTES,
                             OBJ_32X32_8BPP_BYTES);
      }
      Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, state->sheet, 0,
                                 0);
    }
    bgX += 4;
    REG_BG2X = bgX << 8;
    EFFECT_PRESENT_CANVAS(state);
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, source, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motes, 0, 0);
  // DEAD: the dimmed mote copies, which nothing reads; the ROM keeps the
  // loop.
  size = FLORA_DIMMED_MOTE;
  for (i = 0; i != FLORA_DIMMED_MOTE_COPIES; i++) {
    for (j = 0; j != FLORA_DIMMED_MOTE_BYTES; j++) {
      s32 v = source[gBattleEffect_MotePictures[size - 1] + j];

      if (v > 0) {
        v = CANVAS_LEVEL_BRIGHTEST - i * 7;
        if (v <= 0) {
          v = 1;
        }
      }
      motes[i * FLORA_DIMMED_MOTE_BYTES + FLORA_DIMMED_MOTES + j] = v;
    }
  }
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2X = 0;
  // DEAD: Flora's place, and the ring's spin, spread and twist, are each set
  // again before they are read; the ROM keeps the stores.
  posX = 128;
  posY = 44;
  angle = 0;
  spin = FLORA_RING_SPIN;
  spread = 0;
  twist = 0;
  // DEAD: the petals laid out in rows of eight, in 65536ths of a pixel; the
  // first frame places every petal again; the ROM keeps the loop.
  for (i = 0; i != FLORA_PETAL_COUNT; i++) {
    s32 row = i / 8;
    s32 leftX = 256;
    s32 columnX;
    s32 rowY;

    columnX = (i - row * 8) << 5;
    rowY = row * 22;
    FLORA_STATE->x[i] = columnX + leftX;
    FLORA_STATE->y[i] = rowY + 128;
  }
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = Ram_HeapSlots->ramCode;
  for (frame = 0; frame != FLORA_FRAMES &&
                  (!(gInput.repeat & (KEY_A | KEY_B)) || frame <= 16);
       frame++) {
    if (frame == 0) {
      Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      Iwram_ClearWordsEntry(canvas, CANVAS_BYTES);
      for (i = 0; i != FLORA_COLORS; i++) {
        FLORA_STATE->palette[i] = RGB_WHITE;
      }
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, gFlora_SpritePalettes,
                           FLORA_COLORS * 2);
      REG_BG2PA = Q_8_8(1);
      Graphics_LoadGreyRamp();
      REG_BG2X = -(FLORA_CANVAS_X << 8);
    }
    // The canvas's colours turn a level a frame towards Mars Djinn's, then
    // tan, then cyan.
    if (frame > 285) {
      BattleEffect_StepPaletteToResource(RESOURCE_CYAN_PALETTE);
    } else if (frame > 199) {
      BattleEffect_StepPaletteToResource(RESOURCE_TAN_PALETTE);
    } else if (frame > 180) {
      BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
    }
    if (frame == FLORA_WIDE_FRAME) {
      REG_BG2PA = Q_8_8(0.5);
      REG_BG2X = 0;
      // A quarter of each of the four glows: 8, 16, 32 and 48 pixels tall.
      BattleEffect_DrawShadedCircles(source, 8, FIXED_ONE);
      BattleEffect_DrawShadedCircles(Ram_MapCellBuffer + FLORA_GLOW_BYTES(8),
                                     16, FIXED_ONE * 3 / 4);
      BattleEffect_DrawShadedCircles(Ram_MapCellBuffer + FLORA_GLOW_BYTES(8) +
                                         FLORA_GLOW_BYTES(16),
                                     32, FIXED_ONE * 3 / 4);
      BattleEffect_DrawShadedCircles(Ram_MapCellBuffer + FLORA_GLOW_BYTES(8) +
                                         FLORA_GLOW_BYTES(16) +
                                         FLORA_GLOW_BYTES(32),
                                     48, FIXED_ONE * 3 / 4);
    }
    // A glow of a random size flares by her hand each frame until she leaves.
    if ((u32)(frame - FLORA_LAUNCH_FRAME) <=
        FLORA_LEAVE_FRAME - FLORA_LAUNCH_FRAME) {
      s32 k = Random16() & 3;
      s32 n = gFlora_PetalSizes[k] * 2;

      Canvas_BlitAtFoot(source + gFlora_PetalPictures[k], FLORA_GLOW_X,
                        FLORA_GLOW_Y, n);
    }
    // Flora fades in from white, a level a frame.
    shown = frame - FLORA_APPEAR_FRAME;
    if (shown < FLORA_FADE_IN_FRAMES) {
      u16 *resource =
          Resource_GetTableEntry(RESOURCE_GREEN_HAIRED_MAIDEN_TILES);

      for (i = 0; i != FLORA_COLORS; i++, resource++) {
        s32 c = FLORA_STATE->palette[i];
        s32 b = RGB_BLUE((u16)c);
        s32 g = RGB_GREEN((u16)c);
        s32 r = RGB_RED(c);
        s32 t = *resource;
        s32 tb = RGB_BLUE((u16)t);
        s32 tg = RGB_GREEN((u16)t);
        s32 tr = RGB_RED(t);

        if (b > tb) {
          b--;
        }
        if (g > tg) {
          g--;
        }
        if (r > tr) {
          r--;
        }
        FLORA_STATE->palette[i] = RGB(r, g, b);
      }
      Iwram_CopyWordsEntry((void *)OBJ_PLTT, gFlora_SpritePalettes,
                           FLORA_COLORS * 2);
    }
    if (shown < FLORA_FLASH_FRAMES) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST) -
                               shown * CANVAS_FILL_WORD(1) * 8);
    }
    floraScale = gFlora_UnitScale;
    dy = 0;
    place.groundHeight = 0;
    dx = 0;
    place.height = toFIXED(255);
    // Flora circles about (152, 44), 24 pixels out across and 20 up and down,
    // then drifts left and down; from frame 222 she trembles and the sky
    // shakes, and as she leaves she flies up and away to the right.
    if (frame <= 119) {
      posX = Trig_Sin(TRIG_TURN * 205 / 256) * 24 + toFIXED(152);
      posY = Trig_Cos(TRIG_TURN * 205 / 256) * 20 + toFIXED(44);
    } else if (frame <= 179) {
      s32 a = frame * 3 * 256 - TRIG_TURN * 19 / 32;

      posX = Trig_Sin(a) * 24 + toFIXED(152);
      posY = Trig_Cos(a) * 20 + toFIXED(44);
    } else if (frame <= 221) {
      posX += -toFIXED(9) / 8;
      posY += toFIXED(1) / 2;
    } else if (frame < FLORA_LEAVE_FRAME) {
      s32 m = frame % 6;
      s32 m4;

      if (m == 1) {
        dx = 1;
      }
      if (m == 2) {
        dx = 1;
      }
      if (m == 3) {
        dx = 2;
        dy = -1;
      }
      if (m == 4) {
        dx = 2;
        dy = -1;
      }
      if (m == 5) {
        dx = 2;
        dy = -1;
      }
      if (m == 0) {
        dx = 0;
        dy = 0;
      }
      if ((frame & 3) == 0) {
        gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y - 2;
      }
      m4 = frame - frame / 4 * 4;
      if (m4 == 1) {
        gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y - 2;
      }
      if (m4 == 2) {
        gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y + 2;
      }
      if (m4 == 3) {
        gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y + 2;
      }
    } else {
      gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
      posX += toFIXED(8);
      posY += -toFIXED(6);
    }
    if (frame >= FLORA_APPEAR_FRAME) {
      for (i = 0; i != FLORA_FIGURE_PIECES; i++) {
        place.x = posX + INTtoFIXED(i % FLORA_FIGURE_COLUMNS * 32 + dx);
        place.y = posY + INTtoFIXED(i / FLORA_FIGURE_COLUMNS * 32 + dy);
        Render_ApplyProjectedPlacementFar(state->sprites[i],
                                          SpritePlacement_Values(&place),
                                          (s32 *)&floraScale, 0);
      }
    }
    // Petals 0 to 5 blow along paths of their own and are put away off the
    // screen as Flora appears.
    for (i = 0; i != FLORA_PETAL_COUNT; i++) {
      s32 launchFrame = FLORA_LAUNCH_FRAME - FLORA_PATH_PETALS * 2 + i * 2;

      if (i == 0) {
        if (frame == 0) {
          FLORA_STATE->x[i] = toFIXED(240);
          FLORA_STATE->y[i] = toFIXED(48);
        } else if (frame <= 47) {
          FLORA_STATE->x[i] += -toFIXED(3);
          FLORA_STATE->y[i] += toFIXED(3) / 2;
        } else if (frame <= 79) {
          FLORA_STATE->x[i] += -toFIXED(1);
          FLORA_STATE->y[i] += -toFIXED(1);
        } else if (frame < FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] += toFIXED(3);
          // DEAD: y moves by 0; the ROM keeps its load and store.
          FLORA_STATE->y[i] += 0;
        }
        if (frame == FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] = toFIXED(256);
        }
      } else if (i == 1) {
        if (frame <= 27) {
          FLORA_STATE->x[i] = toFIXED(256);
          FLORA_STATE->y[i] = toFIXED(48);
        } else if (frame == 28) {
          FLORA_STATE->x[i] = toFIXED(16);
          FLORA_STATE->y[i] = toFIXED(32);
        } else if (frame <= 62) {
          FLORA_STATE->x[i] += toFIXED(1.83924);
          FLORA_STATE->y[i] += toFIXED(1.99183);
        } else if (frame <= 79) {
          FLORA_STATE->x[i] += toFIXED(1.61036);
          FLORA_STATE->y[i] += toFIXED(0.4026);
        } else if (frame <= 141) {
          FLORA_STATE->x[i] += toFIXED(3);
          // DEAD: y moves by 0; the ROM keeps its load and store.
          FLORA_STATE->y[i] += 0;
        }
        if (frame == FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] = toFIXED(256);
        }
      } else if (i == 2) {
        if (frame <= 49) {
          FLORA_STATE->x[i] = toFIXED(256);
          FLORA_STATE->y[i] = toFIXED(48);
        } else if (frame == 50) {
          FLORA_STATE->x[i] = 0;
          FLORA_STATE->y[i] = toFIXED(124);
        } else if (frame <= 62) {
          FLORA_STATE->x[i] += toFIXED(4);
          FLORA_STATE->y[i] += -toFIXED(5) / 4;
        } else if (frame <= 93) {
          FLORA_STATE->x[i] += toFIXED(3);
          FLORA_STATE->y[i] += toFIXED(3) / 4;
        } else if (frame <= 125) {
          FLORA_STATE->x[i] += toFIXED(3);
          FLORA_STATE->y[i] += -toFIXED(1);
        }
        if (frame == FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] = toFIXED(256);
        }
      } else if (i == 3) {
        if (frame <= 49) {
          FLORA_STATE->x[i] = toFIXED(256);
          FLORA_STATE->y[i] = toFIXED(48);
        } else if (frame == 50) {
          FLORA_STATE->x[i] = 0;
          FLORA_STATE->y[i] = toFIXED(108);
        } else if (frame <= 103) {
          FLORA_STATE->x[i] += toFIXED(3);
          FLORA_STATE->y[i] += -toFIXED(5) / 4;
        } else if (frame <= 135) {
          FLORA_STATE->x[i] += toFIXED(1);
          FLORA_STATE->y[i] += toFIXED(1);
        }
        if (frame == FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] = toFIXED(256);
        }
      } else if (i == 4) {
        if (frame <= 59) {
          FLORA_STATE->x[i] = toFIXED(256);
          FLORA_STATE->y[i] = toFIXED(48);
        } else if (frame == 60) {
          FLORA_STATE->x[i] = toFIXED(16);
          FLORA_STATE->y[i] = toFIXED(32);
        } else if (frame <= 82) {
          FLORA_STATE->x[i] += toFIXED(4);
          FLORA_STATE->y[i] += toFIXED(3);
        } else if (frame <= 135) {
          FLORA_STATE->x[i] += toFIXED(4);
        }
        if (frame == FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] = toFIXED(256);
        }
      } else if (i == 5) {
        if (frame <= 85) {
          FLORA_STATE->x[i] = toFIXED(256);
          FLORA_STATE->y[i] = toFIXED(48);
        } else if (frame == 86) {
          FLORA_STATE->x[i] = toFIXED(144);
          FLORA_STATE->y[i] = toFIXED(32);
        } else if (frame <= 135) {
          FLORA_STATE->x[i] += toFIXED(1);
          FLORA_STATE->y[i] += toFIXED(3);
        }
        if (frame == FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] = toFIXED(256);
        }
      } else if (i < FLORA_BURST_PETAL) {
        // Petals 6 to 15 wait off the screen; then each streams down and to
        // the left from Flora's hand, and starts again from there each time
        // it leaves the screen, until she leaves.
        if (frame == 0) {
          FLORA_STATE->x[i] = toFIXED(248);
          FLORA_STATE->y[i] = frame;
        } else if (frame == launchFrame) {
          FLORA_STATE->x[i] = INTtoFIXED(Random16() & 7) + toFIXED(92);
          FLORA_STATE->y[i] = INTtoFIXED(Random16() & 7) + toFIXED(104);
          FLORA_STATE->vx[i] =
              (Random16() & TRIG_ANGLE_MASK) * 12 - toFIXED(12);
          FLORA_STATE->vy[i] = toFIXED(4);
        } else if (frame >= launchFrame) {
          if (FLORA_STATE->x[i] >= -toFIXED(16) &&
              FLORA_STATE->y[i] <= toFIXED(168)) {
            FLORA_STATE->x[i] += FLORA_STATE->vx[i];
            FLORA_STATE->y[i] += FLORA_STATE->vy[i];
          } else if (frame < FLORA_LEAVE_FRAME) {
            FLORA_STATE->x[i] = INTtoFIXED(Random16() & 7) + toFIXED(92);
            FLORA_STATE->y[i] = INTtoFIXED(Random16() & 7) + toFIXED(104);
          }
        }
      } else if (i < FLORA_PETAL_COUNT) {
        // Petals 16 to 31 burst out in every direction as Flora appears and
        // again from her hand as she leaves, at 1 to 3 pixels a frame (in 8.8),
        // slowing until they are put away off the screen.
        if (frame == 0) {
          FLORA_STATE->x[i] = toFIXED(248);
          FLORA_STATE->y[i] = frame;
        } else if (frame == FLORA_APPEAR_FRAME) {
          u32 a = Random16() & TRIG_ANGLE_MASK;
          s32 speed = ((Random16() & 255) << 1) + Q_8_8(1);

          FLORA_STATE->x[i] = INTtoFIXED(Random16() & 7) + toFIXED(168);
          FLORA_STATE->y[i] = INTtoFIXED(Random16() & 7) + toFIXED(92);
          FLORA_STATE->vx[i] = Trig_Sin(a) * speed / Q_8_8(1);
          FLORA_STATE->vy[i] = Trig_Cos(a) * speed / Q_8_8(1);
        } else if (frame == FLORA_LEAVE_FRAME) {
          u32 a = Random16() & TRIG_ANGLE_MASK;
          s32 speed = ((Random16() & 255) << 1) + Q_8_8(1);

          FLORA_STATE->x[i] = INTtoFIXED(Random16() & 7) + toFIXED(92);
          FLORA_STATE->y[i] = INTtoFIXED(Random16() & 7) + toFIXED(104);
          FLORA_STATE->vx[i] = Trig_Sin(a) * speed / Q_8_8(1);
          FLORA_STATE->vy[i] = Trig_Cos(a) * speed / Q_8_8(1);
        } else if (frame >= FLORA_APPEAR_FRAME) {
          FLORA_STATE->x[i] += FLORA_STATE->vx[i];
          FLORA_STATE->y[i] += FLORA_STATE->vy[i];
          FLORA_STATE->vx[i] = FLORA_STATE->vx[i] * 63 / DAMPING_SCALE;
          FLORA_STATE->vy[i] = FLORA_STATE->vy[i] * 63 / DAMPING_SCALE;
          if ((u32)(FLORA_STATE->vx[i] + FIXED_ONE - 1) <=
                  2 * (FIXED_ONE - 1) &&
              (u32)(FLORA_STATE->vy[i] + FIXED_ONE - 1) <=
                  2 * (FIXED_ONE - 1)) {
            FLORA_STATE->x[i] = toFIXED(256);
            FLORA_STATE->y[i] = 0;
          }
        }
      }
      // Each petal tumbles, those on paths an eighth of a picture a frame and
      // the rest half a picture.
      if (i < FLORA_PATH_PETALS) {
        FLORA_PETAL_PHASE(i) = (FLORA_PETAL_PHASE(i) + FLORA_PHASE_ONE / 8) %
                               (FLORA_PETAL_PICTURE_COUNT << 8);
      } else {
        FLORA_PETAL_PHASE(i) = (FLORA_PETAL_PHASE(i) + FLORA_PHASE_ONE / 2) %
                               (FLORA_PETAL_PICTURE_COUNT << 8);
      }
      ((struct Sprite *)state->sprites[FLORA_PETAL_SPRITES + i])->image.tile =
          FLORA_STATE->tiles[FLORA_PETAL_PHASE(i) >> 8];
    }
    for (i = 0; i != FLORA_PETAL_COUNT; i++) {
      place.x = FLORA_STATE->x[i] - toFIXED(FLORA_PETAL_SIZE / 2);
      place.y = FLORA_STATE->y[i] - toFIXED(FLORA_PETAL_SIZE / 2);
      Render_ApplyProjectedPlacementFar(state->sprites[FLORA_PETAL_SPRITES + i],
                                        SpritePlacement_Values(&place),
                                        (s32 *)&floraScale, 0);
    }
    if (frame == 114) {
      Sound_PlayFar(SE_POWER_UP);
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 7;
    texture.logHeight = 7;
    texture.pixels = state->sheet;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->texture = &texture;
    model->mesh = gBattleEffect_LargeSquareMesh;
    model->vertices = verts;
    // Each ring of light grows an eighth of its size a frame as it turns, and
    // fades from its eighth frame.
    for (i = 0; i != FLORA_LIGHT_RINGS; i++) {
      s32 age = frame - gFlora_BloomFrames[i];

      if (age >= 0) {
        s32 scale = age * (FIXED_ONE / 8);
        s32 shade = 0;

        if (age > 7) {
          shade = (8 - age) * 6;
        }
        model->shade = shade;
        if (shade > MODEL_SHADE_INVISIBLE) {
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(gFlora_BloomX[i]),
                           INTtoFIXED(gFlora_BloomY[i]), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_RotateZ(gFlora_BloomRolls[i]);
          Matrix_RotateX(gFlora_BloomPitches[i]);
          Matrix_ScaleUniform(scale);
          Matrix_RotateY(frame * (TRIG_TURN / 32));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    Matrix_Initialize();
    Matrix_RotateX(TRIG_TURN * 37 / 64);
    Matrix_RotateY(TRIG_TURN * 13 / 32);
    if (frame == FLORA_RING_FRAME) {
      spin = FLORA_RING_SPIN;
      angle = 0;
    }
    Matrix_RotateZ(angle);
    if (frame == FLORA_RING_FRAME) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == FLORA_LAUNCH_FRAME) {
      Sound_PlayFar(SE_LAUNCH);
    }
    if (frame == FLORA_LEAVE_FRAME) {
      Sound_PlayFar(SE_POWER_UP);
    }
    // The ring of motes closes on its centre as it spins; until frame 194 its
    // arms twist and spread, and from frame 208 the motes of each arm shrink
    // into the centre one by one, the outermost first.
    if ((u32)(frame - FLORA_RING_FRAME) < FLORA_RING_FRAMES) {
      s32 ringRadius = 80 - (frame - FLORA_RING_FRAME) / 2;

      if (frame == FLORA_RING_FRAME) {
        spread = 0;
        twist = 0;
      }
      if (frame <= 194) {
        twist += TRIG_TURN / 2048;
        spread += 3;
      }
      spin += TRIG_TURN / 4096;
      angle += spin;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      draw[1] = Ram_HeapSlots->ramCodePaired;
      for (i = 0; i != FLORA_RING_MOTES; i++) {
        s32 a = (i / FLORA_ARM_MOTES) * (TRIG_TURN / FLORA_RING_ARMS);
        s32 m = i % FLORA_ARM_MOTES;
        s32 b = m * twist;
        s32 radius = ringRadius + m * spread / 30;
        s32 x;
        s32 y;

        size = 7;
        for (order = 0; order != FLORA_ARM_MOTES; order++) {
          if (frame >= 208 + order * 4 &&
              i % FLORA_ARM_MOTES == FLORA_ARM_MOTES - 1 - order) {
            s32 late = frame - order * 4 - 208;

            radius = ringRadius - late * 4;
            if (radius < 0) {
              radius = 0;
            }
            size = 7 - late;
            if (size <= 2) {
              size = 3;
            }
          }
        }
        pos.x = FIXEDtoINT(Trig_Sin(a + b) * radius);
        pos.y = FIXEDtoINT(Trig_Cos(a + b) * radius);
        pos.depth = 0;
        EffectPosition_ProjectPoint(&pos, &out);
        out.x >>= 1;
        x = out.x + FLORA_RING_X;
        y = out.y + FLORA_RING_Y;
        draw[1](canvas, motes + gBattleEffect_MotePictures[size - 1],
                x - size / 2, y - size, size, size * 2);
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    if ((u32)(frame - FLORA_WHEEL_FRAME) < FLORA_WHEEL_FRAMES) {
      s32 glow = frame - FLORA_WHEEL_FRAME;
      s32 radius;
      s32 level;
      s32 centerX;
      s32 outerY;
      s32 innerY;
      s32 driftX;
      s32 driftY;
      s32 x1;
      s32 y1;
      s32 x2;
      s32 y2;

      if (frame > 247) {
        glow = 532 - frame * 2;
      } else if (glow > FLORA_WHEEL_GLOW) {
        glow = FLORA_WHEEL_GLOW;
      }
      radius = 64 - glow;
      level = glow;
      driftX = FLORA_WHEEL_GLOW / 2 - level / 2;
      driftY = FLORA_WHEEL_GLOW - level;
      dot[0] = level;
      dot[1] = level;
      Matrix_RotateZ(frame * (TRIG_TURN / 64));
      if (frame <= 221) {
        Matrix_RotateX(driftY * (TRIG_TURN / 64));
      }
      if (driftX < 0) {
        driftX = 0;
      }
      if (driftY < 0) {
        driftY = 0;
      }
      centerX = driftX + FLORA_RING_X;
      outerY = FLORA_RING_Y - driftY;
      innerY = FLORA_RING_Y - 4 - driftY;
      for (i = 0; i != FLORA_WHEEL_DOTS; i++) {
        ringPoint.x =
            FIXEDtoINT(Trig_Sin(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
        ringPoint.y =
            FIXEDtoINT(Trig_Cos(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
        ringPoint.depth = 0;
        EffectPosition_ProjectPoint(&ringPoint, &ringScreen);
        ringScreen.x >>= 1;
        x1 = ringScreen.x + centerX;
        y1 = ringScreen.y + outerY;
        x2 = ringScreen.x * 3 / 4 + centerX;
        y2 = ringScreen.y * 3 / 4 + innerY;
        draw[0](canvas, dot, x1 - 1, y1 - 1, 1, 2);
        draw[0](canvas, dot, x2 - 1, y2 - 1, 1, 2);
        if ((i & (FLORA_WHEEL_DOTS / FLORA_WHEEL_SPOKES - 1)) == 0) {
          BattleEffect_DrawClippedCanvasLine(x1, y1, x2, y2, glow);
          BattleEffect_DrawClippedCanvasLine(x1, y1 + 1, x2, y2 + 1, glow);
        }
      }
      // An echo of the wheel spreads from it and fades until Flora leaves.
      if ((u32)(frame - 228) <= FLORA_LEAVE_FRAME - 228) {
        s32 x1b;
        s32 y1b;
        s32 x2b;
        s32 y2b;

        level = 256 - frame;
        radius = frame - 204;
        dot[0] = level;
        dot[1] = level;
        for (i = 0; i != FLORA_WHEEL_DOTS; i++) {
          fadeRingPoint.x =
              FIXEDtoINT(Trig_Sin(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
          fadeRingPoint.y =
              FIXEDtoINT(Trig_Cos(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
          fadeRingPoint.depth = 0;
          EffectPosition_ProjectPoint(&fadeRingPoint, &fadeRingScreen);
          fadeRingScreen.x >>= 1;
          x1b = centerX + fadeRingScreen.x;
          y1b = fadeRingScreen.y + outerY;
          x2b = fadeRingScreen.x * 3 / 4 + centerX;
          y2b = fadeRingScreen.y * 3 / 4 + innerY;
          draw[0](canvas, dot, x1b - 1, y1b - 1, 1, 2);
          draw[0](canvas, dot, x2b - 1, y2b - 1, 1, 2);
          if ((i & (FLORA_WHEEL_DOTS / FLORA_WHEEL_SPOKES - 1)) == 0) {
            BattleEffect_DrawClippedCanvasLine(x1b, y1b, x2b, y2b, level);
            BattleEffect_DrawClippedCanvasLine(x1b, y1b + 1, x2b, y2b + 1,
                                               level);
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  // The battle comes back for the storm.
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  gBgScroll[1].x = savedScroll;
  backdrop->busy = 0;
  BattleEffect_SetupBlendedDisplay();
  for (i = 0; i != FLORA_PETAL_SPRITES + FLORA_PETAL_COUNT; i++) {
    Sprite_DestroyFar(state->sprites[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT,
          (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  REG_BG2X = 0;
  REG_BLDCNT = 0;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2PA = Q_8_8(0.5);
  {
    s32 picture;
    s32 tileRow;
    s32 tileColumn;
    s32 k;
    s32 m;
    s32 t;
    u8 *sheet;
    s32 hi;

    resource = Resource_GetTableEntry(RESOURCE_PINK_PETALS_TILES);
    Iwram_CopyWordsEntry((void *)BG_PLTT, resource, PALETTE_BANK_BYTES);
    resource += PALETTE_BANK_BYTES;
    Decompress_TaggedLz(resource, SheetOf(state));
    sheet = state->sheet + FLORA_STORM_PICTURES;
    // Each 8x8 tile of the petal's pictures, two pixels a byte, is unpacked
    // to a byte a pixel at its place in its 16x16 picture.
    i = 0;
    for (picture = 0; picture != FLORA_PETAL_PICTURE_COUNT; picture++) {
      for (tileRow = 0; tileRow != 2; tileRow++) {
        for (tileColumn = 0; tileColumn != 2; tileColumn++) {
          s32 offset;

          t = picture * 2;
          t += tileRow;
          t <<= 4;
          t += tileColumn;
          k = 0;
          offset = t << 3;
          for (; k != 8; k++, offset += FLORA_PETAL_SIZE) {
            u8 *d = (u8 *)(offset + (s32)sheet);

            for (m = 0; m != 4; m++) {
              hi = state->sheet[i] >> 4 & 0xf;
              d[0] = state->sheet[i] & 0xf;
              d[1] = hi;
              i++;
              d += 2;
            }
          }
        }
      }
    }
  }
  for (frame = 0; frame != FLORA_STORM_FRAMES; frame++) {
    if (frame == 0) {
      p = (struct Particle *)source;
      for (i = 0; i != FLORA_STORM_PETALS; i++, p++) {
        p->x = INTtoFIXED((Random16() & 63) + 96);
        p->y = INTtoFIXED(-(Random16() & 63));
        p->velocityX = -((Random16() & 255) + 128) << 7;
        p->velocityY = ((Random16() & 255) + 128) << 9;
        p->variant = Random16() & 31;
      }
      for (i = 0; i != args->targetCount; i++) {
        state->particles[i].variant = (Random16() & 15) + 1;
      }
    }
    // Each petal tumbles a picture every four frames as it falls.
    p = (struct Particle *)source;
    for (i = 0; i != FLORA_STORM_DRAWN; i++, p++) {
      if (frame > i) {
        draw[0](canvas,
                state->sheet + FLORA_STORM_PICTURES +
                    (p->variant / 4 % FLORA_PETAL_PICTURE_COUNT << 8),
                FIXED_INTEGER_PART(p->x) - 4, FIXED_INTEGER_PART(p->y),
                FLORA_PETAL_SIZE, FLORA_PETAL_SIZE);
        Particle_AdvanceWithGravity2D(p, PARTICLE_DAMPING_NONE, 0);
        p->velocityX += -toFIXED(1) / 16;
        p->variant++;
      }
    }
    if (frame == 56) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if ((u32)(frame - 57) <= 50) {
      for (i = 0; i != args->targetCount; i++) {
        struct Particle *s = &state->particles[i];

        if (--s->variant == 0) {
          s->variant = FLORA_HIT_INTERVAL;
          state->shakeFrames = 4;
          Sound_PlayFar(SE_IMPACT);
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_NUDGE);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 8);
        }
      }
    }
    BattleCamera_Shake(2, 2);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Unused: nothing calls it. Sends a unit outward: unless it is the Fusion
/// Dragon or one of three of the Doom Dragon's forms, it moves 15 pixels
/// further out on its own side, hopping if it stands under 10 pixels up; every
/// unit then falls under a gravity of 0.6 pixels a frame without bouncing, and
/// takes its damage pose.
UNUSED void BattleEffect_StartActorMotionOutward(s32 id) {
  struct BattleObject *slot;
  struct MotionObject *object;
  u32 enemy;
  FIXED x;

  slot = BattleObject_GetFar(id);
  object = slot->object;
  enemy = Unit_Get(id)->enemyId;
  if (enemy != ENEMY_FUSION_DRAGON && enemy != ENEMY_DOOM_DRAGON_A &&
      enemy != ENEMY_DOOM_DRAGON_E && enemy != ENEMY_DOOM_DRAGON_H) {
    object->acceleration = toFIXED(2);
    object->speedLimit = toFIXED(8);
    if (object->y < toFIXED(10))
      object->velocityY += toFIXED(5) / 2;
    Actor_StartIdleFar(object);
    if (slot->anchorX > 0)
      x = object->x + toFIXED(15);
    else
      x = object->x - toFIXED(15);
    Actor_SetMoveTargetFar(object, x, 0, slot->anchorZ);
  }
  object->verticalMotionStrength = toFIXED(3) / 5;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SelectAnimationFar(object, POSE_DAMAGE);
}

#define SABRE_DANCE_FRAMES 96
/// Of the particles set up, the first SABRE_DANCE_BLADES are drawn.
#define SABRE_DANCE_PARTICLES 32
#define SABRE_DANCE_BLADES 8
/// The blades turn and fall on the target.
#define SABRE_DANCE_FALL_FRAME 80
#define SABRE_DANCE_LAST_HIT 90

/// Dancing blades. The canvas is moved over the target. Eight pairs of crossed
/// blades rise spinning from below it, and the target is struck eight times,
/// six frames apart from frame 20: each strike shakes the screen, flashes the
/// target and knocks it up. At frame 80 the blades turn point down in a fan
/// above the target and plunge into it; the last strike, at frame 90, lets the
/// battle show the damage.
///
/// Abilities: Sabre Dance.
void BattleEffect_RunSabreDance(struct BattleEffectParams *args) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  struct ModelTexture texture;
  struct ScreenPoint target;
  struct Particle *blade;
  struct RenderModel *quad;
  void *vertices;
  s32 frame;
  s32 i;
  s32 strikeFrame;

  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_CROSSED_DAGGERS_SHEET, state->sheet, 1,
                             1);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  REG_BLDCNT = 0;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // Centre the canvas's x 64 on the target.
  BattleObject_GetFar(args->targets[0]);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (64 - target.x) << 8;

  for (frame = 0; frame != SABRE_DANCE_FRAMES; frame++) {
    // The first frame lines the blades up below the canvas, each spinning
    // the other way from its neighbour.
    if (frame == 0) {
      FIXED y;
      s32 zero;

      for (i = 0, zero = 0, y = toFIXED(128), blade = state->particles;
           i != SABRE_DANCE_PARTICLES; i++, blade++) {
        SET_POSITION(blade, INTtoFIXED((Random16() & 63) + 32), y, Random16());
        blade->velocityX = zero;
        blade->velocityY = -((Random16() & 31) + 32) << 10;
        blade->velocityZ = (Random16() & 0x3ff) + toFIXED(1) / 64;
        if (i & 1)
          blade->velocityZ = -blade->velocityZ;
        blade->variant = zero;
        y += toFIXED(8);
      }
    }
    Matrix_Initialize();
    gProjection.centerY = 104;

    for (i = 0, strikeFrame = 20; i != 8; i++, strikeFrame += 6) {
      if (frame == strikeFrame) {
        state->shakeFrames = 4;
        Sound_PlayFar(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 6);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, toFIXED(3),
                                      100);
      }
    }
    if (frame == SABRE_DANCE_LAST_HIT) {
      state->shakeFrames = 4;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 10);
      BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, toFIXED(3), 100);
    }

    // One textured quad, drawn once for each pair of blades.
    vertices = Heap_AllocEwram(32);
    quad = Render_AllocateModelList(1);
    quad->shade = 0;
    texture.logWidth = 5;
    texture.logHeight = 5;
    quad->texture = &texture;
    texture.pixels = state->sheet;
    quad->kind = MODEL_SHADED;
    quad->mesh = gSabreDance_DaggerMesh;
    quad->vertices = vertices;
    for (i = 0, blade = state->particles; i != SABRE_DANCE_BLADES;
         i++, blade++) {
      // The fall: from 48 pixels above the canvas, fanned out by angle and
      // closing on the middle.
      if (frame == SABRE_DANCE_FALL_FRAME) {
        s32 angle;
        s32 drift;

        blade->y = -toFIXED(48);
        angle =
            i * (TRIG_TURN / 64) + (Random16() & 0x3ff) - TRIG_TURN * 9 / 128;
        blade->x = angle * (toFIXED(1) / 64) + toFIXED(64);
        blade->z = angle;
        drift = -angle << 5;
        blade->velocityY = toFIXED(12);
        blade->velocityX = drift;
        blade->velocityZ = 0;
      }
      Matrix_Initialize();
      Matrix_Translate(blade->x - toFIXED(64), blade->y - toFIXED(64), 0);
      Matrix_ScaleUniform(FIXED_ONE / 2);
      Matrix_RotateZ(blade->z);
      Render_TransformVertices(gSabreDance_DaggerCorners, vertices, 4);
      Render_DrawModelList(quad);
      if (frame >= SABRE_DANCE_FALL_FRAME)
        Particle_AdvanceWithGravity3D(blade, PARTICLE_DAMPING_NONE, 0);
      else
        Particle_AdvanceWithGravity3D(blade, PARTICLE_DAMPING_NONE,
                                      -toFIXED(1) / 16);
    }
    Heap_Release(quad);
    Heap_Release(vertices);
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(state);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Sets the OBJ priority of every motion record of the battle object, unless
/// the object is fading out.
void BattleObject_SetRecordPriority(s32 objectId, s32 priority) {
  struct BattleObject *slot = BattleObject_GetFar(objectId);
  struct PlacedSprite *record;
  s32 i;

  for (i = 0; (record = Actor_GetSpriteFar(slot->object, i)) != NULL; i++) {
    if (slot->fading == 0)
      record->priority = priority;
  }
}

/// Backstab's length; the caster leaps at the foe at BACKSTAB_LEAP_FRAME and
/// strikes at BACKSTAB_STRIKE_FRAME.
#define BACKSTAB_FRAMES 60
#define BACKSTAB_LEAP_FRAME 16
#define BACKSTAB_STRIKE_FRAME 32
/// Of the puffs of smoke set up, the first SMOKE_DRAWN rise, each through
/// SmokeSheet's six pictures, four frames each.
#define SMOKE_COUNT 32
#define SMOKE_DRAWN 24
#define STAR_COUNT 16
#define STAR_CELS 16000 ///< the pink star's six 16x16 cels, after the smoke's

/// A pink star burst. Smoke puffs from the caster for its first 24 frames
/// while the caster stands just past the target; at frame 16 it leaps at
/// the target, and at frame 32 the palette turns pink, the caster lands
/// beside the target, which is knocked back and flashes, and sixteen pink
/// stars burst from it for 24 frames. The battle keeps the sprites' priorities
/// as the effect sets them throughout.
///
/// Abilities: Backstab.
void BattleEffect_RunBackstab(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  u8 *canvas;
  struct ScreenPoint caster;
  struct ScreenPoint target;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  struct MotionObject *targetObject;
  struct MotionObject *casterObject;
  struct BattleState *battle;
  struct Particle *particle;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_PINK_STAR_SHEET, state->sheet + STAR_CELS,
                             1, 0);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  targetObject = BattleObject_GetFar(args->targets[0])->object;
  casterObject = BattleObject_GetFar(args->user)->object;
  battle = Ram_HeapSlots->battleState;
  battle->keepObjectPriorities = 1;

  for (frame = 0; frame != BACKSTAB_FRAMES; frame++) {
    BattleObject_SetRecordPriority(args->user, 3);

    // The first frame sends the smoke out from the caster's feet.
    if (frame == 0) {
      EffectPosition_ProjectBattler(args->user, &caster);
      for (i = 0; i != SMOKE_COUNT; i++) {
        s32 angle;
        s32 speed;

        particle = &state->particles[i];
        particle->variant = 0;
        particle->x = INTtoFIXED(caster.x / 2);
        particle->y = INTtoFIXED(caster.y - 8);
        angle = Random16();
        speed = (Random16() & 255) + 127;
        particle->velocityX = (Trig_Sin(angle) * speed) >> 5;
        particle->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      Sound_PlayFar(SE_WHOOSH);
    }

    // The smoke rises through its six 24x48 cels, slowing; the caster
    // starts 160 percent of the way out to the target.
    // DEAD: always true, as frame counts up from 0; the ROM keeps the test.
    if (frame >= 0) {
      struct Particle *puff;

      for (i = 0, puff = state->particles; i != SMOKE_DRAWN; i++, puff++) {
        if (frame >= i / 8 && puff->variant < SMOKE_PUFF_FRAMES) {
          draw[0](canvas,
                  state->sheet +
                      (puff->variant / 4) * PICTURE_BYTES(SMOKE_PUFF),
                  FIXED_INTEGER_PART(puff->x) - 12,
                  FIXED_INTEGER_PART(puff->y) - 24, SMOKE_PUFF_WIDTH,
                  SMOKE_PUFF_HEIGHT);
          Particle_AdvanceWithGravity2D(puff, 48, -toFIXED(1) / 8);
          puff->variant++;
        }
      }
      if (frame == 0) {
        casterObject->z = targetObject->z;
        casterObject->x = targetObject->x * 160 / 100;
        casterObject->angle = targetObject->angle;
      }
    }

    if (frame == BACKSTAB_LEAP_FRAME) {
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 16, 120);
      casterObject->velocityY = toFIXED(7);
    }

    // The strike: pink palette, the caster lands at 120 percent and the
    // target is pushed to 80, and the stars burst upward from it.
    if (frame == BACKSTAB_STRIKE_FRAME) {
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_PINK_STAR_SHEET),
                           SHEET_PALETTE_BYTES);
      casterObject->z = targetObject->z;
      casterObject->x = targetObject->x * 120 / 100;
      targetObject->x = targetObject->x * 80 / 100;
      casterObject->angle = targetObject->angle;
      EffectPosition_ProjectScaledBattler(args->targets[0], &target);
      for (i = 0, particle = &state->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, particle++) {
        s32 angle;
        s32 speed;

        particle->variant = (Random16() & 15) + 16;
        particle->x = INTtoFIXED(target.x / 2);
        particle->y = INTtoFIXED(target.y - 8);
        angle = (Random16() & 0x7fff) + DEGtoANG(90);
        speed = (Random16() & 255) + 127;
        particle->velocityX = (Trig_Sin(angle) * speed) >> 7;
        particle->velocityY = (Trig_Cos(angle) * speed) >> 5;
      }
      state->shakeFrames = 4;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(8),
                                    toFIXED(20), toFIXED(6), 10);
    }

    // The stars twinkle through their six cels as they fly.
    if (frame >= BACKSTAB_STRIKE_FRAME && frame < BACKSTAB_STRIKE_FRAME + 24) {
      struct Particle *star;

      for (i = 0, star = &state->particles[SMOKE_COUNT]; i != STAR_COUNT;
           i++, star++) {
        if (star->variant >= 0) {
          draw[0](canvas,
                  state->sheet + STAR_CELS + (((frame + i) / 4) % 6) * 256,
                  FIXED_INTEGER_PART(star->x) - 8,
                  FIXED_INTEGER_PART(star->y) - 8, 16, 16);
          Particle_AdvanceWithGravity2D(star, 56, 0);
          star->variant--;
        }
      }
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(state);
  }

  battle->keepObjectPriorities = 0;
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}
#undef FLORA_STATE
#undef FLORA_FIGURE_PIECES
#undef FLORA_FIGURE_COLUMNS
#undef FLORA_PETAL_SPRITES
#undef FLORA_PETAL_PICTURE_COUNT
#undef FLORA_PETAL_COUNT
#undef FLORA_PETAL_SIZE
#undef FLORA_PETAL_PALETTE
#undef FLORA_COLORS
#undef FLORA_PHASE_ONE
#undef FLORA_PATH_PETALS
#undef FLORA_BURST_PETAL
#undef FLORA_STORM_FRAMES
#undef FLORA_STORM_PETALS
#undef FLORA_STORM_DRAWN
#undef FLORA_HIT_INTERVAL
#undef FLORA_STORM_PICTURES
#undef FLORA_PETAL_PHASE
#undef FLORA_FRAMES
#undef FLORA_WIDE_FRAME
#undef FLORA_CANVAS_X
#undef FLORA_APPEAR_FRAME
#undef FLORA_FLASH_FRAMES
#undef FLORA_FADE_IN_FRAMES
#undef FLORA_LAUNCH_FRAME
#undef FLORA_LEAVE_FRAME
#undef FLORA_GLOW_BYTES
#undef FLORA_GLOW_X
#undef FLORA_GLOW_Y
#undef FLORA_DIMMED_MOTE
#undef FLORA_DIMMED_MOTE_BYTES
#undef FLORA_DIMMED_MOTE_COPIES
#undef FLORA_DIMMED_MOTES
#undef FLORA_RING_MOTES
#undef FLORA_RING_ARMS
#undef FLORA_ARM_MOTES
#undef FLORA_RING_SPIN
#undef FLORA_RING_FRAME
#undef FLORA_RING_FRAMES
#undef FLORA_LIGHT_RINGS
#undef FLORA_RING_X
#undef FLORA_RING_Y
#undef FLORA_WHEEL_DOTS
#undef FLORA_WHEEL_SPOKES
#undef FLORA_WHEEL_GLOW
#undef FLORA_WHEEL_FRAME
#undef FLORA_WHEEL_FRAMES
#undef SABRE_DANCE_FRAMES
#undef SABRE_DANCE_PARTICLES
#undef SABRE_DANCE_BLADES
#undef SABRE_DANCE_FALL_FRAME
#undef SABRE_DANCE_LAST_HIT
#undef BACKSTAB_FRAMES
#undef BACKSTAB_LEAP_FRAME
#undef BACKSTAB_STRIKE_FRAME
#undef SMOKE_COUNT
#undef SMOKE_DRAWN
#undef STAR_COUNT
#undef STAR_CELS
