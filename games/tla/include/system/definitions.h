#ifndef ALCHEMY_DEFINITIONS_H
#define ALCHEMY_DEFINITIONS_H

// The system library's types and constants, as SGL's sl_def.h holds its own: the hardware's
// registers, and each group's structures, enums and fixed values. system.h includes it; a file
// built by another compiler can include it alone.

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "static_assert.h"

// The io_reg group. The hardware's I/O registers and their bits. mode, layers, windows
#define REG_DISPCNT              (*(volatile Uint16 *)0x04000000)
// blank flags and IRQs
#define REG_DISPSTAT             (*(volatile Uint16 *)0x04000004)
// line being drawn, 0..227
#define REG_VCOUNT               (*(volatile Uint16 *)0x04000006)
// The lines REG_VCOUNT counts through in a tick: the DISPLAY_HEIGHT drawn and those of VBlank. A
// line's HBlank comes before the next line is drawn, the last line's before line 0.
#define DISPLAY_TOTAL_LINES      228
// Each background's control: its priority, tile and map blocks, colours and size, composed from the
// BGCNT_ values below.
#define REG_BG1CNT               (*(volatile Uint16 *)0x0400000a) // BG1
#define REG_BG2CNT               (*(volatile Uint16 *)0x0400000c) // BG2
#define REG_BG3CNT               (*(volatile Uint16 *)0x0400000e) // BG3

#define BGCNT_PRIORITY(n)        (n) // draw order, 0 in front to 3 behind
// The 16 KB character block, 0 to 3, the layer's tiles are read from.
#define BGCNT_CHAR_BASE(block)   ((block) << 2)
#define BGCNT_MOSAIC             0x0040 // applies REG_MOSAIC's block size
#define BGCNT_256_COLORS         0x0080 // 8bpp tiles, not 16-colour 4bpp ones
// The 2 KB screen block, 0 to 31, the layer's map is read from.
#define BGCNT_SCREEN_BASE(block) ((block) << 8)
// Affine backgrounds only: wrap around off the layer's area, which is otherwise transparent.
#define BGCNT_WRAP               0x2000
// The background's size: for an affine one, 0 to 3 are 128, 256, 512 and 1024 pixels square.
#define BGCNT_SIZE(n)            ((n) << 14)

// REG_DISPCNT's video modes and bits: each _ON bit shows its layer or turns its window on.
typedef enum DISPLAY_CONTROL
{
    DISPCNT_MODE_0     = 0x0000, // four text backgrounds
    DISPCNT_MODE_1     = 0x0001, // BG0 and BG1 text, BG2 affine
    DISPCNT_MODE_2     = 0x0002, // BG2 and BG3 affine
    DISPCNT_OBJ_1D_MAP = 0x0040, // sprite tiles in one run, not a 2D sheet
    DISPCNT_BG0_ON     = 0x0100,
    DISPCNT_BG1_ON     = 0x0200,
    DISPCNT_BG2_ON     = 0x0400,
    DISPCNT_BG3_ON     = 0x0800,
    DISPCNT_OBJ_ON     = 0x1000,
    DISPCNT_WIN0_ON    = 0x2000,
    DISPCNT_WIN1_ON    = 0x4000,
    DISPCNT_OBJWIN_ON  = 0x8000 // the sprite window
} DISPLAY_CONTROL;

// REG_DISPCNT's three video mode bits, which hold a DISPCNT_MODE_ value.
#define DISPCNT_MODE_MASK                0x0007
// The text backgrounds' scroll offsets, write-only: the map pixel, 0 to 511, at the screen's left
// edge (HOFS) or top edge (VOFS).
#define REG_BG1HOFS                      (*(volatile Uint16 *)0x04000014) // BG1, horizontal
#define REG_BG1VOFS                      (*(volatile Uint16 *)0x04000016) // BG1, vertical
#define REG_BG2HOFS                      (*(volatile Uint16 *)0x04000018) // BG2, horizontal
#define REG_BG2VOFS                      (*(volatile Uint16 *)0x0400001a) // BG2, vertical
#define REG_BG3HOFS                      (*(volatile Uint16 *)0x0400001c) // BG3, horizontal
#define REG_BG3VOFS                      (*(volatile Uint16 *)0x0400001e) // BG3, vertical

// The BG2/BG3 affine matrix entries are signed 8.8 fixed point: texels stepped per screen pixel.
// Q_8_8(1) draws the layer at its own size and Q_8_8(0.5) twice as large.
#define Q_8_8(n)                         ((Sint16)((n) * 256))

// The affine backgrounds' matrices and origins, write-only.
#define REG_BG2PA                        (*(volatile Uint16 *)0x04000020) // BG2: x step a pixel
#define REG_BG2PB                        (*(volatile Uint16 *)0x04000022) // BG2: x step a line
#define REG_BG2PC                        (*(volatile Uint16 *)0x04000024) // BG2: y step a pixel
#define REG_BG2PD                        (*(volatile Uint16 *)0x04000026) // BG2: y step a line
#define REG_BG2X                         (*(volatile Sint32 *)0x04000028) // BG2: origin x
#define REG_BG2Y                         (*(volatile Sint32 *)0x0400002c) // BG2: origin y
#define REG_BG3PA                        (*(volatile Uint16 *)0x04000030) // BG3: x step a pixel
#define REG_BG3PB                        (*(volatile Uint16 *)0x04000032) // BG3: x step a line
#define REG_BG3PC                        (*(volatile Uint16 *)0x04000034) // BG3: y step a pixel
#define REG_BG3PD                        (*(volatile Uint16 *)0x04000036) // BG3: y step a line
#define REG_BG3X                         (*(volatile Sint32 *)0x04000038) // BG3: origin x
#define REG_BG3Y                         (*(volatile Sint32 *)0x0400003c) // BG3: origin y

// The two windows' edges, each a WIN_RANGE value in pixels.
#define REG_WIN0H                        (*(volatile Uint16 *)0x04000040) // window 0, left and right
#define REG_WIN1H                        (*(volatile Uint16 *)0x04000042) // window 1, left and right
#define REG_WIN0V                        (*(volatile Uint16 *)0x04000044) // window 0, top and bottom
#define REG_WIN1V                        (*(volatile Uint16 *)0x04000046) // window 1, top and bottom
// A REG_WINxH or REG_WINxV value: the window covers start up to, but not including, end.
#define WIN_RANGE(start, end)            (((start) << 8) | (end))
// A WIN_RANGE value's end, its low byte: the window's right or bottom edge.
#define WIN_RANGE_END_MASK               0xff
// what windows 0 and 1 show
#define REG_WININ                        (*(volatile Uint16 *)0x04000048)
#define REG_WINOUT                       (*(volatile Uint16 *)0x0400004a) // what the rest shows

// REG_WININ: the layers shown inside window 0 (low byte) and window 1 (high byte). CLR lets the
// colour effect of REG_BLDCNT apply there.
#define WININ_WIN0_BG0                   0x0001 // window 0: BG0
#define WININ_WIN0_BG1                   0x0002 // window 0: BG1
#define WININ_WIN0_BG2                   0x0004 // window 0: BG2
#define WININ_WIN0_BG3                   0x0008 // window 0: BG3
#define WININ_WIN0_OBJ                   0x0010 // window 0: sprites
#define WININ_WIN0_CLR                   0x0020 // window 0: the colour effect
#define WININ_WIN0_ALL                   0x003f // window 0: everything
#define WININ_WIN1_BG0                   0x0100 // window 1: BG0
#define WININ_WIN1_BG1                   0x0200 // window 1: BG1
#define WININ_WIN1_BG2                   0x0400 // window 1: BG2
#define WININ_WIN1_BG3                   0x0800 // window 1: BG3
#define WININ_WIN1_OBJ                   0x1000 // window 1: sprites
#define WININ_WIN1_CLR                   0x2000 // window 1: the colour effect
#define WININ_WIN1_ALL                   0x3f00 // window 1: everything

// REG_WINOUT: the layers shown outside both windows (low byte) and inside the sprite window (high
// byte).
#define WINOUT_WIN01_BG0                 0x0001 // outside: BG0
#define WINOUT_WIN01_BG1                 0x0002 // outside: BG1
#define WINOUT_WIN01_BG2                 0x0004 // outside: BG2
#define WINOUT_WIN01_BG3                 0x0008 // outside: BG3
#define WINOUT_WIN01_OBJ                 0x0010 // outside: sprites
#define WINOUT_WIN01_CLR                 0x0020 // outside: the colour effect
#define WINOUT_WIN01_ALL                 0x003f // outside: everything
#define WINOUT_WINOBJ_BG0                0x0100 // sprite window: BG0
#define WINOUT_WINOBJ_BG1                0x0200 // sprite window: BG1
#define WINOUT_WINOBJ_BG2                0x0400 // sprite window: BG2
#define WINOUT_WINOBJ_BG3                0x0800 // sprite window: BG3
#define WINOUT_WINOBJ_OBJ                0x1000 // sprite window: sprites
#define WINOUT_WINOBJ_CLR                0x2000 // sprite window: the colour effect
#define WINOUT_WINOBJ_ALL                0x3f00 // sprite window: everything

#define REG_MOSAIC                       (*(volatile Uint16 *)0x0400004c) // mosaic block sizes
// colour effect, targets
#define REG_BLDCNT                       (*(volatile Uint16 *)0x04000050)

// REG_BLDCNT: the first target layers, the colour effect, then the second target layers that a
// blend mixes the first ones with.
#define BLDCNT_TGT1_BG0                  0x0001 // first target: BG0
#define BLDCNT_TGT1_BG1                  0x0002 // first target: BG1
#define BLDCNT_TGT1_BG2                  0x0004 // first target: BG2
#define BLDCNT_TGT1_BG3                  0x0008 // first target: BG3
#define BLDCNT_TGT1_OBJ                  0x0010 // first target: sprites
#define BLDCNT_TGT1_BD                   0x0020 // first target: the backdrop
#define BLDCNT_TGT1_ALL                  0x003f // first target: every layer
#define BLDCNT_EFFECT_NONE               0x0000 // no effect, the targets as drawn
#define BLDCNT_EFFECT_BLEND              0x0040 // weighted by REG_BLDALPHA
#define BLDCNT_EFFECT_LIGHTEN            0x0080 // towards white by REG_BLDY
#define BLDCNT_EFFECT_DARKEN             0x00c0 // towards black by REG_BLDY
#define BLDCNT_EFFECT_MASK               0x00c0 // the effect's two bits
#define BLDCNT_TGT2_BG0                  0x0100 // second target: BG0
#define BLDCNT_TGT2_BG1                  0x0200 // second target: BG1
#define BLDCNT_TGT2_BG2                  0x0400 // second target: BG2
#define BLDCNT_TGT2_BG3                  0x0800 // second target: BG3
#define BLDCNT_TGT2_OBJ                  0x1000 // second target: sprites
#define BLDCNT_TGT2_BD                   0x2000 // second target: the backdrop
#define BLDCNT_TGT2_ALL                  0x3f00 // second target: every layer

// the blend's weights
#define REG_BLDALPHA                     (*(volatile Uint16 *)0x04000052)
// A blend weight's full strength: BLEND_ALPHA(BLEND_WEIGHT_MAX, 0) shows only the top layer.
#define BLEND_WEIGHT_MAX                 16
// A REG_BLDALPHA value: top and bottom are the two targets' weights, out of BLEND_WEIGHT_MAX.
#define BLEND_ALPHA(top, bottom)         ((top) | ((bottom) << 8))
// REG_BLDALPHA's low byte, the top target's weight.
#define BLEND_ALPHA_TOP_MASK             0xff
#define REG_BLDY                         (*(volatile Uint16 *)0x04000054) // fade level, 0 to 16
// REG_BLDY's whole fade: the targets drawn all white, or all black.
#define BLEND_FADE_MAX                   16

// Screen block 7: BG2's affine map of a battle effect's canvas, a byte a tile.
#define BG_CANVAS_MAP                    ((volatile Sint16 *)BG_SCREEN_BLOCK(7))
// BG0HOFS as an address for a DMA: copies of BgScrollWork over all four backgrounds' scroll
// registers start here, and Mind Read's HBlank DMA writes one halfword a line into it.
#define REG_BG0HOFS                      ((void *)0x04000010)

// DMA channel 0's registers as halfwords, for the line scrolls that set the whole channel up.
#define REG_DMA0                         ((volatile Uint16 *)0x040000b0)

// DMA channel 3's registers as words: the channel syDMAXCopy copies with.
#define REG_DMA3                         ((volatile Uint32 *)0x040000d4)

