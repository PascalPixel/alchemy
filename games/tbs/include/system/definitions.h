#ifndef ALCHEMY_DEFINITIONS_H
#define ALCHEMY_DEFINITIONS_H

/* @file */
/* The system library's types and constants, as SGL's sl_def.h holds its */
/* own: the hardware's registers, and each group's structures, enums and fixed
 */
/* values. system.h includes it; a file built by another compiler can include */
/* it alone. */

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "static_assert.h"

/* The io_reg group. The hardware's I/O registers, each `REG_` and GBATEK's */
/* name where GBATEK names it, and their bits. */
#define REG_DISPCNT	 (*(volatile u16 *)0x04000000) /* mode, layers, windows */
#define REG_DISPSTAT (*(volatile u16 *)0x04000004) /* blank flags and IRQs */
/* DISPSTAT's interrupt enables; each is */
/* DISPSTAT_VBLANK_INTR shifted by its IRQ_ number. */
#define DISPSTAT_VBLANK_INTR 0x0008 /* at the start of each VBlank */
#define DISPSTAT_HBLANK_INTR 0x0010 /* at the end of each line */
#define DISPSTAT_VCOUNT_INTR 0x0020 /* on the DISPSTAT_VCOUNT_LINE line */
/* DISPSTAT's high byte: the line the VCount interrupt fires on (the LYC */
/* compare). */
#define DISPSTAT_VCOUNT_LINE(line) ((line) << 8)
#define REG_VCOUNT                                                             \
	(*(volatile u16 *)0x04000006) /* line being drawn, 0..227                  \
								   */
/* The lines REG_VCOUNT counts through in a frame: the DISPLAY_HEIGHT drawn */
/* and those of VBlank. A line's HBlank comes before the next line is drawn, */
/* the last line's before line 0. */
#define DISPLAY_TOTAL_LINES 228
/* Each background's control: its priority, tile and map blocks, colours and */
/* size, composed from the BGCNT_ values below. */
#define REG_BG1CNT (*(volatile u16 *)0x0400000a) /* BG1 */
#define REG_BG2CNT (*(volatile u16 *)0x0400000c) /* BG2 */
#define REG_BG3CNT (*(volatile u16 *)0x0400000e) /* BG3 */

#define BGCNT_PRIORITY(n) (n) /* draw order, 0 in front to 3 behind */
/* The 16 KB character block, 0 to 3, the layer's tiles are read from. */
#define BGCNT_CHAR_BASE(block) ((block) << 2)
#define BGCNT_MOSAIC		   0x0040 /* applies REG_MOSAIC's block size */
#define BGCNT_256_COLORS	   0x0080 /* 8bpp tiles, not 16-colour 4bpp ones */
/* The 2 KB screen block, 0 to 31, the layer's map is read from. */
#define BGCNT_SCREEN_BASE(block) ((block) << 8)
/* Affine backgrounds only: wrap around off the layer's area, which is */
/* otherwise transparent. */
#define BGCNT_WRAP 0x2000
/* The background's size: for an affine one, 0 to 3 are 128, 256, 512 and 1024
 */
/* pixels square. */
#define BGCNT_SIZE(n) ((n) << 14)

/* REG_DISPCNT's video modes and bits: each _ON bit shows its layer or turns */
/* its window on. */
typedef enum DISPLAY_CONTROL {
	DISPCNT_MODE_0 = 0x0000,	 /* four text backgrounds */
	DISPCNT_MODE_1 = 0x0001,	 /* BG0 and BG1 text, BG2 affine */
	DISPCNT_MODE_2 = 0x0002,	 /* BG2 and BG3 affine */
	DISPCNT_OBJ_1D_MAP = 0x0040, /* sprite tiles in one run, not a 2D sheet */
	DISPCNT_BG0_ON = 0x0100,
	DISPCNT_BG1_ON = 0x0200,
	DISPCNT_BG2_ON = 0x0400,
	DISPCNT_BG3_ON = 0x0800,
	DISPCNT_OBJ_ON = 0x1000,
	DISPCNT_WIN0_ON = 0x2000,
	DISPCNT_WIN1_ON = 0x4000,
	DISPCNT_OBJWIN_ON = 0x8000 /* the sprite window OBJ_MODE_WINDOW draws */
} DISPLAY_CONTROL;
/* REG_DISPCNT's three video mode bits, which hold a DISPCNT_MODE_ value. */
#define DISPCNT_MODE_MASK 0x0007
/* The text backgrounds' scroll offsets, write-only: the map pixel, 0 to 511, */
/* at the screen's left edge (HOFS) or top edge (VOFS). */
#define REG_BG1HOFS (*(volatile u16 *)0x04000014) /* BG1, horizontal */
#define REG_BG1VOFS (*(volatile u16 *)0x04000016) /* BG1, vertical */
#define REG_BG2HOFS (*(volatile u16 *)0x04000018) /* BG2, horizontal */
#define REG_BG2VOFS (*(volatile u16 *)0x0400001a) /* BG2, vertical */
#define REG_BG3HOFS (*(volatile u16 *)0x0400001c) /* BG3, horizontal */
#define REG_BG3VOFS (*(volatile u16 *)0x0400001e) /* BG3, vertical */

/* The BG2/BG3 affine matrix entries are signed 8.8 fixed point: texels stepped
 */
/* per screen pixel. */
#define Q_8_8(n) ((s16)((n) * 256))

/* The affine backgrounds' matrices and origins, write-only. */
#define REG_BG2PA (*(volatile u16 *)0x04000020) /* BG2: x step a pixel */
#define REG_BG2PB (*(volatile u16 *)0x04000022) /* BG2: x step a line */
#define REG_BG2PC (*(volatile u16 *)0x04000024) /* BG2: y step a pixel */
#define REG_BG2PD (*(volatile u16 *)0x04000026) /* BG2: y step a line */
#define REG_BG2X  (*(volatile s32 *)0x04000028) /* BG2: origin x */
#define REG_BG2Y  (*(volatile s32 *)0x0400002c) /* BG2: origin y */
#define REG_BG3PA (*(volatile u16 *)0x04000030) /* BG3: x step a pixel */
#define REG_BG3PB (*(volatile u16 *)0x04000032) /* BG3: x step a line */
#define REG_BG3PC (*(volatile u16 *)0x04000034) /* BG3: y step a pixel */
#define REG_BG3PD (*(volatile u16 *)0x04000036) /* BG3: y step a line */
#define REG_BG3X  (*(volatile s32 *)0x04000038) /* BG3: origin x */
#define REG_BG3Y  (*(volatile s32 *)0x0400003c) /* BG3: origin y */

/* The screen's size in pixels. */
#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 160
/* The screen's size in 8-pixel tiles. */
#define DISPLAY_TILE_WIDTH	(DISPLAY_WIDTH / 8)
#define DISPLAY_TILE_HEIGHT (DISPLAY_HEIGHT / 8)

/* The two windows' edges, each a WIN_RANGE value in pixels. */
#define REG_WIN0H (*(volatile u16 *)0x04000040) /* window 0, left and right */
#define REG_WIN1H (*(volatile u16 *)0x04000042) /* window 1, left and right */
#define REG_WIN0V (*(volatile u16 *)0x04000044) /* window 0, top and bottom */
#define REG_WIN1V (*(volatile u16 *)0x04000046) /* window 1, top and bottom */
/* A REG_WINxH or REG_WINxV value: the window covers start up to, but not */
/* including, end. */
#define WIN_RANGE(start, end) (((start) << 8) | (end))
#define REG_WININ                                                              \
	(*(volatile u16 *)0x04000048)				 /* what windows 0 and 1 show  \
												  */
#define REG_WINOUT (*(volatile u16 *)0x0400004a) /* what the rest shows */

/* REG_WININ: the layers shown inside window 0 (low byte) and window 1 (high */
/* byte). CLR lets the colour effect of REG_BLDCNT apply there. */
#define WININ_WIN0_BG0 0x0001 /* window 0: BG0 */
#define WININ_WIN0_BG1 0x0002 /* window 0: BG1 */
#define WININ_WIN0_BG2 0x0004 /* window 0: BG2 */
#define WININ_WIN0_BG3 0x0008 /* window 0: BG3 */
#define WININ_WIN0_OBJ 0x0010 /* window 0: sprites */
#define WININ_WIN0_CLR 0x0020 /* window 0: the colour effect */
#define WININ_WIN0_ALL 0x003f /* window 0: everything */
#define WININ_WIN1_BG0 0x0100 /* window 1: BG0 */
#define WININ_WIN1_BG1 0x0200 /* window 1: BG1 */
#define WININ_WIN1_BG2 0x0400 /* window 1: BG2 */
#define WININ_WIN1_BG3 0x0800 /* window 1: BG3 */
#define WININ_WIN1_OBJ 0x1000 /* window 1: sprites */
#define WININ_WIN1_CLR 0x2000 /* window 1: the colour effect */
#define WININ_WIN1_ALL 0x3f00 /* window 1: everything */

/* REG_WINOUT: the layers shown outside both windows (low byte) and inside the
 */
/* sprite window (high byte). */
#define WINOUT_WIN01_BG0  0x0001 /* outside: BG0 */
#define WINOUT_WIN01_BG1  0x0002 /* outside: BG1 */
#define WINOUT_WIN01_BG2  0x0004 /* outside: BG2 */
#define WINOUT_WIN01_BG3  0x0008 /* outside: BG3 */
#define WINOUT_WIN01_OBJ  0x0010 /* outside: sprites */
#define WINOUT_WIN01_CLR  0x0020 /* outside: the colour effect */
#define WINOUT_WIN01_ALL  0x003f /* outside: everything */
#define WINOUT_WINOBJ_BG0 0x0100 /* sprite window: BG0 */
#define WINOUT_WINOBJ_BG1 0x0200 /* sprite window: BG1 */
#define WINOUT_WINOBJ_BG2 0x0400 /* sprite window: BG2 */
#define WINOUT_WINOBJ_BG3 0x0800 /* sprite window: BG3 */
#define WINOUT_WINOBJ_OBJ 0x1000 /* sprite window: sprites */
#define WINOUT_WINOBJ_CLR 0x2000 /* sprite window: the colour effect */
#define WINOUT_WINOBJ_ALL 0x3f00 /* sprite window: everything */

