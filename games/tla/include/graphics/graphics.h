#ifndef ALCHEMY_GRAPHICS_H
#define ALCHEMY_GRAPHICS_H

/// @file
/// The screen fade's globals, the decoders and other code run from RAM, and
/// the sprite colour remap.
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

#endif