// Background colour 0, the backdrop shown where no layer draws.
#define PLTT_BACKDROP                    (*(volatile Uint16 *)0x05000000)

#define BG_PLTT_SIZE                     512 // bytes of background colours, 256 of them
#define BG_PLTT_COLORS                   (BG_PLTT_SIZE / 2)  // the background's 256 colours
#define OBJ_PLTT_SIZE                    512                 // bytes of sprite colours, 256 of them
#define OBJ_PLTT_COLORS                  (OBJ_PLTT_SIZE / 2) // the sprites' 256 colours
// Colours in palette RAM: the background's, then the sprites'.
#define PLTT_COLORS                      (2 * BG_PLTT_COLORS)

// 16 palettes of 16 colours
#define BG_PLTT                          ((volatile Uint16 *)0x05000000)
// Colours in one 16-colour palette.
#define PALETTE_COLORS                   16
// Bytes in one 16-colour palette, as a 16-colour picture's resource begins with one.
#define PALETTE_BYTES                    (PALETTE_COLORS * 2)
// Colour `color` of 16-colour background palette `palette`.
#define BG_PLTT_COLOR(palette, color)    (BG_PLTT[(palette) * PALETTE_COLORS + (color)])
// the sprites' 256 colours
#define OBJ_PLTT                         ((volatile Uint16 *)0x05000200)
// Colour `color` of 16-colour sprite palette `palette`.
#define OBJ_PLTT_COLOR(palette, color)   (OBJ_PLTT[(palette) * PALETTE_COLORS + (color)])

// Where a 15-bit colour keeps its green and blue levels, above red's five bits.
#define RGB_GREEN_SHIFT                  5
#define RGB_BLUE_SHIFT                   10
// A 15-bit colour from its red, green and blue levels (0 to 31 each), as the palette stores it:
// blue in the high bits.
#define RGB(r, g, b)                     (((b) << RGB_BLUE_SHIFT) | ((g) << RGB_GREEN_SHIFT) | (r))

// The start of the 32 KB of IWRAM, the CPU's fast internal work memory.
#define IWRAM_START                      0x03000000
// The start of the 256 KB of EWRAM, the slower external work memory.
#define EWRAM_START                      0x02000000
// The end of EWRAM: the limit the EWRAM heap's allocators test.
#define EWRAM_END                        0x02040000

// Background tile memory's 16 KB character block `n`, 0 to 3.
#define BG_CHAR_BLOCK(n)                 ((Uint8 *)VRAM + (n) * BG_CHAR_SIZE)
// The 2 KB screen block `n` of VRAM, 0 to 31, that holds a map.
#define BG_SCREEN_BLOCK(n)               ((Uint8 *)VRAM + (n) * BG_SCREEN_SIZE)
#define TILE_SIZE_4BPP                   32 // bytes in an 8x8 tile of 16 colours
#define TILE_SIZE_8BPP                   64 // bytes in an 8x8 tile of 256 colours
// The bytes of one pixel row of a 16-colour tile.
#define TILE_ROW_SIZE_4BPP               (TILE_SIZE_4BPP / 8)
// The tiles a sprite of `width` by `height` pixels takes in sprite tile memory, counted in its
// 32-byte steps, of 16 colours and of 256 (two steps a tile): a picture's sprites follow one
// another so.
#define SPRITE_TILES(width, height)      ((width) / 8 * ((height) / 8))
#define SPRITE_TILES_8BPP(width, height) (2 * SPRITE_TILES(width, height))
// Sprite tile memory: the 32 KB from 0x06010000.
#define OBJ_VRAM0                        0x06010000
// Sprite tile memory as bytes, where a sprite's tile number counts from.
#define OBJ_VRAM                         ((Uint8 *)OBJ_VRAM0)

// Timer 3's reload value and control as one word; it paces the serial transfers.
#define REG_TM3CNT                       (*(volatile Uint32 *)0x0400010c)
#define REG_TM3CNT_H                     (*(volatile Uint16 *)0x0400010e) // timer 3's control

// TMxCNT_H flags.
#define TIMER_INTR_ENABLE                0x0040
#define TIMER_ENABLE                     0x0080 // starts the timer counting
// counts every 256 cycles, 65536 times a second
#define TIMER_256CLK                     0x0002

// The serial data: the 32-bit mode's word, or multiplayer's four received halfwords.
#define REG_SIODATA32                    ((volatile Uint32 *)0x04000120)
// Serial control read as a word, with SIODATA8 above it.
#define REG_SIOCNT                       (*(volatile Uint32 *)0x04000128)
// serial control alone
#define REG_SIOCNT16                     (*(volatile Uint16 *)0x04000128)
// The 8-bit mode's data, which multiplayer mode sends from.
#define REG_SIODATA8                     (*(volatile Uint16 *)0x0400012a)
#define REG_KEYINPUT                     (*(volatile Uint16 *)0x04000130) // a 0 bit is a held key
#define REG_KEYCNT                       (*(volatile Uint16 *)0x04000132) // keys for the keypad IRQ
#define REG_RCNT                         (*(volatile Uint16 *)0x04000134) // the serial port's mode

// SIOCNT flags.
#define SIO_115200_BPS                   0x0003 // multiplayer's fastest baud rate
#define SIO_START                        0x0080 // starts a transfer; set while one runs
#define SIO_32BIT_MODE                   0x1000 // normal mode, 32 bits a transfer
#define SIO_MULTI_MODE                   0x2000 // multiplayer mode
#define SIO_INTR_ENABLE                  0x4000 // an interrupt when a transfer ends

// RCNT's mode: 0 leaves the port to SIOCNT's serial modes; bit 15 alone makes it general-purpose
// pins.
#define RCNT_SIO_MODE                    0x0000
#define RCNT_GPIO_MODE                   0x8000 // the port as general-purpose pins

// DMAxCNT_H flags. A 32-bit control word takes them shifted above its count: (DMA_ENABLE |
// DMA_32BIT) << 16 | words.
#define DMA_DEST_INC                     0x0000 // destination steps up
#define DMA_DEST_DEC                     0x0020 // destination steps down
#define DMA_DEST_FIXED                   0x0040 // destination stays, as for a register
#define DMA_DEST_RELOAD                  0x0060 // steps up, back to its start each repeat
#define DMA_SRC_INC                      0x0000 // source steps up
#define DMA_SRC_DEC                      0x0080 // source steps down
#define DMA_SRC_FIXED                    0x0100 // source stays, as for a fill
#define DMA_REPEAT                       0x0200 // runs again at each start condition
#define DMA_16BIT                        0x0000 // moves halfwords
#define DMA_32BIT                        0x0400 // moves words
#define DMA_DREQ_ON                      0x0800 // channel 3: the cartridge requests it
#define DMA_START_NOW                    0x0000 // as soon as it's enabled
#define DMA_START_VBLANK                 0x1000
#define DMA_START_HBLANK                 0x2000
// channels 1 and 2: a sound FIFO's request
#define DMA_START_SPECIAL                0x3000
#define DMA_INTR_ENABLE                  0x4000 // an interrupt when it ends
#define DMA_ENABLE                       0x8000 // arms the channel
// Where a DMA channel's control halfword (the DMA_ flags) sits in its 32-bit count word.
#define DMA_CONTROL_SHIFT                16
// DMA_ENABLE in a channel's 32-bit count and control word, which the hardware clears once the
// transfer is done.
#define DMA_BUSY                         ((Uint32)DMA_ENABLE << 16)
// A DMA control's start timing (bits 12 and 13), cartridge request (bit 11) and repeat (bit 9): the
// bits cleared before a channel is stopped.
#define DMA_START_TIMING_DRQ_REPEAT      (DMA_START_SPECIAL | DMA_DREQ_ON | DMA_REPEAT)

// DMA control words: copy n words or halfwords at once, or one halfword or word each HBlank to a
// register reloaded every line (a scanline table that drives a window edge, a scroll or an affine
// register).
#define DMA_COPY_WORDS(n)                ((DMA_ENABLE | DMA_32BIT) << 16 | (n))
// Fill n words or halfwords with the value at the source, as The Broken Seal's.
#define DMA_FILL_WORDS(n)                ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (n))
#define DMA_FILL_HALFWORDS(n)            ((DMA_ENABLE | DMA_SRC_FIXED) << 16 | (n))
#define DMA_COPY_HALFWORDS(n)            (DMA_ENABLE << 16 | (n))
// n halfwords copied from the last back to the first, source and destination stepping down, so that
// a run can move up over itself.
#define DMA_COPY_HALFWORDS_DOWN(n)       ((DMA_ENABLE | DMA_SRC_DEC | DMA_DEST_DEC) << 16 | (n))
// A halfword each HBlank, as for a scroll or a window edge.
#define DMA_HBLANK_HALFWORD_PER_LINE                                                               \
    ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD) << 16 | 1)
// n words each HBlank, as for the affine registers of a background written together.
#define DMA_HBLANK_WORDS_PER_LINE(n)                                                               \
    ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT) << 16 | (n))
// A word each HBlank, as for a pair of registers written together.
#define DMA_HBLANK_WORD_PER_LINE DMA_HBLANK_WORDS_PER_LINE(1)

// BIOS CpuSet control flags, above its count.
#define CPU_SET_SRC_FIXED        0x01000000
#define CPU_SET_32BIT            0x04000000           // words rather than halfwords

#define OAM                      ((void *)0x07000000) // the sprites' attributes

// The keys' bits as REG_KEYINPUT and REG_KEYCNT lay them out; the game's key state (PerWork) holds
// them set for a pressed key.
typedef enum KEY_MASK
{
    KEY_A           = 0x001,
    KEY_B           = 0x002,
    KEY_SELECT      = 0x004,
    KEY_START       = 0x008,
    KEY_RIGHT       = 0x010,
    KEY_LEFT        = 0x020,
    KEY_UP          = 0x040,
    KEY_DOWN        = 0x080,
    KEY_R           = 0x100,
    KEY_L           = 0x200,
    KEYS_HORIZONTAL = KEY_RIGHT | KEY_LEFT,
    KEYS_VERTICAL   = KEY_UP | KEY_DOWN,
    KEYS_DPAD       = KEYS_HORIZONTAL | KEYS_VERTICAL,
    KEYS_SHOULDERS  = KEY_R | KEY_L
} KEY_MASK;

#define KEYS_MASK                  0x3ff // the ten keys' bits; KEYINPUT reads 0 for a held key

// The direction pad's four keys (Up, Down, Left, Right) as an index 0 to 15.
#define DPAD_INDEX(keys)           (((keys) >> 4) & 15)

