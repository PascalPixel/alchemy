#ifndef ALCHEMY_GRAPHICS_H
#define ALCHEMY_GRAPHICS_H

/// @file
/// What the window, menu, battle and credits code share: the screen fade's
/// globals, the decoders and other code run from RAM, text and numbers, icons,
/// windows and their sprites, the party status window, the credits' scrolling
/// text, and the far calls into the battle bank that the menus make.
///
/// Several routines declared here are handwritten ARM code that the game copies
/// to RAM before it runs it (RamCode_Load, system/ram_code.h). Such a routine
/// is declared as an array of bytes, and a symbol ending in CodeSize is the
/// length of the copy: no object, but an address the linker sets equal to the
/// length, which the loader reads by casting it to a number.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "edition.h"
#include "system/projection.h"

struct Window;

/// Which way the screen fade goes: 0 towards black, 1 towards white.
extern u8 gScreenFadeToWhite;
// volatile: ScreenFade_Update, which the VBlank handler runs, clears it at the
// fade's end while ScreenFade_Wait waits on it, so it is no device.
extern volatile u8
    gScreenFadeVBlanks; ///< the fade's length in VBlanks, 0 when idle
// volatile: the fade starts set it for ScreenFade_Update, which the VBlank
// handler runs and which counts it down, so it is no device.
/// VBlanks until the running fade reaches its target.
extern volatile u8 gScreenFadeVBlanksLeft;
/// The BLDCNT first-target layers the screen fade covers (the low six bits).
extern u16 gScreenFadeLayers;
/// The brightness the running fade started from, 0 to 16.
extern u8 gScreenFadeStartLevel;
// volatile: ScreenFade_Start sets it for ScreenFade_Update, which the VBlank
// handler runs, so it is no device.
extern volatile u8 gScreenFadeTargetLevel; ///< the brightness the fade ends at
/// Matrix_BuildLookAt's dot product, a Thumb entry and its ARM code: eight
/// words it copies onto its stack to run.
extern const u32 RelocatedInnerProduct[];
/// The tagged LZ decoder in ARM, run by Decompress_TaggedLz from a heap copy.
extern const u8 Decompress_TaggedLzArmCode[];
/// The size of Decompress_TaggedLzArmCode, which Decompress_TaggedLz copies to
/// the heap.
extern u8 Decompress_TaggedLzCodeSize[];
/// The move-to-front LZ decoder's command loop, run by
/// Decompress_MoveToFrontLz from a heap copy.
extern const u8 Decompress_MoveToFrontLzCommandsCode[];
/// The size of Decompress_MoveToFrontLzCommandsCode, which
/// Decompress_MoveToFrontLz copies to the heap.
extern u8 Decompress_MoveToFrontLzCommandsCodeSize[];
/// The byte LZ decoder, run by Decompress_ByteLz from a heap copy.
extern const u8 Resource_DecodeByteLzCode[];
/// The size of Resource_DecodeByteLzCode, which Decompress_ByteLz copies to
/// the heap.
extern u8 Resource_DecodeByteLzCodeSize[];
/// The text decoder's symbol lookup, which Message_Find copies to the heap to
/// run (window/lookup_symbol.s).
extern const u8 Text_LookupSymbolCode[];
/// The size of Text_LookupSymbolCode.
extern u8 Text_LookupSymbolCodeSize[];
/// The size of ScanlineWindow_BuildSpansCode, which ScanlineWindow_FillSpans
/// copies to the heap to run.
extern u8 ScanlineWindow_BuildSpansCodeSize[];
#ifndef OWN_Resource_DecodeByteLzInRam
/// As Decompress_TaggedLz, with the byte LZ decoder.
s32 Decompress_ByteLz(const void *source, void *destination);
#endif
/// Sets up the one-level matrix stack and makes the current matrix the unit
/// matrix.
void Matrix_Initialize(void);

/// The colour index Render_DecodePicture paints every pixel of a layer drawn
/// with SPRITE_COLOR_FILL; the sprite viewer sets it to 3.
extern u8 gDecodeFillByte;
/// Remaps size bytes of pixels in place through gSpriteColorGreyTable.
void SpriteColor_RemapPixelsFar(void *buffer, s32 size);

