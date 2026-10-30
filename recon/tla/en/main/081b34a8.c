// Near miss, differ 6 of 1016 (tla-en, 2026-10-30), no device: the two
// swaps described below, both sched2 ranking. After Task_ResetAll the
// gRenderOamEnabled store (priority 1) waits behind the loop constants'
// literal loads (priority 3, a load ranked a cycle slower) and count's zero
// (2), where the native stores it as soon as its address is loaded; in the
// sparks' draw call the spilled draw-table address reload (priority 7)
// issues before the first stack argument's store (6), where the native
// stores first. Tried (2026-10-30): gRenderOamEnabled = 0 after count = 0
// or j = 0 (6), k = 0 apart from its loop (6), j = 0 before count = 0 (17);
// five more minutes of permute, 4,106 candidates, none below 6.
// Draft, differ 8 of 1016 (tla-en): Lucky Wheels, The Broken Seal's matched
// LuckyWheels_Run (games/tbs/src/field/lucky_wheels.c) on this game's
// heap slots (sheet 100, canvas 96, effect work 92, wheel work 180), with a
// third callback, Func_081b3294, and BattleStaging_ProcessPendingTileTransfer
// as the canvas transfer. Reshaped toward The Broken Seal's C, measured in
// order from 436: resources and messages by their linker names, as the
// native loads each from the pool (ResourceId_FarCall00C, the Slots_ and
// Djinn sheets, MsgSlotsChangeBet and MsgSlotsControls): 403; the
// background shade's address kept in its own local beside the sprites':
// 184; the tile buffer as tiles = Ram_MapCellBuffer, a constant, as The
// Broken Seal writes it: 51; the blitters read through Ram_HeapSlots'
// fields, so the heap base stays in r5 across the second build: 28; the
// Task_Add, Resource_FarCall00C and Graphics_ScaleRgb555 returning s32:
// 8. Left, both scheduling: the store of gRenderOamEnabled comes before the
// loop constants load in the native (an empty asm after it gives 6), and
// in the sparks' draw call the native stores both stack arguments before it
// reloads the blitter pair's address; here after. Variants of the call (the
// pointer through a local, (*draw[j & 1]), size << 1, d = size * 2) change
// nothing or worsen it; permute (15 minutes) found nothing below 8.
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "frame.h"
#include "bg_scroll.h"
#include "effect_step.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct Reel {
  s32 position;
  u8 symbols[21];
  u8 held;
  s8 stop;
};

struct ReelSprite {
  volatile u32 attr01;
  volatile u32 attr2;
};

struct ReelWork {
  struct Reel reels[5];
  s32 state;
  s32 cursor;
  s32 spins;
  s32 bet;
  u16 keys;
  u16 direction;
  u16 pressed;
  u16 repeat;
  s32 timer;
  s32 spriteCount;
  s32 winningLines[7];
  struct ReelSprite obj[128];
  s32 window;
  s32 subWindow;
  u8 unk4d0[8];
  u16 scanlineOffsets[160];
  s32 phase;
};

struct ReelEffectWork {
  struct EffectStep particles[8];
  u8 unk00e0[0x700 - 0xe0];
  u16 bgShade[0x100];
  u16 objShade[0x100];
  u8 unk0b00[0x778c - 0xb00];
  s32 frame;
  u8 unk7790[0x78e8 - 0x7790];
  s32 transferPending;
};

struct ReelSave {
  u8 unk000[0x140];
  s8 wonPrizes[16];
};

typedef void (*DrawRectangle)(void *canvas, const u8 *source, s32 x, s32 y,
                              s32 width, s32 height);

extern u8 Data_0200024c[];
extern const u16 gLuckyWheels_SparkCellOffsets[];