// BG0's control (BGCNT_)
#define REG_BG0CNT                 (*(volatile Uint16 *)0x04000008)
// BG0's vertical scroll
#define REG_BG0VOFS                (*(volatile Uint16 *)0x04000012)
// Direct sound's mixing: volumes, speakers, timers and FIFO resets.
#define REG_SOUNDCNT_H             (*(volatile Uint16 *)0x04000082)
// SOUNDCNT_H flags: the mixing volumes, then each direct sound channel's speakers, timer and FIFO
// reset.
#define SOUND_CGB_MIX_FULL         0x0002 // sounds 1 to 4 at full volume
#define SOUND_A_MIX_FULL           0x0004 // direct sound A at full, not half
#define SOUND_B_MIX_FULL           0x0008 // direct sound B at full, not half
// Sounds 1 to 4 and both direct sound channels all at full volume.
#define SOUND_ALL_MIX_FULL         (SOUND_CGB_MIX_FULL | SOUND_A_MIX_FULL | SOUND_B_MIX_FULL)
#define SOUND_A_RIGHT_OUTPUT       0x0100                           // A to the right speaker
#define SOUND_A_LEFT_OUTPUT        0x0200                           // A to the left speaker
#define SOUND_A_TIMER_0            0x0000                           // A plays at timer 0's rate
#define SOUND_A_FIFO_RESET         0x0800                           // empties A's FIFO
#define SOUND_B_RIGHT_OUTPUT       0x1000                           // B to the right speaker
#define SOUND_B_LEFT_OUTPUT        0x2000                           // B to the left speaker
#define SOUND_B_TIMER_0            0x0000                           // B plays at timer 0's rate
#define SOUND_B_FIFO_RESET         0x8000                           // empties B's FIFO
#define REG_SOUNDBIAS              (*(volatile Uint16 *)0x04000088) // output bias, rate
// A's samples, 4 a write
#define REG_FIFO_A                 (*(volatile Uint32 *)0x040000a0)
// B's samples, 4 a write
#define REG_FIFO_B                 (*(volatile Uint32 *)0x040000a4)
// The four DMA channels: source, destination, then the count with the control halfword (CNT_H, the
// DMA_ flags) above it.
#define REG_DMA0SAD                (*(volatile Uint32 *)0x040000b0) // channel 0: source
#define REG_DMA0DAD                (*(volatile Uint32 *)0x040000b4) // channel 0: destination
#define REG_DMA0CNT                (*(volatile Uint32 *)0x040000b8) // channel 0: count, control
// channel 0: control
#define REG_DMA0CNT_H              (*(volatile Uint16 *)0x040000ba)
#define REG_DMA1SAD                (*(volatile Uint32 *)0x040000bc) // channel 1: source
#define REG_DMA1DAD                (*(volatile Uint32 *)0x040000c0) // channel 1: destination
#define REG_DMA1CNT                (*(volatile Uint32 *)0x040000c4) // channel 1: count, control
// channel 1: control
#define REG_DMA1CNT_H              (*(volatile Uint16 *)0x040000c6)
#define REG_DMA2SAD                (*(volatile Uint32 *)0x040000c8) // channel 2: source
#define REG_DMA2DAD                (*(volatile Uint32 *)0x040000cc) // channel 2: destination
#define REG_DMA2CNT                (*(volatile Uint32 *)0x040000d0) // channel 2: count, control
// channel 2: control
#define REG_DMA2CNT_H              (*(volatile Uint16 *)0x040000d2)
#define REG_DMA3SAD                (*(volatile Uint32 *)0x040000d4) // channel 3: source
#define REG_DMA3DAD                (*(volatile Uint32 *)0x040000d8) // channel 3: destination
#define REG_DMA3CNT                (*(volatile Uint32 *)0x040000dc) // channel 3: count, control
// channel 3: control
#define REG_DMA3CNT_H              (*(volatile Uint16 *)0x040000de)
// Timers 0 to 2: CNT_L is the reload value, and reads back the count; CNT_H takes the TIMER_ flags.
#define REG_TM0CNT_L               (*(volatile Uint16 *)0x04000100) // timer 0: reload
#define REG_TM0CNT_H               (*(volatile Uint16 *)0x04000102) // timer 0: control
#define REG_TM1CNT_L               (*(volatile Uint16 *)0x04000104) // timer 1: reload
#define REG_TM1CNT_H               (*(volatile Uint16 *)0x04000106) // timer 1: control
#define REG_TM2CNT_L               (*(volatile Uint16 *)0x04000108) // timer 2: reload
#define REG_TM2CNT_H               (*(volatile Uint16 *)0x0400010a) // timer 2: control

#define REG_IE                     (*(volatile Uint16 *)0x04000200) // the interrupts enabled
#define REG_IF                     (*(volatile Uint16 *)0x04000202) // pending; a 1 written clears
// cartridge wait states
#define REG_WAITCNT                (*(volatile Uint16 *)0x04000204)
// WAITCNT's slowest SRAM wait (bits 1 and 0), 8 cycles: the wait the SDK's flash functions read the
// chip with; programming and erasing use the chip's wait[0], then restore it.
#define WAITCNT_SRAM_SLOWEST       0x0003
// WAITCNT's SRAM wait bits.
#define WAITCNT_SRAM_MASK          0x0003
#define REG_IME                    (*(volatile Uint16 *)0x04000208) // all interrupts' master switch

// KEYCNT values: the interrupt fires when all the listed keys are held at once.
#define KEYCNT_SOFT_RESET          0xc00f // A, B, Select and Start.
#define KEYCNT_SHOULDERS_WAKE      0xc300 // L and R; no code uses it.
#define KEYCNT_SLEEP_WAKE          0xc304 // L, R and Select, which wake the sleep.

// ROM wait states 3 and 1 with the prefetch buffer on, SRAM at 4; written by the power-on set-up.
#define WAITCNT_GAME               0x4014

// Display status bits and the screen's size.
#define DISPSTAT_VBLANK_INTR       0x0008 // at the start of each VBlank
#define DISPSTAT_HBLANK_INTR       0x0010 // at the end of each line
#define DISPSTAT_VCOUNT_INTR       0x0020 // on the DISPSTAT_VCOUNT_LINE line
// DISPSTAT's high byte: the line the VCount interrupt fires on (the LYC compare).
#define DISPSTAT_VCOUNT_LINE(line) ((line) << 8)
// The screen's size in pixels.
#define DISPLAY_WIDTH              240
#define DISPLAY_HEIGHT             160
// The screen's size in 8-pixel tiles.
#define DISPLAY_TILE_WIDTH         (DISPLAY_WIDTH / 8)
#define DISPLAY_TILE_HEIGHT        (DISPLAY_HEIGHT / 8)
// A 15-bit colour's red, green and blue levels, 0 to 31 each.
#define RGB_RED(color)             ((color) & 0x1f)
#define RGB_GREEN(color)           (((color) >> RGB_GREEN_SHIFT) & 0x1f)
#define RGB_BLUE(color)            (((color) >> RGB_BLUE_SHIFT) & 0x1f)
// A colour channel's brightest level.
#define RGB_LEVEL_MAX              31
// The brightest colour, every channel at its highest level.
#define RGB_WHITE                  RGB(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX)
// A 15-bit colour's red, green and blue bits.
#define RED_BITS                   RGB(RGB_LEVEL_MAX, 0, 0)
#define GREEN_BITS                 RGB(0, RGB_LEVEL_MAX, 0)
#define BLUE_BITS                  RGB(0, 0, RGB_LEVEL_MAX)
// Video RAM: background tiles and maps, then sprite tiles.
#define VRAM                       0x06000000
#define BG_CHAR_SIZE               16384 // bytes in a character block, 16 KB
#define BG_SCREEN_SIZE             2048  // bytes in a screen block, 2 KB
// Tiles in a row of a screen block, whose map is 32 tiles square.
#define BG_SCREEN_WIDTH            32
// The bytes of one row of a text background map, 32 entries.
#define MAP_ROW_BYTES              (BG_SCREEN_WIDTH * 2)
// The words of the same row, two entries a word.
#define MAP_ROW_WORDS              (MAP_ROW_BYTES / 4)
// Two screen entries in a word, tiles n and n + 1 side by side, for numbering a map's tiles two at
// a time; adding TILE_PAIR_STEP moves both on by two.
#define TILE_PAIR(n)               ((Uint32)((n) + 1) << 16 | (n))
#define TILE_PAIR_STEP             ((Uint32)2 << 16 | 2)
// A screen block entry's 16-colour palette, in its top four bits.
#define BG_TILE_PALETTE(n)         ((n) << 12)
// The palette a map entry draws its tile with, 0 to 15.
#define BG_TILE_PALETTE_OF(entry)  (((entry) >> 12) & 0xf)
// A word of eight 4-bit pixels, all colour `color`: one row of a 16-colour tile.
#define PIXELS_4BPP(color)         ((color) * 0x11111111u)
// A word of four 8-bit pixels, all colour `color`.
#define PIXELS_8BPP(color)         ((color) * 0x01010101)
#define SIO_MULTI_SI               0x0004 // multiplayer: this console's SI pin, clear on the parent
#define SIO_MULTI_SD               0x0008 // multiplayer: every console is ready
#define SIO_ID                     0x0030 // multiplayer: this console's id, 0 the parent
#define SIO_ID_SHIFT               4      // SIO_ID's lowest bit
// The multiplayer id (SIO_ID) in a SIOCNT value, taken out by shifts.
#define SIO_ID_OF(control)         (((control) << 26) >> 30)
#define SIO_ERROR                  0x0040 // multiplayer: the last transfer failed
// The SIO_ERROR bit of a SIOCNT value, taken out by shifts as SIO_ID_OF takes the id.
#define SIO_ERROR_OF(control)      (((control) << 25) >> 31)
// OAM's size in bytes: 128 sprites of 8 bytes.
#define OAM_SIZE                   1024

// A sprite's mode, OAM attribute 0's bits 11..10.
typedef enum SPR_MODE
{
    OBJ_MODE_NORMAL           = 0,
    OBJ_MODE_SEMI_TRANSPARENT = 1, // blended as BLDCNT's first target
    OBJ_MODE_WINDOW           = 2  // drawn into the OBJ window, not shown
} SPR_MODE;

// A sprite's affine mode, OAM attribute 0's bits 9..8: a plain sprite, one scaled or turned through
// its matrix, or one drawn so in a box twice its size so that a scale above 1 is not cut off.
typedef enum OAM_AFFINE_MODE
{
    OAM_AFFINE_OFF         = 0,
    OAM_AFFINE_ON          = 1,
    OAM_AFFINE_DOUBLE_SIZE = 3
} OAM_AFFINE_MODE;

// A sprite's colours, OAM attribute 0's bit 13: 16 colours from one of the 16 palettes, or 256.
typedef enum OBJ_COL
{
    OBJ_COLORS_16  = 0,
    OBJ_COLORS_256 = 1
} OBJ_COL;

// A sprite's shape, OAM attribute 0's bits 15..14; its size, attribute 1's bits 15..14, gives its
// dimensions with it (GBATEK, "LCD OBJ - OAM Attributes").
typedef enum OBJ_SHAPE
{
    OBJ_SHAPE_SQUARE     = 0,
    OBJ_SHAPE_HORIZONTAL = 1, // wider than tall
    OBJ_SHAPE_VERTICAL   = 2  // taller than wide
} OBJ_SHAPE;

// A sprite's attributes 0 and 1 as one word, attr0 | attr1 << 16, the way functions that build
// their own sprite lists write them; x, 0 to 511, is shifted into the high half and y, 0 to 255, is
// the low byte.
#define OBJ_SIZE_8X8                0x00000000
#define OBJ_SIZE_16X16              0x40000000
#define OBJ_SIZE_32X32              0x80000000
#define OBJ_SIZE_64X64              0xc0000000
#define OBJ_SIZE_16X8               0x00004000
#define OBJ_SIZE_32X8               0x40004000
#define OBJ_SIZE_32X16              0x80004000
#define OBJ_SIZE_64X32              0xc0004000
#define OBJ_SIZE_8X16               0x00008000
#define OBJ_SIZE_16X32              0x80008000
#define OBJ_SIZE_32X64              0xc0008000
#define OBJ_AFFINE                  0x00000100 // drawn through its affine matrix
// An affine sprite drawn in a box twice its size, so turning never clips it.
#define OBJ_DOUBLE_SIZE             0x00000200
// A sprite's x in the attr01 word of an OAM_WORDS entry, whose attribute 1 is its high half.
#define OBJ_X(x)                    ((x) << 16)
#define OBJ_MODE(mode)              ((mode) << 10) // an SPR_MODE
#define OBJ_256_COLORS              0x00002000     // 8bpp tiles, as BGCNT_256_COLORS
#define OBJ_HFLIP                   0x10000000     // a plain sprite mirrored left to right
#define OBJ_VFLIP                   0x20000000     // a plain sprite mirrored top to bottom
// Attribute 2 above its tile number: the sprite's priority, 0 in front to 3 behind, and its
// 16-colour palette.
#define OBJ_PRIORITY(n)             ((n) << 10)
#define OBJ_PALETTE(n)              ((n) << 12)
// The sprite priority bits of attribute 2's high byte, which holds its priority and palette.
#define ATTR2_HIGH_PRIORITY_MASK    (OBJ_PRIORITY(3) >> 8)
// An affine sprite's matrix number, attribute 1's bits 9 to 13, in the attr01 word.
#define OBJ_MATRIX(n)               ((n) << 25)
// An affine parameter in the high half of an OAM_WORDS entry's attr2 word: the entries 4n to 4n + 3
// hold affine matrix n's four.
#define OBJ_AFFINE_PARAMETER(value) ((value) << 16)
// Attribute 1's nine bits of x: a sprite left of the screen is at 512 less its distance.
#define OBJ_X_MASK                  0x1ff
// Attribute 0's eight bits of y: a sprite's y wraps round at 256 lines.
#define OBJ_Y_MASK                  0xff
// A sprite's place in its attr01 word: x in attribute 1's nine bits and y in attribute 0's eight,
// each wrapped as the screen wraps them.
#define OBJ_POSITION(x, y)          (OBJ_X((x) & OBJ_X_MASK) | ((y) & OBJ_Y_MASK))
// Attribute 2's ten bits of tile number, counted in 4-bit tiles from the start of sprite VRAM.
#define OBJ_TILE_MASK               0x3ff
// Requires all ten keys together to prevent the reset chord retriggering it.
#define KEYCNT_DISARMED             0xc3ff

