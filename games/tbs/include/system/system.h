#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

/// @file
/// The system's routines and globals: interrupts, BIOS sound bias, random
/// numbers and sleep.

#include "types.h"
#include "system/heap_release.h"
#include "edition.h"
#include "gamepak/far_call.h"
#include "system/heap.h"

void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
/// 1 while the CPU is stopped for sleep, until L and R wake it.
extern u16 gSleepActive;

/// gNumberBuffer + 10, the decimal form's units digit (the form ends at
/// + 11); DebugText_PrintDecimal starts n characters before it, so it prints
/// n + 1.
extern u8 gNumberBufferDecimalEnd[];
/// The end of the hexadecimal form in gNumberBuffer.
extern u8 gNumberBufferHexEnd[];
/// The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in
/// French.
extern u8 gLink_TimeLabelString[];
/// Each resource's ROM address by resource id (Resource_GetTableEntry).
extern void *Resource_DirectoryTable[];
/// The size of Resource_PatchThumbBranch (decompress_tagged_lz.s).
extern u8 Resource_PatchThumbBranchCodeSize[];
/// The window font's 16 colours, loaded as BG palette 15
/// (Window_LoadGraphics).
extern const u16 gWindow_Palette[];

struct SpriteAnimation;
struct Sprite;

/// The 152 bytes of IWRAM-resident sound code (0x03000bd8, not the mixer at
/// 0x03007000) that Sound_LoadReverbCode overwrites with a reverb code
/// from IwramSoundReverbCodes.
extern u8 IwramSoundReverbCode[];
/// HEAP_SLOT_SPRITE_ANIMATIONS: the sprite animations (struct
/// SpriteAnimation) that SpriteAnimation_Alloc takes from.
#define gSpriteAnimations                                                      \
  ((struct SpriteAnimation **)&gHeapSlots[HEAP_SLOT_SPRITE_ANIMATIONS])
/// Where the debug text writes next on its 32 by 20 screen in BG0.
extern u16 *gDebugTextCursor;
/// Nonzero lets DebugText_Print write; the power-on set-up and the credits
/// clear it.
extern u8 gDebugTextEnabled;
/// The buffer a save file is built in and written from, in two halves; its
/// summary's 16-byte header lies just before it.
extern u8 gSaveBuffer[];
/// The current save file, or -1 for none (a new game).
extern s16 gSaveSlot;
/// The five reverb codes' mixer code, SOUND_REVERB_CODE_SIZE bytes each.
extern u8 IwramSoundReverbCodes[];
/// The size of Oam_BuildBufferKernel (decompress_byte_lz.s).
extern u8 LoadedRuntime_Size[];
/// The ARM OAM list builder, run from a heap copy by Oam_BuildBuffer.
extern const u8 Oam_BuildBufferKernel[];
/// The ARM register-write executor, run from a stack copy by
/// IoWriteQueue_FlushPending.
extern const u8 Resource_ExecuteTransfer[];
/// The ARM routine that resolves a loaded overlay's long branches, run from a
/// heap copy by Overlay_Load.
extern const u8 Resource_PatchThumbBranch[];
/// The game code's last letter in the cartridge header: `J` in Japanese, `E`,
/// `D`, `F`, `S` or `I` (System_GetLanguage).
extern const u8 gRom_LanguageCode;
/// The basic sprite palettes 0 to 13, 224 colours
/// (Palette_LoadBasicSpriteColors).
extern const u8 gBasicSpritePalettes[];
/// The sprite tile uploader, run from HEAP_SLOT_PICTURE_UPLOADER.
extern u8 Tile_CopyStrided[];
/// The size of Tile_CopyStrided (map_grid_convert.s).
extern u8 Tile_CopyStridedCodeSize[];
/// The size of Resource_ExecuteTransfer (decompress_tagged_lz.s).
extern u8 TransferCommandExecutor_Size[];
void Iwram_Halt(void);
void Mixer_Enter(void);

