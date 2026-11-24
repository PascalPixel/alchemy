#ifndef ALCHEMY_GRAPHICS_H
#define ALCHEMY_GRAPHICS_H

/// @file
/// The screen fade's state, and what the graphics bank holds for the rest of
/// the game: fonts, icons, cursors, the name entry keyboards, the transitions'
/// tables and the ARM routines that decode and shade pictures.
///
/// Many of the routines declared here are handwritten ARM code that the game
/// never runs where it lies: each loader copies it to RAM first, with
/// RamCode_Load or RamCode_LoadSlot (system/ram_code.h) or a DMA into a stack
/// array, and calls the copy. Such a routine is declared as an array of
/// bytes. A symbol ending in CodeSize is the length in bytes of the code the
/// loader copies: it is not an object, but an address the linker sets equal
/// to the length (in the routine's assembly file, or ld_script.ld where the
/// end is the next routine), so the loader reads it by casting it to a
/// number.

#include "types.h"
#include "edition.h"
#include "gamepak/far_call.h"
#include "system/heap.h"

/// The map entry a screen-wide picture gives each row's two columns past the
/// screen's edge, which never show.
#define OFFSCREEN_TILE 511
/// The colours a screen-wide 256-colour picture resource opens with, before its
/// packed tiles: the Nintendo, Camelot and Golden Sun logos' and the To Be
/// Continued pictures'.
#define LOGO_PALETTE_COLORS 224

/// Which way the screen fade goes: 0 towards black, 1 towards white.
extern u8 gScreenFadeToWhite;
/// The running fade's length in VBlanks, 0 when no fade is running.
// volatile: the VBlank handler's ScreenFade_Update clears it when the fade
// ends, while ScreenFade_Wait waits on it.
extern volatile u8 gScreenFadeVBlanks;
/// VBlanks until the running fade reaches its target.
// volatile: the VBlank handler's ScreenFade_Update counts it down.
extern volatile u8 gScreenFadeVBlanksLeft;
/// The BLDCNT first-target layers the screen fade covers (the low six bits).
extern u16 gScreenFadeLayers;
/// The brightness the running fade started from, 0 to 16.
extern u8 gScreenFadeStartLevel;
/// The brightness the running fade ends at, 0 to 16.
// volatile: ScreenFade_Start sets it for the VBlank handler's
// ScreenFade_Update.
extern volatile u8 gScreenFadeTargetLevel;
/// The two page arrow tiles, 4bpp, that PageArrows_Draw slides
/// outward over the border's top edge.
extern u8 gPageArrowTiles[];
/// Matrix_BuildLookAt's ARM dot product, seven words it copies onto its
/// stack to run.
extern const u32 RelocatedInnerProduct[];
/// Decompress_MoveToFrontLzCommands through its command loop, which
/// Decompress_MoveToFrontLz copies to the heap.
extern u8 Decompress_MoveToFrontLzCommandsCodeSize[];
/// Decompress_ByteLzArm: how much Decompress_ByteLz allocates for it.
extern u8 Decompress_ByteLzCodeSize[];
/// A one-entry table of sprite tiles: the pointing hand cursor
/// (gPointingHandTiles), returned by WindowSprite_GetPointerTiles.
extern s32 gWindowSprite_PointerTiles[];
/// The compressed item icons, indexed by an item's icon field.
extern u8 *gItemIcons[];
/// The end of gItemIcons, so Icon_CountItemIcons can count them.
extern u8 *gItemIconsEnd[];
/// The other compressed icons, among them the Status screen's status icons.
extern u8 *gIcon_MiscIconPointers[];
/// The compressed Psynergy icons, drawn over the shared icon base.
extern u8 *gPsynergyIcons[];
/// The end of gPsynergyIcons, for Icon_CountPsynergyIcons.
extern u8 *gPsynergyIconsEnd[];

struct Sprite;
struct PaletteCycleState;
struct MenuCursor;
struct TileMask;