#define REG_MOSAIC (*(volatile u16 *)0x0400004c) /* mosaic block sizes */
#define REG_BLDCNT (*(volatile u16 *)0x04000050) /* colour effect, targets */

/* REG_BLDCNT: the first target layers, the colour effect, then the second */
/* target layers that a blend mixes the first ones with. */
#define BLDCNT_TGT1_BG0		  0x0001 /* first target: BG0 */
#define BLDCNT_TGT1_BG1		  0x0002 /* first target: BG1 */
#define BLDCNT_TGT1_BG2		  0x0004 /* first target: BG2 */
#define BLDCNT_TGT1_BG3		  0x0008 /* first target: BG3 */
#define BLDCNT_TGT1_OBJ		  0x0010 /* first target: sprites */
#define BLDCNT_TGT1_BD		  0x0020 /* first target: the backdrop */
#define BLDCNT_TGT1_ALL		  0x003f /* first target: every layer */
#define BLDCNT_EFFECT_NONE	  0x0000 /* no effect, the targets as drawn */
#define BLDCNT_EFFECT_BLEND	  0x0040 /* weighted by REG_BLDALPHA */
#define BLDCNT_EFFECT_LIGHTEN 0x0080 /* towards white by REG_BLDY */
#define BLDCNT_EFFECT_DARKEN  0x00c0 /* towards black by REG_BLDY */
#define BLDCNT_TGT2_BG0		  0x0100 /* second target: BG0 */
#define BLDCNT_TGT2_BG1		  0x0200 /* second target: BG1 */
#define BLDCNT_TGT2_BG2		  0x0400 /* second target: BG2 */
#define BLDCNT_TGT2_BG3		  0x0800 /* second target: BG3 */
#define BLDCNT_TGT2_OBJ		  0x1000 /* second target: sprites */
#define BLDCNT_TGT2_BD		  0x2000 /* second target: the backdrop */
#define BLDCNT_TGT2_ALL		  0x3f00 /* second target: every layer */

#define REG_BLDALPHA (*(volatile u16 *)0x04000052) /* the blend's weights */
/* A blend weight's full strength: BLEND_ALPHA(BLEND_WEIGHT_MAX, 0) shows only
 */
/* the top layer. */
#define BLEND_WEIGHT_MAX 16
/* A REG_BLDALPHA value: top and bottom are the two targets' weights, out */
/* of BLEND_WEIGHT_MAX. */
#define BLEND_ALPHA(top, bottom) ((top) | ((bottom) << 8))
#define REG_BLDY				 (*(volatile u16 *)0x04000054) /* fade level, 0 to 16 */
/* REG_BLDY's whole fade: the targets drawn all white, or all black. */
#define BLEND_FADE_MAX 16

/* Screen block 7: BG2's affine map of a battle effect's canvas, a byte a */
/* tile (EFFECT_WRITE_CANVAS_MAP_ROW). */
#define BG_CANVAS_MAP ((volatile s16 *)BG_SCREEN_BLOCK(7))
/* BG0HOFS as an address for a DMA: copies of BgScrollWork over all four */
/* backgrounds' scroll registers start here, and Mind Read's HBlank DMA writes
 */
/* one halfword a line into it. */
#define REG_BG0HOFS ((void *)0x04000010)

/* DMA channel 0's registers as halfwords, for the line scrolls that set */
/* the whole channel up. */
#define REG_DMA0 ((volatile u16 *)0x040000b0)
/* A DMA channel's control, DMAxCNT_H, as a halfword after its source, */
/* destination and count (GBATEK "GBA DMA Transfers"). */
#define DMA_CNT_H_INDEX 5

/* DMA channel 3's registers as words: the channel syDMAXCopy copies with. */
#define REG_DMA3 ((volatile u32 *)0x040000d4)

/* Palette RAM: the background colours, then the sprites'. */
#define PLTT		   0x05000000
#define BG_PLTT_SIZE   512 /* bytes of background colours, 256 of them */
#define BG_PLTT_COLORS (BG_PLTT_SIZE / 2) /* the background's 256 colours */
/* Colours in palette RAM: the background's, then the sprites'. */
#define PLTT_COLORS (2 * BG_PLTT_COLORS)

/* Background colour 0, the backdrop shown where no layer draws. */
#define PLTT_BACKDROP (*(volatile u16 *)PLTT)

#define BG_PLTT ((volatile u16 *)PLTT) /* 16 palettes of 16 colours */
/* Colours in one 16-colour palette. */
#define PALETTE_COLORS 16
/* Colour `color` of 16-colour background palette `palette`. */
#define BG_PLTT_COLOR(palette, color)                                          \
	(BG_PLTT[(palette) * PALETTE_COLORS + (color)])
#define OBJ_PLTT                                                               \
	((volatile u16 *)(PLTT + BG_PLTT_SIZE)) /* the sprites' 256 colours */
/* Colour `color` of 16-colour sprite palette `palette`. */
#define OBJ_PLTT_COLOR(palette, color)                                         \
	(OBJ_PLTT[(palette) * PALETTE_COLORS + (color)])

/* A 15-bit colour from its red, green and blue levels (0 to 31 each), as */
/* the palette stores it: blue in the high bits. */
#define RGB(r, g, b) (((b) << BLUE_SHIFT) | ((g) << GREEN_SHIFT) | (r))
/* Where a 15-bit colour keeps its green and blue levels, red being in the low
 */
/* bits. */
#define GREEN_SHIFT 5
#define BLUE_SHIFT	10
/* A 15-bit colour's red, green and blue levels, 0 to 31 each. */
#define RGB_RED(color)	 ((color) & 0x1f)
#define RGB_GREEN(color) (((color) >> GREEN_SHIFT) & 0x1f)
#define RGB_BLUE(color)	 (((color) >> BLUE_SHIFT) & 0x1f)
/* A colour channel's brightest level. */
#define RGB_LEVEL_MAX 31
/* The brightest colour, every channel at its highest level. */
#define RGB_WHITE RGB(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX)
/* A 15-bit colour's red, green and blue bits. */
#define RED_BITS   RGB(RGB_LEVEL_MAX, 0, 0)
#define GREEN_BITS RGB(0, RGB_LEVEL_MAX, 0)
#define BLUE_BITS  RGB(0, 0, RGB_LEVEL_MAX)

/* The start of the 32 KB of IWRAM, the CPU's fast internal work memory. */
#define IWRAM_START 0x03000000
/* The start of the 256 KB of EWRAM, the slower external work memory. */
#define EWRAM_START 0x02000000
/* The end of EWRAM: the limit the EWRAM heap's allocators */
/* test. */
#define EWRAM_END 0x02040000
/* Video RAM: background tiles and maps, then sprite tiles. */
#define VRAM		   0x06000000
#define BG_CHAR_SIZE   16384 /* bytes in a character block, 16 KB */
#define BG_SCREEN_SIZE 2048	 /* bytes in a screen block, 2 KB */
/* Tiles in a row of a screen block, whose map is 32 tiles square. */
#define BG_SCREEN_WIDTH 32
/* The bytes of one row of a text background map, 32 entries. */
#define MAP_ROW_BYTES (BG_SCREEN_WIDTH * 2)
/* Two background map entries side by side, written a word at a time. */
#define MAP_ENTRY_PAIR(left, right) ((right) << 16 | (left))
/* A screen block entry's 16-colour palette, in its top four bits (its flip */
/* bits are window.h's TILE_HFLIP and TILE_VFLIP). */
#define BG_TILE_PALETTE(n) ((n) << 12)
/* The palette a map entry draws its tile with, 0 to 15. */
#define BG_TILE_PALETTE_OF(entry) (((entry) >> 12) & 0xf)

/* Background tile memory's 16 KB character block `n`, 0 to 3. */
#define BG_CHAR_BLOCK(n) ((u8 *)VRAM + (n) * BG_CHAR_SIZE)
/* The 2 KB screen block `n` of VRAM, 0 to 31, that holds a map. */
#define BG_SCREEN_BLOCK(n) ((u8 *)VRAM + (n) * BG_SCREEN_SIZE)
#define TILE_SIZE_4BPP	   32 /* bytes in an 8x8 tile of 16 colours */
/* The bytes of one pixel row of a 16-colour tile. */
#define TILE_ROW_SIZE_4BPP (TILE_SIZE_4BPP / 8)
#define TILE_SIZE_8BPP	   64 /* bytes in an 8x8 tile of 256 colours */
/* The tiles of a picture that fills the screen in 256 colours. */
#define SCREEN_TILE_BYTES                                                      \
	(DISPLAY_TILE_HEIGHT * DISPLAY_TILE_WIDTH * TILE_SIZE_8BPP)
/* A word of eight 4-bit pixels, all colour `color`: one row of a 16-colour */
/* tile. */
#define PIXELS_4BPP(color) ((color) * 0x11111111u)
/* A word of four 8-bit pixels, all colour `color`. */
#define PIXELS_8BPP(color) ((color) * 0x01010101)
/* Sprite tile memory: the 32 KB of sprite tiles after the */
/* four character blocks. */
#define OBJ_VRAM0 (VRAM + 0x10000)

/* Timer 3's reload value and control as one word; it paces the serial */
/* transfers. */
#define REG_TM3CNT	 (*(volatile u32 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e) /* timer 3's control */

/* TMxCNT_H flags. */
#define TIMER_INTR_ENABLE 0x0040
#define TIMER_ENABLE	  0x0080 /* starts the timer counting */
#define TIMER_256CLK                                                           \
	0x0002 /* counts every 256 cycles, 65536 times a second                    \
			*/

