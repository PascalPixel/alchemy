#ifndef ALCHEMY_IO_REG_H
#define ALCHEMY_IO_REG_H

/// @file
/// The hardware's I/O registers and their bits, as pret names them.

#include "types.h"

#define REG_DISPCNT (*(volatile u16 *)0x04000000)  ///< mode, layers, windows
#define REG_DISPSTAT (*(volatile u16 *)0x04000004) ///< blank flags and IRQs
// DISPSTAT's interrupt enables, as pret names them; each is
// DISPSTAT_VBLANK_INTR shifted by its IRQ_ number.
#define DISPSTAT_VBLANK_INTR 0x0008 ///< at the start of each VBlank
#define DISPSTAT_HBLANK_INTR 0x0010 ///< at the end of each line
#define DISPSTAT_VCOUNT_INTR 0x0020 ///< on the DISPSTAT_VCOUNT_LINE line
/// DISPSTAT's high byte: the line the VCount interrupt fires on (the LYC
/// compare).
#define DISPSTAT_VCOUNT_LINE(line) ((line) << 8)
#define REG_VCOUNT (*(volatile u16 *)0x04000006) ///< line being drawn, 0..227
// Each background's control: its priority, tile and map blocks, colours and
// size, composed from the BGCNT_ values below.
#define REG_BG1CNT (*(volatile u16 *)0x0400000a) ///< BG1
#define REG_BG2CNT (*(volatile u16 *)0x0400000c) ///< BG2
#define REG_BG3CNT (*(volatile u16 *)0x0400000e) ///< BG3

#define BGCNT_PRIORITY(n) (n) ///< draw order, 0 in front to 3 behind
/// The 16 KB character block, 0 to 3, the layer's tiles are read from.
#define BGCNT_CHAR_BASE(block) ((block) << 2)
#define BGCNT_MOSAIC 0x0040     ///< applies REG_MOSAIC's block size
#define BGCNT_256_COLORS 0x0080 ///< 8bpp tiles, not 16-colour 4bpp ones
/// The 2 KB screen block, 0 to 31, the layer's map is read from.
#define BGCNT_SCREEN_BASE(block) ((block) << 8)
/// Affine backgrounds only: wrap around instead of showing the backdrop.
#define BGCNT_WRAP 0x2000
/// The background's size: for an affine one, 0 to 3 are 128, 256, 512 and 1024
/// pixels square.
#define BGCNT_SIZE(n) ((n) << 14)

/// REG_DISPCNT's video modes and bits: each _ON bit shows its layer or turns
/// its window on.
enum DisplayControl {
  DISPCNT_MODE_0 = 0x0000,     ///< four text backgrounds
  DISPCNT_MODE_1 = 0x0001,     ///< BG0 and BG1 text, BG2 affine
  DISPCNT_MODE_2 = 0x0002,     ///< BG2 and BG3 affine
  DISPCNT_OBJ_1D_MAP = 0x0040, ///< sprite tiles in one run, not a 2D sheet
  DISPCNT_BG0_ON = 0x0100,
  DISPCNT_BG1_ON = 0x0200,
  DISPCNT_BG2_ON = 0x0400,
  DISPCNT_BG3_ON = 0x0800,
  DISPCNT_OBJ_ON = 0x1000,
  DISPCNT_WIN0_ON = 0x2000,
  DISPCNT_WIN1_ON = 0x4000,
  DISPCNT_OBJWIN_ON = 0x8000 ///< the sprite window OBJ_MODE_WINDOW draws
};
// The text backgrounds' scroll offsets, write-only: the map pixel, 0 to 511,
// at the screen's left edge (HOFS) or top edge (VOFS).
#define REG_BG1HOFS (*(volatile u16 *)0x04000014) ///< BG1, horizontal
#define REG_BG1VOFS (*(volatile u16 *)0x04000016) ///< BG1, vertical
#define REG_BG2HOFS (*(volatile u16 *)0x04000018) ///< BG2, horizontal
#define REG_BG2VOFS (*(volatile u16 *)0x0400001a) ///< BG2, vertical
#define REG_BG3HOFS (*(volatile u16 *)0x0400001c) ///< BG3, horizontal
#define REG_BG3VOFS (*(volatile u16 *)0x0400001e) ///< BG3, vertical

