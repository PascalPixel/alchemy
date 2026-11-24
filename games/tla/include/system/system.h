#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

/// @file
/// The system bank's routines and globals, by the part each belongs to: the
/// frame, random numbers, tasks, sequences and overlays, sprites, matrices,
/// fades, resources, windows, the debug text and the link cable.

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "system/heap_release.h"

// The frame and the console: waiting, interrupts, sleep, the cartridge's
// language and its build stamp.
void wait(s32 frames);
void System_WaitForFrameInterrupt(void);
void System_VBlankHandler(void);
/// The Game Pak interrupt's handler, in IWRAM: stops the console for good once
/// the cartridge is pulled (the BIOS's Stop, again after any wake).
void Iwram_Halt(void);
/// The BIOS's SoundBias: Off ramps the sound output's bias down before the
/// console sleeps, On back up once it wakes, so the speaker does not click.
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
/// 1 while the CPU is stopped for sleep, until L and R wake it.
extern u16 gSleepActive;
/// The cartridge's first byte, as the link places it (rom_header.s).
extern const u8 Rom_Start[];
/// The cartridge's language, from its header (System_GetLanguage).
enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH = 1,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_ITALIAN,
  CARTRIDGE_LANGUAGE_SPANISH
};
s32 System_GetLanguage(void);
void System_CopyBuildStamp(void);

// Random numbers.
u32 Random16(void);
/// A random number from 0 to n - 1: n times a 16-bit random draw, over 65536.
#define Random_Below(n) (Random16() * (n) >> 16)
/// Random16's seed; Lucky Wheels seeds it with the play time, and a link
/// battle's parent sends it as the turn's gBattleRandomSeed.
extern u32 gRandomSeed;

// The task passes and the queued register writes they flush.
/// A task's routine as the task passes call it: returning a value, where the
/// task itself returns none (Task_RunPass's UB).
typedef s32 (*TaskIntFunc)(void);
struct IoWriteQueue;
/// The routine that carries out `count` queued register writes
/// (Resource_ExecuteTransferCode, run from a copy on the stack).
typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);
/// The routine that runs the queued register writes, which
/// IoWriteQueue_FlushPending copies onto its stack and runs.
extern u8 Resource_ExecuteTransferCode[];
/// The size of Resource_ExecuteTransferCode.
extern u8 Resource_ExecuteTransferSize[];

// The sequences the main routine runs, how the game starts, and the
// overlays it loads.
/// Sequence ids above the scene table's rows, as The Broken Seal's
/// SEQUENCE_ values: the sequences other than the field sequence, which
/// MainRoutine_Run runs in place of a scene.
enum {
  /// The Super Lucky Dice sequence (SuperLuckyDice_Run); Contigo's dealer
  /// leaves for it. The Broken Seal's 507, SEQUENCE_NONE, runs nothing.
  SEQUENCE_SUPER_DICE = 507,
  SEQUENCE_SLOTS = 508, ///< the slots sequence (LuckyWheels_Run)
  SEQUENCE_DICE = 509,  ///< the dice sequence (LuckyDice_Run)
  SEQUENCE_BATTLE = 510 ///< a battle; the entrance is the formation
};
/// MainRoutine_Run's mode: where the game starts. Only GAME_START_TITLE is
/// passed (System_Initialize, Game_RestartQuest); the others are its debug
/// starts.
enum GameStart {
  GAME_START_TITLE = 0,                  ///< the Title, with the state cleared
  GAME_START_DEBUG_VENUS_LIGHTHOUSE = 1, ///< SCENE_VENUS_LIGHTHOUSE_ENTRANCE
  GAME_START_DEBUG_CLEAR = 2,            ///< SCENE_CLEAR, as from the title
  GAME_START_DEBUG_MT_ALEPH = 3,         ///< SCENE_MT_ALEPH_SUMMIT
  GAME_START_DEBUG_TITLE = 4 ///< the story crawl (TITLE_STORY_CRAWL)
};
s32 Battle_RunFar(s32 entrance);
/// Plays Lucky Wheels (LuckyWheels_Start).
s32 LuckyWheels_StartFar(s32 entrance);
s32 SuperLuckyDice_StartFar(s32 entrance);
void LuckyDice_Run(void);
void Scene_Arrive(s32 unused, s32 keep);
void Scene_LoadOverlay(s32 unused);
void Scene_PlayMusic(void);
void Overlay_Load(u32 index, void *destination);
/// The ARM routine that resolves a loaded overlay's long branches, run from a
/// heap copy by Overlay_Load.
extern const u8 Resource_PatchThumbBranch[];
/// The size of Resource_PatchThumbBranch (decompress_tagged_lz.s).
extern u8 Resource_PatchThumbBranchCodeSize[];
/// The compiler library's call through r3, which the IWRAM-resident code's
/// callers use to run code copied to RAM.
s32 _call_via_r3(s32, u8 *, u8, s32);