/* The serial data: the 32-bit mode's word, or multiplayer's four received */
/* halfwords. */
#define REG_SIODATA32 ((volatile u32 *)0x04000120)
/* Serial control read as a word, with SIODATA8 above it. */
#define REG_SIOCNT	 (*(volatile u32 *)0x04000128)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128) /* serial control alone */
/* The 8-bit mode's data, which multiplayer mode sends from. */
#define REG_SIODATA8 (*(volatile u16 *)0x0400012a)
#define REG_KEYINPUT (*(volatile u16 *)0x04000130) /* a 0 bit is a held key */
#define REG_KEYCNT	 (*(volatile u16 *)0x04000132) /* keys for the keypad IRQ */
#define REG_RCNT	 (*(volatile u16 *)0x04000134) /* the serial port's mode */

/* SIOCNT flags. */
#define SIO_115200_BPS 0x0003 /* multiplayer's fastest baud rate */
#define SIO_MULTI_SI                                                           \
	0x0004 /* multiplayer: this console's SI pin, clear on the parent */
#define SIO_MULTI_SD 0x0008 /* multiplayer: every console is ready */
#define SIO_ID		 0x0030 /* multiplayer: this console's id, 0 the parent */
#define SIO_ID_SHIFT 4		/* SIO_ID's lowest bit */
/* The multiplayer id (SIO_ID) in a SIOCNT value, taken out by shifts. */
#define SIO_ID_OF(control) (((control) << 26) >> 30)
#define SIO_ERROR		   0x0040 /* multiplayer: the last transfer failed */
/* The SIO_ERROR bit of a SIOCNT value, taken out by shifts as SIO_ID_OF takes
 */
/* the id. */
#define SIO_ERROR_OF(control) (((control) << 25) >> 31)
#define SIO_START			  0x0080 /* starts a transfer; set while one runs */
#define SIO_32BIT_MODE		  0x1000 /* normal mode, 32 bits a transfer */
#define SIO_MULTI_MODE		  0x2000 /* multiplayer mode */
#define SIO_INTR_ENABLE		  0x4000 /* an interrupt when a transfer ends */

/* RCNT's mode: 0 leaves the port to SIOCNT's serial modes; bit 15 alone */
/* makes it general-purpose pins. */
#define RCNT_SIO_MODE  0x0000
#define RCNT_GPIO_MODE 0x8000 /* the port as general-purpose pins */

/* DMAxCNT_H flags. A 32-bit control word takes them */
/* shifted above its count: (DMA_ENABLE | DMA_32BIT) << 16 | words. */
#define DMA_DEST_INC	 0x0000 /* destination steps up */
#define DMA_DEST_DEC	 0x0020 /* destination steps down */
#define DMA_DEST_FIXED	 0x0040 /* destination stays, as for a register */
#define DMA_DEST_RELOAD	 0x0060 /* steps up, back to its start each repeat */
#define DMA_SRC_INC		 0x0000 /* source steps up */
#define DMA_SRC_DEC		 0x0080 /* source steps down */
#define DMA_SRC_FIXED	 0x0100 /* source stays, as for a fill */
#define DMA_REPEAT		 0x0200 /* runs again at each start condition */
#define DMA_16BIT		 0x0000 /* moves halfwords */
#define DMA_32BIT		 0x0400 /* moves words */
#define DMA_DREQ_ON		 0x0800 /* channel 3: the cartridge requests it */
#define DMA_START_NOW	 0x0000 /* as soon as it's enabled */
#define DMA_START_VBLANK 0x1000
#define DMA_START_HBLANK 0x2000
#define DMA_START_SPECIAL                                                      \
	0x3000					   /* channels 1 and 2: a sound FIFO's request     \
								*/
#define DMA_INTR_ENABLE 0x4000 /* an interrupt when it ends */
#define DMA_ENABLE		0x8000 /* arms the channel */
/* DMA_ENABLE in a channel's 32-bit count and control word, which the hardware
 */
/* clears once the transfer is done. */
#define DMA_BUSY ((u32)DMA_ENABLE << 16)
/* A DMA control's start timing (bits 12 and 13), cartridge request (bit 11) */
/* and repeat (bit 9): the bits cleared before a channel is stopped. */
#define DMA_START_TIMING_DRQ_REPEAT                                            \
	(DMA_START_SPECIAL | DMA_DREQ_ON | DMA_REPEAT)

/* DMA control words: copy n words or halfwords at once, fill n words from one
 */
/* fixed source word, or one halfword or word each HBlank to a register */
/* reloaded every line (a scanline table that drives a window edge, a scroll or
 */
/* an affine register). */
#define DMA_COPY_WORDS(n)	  ((DMA_ENABLE | DMA_32BIT) << 16 | (n))
#define DMA_FILL_HALFWORDS(n) ((DMA_ENABLE | DMA_SRC_FIXED) << 16 | (n))
#define DMA_FILL_WORDS(n)	  ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (n))
#define DMA_COPY_HALFWORDS(n) (DMA_ENABLE << 16 | (n))
/* n halfwords copied from the last back to the first, source and destination */
/* stepping down, so that a run can move up over itself. */
#define DMA_COPY_HALFWORDS_DOWN(n)                                             \
	((DMA_ENABLE | DMA_SRC_DEC | DMA_DEST_DEC) << 16 | (n))
/* A halfword each HBlank, as for a scroll or a window edge. */
#define DMA_HBLANK_HALFWORD_PER_LINE                                           \
	((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD) << 16 | 1)
/* n words each HBlank, as for the affine registers of a background written */
/* together. */
#define DMA_HBLANK_WORDS_PER_LINE(n)                                           \
	((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD |           \
	  DMA_32BIT)                                                               \
		 << 16 |                                                               \
	 (n))
/* A word each HBlank, as for a pair of registers written together. */
#define DMA_HBLANK_WORD_PER_LINE DMA_HBLANK_WORDS_PER_LINE(1)

/* BIOS CpuSet control flags, above its count. */
#define CPU_SET_SRC_FIXED 0x01000000
#define CPU_SET_32BIT	  0x04000000 /* words rather than halfwords */

/* The system library's functions that the SDK's units call. They cannot include
 */
/* system.h, so the prototypes are here with the constants they take. */
/* The BIOS CpuSet call (system/bios.s): copies or fills by halfwords or */
/* words, as the CPU_SET_ flags above the count say. */
void Bios_CpuSet(const void *source, void *destination, u32 control);
/* The C library's signed division and remainder (system/iwram_entry.s), which
 */
/* `/` and `%` compile to; declared for the functions that call them by name. */
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

#define OAM ((void *)0x07000000) /* the sprites' attributes */
/* OAM's size in bytes: 128 sprites of 8 bytes. */
#define OAM_SIZE 1024

/* A sprite's mode, OAM attribute 0's bits 11..10. */
typedef enum SPR_MODE {
	OBJ_MODE_NORMAL = 0,
	OBJ_MODE_SEMI_TRANSPARENT = 1, /* blended as BLDCNT's first target */
	OBJ_MODE_WINDOW = 2			   /* drawn into the OBJ window, not shown */
} SPR_MODE;

/* A sprite's attributes 0 and 1 as one word, attr0 | attr1 << 16, the way */
/* functions that build their own sprite lists write them; x, 0 to 511, is */
/* shifted into the high half and y, 0 to 255, is the low byte. */
#define OBJ_SIZE_8X8   0x00000000
#define OBJ_SIZE_16X16 0x40000000
#define OBJ_SIZE_32X32 0x80000000
#define OBJ_SIZE_64X64 0xc0000000
#define OBJ_SIZE_16X8  0x00004000
#define OBJ_SIZE_32X8  0x40004000
#define OBJ_SIZE_32X16 0x80004000
#define OBJ_SIZE_64X32 0xc0004000
#define OBJ_SIZE_8X16  0x00008000
#define OBJ_SIZE_16X32 0x80008000
#define OBJ_SIZE_32X64 0xc0008000
#define OBJ_AFFINE	   0x00000100 /* drawn through its affine matrix */
/* An affine sprite drawn in a box twice its size, so turning never clips it. */
#define OBJ_DOUBLE_SIZE 0x00000200
#define OBJ_MODE(mode)	((mode) << 10) /* an SPR_MODE */
#define OBJ_256_COLORS	0x00002000	   /* 8bpp tiles, as BGCNT_256_COLORS */
#define OBJ_HFLIP		0x10000000 /* a plain sprite mirrored left to right */
#define OBJ_VFLIP		0x20000000 /* a plain sprite mirrored top to bottom */
/* Attribute 2 above its tile number: the sprite's priority, 0 in front to 3 */
/* behind, and its 16-colour palette. */
#define OBJ_PRIORITY(n) ((n) << 10)
#define OBJ_PALETTE(n)	((n) << 12)
/* A sprite's x in the attr01 word of an OAM_WORDS entry, whose attribute 1 is
 */
/* its high half. */
#define OBJ_X(x) ((x) << 16)
/* An affine sprite's matrix number, attribute 1's bits 9 to 13, in the attr01
 */
/* word. */
#define OBJ_MATRIX(n) ((n) << 25)
/* An affine parameter in the high half of an OAM_WORDS entry's attr2 word: the
 */
/* entries 4n to 4n + 3 hold affine matrix n's four. */
#define OBJ_AFFINE_PARAMETER(value) ((value) << 16)
/* Attribute 1's nine bits of x: a sprite left of the screen is at 512 less */
/* its distance. */
#define OBJ_X_MASK 0x1ff
/* A sprite's place in its attr01 word: x in attribute 1's nine bits and y in */
/* attribute 0's eight, each wrapped as the screen wraps them. */
#define OBJ_POSITION(x, y) (OBJ_X((x) & OBJ_X_MASK) | ((y) & 0xff))
/* Attribute 2's ten bits of tile number, counted in 4-bit tiles from the */
/* start of sprite VRAM. */
#define OBJ_TILE_MASK 0x3ff
/* The tile numbers a sprite of width by height pixels takes, of 16 colours */
/* and of 256 (two numbers to a tile): a picture's sprites follow one another */
/* so in sprite VRAM. */
#define SPRITE_TILES(width, height)		 ((width) / 8 * ((height) / 8))
#define SPRITE_TILES_8BPP(width, height) (2 * SPRITE_TILES(width, height))
/* Attribute 0 holds eight bits of a sprite's y, so its y wraps round at 256 */
/* lines. */
#define OBJ_Y_RANGE 256