/// The BG2/BG3 affine matrix entries are signed 8.8 fixed point: texels
/// stepped per screen pixel. Q_8_8(1) draws the layer at its own size and
/// Q_8_8(0.5) twice as large.
#define Q_8_8(n) ((s16)((n) * 256))

// The affine backgrounds' matrices and origins, write-only. PA and PC step
// the map point's x and y for each pixel along a line, PB and PD for each
// line down; X and Y, in 20.8 fixed point, are the map point drawn at the
// screen's top left.
#define REG_BG2PA (*(volatile u16 *)0x04000020) ///< BG2: x step a pixel
#define REG_BG2PB (*(volatile u16 *)0x04000022) ///< BG2: x step a line
#define REG_BG2PC (*(volatile u16 *)0x04000024) ///< BG2: y step a pixel
#define REG_BG2PD (*(volatile u16 *)0x04000026) ///< BG2: y step a line
#define REG_BG2X (*(volatile s32 *)0x04000028)  ///< BG2: origin x
#define REG_BG2Y (*(volatile s32 *)0x0400002c)  ///< BG2: origin y
#define REG_BG3PA (*(volatile u16 *)0x04000030) ///< BG3: x step a pixel
#define REG_BG3PB (*(volatile u16 *)0x04000032) ///< BG3: x step a line
#define REG_BG3PC (*(volatile u16 *)0x04000034) ///< BG3: y step a pixel
#define REG_BG3PD (*(volatile u16 *)0x04000036) ///< BG3: y step a line
#define REG_BG3X (*(volatile s32 *)0x04000038)  ///< BG3: origin x
#define REG_BG3Y (*(volatile s32 *)0x0400003c)  ///< BG3: origin y

// The two windows' edges, each a WIN_RANGE value in pixels.
#define REG_WIN0H (*(volatile u16 *)0x04000040) ///< window 0, left and right
#define REG_WIN1H (*(volatile u16 *)0x04000042) ///< window 1, left and right
#define REG_WIN0V (*(volatile u16 *)0x04000044) ///< window 0, top and bottom
#define REG_WIN1V (*(volatile u16 *)0x04000046) ///< window 1, top and bottom
/// A REG_WINxH or REG_WINxV value: the window covers start up to, but not
/// including, end.
#define WIN_RANGE(start, end) (((start) << 8) | (end))
#define REG_WININ (*(volatile u16 *)0x04000048)  ///< what windows 0 and 1 show
#define REG_WINOUT (*(volatile u16 *)0x0400004a) ///< what the rest shows

// REG_WININ: the layers shown inside window 0 (low byte) and window 1 (high
// byte). CLR lets the colour effect of REG_BLDCNT apply there.
#define WININ_WIN0_BG0 0x0001 ///< window 0: BG0
#define WININ_WIN0_BG1 0x0002 ///< window 0: BG1
#define WININ_WIN0_BG2 0x0004 ///< window 0: BG2
#define WININ_WIN0_BG3 0x0008 ///< window 0: BG3
#define WININ_WIN0_OBJ 0x0010 ///< window 0: sprites
#define WININ_WIN0_CLR 0x0020 ///< window 0: the colour effect
#define WININ_WIN0_ALL 0x003f ///< window 0: everything
#define WININ_WIN1_BG0 0x0100 ///< window 1: BG0
#define WININ_WIN1_BG1 0x0200 ///< window 1: BG1
#define WININ_WIN1_BG2 0x0400 ///< window 1: BG2
#define WININ_WIN1_BG3 0x0800 ///< window 1: BG3
#define WININ_WIN1_OBJ 0x1000 ///< window 1: sprites
#define WININ_WIN1_CLR 0x2000 ///< window 1: the colour effect
#define WININ_WIN1_ALL 0x3f00 ///< window 1: everything

