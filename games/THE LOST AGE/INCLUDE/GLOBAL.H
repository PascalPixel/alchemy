#ifndef GLOBAL_H
#define GLOBAL_H

#include "TYPES.H"

extern const u16 Battle_Effect90Classes[47];
extern const u16 Battle_Effect91Classes[29];
extern const u8 BattleFx_ArcTiles[];
extern u16 BattleFx_CyclePatternWords[];
extern u16 BattleFx_EightWayHeadingWords[];
extern const u8 BattleFx_ParticleScript[];
extern s8 BattleFx_RandomChildValues[];
extern const u8 BattleFx_UntargetedObjectScript[];
extern s32 BattleMotion_VariantAcceleration[];
extern s32 BattleMotion_VariantDistancePercent[];
extern s32 BattleMotion_VariantSpeedLimit[];
extern s32 BattleMotion_VariantVelocityY[];
extern const s8 BattlePlacement_StepPairs[];
extern const s32 Camera_FixedViewMatrix[];
extern u8 CharacterMenu_CursorWidths[];
extern const u8 ColorBuffer_FadeStep1Map[32];
extern const u8 ColorBuffer_FadeStep2Map[32];
extern const u8 ColorBuffer_FadeStep3Map[32];
extern u16 Data_02005830;
extern s32 Data_03001cc0;
extern s32 Data_03001d1c;
extern u8 Data_03001e8c[];
extern u8 Data_03001e90[];
extern u8 Data_08105948[];
extern s16 DisplayScroll_Busy;
extern s16 DisplayScroll_LastPosition;
extern u16 DisplayScroll_Position;
extern const u16 Djinn_Definitions[][6];
extern const u8 DriftScene_SpawnScript[];
extern u8 Encounter_AreaEntryTable[];
extern u16 Encounter_EnemyGroupTable[];
extern const u8 Enemy_DefinitionTable[];
extern u32 Flash_Handler1;
extern u32 Flash_Handler2;
extern u8 GameFlagBytes[];
extern u8 gBlendBrighten;
extern volatile u8 gBlendDuration;
extern u8 gBlendFramesLeft;
extern u16 gBlendLayers;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendTargetLevel;
extern u32 gCameraSceneParameters[3];
extern volatile u8 gFlashTimerNum;
extern const u32 gIdejimaExits[];
extern volatile u16 gLinkStatus;
extern u8 gNumberTextBuffer[];
extern u8 gNumberTextDecimalEnd[];
extern u8 gNumberTextHexEnd[];
extern u8 gPlayerObjectId[];
extern u16 Graphics_ExpandNibbleTable[];
extern u8 gRomShiftedTilePair[];
extern s32 gScanlineWaveCount;
extern s16 gScanlineWavePage;
extern const u32 gSceneExits[];
extern u8 gSongFlags[];
extern const u8 Idejima_ActorDepartureScript[];
extern u8 Link_TimeLabelString[];
extern const u8 LinkLobby_SlotColumns[];
extern const s32 LinkLobby_SlotValues[];
extern u8 Map_TileDissolveOrder[];
extern const u16 Math_ArcTanTable[];
extern u8 Menu_ListOrderDefault[];
extern u8 Menu_ListOrderMode0[];
extern u8 Menu_ListOrderMode1[];
extern u8 Menu_ListOrderMode2[];
extern u8 Menu_MinusSignString;
extern u8 Menu_PlusSignString;
extern s8 Menu_TopEntryCommandByPosition[];
extern s8 Menu_TopEntryPositionByCommand[];
extern const u32 ObjectDispatch_DefaultScript[];
extern const u8 ObjectGroup_BlinkChildValues[4];
extern const u8 ObjectMotion_StepAngleScript[];
extern const u32 RelocatedQ16DotProduct[];
extern u8 Render_DecodeFrameCodeSize[];
extern s32 RenderResource_PairSourceTable[];
extern const u8 Resource_Decode01Code[];
extern u8 Resource_Decode01CodeSize[];
extern const u8 Resource_Decode2Code[];
extern u8 Resource_Decode2CodeSize[];
extern const u8 Resource_DecodeByteLzCode[];
extern u8 Resource_DecodeByteLzCodeSize[];
extern void *Resource_DirectoryTable[];
extern const u8 Resource_PatchThumbBranchCode[];
extern u8 Resource_PatchThumbBranchCodeSize[];
extern u16 Resource_SlotAssignments[];
extern char ResourceId_UiFixedBlockATiles;
extern char ResourceId_UiIconTiles;
extern u8 ResourceSlot_ConversionTables[][256];
extern u8 RomBytes_08029e00[];
extern u8 RomBytes_0802de88[];
extern u8 RomBytes_0802e108[];
extern u8 Runtime_ByteRemapTable[];
extern u16 Script_MainScript[];
extern u8 Shop_GlyphBytes[];
extern u16 Shop_GlyphRowOffsets[];
extern u16 Shop_SelectorOffsets[];
extern const u8 Sound_Cgb3LevelCodes[];
extern const s16 Sound_CgbFrequencySteps[];
extern const u8 Sound_CgbPitchCodes[];
extern const u16 Sound_FrameLengths[];
extern const u8 Sound_NoisePitchCodes[];
extern u32 Sound_PcmFrequencySteps[];
extern u8 Sound_PcmPitchCodes[];
extern u8 Sound_WorkBytes[];
extern u8 StatusMenu_LevelLetterString[];
extern const u8 System_BasicColorPalette[];
extern const u8 Text_LookupSymbolCode[];
extern u8 Text_LookupSymbolCodeSize[];
extern const u8 Ui_EpRowString[];
extern s32 Ui_FixedTileBlocks[];
extern const u8 Ui_HpRowString[];
extern const u8 Ui_HpString[];
extern const u8 Ui_PpString[];
extern const u8 Ui_SlashString[];
extern const u16 Ui_WindowPalette[];
extern u8 UiIcon_ItemIconPointers[];
extern u8 UiIcon_ItemIconPointersEnd[];
extern u8 *UiIcon_MiscIconPointers[];
extern u8 UiIcon_PsynergyIconPointers[];
extern u8 UiIcon_PsynergyIconPointersEnd[];
extern const u8 WorldMap_TerrainBehaviorTable[];
#ifndef OWN_gFlash
extern struct FlashChipInfo *gFlash;
#endif
#ifndef OWN_gFlashTimeoutFlag
extern u8 gFlashTimeoutFlag;
#endif
#ifndef OWN_gFlashSavedIme
extern volatile u16 gFlashSavedIme;
#endif
#ifndef OWN_gFlashTimerCount
extern volatile u16 gFlashTimerCount;
#endif
#ifndef OWN_Flash_Handler4
extern u32 Flash_Handler4;
#endif

#endif
