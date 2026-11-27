#ifndef ALCHEMY_GRAPHICS_H
#define ALCHEMY_GRAPHICS_H

/* @file */
/* What the graphics module holds for the rest of the game: fonts, icons, */
/* cursors, the name entry keyboards, the transitions' tables and the ARM */
/* functions that decode and draw pictures. The screen fade's state and the */
/* system library's own decoders belong to the system library */
/* (system/globals.h). */
/* */
/* Many of the functions declared here are handwritten ARM code that the game */
/* never runs where it lies: each loader copies it to RAM first, with */
/* syRamCodeLoad or syRamCodeLoadSlot (system.h) or a DMA into a stack */
/* array, and calls the copy. Such a function is declared as an array of */
/* bytes. A symbol ending in CodeSize is the length in bytes of the code the */
/* loader copies: it is not an object, but an address the linker sets equal */
/* to the length (in the function's assembly file, or ld_script.ld where the */
/* end is the next function), so the loader reads it by casting it to a */
/* number. */

#include "types.h"
#include "edition.h"
#include "stubs.h"
#include "system.h"

/* The map entry a screen-wide picture gives each row's two columns past the */
/* screen's edge, which never show. */
#define OFFSCREEN_TILE 511
/* The colours a screen-wide 256-colour picture resource opens with, before its
 */
/* packed tiles: the publisher's, Camelot's and Golden Sun's logos' and the To
 */
/* Be Continued pictures'. */
#define LOGO_PALETTE_COLORS 224

/* The two page arrow tiles, 4bpp, that mnPageArrowsDraw slides */
/* outward over the border's top edge. */
extern u8 PageArrowTiles[];
/* A one-entry table of sprite tiles: the pointing hand cursor */
/* (PointingHandTiles), returned by mnWindowSprGetPointerTiles. */
extern s32 WindowSprite_PointerTiles[];
/* The compressed item icons, indexed by an item's icon field. */
extern u8 *ItemIcons[];
/* The end of ItemIcons, so mnIconCountItemIcons can count them. */
extern u8 *ItemIconsEnd[];
/* The other compressed icons, among them the Status screen's status icons. */
extern u8 *Icon_MiscIconPointers[];
/* The compressed Psynergy icons, drawn over the shared icon base. */
extern u8 *PsynergyIcons[];
/* The end of PsynergyIcons, for mnIconCountPsynergyIcons. */
extern u8 *PsynergyIconsEnd[];

struct SPR;
struct PALETTE_CYCLE_STATE;
struct MENU_CURSOR;
struct TILE_MASK;

/* HEAP_SLOT_PARTY_STATUS: the party status window's layout (struct */
/* PARTY_STAT_WINDOW), right after WindowStateWork in IWRAM. */
#define PartyStatusState (*(u8 (*)[]) & HeapSlots[HEAP_SLOT_PARTY_STATUS])
/* HEAP_SLOT_SELECTION_STATE: SELECTION_STATE, which only two */
/* unused functions touch. */
#define SelectionStateWork (*(u8 (*)[]) & HeapSlots[HEAP_SLOT_SELECTION_STATE])
/* One row's masks for a pair of bar-pattern tiles: a word XORed into each */
/* tile's row. */
typedef struct TILE_MASK {
	u32 firstTile;
	u32 secondTile;
} TILE_MASK;
/* The six row patterns that mark the battle menus' range icons, colour 5 */
/* over a colour-4 fill, read by mnRangeIconBuildTiles. */
extern const TILE_MASK Menu_BarPatternRows[];
/* The row masks mnBuildPatternTiles XORs into the range icons' tile */
/* pairs (menu_tiles.s). */
extern const TILE_MASK Menu_PatternRowMasks[];
/* One row of a tile XORed with a row's mask: how mnBuildPatternTiles and */
/* mnRangeIconBuildTiles mark each tile of a pair, at all four of their XORs, */
/* as The Lost Age's twins do (window/screens.h). */
static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }
/* The credits' 8x8 one-bit font, ASCII 32 to 127. */
extern const u8 Credits_Font[];
/* The order in which the dissolve sets the 64 pixels of its tile. */
extern const u8 ScreenTransition_DitherTable[];
/* Pixels: how far each line's edge of Reveal's rippling ellipse moves out. */
extern const s8 ScreenTransition_EdgeOffsets[];
/* Pixels: the width of WINDOW_TRANSITION_BARS' bar on each line, by progress.
 */
extern const u8 ScreenTransition_NoiseWidths[];
/* HEAP_SLOT_PALETTE_CYCLES: the queue of palette cycles, which no code in */
/* the game ever starts. */
#define PaletteCycleStateWork                                                  \
	(*(struct PALETTE_CYCLE_STATE **)&HeapSlots[HEAP_SLOT_PALETTE_CYCLES])