/// The digits String_FormatHex and String_FormatDecimal write, right-aligned
/// up to gNumberTextHexEnd and gNumberTextDecimalEnd.
extern u8 gNumberTextBuffer[];
/// Writes a value to gNumberTextBuffer right-aligned: spaces, a sign and its
/// decimal digits.
void String_FormatDecimal(s32);
/// Each main font glyph's width in pixels, from the space (32) on.
extern const u8 gFont_GlyphWidths[];
/// Clears the text canvas to transparent.
s32 Text_ClearCanvas(void);
/// Draws a byte string into a text canvas window at pixel (x, y).
void Text_DrawString(u8 *text, s32 window, s32 x, s32 y);
/// Draws a byte string as tiles in a window at pixel (offsetX, offsetY) of its
/// interior.
void Text_DrawStringAtOffsetFar(const u8 *text, s32 window, s32 offsetX,
                                s32 offsetY);
#ifndef OWN_Text_DrawStringInWindowFar
/// Draws a byte string into a window's tilemap at pixel (x, y) of its
/// interior, rounded down to tiles.
void Text_DrawStringInWindowFar(u8 *text, s32 window, u32 x, u32 y);
#endif
/// Draws a number, width digits as String_FormatNumber takes it, as tiles in a
/// window at pixel (x, y) of its interior.
void Text_DrawNumberAtOffsetFar(s32 value, s32 width, s32 window, s32 x, s32 y);
/// As Text_DrawNumberAtOffsetFar, into the window's tilemap.
void Text_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x, s32 y);
/// Draws a number right-aligned in a field ending at pixel x of a window: the
/// HP and PP figures outside Japan.
void Text_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y);
/// Sets the text colour (0 to 15) of drawn text.
s32 Text_SetColorFar(s32 color);

/// The Japanese status panel's PP row, "EP    /": label and slash in one
/// string, drawn by the unit summary.
extern const u8 gStatusPanel_PpRowString[];
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
/// The end of gItemIcons, so Icon_CountItemIcons can count them.
extern u8 *gItemIconsEnd[];
/// The other compressed icons, among them the Status screen's status icons.
extern u8 *gIcon_MiscIconPointers[];
/// The compressed Psynergy icons, drawn over the shared icon base.
extern u8 *gPsynergyIcons[];
/// The end of gPsynergyIcons, for Icon_CountPsynergyIcons.
extern u8 *gPsynergyIconsEnd[];
/// Draws an item's icon into the glyph buffer from its inventory code with the
/// layers asked for (ICON_ flags).
s32 ItemIcon_Compose(u32 code, u32 layers);
/// Draws an item's icon as ItemIcon_Compose does and loads it into VRAM cache
/// entry cacheSlot.
s32 ItemIcon_LoadLayersIntoSlot(s32 inventoryCode, s32 layers, s32 cacheSlot);
/// Draws one of the other icons into the glyph buffer.
void Icon_DrawMisc(u32 index);
/// Builds a command icon, overlayIcon over baseIcon, and loads it into a sprite
/// tile slot: *slot, or a free one when reuse is 0.
void Icon_BuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
                                s32 *tile, s32 reuse);
/// Builds a Psynergy icon, over the icon base when withBase is set, and loads
/// it into *slot, or a free slot unless reuse is set.
void Icon_BuildAbilityIconTiles(u32 icon, s32 withBase, s32 *slot, s32 *tile,
                                s32 reuse);
/// Builds the Psynergy icon of a battle action, as Icon_BuildAbilityIconTiles.
void Ability_BuildIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
                            s32 reuse);
/// Ability_BuildIconTiles, for an overlay.
void Ability_BuildIconTilesFar(s32 action, s32 withBase, s32 *slot, s32 *tile,
                               s32 reuse);

struct WindowSprite;

/// The window sprites' fixed tile blocks (window/fixed_tile_blocks.c).
extern s32 gWindow_FixedTileBlocks[];
/// Starts the field's per-frame window update, after reserving the glyph
/// sprites' tiles and resetting the text style.
void WindowSystem_StartFieldUpdate(void);
/// Starts the per-frame window update: the field's when loadGlyphTiles is set,
/// else the battle's.
void WindowSystem_StartBattleUpdate(s32 loadGlyphTiles);
/// Steps each window's open or close animation.
void Window_UpdateZooms(void);
/// Types more of each message whose window is open, and closes or marks its
/// window at the text's end.
void Message_UpdateAll(void);
/// Copies each band of the window tilemap marked dirty to VRAM, a VBlank task.
void WindowSystem_UploadTilemap(void);
/// Erases a rectangle of the window tilemap, in tiles, and frees the text
/// tiles it showed.
void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
/// Frees the text tiles under a rectangle of the window tilemap, in tiles.
void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);
#ifndef OWN_Window_SetTilemapEntry
/// Writes value into a window's tilemap at tile (x, y) of its interior, in the
/// palette mode given (TILEMAP_ENTRY_).
void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y,
                            u32 mode);