void *HeapSlot_Alloc(s32 slot, s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Resource_ReservedNoOp(s32 id);
void Task_ResetAll(void);
s32 Task_Add(void (*func)(void), s32 priority);
void Task_Remove(void (*func)(void));
u8 *Resource_GetTableEntry(s32 id);
void Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FarCall00C(s32 slot, s32 a, s32 b, s32 c, s32 d);
void WindowSystem_InitFieldFar(void);
void Func_081b336c(void);
void Func_081b243c(void);
void Func_081b3294(void);
void BattleStaging_ProcessPendingTileTransfer(void);
void Func_081b21ec(s32 resource);
void Func_081b22b8(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void Palette_DarkenSceneStep(void);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 Graphics_ScaleRgb555(u16 *source, u16 *destination, s32 level,
                          s32 count);
s32 PartyInventory_CountItemFar(s32 item);
s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void wait(s32 frames);

extern char ResourceId_FarCall00C;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_SlotsMachinePicture;
extern char ResourceId_SlotsIcons;
extern char ResourceId_SlotsJackpotBanner;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_LimePalette;
extern char MsgSlotsChangeBet[];
extern char MsgSlotsControls[];

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

void LuckyWheels_Run(void) {
  s32 picks[8];
  s32 litLines[7];
  DrawRectangle draw[2];
  u8 *sheet;
  void *canvas;
  struct ReelEffectWork *work;
  struct ReelWork *state;
  struct ReelSave *save;
  u8 *entry;
  u8 *tiles;
  s32 frame;
  u32 count;
  s32 i;
  s32 j;
  s32 k;

  sheet = HeapSlot_Alloc(100, 0x60e);
  canvas = HeapSlot_Alloc(96, 0x8000);
  work = HeapSlot_AllocEwram(92, 0x7b7c);
  state = HeapSlot_AllocEwram(180, 0x61c);
  tiles = Ram_MapCellBuffer;
  save = (struct ReelSave *)Data_0200024c;
  Resource_ReservedNoOp((s32)&ResourceId_FarCall00C);
  save->wonPrizes[0] = -1;
  state->repeat = 0;
  state->bet = 1;
  Task_ResetAll();
  gRenderOamEnabled = 0;

  count = 0;
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 32; i++, j += 2) {
      if (i >= 5 && i <= 24 && k > 2 && k <= 13) {
        *(volatile u16 *)(0x06002800 + j) = 0xa1a6;
      } else if (i > 29) {
        *(volatile u16 *)(0x06002800 + j) = 0;
      } else {
        *(volatile u16 *)(0x06002800 + j) = 0xa1a8 + count;
        count++;
      }
    }
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), sheet);
  entry = Resource_GetTableEntry((s32)&ResourceId_SlotsMachinePicture);
  Dma_Set(entry, (void *)0x05000140, 0x84000008, REG_DMA3);
  entry += 32;
  Resource_DecodeType01(entry, tiles);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 30; i++) {
      if (!(i >= 5 && i <= 24 && k > 2 && k <= 13)) {
        Dma_Set(tiles + (k * 30 + i) * 32, (void *)(0x0600b500 + j),
                0x84000008, REG_DMA3);
        j += 32;
      }
    }
  }
  Iwram_ClearWords((void *)0x06002d00, 0x300);
  j = 0;
  for (k = 0; k != 20; k++) {
    for (i = 0; i != 32; i++, j += 2) {
      if (k < 2 || k > 16)
        *(volatile u16 *)(0x06003000 + j) = 0xbf;
      else
        *(volatile u16 *)(0x06003000 + j) = k * 32 + i + 148;
    }
  }
  REG_BG1CNT = 0x509;
  REG_BG2CNT = 0x680;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  REG_WININ = 0x3737;
  REG_WINOUT = 0x2727;
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = 0x1010;
  REG_BG1HOFS = 0;
  REG_BG2HOFS = 0;
  REG_BG1VOFS = 0xff60;
  REG_BG2VOFS = 0xff60;
  REG_WIN0H = 0x28c8;
  REG_WIN0V = 0x1878;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0xa0;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0xa0;

  {
    u16 *bgShade = work->bgShade;
    u16 *shade = work->objShade;
    s32 size = 0x8000;

    state->state = 0;
    state->cursor = 0;
    state->spins = 0;
    work->frame = 0;
    state->timer = 0;
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet), (void *)0x05000000, 0x84000020,
            REG_DMA3);
    *(u16 *)0x05000080 = 0x2f8b;
    *(u16 *)0x05000082 = 0x5bf6;
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsIcons);
    Dma_Set(entry, (void *)0x05000200, 0x84000078, REG_DMA3);
    entry += 480;
    Resource_DecodeType01(entry, tiles);
    Dma_Set(tiles, (void *)0x06010000, 0x84001b30, REG_DMA3);
    entry = Resource_GetTableEntry((s32)&ResourceId_SlotsJackpotBanner);
    Dma_Set(entry, (void *)0x050003e0, 0x84000008, REG_DMA3);
    entry += 32;
    Resource_DecodeType01(entry, tiles);
    Dma_Set(tiles, (void *)0x06016e00, 0x84000480, REG_DMA3);
    WindowSystem_InitFieldFar();
    Func_081b336c();

    for (i = 0; i != 5; i++) {
      struct Reel *reel = &state->reels[i];

      reel->position = 8;
      reel->held = 0;
      reel->stop = -1;
      for (k = 0; k != 21; k++)
        reel->symbols[k] = Random16() % 5;
    }
    for (i = 0; i != 5; i++) {
      struct Reel *reel = &state->reels[i];

      for (j = 0; j != 8; j++) {
        picks[j] = Random16() % 21;
        for (k = 0; k != j; k++) {
          if (picks[j] == picks[k]) {
            j--;
            break;
          }
        }
      }
      for (k = 0; k != 8; k++) {
        s32 rank = k;

        if (rank > 5)
          rank = 5;
        reel->symbols[picks[k]] = rank;
      }
    }

    Resource_FarCall00C(104, 8, 7, 3, 1);
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    Resource_FarCall00C(188, 8, 7, 3, 0);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    Iwram_FillWordsEntry(canvas, size, 0);
    Dma_Set(canvas, (void *)0x06003500, 0x84002000, REG_DMA3);
    Dma_Set((void *)0x05000000, bgShade, 0x84000080, REG_DMA3);
    Dma_Set((void *)0x05000200, shade, 0x84000080, REG_DMA3);
    Graphics_ScaleRgb555(shade, (u16 *)0x05000200, 0, 0x100);
    Graphics_ScaleRgb555(bgShade, (u16 *)0x05000000, 0, 0x100);
    REG_DISPCNT = 0x3740;
    if (PartyInventory_CountItemFar(228) == 1) {
      s32 window = Window_CreateFar(2, 16, 26, 4, 6);

      state->window = window;
      Text_DrawCharacterAtOffsetFar((s32)MsgSlotsControls, window, 0, 0);
    } else {
      s32 window = Window_CreateFar(2, 16, 26, 4, 6);
      s32 message;

      state->window = window;
      message = (s32)MsgSlotsChangeBet;
      Text_DrawCharacterAtOffsetFar(message, window, 0, 0);
      Text_DrawCharacterAtOffsetFar(message + 1, state->window, 0, 8);
    }
    work->transferPending = 0;
    Task_Add(Func_081b243c, 0x480);
    Task_Add(Func_081b3294, 0xc80);
    Task_Add(BattleStaging_ProcessPendingTileTransfer,
                                  0xc80);

    for (frame = 0; state->state != 10; frame++) {
      if (frame <= 16) {
        Graphics_ScaleRgb555(shade, (u16 *)0x05000200, frame << 12, 0x100);
        Graphics_ScaleRgb555(bgShade, (u16 *)0x05000000, frame << 12,
                             0x100);
      }
      if (state->state == 3) {
        s32 phase = frame % 80;

        if (phase <= 15)
          Func_081b21ec((s32)&ResourceId_MercuryDjinnSheet);
        else if (phase <= 31)
          Func_081b21ec((s32)&ResourceId_VenusDjinnSheet);
        else if (phase <= 47)
          Func_081b21ec((s32)&ResourceId_EmberStreakSheet);
        else if (phase <= 63)
          Func_081b21ec((s32)&ResourceId_LimePalette);
        else
          Func_081b21ec((s32)&ResourceId_JupiterDjinnSheet);
        if (state->timer <= 15)
          Palette_DarkenSceneStep();
        if (state->timer > 16 && (frame & 7) == 0) {
          s32 x = (Random16() & 127) + 56;
          s32 y = (Random16() & 31) + 48;
          struct EffectStep *spark = &SPARKS[(frame / 8 & 3) * 256];

          for (j = 0; j != 256; j++) {
            s32 speed;
            s32 angle;

            speed = 255;
            speed &= Random16();
            angle = Random16() & 0xffff;
            spark->x = x << 16;
            spark->y = y << 16;
            spark->velocityX = Trig_Sin(angle) * (speed + 64) >> 6;
            spark->velocityY = -(Trig_Cos(angle) * (speed + 64)) >> 6;
            spark->variant = (Random16() & 15) + 16;
            spark++;
          }
        }
        for (j = 0; j != 1024; j++) {
          struct EffectStep *spark = &SPARKS[j];

          if (spark->variant > 0) {
            s32 x;

            spark->variant--;
            x = spark->x;
            if (x >= 0 && x <= 0xffffff) {
              s32 y = spark->y;

              if (y <= 0x7fffff) {
                if (y >= 0) {
                  s32 px = x >> 16;
                  s32 py = y >> 16;
                  s32 size = spark->variant / 12 + 1;

                  draw[j & 1](canvas,
                              sheet + gLuckyWheels_SparkCellOffsets[size - 1],
                              px - size, py - size, size * 2, size * 2);
                }
              }
            }
            spark->x += spark->velocityX;
            spark->y += spark->velocityY;
            spark->velocityX = spark->velocityX * 60 / 64;
            spark->velocityY = spark->velocityY * 60 / 64;
          }
        }
      }
      if (state->state == 0 || state->state == 2) {
        for (j = 0; j != 7; j++)
          litLines[j] = 0;
        if (state->state == 0) {
          litLines[3] = 1;
          if (state->bet > 1) {
            litLines[4] = 1;
            litLines[2] = 1;
          }
          if (state->bet > 2) {
            litLines[5] = 1;
            litLines[1] = 1;
          }
          if (state->bet > 3) {
            litLines[6] = 1;
            litLines[0] = 1;
          }
        } else if ((frame & 7) <= 3) {
          for (j = 0; j != 7; j++)
            litLines[j] = state->winningLines[j];
        }
        for (j = 0; j != 3; j++) {
          s32 color = j != 1;

          color = 65 - color;
          if (litLines[1])
            Func_081b22b8(20, j + 19, 200, j + 19, color);
          if (litLines[2])
            Func_081b22b8(28, j + 35, 200, j + 35, color);
          if (litLines[3])
            Func_081b22b8(20, j + 51, 200, j + 51, color);
          if (litLines[4])
            Func_081b22b8(28, j + 67, 200, j + 67, color);
          if (litLines[5])
            Func_081b22b8(20, j + 83, 200, j + 83, color);
          if (litLines[0])
            Func_081b22b8(28, j + 5, 200, j + 91, color);
          if (litLines[6])
            Func_081b22b8(28, j + 97, 200, j + 11, color);
        }
      }
      work->transferPending = 1;
      wait(1);
    }

    for (j = 0; j != 17; j++) {
      Graphics_ScaleRgb555(shade, (u16 *)0x05000200, 0x10000 - (j << 12),
                           0x100);
      Graphics_ScaleRgb555(bgShade, (u16 *)0x05000000,
                           0x10000 - (j << 12), 0x100);
      wait(1);
    }
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    Task_Remove(BattleStaging_ProcessPendingTileTransfer);
    Task_Remove(Func_081b243c);
    Task_Remove(Func_081b3294);
    HeapSlot_Release(180);
    HeapSlot_Release(96);
    HeapSlot_Release(92);
    HeapSlot_Release(100);
  }
}