// REG_WINOUT: the layers shown outside both windows (low byte) and inside the
// sprite window (high byte).
#define WINOUT_WIN01_BG0 0x0001  ///< outside: BG0
#define WINOUT_WIN01_BG1 0x0002  ///< outside: BG1
#define WINOUT_WIN01_BG2 0x0004  ///< outside: BG2
#define WINOUT_WIN01_BG3 0x0008  ///< outside: BG3
#define WINOUT_WIN01_OBJ 0x0010  ///< outside: sprites
#define WINOUT_WIN01_CLR 0x0020  ///< outside: the colour effect
#define WINOUT_WIN01_ALL 0x003f  ///< outside: everything
#define WINOUT_WINOBJ_BG0 0x0100 ///< sprite window: BG0
#define WINOUT_WINOBJ_BG1 0x0200 ///< sprite window: BG1
#define WINOUT_WINOBJ_BG2 0x0400 ///< sprite window: BG2
#define WINOUT_WINOBJ_BG3 0x0800 ///< sprite window: BG3
#define WINOUT_WINOBJ_OBJ 0x1000 ///< sprite window: sprites
#define WINOUT_WINOBJ_CLR 0x2000 ///< sprite window: the colour effect
#define WINOUT_WINOBJ_ALL 0x3f00 ///< sprite window: everything

#define REG_MOSAIC (*(volatile u16 *)0x0400004c) ///< mosaic block sizes
#define REG_BLDCNT (*(volatile u16 *)0x04000050) ///< colour effect, targets

// REG_BLDCNT: the first target layers, the colour effect, then the second
// target layers that a blend mixes the first ones with.
#define BLDCNT_TGT1_BG0 0x0001       ///< first target: BG0
#define BLDCNT_TGT1_BG1 0x0002       ///< first target: BG1
#define BLDCNT_TGT1_BG2 0x0004       ///< first target: BG2
#define BLDCNT_TGT1_BG3 0x0008       ///< first target: BG3
#define BLDCNT_TGT1_OBJ 0x0010       ///< first target: sprites
#define BLDCNT_TGT1_BD 0x0020        ///< first target: the backdrop
#define BLDCNT_TGT1_ALL 0x003f       ///< first target: every layer
#define BLDCNT_EFFECT_NONE 0x0000    ///< no effect, the targets as drawn
#define BLDCNT_EFFECT_BLEND 0x0040   ///< weighted by REG_BLDALPHA
#define BLDCNT_EFFECT_LIGHTEN 0x0080 ///< towards white by REG_BLDY
#define BLDCNT_EFFECT_DARKEN 0x00c0  ///< towards black by REG_BLDY
#define BLDCNT_TGT2_BG0 0x0100       ///< second target: BG0
#define BLDCNT_TGT2_BG1 0x0200       ///< second target: BG1
#define BLDCNT_TGT2_BG2 0x0400       ///< second target: BG2
#define BLDCNT_TGT2_BG3 0x0800       ///< second target: BG3
#define BLDCNT_TGT2_OBJ 0x1000       ///< second target: sprites
#define BLDCNT_TGT2_BD 0x2000        ///< second target: the backdrop
#define BLDCNT_TGT2_ALL 0x3f00       ///< second target: every layer

#define REG_BLDALPHA (*(volatile u16 *)0x04000052) ///< the blend's weights
/// A REG_BLDALPHA value: top and bottom are the two targets' weights, out
/// of 16.
#define BLEND_ALPHA(top, bottom) ((top) | ((bottom) << 8))
#define REG_BLDY (*(volatile u16 *)0x04000054) ///< fade level, 0 to 16

/// Screen block 7: BG2's affine map of a battle animation's canvas, a byte a
/// tile (ANIM_WRITE_CANVAS_MAP_ROW).
#define BG_CANVAS_MAP ((volatile s16 *)0x06003800)
/// BG0HOFS as an address for a DMA: copies of gBgScroll over all four
/// backgrounds' scroll registers start here, and Mind Read's HBlank DMA writes
/// one halfword a line into it.
#define REG_BG0HOFS ((void *)0x04000010)

/// DMA channel 0's registers as halfwords, for the scanline effects that set
/// the whole channel up.
#define REG_DMA0 ((volatile u16 *)0x040000b0)

/// DMA channel 3's registers as words: the channel Dma_Set copies with.
#define REG_DMA3 ((volatile u32 *)0x040000d4)

/// Background colour 0, the backdrop shown where no layer draws.
#define PLTT_BACKDROP (*(volatile u16 *)0x05000000)