#include "types.h"
#include "system/task.h"
#include "system/irq.h"

struct AffineTransform;

void System_Initialize(void);
void System_IdleForever(void);
/// The cartridge's language, from its header (System_GetLanguage).
enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_SPANISH,
  CARTRIDGE_LANGUAGE_ITALIAN
};
s32 System_GetLanguage(void);
void Stub_ReservedNoOpA(void);
void Stub_ReservedNoOpB(void);
void Stub_ReservedNoOpC(void);
void Stub_ReservedNoOpD(void);
void Stub_ReservedNoOpE(void);
s32 Stub_ReturnZero(s32 unused0, const u8 *unused1);
void System_CopyBuildStamp(void);
void Stub_NoOpHook(s32 unused);
void *Resource_GetTableEntry(s32 index);
void Code_ResolveLongBranches(u16 *entries, s32 byteLength);
void Overlay_Load(u32 index, void *destination);
void Irq_Ignore(void);
void System_InstallIwramAndIrqs(void);
void Irq_SetHandler(u32 irq, s32 vcount, InterruptHandler handler);
void wait(s32 frames);
void Input_ResetKeyRepeat(void);
void Input_UpdateKeyRepeatAndDirection(void);
void System_VBlankHandler(void);
s32 Sound_LoadReverbCode(s32 code);
void Sound_ReservedNoOpA(void);
void Sound_ReservedNoOpB(void);
void Input_HandleKeyInterrupt(void);
void ScreenFade_ToBlack(s32 vblanks);
void ScreenFade_FromBlack(s32 vblanks);
void ScreenFade_ToWhite(s32 vblanks);
void ScreenFade_FromWhite(s32 vblanks);
s32 Decompress_TaggedLz(const void *source, void *destination);
s32 Decompress_MoveToFrontLz(const void *source, void *destination);
s32 Decompress_ByteLz(const void *source, void *destination);
void Serial_WaitForSend(void);
void Serial_WaitForReceive(void);

void IoWriteQueue_FlushPending(void);

void ScreenFade_Update(void);
void ScreenFade_Start(s8 bright, s32 layer, u32 start, s32 target, s32 vblanks);
void ScreenFade_Wait(void);

void Oam_ResetFrame(void);
s32 OamMatrix_AllocFromTransform(struct AffineTransform *source);
void Oam_AddSpriteAtDepth(void *slotEntry, s32 slot);
void Oam_BuildBuffer(void *work);

s32 VramCache_AllocateBlocks(u32 id, u32 size);
s32 VramCache_GetLongestFreeRun(void);
s32 VramCache_FreeBlocks(s32 id);
s32 VramCache_FreeEntry(u32 no);
s32 VramCache_ReleaseVram(u32 no);
s32 VramCache_Load(u32 no, u32 size, const void *src);
void VramCache_Reset(void);
s32 VramCache_FindFreeEntry(void);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 no, s32 src);

void Task_ResetAll(void);
void Task_CopyWords(u32 *destination, u32 *source, u32 byteCount);
void Task_Sort(void);
s32 Task_Find(TaskFunc func);
s32 Task_Add(TaskFunc func, s32 order);
void Task_Idle(void);
void Task_Empty(void);
s32 Task_Remove(TaskFunc func);
s32 Task_SuspendAll(TaskFunc func);
s32 Task_SuspendOverlayTasks(void);
s32 Task_SetFlags(TaskFunc func, u32 mask);
s32 Task_ResumeAll(TaskFunc func);
s32 Task_ResumeOverlayTasks(void);
void Task_RunPass(s32 pass);

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, void *position);
s32 ArcTan2(s32 x, s32 y);
s32 Math_IntegerSqrt(s32 value);
s32 FixedSqrt(s32 value);