/* The keys' bits as REG_KEYINPUT and REG_KEYCNT lay them out; the game's */
/* key state (KeysHeld and the rest) holds them set for a pressed key. */
typedef enum KEY_MASK {
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
} KEY_MASK;

#define KEYS_MASK                                                              \
	0x3ff /* the ten keys' bits; KEYINPUT reads 0 for a held key */

/* The direction pad's four keys (KEY_RIGHT to KEY_DOWN) as a number 0 to 15, */
/* to index a table with a row for each combination held. */
#define DPAD_INDEX(keys) (((keys) >> 4) & 15)

#define REG_BG0CNT	(*(volatile u16 *)0x04000008) /* BG0's control (BGCNT_) */
#define REG_BG0VOFS (*(volatile u16 *)0x04000012) /* BG0's vertical scroll */
/* Direct sound's mixing: volumes, speakers, timers and FIFO resets. */
#define REG_SOUNDCNT_H (*(volatile u16 *)0x04000082)
/* SOUNDCNT_H flags: the mixing volumes, then each direct */
/* sound channel's speakers, timer and FIFO reset. */
#define SOUND_CGB_MIX_FULL 0x0002 /* sounds 1 to 4 at full volume */
#define SOUND_A_MIX_FULL   0x0004 /* direct sound A at full, not half */
#define SOUND_B_MIX_FULL   0x0008 /* direct sound B at full, not half */
/* Sounds 1 to 4 and both direct sound channels all at full volume. */
#define SOUND_ALL_MIX_FULL                                                     \
	(SOUND_CGB_MIX_FULL | SOUND_A_MIX_FULL | SOUND_B_MIX_FULL)
#define SOUND_A_RIGHT_OUTPUT 0x0100 /* A to the right speaker */
#define SOUND_A_LEFT_OUTPUT	 0x0200 /* A to the left speaker */
#define SOUND_A_TIMER_0		 0x0000 /* A plays at timer 0's rate */
#define SOUND_A_FIFO_RESET	 0x0800 /* empties A's FIFO */
#define SOUND_B_RIGHT_OUTPUT 0x1000 /* B to the right speaker */
#define SOUND_B_LEFT_OUTPUT	 0x2000 /* B to the left speaker */
#define SOUND_B_TIMER_0		 0x0000 /* B plays at timer 0's rate */
#define SOUND_B_FIFO_RESET	 0x8000 /* empties B's FIFO */
#define REG_SOUNDBIAS		 (*(volatile u16 *)0x04000088) /* output bias, rate */
#define REG_FIFO_A			 (*(volatile u32 *)0x040000a0) /* A's samples, 4 a write */
#define REG_FIFO_B			 (*(volatile u32 *)0x040000a4) /* B's samples, 4 a write */
/* The four DMA channels: source, destination, then the count with the control
 */
/* halfword (CNT_H, the DMA_ flags) above it. */
#define REG_DMA0SAD (*(volatile u32 *)0x040000b0) /* channel 0: source */
#define REG_DMA0DAD (*(volatile u32 *)0x040000b4) /* channel 0: destination */
#define REG_DMA0CNT                                                            \
	(*(volatile u32 *)0x040000b8) /* channel 0: count, control */
#define REG_DMA0CNT_H (*(volatile u16 *)0x040000ba) /* channel 0: control */
#define REG_DMA1SAD	  (*(volatile u32 *)0x040000bc) /* channel 1: source */
#define REG_DMA1DAD	  (*(volatile u32 *)0x040000c0) /* channel 1: destination */
#define REG_DMA1CNT                                                            \
	(*(volatile u32 *)0x040000c4) /* channel 1: count, control */
#define REG_DMA1CNT_H (*(volatile u16 *)0x040000c6) /* channel 1: control */
#define REG_DMA2SAD	  (*(volatile u32 *)0x040000c8) /* channel 2: source */
#define REG_DMA2DAD	  (*(volatile u32 *)0x040000cc) /* channel 2: destination */
#define REG_DMA2CNT                                                            \
	(*(volatile u32 *)0x040000d0) /* channel 2: count, control */
#define REG_DMA2CNT_H (*(volatile u16 *)0x040000d2) /* channel 2: control */
#define REG_DMA3SAD	  (*(volatile u32 *)0x040000d4) /* channel 3: source */
#define REG_DMA3DAD	  (*(volatile u32 *)0x040000d8) /* channel 3: destination */
#define REG_DMA3CNT                                                            \
	(*(volatile u32 *)0x040000dc) /* channel 3: count, control */
#define REG_DMA3CNT_H (*(volatile u16 *)0x040000de) /* channel 3: control */
/* Timers 0 to 2: CNT_L is the reload value, and reads back the count; CNT_H */
/* takes the TIMER_ flags. */
#define REG_TM0CNT_L (*(volatile u16 *)0x04000100) /* timer 0: reload */
#define REG_TM0CNT_H (*(volatile u16 *)0x04000102) /* timer 0: control */
#define REG_TM1CNT_L (*(volatile u16 *)0x04000104) /* timer 1: reload */
#define REG_TM1CNT_H (*(volatile u16 *)0x04000106) /* timer 1: control */
#define REG_TM2CNT_L (*(volatile u16 *)0x04000108) /* timer 2: reload */
#define REG_TM2CNT_H (*(volatile u16 *)0x0400010a) /* timer 2: control */

#define REG_IE		(*(volatile u16 *)0x04000200) /* the interrupts enabled */
#define REG_IF		(*(volatile u16 *)0x04000202) /* pending; a 1 written clears */
#define REG_WAITCNT (*(volatile u16 *)0x04000204) /* cartridge wait states */
/* WAITCNT's slowest SRAM wait (bits 1 and 0), 8 cycles, pret's */
/* WAITCNT_SRAM_8: the wait the flash functions read the chip with; */
/* programming and erasing use the chip's wait[0], then restore it. */
#define WAITCNT_SRAM_SLOWEST 0x0003
/* WAITCNT's SRAM wait bits. */
#define WAITCNT_SRAM_MASK 0x0003
#define REG_IME                                                                \
	(*(volatile u16 *)0x04000208) /* all interrupts' master switch */

/* KEYCNT values: the interrupt fires when all the listed keys are held at once.
 */
#define KEYCNT_SOFT_RESET 0xc00f /* A, B, Select and Start. */
/* Requires all ten keys together to prevent the reset chord retriggering it. */
#define KEYCNT_DISARMED		  0xc3ff
#define KEYCNT_SHOULDERS_WAKE 0xc300 /* L and R, which wake the sleep. */
#define KEYCNT_SLEEP_WAKE	  0xc304 /* L, R and Select; no code uses it */

/* ROM wait states 3 and 1 with the prefetch buffer on, SRAM at 4; written by */
/* the power-on set-up. */
#define WAITCNT_GAME 0x4014

/* The task group. Tasks, their priorities and the task runner's state. */
/* The most tasks that can run at once. */
typedef enum TASK_LIMITS { TASK_COUNT = 20 } TASK_LIMITS;

typedef void (*TASK_FUNC)(void);

/* A task: its function's address, its priority (see TASK_PASS_) and its */
/* flags, where TASK_FLAG_KEEP_RUNNING keeps it running while overlay tasks are
 */
/* suspended. */
typedef struct TASK {
	u32 func;
	u16 pri;
	u8 flg;
	u8 reserved;
} TASK;

STATIC_ASSERT(sizeof(TASK) == 8);
STATIC_ASSERT(offsetof(TASK, pri) == 4);
STATIC_ASSERT(offsetof(TASK, flg) == 6);

/* A task's priority: the high byte picks the pass that runs it, the low byte */
/* ranks it within the pass (higher runs first). An odd high byte means the */
/* task is suspended, so no pass matches it. */
/* */
/* The two passes: the VBlank handler runs TASK_PASS_VBLANK's tasks, and */
/* sySynch() runs TASK_PASS_FRAME's once a frame. Each ranks 0x80, the middle */
/* of the low byte, so a task added at TASK_PASS_FRAME + 10 runs before those */
/* added at TASK_PASS_FRAME. */
#define TASK_PASS_VBLANK 0x480
#define TASK_PASS_FRAME	 0xc80
/* The highest rank in the VBlank pass: a task added here runs first of the */
/* VBlank tasks, before the frame's first line is drawn. */
#define TASK_PASS_VBLANK_FIRST (TASK_PASS_VBLANK + 127)
/* An empty slot's priority, sorted last: the sort reads priorities signed. */
#define TASK_PRIORITY_EMPTY -1
/* A removed task's priority: its odd high byte matches no pass, and as the */
/* highest priority it sorts to the front of the table. */
#define TASK_PRIORITY_REMOVED 0x7fff

/* A task's priority's high byte: the pass it runs in. */
#define TASK_STATE_HIGH(task) (((u8 *)&(task)->pri)[1])
/* The bit of a task's high priority byte (TASK_STATE_HIGH) that suspends it. */
#define TASK_SUSPENDED 1
/* The bit of a task's flags that keeps it running while */
/* syTaskSuspendOverlayTasks suspends the others. */
#define TASK_FLAG_KEEP_RUNNING 1
/* Whether the task's function is in EWRAM, where a scene's overlay runs. */
#define TASK_IN_OVERLAY(task) (((task)->func >> 24) == EWRAM_START >> 24)