// The canvas fades' ARM routines and their sizes: each Canvas_CopyAnd...
// function in system/system_decompress.c copies its routine onto its own
// stack and runs it there.
/// Canvas_BrightenArm, for Canvas_CopyAndBrighten.
extern u8 Canvas_CopyAndBrightenCodeSize[];
/// Canvas_BrightenPartialArm, for
/// Canvas_CopyAndBrightenPartial.
extern u8 Canvas_CopyAndBrightenPartialCodeSize[];
/// Canvas_DarkenArm, for Canvas_CopyAndDarken.
extern u8 Canvas_CopyAndDarkenCodeSize[];
/// Canvas_DarkenPartialArm, for Canvas_CopyAndDarkenPartial.
extern u8 Canvas_CopyAndDarkenPartialCodeSize[];
/// Canvas_HalveArm, for Canvas_CopyAndHalve.
extern u8 Canvas_CopyAndHalveCodeSize[];
/// Canvas_HalveNonzeroArm, for Canvas_CopyAndHalveNonzero.
extern u8 Canvas_CopyAndHalveNonzeroCodeSize[];
/// Canvas_ScaleNonzeroThreeQuartersArm, for
/// Canvas_CopyAndScaleNonzeroThreeQuarters.
extern u8 Canvas_CopyAndScaleNonzeroThreeQuartersCodeSize[];
/// Canvas_ScaleThreeQuartersArm, for
/// Canvas_CopyAndScaleThreeQuarters.
extern u8 Canvas_CopyAndScaleThreeQuartersCodeSize[];
/// Raises each pixel step by an amount, capped at 63, after backing the
/// buffer up.
extern const u8 Canvas_BrightenArm[];
/// Brightens as Canvas_BrightenArm, writing back only groups of four
/// pixels not all at 63 already.
extern const u8 Canvas_BrightenPartialArm[];
/// Lowers each pixel step by an amount, stopping at 0, after backing the
/// buffer up.
extern const u8 Canvas_DarkenArm[];
/// Darkens as Canvas_DarkenArm, writing back only groups of four
/// pixels not all 0 already.
extern const u8 Canvas_DarkenPartialArm[];
/// Halves each pixel step; the battle effect canvas's transfers use it.
extern const u8 Canvas_HalveArm[];
/// Halves each pixel step, skipping all-zero groups; Lucky Wheels' canvas uses
/// it.
extern const u8 Canvas_HalveNonzeroArm[];
/// Scales each pixel step to three quarters, skipping all-zero groups.
extern const u8 Canvas_ScaleNonzeroThreeQuartersArm[];
/// Scales each pixel step to three quarters.
extern const u8 Canvas_ScaleThreeQuartersArm[];
/// The 256 depth slots' list heads that every drawn sprite is linked into
/// (Oam_AddSpriteAtDepth); Oam_ResetFrame empties them each frame.
extern void *gOamDepthLists[];
/// HEAP_SLOT_PARTY_STATUS: the party status window's layout (struct
/// PartyStatusWindow), right after gWindowState in IWRAM.
#define gPartyStatusState (*(u8 (*)[]) & gHeapSlots[HEAP_SLOT_PARTY_STATUS])
/// HEAP_SLOT_SELECTION_STATE: struct SelectionState, which only two
/// unused functions touch.
#define gSelectionState (*(u8 (*)[]) & gHeapSlots[HEAP_SLOT_SELECTION_STATE])
/// One row's masks for a pair of bar-pattern tiles: a word XORed into each
/// tile's row.
struct TileMask {
  u32 firstTile;
  u32 secondTile;
};
/// The six row patterns that mark the battle menus' range icons, colour 5
/// over a colour-4 fill, read by RangeIcon_BuildTiles.
extern const struct TileMask gMenu_BarPatternRows[];
/// The row masks Menu_BuildPatternTiles XORs into the range icons' tile
/// pairs (menu_strings.s).
extern const struct TileMask gMenu_PatternRowMasks[];
/// One row of a tile XORed with a row's mask: how Menu_BuildPatternTiles and
/// RangeIcon_BuildTiles mark each tile of a pair.
static __inline__ u32 XorWord(u32 word, u32 mask) {
  /* FAKEMATCH(wrapper): the XOR through this inline; plainly, row ^= mask. */
  return word ^ mask;
}
/// The credits' 8x8 one-bit font, ASCII 32 to 127.
extern const u8 gCredits_Font[];
/// The order in which the dissolve sets the 64 pixels of its tile.
extern const u8 gScreenTransition_DitherTable[];
/// Pixels: how far each line's edge of Reveal's rippling ellipse moves out.
extern const s8 gScreenTransition_EdgeOffsets[];
/// Pixels: the width of WINDOW_TRANSITION_BARS' bar on each line, by progress.
extern const u8 gScreenTransition_NoiseWidths[];
/// The resource bank's far-call stubs, listed in the resource directory as
/// both ResourceFarCalls and ResourceFarCallsAlternate.
extern const u8 FarCall_ResourceTable[];
/// The colour index Render_DecodePicture paints every pixel of a layer drawn
/// with SPRITE_COLOR_FILL; the sprite viewer sets it to 3.
extern u8 gDecodeFillByte;
/// HEAP_SLOT_PALETTE_CYCLES: the queue of palette cycles, which no code in
/// the game ever starts.
#define gPaletteCycleState                                                     \
  (*(struct PaletteCycleState **)&gHeapSlots[HEAP_SLOT_PALETTE_CYCLES])