// The event group. Events, their priorities and the event table, with room for 24 events. How many
// events can run at once.
typedef enum EVENT_LIMITS
{
    EVENT_COUNT = 24
} EVENT_LIMITS;

typedef void (*EVENT_FUNC)(void);

// An event: its function's address, its priority (see EVENT_PASS_TICK) and its flags
// (EVENT_FLAG_KEEP_RUNNING).
typedef struct EVENT
{
    Uint32 callback;
    Uint16 priority;
    Uint8  flags;
    Uint8  reserved;
} EVENT;

STATIC_ASSERT(sizeof(EVENT) == 8);
STATIC_ASSERT(offsetof(EVENT, priority) == 4);
STATIC_ASSERT(offsetof(EVENT, flags) == 6);

// An event's priority: the high byte picks the pass that runs it, the low byte ranks it within the
// pass (higher runs first). An odd high byte means the event is suspended, so no pass matches it.
//
// The two passes: the VBlank handler runs EVENT_PASS_VBLANK's events, and sySynch() runs
// EVENT_PASS_TICK's once a tick. Each ranks 0x80, the middle of the low byte, so an event added at
// EVENT_PASS_TICK + 10 runs before those added at EVENT_PASS_TICK.
#define EVENT_PASS_TICK         0x480
#define EVENT_PASS_VBLANK       0xc80
// An empty slot's priority, sorted last: the sort reads priorities signed.
#define EVENT_PRIORITY_EMPTY    -1
// A removed event's priority: its odd high byte matches no pass, and as the highest priority it
// sorts to the front of the table.
#define EVENT_PRIORITY_REMOVED  0x7fff

// An event's priority's high byte: the pass it runs in.
#define EVENT_STATE_HIGH(task)  (((Uint8 *)&(task)->priority)[1])
// The bit of an event's high priority byte (EVENT_STATE_HIGH) that suspends it.
#define EVENT_SUSPENDED         1
// The bit of an event's flags that keeps it running while sySuspendOverlayEvents suspends the
// others.
#define EVENT_FLAG_KEEP_RUNNING 1
// Whether the event's function is in EWRAM, where a scene's overlay runs.
#define EVENT_IN_OVERLAY(task)  (((task)->callback >> 24) == EWRAM_START >> 24)