// Sprites: their depth slots, affine matrices, VRAM and colours.
void Oam_ResetFrame(void);
/// Which of the 256 depth slots of gOamDepthLists hold sprites this frame, a
/// bit a slot; Oam_ResetFrame clears it.
extern u8 gOamDepthMasks[];
/// The 256 depth slots' list heads that every drawn sprite is linked into
/// (Oam_AddSpriteAtDepth); Oam_ResetFrame empties them each frame.
extern void *gOamDepthLists[];
/// Depth slots of Oam_AddSpriteAtDepth: a higher slot is drawn in front. The
/// windows' sprites, icons and cursors take the slots from OAM_DEPTH_WINDOW up,
/// in front of the field's and the battle's, and OAM_DEPTH_FRONT is in front of
/// everything.
#define OAM_DEPTH_WINDOW 240
#define OAM_DEPTH_FRONT 255
/// The depth slot the field Psynergy's effect sprites take: in front of every
/// actor, whose slots are clamped below OAM_DEPTH_WINDOW, and among the window
/// sprites' slots.
#define EFFECT_SPRITE_FRONT_DEPTH 250
void Oam_AddSpriteAtDepthClamped(void *entry, s32 slot);
void VramCache_Reset(void);
struct SpriteAnimation;
struct SpriteAnimation *SpriteAnimation_Alloc(s32 id);
void SpriteAnimation_Clear(void *destination);
s32 SpritePictureCache_FindByTag(s32 key);
/// Five tables that map a picture's 256 colour indices onto a sprite's palette,
/// one for each palette a field sprite can take (field/sprite_color_tables.c).
extern u8 gSpriteColorTables[][256];
void Palette_LoadBasicSpriteColors(void);
s32 Palette_GetBasicSpriteColors(void);
/// Two 16-entry colour maps for 4bpp pixels: the first draws colour 1 as
/// colour 10, the second leaves every colour as it is.
extern u16 gGraphics_ExpandNibbleTable[];

// Matrices and the projection.
void Matrix_RotateY(s32 angle);
void Matrix_SetRotateY(s32 angle);
void Matrix_RotateX(s32 angle);
void Matrix_RotateZ(s32 angle);
/// The basic sprite palettes, loaded for the debug text
/// (Palette_GetBasicSpriteColors).
extern const u8 gSystem_BasicColorPalette[];
FIXED Math_SqrtFX(FIXED value);

void Matrix_TranslateVector(const FIXED *position);
void Matrix_Translate(FIXED x, FIXED y, FIXED z);
void Matrix_ScaleUniform(FIXED scale);
void Matrix_Scale(FIXED x, FIXED y, FIXED z);
void Matrix_Pop(void);
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth);
/// The far depth well past the battlefield: the largest whole 16.16 number.
#define BATTLE_FAR_DEPTH toFIXED(32767)
s32 Trig_Atan(s32 x, s32 y);
void Vector_AddPolarOffset(FIXED radius, s32 angle, FIXED *position);
/// The dot product of (ax, ay, az) and (bx, by, bz), in 16.16
/// (RelocatedInnerProduct, run from a copy on the stack).
typedef s32 (*DotProductRoutine)(s32 ax, s32 bx, s32 ay, s32 by, s32 az,
                                 s32 bz);

// Screen fades.
void ScreenFade_ToBlack(s32 vblanks);
void ScreenFade_FromBlack(s32 vblanks);
void ScreenFade_ToWhite(s32 vblanks);
void ScreenFade_FromWhite(s32 vblanks);
void ScreenFade_Wait(void);
void ScreenFade_Update(void);

// Resources and decompression.
/// Each resource's ROM address by resource id (Resource_GetTableEntry).
extern void *Resource_DirectoryTable[];
s32 Stub_ReturnZero(s32 resourceId, const u8 *table);
void Resource_ReservedNoOp(s32 unused);
void Decompress_Halfwords(const void *source, void *destination);
/// The halfword LZ decoder that Decompress_Halfwords runs from its stack.
extern const u8 Decompress_HalfwordLz[];
/// The size of Decompress_HalfwordLz, copied onto the stack by
/// Decompress_Halfwords.
extern u8 Decompress_HalfwordsCodeSize[];
s32 Decompress_MoveToFrontLz(const void *source, void *destination);
s32 AbilityIcon_LoadIntoSlot(s32 resource, s32 index);

// Windows and their text.
void Window_LoadGraphics(void);
/// The window font's 16 colours, loaded as BG palette 15
/// (Window_LoadGraphics).
extern const u16 gWindow_Palette[];
void PartyStatus_Close(void);
/// The party status window's layout (struct PartyStatusWindow), the block of
/// heap slot HEAP_SLOT_PARTY_STATUS.
extern u8 gPartyStatusState[];
/// The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in
/// French.
extern u8 gLink_TimeLabelString[];
void Input_ResetKeyRepeat(void);

// The debug text and the number strings it prints.
/// Nonzero lets DebugText_Print write; the power-on set-up and the credits
/// clear it.
extern u8 gDebugTextEnabled;
/// Where the debug text writes next on its screen in BG0.
extern u16 *gDebugTextCursor;
void DebugText_Print(const u8 *src);
void DebugText_Clear(void);
void String_FormatHex(u32);
/// The hexadecimal digits 0 to F, which String_FormatHex writes from.
extern const u8 gString_HexDigits[];
/// The end of String_FormatDecimal's digits: DebugText_PrintDecimal prints the
/// last places before it.
extern u8 gNumberTextDecimalEnd[];
/// The end of String_FormatHex's digits: DebugText_PrintHex prints the last
/// places before it.
extern u8 gNumberTextHexEnd[];

// The link cable.
void Serial_Initialize(void);
/// A halfword in a record of its own, which Serial_StepBlockTransfer keeps its
/// constant 1 in.
struct SerialHalfword {
  u16 value;
};

// Declarations other banks' code shares.
struct BattleObject;
struct Actor;
#ifndef OWN_BattleMotion_ApproachTarget
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelFrames,
                                 s32 initialVelocityY);
#endif
/// An empty function, reached only through an unnamed veneer in the object
/// bank's far-call table.
void Actor_NoOp(void);
s32 TilesetAnim_ResumeAllFar(void);
s32 TilesetAnim_SuspendAllFar(void);

#endif