#define BG_PLTT ((volatile u16 *)0x05000000) ///< 16 palettes of 16 colours
/// Colour `color` of 16-colour background palette `palette`.
#define BG_PLTT_COLOR(palette, color) (BG_PLTT[(palette) * 16 + (color)])
#define OBJ_PLTT ((volatile u16 *)0x05000200) ///< the sprites' 256 colours
/// Colour `color` of 16-colour sprite palette `palette`.
#define OBJ_PLTT_COLOR(palette, color) (OBJ_PLTT[(palette) * 16 + (color)])

/// A 15-bit colour from its red, green and blue levels (0 to 31 each), as
/// the palette stores it: blue in the high bits.
#define RGB(r, g, b) (((b) << 10) | ((g) << 5) | (r))

/// Background tile memory's 16 KB character block `n`, 0 to 3.
#define BG_CHAR_BLOCK(n) ((u8 *)0x06000000 + (n) * 0x4000)
/// The 2 KB screen block `n` of VRAM, 0 to 31, that holds a map.
#define BG_SCREEN_BLOCK(n) ((u8 *)0x06000000 + (n) * 0x800)
#define TILE_SIZE_4BPP 32 ///< bytes in an 8x8 tile of 16 colours
/// Sprite tile memory, as pret names it: the 32 KB from 0x06010000.
#define OBJ_VRAM0 0x06010000

/// Timer 3's reload value and control as one word; it paces the serial
/// transfers.
#define REG_TM3CNT (*(volatile u32 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e) ///< timer 3's control

/// TMxCNT_H flags, as pret names them.
#define TIMER_INTR_ENABLE 0x0040
#define TIMER_ENABLE 0x0080 ///< starts the timer counting

/// The serial data: the 32-bit mode's word, or multiplayer's four received
/// halfwords.
#define REG_SIODATA32 ((volatile u32 *)0x04000120)
/// Serial control read as a word, with SIODATA8 above it.
#define REG_SIOCNT (*(volatile u32 *)0x04000128)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128) ///< serial control alone
/// The 8-bit mode's data, which multiplayer mode sends from.
#define REG_SIODATA8 (*(volatile u16 *)0x0400012a)
#define REG_KEYINPUT (*(volatile u16 *)0x04000130) ///< a 0 bit is a held key
#define REG_KEYCNT (*(volatile u16 *)0x04000132)   ///< keys for the keypad IRQ
#define REG_RCNT (*(volatile u16 *)0x04000134)     ///< the serial port's mode

// SIOCNT flags, as pret names them.
#define SIO_115200_BPS 0x0003 ///< multiplayer's fastest baud rate
#define SIO_MULTI_SI                                                           \
  0x0004 ///< multiplayer: this console's SI pin, clear on the parent
#define SIO_MULTI_SD 0x0008    ///< multiplayer: every console is ready
#define SIO_ID 0x0030          ///< multiplayer: this console's id, 0 the parent
#define SIO_ID_SHIFT 4         ///< SIO_ID's lowest bit
#define SIO_ERROR 0x0040       ///< multiplayer: the last transfer failed
#define SIO_START 0x0080       ///< starts a transfer; set while one runs
#define SIO_32BIT_MODE 0x1000  ///< normal mode, 32 bits a transfer
#define SIO_MULTI_MODE 0x2000  ///< multiplayer mode
#define SIO_INTR_ENABLE 0x4000 ///< an interrupt when a transfer ends

/// RCNT's mode: 0 leaves the port to SIOCNT's serial modes; bit 15 alone
/// makes it general-purpose pins.
#define RCNT_SIO_MODE 0x0000
#define RCNT_GPIO_MODE 0x8000 ///< the port as general-purpose pins