/* The irq group. The hardware's interrupt sources and the interrupt handler */
/* table. */
/* The hardware's interrupt sources: each one's bit number in IE and IF, and */
/* its slot in IrqHandlers. */
enum {
	IRQ_VBLANK,
	IRQ_HBLANK,
	IRQ_VCOUNT,
	IRQ_TIMER0,
	IRQ_TIMER1,
	IRQ_TIMER2,
	IRQ_TIMER3,
	IRQ_SERIAL,
	IRQ_DMA0,
	IRQ_DMA1,
	IRQ_DMA2,
	IRQ_DMA3,
	IRQ_KEYPAD,
	IRQ_GAMEPAK,
	IRQ_COUNT
};

/* The interrupt sources' bits in REG_IE and REG_IF. */
#define INTR_FLAG_VBLANK  (1 << IRQ_VBLANK)
#define INTR_FLAG_TIMER3  (1 << IRQ_TIMER3)
#define INTR_FLAG_SERIAL  (1 << IRQ_SERIAL)
#define INTR_FLAG_KEYPAD  (1 << IRQ_KEYPAD)
#define INTR_FLAG_GAMEPAK (1 << IRQ_GAMEPAK)

/* An interrupt source's handler, called by the IWRAM dispatcher. */
typedef void (*INTERRUPT_HANDLER)(void);

/* The heap group. The heap's slots and HEAP_STATE. */
/* */
/* The heap is two stacks of blocks, one in IWRAM and one in EWRAM; "The */
/* heap" in the system library's README gives the allocators and the */
/* lifetime rule. Every scene change resets both pools and clears every slot */
/* (syHeapInit, from the main routine in system/main_routine.c), so no */
/* block outlives the scene, battle or minigame that took it. */
/* Slot n's block, or null before it is allocated. */
#define HEAP_SLOT_BLOCK(n) (*(HeapSlots + (n)))

/* Heap slot `slot` read through `slots`, a pointer to slot `from` of */
/* HeapSlots: code that holds one slot's address reaches its neighbours by */
/* their distance from it. */
#define HEAP_SLOT_FROM(slots, from, slot) ((slots)[(slot) - (from)])

/* The heap's 64 slots, the first two holding the next free EWRAM and */
/* IWRAM addresses. */
typedef union HEAP_STATE {
	struct {
		void *nextEwram;  /* the EWRAM pool's next free byte, up to EWRAM_END */
		void *nextIwram;  /* the IWRAM pool's next free byte, up to the heap's
							 end */
		void *blocks[62]; /* heap slots 2 to 63 */
	} heap;
	/* Each slot's block once allocated, else null; slots 0 and 1 are the pool
	 */
	/* pointers. */
	void *slots[64];
} HEAP_STATE;

STATIC_ASSERT(sizeof(HEAP_STATE) == 256);
STATIC_ASSERT(offsetof(HEAP_STATE, heap.nextIwram) == 4);
STATIC_ASSERT(offsetof(HEAP_STATE, heap.blocks) == 8);

/* The heap slots shared across folders, by index into HeapSlots. */
typedef enum HEAP_SLOT {
	HEAP_SLOT_MATRIX_STACK = 2, /* The 3D transform's one saved level. */
	/* The sprite animations (SPR_ANIMATION) of the actor */
	/* system's sprites (SpriteAnimations). */
	HEAP_SLOT_SPRITE_ANIMATIONS = 3,
	/* The actor system's sprite pool (SPR). */
	HEAP_SLOT_SPRITES = 4,
	/* The 64 actors (ACTOR). */
	HEAP_SLOT_ACTORS = 5,
	/* The actor system's state (ACTOR_SYSTEM_STATE, ActorSystemStateWork),
	 */
	/* taken by fdActorSystemInit. */
	HEAP_SLOT_ACTOR_SYSTEM = 6,
	/* The world map's affine state (WORLD_MAP_AFFINE_STATE). */
	HEAP_SLOT_WORLD_MAP_AFFINE = 7,
	/* The field map's scrolling state (MAP_GRID_SCROLL_STATE), or on the */
	/* world map its perspective view (WORLD_MAP_VIEW); MapGridState. */
	HEAP_SLOT_MAP_GRID = 8,
	/* A battle's BATTLE_STATE (BattleStateWork), from btRun to */
	/* the next scene change. */
	HEAP_SLOT_BATTLE = 9,
	/* The battle background's scanline state (BATTLE_AFFINE_HDMA). */
	HEAP_SLOT_BATTLE_SCANLINES = 10,
	/* Unused: 640 bytes btRun allocates with the battle's other blocks, */
	/* which nothing reads or writes. */
	HEAP_SLOT_BATTLE_UNUSED = 11,
	/* The 3D camera (BATTLE_CAMERA, BattleCameraWork) of a battle, the */
	/* world map or Lucky Dice. */
	HEAP_SLOT_CAMERA = 12,
	/* A scratch buffer, usually 1 KB, that a picture is decoded into and */
	/* released as soon as its tiles are in VRAM. */
	HEAP_SLOT_TILE_SCRATCH = 14,
	/* The window renderer's state (WIN_STATE, WindowStateWork). */
	HEAP_SLOT_WINDOW = 15,
	/* The party status window's layout (PARTY_STAT_WINDOW). */
	HEAP_SLOT_PARTY_STATUS = 16,
	/* The buffers an icon or portrait is decoded through (ICON_TRANSFER). */
	HEAP_SLOT_ICON_TRANSFER = 17,
	/* The selection screen's state (SELECTION_MENU_STATE). */
	HEAP_SLOT_SELECTION_MENU = 18,
	/* The selection state mnAllocateSelectionState allocates, which nothing */
	/* calls. */
	HEAP_SLOT_SELECTION_STATE = 19,
	/* The settings menu's state (SETTINGS_MENU_STATE, OptionMenuState). */
	HEAP_SLOT_SETTINGS_MENU = 20,
	/* Mind Read's wave state (MIND_READ_STATE, MindReadStateWork). */
	HEAP_SLOT_MIND_READ = 22,
	/* A field scene's FIELD_STATE, which scenes read as FIELD_STATE
	 */
	/* (FieldStateWork); fdSequenceRun takes it and frees it on the way out. */
	HEAP_SLOT_FIELD = 27,
	/* The palette cycles (PALETTE_CYCLE_STATE). */
	HEAP_SLOT_PALETTE_CYCLES = 28,
	/* The running ground particles, motes or sparkles (struct */
	/* WEATHER_STATE). */
	HEAP_SLOT_WEATHER = 29,
	/* The field's colour blend state (FieldBlendStateWork): the struct */
	/* FIELD_BLEND_STATE the storm's lightning and fdWeatherStartLightningFlash
	 */
	/* allocate. */
	HEAP_SLOT_FIELD_BLEND = 30,
	/* The screen transition (SCR_TRANSITION_STATE, */
	/* ScreenTransitionStateWork): the window edges and dissolve of a scene */
	/* change. */
	HEAP_SLOT_SCREEN_TRANSITION = 31,
	/* The palette fade (PaletteFadeState) that moves the BG and sprite palettes
	 */
	/* toward target colours: the PALETTE_FADE */
	/* fdPaletteFadeInit allocates on scene load, or the title */
	/* intro's TITLE_PALETTE_FADE. */
	HEAP_SLOT_PALETTE_FADE = 32,
	/* The floating lights of Sol Sanctum's Elemental Star room (struct */
	/* SOL_SANCTUM_LIGHT_LIST, sol_sanctum_elemental_star_lights.c). */
	HEAP_SLOT_SOL_SANCTUM_LIGHTS = 33,
	/* The line scroll's state (LINE_SCROLL_STATE). */
	HEAP_SLOT_LINE_SCROLL = 34,
	/* Where Mercury Lighthouse keeps a pointer to the state of the lights that
	 */
	/* carry the leader over its pools (MERCURY_LIGHTHOUSE_LIGHTS). */
	HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS = 35,
	/* The ground mask's state (GROUND_MASK_STATE). */
	HEAP_SLOT_GROUND_MASK = 36,
	/* The battle message window's state (BATTLE_MESSAGE_STATE). */
	HEAP_SLOT_BATTLE_MESSAGE = 37,
	/* The running battle effect's own state (BattleEffectStateWork). Lucky Dice
	 */
	/* keeps its own state in this slot and the next two. */
	HEAP_SLOT_BATTLE_EFFECT = 39,
	/* The running battle effect's pixel canvas. */
	HEAP_SLOT_BATTLE_CANVAS = 40,
	/* The running battle effect's tile sheet, or its set of mote cells. */
	HEAP_SLOT_BATTLE_SHEET = 41,
	/* The battle's entry transition timer, in frames (BattleEntryTimer). */
	HEAP_SLOT_BATTLE_ENTRY_TIMER = 42,
	/* The title intro's state (INTRO_STATE). */
	HEAP_SLOT_TITLE_INTRO = 43,
	/* The battle background's view and entry transition (32 bytes), from */
	/* btRun. */
	HEAP_SLOT_BATTLE_BACKGROUND = 44,
	/* Lucky Dice's sprites (1,560 bytes), for as long as the game runs. */
	HEAP_SLOT_LUCKY_GAME = 45,
	/* A function run from RAM: the canvas drawing function */
	/* btEffectBuildBlitter writes, called as a DRAW_RECTANGLE and released */
	/* before its canvas, or the world map's copy of
	   fdWorldMapUpdateAffineLines. */
	HEAP_SLOT_RAM_CODE = 46,
	/* A second function as HEAP_SLOT_RAM_CODE, released before it: always a */
	/* btEffectBuildBlitter function, the second of a pair whose first is in */
	/* HEAP_SLOT_RAM_CODE. */
	HEAP_SLOT_RAM_CODE_PAIRED = 47,
	/* Copied field row drawer or window tile decoder, released after each call.
	 */
	HEAP_SLOT_RAM_CODE_TEMP = 49,
	/* The message decoder function (Text_DecodeSymbol), copied here to run for
	 */
	/* the length of one message's decoding (mnMessageDecode), or of one */
	/* mnMessageDecodeRaw call when the slot is empty. */
	HEAP_SLOT_MESSAGE_DECODER = 50,
	/* The save state (SAVE_STATE), from */
	/* sySaveInitState to sySaveReleaseState around each */
	/* save, load, copy or delete and each scan of the files, as the main */
	/* menu's (mnSaveScanFileSummaries). */
	HEAP_SLOT_SAVE = 51,
	/* Lives for one frame: sySynch() builds the OAM list in it, and the field
	 */
	/* sprite decoder keeps its code there between its own uses. */
	HEAP_SLOT_FRAME_SCRATCH = 52,
	/* A RAM copy of fdTileCopyStrided, which uploads field sprite pictures to
	 */
	/* VRAM; loaded with the animation pools. */
	HEAP_SLOT_PICTURE_UPLOADER = 53,
	/* Each linked console's party (ENEMY_UNITS_SIZE bytes), from the */
	/* Battle Arena and btRun. */
	HEAP_SLOT_ENEMY_UNITS = 54,
	/* The menu state (MENU_STATE_SIZE bytes) shared by the field menu's screens
	 */
	/* and the shop; each takes it on opening and releases it on closing. */
	HEAP_SLOT_MENU = 55,
	/* The Psynergy effect's state (FIELD_EFFECT_STATE, */
	/* FieldEffectStateWork). */
	HEAP_SLOT_FIELD_EFFECT = 56,
	/* The battle command menus' state (BATTLE_MENU_STATE). */
	HEAP_SLOT_BATTLE_MENU = 57,
	/* A command icon menu's state (ICON_MENU_STATE). */
	HEAP_SLOT_ICON_MENU = 58,
	/* The Colosso finals' race state (COLOSSO_RACE_STATE), which */
	/* ColossoState reads by its own name. */
	HEAP_SLOT_COLOSSO_RACE = 59
} HEAP_SLOT;