/* Two 16-entry colour maps for 4bpp pixels: the first draws colour 1 as */
/* colour 10, the second leaves every colour as it is. */
extern const u16 Graphics_ExpandNibbleTable[];
/* HEAP_SLOT_SPRITES: the actor system's sprite pool, which the */
/* field animation and the sprite viewer walk. */
#define SpritesWork (*(struct SPR **)&HeapSlots[HEAP_SLOT_SPRITES])
/* The right-pointing arrow cursor, an 8x16 sprite of 256 colours, beside */
/* its left-pointing twin. */
extern const u8 Menu_CursorActorTiles[];
/* 224 zeroed bytes right after Render_DecodePicture's code and tables; its */
/* address ends the stretch the field sprites' draw copies to RAM. */
extern const u8 Render_DecodePictureEnd[];
/* The pointing hand cursor, a 16x16 sprite, shared by the battle menus, the */
/* name entry screen and the talk choices. */
extern u8 PointingHandTiles[];
/* Decodes a credits slide's packed picture into 8bpp tiles, adding a palette */
/* offset to each pixel; it relocates its own command table wherever it's */
/* copied. */
extern const u8 Credits_DecodeBackground[];
/* Credits_DecodeBackground with its command table, copied into */
/* HEAP_SLOT_RAM_CODE_TEMP for each slide. */
extern u8 Credits_DecodeBackgroundCodeSize[];
/* Decodes one Huffman-coded symbol of a message for a MESSAGE_READER. */
extern const u8 Text_DecodeSymbol[];
/* Text_DecodeSymbol, which the message decoders copy into */
/* HEAP_SLOT_MESSAGE_DECODER. */
extern u8 Text_DecodeSymbolCodeSize[];
/* mnScanlineWindowBuildEdges, copied to the heap by mnScanlineWindowFillSpans
 */
/* for each frame of a window transition. */
extern u8 mnScanlineWindowBuildEdgesCodeSize[];
/* Interleaves a layout's three decoded cell planes into cell words, for */
/* fdTileMapConvertBuffer. */
extern const u8 Tile_ConvertMapGrid[];
/* Tile_ConvertMapGrid, which fdTileMapConvertBuffer runs from a heap copy. */
extern u8 Tile_ConvertMapGridCodeSize[];
/* Unpacks a compressed icon into a glyph transfer's input buffer. */
extern const u8 Tile_Decompress4bpp[];
/* Tile_Decompress4bpp, which mnIconDecode copies into */
/* HEAP_SLOT_RAM_CODE_TEMP. */
extern u8 Tile_Decompress4bppCodeSize[];
/* Packs an unpacked icon into 4bpp tiles over the layers already drawn */
/* there. */
extern const u8 Tile_ExpandMasked[];
/* Tile_ExpandMasked, which mnIconDecode loads for an icon's upper layers. */
extern u8 Tile_ExpandMaskedCodeSize[];
/* Packs an unpacked icon into 4bpp tiles, overwriting every pixel. */
extern const u8 Tile_ExpandOpaque[];
/* Tile_ExpandOpaque, which mnIconDecode loads for an icon's first layer. */
extern u8 Tile_ExpandOpaqueCodeSize[];
/* The title screen's "Press Start" prompt, byte-LZ compressed, in each */
/* language. */
extern const u8 Title_PromptTiles[];
/* The eight scales, in 1/65536, that mnWindowSprPulse pulses the */
/* settings menu's selected icon through. */
extern const s32 WindowSprite_PulseScales[];
/* The pixel offsets mnTitleBob moves a sprite pair up and down by. */
extern const u8 MenuTitleBobOffsets[];
/* The icon count's digits: 0 to 9, then the tens 1 to 3. */
extern u8 *Icon_DigitPointers[];
/* The three icon bases; base 2 is the one item and Psynergy icons share. */
extern u8 *Icon_BasePointerTable[];
/* The three marks mnItemIconCompose draws over an item icon: 0 on a broken */
/* item, 1 on an equipped one and 2 on a cursed one. */
extern u8 *Icon_MarkPointers[];
/* The command icons' overlays, all 43 entries the same picture. */
extern u8 *Icon_OverlayPointerTable[];
/* The menus' hand cursor and gem, 16x16 4bpp sprites; the hand is loaded */
/* by mnIconCreateHandCursor. */
extern u8 Icon_ResourceTiles[];
/* Pixels: the hop a window's sprite makes, one step a frame. */
extern const s8 WindowSprite_BounceOffsets[];
/* The animated page arrow's pictures, 8x16 sprites of 256 colours, 128 bytes */
/* each. */
extern const u8 WindowSprite_PageArrowTiles[];
/* Pixels: the x and y a window's wobbling sprite is moved by, one step every */
/* other frame. */
extern const s8 WindowSprite_WobbleOffsets[][2];
/* For each of the 16 text colours, the pen the font's light shade takes in */
/* the UI palette. */
extern const u8 Font_ColorPens[];
/* mnGlyphDrawArm, copied to the heap for each character drawn. */
extern u8 mnGlyphDrawCodeSize[];
/* Each main font glyph's width in pixels, from the space (32) on. */
extern const u8 Font_GlyphWidths[];
/* Points a MESSAGE_READER at a message's compressed bits and first */
/* context. */
extern const u8 mnMessageFindArm[];
/* mnMessageFindArm; mnMessageFind frees its copy on return. */
extern u8 mnMessageFindCodeSize[];
/* The party status window's column lines, ended by -1: the Djinn column's */
/* first, then one between members. */
extern const s8 Window_PartyColumnOffsets[];
/* The battle effect module's stub table, declared as a function so that a */
/* call reaches its first stub, btEffectBuildBlitterStub; nothing in this */
/* game calls it by this name. */
s32 btStubsBattleEffect(s32 slot, s32 width_shift, s32 height_shift, s32 flags,
						u32 mode);

#endif