// DMAxCNT_H flags, as pret names them. A 32-bit control word takes them
// shifted above its count: (DMA_ENABLE | DMA_32BIT) << 16 | words.
#define DMA_DEST_INC 0x0000      ///< destination steps up
#define DMA_DEST_DEC 0x0020      ///< destination steps down
#define DMA_DEST_FIXED 0x0040    ///< destination stays, as for a register
#define DMA_DEST_RELOAD 0x0060   ///< steps up, back to its start each repeat
#define DMA_SRC_INC 0x0000       ///< source steps up
#define DMA_SRC_DEC 0x0080       ///< source steps down
#define DMA_SRC_FIXED 0x0100     ///< source stays, as for a fill
#define DMA_REPEAT 0x0200        ///< runs again at each start condition
#define DMA_16BIT 0x0000         ///< moves halfwords
#define DMA_32BIT 0x0400         ///< moves words
#define DMA_DREQ_ON 0x0800       ///< channel 3: the cartridge requests it
#define DMA_START_NOW 0x0000     ///< as soon as it's enabled
#define DMA_START_VBLANK 0x1000  ///< at each VBlank
#define DMA_START_HBLANK 0x2000  ///< at each HBlank
#define DMA_START_SPECIAL 0x3000 ///< channels 1 and 2: a sound FIFO's request
#define DMA_INTR_ENABLE 0x4000   ///< an interrupt when it ends
#define DMA_ENABLE 0x8000        ///< arms the channel
/// A DMA control's start timing (bits 12 and 13), cartridge request (bit 11)
/// and repeat (bit 9): the bits cleared before a channel is stopped.
#define DMA_START_TIMING_DRQ_REPEAT                                            \
  (DMA_START_SPECIAL | DMA_DREQ_ON | DMA_REPEAT)

// DMA control words: copy n words or halfwords at once, or one halfword or
// word each HBlank to a register reloaded every line (a scanline table that
// drives a window edge, a scroll or an affine register).
#define DMA_COPY_WORDS(n) ((DMA_ENABLE | DMA_32BIT) << 16 | (n)) ///< n words
#define DMA_COPY_HALFWORDS(n) (DMA_ENABLE << 16 | (n)) ///< n halfwords
/// A halfword each HBlank, as for a scroll or a window edge.
#define DMA_HBLANK_HALFWORD_PER_LINE                                           \
  ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD) << 16 | 1)
/// A word each HBlank, as for a pair of registers written together.
#define DMA_HBLANK_WORD_PER_LINE                                               \
  ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT)  \
       << 16 |                                                                 \
   1)

/// BIOS CpuSet control flags, above its count.
#define CPU_SET_SRC_FIXED 0x01000000
#define CPU_SET_32BIT 0x04000000 ///< words rather than halfwords

#define OAM ((void *)0x07000000) ///< the sprites' attributes
/// OAM's size in bytes: 128 sprites of 8 bytes.
#define OAM_SIZE 0x400

/// A sprite's mode, OAM attribute 0's bits 11..10.
enum ObjMode {
  OBJ_MODE_NORMAL = 0,
  OBJ_MODE_SEMI_TRANSPARENT = 1, ///< blended as BLDCNT's first target
  OBJ_MODE_WINDOW = 2            ///< drawn into the OBJ window, not shown
};
/// The keys' bits as REG_KEYINPUT and REG_KEYCNT lay them out; the game's
/// key state (gKeysHeld and the rest) holds them set for a pressed key.
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

#define KEYS_MASK 0x3ff ///< the ten keys' bits; KEYINPUT reads 0 for a held key

#define REG_BG0CNT (*(volatile u16 *)0x04000008)  ///< BG0's control (BGCNT_)
#define REG_BG0VOFS (*(volatile u16 *)0x04000012) ///< BG0's vertical scroll
/// Direct sound's mixing: volumes, speakers, timers and FIFO resets.
#define REG_SOUNDCNT_H (*(volatile u16 *)0x04000082)
// SOUNDCNT_H flags, as pret names them: the mixing volumes, then each direct
// sound channel's speakers, timer and FIFO reset.
#define SOUND_CGB_MIX_FULL 0x0002 ///< the four tone channels at full volume
#define SOUND_A_MIX_FULL 0x0004   ///< direct sound A at full, not half
#define SOUND_B_MIX_FULL 0x0008   ///< direct sound B at full, not half
/// The tone channels and both direct sound channels all at full volume.
#define SOUND_ALL_MIX_FULL                                                     \
  (SOUND_CGB_MIX_FULL | SOUND_A_MIX_FULL | SOUND_B_MIX_FULL)