// The irq group. The interrupt sources, their handler table and the resident IWRAM code the
// interrupts enter through. The hardware's interrupt sources: each one's bit number in IE and IF,
// and its slot in IrqHandlers.
enum
{
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

// The bits of IE and IF for the interrupts the game handles.
#define INTR_FLAG_VBLANK  (1 << IRQ_VBLANK)
#define INTR_FLAG_TIMER3  (1 << IRQ_TIMER3)
#define INTR_FLAG_SERIAL  (1 << IRQ_SERIAL)
#define INTR_FLAG_KEYPAD  (1 << IRQ_KEYPAD)
#define INTR_FLAG_GAMEPAK (1 << IRQ_GAMEPAK)

typedef void (*INTERRUPT_HANDLER)(void);

// Where syInstallIWRAMAndIRQs copies the resident IWRAM code.
#define RamIwramIrqMain FIXED_ADDRESS((Uint8 *), 0x03000100, syIWRAMIRQMain)
// The BIOS's interrupt vector at the top of IWRAM.
#define RamIrqVector    FIXED_ADDRESS((void **), 0x03007ffc, IrqVector)

// The heap group. The heap's slots.
//
// The heap is two stacks of blocks, one in IWRAM and one in EWRAM, as in The Broken Seal; the
// system library README's "The heap" gives the allocators and the lifetime rule. Every scene change
// resets both pools and clears every slot (syHeapInit), so no block outlives the scene, battle or
// minigame that took it. The Lost Age's slot functions (syHeapSlotAllocate,
// syHeapSlotAllocateEWRAM, syHeapSlotRelease) take a slot by its byte offset into HeapSlotsWork, so
// each HEAP_SLOT_ name below is that offset.
struct BATTLE_BACKGROUND_STATE;
struct BATTLE_STATE;
struct BATTLE_MESSAGE_STATE;
struct MAP_STATE;
struct ACTOR;
struct ACTOR_SYSTEM_STATE;
struct SCREEN_TRANSITION_STATE;
struct ROPE_WORK;
struct TIGHTROPE_WORK;

// The heap's 64 words in IWRAM (HeapSlotsWork, 0x03000000): the two pools' next free addresses,
// then each slot's block once allocated, else null.
typedef struct HEAP_SLOTS
{
    void                      *nextEwram;        // the EWRAM pool's next free byte
    void                      *nextIwram;        // the IWRAM pool's next free byte
    Uint8                     *matrixStack;      // syPushMatrix's save area, one matrix deep
    void                      *spriteAnimations; // the actor system's sprite animations
    void                      *sprites;          // the actor system's sprite pool
    struct ACTOR              *actors;           // the 64 actors
    struct ACTOR_SYSTEM_STATE *actorSystem;      // the actor system's state
    void                      *worldMapAffine;   // the world map's affine state
    // The field map's scrolling state, or on the world map its perspective view (WORLD_MAP_VIEW).
    void                      *mapGrid;
    struct BATTLE_STATE       *battleState;     // a battle's state, from btRun on
    void                      *battleScanlines; // the battle background's scanline state
    // Unused: 640 bytes btRun allocates with the battle's other blocks, which nothing reads or
    // writes.
    void                      *battleUnused;
    // The 3D camera (BATTLE_CAMERA) of a battle, the world map or the dice games.
    void                      *cameraState;
    void                      *unusedAfterCamera; // no code allocates or reads this slot
    // A scratch buffer, usually 1 KB, that a picture is decoded into and released as soon as its
    // tiles are in VRAM.
    void                      *tileScratch;
    Uint8                     *windowState; // the window renderer's state (WIN_STATE)
    void                      *partyStatus; // the party status window's layout
    // the buffers an icon or portrait is decoded through
    void                      *glyphTransfer;
    void                      *selectionMenu;            // the selection screen's state
    void                      *unusedAfterSelectionMenu; // no code allocates or reads this slot
    // Lives for one tick: sySynch() builds the OAM list in it, and the field sprite decoder keeps
    // its code there between its own uses.
    void                      *frameScratch;
    // A RAM copy of fdRenderCopyStrided, which uploads field sprite pictures to VRAM.
    void                      *pictureUploader;
    void                      *mindRead; // Mind Read's wave state
    // The running battle effect's own state. The dice games keep their own state in this slot and
    // the next two.
    void                      *battleEffect;
    void                      *battleCanvas; // the running battle effect's pixel canvas
    void                      *battleSheet;  // the running battle effect's tile sheet
    // A function run from RAM: the canvas drawing function btEffectBuildBlitter writes (a
    // DRAW_RECTANGLE), or the world map's copy of fdWorldMapUpdateAffineLines, which the world map
    // calls as its own.
    Sint32                     (*ramCode)(
        void *destination, const void *source, Sint32 x, Sint32 y, Sint32 width, Sint32 height);
    void                           *fieldState;    // a field scene's FIELD_STATE
    void                           *paletteCycles; // the palette cycles
    void                           *weather;    // the running ground particles, motes or sparkles
    void                           *fieldBlend; // the field's colour blend state
    // The screen transition: the window edges and dissolve of a scene change.
    struct SCREEN_TRANSITION_STATE *screenTransition;
    // The palette fade that moves the BG and sprite palettes toward target colours (PALETTE_FADE).
    void                           *paletteFade;
    void                           *unusedAfterPaletteFade; // no code allocates or reads this slot
    void                           *lineScroll;             // the line scroll's state
    void                           *unusedAfterLineScroll;  // no code allocates or reads this slot
    void                           *fadeOverlay;            // the fade overlay's state
    struct BATTLE_MESSAGE_STATE    *battleMsg;              // the battle message window
    void                           *unusedAfterBattleMsg;   // no code allocates or reads this slot
    struct ROPE_WORK               *rope;                   // a rope's state
    struct TIGHTROPE_WORK          *tightrope;              // a tightrope's state
    void                           *hangingVine; // a hanging vine's state (HANGING_VINE_WORK)
    void *battleEntryTimer;                      // the battle's entry transition timer, in ticks
    void *titleIntro;                            // the title intro's state
    // The battle background's view and entry transition (32 bytes), from btRun.
    struct BATTLE_BACKGROUND_STATE *battleBg;
    // The dice games' sprites, or a field Psynergy's canvas map.
    Uint16                         *canvasMap;
    void                           *unusedAfterCanvasMap; // no code allocates this slot
    // A second function as ramCode, released before it: always a btEffectBuildBlitter function, the
    // second of a pair.
    Sint32                          (*ramCodePaired)(
        void *destination, const void *source, Sint32 x, Sint32 y, Sint32 width, Sint32 height);
    void   *unusedAfterRamCodePaired; // no code allocates or reads this slot
    void   *unusedBeforeMsgDecoder;   // no code allocates or reads this slot
    // The message decoder function (TextDecodeSymbol), copied here to run for the length of one
    // message's decoding.
    void   *msgDecoder;
    void   *bupState;                // the save workspace, around each save or load
    void   *settingsMenu;            // the settings menu's state
    void   *unusedAfterSettingsMenu; // no code allocates or reads this slot
    void   *enemyUnits;              // each linked console's party, from btRun
    // The menu state shared by the field menu's screens and the shop; each takes it on opening and
    // releases it on closing.
    void   *menuState;
    void   *fieldEffectState;        // the Psynergy effect's state
    void   *battleMenuState;         // the battle menus' BATTLE_MENU_STATE
    void   *iconMenu;                // a command icon menu's state
    void   *trialRoadLink;           // the Trial Road's link state
    void   *canvasSprite;            // a field Psynergy's canvas sprite
    void   *unusedAfterCanvasSprite; // no code allocates or reads this slot
    void   *arcEffect;               // a field Psynergy's arc state
    // The Djinn the Naribwe fortune teller may hint at, 320 bytes, while she picks one
    // (naribwe_rooms.c).
    Sint32 *djinniHints;
} HEAP_SLOTS;

STATIC_ASSERT(sizeof(HEAP_SLOTS) == 256);
STATIC_ASSERT(offsetof(HEAP_SLOTS, fieldState) == 27 * 4);
STATIC_ASSERT(offsetof(HEAP_SLOTS, menuState) == 55 * 4);

// The heap slots shared across folders, by byte offset into HeapSlotsWork.
typedef enum HEAP_SLOT
{
    HEAP_SLOT_MATRIX_STACK       = offsetof(HEAP_SLOTS, matrixStack),
    HEAP_SLOT_SPRITE_ANIMATIONS  = offsetof(HEAP_SLOTS, spriteAnimations),
    HEAP_SLOT_SPRITES            = offsetof(HEAP_SLOTS, sprites),
    HEAP_SLOT_ACTORS             = offsetof(HEAP_SLOTS, actors),
    HEAP_SLOT_ACTOR_SYSTEM       = offsetof(HEAP_SLOTS, actorSystem),
    HEAP_SLOT_WORLD_MAP_AFFINE   = offsetof(HEAP_SLOTS, worldMapAffine),
    HEAP_SLOT_MAP_GRID           = offsetof(HEAP_SLOTS, mapGrid),
    HEAP_SLOT_BATTLE             = offsetof(HEAP_SLOTS, battleState),
    HEAP_SLOT_BATTLE_SCANLINES   = offsetof(HEAP_SLOTS, battleScanlines),
    HEAP_SLOT_BATTLE_UNUSED      = offsetof(HEAP_SLOTS, battleUnused),
    HEAP_SLOT_CAMERA             = offsetof(HEAP_SLOTS, cameraState),
    HEAP_SLOT_TILE_SCRATCH       = offsetof(HEAP_SLOTS, tileScratch),
    HEAP_SLOT_WINDOW             = offsetof(HEAP_SLOTS, windowState),
    HEAP_SLOT_PARTY_STATUS       = offsetof(HEAP_SLOTS, partyStatus),
    HEAP_SLOT_ICON_TRANSFER      = offsetof(HEAP_SLOTS, glyphTransfer),
    HEAP_SLOT_SELECTION_MENU     = offsetof(HEAP_SLOTS, selectionMenu),
    HEAP_SLOT_TICK_SCRATCH       = offsetof(HEAP_SLOTS, frameScratch),
    HEAP_SLOT_PICTURE_UPLOADER   = offsetof(HEAP_SLOTS, pictureUploader),
    HEAP_SLOT_MIND_READ          = offsetof(HEAP_SLOTS, mindRead),
    HEAP_SLOT_BATTLE_EFFECT      = offsetof(HEAP_SLOTS, battleEffect),
    HEAP_SLOT_BATTLE_CANVAS      = offsetof(HEAP_SLOTS, battleCanvas),
    HEAP_SLOT_BATTLE_SHEET       = offsetof(HEAP_SLOTS, battleSheet),
    HEAP_SLOT_RAM_CODE           = offsetof(HEAP_SLOTS, ramCode),
    HEAP_SLOT_FIELD              = offsetof(HEAP_SLOTS, fieldState),
    HEAP_SLOT_PALETTE_CYCLES     = offsetof(HEAP_SLOTS, paletteCycles),
    HEAP_SLOT_WEATHER            = offsetof(HEAP_SLOTS, weather),
    HEAP_SLOT_FIELD_BLEND        = offsetof(HEAP_SLOTS, fieldBlend),
    HEAP_SLOT_SCREEN_TRANSITION  = offsetof(HEAP_SLOTS, screenTransition),
    HEAP_SLOT_PALETTE_FADE       = offsetof(HEAP_SLOTS, paletteFade),
    HEAP_SLOT_LINE_SCROLL        = offsetof(HEAP_SLOTS, lineScroll),
    HEAP_SLOT_FADE_OVERLAY       = offsetof(HEAP_SLOTS, fadeOverlay),
    HEAP_SLOT_BATTLE_MESSAGE     = offsetof(HEAP_SLOTS, battleMsg),
    HEAP_SLOT_ROPE               = offsetof(HEAP_SLOTS, rope),
    HEAP_SLOT_TIGHTROPE          = offsetof(HEAP_SLOTS, tightrope),
    HEAP_SLOT_HANGING_VINE       = offsetof(HEAP_SLOTS, hangingVine),
    HEAP_SLOT_BATTLE_ENTRY_TIMER = offsetof(HEAP_SLOTS, battleEntryTimer),
    HEAP_SLOT_TITLE_INTRO        = offsetof(HEAP_SLOTS, titleIntro),
    HEAP_SLOT_BATTLE_BACKGROUND  = offsetof(HEAP_SLOTS, battleBg),
    HEAP_SLOT_CANVAS_MAP         = offsetof(HEAP_SLOTS, canvasMap),
    HEAP_SLOT_RAM_CODE_PAIRED    = offsetof(HEAP_SLOTS, ramCodePaired),
    HEAP_SLOT_MESSAGE_DECODER    = offsetof(HEAP_SLOTS, msgDecoder),
    HEAP_SLOT_BUP                = offsetof(HEAP_SLOTS, bupState),
    HEAP_SLOT_SETTINGS_MENU      = offsetof(HEAP_SLOTS, settingsMenu),
    HEAP_SLOT_ENEMY_UNITS        = offsetof(HEAP_SLOTS, enemyUnits),
    HEAP_SLOT_MENU               = offsetof(HEAP_SLOTS, menuState),
    HEAP_SLOT_FIELD_EFFECT       = offsetof(HEAP_SLOTS, fieldEffectState),
    HEAP_SLOT_BATTLE_MENU        = offsetof(HEAP_SLOTS, battleMenuState),
    HEAP_SLOT_ICON_MENU          = offsetof(HEAP_SLOTS, iconMenu),
    HEAP_SLOT_TRIAL_ROAD_LINK    = offsetof(HEAP_SLOTS, trialRoadLink),
    HEAP_SLOT_CANVAS_SPRITE      = offsetof(HEAP_SLOTS, canvasSprite),
    HEAP_SLOT_ARC_EFFECT         = offsetof(HEAP_SLOTS, arcEffect),
    HEAP_SLOT_DJINNI_HINTS       = offsetof(HEAP_SLOTS, djinniHints)
} HEAP_SLOT;

#define RamHeapSlots              FIXED_ADDRESS((HEAP_SLOTS *), 0x03000000, HeapSlotsWork)

// Heap slot `kind` of the heap's words `state`, by its byte offset (enum HEAP_SLOT): the block, or
// null while none is allocated.
#define HEAP_SLOT_AT(state, kind) (*(void **)((Uint8 *)(state) + (kind)))

// The affine group. Sprite rotation and scaling: a sprite's transform and the OAM matrices this
// tick hands out for it. Full size in an AFFINE_TRANSFORM's 8.8 scales.
#define AFFINE_SCALE_ONE          (1 << 8)

// A sprite's rotation and scale: scales in 8.8 fixed point (AFFINE_SCALE_ONE is full size), the
// angle in 1/65536 turns.
typedef struct AFFINE_TRANSFORM
{
    Uint16 scaleX;  // 8.8 fixed point, read signed
    Uint16 scaleY;  // 8.8 fixed point, read signed
    Uint16 angle;   // 1/65536 turns
    Uint16 padding; // never read; keeps the struct 8 bytes
} AFFINE_TRANSFORM;

// One OAM rotation-scaling matrix: pa, pb, pc and pd in 8.8 fixed point, the inverse of the
// transform the sprite is drawn with.
typedef union AFFINE_MATRIX
{
    Sint16 coefficients[4]; // pa, pb, pc and pd
    // pa and pb, then pc and pd, written whole when only the diagonal is set.
    Uint32 rows[2];
} AFFINE_MATRIX;

STATIC_ASSERT(sizeof(AFFINE_TRANSFORM) == 8);
STATIC_ASSERT(offsetof(AFFINE_TRANSFORM, scaleY) == 2);
STATIC_ASSERT(offsetof(AFFINE_TRANSFORM, angle) == 4);
STATIC_ASSERT(sizeof(AFFINE_MATRIX) == 8);

// The bg_scroll group. The four backgrounds' scroll offsets, written to the hardware each VBlank. A
// background's hardware scroll offsets.
typedef struct BG_SCROLL
{
    Uint16 x, y; // pixels
} BG_SCROLL;

// The dma group. DMA and the BIOS copy service: fills, and one transfer on a channel. DMA channel
// registers (DMAxSAD, DMAxDAD, DMAxCNT).
typedef struct DMA_REGISTERS
{
    Uint32 src;
    Uint32 dest;
    Uint32 control;
} DMA_REGISTERS;

// DMA channel 0's control halfword (DMA0CNT_H) among REG_DMA0's halfwords.
#define DMA0_CONTROL 5

// DMA3, the channel the game copies with, as its three registers.
#define DMA3         ((volatile DMA_REGISTERS *)&REG_DMA3SAD)

// The stub group. The system module's stub table (system/stubs.s), as the world map's tilt reads
// it. An eight-byte entry of the system module's stub table holds the ldr/bx instructions and a
// target word.
typedef struct STUB
{
    Uint32 stub;
    Uint32 target;
} STUB;

// A system module table entry whose two-argument target the world map's tilt calls, the arctangent.
typedef struct ARC_TAN_STUB
{
    Uint32 stub;
    Sint32 (*target)(Sint32 x, Sint32 y);
} ARC_TAN_STUB;

// A system module table entry whose one-argument target the world map's tilt calls, the sine or the
// cosine.
typedef struct TRIG_STUB
{
    Uint32 stub;
    Sint32 (*target)(Sint32 angle);
} TRIG_STUB;

// The start of the system module's stub table, as far as syCos.
typedef struct SYSTEM_STUB_TABLE
{
    STUB         preceding[8];
    ARC_TAN_STUB arcTan2;
    STUB         squareRoots[2];
    TRIG_STUB    sin;
    TRIG_STUB    cos;
} SYSTEM_STUB_TABLE;

// The tick group. The tick loop's globals: the frames a tick takes, the CPU load display and the
// VBlank flags, which sySynch() (system/system_frame.c) and the VBlank handler share. Ticks in a
// second: the screen refreshes about 60 times a second.
#define TICKS_PER_SECOND 60

// The input group. The keypad state the VBlank interrupt keeps. The keypad state (PerWork): the
// keys held, newly pressed and repeating, the D-pad direction kept, and the repeat timer.
typedef struct INPUT_STATE
{
    volatile Uint32 data;           // keys down now
    volatile Uint32 push;           // keys that went down this tick
    Uint32          prevVblankData; // held at the previous VBlank
    // Pressed keys, repeating while held: after 19 ticks, then every 6.
    volatile Uint32 repeat;
    // repeat with at most one D-pad direction.
    volatile Uint32 direction;
    // The D-pad direction kept: of several held, the one added last.
    volatile Uint32 prevDirection;
    Uint32          prevData;    // held when the tick loop last read it
    Uint32          pendingPush; // presses since a reader last cleared it
    // Ticks until the held keys repeat; -1 makes them repeat at once.
    volatile Sint32 repeatTimer;
} INPUT_STATE;

STATIC_ASSERT(sizeof(INPUT_STATE) == 36);
STATIC_ASSERT(offsetof(INPUT_STATE, data) == 0);
STATIC_ASSERT(offsetof(INPUT_STATE, push) == 4);
STATIC_ASSERT(offsetof(INPUT_STATE, repeat) == 12);
STATIC_ASSERT(offsetof(INPUT_STATE, repeatTimer) == 32);

// The io_write_queue group. The I/O write queue: register writes held until VBlank. The writes the
// queue holds.
#define IO_WRITE_QUEUE_SIZE 32

// The register writes VBlank carries out once the tick loop has finished a tick, each its value,
// address and control word; syIOWriteQueueFlushPending empties it.
typedef struct IO_WRITE_QUEUE
{
    Uint16 count;
    Uint16 padding;
    Uint32 entries[IO_WRITE_QUEUE_SIZE][3];
} IO_WRITE_QUEUE;

// A control word's bits 16 and 17 give a register write's width and bits 18 and 19 its operation,
// as ResourceExecuteTransfer reads them (system/decompress_tagged_lz.s); a word with no width is a
// DMA.

// Writes a byte.
#define IO_WRITE_BYTE     (1 << 16)
// Writes a halfword.
#define IO_WRITE_HALFWORD (2 << 16)
// Writes a word.
#define IO_WRITE_WORD     (3 << 16)
// ORs the value into the register instead of storing it.
#define IO_WRITE_SET      (4 << 16)
// Clears the value's bits in the register instead of storing it.
#define IO_WRITE_CLEAR    (8 << 16)

// The iwram_call group. The functions resident in IWRAM, called at their fixed addresses: the
// fixed-point multiply and divides, the square root, word fills and copies, the matrix multiply and
// the picture decoder. Fixed addresses of the functions resident in IWRAM.
#define syWordsClearEntry                                                                          \
    FIXED_ADDRESS((Sint32 (*)(void *dest, Sint32 size)), 0x03000258, syIWRAMWordsClear)
#define syWordsFillEntry                                                                           \
    FIXED_ADDRESS((Sint32 (*)(void *dest, Sint32 size, Uint32 value)), 0x03000260, syIWRAMWordsFill)
// (left * right) >> 16 with a 64-bit product: the 16.16 product of two 16.16 numbers.
#define syMulFX FIXED_ADDRESS((Sint32 (*)(Sint32 a, Sint32 b)), 0x0300021c, syIWRAMMulFX)
// value / divisor as a 16.16 number: value times 2^30 / divisor, shifted down 14.
#define syFixedDivEntry                                                                            \
    FIXED_ADDRESS((Sint32 (*)(Sint32 divisor, Sint32 value)), 0x03000230, syIWRAMDivFX)
// The square root of value, rounded down; 0 for a negative value.
#define sySquartEntry FIXED_ADDRESS((Sint32 (*)(Sint32 value)), 0x030002d4, syIWRAMSquart)
// Paints value into destination's 16-pixel rows of bytes wherever a glyph's rows of bits, a
// halfword each, are set, as a glyph's shadow is drawn: twelve rows, fourteen outside the Japanese
// edition.
#define syDecompressBitRuns                                                                        \
    FIXED_ADDRESS((Sint32 (*)(const void *bits, void *destination, Sint32 value)),                 \
                  0x03000314,                                                                      \
                  syIWRAMExpandBitRuns)
// Multiplies the current matrix by matrix.
#define syMultiMatrixEntry                                                                         \
    FIXED_ADDRESS((void (*)(const FIXED *matrix)), 0x030003c8, syIWRAMMultiMatrix)
// Reads all three word-aligned coordinates before writing the result. Input may be an array or a
// record's coordinate prefix; output may overlap it.
#define syVectorTransformEntry                                                                     \
    FIXED_ADDRESS(                                                                                 \
        (Sint32 (*)(const void *vector, Sint32 *result)), 0x03000354, syIWRAMTransformVector)
#define syIntegerDivideEntry                                                                       \
    FIXED_ADDRESS((Sint32 (*)(Sint32 dividend, Sint32 divisor)), 0x03000528, syIWRAMSignedDivide)
#define syIntegerDivideUnsignedEntry                                                               \
    FIXED_ADDRESS((Uint32 (*)(Uint32 dividend, Uint32 divisor)), 0x03000534, syIWRAMUnsignedDivide)
#define syDecompressSpritePictureEntry                                                             \
    FIXED_ADDRESS((void *(*)(const void *src, void *dest)), 0x03000694, syIWRAMDecompress)
#define syWordsCopyEntry                                                                           \
    FIXED_ADDRESS(                                                                                 \
        (Sint32 (*)(void *dest, const void *src, Sint32 size)), 0x03000730, syIWRAMWordsCopy)

// syWordsCopy's offset from syIWRAMIRQMain, for the callers that reach it from the linked IWRAM
// code rather than at its fixed address.
#define syWordsCopyOffset FIXED_OFFSET(1584, syIWRAMWordsCopy, syIWRAMIRQMain)

// syMulFX's offset from syIWRAMIRQMain, for the callers that reach it from the linked IWRAM code
// rather than at its fixed address.
#define syMulFXOffset     FIXED_OFFSET(284, syIWRAMMulFX, syIWRAMIRQMain)

// The matrix group. The current 3D matrix, its one-deep stack, and the unit matrix. A transform's
// twelve 16.16 words: a 3x3 rotation, then the translation's x, y and z.
enum
{
    MATRIX_TRANSLATION_X = 9,
    MATRIX_TRANSLATION_Y = 10,
    MATRIX_TRANSLATION_Z = 11,
    MATRIX_WORDS         = 12
};

// The mul_fx group. syMulFXArm, the 16.16 fixed-point multiply. The reciprocal of `d` as a 32-bit
// fraction, rounded up: the high word of v times it is v / d, for the small v divided here.
#define RECIPROCAL_OF(d) (0xffffffffU / (d) + 1)

// The oam group. How much of OAM the tick's sprites take. How many OAM entries are in use, and the
// most that may be.
typedef struct OAM_USAGE
{
    Uint16 used;  // 32-bit words DMA copies to OAM each tick; 0 when reset
    Uint16 limit; // OAM_SIZE / 4, all of OAM, when reset
} OAM_USAGE;

STATIC_ASSERT(sizeof(OAM_USAGE) == 4);
STATIC_ASSERT(offsetof(OAM_USAGE, limit) == 2);

// The point group. A point in 3D space. A position in three coordinates, each in 16.16 fixed point.
typedef struct POS
{
    FIXED x; // 16.16 fixed point
    FIXED y; // 16.16 fixed point
    FIXED z; // 16.16 fixed point
} POS;

STATIC_ASSERT(sizeof(POS) == 12);
STATIC_ASSERT(offsetof(POS, x) == 0);
STATIC_ASSERT(offsetof(POS, y) == 4);
STATIC_ASSERT(offsetof(POS, z) == 8);

// The projection group. The 3D projection the battle engine and the field engine draw with
// (syPerspective, syConvert3Dto2D). The 3D projection onto the screen: its focal length, near and
// far depths, and the screen point the view centres on.
typedef struct PROJECTION
{
    FIXED  focal;   // 16.16 pixels
    FIXED  near;    // nearest depth drawn, 16.16 pixels
    FIXED  far;     // farthest depth drawn, 16.16 pixels
    Sint32 centerX; // screen pixels
    Sint32 centerY; // screen pixels
} PROJECTION;

// Where the battle engine centres its projection on the screen: across the middle, and 120 pixels
// down, below the screen's middle.
#define BATTLE_PROJECTION_CENTER_X (DISPLAY_WIDTH / 2)
#define BATTLE_PROJECTION_CENTER_Y 120

// The ram_buffer group. EWRAM's and IWRAM's buffers at fixed addresses, named by their address
// where a function reaches them so (FIXED_ADDRESS, with the linked symbol beside it), and the
// scene's own state. EWRAM's buffers at fixed addresses: the map's metatiles, the decode buffer,
// the map's collision shapes, its scene points and the BG tiles.
#define RamMapMetatileBuffer       FIXED_ADDRESS((Uint8 *), 0x02010000, MapGridMetatileBuffer)
#define RamDecodeBuffer            FIXED_ADDRESS((Uint8 *), 0x0201c000, DecodeBuffer)
#define RamMapCollision            FIXED_ADDRESS((Uint8 *), 0x0202c000, MapGridShapes)
#define RamMapScenePoints          FIXED_ADDRESS((Uint8 *), 0x0202e008, MapScenePoints)
#define RamBgTileBuffer            FIXED_ADDRESS((Uint8 *), 0x02038000, BgTileBuffer)
// The metatile tiles table (MetatileTiles), as fdMapGridRenderMetatileTilesRow reads it.
#define syRamMapBlocks             FIXED_ADDRESS((Uint8 *), 0x02020000, MetatileTiles)
#define RamMapShapeGrid            FIXED_ADDRESS((Uint8 *), 0x02024000, MapShapeGrid)

// The end of the IWRAM heap, which the allocators test against, and the bottom of the stacks.
#define RamIwramHeapEnd            FIXED_ADDRESS((Uint8 *), 0x03006fc0, IwramSoundMixWorkspace)

// The tick counter, as the overlays of Airs Rock, Atteka Inlet and Ancient Lemuria read it.
#define RamTickCount               FIXED_ADDRESS((Uint32 *), 0x0300122c, TickCount)

// The map screen's scroll (x, then y, 16.16 pixels), as fdMapScreenUpdateScroll reads it.
#define RamWorldMapScroll          FIXED_ADDRESS((Sint32 *), 0x0202a630, MapScreenScrollX)

// The map screen's state, as its setup (fdMapScreenInit) reaches it.
#define RamWorldMapState           FIXED_ADDRESS((struct WORLD_MAP_STATE *), 0x0202a000, WorldMapStateWork)
// Where fdMapScreenUpdateScroll reads the cursor from (x, then y, 16.16 pixels): the setup points
// it at the state's cursor.
#define RamWorldMapCursor          FIXED_ADDRESS((Sint32 **), 0x0202a62c, MapScreenCursor)
// The cursor's place when the screen opens (x, then y, 16.16 pixels).
#define RamWorldMapCursorStart     FIXED_ADDRESS((Sint32 *), 0x0202a64c, MapScreenCursorStartX)
// Where SceneLocalState keeps, by element, the Djinni gmPartyAddIsaacsParty picked at random from
// The Broken Seal's Djinn the party lacks: its place in its element plus one, or 0 when the party
// has them all. An element's last Djinni stands for it in actor placements and Djinni battles.
#define SCENE_STATE_CHOSEN_DJINN   6480

// The game's blocks the system library's RAM holds, complete here so that each variable globals.h
// declares has its whole type. MainRoutineStateWork, the state a save file keeps: play time, coins,
// the scene and the leader's position, the active party and the options.
typedef struct MAIN_ROUTINE_STATE
{
    // gmBuildStampGetTime's value when the game was last saved; 0 on a new game.
    Uint32 buildStamp;
    Uint32 playTime; // ticks
    Uint8  unused[4];
    // The summons the party can call, bit n for summon n: the first word of the party's
    // DJINN_RECOVERY_TABLE, which gmDjinnGetRecoveryTable returns from here, its next word running
    // over coins.
    Uint32 summonMask;
    Uint32 coins; // at most COIN_MAX (gmPartyAdjustCoins)
    // The party's Djinn recovery list (DJINN_RECOVERY_TABLE from its offers on), read through
    // gmDjinnGetRecoveryTable.
    Uint8  djinnRecovery[292];
    // shop credit, at most COIN_MAX (gmPartyAdjustShopCredit)
    Sint32 shopCredit;
    // shop gifts, 0 to SHOP_GIFTS_MAX (gmPartyAdjustShopGifts)
    Sint8  shopGifts;
    // The weights of the spring game's 15 prizes: three each of 4, 8, 16, 32 and 64 on a new game;
    // Lemuria's spring halves a prize's weight each time it is won.
    Uint8  prizeWeights[15];
    // The icons matched on the Lucky Wheels, a prize each, ended by WHEELS_PRIZES_END, for
    // Contigo's attendant to hand out.
    Sint8  wheelsPrizes[8];
    // For each icon, which of its three prizes it gave last (scContigoSelectLuckyWheelsPrize).
    Sint8  wheelsPrizeCursors[6];
    Uint8  unusedAfterWheelsPrizes[134]; // no code names it
    // The scene the party is in, or bound for once gmTakeExit has chosen an exit.
    Sint16 next;
    Sint16 entrance;     // its entrance, a number of that scene's entrance list
    Uint16 restartScene; // where a defeat returns to without a defeat point
    // where a defeat returns to without a defeat point
    Uint16 restartEntrance;
    // the default scene: the fallback of both destinations
    Sint16 returnScene;
    // the default entrance: the fallback of both destinations
    Sint16 returnEntrance;
    Sint16 area; // the current scene's group; a new group resets its state
    // The after-battle destination, -1 for none: read by gmReturnFromMode when the party was not
    // defeated, before the default scene and entrance.
    Uint16 afterBattleScene;
    Uint16 afterBattleEntrance; // its entrance; -1 for none
    // The defeat destination, -1 for none: read by gmReturnFromMode after a defeat.
    Uint16 defeatScene;
    Uint16 defeatEntrance; // its entrance; -1 for none
    // the battle background chosen for the scene or terrain
    Sint16 battleBg;
    Uint8  unusedAfterBg[2];
    Sint16 mapId; // the current scene's map, a row of MapsWork
    // The leader's position in 16.16 pixels and facing (an angle), set from the entrance taken and
    // noted again each time the field handles a request.
    FIXED  x;
    FIXED  y;
    FIXED  z;
    Sint32 heading;
    Uint16 layer;               // the leader's layer; 0 on taking an entrance
    Sint16 battleMusic;         // the battle music chosen for the encounter
    Sint16 sceneMusic;          // the current scene's field music
    Uint8  movementMode;        // how the leader moves, an LEADER_MOVEMENT_MODE
    Sint32 leaderActorId;       // the leader
    Uint8  party[8];            // the party in order
    Uint8  unusedAfterParty[5]; // no code names it
    Uint8  windowHue;           // the "Window color" setting (option 0)
    Uint8  windowBrightness;    // the "Window brightness" setting (option 1)
    Uint8  unusedAfterBrightness[3];
    Uint8  speech;              // option 3, on or off; 1 on a new game
    Uint8  unusedAfterSpeech;   // 1 on a new game; nothing reads it
    // Option 2, the message speed, 0 to 2: 1 on a new game; some scenes set 2 and restore it after.
    Uint8  msgSpeed;
    Uint8  unusedAfterMsgSpeed[2];
    // Set on saving at the end of the game, marking the file cleared: its data can be sent. Copied
    // to the file's summary as sendFlag.
    Uint8  sendFlag;
    // The buttons for each field command, as KEY_ masks, set on a new game; read with PerWork.push
    // except dashKeys, held.
    Uint16 aKeys;              // examine; KEY_A
    Uint16 bKeys;              // KEY_B; held with aKeys, it makes flight climb
    Uint16 selectKeys;         // the menu; KEY_SELECT
    Uint16 startKeys;          // KEY_START
    Uint16 firstShortcutKeys;  // KEY_L
    Uint16 secondShortcutKeys; // KEY_R
    Uint16 dashKeys;           // KEY_B
    // the music to play on returning, -1 for the scene's own
    Sint16 returnMusic;
    // The shortcuts' assignments, run by firstShortcutKeys and secondShortcutKeys
    // (mnPsynergyMenuSetShortcut: kind << 10 | id).
    Uint16 firstShortcut;
    Uint16 secondShortcut;

    Uint8  perspectiveMap; // set to 1 on the world map
    Uint8  unusedAfterPerspective[5];
    // Option 4, Auto-Sleep, saved from IdleSleepEnabled.
    Uint8  idleSleepEnabled;
    // an BATTLE_TYPE, set back to BATTLE_TYPE_NORMAL at the end
    Uint8  battleType;
    // Hazardous ground: when hazardSteps reaches hazardLimit the party takes hazardDamage and the
    // count starts again.
    Sint16 hazardLimit;
    Sint16 hazardMode;
    Sint16 hazardDamage;
    Uint16 hazardSteps; // reset on entering a new scene group
    // The Djinni battle pending (DJINNI_BATTLE_SCRIPT_FIRST plus its number, flags in bits 15 to
    // 11), or a Mimic's flag with CONDITION_FLAG_SET, resolved on returning to the field.
    Sint16 pendingDjinni;
    // The last battle's result: the enemies defeated, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST.
    Sint16 modeResult;
    Sint32 encounterSteps; // reset on entering a new scene group
    // Cleared with the battle's spoils; no code reads it by name.
    Uint16 pendingItem;
    Sint16 sceneType;       // the current scene's SCENE_TYPE
    Sint16 retreatScene;    // where Retreat goes; the group's first entrance
    Sint16 retreatEntrance; // where Retreat goes; the group's first entrance
    FIXED  avoidDistance;   // distance left of Avoid, 16.16 pixels; 0 when off
    Sint8  avoidByItem;     // Avoid came from an item (FLAG_PSYNERGY_FROM_ITEM)
    Uint8  pausedColorMode; // the paused object's first animation param
    Sint16 haltedActor;     // the object field Psynergy has stopped, -1 for none
    // Nonzero while the leader is sand (fdPsynergySand): ending the pending actions ends Sand too,
    // and the field's push and button handler idles.
    Sint8  sandActive;
    Sint8  cloaked;      // Cloak: the leader is hidden
    Uint8  unusedAfterCloaked[2];
    void  *pausedUpdate; // the paused object's update, restored on resuming
    // The weird answers Felix gives, counted up by fdScriptCountAnswer from 0 on a new game:
    // telling Kraden on Idejima that the Western Sea is not where they go, agreeing with Alhafra's
    // mayor that the party owes him, and others. Past four, as Felix answers Ancient Lemuria's
    // guards' "Do we have your word?", Kraden says "Sometimes, you can say the weirdest things..."
    // to a yes and "Are you INSANE!?" to a no.
    Uint8  weirdAnswers;
    // Nonzero makes actor 8, not leaderActorId, the one scripts control.
    Sint16 ownerOverride;
    Sint32 shipX;                    // the ship's x on the world map, where it is moored
    Sint32 shipZ;                    // the ship's z on the world map
    Sint32 shipFacing;               // the ship's facing on the world map
    Uint8  unusedAfterShipFacing[2]; // no code names it
    // AnyFileCleared as the new game began (gmMainRoutineStateInitDefaults): 1 when a file on the
    // cartridge had been cleared. Nothing reads it back.
    Uint8  clearedFileAtStart;
    Uint8  difficulty;               // the level chosen from a cleared game (Difficulty)
    Uint8  unusedAfterDifficulty[4]; // no code names it
    // The Broken Seal's coins, kept from the data transfer (gmDataTransferApply) until Isaac's
    // party joins and gmPartyAddIsaacsParty adds them.
    Sint32 transferredCoins;
    Uint8  unusedAfterTransferredCoins[44]; // no code names it
    Uint16 forgeItem;                       // the item left at the forge (mnForgeSetItem)
    // Teleport's destination: SCENE_WORLD_MAP once a place is chosen on the map screen
    // (fdMapScreenShow), -1 after cancelling.
    Sint16 teleportScene;
    // The chosen place's flag, which the world map takes as its entrance; -1 after cancelling.
    Sint16 teleportEntrance;
    // An TELEPORT_STATE: the scene the party lands in plays the arrival and clears it.
    Sint8  teleportState;
    // Set while the leader stands on the upper level of the Alhafran Cave, where Sand ends; cleared
    // on entering a scene.
    Sint8  onUpperLevel;
    // The Link Lobby's records (LINK_TALLY): the wins, losses, best streak and current streak of
    // its monster battles, then the same four of its battles against another player.
    Uint16 linkTallies[8];
    Uint8  unusedAfterTallies[4];
    Uint32 startStamp; // gmBuildStampGetTime's value at a new game
    Uint8  unusedAfterStartStamp[4];
} MAIN_ROUTINE_STATE;

STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, coins) == 16);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, next) == 480);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, leaderActorId) == 532);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, battleType) == 587);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, ownerOverride) == 630);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, linkTallies) == 708);
STATIC_ASSERT(sizeof(MAIN_ROUTINE_STATE) == 736);

