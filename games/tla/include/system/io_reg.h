#ifndef GUARD_IO_REG_H
#define GUARD_IO_REG_H

#include "types.h"

#define REG_DISPCNT (*(volatile u16 *)0x04000000)
#define REG_DISPSTAT (*(volatile u16 *)0x04000004)
#define REG_VCOUNT (*(volatile u16 *)0x04000006)
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)
#define REG_BG2CNT (*(volatile u16 *)0x0400000c)
#define REG_BG3CNT (*(volatile u16 *)0x0400000e)

#define BGCNT_PRIORITY(n) (n)
#define BGCNT_CHAR_BASE(block) ((block) << 2)
#define BGCNT_MOSAIC 0x0040
#define BGCNT_256_COLORS 0x0080
#define BGCNT_SCREEN_BASE(block) ((block) << 8)
/// Affine backgrounds only: wrap around instead of showing the backdrop.
#define BGCNT_WRAP 0x2000
/// The background's size: for an affine one, 0 to 3 are 128, 256, 512 and 1024
/// pixels square.
#define BGCNT_SIZE(n) ((n) << 14)

enum DisplayControl {
  DISPCNT_MODE_0 = 0x0000, ///< four text backgrounds
  DISPCNT_MODE_1 = 0x0001, ///< BG0 and BG1 text, BG2 affine
  DISPCNT_MODE_2 = 0x0002, ///< BG2 and BG3 affine
  DISPCNT_OBJ_1D_MAP = 0x0040,
  DISPCNT_BG0_ON = 0x0100,
  DISPCNT_BG1_ON = 0x0200,
  DISPCNT_BG2_ON = 0x0400,
  DISPCNT_BG3_ON = 0x0800,
  DISPCNT_OBJ_ON = 0x1000,
  DISPCNT_WIN0_ON = 0x2000,
  DISPCNT_WIN1_ON = 0x4000,
  DISPCNT_OBJWIN_ON = 0x8000 ///< the sprite window
};
#define REG_BG1HOFS (*(volatile u16 *)0x04000014)
#define REG_BG1VOFS (*(volatile u16 *)0x04000016)
#define REG_BG2HOFS (*(volatile u16 *)0x04000018)
#define REG_BG2VOFS (*(volatile u16 *)0x0400001a)
#define REG_BG3HOFS (*(volatile u16 *)0x0400001c)
#define REG_BG3VOFS (*(volatile u16 *)0x0400001e)

/// The BG2/BG3 affine matrix entries are signed 8.8 fixed point: texels
/// stepped per screen pixel. Q_8_8(1) draws the layer at its own size and
/// Q_8_8(0.5) twice as large.
#define Q_8_8(n) ((s16)((n) * 256))

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
/// A REG_WINxH or REG_WINxV value: the window covers start up to, but not
/// including, end.
#define WIN_RANGE(start, end) (((start) << 8) | (end))
#define REG_WININ (*(volatile u16 *)0x04000048)
#define REG_WINOUT (*(volatile u16 *)0x0400004a)

/// REG_WININ: the layers shown inside window 0 (low byte) and window 1 (high
/// byte). CLR lets the colour effect of REG_BLDCNT apply there.
#define WININ_WIN0_BG0 0x0001
#define WININ_WIN0_BG1 0x0002
#define WININ_WIN0_BG2 0x0004
#define WININ_WIN0_BG3 0x0008
#define WININ_WIN0_OBJ 0x0010
#define WININ_WIN0_CLR 0x0020
#define WININ_WIN0_ALL 0x003f
#define WININ_WIN1_BG0 0x0100
#define WININ_WIN1_BG1 0x0200
#define WININ_WIN1_BG2 0x0400
#define WININ_WIN1_BG3 0x0800
#define WININ_WIN1_OBJ 0x1000
#define WININ_WIN1_CLR 0x2000
#define WININ_WIN1_ALL 0x3f00

/// REG_WINOUT: the layers shown outside both windows (low byte) and inside the
/// sprite window (high byte).
#define WINOUT_WIN01_BG0 0x0001
#define WINOUT_WIN01_BG1 0x0002
#define WINOUT_WIN01_BG2 0x0004
#define WINOUT_WIN01_BG3 0x0008
#define WINOUT_WIN01_OBJ 0x0010
#define WINOUT_WIN01_CLR 0x0020
#define WINOUT_WIN01_ALL 0x003f
#define WINOUT_WINOBJ_BG0 0x0100
#define WINOUT_WINOBJ_BG1 0x0200
#define WINOUT_WINOBJ_BG2 0x0400
#define WINOUT_WINOBJ_BG3 0x0800
#define WINOUT_WINOBJ_OBJ 0x1000
#define WINOUT_WINOBJ_CLR 0x2000
#define WINOUT_WINOBJ_ALL 0x3f00

#define REG_MOSAIC (*(volatile u16 *)0x0400004c)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)