/* The room an effect gives its glow pictures: HEAP_SLOT_BATTLE_SHEET's size, */
/* and where an effect's own pictures start in a sheet whose glow pictures */
/* come first. The largest glow sheet, BlueGlowSheet's ten flares from 2x2 to */
/* 20x20, takes 1540 bytes of it. */
#define GLOW_SHEET_ROOM 1550

/* The heap_slots group. The heap slots of HeapSlots, read through their own */
/* names. */
struct SCR_TRANSITION_STATE;

/* The heap slots from HEAP_SLOT_MAP_GRID on: MapGridState[0] is the map's */
/* state (MAP_GRID_SCROLL_STATE, or the world map's WORLD_MAP_VIEW). */
#define MapGridState (*(void *(*)[]) & HeapSlots[HEAP_SLOT_MAP_GRID])
/* The menu state, heap slot HEAP_SLOT_MENU: each menu screen and the shop */
/* read it through their own struct while they are open. */
#define MenuState (*(void **)&HeapSlots[HEAP_SLOT_MENU])

/* The heap slots from HEAP_SLOT_FIELD_BLEND on: FieldBlendStateWork[0] is the
 */
/* field's colour blend state. */
#define FieldBlendStateWork (*(void *(*)[]) & HeapSlots[HEAP_SLOT_FIELD_BLEND])
/* The screen transition's state, heap slot HEAP_SLOT_SCREEN_TRANSITION. */
#define ScreenTransitionStateWork                                              \
	(*(struct SCR_TRANSITION_STATE **)&HeapSlots[HEAP_SLOT_SCREEN_TRANSITION])
/* HEAP_SLOT_BATTLE_ENTRY_TIMER: the battle's entry transition timer in frames.
 */
#define BattleEntryTimer (*(s32 **)&HeapSlots[HEAP_SLOT_BATTLE_ENTRY_TIMER])
/* The running Psynergy effect's state (FIELD_EFFECT_STATE), heap slot */
/* HEAP_SLOT_FIELD_EFFECT. */
#define FieldEffectStateWork (*(void **)&HeapSlots[HEAP_SLOT_FIELD_EFFECT])

/* The affine group. Sprite rotation and scale, and the affine matrices handed
 */
/* out each frame. */
/* A sprite's rotation and scale: scales in 8.8 fixed point (256 is full */
/* size), the angle in 1/65536 turns. */
typedef struct AFFINE_TRANSFORM {
	u16 scaleX;	 /* 8.8 fixed point, read signed: negative mirrors the sprite */
	u16 scaleY;	 /* 8.8 fixed point, read signed */
	u16 ang;	 /* 1/65536 turns */
	u16 padding; /* never read; keeps the struct 8 bytes */
} AFFINE_TRANSFORM;

/* One OAM rotation-scaling matrix: pa, pb, pc and pd in 8.8 fixed point, the */
/* inverse of the transform the sprite is drawn with. */
typedef union OAM_MATRIX {
	s16 coefficients[4]; /* pa, pb, pc and pd */
	/* pa and pb, then pc and pd, written whole when only the diagonal is set.
	 */
	u32 rows[2];
} OAM_MATRIX;

STATIC_ASSERT(sizeof(AFFINE_TRANSFORM) == 8);
STATIC_ASSERT(offsetof(AFFINE_TRANSFORM, scaleY) == 2);
STATIC_ASSERT(offsetof(AFFINE_TRANSFORM, ang) == 4);
STATIC_ASSERT(sizeof(OAM_MATRIX) == 8);

/* The dma group. DMA channel registers, and the DMA and CPU fills. */
/* A DMA channel's registers (DMAxSAD, DMAxDAD, DMAxCNT). */
typedef struct DMA_REGISTERS {
	u32 src;
	u32 dst;
	u32 ctrl; /* The count in the low half, DMAxCNT_H's flags above it. */
} DMA_REGISTERS;

/* DMA3, the channel the game copies with, as its three registers. */
#define DMA3 ((volatile DMA_REGISTERS *)REG_DMA3)

/* The halfword of a channel's REG_DMA0-style halfword view that holds its */
/* control (DMAxCNT_H): source, destination, count, then control. */
#define DMA_CONTROL_HALFWORD 5

/* The stub group. The system library's stub table */
/* (system/stubs.s), seen as the entries the world map's tilt calls through */
/* it. */
/* An entry of the system library's stub table: a stub that jumps to its */
/* target, and the target. */
typedef struct STUB {
	u32 stub;
	u32 tgt;
} STUB;

/* An entry of the system library's table whose two-argument target the world */
/* map's tilt calls, the arctangent. */
typedef struct ARC_TAN_STUB {
	u32 stub;
	s32 (*tgt)(s32 x, s32 y);
} ARC_TAN_STUB;

/* An entry of the system library's table whose one-argument target the world */
/* map's tilt calls, the sine or the cosine. */
typedef struct TRIG_STUB {
	u32 stub;
	s32 (*tgt)(s32 angle);
} TRIG_STUB;

/* The start of the system library's stub table, as far as syCos. */
typedef struct SYSTEM_STUB_TABLE {
	STUB preceding[8];
	ARC_TAN_STUB arcTan2;
	STUB squareRoots[2];
	TRIG_STUB sin;
	TRIG_STUB cos;
} SYSTEM_STUB_TABLE;

STATIC_ASSERT(sizeof(STUB) == 8);
STATIC_ASSERT(sizeof(ARC_TAN_STUB) == 8);
STATIC_ASSERT(sizeof(TRIG_STUB) == 8);
STATIC_ASSERT(offsetof(SYSTEM_STUB_TABLE, arcTan2) == 64);
STATIC_ASSERT(offsetof(SYSTEM_STUB_TABLE, sin) == 88);
STATIC_ASSERT(offsetof(SYSTEM_STUB_TABLE, cos) == 96);

/* The frame group. The frame loop's globals: the VBlanks a frame takes, the */
/* CPU load display and the VBlank flags. */
/* */
/* sySynch() (system/system_frame.c) and the VBlank handler share these cells;
 */
/* those marked volatile are changed by an interrupt or counted by sySynch() */
/* while other code reads them. */
/* Frames in a second: the screen refreshes about 60 times a second. */
#define FRAMES_PER_SECOND 60

/* The io_write_queue group. The register write queue that VBlank carries out */
/* after each finished frame. */
/* The register writes VBlank carries out once the frame loop has finished a */
/* frame; syIoWriteQueueFlushPending empties it. */
typedef struct IO_WRITE_QUEUE {
	u16 cnt; /* Entries waiting, at most 32. */
	u16 pad;
	/* Each entry is the value, the address and the control word, in that order.
	 */
	u32 entries[32][3];
} IO_WRITE_QUEUE;

/* A control word's bits 16 and 17 give a register write's width and bits 18 */
/* and 19 its operation; syResourceExecuteTransfer reads them. */

/* Writes a byte. */
#define IO_WRITE_BYTE (1 << 16)
/* Writes a halfword. */
#define IO_WRITE_HALFWORD (2 << 16)
/* Writes a word. */
#define IO_WRITE_WORD (3 << 16)
/* ORs the value into the register instead of storing it. */
#define IO_WRITE_SET (4 << 16)
/* Clears the value's bits in the register instead of storing it. */
#define IO_WRITE_CLEAR (8 << 16)

/* The iwram_call group. The IWRAM call layer: the functions the game keeps */
/* resident in IWRAM, each at its fixed address, and one typed inline per */
/* function that every call goes through. */
/* The IWRAM-resident functions at their fixed addresses, each called through */
/* the inline below it. */
#define fdFixedDivEntry                                                        \
	FIXED_ADDRESS((s32 (*)(s32 divisor, s32 value)), 0x0300013c, IwramDivFX)
#define syWordsClearEntry                                                      \
	FIXED_ADDRESS((s32 (*)(void *destination, s32 size)), 0x03000164,          \
				  IwramClearWords)
#define syWordsFillEntry                                                       \
	FIXED_ADDRESS((s32 (*)(void *destination, s32 size, u32 value)),           \
				  0x03000168, IwramFillWords)
#define syMathSqrtEntry                                                        \
	FIXED_ADDRESS((s32 (*)(s32 value)), 0x030001d8, IwramSqrt)
