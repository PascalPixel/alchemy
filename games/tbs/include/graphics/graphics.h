#ifndef GRAPHICS_H
#define GRAPHICS_H

/// @file
/// The screen blend's globals and the graphics routines.
///
/// Many of the routines declared here are handwritten ARM code that the game
/// never runs where it lies: each loader copies it to RAM first, with
/// RamCode_Load or RamCode_LoadSlot (system/ramcode.h) or a DMA into a stack
/// array, and calls the copy. Such a routine is declared as an array of
/// bytes. A symbol ending in CodeSize is the length in bytes of the code the
/// loader copies: it is not an object, but an address the linker sets equal
/// to the length (in the routine's assembly file, or constants.ld where the
/// end is the next routine), so the loader reads it by casting it to a
/// number.

#include "types.h"
#include "edition.h"
#include "farcall.h"

struct Window;

/// Which way the screen fade goes: 0 towards black, 1 towards white.
extern u8 gScreenFadeToWhite;
/* FAKEMATCH(volatile): the fades load the duration back after storing it; as
 * plain u8, 13 of 29 instructions differ in tbs-en (ScreenFade_Start). */
extern volatile u8
    gScreenFadeVBlanks; ///< the fade's length in VBlanks, 0 when idle
/// VBlanks until the running fade reaches its target; ScreenFade_Update counts
/// it down.
extern u8 gScreenFadeVBlanksLeft;
/// The BLDCNT first-target layers the screen fade covers (the low six bits).
extern u16 gScreenFadeLayers;
/// The brightness the running fade started from, 0 to 16.
extern u8 gScreenFadeStartLevel;
/* FAKEMATCH(volatile): ScreenFade_Update loads the target twice, for the level
 * and for the delta; as plain u8, 11 of 46 instructions differ in tbs-en. */
extern volatile u8 gScreenFadeTargetLevel; ///< the brightness the fade ends at
/// The two page arrow tiles, 4bpp, that Link_DrawPageArrows slides
/// outward over the window frame's top edge.
extern u8 gPageArrowTiles[];
/// Transform_BuildLookAt's ARM dot product, seven words it copies onto its
/// stack to run.
extern const u32 RelocatedQ16DotProduct[];
/// Decompress_MoveToFrontLzCommands through its command loop, which
/// Decompress_MoveToFrontLz copies to the heap.
extern u8 Decompress_MoveToFrontLzCommandsCodeSize[];
/// Decompress_ByteLzArm: how much Decompress_ByteLz allocates for it.
extern u8 Decompress_ByteLzCodeSize[];
/// The Japanese status panel's PP row, "EP    /": label and slash in one
/// string, drawn by the unit summary in menu/menu/shared.c.
extern const u8 gStatusPanel_PpRowString[];
/// A one-entry table of sprite tiles: the pointing hand cursor
/// (gPointingHandTiles), returned by WindowSprite_GetPointerTiles.
extern s32 gWindowSprite_PointerTiles[];
/// The Japanese status panel's HP row, "HP    /", the slash already in its
/// cell at 48.
extern const u8 gStatusPanel_HpRowString[];
/// The HP label outside Japan, in each language's form ("KP" in German, "PV"
/// in Spanish and French, "PS" in Italian).
extern const u8 gStatusPanel_HpString[];
/// The PP label outside Japan, which every language writes "PP".
extern const u8 gStatusPanel_PpString[];
/// The slash between a current and maximum figure on the status panel,
/// outside Japan.
extern const u8 gStatusPanel_SlashString[];
/// The compressed item icons, indexed by an item's icon field.
extern u8 *gItemIcons[];
/// The end of gItemIcons, so Icon_CountItemIcons can count
/// them.
extern u8 *gItemIconsEnd[];
/// The other compressed icons, among them the Status screen's status
/// icons.
extern u8 *gIcon_MiscIconPointers[];
/// The compressed Psynergy icons, drawn over the shared icon base.
extern u8 *gPsynergyIcons[];
/// The end of gPsynergyIcons, for Icon_CountPsynergyIcons.
extern u8 *gPsynergyIconsEnd[];

struct Sprite;
struct ArticleTable;
struct Unit;
struct EffectLaunch;
struct GlyphPalette;
struct NumberRow;
struct PaletteCycleWork;
struct GlyphRect;
struct WindowSprite;
struct MenuCursor;
struct TileMask;
struct TransitionShape;
struct UiBobPairOutput;
struct Message;
struct UiScaleSprite;
struct Window;
struct Vec2;
struct OptionMenuWork;