// A marker sprite's OAM entry, after a word of its own.
typedef struct WORLD_MAP_MARKER
{
    Uint8  unused[4]; // no code names it
    Uint16 y           : 8;
    Uint16 affine      : 2;
    Uint16 objectMode  : 2;
    Uint16 mosaic      : 1;
    Uint16 fullColor   : 1;
    Uint16 shape       : 2;
    Uint16 x           : 9;
    Uint16 affineIndex : 5;
    Uint16 size        : 2;
    Uint16 tile        : 10;
    Uint16 priority    : 2;
    Uint16 palette     : 4;
    Uint8  unusedAfterPalette[2]; // no code names it
} WORLD_MAP_MARKER;

// WorldMapStateWork, the map screen's state while it is open: its resources, the cursor and the
// marker sprites.
typedef struct WORLD_MAP_STATE
{
    Uint16           vramBlock;           // owns the marker tiles
    Uint16           windowSlot;          // the resource slot of the name window's tiles
    FIXED            x;                   // the cursor's screen column, 16.16 pixels
    FIXED            z;                   // the cursor's screen row, 16.16 pixels
    Uint8            unusedAfterZ[6];     // no code names them
    Sint16           shown;               // the WORLD_MAP_PLACE whose name is drawn; -1 for none
    Uint8            unusedAfterShown[4]; // no code names them
    FIXED            speed;               // the cursor's speed, 16.16 pixels per tick
    void            *window;              // the name window
    WORLD_MAP_MARKER markers[129];        // the marker sprites
    Sint32          *cursor;              // the cursor the picture scrolls to
    FIXED            scrollX;             // the picture's scroll, 16.16 pixels
    Sint32           scrollZ;

    // The box of the place name the map screen shows, in pixels.
    struct
    {
        Sint16 x;
        Sint16 y;
        Sint16 width;
        Sint16 height;
    } nameBox;

    // Nonzero draws the whole world (RESOURCE_WORLD_MAP_PICTURE) at the origin rather than about
    // the cursor (fdMapScreenDraw).
    Sint16 centered;
    Uint16 mode; // 0 to look, nonzero to choose a destination
    // Where the nearest-place search looks from in place of the cursor, 16.16 pixels, when nonzero;
    // the field engine only ever clears it.
    FIXED  searchX;
    FIXED  searchZ;
} WORLD_MAP_STATE;