#endif
/// Draws a divider from (x1, y1) to (x2, y2), one of which must agree, joining
/// it to the frame and divider tiles already on the tilemap.
void Window_DrawDividerLineFar(s32 window, s32 x1, s32 y1, s32 x2, s32 y2);
/// Moves each sprite an open window carries and queues it for drawing.
void WindowSprite_UpdateAll(void);
/// Releases every window sprite in a list, or in a window's list, and empties
/// it.
void WindowSprite_ClearList(void *sprites);
/// Frees a window sprite and the VRAM cache entry it showed.
void WindowSprite_Release(struct WindowSprite *node);
/// Marks both portrait slots free.
void Portrait_ClearSlots(void);
/// Reloads the portrait picture for the key into a portrait slot with that
/// slot's palette (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache
/// entry.
void Portrait_ReloadFar(s32 slot, s32 key, s32 unused);
/// Decompresses resource `resourceId` and loads its first `size` bytes into
/// VRAM cache entry `entryId`.
void VramBlock_LoadResourceFar(s32 entryId, s32 size, s32 resourceId);

/// The party status window's column lines, ended by -1: the Djinn column's
/// first, then one between members.
extern const s8 gWindow_PartyColumnOffsets[];
/// Draws the party status window with `flags` (PARTY_STATUS_): each member's
/// name, HP and PP, and in battle the standby Djinn.
void PartyStatus_Draw(s32 flags);
#ifndef OWN_BattleLayout_HighlightPartyPanels
/// Highlights the party status columns of the units in `ids`, ended by
/// BATTLE_UNIT_LIST_END.
s32 PartyStatus_HighlightMembers(u16 *ids);
#endif

/// The two tiles of the link screen's page arrows, which PageArrows_Draw
/// shifts and merges into the window border each frame.
extern u8 gPageArrowTiles[];
/// Merges two 4bpp tiles row by row into `output`: `second` moved `shift`
/// pixels, drawn over `first`.
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift);
/// The file menu's 64x16 headings by number, each drawn as two 32x16 sprites
/// (MenuTitle_Load; graphics/ui/menu_titles.s).
extern s32 gMenuTitlePictures[];
/// The task that makes the file list's highlight glow.
void FileList_UpdatePulse(void);
/// Empties the selection screen's list of entries.
void SelectionMenu_Clear(void);
/// Loads the icon of a selection entry of kind mode (enum IconKind) into a free
/// sprite tile slot and returns the slot, or -1.
s32 SelectionIcon_Load(u32 mode, s32 value);
/// Frees an arrow sprite's tile slot.
void MenuArrow_Release(s32 slot);
/// The eight scales WindowSprite_Pulse pulses the settings menu's selected icon
/// through.
extern const FIXED gWindowSprite_PulseScales[];
/// Eight offsets, 0 to 3 and back, that make the name entry screen's caret
/// circle: the x offset leads the y offset by five steps.
extern const s8 gNameEntry_CaretWave[];
/// The name entry screen keyboard's compressed tilemap, the Spanish edition's
/// with its own letters.
extern const u8 gNameEntry_KeyboardTilemap[];
/// The Title's "Press Start" prompt, byte-LZ compressed, in each
/// language.
extern const u8 gTitle_PromptTiles[];

/// 0 while the credits text scrolls; once a line stops it, the line's stop
/// (1 at a page mark, which A or B releases), then counted up each frame after
/// the end (CREDITS_FRAMES_AFTER_END).
extern s16 gDisplayScroll_Busy;
/// gDisplayScroll_Position when the credits last drew a line.
extern s16 gDisplayScroll_LastPosition;
/// How far the credits text has scrolled, in pixels: one every four VBlanks,
/// faster while B is held.
extern u16 gDisplayScroll_Position;
/// Draws one line of the credits' scrolling text into object tiles from slot
/// on.
s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align);

/// Lists the units on the sides in sideMask into unitIds, or only counts them
/// when it is NULL, and returns how many.
s32 BattleUnit_ListIdsFar(s32 sideMask, u16 *unitIds);
/// Lists the party members who fight into owners when it is not NULL, and
/// returns how many.
s32 BattleParty_PrepareMembersFar(u16 *owners);

#endif
