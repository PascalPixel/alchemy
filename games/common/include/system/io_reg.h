#ifndef GUARD_IO_REG_H
#define GUARD_IO_REG_H

#include "types.h"

#define REG_DISPCNT (*(volatile u16 *)0x04000000)
#define REG_DISPSTAT (*(volatile u16 *)0x04000004)
#define REG_VCOUNT (*(volatile u16 *)0x04000006)
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)
#define REG_BG2CNT (*(volatile u16 *)0x0400000c)
#define REG_BG3CNT (*(volatile u16 *)0x0400000e)

#define BGCNT_CHAR_BASE(block) ((block) << 2)
#define BGCNT_SCREEN_BASE(block) ((block) << 8)
#define BGCNT_256_COLORS 0x0080

enum DisplayControl {
  DISPCNT_OBJ_1D_MAP = 0x0040,
  DISPCNT_BG0_ON = 0x0100,
  DISPCNT_BG1_ON = 0x0200,
  DISPCNT_BG2_ON = 0x0400,
  DISPCNT_BG3_ON = 0x0800,
  DISPCNT_OBJ_ON = 0x1000,
  DISPCNT_WIN0_ON = 0x2000,
  DISPCNT_WIN1_ON = 0x4000
};
#define REG_BG1HOFS (*(volatile u16 *)0x04000014)
#define REG_BG1VOFS (*(volatile u16 *)0x04000016)
#define REG_BG2HOFS (*(volatile u16 *)0x04000018)
#define REG_BG2VOFS (*(volatile u16 *)0x0400001a)
#define REG_BG3HOFS (*(volatile u16 *)0x0400001c)
#define REG_BG3VOFS (*(volatile u16 *)0x0400001e)

#define REG_BG2PA (*(volatile u16 *)0x04000020)
#define REG_BG2PB (*(volatile u16 *)0x04000022)
#define REG_BG2PC (*(volatile u16 *)0x04000024)
#define REG_BG2PD (*(volatile u16 *)0x04000026)
#define REG_BG2X (*(volatile s32 *)0x04000028)
#define REG_BG2Y (*(volatile s32 *)0x0400002c)
#define REG_BG3PA (*(volatile u16 *)0x04000030)
#define REG_BG3PB (*(volatile u16 *)0x04000032)
#define REG_BG3PC (*(volatile u16 *)0x04000034)
#define REG_BG3PD (*(volatile u16 *)0x04000036)
#define REG_BG3X (*(volatile s32 *)0x04000038)
#define REG_BG3Y (*(volatile s32 *)0x0400003c)

#define REG_WIN0H (*(volatile u16 *)0x04000040)
#define REG_WIN1H (*(volatile u16 *)0x04000042)
#define REG_WIN0V (*(volatile u16 *)0x04000044)
#define REG_WIN1V (*(volatile u16 *)0x04000046)
#define REG_WININ (*(volatile u16 *)0x04000048)
#define REG_WINOUT (*(volatile u16 *)0x0400004a)
#define REG_MOSAIC (*(volatile u16 *)0x0400004c)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
// A REG_BLDALPHA value: top and bottom are the two targets' weights, out of 16.
#define BLEND_ALPHA(top, bottom) ((top) | ((bottom) << 8))
#define REG_BLDY (*(volatile u16 *)0x04000054)

#define BG_CANVAS_MAP ((volatile s16 *)0x06003800)
#define REG_BG0HOFS ((void *)0x04000010)

#define REG_DMA0 ((volatile u16 *)0x040000b0)

#define REG_DMA3 ((volatile u32 *)0x040000d4)

#define PLTT_BACKDROP (*(volatile u16 *)0x05000000)

#define BG_PLTT ((volatile u16 *)0x05000000)
#define BG_PLTT_COLOR(palette, color) (BG_PLTT[(palette) * 16 + (color)])
#define OBJ_PLTT ((volatile u16 *)0x05000200)
#define OBJ_PLTT_COLOR(palette, color) (OBJ_PLTT[(palette) * 16 + (color)])

#define BG_CHAR_BLOCK(n) ((u8 *)0x06000000 + (n) * 0x4000)
#define BG_SCREEN_BLOCK(n) ((u8 *)0x06000000 + (n) * 0x800)
#define TILE_SIZE_4BPP 32