#define SOUND_A_RIGHT_OUTPUT 0x0100 ///< A to the right speaker
#define SOUND_A_LEFT_OUTPUT 0x0200  ///< A to the left speaker
#define SOUND_A_TIMER_0 0x0000      ///< A plays at timer 0's rate
#define SOUND_A_FIFO_RESET 0x0800   ///< empties A's FIFO
#define SOUND_B_RIGHT_OUTPUT 0x1000 ///< B to the right speaker
#define SOUND_B_LEFT_OUTPUT 0x2000  ///< B to the left speaker
#define SOUND_B_TIMER_0 0x0000      ///< B plays at timer 0's rate
#define SOUND_B_FIFO_RESET 0x8000   ///< empties B's FIFO
#define REG_SOUNDBIAS (*(volatile u16 *)0x04000088) ///< output bias, rate
#define REG_FIFO_A (*(volatile u32 *)0x040000a0)    ///< A's samples, 4 a write
#define REG_FIFO_B (*(volatile u32 *)0x040000a4)    ///< B's samples, 4 a write
// The four DMA channels: source, destination, then the count with the control
// halfword (CNT_H, the DMA_ flags) above it.
#define REG_DMA0SAD (*(volatile u32 *)0x040000b0) ///< channel 0: source
#define REG_DMA0DAD (*(volatile u32 *)0x040000b4) ///< channel 0: destination
#define REG_DMA0CNT (*(volatile u32 *)0x040000b8) ///< channel 0: count, control
#define REG_DMA0CNT_H (*(volatile u16 *)0x040000ba) ///< channel 0: control
#define REG_DMA1SAD (*(volatile u32 *)0x040000bc)   ///< channel 1: source
#define REG_DMA1DAD (*(volatile u32 *)0x040000c0)   ///< channel 1: destination
#define REG_DMA1CNT (*(volatile u32 *)0x040000c4) ///< channel 1: count, control
#define REG_DMA1CNT_H (*(volatile u16 *)0x040000c6) ///< channel 1: control
#define REG_DMA2SAD (*(volatile u32 *)0x040000c8)   ///< channel 2: source
#define REG_DMA2DAD (*(volatile u32 *)0x040000cc)   ///< channel 2: destination
#define REG_DMA2CNT (*(volatile u32 *)0x040000d0) ///< channel 2: count, control
#define REG_DMA2CNT_H (*(volatile u16 *)0x040000d2) ///< channel 2: control
#define REG_DMA3SAD (*(volatile u32 *)0x040000d4)   ///< channel 3: source
#define REG_DMA3DAD (*(volatile u32 *)0x040000d8)   ///< channel 3: destination
#define REG_DMA3CNT (*(volatile u32 *)0x040000dc) ///< channel 3: count, control
#define REG_DMA3CNT_H (*(volatile u16 *)0x040000de) ///< channel 3: control
// Timers 0 to 2: CNT_L is the reload value, and reads back the count; CNT_H
// takes the TIMER_ flags.
#define REG_TM0CNT_L (*(volatile u16 *)0x04000100) ///< timer 0: reload
#define REG_TM0CNT_H (*(volatile u16 *)0x04000102) ///< timer 0: control
#define REG_TM1CNT_L (*(volatile u16 *)0x04000104) ///< timer 1: reload
#define REG_TM1CNT_H (*(volatile u16 *)0x04000106) ///< timer 1: control
#define REG_TM2CNT_L (*(volatile u16 *)0x04000108) ///< timer 2: reload
#define REG_TM2CNT_H (*(volatile u16 *)0x0400010a) ///< timer 2: control

#define REG_IE (*(volatile u16 *)0x04000200) ///< the interrupts enabled
#define REG_IF (*(volatile u16 *)0x04000202) ///< pending; a 1 written clears
#define REG_WAITCNT (*(volatile u16 *)0x04000204) ///< cartridge wait states
#define REG_IME (*(volatile u16 *)0x04000208) ///< all interrupts' master switch

// KEYCNT values: the interrupt fires when all the listed keys are held at once.
#define KEYCNT_SOFT_RESET 0xc00f ///< A, B, Select and Start.
/// Requires all ten keys together to prevent the reset chord retriggering it.
#define KEYCNT_DISARMED 0xc3ff
#define KEYCNT_SHOULDERS_WAKE 0xc300 ///< L and R, which wake the sleep.
#define KEYCNT_SLEEP_WAKE 0xc304     ///< L, R and Select; no code uses it

/// ROM wait states 3 and 1 with the prefetch buffer on, SRAM at 4; written by
/// the power-on set-up.
#define WAITCNT_GAME 0x4014

#endif