STATIC_ASSERT(offsetof(WORLD_MAP_STATE, window) == 28);
STATIC_ASSERT(offsetof(WORLD_MAP_STATE, cursor) == 1580);
STATIC_ASSERT(offsetof(WORLD_MAP_STATE, mode) == 1602);
STATIC_ASSERT(offsetof(WORLD_MAP_STATE, searchZ) == 1608);

// MainRoutineStateWork.mapScreenFlag under a label of its own, MapScreenChoice: the destination
// last chosen on the map screen. Written through the member, fdMapScreenShow compiles otherwise.
typedef struct WORLD_MAP_CHOICE
{
    Sint16 flags; // the chosen WORLD_MAP_PLACE's flag; -1 when cancelled
} WORLD_MAP_CHOICE;

// The save group. The save in flash: its sectors, parts and summaries, and the save's state. The
// save's place in flash and its limits.
enum
{
    BUP_FLASH_TIMER             = 2, // the timer handed to the SDK's flash library
    BUP_FLASH_IDENTIFY_ATTEMPTS = 8, // tries, a tick apart
    // Flash sectors used; as a sector number, "none" in the save functions.
    BUP_SECTOR_COUNT            = 15,
    BUP_SECTORS_PER_PART        = 3,  // a part fills three sectors in a row
    BUP_SECTOR_SIZE             = 4096,
    BUP_PART_NONE               = 16, // the part id of a free or invalidated sector
    BUP_FILE_COUNT              = 3,  // the save files, parts 0 to 2
    // The part after the files, whose summary is the extra one in the save's state.
    BUP_PART_EXTRA              = 3,
    // The highest counter a write keeps; a copy written past it is rewritten as 1.
    BUP_COUNTER_LIMIT           = 65000,
    // Payload bytes summed into the checksum: a part's three sectors less its header.
    BUP_CHECKSUM_BYTES          = BUP_SECTORS_PER_PART * BUP_SECTOR_SIZE - 16
};

// A save file's summary for the file screen: the leader's name, level and class, the area, play
// time, coins, Djinn and party.
typedef struct BUP_SUMMARY
{
    Uint8  slotHeader[16];   // room for the part's BUP_SECTOR_HEADER
    Uint8  name[12];         // the leader's
    Uint8  level;            // the leader's
    Uint8  classId;          // the leader's
    Uint16 area;             // area name number (fdAreaGetNameIndex)
    Uint32 playTime;         // ticks
    Uint32 coins;
    Sint8  djinn[4];         // the party's Djinn of each element
    Sint8  party[5];         // up to four party members, ended by -1
    Sint8  sendFlag;         // MainRoutineStateWork.sendFlag: the file is cleared
    Sint8  djinnCount;       // the Djinn found: their flags from FLAG_DJINN_FIRST
    Sint8  reunited;         // FLAG_REUNION is set
    Uint8  windowHue;        // MainRoutineStateWork.windowHue
    Uint8  windowBrightness; // MainRoutineStateWork.windowBrightness
    Uint16 buildStamp;       // low half of the build stamp saved with the game
    // The play time saved at the start of the file's next sector (offset 0x10, or 0x110 for the
    // extra summary), filled on reading; a file whose playTime differs from it was not written
    // whole.
    Uint32 secondPartPlayTime;
    Uint32 checksum; // the sum of the 968 words from GameFlagBytes
} BUP_SUMMARY;

// The save's state: which of the BUP_SECTOR_COUNT flash sectors hold a part, their part ids and
// counters, one part's three sectors and the summaries.
typedef struct BUP_WORKSPACE
{
    // 1 for a sector holding the current copy of a part
    Uint8  occupied[BUP_SECTOR_COUNT];
    Uint8  partId[BUP_SECTOR_COUNT]; // each sector's part
    Uint16 count[BUP_SECTOR_COUNT];  // each sector's counter
    // one part's sectors, read from or written to flash
    Uint8  slot[BUP_SECTORS_PER_PART * BUP_SECTOR_SIZE];
    BUP_SUMMARY
    summary[BUP_FILE_COUNT];              // each file's, from parts 0 to 2
    BUP_SUMMARY extra;                    // part 3's summary
    void        (*progress)(Sint32 step); // called between sector writes, if set
    Uint16      sectorCount;              // 0 when a write starts
    // Sectors to write: a part's, and as many again when an old copy is invalidated.
    Uint16      sectorTotal;
} BUP_WORKSPACE;

// The header of a save sector in flash: its signature, the part it holds, its checksum and its
// counter.
typedef struct BUP_SECTOR_HEADER
{
    Uint8  signature[7]; // `CAMELOT`, unterminated
    Uint8  partId;
    Uint16 checksum;
    Uint16 count;
    Uint8  reserved[4]; // no code names it
} BUP_SECTOR_HEADER;

// Where a save image, EWRAM from BupBuffer as a part holds it, keeps the game flags,
// MainRoutineStateWork and CharacterUnits. The Clear Data and the data transferred from The Broken
// Seal come in this layout.
#define BUP_IMAGE_GAME_FLAGS         64
#define BUP_IMAGE_MAIN_ROUTINE_STATE 576
#define BUP_IMAGE_UNITS              1312
// Where The Broken Seal's save image keeps its units (CharacterUnits), 32 bytes before this game's
// BUP_IMAGE_UNITS: the Clear scene copies an image it receives by link from the main routine's
// state on that much further on, its units landing on this game's.
#define BROKEN_SEAL_SAVE_IMAGE_UNITS 1280

STATIC_ASSERT(sizeof(BUP_SUMMARY) == 64);
STATIC_ASSERT(offsetof(BUP_WORKSPACE, count) == 30);
STATIC_ASSERT(offsetof(BUP_WORKSPACE, summary) == 12348);
STATIC_ASSERT(offsetof(BUP_WORKSPACE, extra) == 12540);
STATIC_ASSERT(sizeof(BUP_WORKSPACE) == 12612);
STATIC_ASSERT(sizeof(BUP_SECTOR_HEADER) == 16);