// The pixel shading kernels and their sizes: each
// PaletteFade_BackupAnd... function in runtime/decompress.c copies its
// kernel onto its own stack and runs it there.
/// PaletteFade_BrightenKernel, for PaletteFade_BackupAndBrighten.
extern u8 PaletteFade_BackupAndBrightenCodeSize[];
/// PaletteFade_BrightenPartialKernel, for
/// PaletteFade_BackupAndBrightenPartial.
extern u8 PaletteFade_BackupAndBrightenPartialCodeSize[];
/// PaletteFade_DarkenKernel, for PaletteFade_BackupAndDarken.
extern u8 PaletteFade_BackupAndDarkenCodeSize[];
/// PaletteFade_DarkenPartialKernel, for PaletteFade_BackupAndDarkenPartial.
extern u8 PaletteFade_BackupAndDarkenPartialCodeSize[];
/// PaletteFade_HalveKernel, for PaletteFade_BackupAndHalve.
extern u8 PaletteFade_BackupAndHalveCodeSize[];
/// PaletteFade_HalveNonzeroKernel, for PaletteFade_BackupAndHalveNonzero.
extern u8 PaletteFade_BackupAndHalveNonzeroCodeSize[];
/// PaletteFade_ScaleNonzeroThreeQuartersKernel, for
/// PaletteFade_BackupAndScaleNonzeroThreeQuarters.
extern u8 PaletteFade_BackupAndScaleNonzeroThreeQuartersCodeSize[];
/// PaletteFade_ScaleThreeQuartersKernel, for
/// PaletteFade_BackupAndScaleThreeQuarters.
extern u8 PaletteFade_BackupAndScaleThreeQuartersCodeSize[];
/// Raises each pixel step by an amount, capped at 63, after backing the
/// buffer up.
extern const u8 PaletteFade_BrightenKernel[];
/// Brightens as PaletteFade_BrightenKernel, writing back only groups of four
/// pixels not all at 63 already.
extern const u8 PaletteFade_BrightenPartialKernel[];
/// Lowers each pixel step by an amount, stopping at 0, after backing the
/// buffer up.
extern const u8 PaletteFade_DarkenKernel[];
/// Darkens as PaletteFade_DarkenKernel, writing back only groups of four
/// pixels not all 0 already.
extern const u8 PaletteFade_DarkenPartialKernel[];
/// Halves each pixel step; the battle animation canvas's transfers use it.
extern const u8 PaletteFade_HalveKernel[];
/// Halves each pixel step, skipping all-zero groups; Lucky Wheels' canvas uses
/// it.
extern const u8 PaletteFade_HalveNonzeroKernel[];
/// Scales each pixel step to three quarters, skipping all-zero groups.
extern const u8 PaletteFade_ScaleNonzeroThreeQuartersKernel[];
/// Scales each pixel step to three quarters.
extern const u8 PaletteFade_ScaleThreeQuartersKernel[];
#ifndef OWN_gOamDepthLists
/// The 256 depth slots' list heads that every drawn sprite is linked into
/// (Oam_AddSpriteAtDepth); Oam_ResetFrame empties them each frame.
extern void *gOamDepthLists[];
#endif
/// HEAP_SLOT_PARTY_STATUS: the party status window's layout (struct
/// PartyStatusWindow), right after gWindowWork in IWRAM.
extern u8 gPartyStatusWork[];
/// HEAP_SLOT_SELECTION_WORKSPACE: struct SelectionWorkspace, which only two
/// unused functions touch.
extern u8 gSelectionWorkspace[];
extern const struct ArticleTable gArticle_Table;
/// One row's masks for a pair of bar-pattern tiles: a word XORed into each
/// tile's row.
struct TileMask {
  u32 firstTile;
  u32 secondTile;
};
/// The six row patterns that mark the battle menus' range icons, colour 5
/// over a colour-4 fill, read by Menu_BuildLocalizedPatternTiles.
extern const struct TileMask gMenu_BarPatternRows[];
/// The credits' 8x8 one-bit font, ASCII 0x20 to 0x7f.
extern const u8 gCredits_Font[];
extern const u8 gScreenTransition_DitherTable[];
extern s8 gScreenTransition_EdgeOffsets[];
extern u8 gScreenTransition_NoiseWidths[];
/// The resource bank's far-call stubs, listed in the resource directory as
/// both ResourceFarCalls and ResourceFarCallsAlternate.
extern const u8 FarCall_ResourceTable[];
/// The colour index Render_DecodePicture paints every pixel of a layer drawn
/// with SPRITE_COLOR_FILL; the sprite viewer sets it to 3.
extern u8 gDecodeFillByte;
/// HEAP_SLOT_PALETTE_CYCLES: the queue of palette cycles, which no code in
/// the game ever starts.
extern struct PaletteCycleWork *gPaletteCycleWork;
/// Two 16-entry colour maps for 4bpp pixels: the first draws colour 1 as
/// colour 10, the second leaves every colour as it is.
extern u16 gGraphics_ExpandNibbleTable[];
/// HEAP_SLOT_SPRITES: the actor system's sprite pool, which the
/// field animation and the sprite viewer walk.
extern struct Sprite *gSprites;
#ifndef OWN_Menu_CursorActorTiles
/// The right-pointing arrow cursor, an 8x16 sprite of 256 colours, beside
/// its left-pointing twin.
extern const u8 gMenu_CursorActorTiles[];
#endif
/// The Japanese name entry screen's second keyboard, which Select switches to.
extern const u8 gNameEntry_AlternateKeyboardTilemap[];
/// Eight offsets, 0 to 3 and back, that make the name entry screen's caret
/// circle: the x offset leads the y offset by five steps.
extern s8 gNameEntry_CaretWave[];
/// The kana that take the voiced mark, as first and last pairs ended by 0.
extern const u8 gNameEntry_VoicedMarkRanges[];
/// The Delete key's label on the German, Spanish and French name entry screen
/// keyboards ("ENT", "SUP", "EFF").
extern u8 gNameEntry_DeleteKeyLabel[];
/// The End key's label on the German, Spanish and French keyboards ("ENDE",
/// "FIN", "FIN").
extern u8 gNameEntry_EndKeyLabel[];
/// The kana that take the semi-voiced mark, in the same pairs.
extern const u8 gNameEntry_SemivoicedMarkRanges[];
/// The name entry screen keyboard's compressed tilemap, the Spanish edition's
/// with its own letters.
extern const u8 gNameEntry_KeyboardTilemap[];
/// 224 zeroed bytes right after Render_DecodePicture's code and tables; its
/// address ends the stretch the field sprites' draw copies to RAM.
extern const u8 gRender_DecodePictureEnd[];
/// The move-to-front LZ decoder's ARM entry: it returns 4 unless the
/// stream's first byte is 2, the move-to-front tag.
extern const u8 Decompress_MoveToFrontLzCommands[];
/// The byte-oriented LZ decoder, run by Decompress_ByteLz from a heap
/// copy.
extern const u8 Decompress_ByteLzArm[];
/// The halfword LZ decoder that Decompress_Halfwords runs from its
/// stack.
extern const u8 Decompress_HalfwordLz[];
/// The tagged LZ decoder (a first byte of 0 for the general form, 1 for the
/// palette form), run by Decompress_TaggedLz from a heap
/// copy.
extern const u8 Decompress_TaggedLzArm[];
/// Decompress_TaggedLzArm, which Decompress_TaggedLz copies to the heap.
extern u8 Decompress_TaggedLzCodeSize[];
/// Decompress_HalfwordLz, copied onto the stack by
/// Decompress_Halfwords.
extern u8 Decompress_HalfwordsCodeSize[];
/// The pointing hand cursor, a 16x16 sprite, shared by the battle menus, the
/// name entry screen and the talk choices.
extern u8 gPointingHandTiles[];
/// The 16x16 sprite the name entry screen shows as its name caret.
extern const u8 gNameEntry_CaretTiles[];
/// Decodes a credits slide's packed picture into 8bpp tiles, adding a palette
/// offset to each pixel; it relocates its own command table wherever it's
/// copied.
extern const u8 Credits_DecodeBackground[];
/// Credits_DecodeBackground with its command table, copied into
/// HEAP_SLOT_RAM_CODE_TEMP for each slide.
extern u8 Credits_DecodeBackgroundCodeSize[];
/// Decodes one Huffman-coded symbol of a message for a struct MessageReader.
extern const u8 Text_DecodeSymbol[];
/// Text_DecodeSymbol, which the message decoders copy into heap slot
/// TEXT_WORK_BLOCK.
extern u8 Text_DecodeSymbolCodeSize[];
/// Tile_BuildCellPictures, copied to the heap by ScanlineWindow_FillSpans for
/// each frame of a window transition.
extern u8 Tile_BuildCellPicturesCodeSize[];
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
/// The Title's "Press Start" prompt, byte-LZ compressed, in each
/// language.
extern const u8 gTitle_PromptTiles[];
/// The eight scales, in 1/65536, that WindowSprite_Pulse pulses the
/// settings menu's selected icon through.
extern s32 gWindowSprite_PulseScales[];
/// The pixel offsets MenuTitle_Bob moves a sprite pair up and down
/// by.
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
/// by Icon_CreateWithResourceVariant.
extern u8 gIcon_ResourceTiles[];
extern const s8 gWindowSprite_BounceOffsets[];
/// The animated page arrow's pictures, 8x16 sprites of 256 colours, 128 bytes
/// each.
extern const u8 gWindowSprite_PageArrowTiles[];
extern const s8 gWindowSprite_WobbleOffsets[][2];
/// For each of the 16 text colours, the pen the font's light shade takes in
/// the UI palette.
extern const u8 gFont_ColorPens[];
/// Glyph_DrawArm, copied to the heap for each character drawn.
extern u8 Glyph_DrawCodeSize[];
/// Each main font glyph's width in pixels, from the space (0x20) on.
extern const u8 gFont_GlyphWidths[];
extern const struct ArticleTable gArticle_Indefinite;
/// Points a struct MessageReader at a message's compressed bits and first
/// context.
extern const u8 Message_FindArm[];
/// Message_FindArm; Message_Find frees its copy on return.
extern u8 Message_FindCodeSize[];
/// The party status window's column lines, ended by -1: the Djinn column's
/// first, then one between members.
extern const s8 gWindow_PartyColumnOffsets[];
#ifndef OWN_FarCall_BattleAnimTable
/// The battle animation bank's far-call table, declared as a function so that a
/// call reaches its first stub, BattleAnim_BuildBlitterFar; nothing in this
/// game calls it by this name.
s32 FarCall_BattleAnimTable(s32 slot, s32 width_shift, s32 height_shift,
                            s32 flags, u32 mode);
#endif

#endif