#define fdDecompressBitRuns                                                    \
	FIXED_ADDRESS((s32 (*)(const void *bits, void *destination, s32 value)),   \
				  0x03000214, IwramExpandBitRuns)
/* Reads all three word-aligned coordinates before writing the result. Input */
/* may be an array or a record's coordinate prefix; output may overlap it. */
#define syVectorTransformEntry                                                 \
	FIXED_ADDRESS((s32 (*)(const void *vector, s32 *result)), 0x03000250,      \
				  IwramTransformVector)
#define syMatrixMultiplyEntry                                                  \
	FIXED_ADDRESS((void (*)(const FIXED *matrix)), 0x030002c0,                 \
				  IwramMultiplyMatrix)
#define syIntegerDivideEntry                                                   \
	FIXED_ADDRESS((s32 (*)(s32 dividend, s32 divisor)), 0x03000380,            \
				  IwramSignedDivide)
#define syIntegerDivideUnsignedEntry                                           \
	FIXED_ADDRESS((u32 (*)(u32 dividend, u32 divisor)), 0x030003f0,            \
				  IwramUnsignedDivide)
#define fdDecompressSprPictureEntry                                            \
	FIXED_ADDRESS((void *(*)(const void *source, void *destination)),          \
				  0x030005c0, IwramDecompress)
#define syWordsCopyEntry                                                       \
	FIXED_ADDRESS((s32 (*)(void *destination, const void *source, s32 size)),  \
				  0x03001388, IwramCopyWords)

/* Three IWRAM functions' distances in bytes from IwramIrqMain, for code that */
/* calls them from the interrupt handler's address. */
#define syWordsCopyOffset FIXED_OFFSET(5000, IwramCopyWords, IwramIrqMain)
#define syDivFXOffset	  FIXED_OFFSET(316, IwramDivFX, IwramIrqMain)
#define sySquartOffset	  FIXED_OFFSET(472, IwramSqrt, IwramIrqMain)

/* The matrix group. The 3D transform and its stack. */
/* A transform's twelve 16.16 words: a 3x3 rotation, then the translation's */
/* x, y and z. */
enum {
	MATRIX_TRANSLATION_X = 9,
	MATRIX_TRANSLATION_Y = 10,
	MATRIX_TRANSLATION_Z = 11,
	MATRIX_WORDS = 12
};

/* The oam_words group. OAM_WORDS, a sprite entry a function builds by hand and
 */
/* DMA copies to OAM. */
/* One sprite's two OAM words. */
/* volatile: the words are built in place for the DMA to copy to OAM. */
typedef struct OAM_WORDS {
	volatile u32 attr01; /* attr0 | attr1 << 16 */
	volatile u32 attr2;	 /* attr2 | affine value << 16 */
} OAM_WORDS;

/* The point group. Point, a position in 16.16 fixed point. */
/* A position in three coordinates, each in 16.16 fixed point. */
typedef struct POS {
	FIXED x; /* 16.16 fixed point */
	FIXED y; /* 16.16 fixed point */
	FIXED z; /* 16.16 fixed point */
} POS;

STATIC_ASSERT(sizeof(POS) == 12);
STATIC_ASSERT(offsetof(POS, x) == 0);
STATIC_ASSERT(offsetof(POS, y) == 4);
STATIC_ASSERT(offsetof(POS, z) == 8);

/* The projection group. The 3D projection. */
/* The 3D projection onto the screen: its focal length, near and far depths, */
/* and the screen point the view centres on. */
typedef struct PROJECTION {
	/* The focal length in 16.16 pixels: 80, half the screen height, over the */
	/* tangent of half the vertical field of view (syPerspective). */
	FIXED focal;
	FIXED near;	 /* nearer points are not drawn; 16.16 along the view axis */
	FIXED far;	 /* farther points are not drawn; 16.16 along the view axis */
	s32 centerX; /* screen pixels */
	s32 centerY; /* screen pixels */
} PROJECTION;

STATIC_ASSERT(sizeof(PROJECTION) == 20);
STATIC_ASSERT(offsetof(PROJECTION, centerX) == 12);
STATIC_ASSERT(offsetof(PROJECTION, centerY) == 16);

/* Where the battle centres its projection on the screen: across the middle, */
/* and 120 pixels down, below the screen's middle. */
#define BATTLE_PROJECTION_CENTER_X (DISPLAY_WIDTH / 2)
#define BATTLE_PROJECTION_CENTER_Y 120

/* The ram_buffer group. The buffers and IWRAM variables the code reaches at */
/* fixed addresses, and the state pointers of HeapSlots by name. */
/* The map and decode buffers, from here to Ram_BgTileBuffer, follow the */
/* overlay area at fixed addresses because code reaches across their */
/* boundaries by arithmetic: the map grid's cells fill 64 KB from */
/* Ram_MapGridCellBuffer, through the sprite slots and the decode buffer. */
#define Ram_MapGridCellBuffer                                                  \
	FIXED_ADDRESS((u8 *), 0x02010000, MapGridCellBuffer)
#define Ram_DecodeBuffer  FIXED_ADDRESS((u8 *), 0x0201c000, DecodeBuffer)
#define Ram_CellTiles	  FIXED_ADDRESS((u8 *), 0x02020000, CellTiles)
#define Ram_MapGridShapes FIXED_ADDRESS((u8 *), 0x0202c000, MapGridShapes)
#define Ram_BgTileBuffer  FIXED_ADDRESS((u8 *), 0x02038000, BgTileBuffer)

/* The battle's enemy sprite slots, 16 KB each, inside the map grid's cells. */
#define Ram_EnemySpriteSlots FIXED_ADDRESS((u8 *), 0x02018000, ActorSpriteSlots)

/* The sound driver's mixer, SoundMainRAM, 932 bytes. m4aSoundInit copies */
/* SOUND_MIXER_CODE_SIZE (1 KB) there at once, so the 92 bytes after the mixer
 */
/* in the ROM land just after it. */
#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03007000, SoundMainRAM)

/* The end of the IWRAM heap, which the allocators test against, and the */
/* bottom of the stacks. */
#define syRamIwramHeapEnd                                                      \
	FIXED_ADDRESS((u8 *), 0x03007800, CartridgeResetMarker)

/* The stack top sySynch() runs its frames from: the 512 bytes below it are */
/* what the frame loop, its tasks and the VBlank work always have. */
#define Ram_FrameWaitStackTop                                                  \
	FIXED_ADDRESS((u8 *), 0x03007a00, FrameWaitStackTop)

/* IWRAM variables the code reaches as constants; German moves them 16 bytes */
/* up. */
#if LANGUAGE == LANGUAGE_GERMAN
#define Ram_BgScroll		 FIXED_ADDRESS((u8 *), 0x03001ae0, BgScrollWork)
#define Ram_VramCacheEntries FIXED_ADDRESS((u8 *), 0x03001b20, VramCacheEntries)
#else
#define Ram_BgScroll		 FIXED_ADDRESS((u8 *), 0x03001ad0, BgScrollWork)
#define Ram_VramCacheEntries FIXED_ADDRESS((u8 *), 0x03001b10, VramCacheEntries)
#endif

/* State pointers in HeapSlots, by name, from the heap group's slot numbers. */
#define syRamMapGridState	  ((void **)&HeapSlots[HEAP_SLOT_MAP_GRID])
#define syRamCameraState	  ((void **)&HeapSlots[HEAP_SLOT_CAMERA])
#define syRamFieldState		  ((void **)&HeapSlots[HEAP_SLOT_FIELD])
#define syRamDispState		  ((void **)&HeapSlots[HEAP_SLOT_SCREEN_TRANSITION])
#define syRamActorEffectState ((void **)&HeapSlots[HEAP_SLOT_GROUND_MASK])
#define Ram_BattleEntryTimer  ((void **)&HeapSlots[HEAP_SLOT_BATTLE_ENTRY_TIMER])
#define syRamFieldEffectState ((void **)&HeapSlots[HEAP_SLOT_FIELD_EFFECT])

/* How many bytes FieldBlendStateWork's slot lies past FieldStateWork's in */
/* HeapSlots: the three slots from HEAP_SLOT_FIELD to HEAP_SLOT_FIELD_BLEND. */
/* Written as the number of bytes, three slots of four. */
#define Ram_FieldBlendStateOffset                                              \
	FIXED_OFFSET(12, FieldBlendStateWork, FieldStateWork)

/* The serial group. The link cable's runtime: serial packets, registers and */
/* transfer state. */
/* A link cable packet: its packet number, the consoles not yet heard from, */
/* its checksum and a 24-byte payload. */
typedef struct SERIAL_PACKET {
	u8 packetNum; /* SERIAL_STATE's packet number when the packet was built
				   */
	/* The consoles heard from before but not this frame (receivedMask ^ */
	/* currentMask); a receiver complements it once the checksum is good. */
	u8 missingMask;
	/* The complement of the sum of the packet's first 14 halfwords, taken with
	 */
	/* this field 0. */
	u16 checksum;
	u8 payload[24];
	u8 reservedTail[4]; /* past the 14 halfwords sent; never written */
} SERIAL_PACKET;

STATIC_ASSERT(sizeof(SERIAL_PACKET) == 32);
STATIC_ASSERT(offsetof(SERIAL_PACKET, packetNum) == 0);
STATIC_ASSERT(offsetof(SERIAL_PACKET, missingMask) == 1);
STATIC_ASSERT(offsetof(SERIAL_PACKET, checksum) == 2);
STATIC_ASSERT(offsetof(SERIAL_PACKET, payload) == 4);
STATIC_ASSERT(offsetof(SERIAL_PACKET, reservedTail) == 28);

/* SERIAL_STATE.phase. */
typedef enum SERIAL_PHASE {
	SERIAL_PHASE_SEARCHING = 0, /* looking for the partner */
	SERIAL_PHASE_CONNECTED = 1
} SERIAL_PHASE;