/// Two 16-entry colour maps for 4bpp pixels: the first draws colour 1 as
/// colour 10, the second leaves every colour as it is.
extern const u16 gGraphics_ExpandNibbleTable[];
/// HEAP_SLOT_SPRITES: the actor system's sprite pool, which the
/// field animation and the sprite viewer walk.
#define gSprites (*(struct Sprite **)&gHeapSlots[HEAP_SLOT_SPRITES])
/// The right-pointing arrow cursor, an 8x16 sprite of 256 colours, beside
/// its left-pointing twin.
extern const u8 gMenu_CursorActorTiles[];
/// 224 zeroed bytes right after Render_DecodePicture's code and tables; its
/// address ends the stretch the field sprites' draw copies to RAM.
extern const u8 gRender_DecodePictureEnd[];
/// The move-to-front LZ decoder's ARM entry: it returns 4 unless the
/// stream's first byte is 2, the move-to-front tag.
extern const u8 Decompress_MoveToFrontLzCommands[];
/// The byte-oriented LZ decoder, run by Decompress_ByteLz from a heap copy.
extern const u8 Decompress_ByteLzArm[];
/// The halfword LZ decoder that Decompress_Halfwords runs from its stack.
extern const u8 Decompress_HalfwordLz[];
/// The tagged LZ decoder (a first byte of 0 for the general form, 1 for the
/// palette form), run by Decompress_TaggedLz from a heap copy.
extern const u8 Decompress_TaggedLzArm[];
/// Decompress_TaggedLzArm, which Decompress_TaggedLz copies to the heap.
extern u8 Decompress_TaggedLzCodeSize[];
/// Decompress_HalfwordLz, copied onto the stack by Decompress_Halfwords.
extern u8 Decompress_HalfwordsCodeSize[];
/// The pointing hand cursor, a 16x16 sprite, shared by the battle menus, the
/// name entry screen and the talk choices.
extern u8 gPointingHandTiles[];
/// Decodes a credits slide's packed picture into 8bpp tiles, adding a palette
/// offset to each pixel; it relocates its own command table wherever it's
/// copied.
extern const u8 Credits_DecodeBackground[];
/// Credits_DecodeBackground with its command table, copied into
/// HEAP_SLOT_RAM_CODE_TEMP for each slide.
extern u8 Credits_DecodeBackgroundCodeSize[];
/// Decodes one Huffman-coded symbol of a message for a struct MessageReader.
extern const u8 Text_DecodeSymbol[];
/// Text_DecodeSymbol, which the message decoders copy into
/// HEAP_SLOT_MESSAGE_DECODER.
extern u8 Text_DecodeSymbolCodeSize[];
/// ScanlineWindow_BuildEdges, copied to the heap by ScanlineWindow_FillSpans
/// for each frame of a window transition.
extern u8 ScanlineWindow_BuildEdgesCodeSize[];
/// Interleaves a layout's three decoded cell planes into cell words, for
/// TileMap_ConvertBuffer.
extern const u8 Tile_ConvertMapGrid[];
/// Tile_ConvertMapGrid, which TileMap_ConvertBuffer runs from a heap copy.
extern u8 Tile_ConvertMapGridCodeSize[];
/// Unpacks a compressed icon into a glyph transfer's input buffer.
extern const u8 Tile_Decompress4bpp[];
/// Tile_Decompress4bpp, which Icon_Decode copies into
/// HEAP_SLOT_RAM_CODE_TEMP.
extern u8 Tile_Decompress4bppCodeSize[];
/// Packs an unpacked icon into 4bpp tiles over the layers already drawn
/// there.
extern const u8 Tile_ExpandMasked[];
/// Tile_ExpandMasked, which Icon_Decode loads for an icon's upper layers.
extern u8 Tile_ExpandMaskedCodeSize[];
/// Packs an unpacked icon into 4bpp tiles, overwriting every pixel.
extern const u8 Tile_ExpandOpaque[];
/// Tile_ExpandOpaque, which Icon_Decode loads for an icon's first layer.
extern u8 Tile_ExpandOpaqueCodeSize[];
/// The title screen's "Press Start" prompt, byte-LZ compressed, in each
/// language.
extern const u8 gTitle_PromptTiles[];
/// The eight scales, in 1/65536, that WindowSprite_Pulse pulses the
/// settings menu's selected icon through.
extern const s32 gWindowSprite_PulseScales[];
/// The pixel offsets MenuTitle_Bob moves a sprite pair up and down by.
extern const u8 gMenuTitleBobOffsets[];
/// The icon count's digits: 0 to 9, then the tens 1 to 3.
extern u8 *gIcon_DigitPointers[];
/// The three icon bases; base 2 is the one item and Psynergy icons share.
extern u8 *gIcon_BasePointerTable[];
/// The three marks ItemIcon_Compose draws over an item icon: 0 on a broken
/// item, 1 on an equipped one and 2 on a cursed one.
extern u8 *gIcon_MarkPointers[];
/// The command icons' overlays, all 43 entries the same picture.
extern u8 *gIcon_OverlayPointerTable[];
/// The menus' hand cursor and gem, 16x16 4bpp sprites; the hand is loaded
/// by Icon_CreateHandCursor.
extern u8 gIcon_ResourceTiles[];
/// Pixels: the hop a window's sprite makes, one step a frame.
extern const s8 gWindowSprite_BounceOffsets[];
/// The animated page arrow's pictures, 8x16 sprites of 256 colours, 128 bytes
/// each.
extern const u8 gWindowSprite_PageArrowTiles[];
/// Pixels: the x and y a window's wobbling sprite is moved by, one step every
/// other frame.
extern const s8 gWindowSprite_WobbleOffsets[][2];
/// For each of the 16 text colours, the pen the font's light shade takes in
/// the UI palette.
extern const u8 gFont_ColorPens[];
/// Glyph_DrawArm, copied to the heap for each character drawn.
extern u8 Glyph_DrawCodeSize[];
/// Each main font glyph's width in pixels, from the space (32) on.
extern const u8 gFont_GlyphWidths[];
/// Points a struct MessageReader at a message's compressed bits and first
/// context.
extern const u8 Message_FindArm[];
/// Message_FindArm; Message_Find frees its copy on return.
extern u8 Message_FindCodeSize[];
/// The party status window's column lines, ended by -1: the Djinn column's
/// first, then one between members.
extern const s8 gWindow_PartyColumnOffsets[];
/// The battle effect bank's far-call table, declared as a function so that a
/// call reaches its first stub, BattleEffect_BuildBlitterFar; nothing in this
/// game calls it by this name.
s32 FarCall_BattleEffectTable(s32 slot, s32 width_shift, s32 height_shift,
                              s32 flags, u32 mode);

#endif