s32 Palette_GetBasicSpriteColors(void);
void String_FormatHex(u32 value);
void String_FormatDecimal(s32 number);
void DebugText_PrintBlanks(u32 count);
void *memcpy(void *destination, const void *source, unsigned long size);
void DebugText_Print(const u8 *src);
void DebugText_PrintHex(u32 value, s32 digits);
void DebugText_PrintDecimal(s32 value, s32 digits);
void DebugText_Clear(void);
void Window_LoadGraphics(void);
void Palette_LoadBasicSpriteColors(void);

void Heap_Initialize(void);
s32 Heap_GetRemainingIwram(void);
s32 Heap_GetRemainingEwram(void);
void *HeapSlot_Alloc(s32 kind, s32 size);
void *HeapSlot_AllocEwram(s32 kind, s32 size);
void *Heap_Alloc(s32 size);
void *Heap_AllocEwram(s32 size);
void Heap_ReservedNoOp(void);

void Transform_Reset(void);
void Transform_Push(void);
void Transform_Store(void *dst);
void Transform_Load(const void *src);
void Transform_Pop(void);
void Transform_ResetMatrix(void);
void Transform_ApplyRotation(s32 *angles);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void Transform_ApplyRoll(s32 angle);
void Transform_ApplyPosition(const s32 *position);
void Transform_ApplyScale(const s32 *scale);
void Transform_ApplyRotationTranslation(s32 *angles, s32 *position);
void Transform_ApplyScaledRotation(s32 *angles, s32 *position, s32 *scale);
void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Transform_ApplyLookAt(s32 *eye, s32 *target);
void Projection_SetFieldOfView(u32 fieldOfView, s32 nearDepth, s32 farDepth);
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth);
s32 Transform_ProjectPoint(const void *point, s32 *screen);

void Decompress_Halfwords(const void *source, void *destination);
void PaletteFade_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup,
                                   u32 bytes);
void PaletteFade_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
void PaletteFade_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
void PaletteFade_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes);
void PaletteFade_BackupAndBrightenPartial(u8 *buffer, u32 amount, u8 *backup,
                                          u32 bytes);
void PaletteFade_BackupAndDarkenPartial(u8 *buffer, u32 amount, u8 *backup,
                                        u32 bytes);
void PaletteFade_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup,
                                                    u32 bytes);
void PaletteFade_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
s32 Save_InitializeWork(void);
u32 Save_SelectWriteSector(s32 mode);
u32 Save_WriteSector(s32 sector);
s32 Save_ReadSector(s32 index);
u16 Save_EraseSector(u16 sector);
s32 Save_WritePart(s32 partId, void *source);
u32 Save_ReadPart(s32 partId, void *destination);
u32 Save_DeletePart(s32 partId);
s32 Save_ChecksumSector(void);
u32 Save_FindLatestSector(s32 partId);
s32 Save_InvalidateSector(s32 index);
s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);
u32 Save_GetLatestCounter(s32 partId);
s32 Save_LoadFileSummaries(void);
u32 Save_ReleaseWork(void);

void Serial_Initialize(void);
void Serial_EnableTransferTimer(void);
void Serial_DisableTransfer(void);
s32 Serial_PollAndTransfer(void *send, void *receive);
s32 Serial_PollStatus(void);
s32 Serial_ExchangePayloads(void *send, void *receive);
void Serial_PrepareSendPacket(void *payload);
u8 Serial_CollectReceivedPayloads(void *payload);
void Serial_HandleTransferInterrupt(void);
void Serial_RemoveIrqHandlers(void);
u32 Serial_WaitForStatusMask(s32 mask);
s32 Serial_StartSend(void *source, s32 size);
s32 Serial_StartReceive(void *destination);
void Serial_WaitForTransfers(void);
s32 Serial_GetActiveTransfers(void);
void LinkBattle_ResetTransferState(void);
void Serial_StepBlockTransfer(void);
void SerialTest_Delay(void);
void SerialTest_Run(void);

#endif