#define REG_TM3CNT (*(volatile u32 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e)

#define REG_SIODATA32 ((volatile u32 *)0x04000120)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128)
#define REG_SIODATA8 (*(volatile u16 *)0x0400012a)
#define REG_KEYINPUT (*(volatile u16 *)0x04000130)
#define REG_KEYCNT (*(volatile u16 *)0x04000132)
#define REG_RCNT (*(volatile u16 *)0x04000134)

#define DMA_ENABLE 0x8000
#define DMA_START_HBLANK_REPEAT 0x3a00
#define DMA_32BIT 0x04000000
#define DMA_SRC_FIXED32 0x01000000
#define DMA_ENABLE32 0x80000000

#define OAM ((void *)0x07000000)
enum KeyMask {
  KEY_A = 0x001,
  KEY_B = 0x002,
  KEY_SELECT = 0x004,
  KEY_START = 0x008,
  KEY_RIGHT = 0x010,
  KEY_LEFT = 0x020,
  KEY_UP = 0x040,
  KEY_DOWN = 0x080,
  KEY_R = 0x100,
  KEY_L = 0x200,
  KEYS_HORIZONTAL = KEY_RIGHT | KEY_LEFT,
  KEYS_VERTICAL = KEY_UP | KEY_DOWN,
  KEYS_DPAD = KEYS_HORIZONTAL | KEYS_VERTICAL,
  KEYS_SHOULDERS = KEY_R | KEY_L
};

#define KEYS_MASK 0x3ff

#define REG_BG0CNT (*(volatile u16 *)0x04000008)
#define REG_BG0VOFS (*(volatile u16 *)0x04000012)
#define REG_SOUNDCNT_H (*(volatile u16 *)0x04000082)
#define REG_SOUNDBIAS (*(volatile u16 *)0x04000088)
#define REG_FIFO_A (*(volatile u32 *)0x040000a0)
#define REG_FIFO_B (*(volatile u32 *)0x040000a4)
#define REG_DMA0SAD (*(volatile u32 *)0x040000b0)
#define REG_DMA0DAD (*(volatile u32 *)0x040000b4)
#define REG_DMA0CNT (*(volatile u32 *)0x040000b8)
#define REG_DMA0CNT_H (*(volatile u16 *)0x040000ba)
#define REG_DMA1SAD (*(volatile u32 *)0x040000bc)
#define REG_DMA1DAD (*(volatile u32 *)0x040000c0)
#define REG_DMA1CNT (*(volatile u32 *)0x040000c4)
#define REG_DMA1CNT_H (*(volatile u16 *)0x040000c6)
#define REG_DMA2SAD (*(volatile u32 *)0x040000c8)
#define REG_DMA2DAD (*(volatile u32 *)0x040000cc)
#define REG_DMA2CNT (*(volatile u32 *)0x040000d0)
#define REG_DMA2CNT_H (*(volatile u16 *)0x040000d2)
#define REG_DMA3SAD (*(volatile u32 *)0x040000d4)
#define REG_DMA3DAD (*(volatile u32 *)0x040000d8)
#define REG_DMA3CNT (*(volatile u32 *)0x040000dc)
#define REG_DMA3CNT_H (*(volatile u16 *)0x040000de)
#define REG_TM0CNT_L (*(volatile u16 *)0x04000100)
#define REG_TM0CNT_H (*(volatile u16 *)0x04000102)
#define REG_TM1CNT_L (*(volatile u16 *)0x04000104)
#define REG_TM1CNT_H (*(volatile u16 *)0x04000106)
#define REG_TM2CNT_L (*(volatile u16 *)0x04000108)
#define REG_TM2CNT_H (*(volatile u16 *)0x0400010a)

#define REG_IE (*(volatile u16 *)0x04000200)
#define REG_IF (*(volatile u16 *)0x04000202)
#define REG_WAITCNT (*(volatile u16 *)0x04000204)
#define REG_IME (*(volatile u16 *)0x04000208)

#define KEYCNT_SOFT_RESET 0xc00f
#define KEYCNT_SHOULDERS_WAKE 0xc300
#define KEYCNT_SLEEP_WAKE 0xc304

#define WAITCNT_GAME 0x4014

#endif