/// REG_BLDCNT: the first target layers, the colour effect, then the second
/// target layers that a blend mixes the first ones with.
#define BLDCNT_TGT1_BG0 0x0001
#define BLDCNT_TGT1_BG1 0x0002
#define BLDCNT_TGT1_BG2 0x0004
#define BLDCNT_TGT1_BG3 0x0008
#define BLDCNT_TGT1_OBJ 0x0010
#define BLDCNT_TGT1_BD 0x0020
#define BLDCNT_TGT1_ALL 0x003f
#define BLDCNT_EFFECT_NONE 0x0000
#define BLDCNT_EFFECT_BLEND 0x0040   ///< weighted by REG_BLDALPHA
#define BLDCNT_EFFECT_LIGHTEN 0x0080 ///< towards white by REG_BLDY
#define BLDCNT_EFFECT_DARKEN 0x00c0  ///< towards black by REG_BLDY
#define BLDCNT_TGT2_BG0 0x0100
#define BLDCNT_TGT2_BG1 0x0200
#define BLDCNT_TGT2_BG2 0x0400
#define BLDCNT_TGT2_BG3 0x0800
#define BLDCNT_TGT2_OBJ 0x1000
#define BLDCNT_TGT2_BD 0x2000
#define BLDCNT_TGT2_ALL 0x3f00

#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
/// A REG_BLDALPHA value: top and bottom are the two targets' weights, out
/// of 16.
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

/// A 15-bit colour from its red, green and blue levels (0 to 31 each), as
/// the palette stores it: blue in the high bits.
#define RGB(r, g, b) (((b) << 10) | ((g) << 5) | (r))

#define BG_CHAR_BLOCK(n) ((u8 *)0x06000000 + (n) * 0x4000)
#define BG_SCREEN_BLOCK(n) ((u8 *)0x06000000 + (n) * 0x800)
#define TILE_SIZE_4BPP 32
/// Sprite tile memory, as pret names it: the 32 KB from 0x06010000.
#define OBJ_VRAM0 0x06010000

#define REG_TM3CNT (*(volatile u32 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e)

/// TMxCNT_H flags, as pret names them.
#define TIMER_INTR_ENABLE 0x0040
#define TIMER_ENABLE 0x0080

#define REG_SIODATA32 ((volatile u32 *)0x04000120)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128)
#define REG_SIODATA8 (*(volatile u16 *)0x0400012a)
#define REG_KEYINPUT (*(volatile u16 *)0x04000130)
#define REG_KEYCNT (*(volatile u16 *)0x04000132)
#define REG_RCNT (*(volatile u16 *)0x04000134)

/// SIOCNT flags, as pret names them.
#define SIO_115200_BPS 0x0003
#define SIO_START 0x0080
#define SIO_32BIT_MODE 0x1000
#define SIO_MULTI_MODE 0x2000
#define SIO_INTR_ENABLE 0x4000

/// RCNT's mode: 0 leaves the port to SIOCNT's serial modes; bit 15 alone
/// makes it general-purpose pins.
#define RCNT_SIO_MODE 0x0000
#define RCNT_GPIO_MODE 0x8000

/// DMAxCNT_H flags, as pret names them. A 32-bit control word takes them
/// shifted above its count: (DMA_ENABLE | DMA_32BIT) << 16 | words.
#define DMA_DEST_INC 0x0000
#define DMA_DEST_DEC 0x0020
#define DMA_DEST_FIXED 0x0040
#define DMA_DEST_RELOAD 0x0060
#define DMA_SRC_INC 0x0000
#define DMA_SRC_DEC 0x0080
#define DMA_SRC_FIXED 0x0100
#define DMA_REPEAT 0x0200
#define DMA_16BIT 0x0000
#define DMA_32BIT 0x0400
#define DMA_DREQ_ON 0x0800
#define DMA_START_NOW 0x0000
#define DMA_START_VBLANK 0x1000
#define DMA_START_HBLANK 0x2000
#define DMA_START_SPECIAL 0x3000
#define DMA_INTR_ENABLE 0x4000
#define DMA_ENABLE 0x8000
/// A DMA control's start timing (bits 12 and 13), cartridge request (bit 11)
/// and repeat (bit 9): the bits cleared before a channel is stopped.
#define DMA_START_TIMING_DRQ_REPEAT                                            \
  (DMA_START_SPECIAL | DMA_DREQ_ON | DMA_REPEAT)

/// DMA control words: copy n words or halfwords at once, or one halfword or
/// word each HBlank to a register reloaded every line (a scanline table that
/// drives a window edge, a scroll or an affine register).
#define DMA_COPY_WORDS(n) ((DMA_ENABLE | DMA_32BIT) << 16 | (n))
#define DMA_COPY_HALFWORDS(n) (DMA_ENABLE << 16 | (n))
#define DMA_HBLANK_HALFWORD_PER_LINE                                           \
  ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD) << 16 | 1)
#define DMA_HBLANK_WORD_PER_LINE                                               \
  ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT)  \
       << 16 |                                                                 \
   1)

/// BIOS CpuSet control flags, above its count.
#define CPU_SET_SRC_FIXED 0x01000000
#define CPU_SET_32BIT 0x04000000

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
/// SOUNDCNT_H flags, as pret names them: the mixing volumes, then each direct
/// sound channel's speakers, timer and FIFO reset.
#define SOUND_CGB_MIX_FULL 0x0002
#define SOUND_A_MIX_FULL 0x0004
#define SOUND_B_MIX_FULL 0x0008
#define SOUND_ALL_MIX_FULL                                                     \
  (SOUND_CGB_MIX_FULL | SOUND_A_MIX_FULL | SOUND_B_MIX_FULL)
#define SOUND_A_RIGHT_OUTPUT 0x0100
#define SOUND_A_LEFT_OUTPUT 0x0200
#define SOUND_A_TIMER_0 0x0000
#define SOUND_A_FIFO_RESET 0x0800
#define SOUND_B_RIGHT_OUTPUT 0x1000
#define SOUND_B_LEFT_OUTPUT 0x2000
#define SOUND_B_TIMER_0 0x0000
#define SOUND_B_FIFO_RESET 0x8000
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