// The serial group. The link cable's runtime: serial packets, registers and transfer state. A link
// cable packet: its packet number, the consoles not yet heard from, its checksum and a 24-byte
// payload.
typedef struct SERIAL_PACKET
{
    // SERIAL_STATE's packet number when the packet was built
    Uint8  packetNumber;
    // The consoles heard from before but not this tick (receivedMask ^ currentMask); a receiver
    // complements it once the checksum is good.
    Uint8  missingMask;
    // The complement of the sum of the packet's first 14 halfwords, taken with this field 0.
    Uint16 checksum;
    Uint8  payload[24];
    Uint8  reservedTail[4]; // past the 14 halfwords sent; never written
} SERIAL_PACKET;

STATIC_ASSERT(sizeof(SERIAL_PACKET) == 32);
STATIC_ASSERT(offsetof(SERIAL_PACKET, packetNumber) == 0);
STATIC_ASSERT(offsetof(SERIAL_PACKET, missingMask) == 1);
STATIC_ASSERT(offsetof(SERIAL_PACKET, checksum) == 2);
STATIC_ASSERT(offsetof(SERIAL_PACKET, payload) == 4);
STATIC_ASSERT(offsetof(SERIAL_PACKET, reservedTail) == 28);

// The serial data registers read as two words or four halfwords; no code names this union.
typedef union SERIAL_DATA_REGISTERS
{
    Uint32 words[2];
    Uint16 halfwords[4];
} SERIAL_DATA_REGISTERS;

// SERIAL_STATE.phase.
typedef enum SERIAL_PHASE
{
    SERIAL_PHASE_SEARCHING = 0, // looking for the partner
    SERIAL_PHASE_CONNECTED = 1
} SERIAL_PHASE;

// SERIAL_STATE.mode: how this console takes part in the link.
typedef enum SERIAL_MODE
{
    SERIAL_MODE_CHILD    = 0,
    // SIO_MULTI_SD alone of SIOCNT's SIO_START and SIO_MULTI_SD: this console armed timer 3 and
    // clocks the transfers.
    SERIAL_MODE_CLOCKING = SIO_MULTI_SD
} SERIAL_MODE;

// The link status bits sySerialExchangePayloads returns.
typedef enum LINK_STAT
{
    LINK_STATUS_HEARD_BOTH    = 3,    // both consoles heard from this tick
    LINK_STATUS_CLOCKING      = 0x80, // this console clocks the transfers
    // The last transfer interrupt found SIOCNT's error bit (SIO_ERROR) set.
    LINK_STATUS_ERROR         = 0x1000,
    // SIOCNT's multiplayer id is 2 or more.
    LINK_STATUS_EXTRA_CONSOLE = 0x2000
} LINK_STAT;

// The bytes of one packet buffer in SERIAL_STATE.storage.
#define SERIAL_PACKET_BYTES 32

// The link cable's runtime: its mode and phase, the consoles heard from, the send position and the
// packet buffers the interrupt handler fills.
typedef struct SERIAL_STATE
{
    // An SERIAL_MODE; SERIAL_MODE_CLOCKING adds LINK_STATUS_CLOCKING to the link status.
    Uint8   mode;
    Uint8   phase;        // an SERIAL_PHASE
    Uint8   receivedMask; // the consoles heard from since the link began
    Uint8   currentMask;  // the consoles heard from this tick
    // Each console's SERIAL_CHANNEL_FLAG bits, set by the transfer interrupt and cleared each tick.
    Uint8   channelFlags[4];
    // Nonzero lets a new packet restart timer 3.
    Uint8   transferEnabled;
    // SIOCNT's error bit (SIO_ERROR), which multiplayer mode sets when a transfer fails, copied by
    // each transfer interrupt; nonzero adds LINK_STATUS_ERROR to the link status and refuses the
    // packets heard meanwhile.
    Uint8   transferError;
    Uint8   reserved0a;
    // The packet number, counted up each tick once connected.
    Uint8   packetNumber;
    Uint8   reserved0c[8];
    // The halfword of sendBuf[1] sent next; -1 between packets, when system_serial.c's
    // SERIAL_IDLE_HALFWORD is sent and the two send buffers swap.
    Sint32  sendIndex;
    // Each console's next halfword in its incomingBuf; -1 after an idle system_serial.c's
    // SERIAL_IDLE_HALFWORD.
    Sint32  receiveIndex[2];
    Uint8   reservedAfterReceiveIndex[8];
    // The packet being built ([0]) and the packet being sent ([1]).
    Uint16 *sendBuf[2];
    // Each console's packet being received; two are used.
    Uint16 *incomingBuf[4];
    // Each console's last complete packet, swapped in by the interrupt.
    Uint16 *readyBuf[4];
    // The packets the tick code checks and copies, swapped with readyBuf each tick.
    Uint16 *pendingBuf[4];
    // The buffers' memory: two 32-byte send buffers, then 96 bytes a console for its incoming,
    // ready and pending packets.
    Uint8   storage[SERIAL_PACKET_BYTES * 8];
} SERIAL_STATE;

STATIC_ASSERT(sizeof(SERIAL_STATE) == 352);
STATIC_ASSERT(offsetof(SERIAL_STATE, mode) == 0);
STATIC_ASSERT(offsetof(SERIAL_STATE, transferEnabled) == 8);
STATIC_ASSERT(offsetof(SERIAL_STATE, sendIndex) == 20);
STATIC_ASSERT(offsetof(SERIAL_STATE, storage) == 96);

// SERIAL_STATE.channelFlags: what the transfer interrupts saw of a console's packets since the last
// tick.
typedef enum SERIAL_CHANNEL_FLAG
{
    SERIAL_CHANNEL_PACKET_COMPLETE = 1, // its fourteenth halfword arrived
    // a transfer interrupt found SIOCNT's error bit (SIO_ERROR) set
    SERIAL_CHANNEL_ERROR           = 2
} SERIAL_CHANNEL_FLAG;

// SERIAL_TRANSFER_STATE.flags: how far this console's receive has got.
typedef enum SERIAL_RECEIVE_STATE
{
    SERIAL_RECEIVE_IDLE   = 0,
    SERIAL_RECEIVE_ACTIVE = 1,
    SERIAL_RECEIVE_DONE   = 2 // the last block taken
} SERIAL_RECEIVE_STATE;

// SERIAL_TRANSFER_STATE.peerFlags: the kind of block this console sends.
typedef enum SERIAL_SEND_STATE
{
    SERIAL_SEND_IDLE = 0,
    SERIAL_SEND_MORE = 1, // a block with more to follow
    SERIAL_SEND_LAST = 2
} SERIAL_SEND_STATE;

// SERIAL_TRANSFER_STATE.active and SerialBlockNumber hold a block number in their low seven bits,
// counting up and wrapping; the receiver's, with bit 7 set, asks the sender to rewind to that block
// (sySerialStepBlockTransfer).
#define SERIAL_BLOCK_NUMBER_MASK 0x7f
#define SERIAL_BLOCK_REWIND      0x80 // bit 7: rewind to the block number
// SERIAL_TRANSFER_STATE.status's bit 7, set when a send or receive starts; the receiver counts its
// blocks in the low bits (sySerialStepBlockTransfer).
#define SERIAL_STATUS_RUNNING    0x80

// A console's block transfer state, exchanged with the partner each VBlank: its block number, its
// status and flags, and a 20-byte block.
typedef struct SERIAL_TRANSFER_STATE
{
    // The number of the block in payload; as receiver, the one it expects with bit 7 set to ask for
    // a resend; 1 when a transfer starts.
    Uint8 active;
    // SERIAL_STATUS_RUNNING when started, plus 1 for a receive; the receiver counts blocks (status
    // + 1, with bit 7) and sets 0 after the last.
    Uint8 status;
    Uint8 flags;       // the receive state, an SERIAL_RECEIVE_STATE
    Uint8 peerFlags;   // the send state, an SERIAL_SEND_STATE
    Uint8 payload[20]; // one block of the data sent
} SERIAL_TRANSFER_STATE;

STATIC_ASSERT(sizeof(SERIAL_TRANSFER_STATE) == 24);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, active) == 0);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, status) == 1);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, flags) == 2);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, peerFlags) == 3);
STATIC_ASSERT(offsetof(SERIAL_TRANSFER_STATE, payload) == 4);

#define SERIAL_RUNTIME (&SerialStateWork)

// The sprite_metadata group. A sprite's animation metadata, its row of SpriteAnimationMetadata. An
// animation's metadata: the sprite's size, scale and offsets, how its pictures are drawn and
// decoded, its pictures and its animation scripts.
typedef struct ANIMATION_METADATA
{
    Uint8   width;  // pixels; 0 marks a row with no sprite
    Uint8   height; // pixels
    // 8.8 fixed point, 256 full size (a Sprite's scale over 256)
    Uint16  scale;
    // The ANIMATION_FACING_KIND: which facing table turns its pictures.
    Uint8   drawKind;
    Uint8   animationCount; // scripts in animation
    Sint8   adjustX;        // pixels the sprite is drawn off its position
    Sint8   adjustY;        // pixels the sprite is drawn off its position
    // Pixels from the actor's feet up to the top of its sprite, read signed: where Mind Read's line
    // starts, and what an actor carried on top of another is raised by.
    Uint8   spriteTop;
    Uint8   footprint; // pixels across the actor stands on, twice its radius
    // How its pictures are packed: 1 type-01, 3 LZ, anything else raw.
    Uint8   pictureCodec;
    void  **pictures;  // null for a sprite whose pictures a cache loads
    Uint8 **animation; // its command scripts, one per animation number
} ANIMATION_METADATA;

STATIC_ASSERT(sizeof(ANIMATION_METADATA) == 20);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, scale) == 2);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, animationCount) == 5);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, spriteTop) == 8);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, footprint) == 9);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, pictureCodec) == 10);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, pictures) == 12);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, animation) == 16);

// Looks up animation metadata using the descriptor ID in the low twelve bits: that row of
// SpriteAnimationMetadata (graphics/sprite/sprites.s), with no bounds check.
ANIMATION_METADATA *syResourceGetMetadataRecord(Uint32 number);
ANIMATION_METADATA *fdResourceGetMetadataRecordStubStub(Uint32 number);

// The vram_cache group. The VRAM cache: numbered entries, each holding a run of 64-byte blocks of
// sprite VRAM for a sprite or a loaded picture. The VRAM cache's sizes: its entries, the 64-byte
// blocks of sprite VRAM it hands out, and the markers of a free block and a free entry.
typedef enum VRAM_BLOCK_LIMITS
{
    VRAM_CACHE_ENTRY_COUNT    = 96,
    VRAM_BLOCK_COUNT          = 512,
    VRAM_BLOCK_BYTES          = 64,
    VRAM_BLOCK_OWNER_FREE     = 255,
    VRAM_CACHE_OFFSET_FREE    = 65535,
    // The largest size an entry has while it holds no blocks: syVRAMCacheReleaseVRAM leaves 1,
    // which keeps the entry taken.
    VRAM_CACHE_SIZE_NO_BLOCKS = 16,
    // the most bytes syVRAMCacheLoad takes at once
    VRAM_CACHE_LOAD_MAX       = 8192
} VRAM_BLOCK_LIMITS;

// A sprite slot of the VRAM cache: the size of its block and where the block starts in sprite VRAM.
typedef struct VRAM_CACHE_ENTRY
{
    Uint16 size;
    Uint16 offset;
} VRAM_CACHE_ENTRY;

STATIC_ASSERT(sizeof(VRAM_CACHE_ENTRY) == 4);
STATIC_ASSERT(offsetof(VRAM_CACHE_ENTRY, offset) == 2);

// The vector3 group. Three coordinates as one value, and their array view. Three coordinates, x, y
// and z: a position, a velocity or a direction.
typedef struct VECTOR3
{
#if FIX_POINTER_OUTSIDE_OBJECT
    union
    {
        struct
        {
            Sint32 x;
            Sint32 y;
            Sint32 z;
        };

        // x, y and z, as syVector3Coordinates returns them
        Sint32 coordinates[3];
    };
#else
    Sint32 x;
    Sint32 y;
    Sint32 z;
#endif
} VECTOR3;

STATIC_ASSERT(sizeof(VECTOR3) == 12);
STATIC_ASSERT(offsetof(VECTOR3, x) == 0);
STATIC_ASSERT(offsetof(VECTOR3, y) == 4);
STATIC_ASSERT(offsetof(VECTOR3, z) == 8);

#endif