/* SERIAL_STATE.mode: how this console takes part in the link. */
typedef enum SERIAL_MODE {
	SERIAL_MODE_CHILD = 0,
	/* SIO_MULTI_SD alone of SIOCNT's SIO_START and SIO_MULTI_SD: this console
	 */
	/* armed timer 3 and clocks the transfers. */
	SERIAL_MODE_CLOCKING = SIO_MULTI_SD
} SERIAL_MODE;

/* The link status bits sySerialExchangePayloads returns. */
typedef enum LINK_STAT {
	LINK_STATUS_HEARD_BOTH = 3,	 /* both consoles heard from this frame */
	LINK_STATUS_CLOCKING = 0x80, /* this console clocks the transfers */
	/* The last transfer interrupt found SIOCNT's error bit (SIO_ERROR) set. */
	LINK_STATUS_ERROR = 0x1000,
	/* SIOCNT's multiplayer id is 2 or more. */
	LINK_STATUS_EXTRA_CONSOLE = 0x2000
} LINK_STAT;

/* The bytes of one packet buffer in SERIAL_STATE.storage. */
#define SERIAL_PACKET_BYTES 32

/* The link cable's runtime: its mode and phase, the consoles heard from, the */
/* send position and the packet buffers the interrupt handler fills. */
typedef struct SERIAL_STATE {
	/* An SERIAL_MODE; SERIAL_MODE_CLOCKING adds LINK_STATUS_CLOCKING to the
	 */
	/* link status. */
	u8 mode;
	u8 phase;		 /* an SERIAL_PHASE */
	u8 receivedMask; /* the consoles heard from since the link began */
	u8 curMask;		 /* the consoles heard from this frame */
	/* Each console's SERIAL_CHANNEL_FLAG bits, set by the transfer interrupt
	 */
	/* and cleared each frame. */
	u8 channelFlg[4];
	/* Nonzero lets a new packet restart timer 3. */
	u8 transferEnabled;
	/* SIOCNT's error bit (SIO_ERROR), which multiplayer mode sets when a */
	/* transfer fails, copied by each transfer interrupt; nonzero adds */
	/* LINK_STATUS_ERROR to the link status and refuses the packets heard */
	/* meanwhile. */
	u8 transferError;
	u8 reservedAfterError;
	/* The packet number, counted up each frame once connected. */
	u8 packetNum;
	u8 reservedAfterPacketNum[8];
	/* The halfword of sendBuf[1] sent next; -1 between packets, when */
	/* system_serial.c's SERIAL_IDLE_HALFWORD is sent and the two send buffers
	 */
	/* swap. */
	s32 sendIdx;
	/* Each console's next halfword in its incomingBuf; -1 after an idle */
	/* system_serial.c's SERIAL_IDLE_HALFWORD. */
	s32 receiveIdx[2];
	u8 reservedAfterReceiveIdx[8];
	/* The packet being built ([0]) and the packet being sent ([1]). */
	u16 *sendBuf[2];
	/* Each console's packet being received; two are used. */
	u16 *incomingBuf[4];
	/* Each console's last complete packet, swapped in by the interrupt. */
	u16 *readyBuf[4];
	/* The packets the frame code checks and copies, swapped with readyBuf */
	/* each frame. */
	u16 *pendingBuf[4];
	/* The buffers' memory: two 32-byte send buffers, then 96 bytes a console */
	/* for its incoming, ready and pending packets. */
	u8 storage[SERIAL_PACKET_BYTES * 8];
} SERIAL_STATE;

STATIC_ASSERT(sizeof(SERIAL_STATE) == 352);
STATIC_ASSERT(offsetof(SERIAL_STATE, mode) == 0);
STATIC_ASSERT(offsetof(SERIAL_STATE, transferEnabled) == 8);
STATIC_ASSERT(offsetof(SERIAL_STATE, sendIdx) == 20);
STATIC_ASSERT(offsetof(SERIAL_STATE, storage) == 96);

/* SERIAL_STATE.channelFlg: what the transfer interrupts saw of a console's */
/* packets since the last frame. */
typedef enum SERIAL_CHANNEL_FLAG {
	SERIAL_CHANNEL_PACKET_COMPLETE = 1, /* its fourteenth halfword arrived */
	/* a transfer interrupt found SIOCNT's error bit (SIO_ERROR) set */
	SERIAL_CHANNEL_ERROR = 2
} SERIAL_CHANNEL_FLAG;

/* SERIAL_TRANSFER_STATE.flg: how far this console's receive has got. */
typedef enum SERIAL_RECEIVE_STATE {
	SERIAL_RECEIVE_IDLE = 0,
	SERIAL_RECEIVE_ACTIVE = 1,
	SERIAL_RECEIVE_DONE = 2 /* the last block taken */
} SERIAL_RECEIVE_STATE;

/* SERIAL_TRANSFER_STATE.peerFlg: the kind of block this console sends. */
typedef enum SERIAL_SEND_STATE {
	SERIAL_SEND_IDLE = 0,
	SERIAL_SEND_MORE = 1, /* a block with more to follow */
	SERIAL_SEND_LAST = 2
} SERIAL_SEND_STATE;

/* SERIAL_TRANSFER_STATE.active and SerialBlockNumber hold a block number in */
/* their low seven bits, counting up and wrapping; the receiver's, with bit 7 */
/* set, asks the sender to rewind to that block (sySerialStepBlockTransfer). */
#define SERIAL_BLOCK_NUMBER_MASK 0x7f
#define SERIAL_BLOCK_REWIND		 0x80 /* bit 7: rewind to the block number */
/* SERIAL_TRANSFER_STATE.stat's bit 7, set when a send or receive starts; the
 */
/* receiver counts its blocks in the low bits (sySerialStepBlockTransfer). */
#define SERIAL_STATUS_RUNNING 0x80

/* A console's block transfer state, exchanged with the partner each VBlank: */
/* its block number, its status and flags, and a 20-byte block. */
typedef struct SERIAL_TRANSFER_STATE {
	/* The number of the block in payload; as receiver, the one it */
	/* expects with bit 7 set to ask for a resend; 1 when a transfer starts. */
	u8 active;
	/* SERIAL_STATUS_RUNNING when started, plus 1 for a receive; the receiver */
	/* counts blocks (status + 1, with bit 7) and sets 0 after the last. */
	u8 stat;
	u8 flg;			/* the receive state, an SERIAL_RECEIVE_STATE */
	u8 peerFlg;		/* the send state, an SERIAL_SEND_STATE */
	u8 payload[20]; /* one block of the data sent */
} SERIAL_TRANSFER_STATE;

STATIC_ASSERT(sizeof(SERIAL_TRANSFER_STATE) == 24);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, active) == 0);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, stat) == 1);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, flg) == 2);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, peerFlg) == 3);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, payload) == 4);

/* The vector3 group. Vector3, a three-coordinate vector. */
/* Three coordinates, x, y and z: a position, a velocity or a direction. */
typedef struct VECTOR3 {
#ifdef UBFIX
	union {
		struct {
			s32 x;
			s32 y;
			s32 z;
		};
		s32 coordinates[3]; /* x, y and z, as syVector3Coordinates returns them
							 */
	};
#else
	s32 x;
	s32 y;
	s32 z;
#endif
} VECTOR3;

STATIC_ASSERT(sizeof(VECTOR3) == 12);
STATIC_ASSERT(offsetof(VECTOR3, x) == 0);
STATIC_ASSERT(offsetof(VECTOR3, y) == 4);
STATIC_ASSERT(offsetof(VECTOR3, z) == 8);

/* The vram_cache group. The sprite VRAM block cache and its owners. */
/* The sprite VRAM cache's sizes and its free marks. Sprite tile memory, from */
/* OBJ_VRAM0, is split into blocks that the cache entries own. */
typedef enum VRAM_CACHE_LIMITS {
	/* Cache entries; syVramCacheFindFreeEntry returns this when none is free.
	 */
	VRAM_CACHE_ENTRY_COUNT = 96,
	VRAM_BLOCK_COUNT = 512, /* blocks of sprite VRAM */
	VRAM_BLOCK_BYTES = 64,
	VRAM_BLOCK_OWNER_FREE = 255, /* VramCacheBlockOwners of a free block */
	/* VRAM_CACHE_ENTRY.ofs of a free entry. */
	VRAM_CACHE_OFFSET_FREE = 65535,
	/* The largest size an entry has while it holds no blocks: */
	/* syVramCacheReleaseVram leaves 1, which keeps the entry taken. */
	VRAM_CACHE_SIZE_NO_BLOCKS = 16,
	/* The most bytes syVramCacheLoad takes at once: 128 blocks, a quarter of */
	/* sprite VRAM. */
	VRAM_CACHE_LOAD_MAX = 8192
} VRAM_CACHE_LIMITS;

/* The cache entries kept for one use each, loaded by number. */
typedef enum VRAM_CACHE_FIXED_ENTRY {
	/* The actors' shared shadow (Actor_ShadowTiles), each sprite animation's */
	/* part 1 (fdActorSystemConfigure). */
	VRAM_CACHE_ENTRY_ACTOR_SHADOW = 93,
	/* The ground mask's solid tiles (fdEffectStartGroundMask). */
	VRAM_CACHE_ENTRY_GROUND_MASK = 94,
	/* The window sprites' glyph tiles (WIN_STATE.glyphTiles). */
	VRAM_CACHE_ENTRY_GLYPH_TILES = 95
} VRAM_CACHE_FIXED_ENTRY;

/* A sprite slot of the VRAM cache: the size of its block and where the block */
/* starts in sprite VRAM. */
typedef struct VRAM_CACHE_ENTRY {
	u16 size; /* bytes */
	u16 ofs;  /* bytes from the start of sprite VRAM */
} VRAM_CACHE_ENTRY;

STATIC_ASSERT(sizeof(VRAM_CACHE_ENTRY) == 4);
STATIC_ASSERT(offsetof(VRAM_CACHE_ENTRY, ofs) == 2);

/* syVramCacheLoad's source that clears the entry's VRAM instead of copying. */
#define VRAM_CACHE_CLEAR ((const void *)-1)

#endif
