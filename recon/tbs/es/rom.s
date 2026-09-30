@ tbs-es's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00002e20, "ax"
	.global System_Initialize
	.type System_Initialize, %function
	.thumb_func
System_Initialize:
	.incbin "baserom.gba", 0x00002e20, 0x00000138
	.section .rom.00002f58, "ax"
	.global RuntimeDispatch_ReservedNoOpA
	.type RuntimeDispatch_ReservedNoOpA, %function
	.thumb_func
RuntimeDispatch_ReservedNoOpA:
	.incbin "baserom.gba", 0x00002f58, 0x00000004
	.section .rom.00002f5c, "ax"
	.global RuntimeDispatch_ReservedNoOpB
	.type RuntimeDispatch_ReservedNoOpB, %function
	.thumb_func
RuntimeDispatch_ReservedNoOpB:
	.incbin "baserom.gba", 0x00002f5c, 0x00000004
	.section .rom.00002f60, "ax"
	.global RuntimeDispatch_ReservedNoOpC
	.type RuntimeDispatch_ReservedNoOpC, %function
	.thumb_func
RuntimeDispatch_ReservedNoOpC:
	.incbin "baserom.gba", 0x00002f60, 0x00000004
	.section .rom.00002f64, "ax"
	.global RuntimeDispatch_ReservedNoOpD
	.type RuntimeDispatch_ReservedNoOpD, %function
	.thumb_func
RuntimeDispatch_ReservedNoOpD:
	.incbin "baserom.gba", 0x00002f64, 0x00000004
	.section .rom.00002f68, "ax"
	.global RuntimeDispatch_ReservedNoOpE
	.type RuntimeDispatch_ReservedNoOpE, %function
	.thumb_func
RuntimeDispatch_ReservedNoOpE:
	.incbin "baserom.gba", 0x00002f68, 0x00000004
	.section .rom.00002f6c, "ax"
	.global RuntimeDispatch_ReturnZero
	.type RuntimeDispatch_ReturnZero, %function
	.thumb_func
RuntimeDispatch_ReturnZero:
	.incbin "baserom.gba", 0x00002f6c, 0x00000004
	.section .rom.00002f70, "ax"
	.global Resource_LoadWorkHeader
	.type Resource_LoadWorkHeader, %function
	.thumb_func
Resource_LoadWorkHeader:
	.incbin "baserom.gba", 0x00002f70, 0x0000002c
	.section .rom.00002f9c, "ax"
	.global RuntimeDispatch_NoOpHook
	.type RuntimeDispatch_NoOpHook, %function
	.thumb_func
RuntimeDispatch_NoOpHook:
	.incbin "baserom.gba", 0x00002f9c, 0x00000004
	.section .rom.00002fa0, "ax"
	.global Resource_GetTableEntry
	.type Resource_GetTableEntry, %function
	.thumb_func
Resource_GetTableEntry:
	.incbin "baserom.gba", 0x00002fa0, 0x00000070
	.section .rom.00003010, "ax"
	.global Resource_LoadCode
	.type Resource_LoadCode, %function
	.thumb_func
Resource_LoadCode:
	.incbin "baserom.gba", 0x00003010, 0x00000058
	.section .rom.0000306a, "ax"
	.incbin "baserom.gba", 0x0000306a, 0x00000072
	.section .rom.00003eb8, "ax"
	.global ResourceTable_AllocateBlocks
	.type ResourceTable_AllocateBlocks, %function
	.thumb_func
ResourceTable_AllocateBlocks:
	.incbin "baserom.gba", 0x00003eb8, 0x0000007c
	.section .rom.000047fc, "ax"
	.global Ui_LoadWindowGraphics
	.type Ui_LoadWindowGraphics, %function
	.thumb_func
Ui_LoadWindowGraphics:
	.incbin "baserom.gba", 0x000047fc, 0x0000009c
	.section .rom.00005044, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x00005044, 0x000001f4
	.section .rom.0000572c, "ax"
	.global SaveState_InitializeWorkspace
	.type SaveState_InitializeWorkspace, %function
	.thumb_func
SaveState_InitializeWorkspace:
	.incbin "baserom.gba", 0x0000572c, 0x00000144
	.section .rom.000061bc, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x000061bc, 0x000000e4
	.section .rom.00006468, "ax"
	.global SerialRuntime_BeginTransferB
	.type SerialRuntime_BeginTransferB, %function
	.thumb_func
SerialRuntime_BeginTransferB:
	.global Party_Check
	.type Party_Check, %function
	.thumb_func
Party_Check:
	.incbin "baserom.gba", 0x00006468, 0x00000050
	.section .rom.000065bc, "ax"
	.global SerialRuntime_StepBlockTransfer
	.type SerialRuntime_StepBlockTransfer, %function
	.thumb_func
SerialRuntime_StepBlockTransfer:
	.incbin "baserom.gba", 0x000065bc, 0x0000023c
	.section .rom.000068ce, "ax"
	.incbin "baserom.gba", 0x000068ce, 0x00000002
	.section .rom.000068d6, "ax"
	.incbin "baserom.gba", 0x000068d6, 0x00000002
	.section .rom.000068d8, "ax"
	.global ReadFlashId
	.type ReadFlashId, %function
	.thumb_func
ReadFlashId:
	.incbin "baserom.gba", 0x000068d8, 0x00000098
	.section .rom.00006ad8, "ax"
	.incbin "baserom.gba", 0x00006ad8, 0x00000048
	.section .rom.00006b20, "ax"
	.global CopyFlashReadRoutineToRam
	.type CopyFlashReadRoutineToRam, %function
	.thumb_func
CopyFlashReadRoutineToRam:
	.incbin "baserom.gba", 0x00006b20, 0x000000c4
	.section .rom.00006be4, "ax"
	.global ReadFlashCore
	.type ReadFlashCore, %function
	.thumb_func
ReadFlashCore:
	.incbin "baserom.gba", 0x00006be4, 0x00000024
	.section .rom.00006c84, "ax"
	.global VerifyFlashCore
	.type VerifyFlashCore, %function
	.thumb_func
VerifyFlashCore:
	.incbin "baserom.gba", 0x00006c84, 0x00000044
	.section .rom.00006fa8, "ax"
	.global CountRemainingErasedFlashBytes
	.type CountRemainingErasedFlashBytes, %function
	.thumb_func
CountRemainingErasedFlashBytes:
	.incbin "baserom.gba", 0x00006fa8, 0x00000024
	.section .rom.00006fcc, "ax"
	.global RunFlashEraseVerifier
	.type RunFlashEraseVerifier, %function
	.thumb_func
RunFlashEraseVerifier:
	.incbin "baserom.gba", 0x00006fcc, 0x00000018
	.section .rom.00007380, "ax"
	.incbin "baserom.gba", 0x00007380, 0x00000356
	.global Math_ArcTanTable
Math_ArcTanTable:
	.incbin "baserom.gba", 0x000076d6, 0x00000126
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "baserom.gba", 0x000077fc, 0x000001c0
	.global RomBytes_0800795c
RomBytes_0800795c:
	.incbin "baserom.gba", 0x000079bc, 0x00000014
	.global Text_PowersOfTen
Text_PowersOfTen:
	.incbin "baserom.gba", 0x000079d0, 0x00000024
	.section .rom.00007a10, "ax"
	.incbin "baserom.gba", 0x00007a10, 0x00000008
	.global Save_HeaderTemplate
Save_HeaderTemplate:
	.incbin "baserom.gba", 0x00007a18, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00007a6c, 0x00000014
	.section .rom.00007ac8, "ax"
	.incbin "baserom.gba", 0x00007ac8, 0x00000024
	.section .rom.00007b04, "ax"
	.incbin "baserom.gba", 0x00007b04, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00007b1c, 0x00000058
	.section .rom.00007b98, "ax"
	.incbin "baserom.gba", 0x00007b98, 0x0000008c
	.section .rom.00007c2c, "ax"
	.incbin "baserom.gba", 0x00007c2c, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00007c44, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00007c70, 0x0000002c
	.section .rom.00007cc4, "ax"
	.incbin "baserom.gba", 0x00007cc4, 0x00000b3c
	.section .rom.00008ab8, "ax"
	.incbin "baserom.gba", 0x00008ab8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x00008fb8, 0x00000400
	.section .rom.00009590, "ax"
	.incbin "baserom.gba", 0x00009590, 0x000000ec
	.global Render_DecodeFrameEnd
Render_DecodeFrameEnd:
	.section .rom.000098f8, "ax"
	.incbin "baserom.gba", 0x000098f8, 0x00000284
	.section .rom.00009c94, "ax"
	.incbin "baserom.gba", 0x00009c94, 0x000004e8
	.section .rom.0000a20c, "ax"
	.global Func_0800aa0c
	.type Func_0800aa0c, %function
	.thumb_func
Func_0800aa0c:
	.incbin "baserom.gba", 0x0000a20c, 0x00000668
	.section .rom.0000a966, "ax"
	.incbin "baserom.gba", 0x0000a966, 0x00000002
	.section .rom.0000a968, "ax"
	.global Render_ApplyProjectedPlacement
	.type Render_ApplyProjectedPlacement, %function
	.thumb_func
Render_ApplyProjectedPlacement:
	.incbin "baserom.gba", 0x0000a968, 0x00000220
	.section .rom.0000aeb8, "ax"
	.global ResourceSlot_Load
	.type ResourceSlot_Load, %function
	.thumb_func
ResourceSlot_Load:
	.incbin "baserom.gba", 0x0000aeb8, 0x000000e0
	.section .rom.0000be2c, "ax"
	.global ObjectSystem_UpdateCamera
	.type ObjectSystem_UpdateCamera, %function
	.thumb_func
ObjectSystem_UpdateCamera:
	.incbin "baserom.gba", 0x0000be2c, 0x00000250
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.0000c2cc, "ax"
	.global Object_UpdateAllThumb
	.type Object_UpdateAllThumb, %function
	.thumb_func
Object_UpdateAllThumb:
	.incbin "baserom.gba", 0x0000c2cc, 0x00000664
	.section .rom.0000cb04, "ax"
	.incbin "baserom.gba", 0x0000cb04, 0x0000003c
	.section .rom.0000d2f0, "ax"
	.incbin "baserom.gba", 0x0000d2f0, 0x000001ec
	.section .rom.0000d570, "ax"
	.incbin "baserom.gba", 0x0000d570, 0x000004b0
	.section .rom.0000e3ec, "ax"
	.incbin "baserom.gba", 0x0000e3ec, 0x00000bf0
	.section .rom.0000eff2, "ax"
	.incbin "baserom.gba", 0x0000eff2, 0x000001da
	.section .rom.0000f338, "ax"
	.global Map_LoadLayeredScene
	.type Map_LoadLayeredScene, %function
	.thumb_func
Map_LoadLayeredScene:
	.incbin "baserom.gba", 0x0000f338, 0x00000364
	.section .rom.0000fc24, "ax"
	.global Map_CopyMetatileIndicesRect
	.type Map_CopyMetatileIndicesRect, %function
	.thumb_func
Map_CopyMetatileIndicesRect:
	.incbin "baserom.gba", 0x0000fc24, 0x0000013c
	.section .rom.0000fd60, "ax"
	.global Map_PlayMetatileCopySequence
	.type Map_PlayMetatileCopySequence, %function
	.thumb_func
Map_PlayMetatileCopySequence:
	.incbin "baserom.gba", 0x0000fd60, 0x00000074
	.section .rom.0000fdd4, "ax"
	.global Map_CopyMetatileCellsRect
	.type Map_CopyMetatileCellsRect, %function
	.thumb_func
Map_CopyMetatileCellsRect:
	.incbin "baserom.gba", 0x0000fdd4, 0x00000130
	.section .rom.0000ff88, "ax"
	.global Func_08010788
	.type Func_08010788, %function
	.thumb_func
Func_08010788:
	.incbin "baserom.gba", 0x0000ff88, 0x0000013c
	.section .rom.000100e4, "ax"
	.global Map_WriteLayerCellTile
	.type Map_WriteLayerCellTile, %function
	.thumb_func
Map_WriteLayerCellTile:
	.incbin "baserom.gba", 0x000100e4, 0x00000104
	.section .rom.000101e8, "ax"
	.global Map_InitializePerspectiveScene
	.type Map_InitializePerspectiveScene, %function
	.thumb_func
Map_InitializePerspectiveScene:
	.incbin "baserom.gba", 0x000101e8, 0x00000360
	.section .rom.000107f0, "ax"
	.incbin "baserom.gba", 0x000107f0, 0x000000f0
	.section .rom.00010be4, "ax"
	.global Map_UpdateCurrentTileBlock
	.type Map_UpdateCurrentTileBlock, %function
	.thumb_func
Map_UpdateCurrentTileBlock:
	.incbin "baserom.gba", 0x00010be4, 0x000000bc
	.section .rom.00010ca0, "ax"
	.global Map_UpdateCurrentTileBlockUntilBlocked
	.type Map_UpdateCurrentTileBlockUntilBlocked, %function
	.thumb_func
Map_UpdateCurrentTileBlockUntilBlocked:
	.incbin "baserom.gba", 0x00010ca0, 0x000000c8
	.section .rom.00010e44, "ax"
	.global Map_LoadAreaGraphics
	.type Map_LoadAreaGraphics, %function
	.thumb_func
Map_LoadAreaGraphics:
	.incbin "baserom.gba", 0x00010e44, 0x000000f8
	.section .rom.00010f3c, "ax"
	.global Map_LoadDefaultCellsAndUpdateBlock
	.type Map_LoadDefaultCellsAndUpdateBlock, %function
	.thumb_func
Map_LoadDefaultCellsAndUpdateBlock:
	.incbin "baserom.gba", 0x00010f3c, 0x00000060
	.section .rom.00010f9c, "ax"
	.global MapAnimation_Update
	.type MapAnimation_Update, %function
	.thumb_func
MapAnimation_Update:
	.incbin "baserom.gba", 0x00010f9c, 0x0000010c
	.section .rom.000113f4, "ax"
	.global Func_08011bf4
	.type Func_08011bf4, %function
	.thumb_func
Func_08011bf4:
	.incbin "baserom.gba", 0x000113f4, 0x000000ec
	.section .rom.00011752, "ax"
	.incbin "baserom.gba", 0x00011752, 0x00000002
	.section .rom.00011754, "ax"
	.global Func_08011f54
	.type Func_08011f54, %function
	.thumb_func
Func_08011f54:
	.incbin "baserom.gba", 0x00011754, 0x00000084
	.section .rom.000118dc, "ax"
	.global Func_080120dc
	.type Func_080120dc, %function
	.thumb_func
Func_080120dc:
	.incbin "baserom.gba", 0x000118dc, 0x000000c0
	.section .rom.00011d18, "ax"
	.global Ui_RunIconMonitor
	.type Ui_RunIconMonitor, %function
	.thumb_func
Ui_RunIconMonitor:
	.incbin "baserom.gba", 0x00011d18, 0x000005e0
	.section .rom.0001232c, "ax"
	.incbin "baserom.gba", 0x0001232c, 0x000001f4
	.section .rom.00012720, "ax"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012720, 0x0000022c
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001294c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00012990, 0x000000b0
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x00012a40, 0x0000008c
	.global Data_080132cc
Data_080132cc:
	.incbin "baserom.gba", 0x00012acc, 0x00000030
	.global Curve_LerpWeightTable
Curve_LerpWeightTable:
	.incbin "baserom.gba", 0x00012afc, 0x00000100
	.global Curve_SampleIndexTable
Curve_SampleIndexTable:
	.incbin "baserom.gba", 0x00012bfc, 0x00000140
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.incbin "baserom.gba", 0x00012d3c, 0x00000048
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x00012d84, 0x00000008
	.global ObjectDispatch_DefaultScript
ObjectDispatch_DefaultScript:
	.incbin "baserom.gba", 0x00012d8c, 0x00000004
	.global ObjectDispatch_Table0Script
ObjectDispatch_Table0Script:
	.incbin "baserom.gba", 0x00012d90, 0x00000018
	.global ObjectDispatch_Table1Script
ObjectDispatch_Table1Script:
	.incbin "baserom.gba", 0x00012da8, 0x00000018
	.global ObjectDispatch_Table2Script
ObjectDispatch_Table2Script:
	.incbin "baserom.gba", 0x00012dc0, 0x00000018
	.global ObjectDispatch_Table3Script
ObjectDispatch_Table3Script:
	.incbin "baserom.gba", 0x00012dd8, 0x00000018
	.global ObjectDispatch_Table4Script
ObjectDispatch_Table4Script:
	.incbin "baserom.gba", 0x00012df0, 0x00000018
	.global ObjectDispatch_Table5Script
ObjectDispatch_Table5Script:
	.incbin "baserom.gba", 0x00012e08, 0x00000018
	.global ObjectDispatch_Table6Script
ObjectDispatch_Table6Script:
	.incbin "baserom.gba", 0x00012e20, 0x000000c0
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x00012ee0, 0x00001120
	.section .rom.000145d0, "ax"
	.incbin "baserom.gba", 0x000145d0, 0x00000508
	.section .rom.000154ae, "ax"
	.incbin "baserom.gba", 0x000154ae, 0x00000002
	.section .rom.000154b0, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x000154b0, 0x00000098
	.section .rom.000157ba, "ax"
	.incbin "baserom.gba", 0x000157ba, 0x0000008a
	.section .rom.000158d0, "ax"
	.global UiWork_StepChannelScript
	.type UiWork_StepChannelScript, %function
	.thumb_func
UiWork_StepChannelScript:
	.incbin "baserom.gba", 0x000158d0, 0x0000062c
	.section .rom.00016a34, "ax"
	.incbin "baserom.gba", 0x00016a34, 0x00000028
	.section .rom.00016a5c, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x00016a5c, 0x00000190
	.section .rom.00016c5a, "ax"
	.incbin "baserom.gba", 0x00016c5a, 0x0000012e
	.section .rom.00016e3c, "ax"
	.incbin "baserom.gba", 0x00016e3c, 0x000001a4
	.section .rom.00016fe0, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.global Func_08018038
Func_08018038:
	.incbin "baserom.gba", 0x00016fe0, 0x00000660
	.section .rom.00017640, "ax"
	.global UiWindow_FitOnScreen
	.type UiWindow_FitOnScreen, %function
	.thumb_func
UiWindow_FitOnScreen:
	.incbin "baserom.gba", 0x00017640, 0x000000d8
	.section .rom.000177d8, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x000177d8, 0x000006ac
	.section .rom.00018014, "ax"
	.incbin "baserom.gba", 0x00018014, 0x00000140
	.section .rom.00018154, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x00018154, 0x00000480
	.section .rom.0001864c, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0001864c, 0x00000148
	.section .rom.00018bdc, "ax"
	.incbin "baserom.gba", 0x00018bdc, 0x00000110
	.section .rom.00019068, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x00019068, 0x0000021c
	.section .rom.000194dc, "ax"
	.global UiGlyph_LoadEntryWithPalette
	.type UiGlyph_LoadEntryWithPalette, %function
	.thumb_func
UiGlyph_LoadEntryWithPalette:
	.incbin "baserom.gba", 0x000194dc, 0x000000a4
	.section .rom.000197d4, "ax"
	.global MenuSelection_BuildEntries
	.type MenuSelection_BuildEntries, %function
	.thumb_func
MenuSelection_BuildEntries:
	.incbin "baserom.gba", 0x000197d4, 0x00000118
	.section .rom.0001996c, "ax"
	.global MenuSelection_DrawFrame
	.type MenuSelection_DrawFrame, %function
	.thumb_func
MenuSelection_DrawFrame:
	.incbin "baserom.gba", 0x0001996c, 0x00000560
	.section .rom.0001a228, "ax"
	.global Menu_SetupSelectionSide
	.type Menu_SetupSelectionSide, %function
	.thumb_func
Menu_SetupSelectionSide:
	.incbin "baserom.gba", 0x0001a228, 0x00000124
	.section .rom.0001aa48, "ax"
	.global Menu_ScrollSelectionList
	.type Menu_ScrollSelectionList, %function
	.thumb_func
Menu_ScrollSelectionList:
	.incbin "baserom.gba", 0x0001aa48, 0x000001cc
	.section .rom.0001ae60, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0001ae60, 0x00000244
	.section .rom.0001b166, "ax"
	.incbin "baserom.gba", 0x0001b166, 0x00000002
	.section .rom.0001b168, "ax"
	.global Menu_LoadSelectedResource
	.type Menu_LoadSelectedResource, %function
	.thumb_func
Menu_LoadSelectedResource:
	.incbin "baserom.gba", 0x0001b168, 0x00000094
	.section .rom.0001b224, "ax"
	.global Menu_RunTopSelection
	.type Menu_RunTopSelection, %function
	.thumb_func
Menu_RunTopSelection:
	.incbin "baserom.gba", 0x0001b224, 0x000000bc
	.section .rom.0001b2e0, "ax"
	.global UiWindow_OpenMode1AndWaitFrame
	.type UiWindow_OpenMode1AndWaitFrame, %function
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	.incbin "baserom.gba", 0x0001b2e0, 0x00000014
	.section .rom.0001b4ac, "ax"
	.global Debug_SelectAbilityPair
	.type Debug_SelectAbilityPair, %function
	.thumb_func
Debug_SelectAbilityPair:
	.global Menu_Check
	.type Menu_Check, %function
	.thumb_func
Menu_Check:
	.incbin "baserom.gba", 0x0001b4ac, 0x00000360
	.section .rom.0001bf56, "ax"
	.incbin "baserom.gba", 0x0001bf56, 0x00000002
	.section .rom.0001bf58, "ax"
	.global GraphicsPalette_LoadSelectionResourcesAndAdvance
	.type GraphicsPalette_LoadSelectionResourcesAndAdvance, %function
	.thumb_func
GraphicsPalette_LoadSelectionResourcesAndAdvance:
	.incbin "baserom.gba", 0x0001bf58, 0x000000cc
	.section .rom.0001c9e4, "ax"
	.global Menu_CreateWorkspaceWindows
	.type Menu_CreateWorkspaceWindows, %function
	.thumb_func
Menu_CreateWorkspaceWindows:
	.incbin "baserom.gba", 0x0001c9e4, 0x0000019c
	.section .rom.0001cd38, "ax"
	.incbin "baserom.gba", 0x0001cd38, 0x00000134
	.section .rom.0001ce6c, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0001ce6c, 0x00000410
	.section .rom.0001d334, "ax"
	.global UiWindow_MarkVisibleTileAttributes
	.type UiWindow_MarkVisibleTileAttributes, %function
	.thumb_func
UiWindow_MarkVisibleTileAttributes:
	.incbin "baserom.gba", 0x0001d334, 0x000000b0
	.section .rom.0001d768, "ax"
	.global UiText_DrawResource
	.type UiText_DrawResource, %function
	.thumb_func
UiText_DrawResource:
	.incbin "baserom.gba", 0x0001d768, 0x00000074
	.section .rom.0001d7dc, "ax"
	.global UiText_DrawCharacterAtOffset
	.type UiText_DrawCharacterAtOffset, %function
	.thumb_func
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x0001d7dc, 0x00000098
	.section .rom.0001d874, "ax"
	.global UiText_DrawString
	.type UiText_DrawString, %function
	.thumb_func
UiText_DrawString:
	.incbin "baserom.gba", 0x0001d874, 0x00000058
	.section .rom.0001d8cc, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x0001d8cc, 0x00000090
	.section .rom.0001d95c, "ax"
	.global UiText_DrawStringInWindow
	.type UiText_DrawStringInWindow, %function
	.thumb_func
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x0001d95c, 0x00000060
	.section .rom.0001d9bc, "ax"
	.global UiText_DrawNumber
	.type UiText_DrawNumber, %function
	.thumb_func
UiText_DrawNumber:
	.incbin "baserom.gba", 0x0001d9bc, 0x00000034
	.section .rom.0001d9f0, "ax"
	.global UiText_DrawNumberAtOffset
	.type UiText_DrawNumberAtOffset, %function
	.thumb_func
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x0001d9f0, 0x00000034
	.section .rom.0001da24, "ax"
	.global UiText_DrawNumberInWindow
	.type UiText_DrawNumberInWindow, %function
	.thumb_func
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x0001da24, 0x000000d4
	.section .rom.0001de06, "ax"
	.incbin "baserom.gba", 0x0001de06, 0x00000002
	.section .rom.0001de08, "ax"
	.global UiWindow_FillFromScene
	.type UiWindow_FillFromScene, %function
	.thumb_func
UiWindow_FillFromScene:
	.incbin "baserom.gba", 0x0001de08, 0x0000007c
	.section .rom.0001df84, "ax"
	.incbin "baserom.gba", 0x0001df84, 0x00000298
	.section .rom.0001e21c, "ax"
	.global UiWindow_DrawPartyStatusContents
	.type UiWindow_DrawPartyStatusContents, %function
	.thumb_func
UiWindow_DrawPartyStatusContents:
	.incbin "baserom.gba", 0x0001e21c, 0x000003d4
	.section .rom.0001e720, "ax"
	.incbin "baserom.gba", 0x0001e720, 0x0000002c
	.section .rom.0001e74c, "ax"
	.global SaveState_CountRecordsExcludingFlagged
	.type SaveState_CountRecordsExcludingFlagged, %function
	.thumb_func
SaveState_CountRecordsExcludingFlagged:
	.incbin "baserom.gba", 0x0001e74c, 0x0000004c
	.section .rom.0001e798, "ax"
	.global SaveState_ScanRecordFlags
	.type SaveState_ScanRecordFlags, %function
	.thumb_func
SaveState_ScanRecordFlags:
	.incbin "baserom.gba", 0x0001e798, 0x0000009c
	.section .rom.0001e9d0, "ax"
	.global SaveState_WriteCurrentSlotPair
	.type SaveState_WriteCurrentSlotPair, %function
	.thumb_func
SaveState_WriteCurrentSlotPair:
	.incbin "baserom.gba", 0x0001e9d0, 0x00000088
	.section .rom.0001ea58, "ax"
	.global SaveState_WriteSlotPair
	.type SaveState_WriteSlotPair, %function
	.thumb_func
SaveState_WriteSlotPair:
	.incbin "baserom.gba", 0x0001ea58, 0x0000006c
	.section .rom.0001eb64, "ax"
	.incbin "baserom.gba", 0x0001eb64, 0x00000060
	.section .rom.0001ebc4, "ax"
	.global SaveState_CopySlotPair
	.type SaveState_CopySlotPair, %function
	.thumb_func
SaveState_CopySlotPair:
	.incbin "baserom.gba", 0x0001ebc4, 0x000000dc
	.section .rom.0001eca0, "ax"
	.global SaveState_DeleteSelectedSlot
	.type SaveState_DeleteSelectedSlot, %function
	.thumb_func
SaveState_DeleteSelectedSlot:
	.incbin "baserom.gba", 0x0001eca0, 0x000000b0
	.section .rom.0001f1b4, "ax"
	.incbin "baserom.gba", 0x0001f1b4, 0x0000062c
	.section .rom.0001f7e0, "ax"
	.global Save_WriteSelectedSlot
	.type Save_WriteSelectedSlot, %function
	.thumb_func
Save_WriteSelectedSlot:
	.incbin "baserom.gba", 0x0001f7e0, 0x00000120
	.section .rom.0001f900, "ax"
	.global SaveState_LoadRecordIntoWork
	.type SaveState_LoadRecordIntoWork, %function
	.thumb_func
SaveState_LoadRecordIntoWork:
	.incbin "baserom.gba", 0x0001f900, 0x000000cc
	.section .rom.0001fbf2, "ax"
	.incbin "baserom.gba", 0x0001fbf2, 0x00000002
	.section .rom.0001fbf4, "ax"
	.global NameEntry_EditOwnerName
	.type NameEntry_EditOwnerName, %function
	.thumb_func
NameEntry_EditOwnerName:
	.incbin "baserom.gba", 0x0001fbf4, 0x000004b4
	.section .rom.000200a8, "ax"
	.global PartyTalkMenu_Choose
	.type PartyTalkMenu_Choose, %function
	.thumb_func
PartyTalkMenu_Choose:
	.incbin "baserom.gba", 0x000200a8, 0x000001bc
	.section .rom.00020264, "ax"
	.global Djinn_ShowJoinedMessage
	.type Djinn_ShowJoinedMessage, %function
	.thumb_func
Djinn_ShowJoinedMessage:
	.incbin "baserom.gba", 0x00020264, 0x00000138
	.section .rom.000203cc, "ax"
	.global Party_ShowJoinedMessage
	.type Party_ShowJoinedMessage, %function
	.thumb_func
Party_ShowJoinedMessage:
	.incbin "baserom.gba", 0x000203cc, 0x000000f8
	.section .rom.000204c4, "ax"
	.global Party_ShowPairJoinedMessage
	.type Party_ShowPairJoinedMessage, %function
	.thumb_func
Party_ShowPairJoinedMessage:
	.incbin "baserom.gba", 0x000204c4, 0x00000158
	.section .rom.00020724, "ax"
	.global RenderResource_LoadFrame
	.type RenderResource_LoadFrame, %function
	.thumb_func
RenderResource_LoadFrame:
	.incbin "baserom.gba", 0x00020724, 0x00000068
	.section .rom.0002078c, "ax"
	.global RenderResource_CreateFrame
	.type RenderResource_CreateFrame, %function
	.thumb_func
RenderResource_CreateFrame:
	.incbin "baserom.gba", 0x0002078c, 0x00000054
	.section .rom.00020c70, "ax"
	.global Link_CreateCountdownLabelWindow
	.type Link_CreateCountdownLabelWindow, %function
	.thumb_func
Link_CreateCountdownLabelWindow:
	.incbin "baserom.gba", 0x00020c70, 0x00000030
	.section .rom.00020ca0, "ax"
	.global Resource_LoadIndexedIntoBuffer
	.type Resource_LoadIndexedIntoBuffer, %function
	.thumb_func
Resource_LoadIndexedIntoBuffer:
	.incbin "baserom.gba", 0x00020ca0, 0x00000054
	.section .rom.00020cf4, "ax"
	.global UiText_LoadRemappedGlyph
	.type UiText_LoadRemappedGlyph, %function
	.thumb_func
UiText_LoadRemappedGlyph:
	.incbin "baserom.gba", 0x00020cf4, 0x000000d0
	.section .rom.00020ea6, "ax"
	.incbin "baserom.gba", 0x00020ea6, 0x000008fe
	.section .rom.00021858, "ax"
	.global BattleLayout_HighlightPartyPanels
	.type BattleLayout_HighlightPartyPanels, %function
	.thumb_func
BattleLayout_HighlightPartyPanels:
	.incbin "baserom.gba", 0x00021858, 0x000000a0
	.section .rom.00021ab6, "ax"
	.incbin "baserom.gba", 0x00021ab6, 0x000027b2
	.section .rom.000242e8, "ax"
	.incbin "baserom.gba", 0x000242e8, 0x00001d58
	.section .rom.000262d4, "ax"
	.global Battle_CollectPartyCommands
	.type Battle_CollectPartyCommands, %function
	.thumb_func
Battle_CollectPartyCommands:
	.incbin "baserom.gba", 0x000262d4, 0x00001080
	.section .rom.00027354, "ax"
	.global AffineEffect_UpdateFrame
	.type AffineEffect_UpdateFrame, %function
	.thumb_func
AffineEffect_UpdateFrame:
	.incbin "baserom.gba", 0x00027354, 0x00000348
	.section .rom.00027734, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x00027734, 0x0000012c
	.section .rom.00027860, "ax"
	.global Menu_SelectResource
	.type Menu_SelectResource, %function
	.thumb_func
Menu_SelectResource:
	.incbin "baserom.gba", 0x00027860, 0x00000108
	.section .rom.00027968, "ax"
	.global Menu_AppendResourceEntry
	.type Menu_AppendResourceEntry, %function
	.thumb_func
Menu_AppendResourceEntry:
	.incbin "baserom.gba", 0x00027968, 0x00000060
	.section .rom.00027dc4, "ax"
	.global Menu_SelectResourceLayout
	.type Menu_SelectResourceLayout, %function
	.thumb_func
Menu_SelectResourceLayout:
	.incbin "baserom.gba", 0x00027dc4, 0x00000170
	.section .rom.00028014, "ax"
	.global Menu_RunConfirmSelectionAt
	.type Menu_RunConfirmSelectionAt, %function
	.thumb_func
Menu_RunConfirmSelectionAt:
	.incbin "baserom.gba", 0x00028014, 0x00000054
	.section .rom.00028714, "ax"
	.global DebugMenu_BrowseIcons
	.type DebugMenu_BrowseIcons, %function
	.thumb_func
DebugMenu_BrowseIcons:
	.incbin "baserom.gba", 0x00028714, 0x00000228
	.section .rom.0002893c, "ax"
	.global DebugMenu_BrowseEntryGlyphs
	.type DebugMenu_BrowseEntryGlyphs, %function
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	.incbin "baserom.gba", 0x0002893c, 0x00000194
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00028ad0, 0x00000100
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.global RomBytes_08029a10
RomBytes_08029a10:
	.incbin "baserom.gba", 0x00028bd0, 0x000003f0
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00028fc0, 0x000000e4
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x000290a4, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x000294a0, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002d048, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002d2c8, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002fa60, 0x00000804
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x00030264, 0x00000740
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x000309a4, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00030a24, 0x00000690
	.global UiText_SecondGlyphs
UiText_SecondGlyphs:
	.incbin "baserom.gba", 0x000310b4, 0x00000400
	.section .rom.000330b4, "ax"
	.incbin "baserom.gba", 0x000330b4, 0x0000010c
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x000331c0, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x000335c0, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x000339c0, 0x00002058
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00035a18, 0x00000079
	.global Data_080367c9
Data_080367c9:
	.incbin "baserom.gba", 0x00035a91, 0x00000003
	.global Data_080367cc
Data_080367cc:
	.incbin "baserom.gba", 0x00035a94, 0x00000002
	.global Data_080367ce
Data_080367ce:
	.incbin "baserom.gba", 0x00035a96, 0x00000002
	.global Data_080367d0
Data_080367d0:
	.incbin "baserom.gba", 0x00035a98, 0x00000006
	.global Data_080367d6
Data_080367d6:
	.incbin "baserom.gba", 0x00035a9e, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x00035aa4, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x00035aac, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x00035ad4, 0x00000a04
	.global Party_CharacterValues
Party_CharacterValues:
	.incbin "baserom.gba", 0x000364d8, 0x00000010
	.global Party_CharacterValuesFlag32
Party_CharacterValuesFlag32:
	.incbin "baserom.gba", 0x000364e8, 0x00000010
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
	.incbin "baserom.gba", 0x000364f8, 0x00000008
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.incbin "baserom.gba", 0x00036500, 0x00000020
	.global Data_08037250
Data_08037250:
	.incbin "baserom.gba", 0x00036520, 0x00000030
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "baserom.gba", 0x00036550, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x00036590, 0x00000147
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x000366d7, 0x0000000c
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x000366e3, 0x0000000c
	.global Menu_SaveSlotActionByPosition
Menu_SaveSlotActionByPosition:
	.incbin "baserom.gba", 0x000366ef, 0x00000019
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x00036708, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x0003670c, 0x00000038
	.section .rom.00073368, "ax"
	.incbin "baserom.gba", 0x00073368, 0x0000000a
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00073372, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x000733b4, 0x00000120
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000734d4, 0x00008b2c
	.section .rom.0007c000, "ax"
	.global Trade_GetOfferStateFar
	.type Trade_GetOfferStateFar, %function
	.thumb_func
Trade_GetOfferStateFar:
	.incbin "baserom.gba", 0x0007c000, 0x00000008
	.section .rom.0007c008, "ax"
	.global Runtime_GetObject
	.type Runtime_GetObject, %function
	.thumb_func
Runtime_GetObject:
	.global Owner_GetStateFar
	.type Owner_GetStateFar, %function
	.thumb_func
Owner_GetStateFar:
	.incbin "baserom.gba", 0x0007c008, 0x00000008
	.section .rom.0007c010, "ax"
	.global Owner_RecalculateStatsFar
	.type Owner_RecalculateStatsFar, %function
	.thumb_func
Owner_RecalculateStatsFar:
	.global BattleUnit_Recalculate
	.type BattleUnit_Recalculate, %function
	.thumb_func
BattleUnit_Recalculate:
	.incbin "baserom.gba", 0x0007c010, 0x00000008
	.section .rom.0007c018, "ax"
	.global Item_Get
	.type Item_Get, %function
	.thumb_func
Item_Get:
	.incbin "baserom.gba", 0x0007c018, 0x00000008
	.section .rom.0007c020, "ax"
	.global Shop_GetSelectionState
	.type Shop_GetSelectionState, %function
	.thumb_func
Shop_GetSelectionState:
	.incbin "baserom.gba", 0x0007c020, 0x00000008
	.section .rom.0007c028, "ax"
	.global Inventory_AddItemFar
	.type Inventory_AddItemFar, %function
	.thumb_func
Inventory_AddItemFar:
	.incbin "baserom.gba", 0x0007c028, 0x00000008
	.section .rom.0007c030, "ax"
	.global PartyInventory_AddFar
	.type PartyInventory_AddFar, %function
	.thumb_func
PartyInventory_AddFar:
	.incbin "baserom.gba", 0x0007c030, 0x00000008
	.section .rom.0007c038, "ax"
	.global Item_FindSlot
	.type Item_FindSlot, %function
	.thumb_func
Item_FindSlot:
	.incbin "baserom.gba", 0x0007c038, 0x00000010
	.section .rom.0007c048, "ax"
	.global PartyInventory_RemoveFar
	.type PartyInventory_RemoveFar, %function
	.thumb_func
PartyInventory_RemoveFar:
	.incbin "baserom.gba", 0x0007c048, 0x00000008
	.section .rom.0007c050, "ax"
	.global Inventory_EquipFar
	.type Inventory_EquipFar, %function
	.thumb_func
Inventory_EquipFar:
	.incbin "baserom.gba", 0x0007c050, 0x00000008
	.section .rom.0007c058, "ax"
	.global Inventory_RemoveFar
	.type Inventory_RemoveFar, %function
	.thumb_func
Inventory_RemoveFar:
	.incbin "baserom.gba", 0x0007c058, 0x00000008
	.section .rom.0007c060, "ax"
	.global Inventory_BreakFar
	.type Inventory_BreakFar, %function
	.thumb_func
Inventory_BreakFar:
	.incbin "baserom.gba", 0x0007c060, 0x00000008
	.section .rom.0007c068, "ax"
	.global Func_08077068
	.type Func_08077068, %function
	.thumb_func
Func_08077068:
	.incbin "baserom.gba", 0x0007c068, 0x00000010
	.section .rom.0007c078, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x0007c078, 0x00000008
	.section .rom.0007c080, "ax"
	.global BattleAction_Get
	.type BattleAction_Get, %function
	.thumb_func
BattleAction_Get:
	.global Ability_GetData
	.type Ability_GetData, %function
	.thumb_func
Ability_GetData:
	.incbin "baserom.gba", 0x0007c080, 0x00000008
	.section .rom.0007c088, "ax"
	.global OwnerAction_AddFar
	.type OwnerAction_AddFar, %function
	.thumb_func
OwnerAction_AddFar:
	.incbin "baserom.gba", 0x0007c088, 0x00000008
	.section .rom.0007c090, "ax"
	.global Equipment_HasValueFar
	.type Equipment_HasValueFar, %function
	.thumb_func
Equipment_HasValueFar:
	.incbin "baserom.gba", 0x0007c090, 0x00000028
	.section .rom.0007c0b8, "ax"
	.global Func_080770b8
	.type Func_080770b8, %function
	.thumb_func
Func_080770b8:
	.incbin "baserom.gba", 0x0007c0b8, 0x00000008
	.section .rom.0007c0c0, "ax"
	.global GameFlag_TestFar
	.type GameFlag_TestFar, %function
	.thumb_func
GameFlag_TestFar:
	.global GameFlag_IsSet
	.type GameFlag_IsSet, %function
	.thumb_func
GameFlag_IsSet:
	.incbin "baserom.gba", 0x0007c0c0, 0x00000008
	.section .rom.0007c0c8, "ax"
	.global GameFlag_SetBitFar
	.type GameFlag_SetBitFar, %function
	.thumb_func
GameFlag_SetBitFar:
	.incbin "baserom.gba", 0x0007c0c8, 0x00000008
	.section .rom.0007c0d0, "ax"
	.global GameFlag_ClearBitFar
	.type GameFlag_ClearBitFar, %function
	.thumb_func
GameFlag_ClearBitFar:
	.incbin "baserom.gba", 0x0007c0d0, 0x00000010
	.section .rom.0007c0e0, "ax"
	.global GameFlag_GetByteFar
	.type GameFlag_GetByteFar, %function
	.thumb_func
GameFlag_GetByteFar:
	.incbin "baserom.gba", 0x0007c0e0, 0x00000038
	.section .rom.0007c118, "ax"
	.global Owner_AdjustFirstValueFar
	.type Owner_AdjustFirstValueFar, %function
	.thumb_func
Owner_AdjustFirstValueFar:
	.incbin "baserom.gba", 0x0007c118, 0x00000008
	.section .rom.0007c120, "ax"
	.global Owner_AdjustSecondValueFar
	.type Owner_AdjustSecondValueFar, %function
	.thumb_func
Owner_AdjustSecondValueFar:
	.incbin "baserom.gba", 0x0007c120, 0x00000008
	.section .rom.0007c128, "ax"
	.global Owner_RecalculateRatiosFar
	.type Owner_RecalculateRatiosFar, %function
	.thumb_func
Owner_RecalculateRatiosFar:
	.incbin "baserom.gba", 0x0007c128, 0x00000008
	.section .rom.0007c130, "ax"
	.global Owner_UpdateRatioPairFar
	.type Owner_UpdateRatioPairFar, %function
	.thumb_func
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x0007c130, 0x00000010
	.section .rom.0007c140, "ax"
	.global BattleUnit_AssignFar
	.type BattleUnit_AssignFar, %function
	.thumb_func
BattleUnit_AssignFar:
	.incbin "baserom.gba", 0x0007c140, 0x00000008
	.section .rom.0007c148, "ax"
	.global Party_CountActiveOwnersFar
	.type Party_CountActiveOwnersFar, %function
	.thumb_func
Party_CountActiveOwnersFar:
	.incbin "baserom.gba", 0x0007c148, 0x00000008
	.section .rom.0007c150, "ax"
	.global Party_AddActiveOwnerFar
	.type Party_AddActiveOwnerFar, %function
	.thumb_func
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x0007c150, 0x00000008
	.section .rom.0007c158, "ax"
	.global Party_ListActiveOwnersFar
	.type Party_ListActiveOwnersFar, %function
	.thumb_func
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x0007c158, 0x00000010
	.section .rom.0007c168, "ax"
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x0007c168, 0x00000010
	.section .rom.0007c178, "ax"
	.global Battle_HitCheck
	.type Battle_HitCheck, %function
	.thumb_func
Battle_HitCheck:
	.incbin "baserom.gba", 0x0007c178, 0x00000008
	.section .rom.0007c180, "ax"
	.global Battle_CalcAttack
	.type Battle_CalcAttack, %function
	.thumb_func
Battle_CalcAttack:
	.incbin "baserom.gba", 0x0007c180, 0x00000008
	.section .rom.0007c188, "ax"
	.global Battle_CalcPower
	.type Battle_CalcPower, %function
	.thumb_func
Battle_CalcPower:
	.incbin "baserom.gba", 0x0007c188, 0x00000008
	.section .rom.0007c190, "ax"
	.global Battle_CalcRestore
	.type Battle_CalcRestore, %function
	.thumb_func
Battle_CalcRestore:
	.incbin "baserom.gba", 0x0007c190, 0x00000008
	.section .rom.0007c198, "ax"
	.global Owner_GetRecordFar
	.type Owner_GetRecordFar, %function
	.thumb_func
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x0007c198, 0x00000008
	.section .rom.0007c1a0, "ax"
	.global BattleRandom16Far
	.type BattleRandom16Far, %function
	.thumb_func
BattleRandom16Far:
	.incbin "baserom.gba", 0x0007c1a0, 0x00000008
	.section .rom.0007c1a8, "ax"
	.global Djinn_AddToOwnerFar
	.type Djinn_AddToOwnerFar, %function
	.thumb_func
Djinn_AddToOwnerFar:
	.incbin "baserom.gba", 0x0007c1a8, 0x00000008
	.section .rom.0007c1b0, "ax"
	.global Djinn_ActivateFar
	.type Djinn_ActivateFar, %function
	.thumb_func
Djinn_ActivateFar:
	.incbin "baserom.gba", 0x0007c1b0, 0x00000010
	.section .rom.0007c1c0, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x0007c1c0, 0x00000008
	.section .rom.0007c1c8, "ax"
	.global Trade_AddOfferFar
	.type Trade_AddOfferFar, %function
	.thumb_func
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x0007c1c8, 0x00000018
	.section .rom.0007c1e0, "ax"
	.global SummonDefinition_Get
	.type SummonDefinition_Get, %function
	.thumb_func
SummonDefinition_Get:
	.incbin "baserom.gba", 0x0007c1e0, 0x00000008
	.section .rom.0007c1e8, "ax"
	.global Func_080771e8
	.type Func_080771e8, %function
	.thumb_func
Func_080771e8:
	.incbin "baserom.gba", 0x0007c1e8, 0x00000008
	.section .rom.0007c1f0, "ax"
	.global Func_080771f0
	.type Func_080771f0, %function
	.thumb_func
Func_080771f0:
	.incbin "baserom.gba", 0x0007c1f0, 0x00000020
	.section .rom.0007c210, "ax"
	.global Trade_CanOfferDjinnFar
	.type Trade_CanOfferDjinnFar, %function
	.thumb_func
Trade_CanOfferDjinnFar:
	.incbin "baserom.gba", 0x0007c210, 0x00000008
	.section .rom.0007c218, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x0007c218, 0x00000008
	.section .rom.0007c220, "ax"
	.global Item_IsCompatibleWithOwnerFar
	.type Item_IsCompatibleWithOwnerFar, %function
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x0007c220, 0x00000008
	.section .rom.0007c228, "ax"
	.global Inventory_FindEquippedFar
	.type Inventory_FindEquippedFar, %function
	.thumb_func
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x0007c228, 0x00000008
	.section .rom.0007c230, "ax"
	.global Party_AdjustSixDigitCounterAFar
	.type Party_AdjustSixDigitCounterAFar, %function
	.thumb_func
Party_AdjustSixDigitCounterAFar:
	.incbin "baserom.gba", 0x0007c230, 0x00000008
	.section .rom.0007c238, "ax"
	.global Item_GetEquipmentGroupFar
	.type Item_GetEquipmentGroupFar, %function
	.thumb_func
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x0007c238, 0x00000008
	.section .rom.0007c240, "ax"
	.global Ability_GetMaximum
	.type Ability_GetMaximum, %function
	.thumb_func
Ability_GetMaximum:
	.incbin "baserom.gba", 0x0007c240, 0x00000008
	.section .rom.0007c248, "ax"
	.global Inventory_CountFar
	.type Inventory_CountFar, %function
	.thumb_func
Inventory_CountFar:
	.incbin "baserom.gba", 0x0007c248, 0x00000010
	.section .rom.0007c258, "ax"
	.global Owner_GetLevelThresholdFar
	.type Owner_GetLevelThresholdFar, %function
	.thumb_func
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x0007c258, 0x00000030
	.section .rom.0007c288, "ax"
	.global Djinn_AddToLeastLoadedOwnerFar
	.type Djinn_AddToLeastLoadedOwnerFar, %function
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x0007c288, 0x00000008
	.section .rom.0007c290, "ax"
	.global Party_SumDjinnCountsFar
	.type Party_SumDjinnCountsFar, %function
	.thumb_func
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x0007c290, 0x00000008
	.section .rom.0007c298, "ax"
	.global Party_AdjustSixDigitCounterBFar
	.type Party_AdjustSixDigitCounterBFar, %function
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x0007c298, 0x00000008
	.section .rom.0007c2a0, "ax"
	.global Func_080772a0
	.type Func_080772a0, %function
	.thumb_func
Func_080772a0:
	.incbin "baserom.gba", 0x0007c2a0, 0x00000010
	.section .rom.0007c2b0, "ax"
	.global Func_080772b0
	.type Func_080772b0, %function
	.thumb_func
Func_080772b0:
	.incbin "baserom.gba", 0x0007c2b0, 0x00000008
	.section .rom.0007c2b8, "ax"
	.global BattleFx_IsReviveFar
	.type BattleFx_IsReviveFar, %function
	.thumb_func
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x0007c2b8, 0x00000020
	.section .rom.0007c2d8, "ax"
	.global Inventory_CountItemFar
	.type Inventory_CountItemFar, %function
	.thumb_func
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x0007c2d8, 0x00000018
	.section .rom.0007c2f0, "ax"
	.global GameFlag_RefreshLureCapFar
	.type GameFlag_RefreshLureCapFar, %function
	.thumb_func
GameFlag_RefreshLureCapFar:
	.incbin "baserom.gba", 0x0007c2f0, 0x00000010
	.section .rom.0007c300, "ax"
	.global Runtime_GetBuildStampTimeFar
	.type Runtime_GetBuildStampTimeFar, %function
	.thumb_func
Runtime_GetBuildStampTimeFar:
	.incbin "baserom.gba", 0x0007c300, 0x00000020
	.section .rom.0007cc10, "ax"
	.incbin "baserom.gba", 0x0007cc10, 0x00000128
	.section .rom.0007cd38, "ax"
	.global GameState_InitDefaults
	.type GameState_InitDefaults, %function
	.thumb_func
GameState_InitDefaults:
	.incbin "baserom.gba", 0x0007cd38, 0x00000208
	.section .rom.0007dbf0, "ax"
	.global Owner_RefreshClassActions
	.type Owner_RefreshClassActions, %function
	.thumb_func
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x0007dbf0, 0x00000238
	.section .rom.0007e460, "ax"
	.incbin "baserom.gba", 0x0007e460, 0x0000019c
	.section .rom.0007eb24, "ax"
	.global Curve_LookupScaledValue
	.type Curve_LookupScaledValue, %function
	.thumb_func
Curve_LookupScaledValue:
	.incbin "baserom.gba", 0x0007eb24, 0x000000a0
	.section .rom.0007f664, "ax"
	.incbin "baserom.gba", 0x0007f664, 0x0000013c
	.section .rom.0007f828, "ax"
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.incbin "baserom.gba", 0x0007f828, 0x00000008
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "baserom.gba", 0x0007f830, 0x00000c60
	.global Item_ArtifactSlotTable
Item_ArtifactSlotTable:
	.incbin "baserom.gba", 0x00080490, 0x00000200
	.global Character_StartingEquipOwnerIds
Character_StartingEquipOwnerIds:
	.incbin "baserom.gba", 0x00080690, 0x00000018
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000806a8, 0x000037b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x00083e58, 0x00002070
	.global Data_08080ec8
Data_08080ec8:
	.incbin "baserom.gba", 0x00085ec8, 0x00003624
	.global Character_DefinitionTable
Character_DefinitionTable:
	.incbin "baserom.gba", 0x000894ec, 0x000005a0
	.global Summon_OrderList
Summon_OrderList:
	.incbin "baserom.gba", 0x00089a8c, 0x00000010
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x00089a9c, 0x00000080
	.global Class_DefinitionTable
Class_DefinitionTable:
	.incbin "baserom.gba", 0x00089b1c, 0x0000429c
	.global Data_08088db8
Data_08088db8:
	.incbin "baserom.gba", 0x0008ddb8, 0x00000040
	.global Element_PowerResistByLevel
Element_PowerResistByLevel:
	.incbin "baserom.gba", 0x0008ddf8, 0x00000040
	.global Enemy_ElementPresetTable
Enemy_ElementPresetTable:
	.incbin "baserom.gba", 0x0008de38, 0x00000434
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008e26c, 0x00000d94
	.section .rom.0008f5f8, "ax"
	.incbin "baserom.gba", 0x0008f5f8, 0x00000008
	.section .rom.0008f6ec, "ax"
	.incbin "baserom.gba", 0x0008f6ec, 0x000001ec
	.section .rom.0008f8ec, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x0008f8ec, 0x00000264
	.section .rom.0008fe7c, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x0008fe7c, 0x000001b8
	.section .rom.00090160, "ax"
	.global BattleFx_FindConditionResource
	.type BattleFx_FindConditionResource, %function
	.thumb_func
BattleFx_FindConditionResource:
	.incbin "baserom.gba", 0x00090160, 0x00000080
	.section .rom.000902b8, "ax"
	.global BattleFx_SelectResultPointer
	.type BattleFx_SelectResultPointer, %function
	.thumb_func
BattleFx_SelectResultPointer:
	.incbin "baserom.gba", 0x000902b8, 0x00000070
	.section .rom.000903f2, "ax"
	.incbin "baserom.gba", 0x000903f2, 0x00000002
	.section .rom.000903f4, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000903f4, 0x00000260
	.section .rom.00090b34, "ax"
	.global ObjectTable_Restore
	.type ObjectTable_Restore, %function
	.thumb_func
ObjectTable_Restore:
	.incbin "baserom.gba", 0x00090b34, 0x00000118
	.section .rom.00091500, "ax"
	.global Func_0808c4f8
	.type Func_0808c4f8, %function
	.thumb_func
Func_0808c4f8:
	.incbin "baserom.gba", 0x00091500, 0x0000097c
	.section .rom.00092460, "ax"
	.incbin "baserom.gba", 0x00092460, 0x00000034
	.section .rom.00092494, "ax"
	.global BattleFx_FindDescriptor
	.type BattleFx_FindDescriptor, %function
	.thumb_func
BattleFx_FindDescriptor:
	.incbin "baserom.gba", 0x00092494, 0x00000118
	.section .rom.000925ac, "ax"
	.global BattleFx_FindDescriptorWithOverride
	.type BattleFx_FindDescriptorWithOverride, %function
	.thumb_func
BattleFx_FindDescriptorWithOverride:
	.incbin "baserom.gba", 0x000925ac, 0x0000034c
	.section .rom.000929ac, "ax"
	.incbin "baserom.gba", 0x000929ac, 0x00000414
	.section .rom.00093154, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x00093154, 0x00000368
	.section .rom.000939c8, "ax"
	.incbin "baserom.gba", 0x000939c8, 0x00000254
	.section .rom.00093e14, "ax"
	.global BattleFx_EmitRandomParticle
	.type BattleFx_EmitRandomParticle, %function
	.thumb_func
BattleFx_EmitRandomParticle:
	.incbin "baserom.gba", 0x00093e14, 0x000000d8
	.section .rom.00094294, "ax"
	.incbin "baserom.gba", 0x00094294, 0x00000048
	.section .rom.000942dc, "ax"
	.global BattleFx_SpawnRandomParticleAtPosition
	.type BattleFx_SpawnRandomParticleAtPosition, %function
	.thumb_func
BattleFx_SpawnRandomParticleAtPosition:
	.incbin "baserom.gba", 0x000942dc, 0x00000094
	.section .rom.00094598, "ax"
	.global DisplayTransition_UpdateScanlineTable
	.type DisplayTransition_UpdateScanlineTable, %function
	.thumb_func
DisplayTransition_UpdateScanlineTable:
	.incbin "baserom.gba", 0x00094598, 0x0000090c
	.section .rom.00094f68, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x00094f68, 0x000002c4
	.section .rom.000956c2, "ax"
	.incbin "baserom.gba", 0x000956c2, 0x00000002
	.section .rom.000956c4, "ax"
	.global DisplayTransition_UpdateFrame
	.type DisplayTransition_UpdateFrame, %function
	.thumb_func
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000956c4, 0x00000158
	.section .rom.00095ac8, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x00095ac8, 0x00000718
	.section .rom.00096324, "ax"
	.global Object_EffectSpawnCallback
	.type Object_EffectSpawnCallback, %function
	.thumb_func
Object_EffectSpawnCallback:
	.incbin "baserom.gba", 0x00096324, 0x000001dc
	.section .rom.00096ac4, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x00096ac4, 0x000001e4
	.section .rom.00096f3c, "ax"
	.global BattleFx_SetWeightedResult
	.type BattleFx_SetWeightedResult, %function
	.thumb_func
BattleFx_SetWeightedResult:
	.incbin "baserom.gba", 0x00096f3c, 0x00000064
	.section .rom.00096fa0, "ax"
	.global BattleFx_SetPhaseRequest
	.type BattleFx_SetPhaseRequest, %function
	.thumb_func
BattleFx_SetPhaseRequest:
	.incbin "baserom.gba", 0x00096fa0, 0x0000007c
	.section .rom.00097ccc, "ax"
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x00097ccc, 0x00000344
	.section .rom.0009902c, "ax"
	.global battle_owner_69
	.type battle_owner_69, %function
	.thumb_func
battle_owner_69:
	.incbin "baserom.gba", 0x0009902c, 0x000001b4
	.section .rom.0009926a, "ax"
	.incbin "baserom.gba", 0x0009926a, 0x00000102
	.section .rom.000995d0, "ax"
	.global DisplayScroll_BuildAndSwapHBlankPage
	.type DisplayScroll_BuildAndSwapHBlankPage, %function
	.thumb_func
DisplayScroll_BuildAndSwapHBlankPage:
	.incbin "baserom.gba", 0x000995d0, 0x000001ec
	.section .rom.000998ac, "ax"
	.incbin "baserom.gba", 0x000998ac, 0x00000188
	.section .rom.00099b54, "ax"
	.global Unnamed_08094ac8
	.type Unnamed_08094ac8, %function
	.thumb_func
Unnamed_08094ac8:
	.incbin "baserom.gba", 0x00099b54, 0x000002d8
	.section .rom.00099e2c, "ax"
	.global FieldMotes_Start
	.type FieldMotes_Start, %function
	.thumb_func
FieldMotes_Start:
	.incbin "baserom.gba", 0x00099e2c, 0x000000dc
	.section .rom.0009ac94, "ax"
	.global BattleEffect_UpdatePhasedRadialParticle
	.type BattleEffect_UpdatePhasedRadialParticle, %function
	.thumb_func
BattleEffect_UpdatePhasedRadialParticle:
	.incbin "baserom.gba", 0x0009ac94, 0x000001c8
	.section .rom.0009c03c, "ax"
	.global BattleFx_LoadActionEffectResources
	.type BattleFx_LoadActionEffectResources, %function
	.thumb_func
BattleFx_LoadActionEffectResources:
	.incbin "baserom.gba", 0x0009c03c, 0x00000148
	.section .rom.0009c6d0, "ax"
	.global Func_08097644
	.type Func_08097644, %function
	.thumb_func
Func_08097644:
	.incbin "baserom.gba", 0x0009c6d0, 0x00000224
	.section .rom.0009ccc8, "ax"
	.global FunctionHead_08097c3c
	.type FunctionHead_08097c3c, %function
	.thumb_func
FunctionHead_08097c3c:
	.incbin "baserom.gba", 0x0009ccc8, 0x00000344
	.section .rom.0009d42c, "ax"
	.global RunBattleEffect08
	.type RunBattleEffect08, %function
	.thumb_func
RunBattleEffect08:
	.incbin "baserom.gba", 0x0009d42c, 0x0000012c
	.section .rom.0009e566, "ax"
	.incbin "baserom.gba", 0x0009e566, 0x00000002
	.section .rom.0009e568, "ax"
	.global RunBattleEffect03
	.type RunBattleEffect03, %function
	.thumb_func
RunBattleEffect03:
	.incbin "baserom.gba", 0x0009e568, 0x000001a8
	.section .rom.0009ea86, "ax"
	.incbin "baserom.gba", 0x0009ea86, 0x00000002
	.section .rom.0009ea88, "ax"
	.global RunBattleEffect05
	.type RunBattleEffect05, %function
	.thumb_func
RunBattleEffect05:
	.incbin "baserom.gba", 0x0009ea88, 0x00000328
	.section .rom.0009ee3c, "ax"
	.global Battle_unk3_2
	.type Battle_unk3_2, %function
	.thumb_func
Battle_unk3_2:
	.incbin "baserom.gba", 0x0009ee3c, 0x000004f0
	.section .rom.0009fc4c, "ax"
	.global BattleEffect_RunFallbackObjectTransition
	.type BattleEffect_RunFallbackObjectTransition, %function
	.thumb_func
BattleEffect_RunFallbackObjectTransition:
	.incbin "baserom.gba", 0x0009fc4c, 0x000001bc
	.section .rom.0009fefa, "ax"
	.incbin "baserom.gba", 0x0009fefa, 0x00000002
	.section .rom.0009fefc, "ax"
	.global RunBattleEffect13
	.type RunBattleEffect13, %function
	.thumb_func
RunBattleEffect13:
	.incbin "baserom.gba", 0x0009fefc, 0x0000024c
	.section .rom.000a03fc, "ax"
	.global BattleFx_UpdateDescendingParticlePositiveArc
	.type BattleFx_UpdateDescendingParticlePositiveArc, %function
	.thumb_func
BattleFx_UpdateDescendingParticlePositiveArc:
	.incbin "baserom.gba", 0x000a03fc, 0x00000074
	.section .rom.000a0470, "ax"
	.global BattleFx_UpdateDescendingParticleNegativeArc
	.type BattleFx_UpdateDescendingParticleNegativeArc, %function
	.thumb_func
BattleFx_UpdateDescendingParticleNegativeArc:
	.incbin "baserom.gba", 0x000a0470, 0x00000078
	.section .rom.000a0674, "ax"
	.incbin "baserom.gba", 0x000a0674, 0x0000006c
	.section .rom.000a0730, "ax"
	.global RunBattleEffect16
	.type RunBattleEffect16, %function
	.thumb_func
RunBattleEffect16:
	.incbin "baserom.gba", 0x000a0730, 0x0000016c
	.section .rom.000a098c, "ax"
	.global EffectSlot_UpdateMotion
	.type EffectSlot_UpdateMotion, %function
	.thumb_func
EffectSlot_UpdateMotion:
	.incbin "baserom.gba", 0x000a098c, 0x00000140
	.section .rom.000a0d90, "ax"
	.incbin "baserom.gba", 0x000a0d90, 0x000006dc
	.section .rom.000a1568, "ax"
	.incbin "baserom.gba", 0x000a1568, 0x00000200
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x000a1768, 0x00000b60
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x000a22c8, 0x00000638
	.global BattleFx_ResultRules
BattleFx_ResultRules:
	.incbin "baserom.gba", 0x000a2900, 0x00000108
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x000a2a08, 0x00000140
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000a2b48, 0x000007e8
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000a3330, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x000a33c8, 0x00000218
	.global gBattleCueTable
gBattleCueTable:
	.incbin "baserom.gba", 0x000a35e0, 0x00000046
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x000a3626, 0x000001b8
	.global BattleFx_TargetRangeByMode
BattleFx_TargetRangeByMode:
	.incbin "baserom.gba", 0x000a37de, 0x00000032
	.global Animation_ChildPaletteCycle
Animation_ChildPaletteCycle:
	.incbin "baserom.gba", 0x000a3810, 0x00000008
	.global BattleFx_ParticleEmitterScript
BattleFx_ParticleEmitterScript:
	.incbin "baserom.gba", 0x000a3818, 0x0000009c
	.global RomBytes_0809e75c
RomBytes_0809e75c:
	.incbin "baserom.gba", 0x000a38b4, 0x00000120
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x000a39d4, 0x00000024
	.global BattleFx_MarkerParticleScript
BattleFx_MarkerParticleScript:
	.incbin "baserom.gba", 0x000a39f8, 0x00000150
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x000a3b48, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x000a3d54, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x000a3ed8, 0x000002a4
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x000a417c, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x000a41fc, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000a4208, 0x00000024
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x000a422c, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000a4250, 0x00000024
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x000a4274, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000a42b8, 0x00000048
	.section .rom.000a4948, "ax"
	.incbin "baserom.gba", 0x000a4948, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x000a4968, 0x000003bc
	.global ObjectMotion_LaunchScript
ObjectMotion_LaunchScript:
	.incbin "baserom.gba", 0x000a4d24, 0x00000020
	.global BattleFx_BurstParticleScriptA
BattleFx_BurstParticleScriptA:
	.incbin "baserom.gba", 0x000a4d44, 0x00000018
	.global BattleFx_BurstParticleScriptB
BattleFx_BurstParticleScriptB:
	.incbin "baserom.gba", 0x000a4d5c, 0x00000018
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000a4d74, 0x0000000c
	.global Ui_RenderResultValues
Ui_RenderResultValues:
	.incbin "baserom.gba", 0x000a4d80, 0x00000004
	.global BattleFx_LinkedObjectScript
BattleFx_LinkedObjectScript:
	.incbin "baserom.gba", 0x000a4d84, 0x0000010c
	.global Data_0809fd38
Data_0809fd38:
	.incbin "baserom.gba", 0x000a4e90, 0x0000000c
	.global ObjectMotion_ActionKind2Script
ObjectMotion_ActionKind2Script:
	.incbin "baserom.gba", 0x000a4e9c, 0x000000bc
	.global ObjectMotion_ActionKind1Script
ObjectMotion_ActionKind1Script:
	.incbin "baserom.gba", 0x000a4f58, 0x00000004
	.global ObjectMotion_ResetActionScript
ObjectMotion_ResetActionScript:
	.incbin "baserom.gba", 0x000a4f5c, 0x0000000c
	.global ObjectMotion_ActionKind3Script
ObjectMotion_ActionKind3Script:
	.incbin "baserom.gba", 0x000a4f68, 0x000000bc
	.global ObjectMotion_ActionKind4Script
ObjectMotion_ActionKind4Script:
	.incbin "baserom.gba", 0x000a5024, 0x0000004c
	.global ObjectMotion_MoveTowardTargetScript
ObjectMotion_MoveTowardTargetScript:
	.incbin "baserom.gba", 0x000a5070, 0x00000014
	.global ObjectMotion_TurnTowardLinkedScript
ObjectMotion_TurnTowardLinkedScript:
	.incbin "baserom.gba", 0x000a5084, 0x00000014
	.global ObjectMotion_LinkedActionScript
ObjectMotion_LinkedActionScript:
	.incbin "baserom.gba", 0x000a5098, 0x00000178
	.global Data_080a00b8
Data_080a00b8:
	.incbin "baserom.gba", 0x000a5210, 0x00000050
	.global Data_080a0108
Data_080a0108:
	.incbin "baserom.gba", 0x000a5260, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000a5280, 0x00000004
	.global gEffectScripts
gEffectScripts:
	.incbin "baserom.gba", 0x000a5284, 0x0000057c
	.section .rom.000a592c, "ax"
	.global Menu_DrawOwnerStatusPanel
	.type Menu_DrawOwnerStatusPanel, %function
	.thumb_func
Menu_DrawOwnerStatusPanel:
	.incbin "baserom.gba", 0x000a592c, 0x000003b8
	.section .rom.000a5de4, "ax"
	.incbin "baserom.gba", 0x000a5de4, 0x0000013c
	.section .rom.000a62b2, "ax"
	.incbin "baserom.gba", 0x000a62b2, 0x00000002
	.section .rom.000a62b4, "ax"
	.global UiMenu_SlideCursor
	.type UiMenu_SlideCursor, %function
	.thumb_func
UiMenu_SlideCursor:
	.incbin "baserom.gba", 0x000a62b4, 0x00000108
	.section .rom.000a6cc4, "ax"
	.global RunAssetSelectionScreen
	.type RunAssetSelectionScreen, %function
	.thumb_func
RunAssetSelectionScreen:
	.incbin "baserom.gba", 0x000a6cc4, 0x00000de4
	.section .rom.000a80c0, "ax"
	.incbin "baserom.gba", 0x000a80c0, 0x00000338
	.section .rom.000a858c, "ax"
	.incbin "baserom.gba", 0x000a858c, 0x00000044
	.section .rom.000a86e2, "ax"
	.incbin "baserom.gba", 0x000a86e2, 0x00000002
	.section .rom.000a86e4, "ax"
	.global ItemMenu_DrawEquipPreview
	.type ItemMenu_DrawEquipPreview, %function
	.thumb_func
ItemMenu_DrawEquipPreview:
	.incbin "baserom.gba", 0x000a86e4, 0x000001bc
	.section .rom.000a893e, "ax"
	.incbin "baserom.gba", 0x000a893e, 0x00000342
	.section .rom.000a96fc, "ax"
	.incbin "baserom.gba", 0x000a96fc, 0x000002c8
	.section .rom.000a9b7c, "ax"
	.incbin "baserom.gba", 0x000a9b7c, 0x000001ac
	.section .rom.000aa386, "ax"
	.incbin "baserom.gba", 0x000aa386, 0x00000002
	.section .rom.000aa388, "ax"
	.global Menu_OpenConfirmPrompt
	.type Menu_OpenConfirmPrompt, %function
	.thumb_func
Menu_OpenConfirmPrompt:
	.incbin "baserom.gba", 0x000aa388, 0x000004c8
	.section .rom.000aa944, "ax"
	.global PsynergyMenu_SetupActionIcons
	.type PsynergyMenu_SetupActionIcons, %function
	.thumb_func
PsynergyMenu_SetupActionIcons:
	.incbin "baserom.gba", 0x000aa944, 0x000002b0
	.section .rom.000aae84, "ax"
	.incbin "baserom.gba", 0x000aae84, 0x00000180
	.section .rom.000ac014, "ax"
	.global Func_080a77a4
	.type Func_080a77a4, %function
	.thumb_func
Func_080a77a4:
	.global CharacterMenu_SelectOwner
	.type CharacterMenu_SelectOwner, %function
	.thumb_func
CharacterMenu_SelectOwner:
	.incbin "baserom.gba", 0x000ac014, 0x000000ac
	.section .rom.000ace74, "ax"
	.global CharacterMenu_DrawStatusAilments
	.type CharacterMenu_DrawStatusAilments, %function
	.thumb_func
CharacterMenu_DrawStatusAilments:
	.incbin "baserom.gba", 0x000ace74, 0x00000300
	.section .rom.000ad530, "ax"
	.incbin "baserom.gba", 0x000ad530, 0x00000074
	.section .rom.000ad5a4, "ax"
	.global PsynergyMenu_DrawRangePage
	.type PsynergyMenu_DrawRangePage, %function
	.thumb_func
PsynergyMenu_DrawRangePage:
	.incbin "baserom.gba", 0x000ad5a4, 0x00000218
	.section .rom.000ad7bc, "ax"
	.global PsynergyMenu_DrawListPage
	.type PsynergyMenu_DrawListPage, %function
	.thumb_func
PsynergyMenu_DrawListPage:
	.incbin "baserom.gba", 0x000ad7bc, 0x0000017c
	.section .rom.000adc1e, "ax"
	.incbin "baserom.gba", 0x000adc1e, 0x00000002
	.section .rom.000adc20, "ax"
	.global ItemMenu_DrawEquipPage
	.type ItemMenu_DrawEquipPage, %function
	.thumb_func
ItemMenu_DrawEquipPage:
	.incbin "baserom.gba", 0x000adc20, 0x00000200
	.section .rom.000ade20, "ax"
	.global Shop_DrawItemPage
	.type Shop_DrawItemPage, %function
	.thumb_func
Shop_DrawItemPage:
	.incbin "baserom.gba", 0x000ade20, 0x00000140
	.section .rom.000ae798, "ax"
	.global BattleEffect_ApplyToTargets
	.type BattleEffect_ApplyToTargets, %function
	.thumb_func
BattleEffect_ApplyToTargets:
	.incbin "baserom.gba", 0x000ae798, 0x00000538
	.section .rom.000aeff0, "ax"
	.global Func_080aa768
Func_080aa768:
	.incbin "baserom.gba", 0x000aeff0, 0x0000051c
	.section .rom.000af840, "ax"
	.global Func_080aafb8
Func_080aafb8:
	.incbin "baserom.gba", 0x000af840, 0x0000023c
	.section .rom.000afb9c, "ax"
	.incbin "baserom.gba", 0x000afb9c, 0x000015cc
	.section .rom.000b1324, "ax"
	.global DjinnMenu_DrawStatPreview
	.type DjinnMenu_DrawStatPreview, %function
	.thumb_func
DjinnMenu_DrawStatPreview:
	.incbin "baserom.gba", 0x000b1324, 0x000007bc
	.section .rom.000b1c78, "ax"
	.global FourObjectMotion_UpdateBottomRow
	.type FourObjectMotion_UpdateBottomRow, %function
	.thumb_func
FourObjectMotion_UpdateBottomRow:
	.incbin "baserom.gba", 0x000b1c78, 0x000000fc
	.section .rom.000b1f40, "ax"
	.incbin "baserom.gba", 0x000b1f40, 0x00001040
	.section .rom.000b32b8, "ax"
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "baserom.gba", 0x000b32b8, 0x00000100
	.global Data_080aeb4c
Data_080aeb4c:
	.incbin "baserom.gba", 0x000b33b8, 0x00000080
	.global Data_080aebcc
Data_080aebcc:
	.incbin "baserom.gba", 0x000b3438, 0x00000180
	.global Data_080aed4c
Data_080aed4c:
	.incbin "baserom.gba", 0x000b35b8, 0x00000080
	.global Data_080aedcc
Data_080aedcc:
	.incbin "baserom.gba", 0x000b3638, 0x00000444
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x000b3a7c, 0x00000004
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x000b3a80, 0x00000004
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x000b3a84, 0x00000004
	.global Data_080af21c
Data_080af21c:
	.incbin "baserom.gba", 0x000b3a88, 0x00000004
	.global Data_080af220
Data_080af220:
	.incbin "baserom.gba", 0x000b3a8c, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000b3a90, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000b3a94, 0x00000004
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000b3a98, 0x00000004
	.global Data_080af230
Data_080af230:
	.incbin "baserom.gba", 0x000b3a9c, 0x00000004
	.global Data_080af234
Data_080af234:
	.incbin "baserom.gba", 0x000b3aa0, 0x00000004
	.global Data_080af238
Data_080af238:
	.incbin "baserom.gba", 0x000b3aa4, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000b3aa8, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000b3ad8, 0x00000028
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000b3b00, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000b3b09, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x000b3b12, 0x0000000b
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x000b3b1d, 0x0000000b
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x000b3b28, 0x00000014
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x000b3b3c, 0x00000014
	.global ItemMenu_CommandColumnXTable
ItemMenu_CommandColumnXTable:
	.incbin "baserom.gba", 0x000b3b50, 0x00000018
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x000b3b68, 0x00000008
	.global RomBytes_080af304
RomBytes_080af304:
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.incbin "baserom.gba", 0x000b3b70, 0x00000490
	.section .rom.000b4a1e, "ax"
	.incbin "baserom.gba", 0x000b4a1e, 0x00000002
	.section .rom.000b4a20, "ax"
	.global ShopCursor_SetPositionImmediate
	.type ShopCursor_SetPositionImmediate, %function
	.thumb_func
ShopCursor_SetPositionImmediate:
	.incbin "baserom.gba", 0x000b4a20, 0x0000004c
	.section .rom.000b4aac, "ax"
	.global Shop_SelBuy
	.type Shop_SelBuy, %function
	.thumb_func
Shop_SelBuy:
	.incbin "baserom.gba", 0x000b4aac, 0x000004f8
	.section .rom.000b510c, "ax"
	.global Shop_DrawItemPrice
	.type Shop_DrawItemPrice, %function
	.thumb_func
Shop_DrawItemPrice:
	.incbin "baserom.gba", 0x000b510c, 0x00000098
	.section .rom.000b5260, "ax"
	.incbin "baserom.gba", 0x000b5260, 0x00000210
	.section .rom.000b5614, "ax"
	.global Shop_SelectQuantity
	.type Shop_SelectQuantity, %function
	.thumb_func
Shop_SelectQuantity:
	.incbin "baserom.gba", 0x000b5614, 0x000001d0
	.section .rom.000b69a8, "ax"
	.global Shop_ConfirmAct
	.type Shop_ConfirmAct, %function
	.thumb_func
Shop_ConfirmAct:
	.incbin "baserom.gba", 0x000b69a8, 0x00000168
	.section .rom.000b7444, "ax"
	.global Shop_PickUnitItem
	.type Shop_PickUnitItem, %function
	.thumb_func
Shop_PickUnitItem:
	.incbin "baserom.gba", 0x000b7444, 0x000004fc
	.global Shop_HandTiles
Shop_HandTiles:
	.incbin "baserom.gba", 0x000b7940, 0x00000080
	.global Shop_GemTiles
Shop_GemTiles:
	.incbin "baserom.gba", 0x000b79c0, 0x00000080
	.global Shop_SmallDownArrowTiles
Shop_SmallDownArrowTiles:
	.incbin "baserom.gba", 0x000b7a40, 0x00000080
	.global Shop_SmallUpArrowTiles
Shop_SmallUpArrowTiles:
	.incbin "baserom.gba", 0x000b7ac0, 0x00000080
	.global Shop_UpArrowTiles
Shop_UpArrowTiles:
	.incbin "baserom.gba", 0x000b7b40, 0x00000080
	.global Shop_DownArrowTiles
Shop_DownArrowTiles:
	.incbin "baserom.gba", 0x000b7bc0, 0x00000180
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x000b7d40, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x000b7e80, 0x00000280
	.global RomBytes_080b4100
RomBytes_080b4100:
	.incbin "baserom.gba", 0x000b8100, 0x0000003c
	.global RomBytes_080b413c
RomBytes_080b413c:
	.incbin "baserom.gba", 0x000b813c, 0x0000000a
	.global Shop_SpecialItemPrices
Shop_SpecialItemPrices:
	.incbin "baserom.gba", 0x000b8146, 0x00000066
	.global EventTable_AbilityLoadouts
EventTable_AbilityLoadouts:
	.incbin "baserom.gba", 0x000b81ac, 0x00000906
	.global Data_080b4ab2
Data_080b4ab2:
	.incbin "baserom.gba", 0x000b8ab2, 0x00000004
	.global Inn_PriceMultipliers
Inn_PriceMultipliers:
	.incbin "baserom.gba", 0x000b8ab6, 0x0000054a
	.section .rom.000b9204, "ax"
	.incbin "baserom.gba", 0x000b9204, 0x00000008
	.section .rom.000b9218, "ax"
	.incbin "baserom.gba", 0x000b9218, 0x00000040
	.section .rom.000b9534, "ax"
	.incbin "baserom.gba", 0x000b9534, 0x000001ac
	.section .rom.000b96e0, "ax"
	.global Unnamed_080b56e0
	.type Unnamed_080b56e0, %function
	.thumb_func
Unnamed_080b56e0:
	.incbin "baserom.gba", 0x000b96e0, 0x00000184
	.section .rom.000b9f22, "ax"
	.incbin "baserom.gba", 0x000b9f22, 0x00000162
	.section .rom.000ba3e0, "ax"
	.global Battle_RunEncounter
	.type Battle_RunEncounter, %function
	.thumb_func
Battle_RunEncounter:
	.incbin "baserom.gba", 0x000ba3e0, 0x00000698
	.section .rom.000bb5f4, "ax"
	.incbin "baserom.gba", 0x000bb5f4, 0x00000130
	.section .rom.000bb750, "ax"
	.incbin "baserom.gba", 0x000bb750, 0x000001ac
	.section .rom.000bbb82, "ax"
	.incbin "baserom.gba", 0x000bbb82, 0x00000002
	.section .rom.000bbb84, "ax"
	.global BattleActor_SpawnObjectsForList
	.type BattleActor_SpawnObjectsForList, %function
	.thumb_func
BattleActor_SpawnObjectsForList:
	.incbin "baserom.gba", 0x000bbb84, 0x00000264
	.section .rom.000bc58a, "ax"
	.incbin "baserom.gba", 0x000bc58a, 0x0000017a
	.section .rom.000bcc34, "ax"
	.global BattlePres_RunUnitAction
	.type BattlePres_RunUnitAction, %function
	.thumb_func
BattlePres_RunUnitAction:
	.incbin "baserom.gba", 0x000bcc34, 0x0000019c
	.section .rom.000bd56c, "ax"
	.incbin "baserom.gba", 0x000bd56c, 0x000001d0
	.section .rom.000bd73c, "ax"
	.global BattlePresentation_AppendLinkedActions
	.type BattlePresentation_AppendLinkedActions, %function
	.thumb_func
BattlePresentation_AppendLinkedActions:
	.incbin "baserom.gba", 0x000bd73c, 0x00000190
	.section .rom.000bdb46, "ax"
	.incbin "baserom.gba", 0x000bdb46, 0x00000206
	.section .rom.000bdddc, "ax"
	.incbin "baserom.gba", 0x000bdddc, 0x000004b8
	.section .rom.000be2d6, "ax"
	.incbin "baserom.gba", 0x000be2d6, 0x000002c6
	.section .rom.000be6c4, "ax"
	.incbin "baserom.gba", 0x000be6c4, 0x0000026c
	.section .rom.000be98e, "ax"
	.incbin "baserom.gba", 0x000be98e, 0x00000266
	.section .rom.000bec84, "ax"
	.global BattleActor_RemoveFromLists
	.type BattleActor_RemoveFromLists, %function
	.thumb_func
BattleActor_RemoveFromLists:
	.incbin "baserom.gba", 0x000bec84, 0x0000007c
	.section .rom.000bee56, "ax"
	.incbin "baserom.gba", 0x000bee56, 0x0000074a
	.section .rom.000bf674, "ax"
	.global BattlePresentation_WaitForAdvance
	.type BattlePresentation_WaitForAdvance, %function
	.thumb_func
BattlePresentation_WaitForAdvance:
	.incbin "baserom.gba", 0x000bf674, 0x00000164
	.section .rom.000bf7d8, "ax"
	.global Unnamed_080bb7c0
	.type Unnamed_080bb7c0, %function
	.thumb_func
Unnamed_080bb7c0:
	.incbin "baserom.gba", 0x000bf7d8, 0x00000118
	.section .rom.000c143a, "ax"
	.incbin "baserom.gba", 0x000c143a, 0x00000002
	.section .rom.000c143c, "ax"
	.global BattleCommand_SelectAutomatic
	.type BattleCommand_SelectAutomatic, %function
	.thumb_func
BattleCommand_SelectAutomatic:
	.incbin "baserom.gba", 0x000c143c, 0x00000380
	.section .rom.000c1868, "ax"
	.incbin "baserom.gba", 0x000c1868, 0x00000048
	.section .rom.000c18b0, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x000c18b0, 0x00000754
	.section .rom.000c21a2, "ax"
	.incbin "baserom.gba", 0x000c21a2, 0x0000107e
	.section .rom.000c3bbc, "ax"
	.incbin "baserom.gba", 0x000c3bbc, 0x00000414
	.section .rom.000c42bc, "ax"
	.incbin "baserom.gba", 0x000c42bc, 0x0000045c
	.section .rom.000c4904, "ax"
	.global BattleBackground_Load
	.type BattleBackground_Load, %function
	.thumb_func
BattleBackground_Load:
	.incbin "baserom.gba", 0x000c4904, 0x00000138
	.section .rom.000c5488, "ax"
	.incbin "baserom.gba", 0x000c5488, 0x00000260
	.section .rom.000c57ae, "ax"
	.incbin "baserom.gba", 0x000c57ae, 0x00000002
	.section .rom.000c57b0, "ax"
	.global BattleFx_PlayUnitElementEffect
	.type BattleFx_PlayUnitElementEffect, %function
	.thumb_func
BattleFx_PlayUnitElementEffect:
	.incbin "baserom.gba", 0x000c57b0, 0x0000027c
	.section .rom.000c6014, "ax"
	.incbin "baserom.gba", 0x000c6014, 0x0000036c
	.section .rom.000c6508, "ax"
	.global BattleEnemy_RecordDefeat
	.type BattleEnemy_RecordDefeat, %function
	.thumb_func
BattleEnemy_RecordDefeat:
	.incbin "baserom.gba", 0x000c6508, 0x00000234
	.section .rom.000c6a22, "ax"
	.incbin "baserom.gba", 0x000c6a22, 0x00000006
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.incbin "baserom.gba", 0x000c6a28, 0x0000000c
	.global RomBytes_080c2a1c
RomBytes_080c2a1c:
	.incbin "baserom.gba", 0x000c6a34, 0x0000000e
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.incbin "baserom.gba", 0x000c6a42, 0x0000000e
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.incbin "baserom.gba", 0x000c6a50, 0x0000000e
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.incbin "baserom.gba", 0x000c6a5e, 0x0000000e
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.incbin "baserom.gba", 0x000c6a6c, 0x0000000e
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x000c6a7a, 0x0000001a
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.incbin "baserom.gba", 0x000c6a94, 0x0000003c
	.global HitFalloff
HitFalloff:
	.incbin "baserom.gba", 0x000c6ad0, 0x00000008
	.global PpLossFalloff
PpLossFalloff:
	.incbin "baserom.gba", 0x000c6ad8, 0x00000018
	.global HpHealFalloff
HpHealFalloff:
	.incbin "baserom.gba", 0x000c6af0, 0x00000018
	.global PpDmgFalloff
PpDmgFalloff:
	.incbin "baserom.gba", 0x000c6b08, 0x00000018
	.global HpDmgFalloff5
HpDmgFalloff5:
	.incbin "baserom.gba", 0x000c6b20, 0x00000018
	.global HpDmgFalloff8
HpDmgFalloff8:
	.incbin "baserom.gba", 0x000c6b38, 0x00000018
	.global HpDmgFalloff6
HpDmgFalloff6:
	.incbin "baserom.gba", 0x000c6b50, 0x00000018
	.global PpHealFalloff
PpHealFalloff:
	.incbin "baserom.gba", 0x000c6b68, 0x00000018
	.global HpDmgFalloff
HpDmgFalloff:
	.incbin "baserom.gba", 0x000c6b80, 0x00000a54
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.incbin "baserom.gba", 0x000c75d4, 0x00000048
	.global Data_080c3604
Data_080c3604:
	.incbin "baserom.gba", 0x000c761c, 0x0000001c
	.global Data_080c3620
Data_080c3620:
	.incbin "baserom.gba", 0x000c7638, 0x00000008
	.global Data_080c3628
Data_080c3628:
	.incbin "baserom.gba", 0x000c7640, 0x0000090c
	.global Data_080c3f34
Data_080c3f34:
	.incbin "baserom.gba", 0x000c7f4c, 0x00001a04
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.incbin "baserom.gba", 0x000c9950, 0x00000004
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x000c9954, 0x00000068
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.incbin "baserom.gba", 0x000c99bc, 0x00000020
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.incbin "baserom.gba", 0x000c99dc, 0x00000020
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.incbin "baserom.gba", 0x000c99fc, 0x00000020
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.incbin "baserom.gba", 0x000c9a1c, 0x0000002c
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "baserom.gba", 0x000c9a48, 0x000001e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000c9c28, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "baserom.gba", 0x000c9c50, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000cb410, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000cb438, 0x000013c8
	.section .rom.000cc9dc, "ax"
	.incbin "baserom.gba", 0x000cc9dc, 0x00000a84
	.section .rom.000cd4a8, "ax"
	.global BattleFx_RunFiveMode
	.type BattleFx_RunFiveMode, %function
	.thumb_func
BattleFx_RunFiveMode:
	.incbin "baserom.gba", 0x000cd4a8, 0x0000053c
	.section .rom.000cd9fc, "ax"
	.global BattleFx_RunParticlePool
	.type BattleFx_RunParticlePool, %function
	.thumb_func
BattleFx_RunParticlePool:
	.incbin "baserom.gba", 0x000cd9fc, 0x00000380
	.section .rom.000cde0c, "ax"
	.global BattleFx_RunTwelveMode
	.type BattleFx_RunTwelveMode, %function
	.thumb_func
BattleFx_RunTwelveMode:
	.incbin "baserom.gba", 0x000cde0c, 0x000011ec
	.section .rom.000ceff8, "ax"
	.global Unnamed_080cb7f8
	.type Unnamed_080cb7f8, %function
	.thumb_func
Unnamed_080cb7f8:
	.incbin "baserom.gba", 0x000ceff8, 0x00000414
	.section .rom.000cf40c, "ax"
	.global BattleEffect_RunTileAndPaletteAnimation
	.type BattleEffect_RunTileAndPaletteAnimation, %function
	.thumb_func
BattleEffect_RunTileAndPaletteAnimation:
	.incbin "baserom.gba", 0x000cf40c, 0x000009cc
	.section .rom.000cfdd8, "ax"
	.global Func_080cc5d8
	.type Func_080cc5d8, %function
	.thumb_func
Func_080cc5d8:
	.incbin "baserom.gba", 0x000cfdd8, 0x00000388
	.section .rom.000d02ec, "ax"
	.global BattlePresentation_PrepareScene
	.type BattlePresentation_PrepareScene, %function
	.thumb_func
BattlePresentation_PrepareScene:
	.incbin "baserom.gba", 0x000d02ec, 0x000000f0
	.section .rom.000d03dc, "ax"
	.global BattleFx_ScheduleCallbacksAndReleaseBlocks
	.type BattleFx_ScheduleCallbacksAndReleaseBlocks, %function
	.thumb_func
BattleFx_ScheduleCallbacksAndReleaseBlocks:
	.incbin "baserom.gba", 0x000d03dc, 0x00000528
	.section .rom.000d1834, "ax"
	.incbin "baserom.gba", 0x000d1834, 0x00000afc
	.section .rom.000d2354, "ax"
	.global BattleFx_RunMemberBurst
	.type BattleFx_RunMemberBurst, %function
	.thumb_func
BattleFx_RunMemberBurst:
	.incbin "baserom.gba", 0x000d2354, 0x00000410
	.section .rom.000d27f8, "ax"
	.global BattleFx_RunFortyEightFrameEffect
	.type BattleFx_RunFortyEightFrameEffect, %function
	.thumb_func
BattleFx_RunFortyEightFrameEffect:
	.incbin "baserom.gba", 0x000d27f8, 0x000002a8
	.section .rom.000d2ab8, "ax"
	.global BattleFx_RunMemberBeam
	.type BattleFx_RunMemberBeam, %function
	.thumb_func
BattleFx_RunMemberBeam:
	.incbin "baserom.gba", 0x000d2ab8, 0x000005d4
	.section .rom.000d30e0, "ax"
	.global BattleFx_RunSevenMode
	.type BattleFx_RunSevenMode, %function
	.thumb_func
BattleFx_RunSevenMode:
	.incbin "baserom.gba", 0x000d30e0, 0x00000b88
	.section .rom.000d3dfc, "ax"
	.incbin "baserom.gba", 0x000d3dfc, 0x00001118
	.section .rom.000d4f14, "ax"
	.global Unnamed_080d1714
	.type Unnamed_080d1714, %function
	.thumb_func
Unnamed_080d1714:
	.incbin "baserom.gba", 0x000d4f14, 0x00000d38
	.section .rom.000d5c64, "ax"
	.global BattleEffect_RunPaletteParticles
	.type BattleEffect_RunPaletteParticles, %function
	.thumb_func
BattleEffect_RunPaletteParticles:
	.incbin "baserom.gba", 0x000d5c64, 0x00000934
	.section .rom.000d6598, "ax"
	.global BattleEffect_RunEmberColumns
	.type BattleEffect_RunEmberColumns, %function
	.thumb_func
BattleEffect_RunEmberColumns:
	.incbin "baserom.gba", 0x000d6598, 0x00001354
	.section .rom.000d79a4, "ax"
	.incbin "baserom.gba", 0x000d79a4, 0x00000448
	.section .rom.000d7e04, "ax"
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x000d7e04, 0x00000c54
	.section .rom.000d8ac8, "ax"
	.global BattleFx_RenderMode
	.type BattleFx_RenderMode, %function
	.thumb_func
BattleFx_RenderMode:
	.incbin "baserom.gba", 0x000d8ac8, 0x0000123c
	.section .rom.000da170, "ax"
	.global BattleEffect_RunDitherDissolveScene
	.type BattleEffect_RunDitherDissolveScene, %function
	.thumb_func
BattleEffect_RunDitherDissolveScene:
	.incbin "baserom.gba", 0x000da170, 0x00000cec
	.section .rom.000dae5c, "ax"
	.global BattleFx_InitializeMode10
	.type BattleFx_InitializeMode10, %function
	.thumb_func
BattleFx_InitializeMode10:
	.incbin "baserom.gba", 0x000dae5c, 0x000012ec
	.section .rom.000dc1ac, "ax"
	.global BattleEffectA
	.type BattleEffectA, %function
	.thumb_func
BattleEffectA:
	.incbin "baserom.gba", 0x000dc1ac, 0x000007e8
	.section .rom.000dc9dc, "ax"
	.global BattleEffectB
	.type BattleEffectB, %function
	.thumb_func
BattleEffectB:
	.incbin "baserom.gba", 0x000dc9dc, 0x000008dc
	.section .rom.000dd2e8, "ax"
	.global RunPaletteRampEffect
	.type RunPaletteRampEffect, %function
	.thumb_func
RunPaletteRampEffect:
	.incbin "baserom.gba", 0x000dd2e8, 0x00000764
	.section .rom.000ddaac, "ax"
	.incbin "baserom.gba", 0x000ddaac, 0x0000141c
	.section .rom.000deee0, "ax"
	.global RunParticleFieldEffect
	.type RunParticleFieldEffect, %function
	.thumb_func
RunParticleFieldEffect:
	.incbin "baserom.gba", 0x000deee0, 0x00000444
	.section .rom.000df3dc, "ax"
	.incbin "baserom.gba", 0x000df3dc, 0x00000d8c
	.section .rom.000e0168, "ax"
	.global BattleEffect_RunStagedParticles
	.type BattleEffect_RunStagedParticles, %function
	.thumb_func
BattleEffect_RunStagedParticles:
	.incbin "baserom.gba", 0x000e0168, 0x00000944
	.section .rom.000e0ac4, "ax"
	.global BattleFx_RunDualTable
	.type BattleFx_RunDualTable, %function
	.thumb_func
BattleFx_RunDualTable:
	.incbin "baserom.gba", 0x000e0ac4, 0x00001034
	.section .rom.000e1af8, "ax"
	.global BattleFx_PrepareCanvasEffect
	.type BattleFx_PrepareCanvasEffect, %function
	.thumb_func
BattleFx_PrepareCanvasEffect:
	.incbin "baserom.gba", 0x000e1af8, 0x0000067c
	.section .rom.000e226e, "ax"
	.incbin "baserom.gba", 0x000e226e, 0x00000002
	.section .rom.000e2270, "ax"
	.global BattleFx_RunProjectileVolley
	.type BattleFx_RunProjectileVolley, %function
	.thumb_func
BattleFx_RunProjectileVolley:
	.incbin "baserom.gba", 0x000e2270, 0x00000e48
	.section .rom.000e3248, "ax"
	.global BattleFx_RunParticleFieldVariant
	.type BattleFx_RunParticleFieldVariant, %function
	.thumb_func
BattleFx_RunParticleFieldVariant:
	.incbin "baserom.gba", 0x000e3248, 0x00000394
	.section .rom.000e362a, "ax"
	.incbin "baserom.gba", 0x000e362a, 0x000006fa
	.section .rom.000e3d64, "ax"
	.incbin "baserom.gba", 0x000e3d64, 0x00000ff8
	.section .rom.000e4de8, "ax"
	.global BattleFx_InitializeMode12
	.type BattleFx_InitializeMode12, %function
	.thumb_func
BattleFx_InitializeMode12:
	.incbin "baserom.gba", 0x000e4de8, 0x0000130c
	.section .rom.000e6172, "ax"
	.incbin "baserom.gba", 0x000e6172, 0x00000002
	.section .rom.000e6174, "ax"
	.global BattlePres_RunBurstScene
	.type BattlePres_RunBurstScene, %function
	.thumb_func
BattlePres_RunBurstScene:
	.incbin "baserom.gba", 0x000e6174, 0x00000f44
	.section .rom.000e72a0, "ax"
	.global BattlePres_RunBeamSequence
	.type BattlePres_RunBeamSequence, %function
	.thumb_func
BattlePres_RunBeamSequence:
	.incbin "baserom.gba", 0x000e72a0, 0x00000604
	.section .rom.000e78a4, "ax"
	.global Unnamed_080e40a4
	.type Unnamed_080e40a4, %function
	.thumb_func
Unnamed_080e40a4:
	.incbin "baserom.gba", 0x000e78a4, 0x0000064c
	.section .rom.000e7fb8, "ax"
	.global BattleFx_RunCastingImpact
	.type BattleFx_RunCastingImpact, %function
	.thumb_func
BattleFx_RunCastingImpact:
	.incbin "baserom.gba", 0x000e7fb8, 0x00002190
	.section .rom.000ea18c, "ax"
	.incbin "baserom.gba", 0x000ea18c, 0x000003b0
	.section .rom.000ea6ac, "ax"
	.incbin "baserom.gba", 0x000ea6ac, 0x000003d0
	.section .rom.000eab38, "ax"
	.incbin "baserom.gba", 0x000eab38, 0x000000cc
	.section .rom.000eac04, "ax"
	.global BattleEffect_RunParticleStreams
	.type BattleEffect_RunParticleStreams, %function
	.thumb_func
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x000eac04, 0x00000e38
	.section .rom.000eba3c, "ax"
	.global BattleEffect_RunCirclingFallingScene
	.type BattleEffect_RunCirclingFallingScene, %function
	.thumb_func
BattleEffect_RunCirclingFallingScene:
	.incbin "baserom.gba", 0x000eba3c, 0x00001e9c
	.section .rom.000ed8d8, "ax"
	.global Unnamed_080ea0d8
	.type Unnamed_080ea0d8, %function
	.thumb_func
Unnamed_080ea0d8:
	.incbin "baserom.gba", 0x000ed8d8, 0x0000167c
	.section .rom.000eef54, "ax"
	.global Unnamed_080eb754
	.type Unnamed_080eb754, %function
	.thumb_func
Unnamed_080eb754:
	.incbin "baserom.gba", 0x000eef54, 0x0000098c
	.section .rom.000ef900, "ax"
	.global BattleFx_InitializeMode6
	.type BattleFx_InitializeMode6, %function
	.thumb_func
BattleFx_InitializeMode6:
	.incbin "baserom.gba", 0x000ef900, 0x00000d7c
	.section .rom.000f06f4, "ax"
	.global BattleFx_RunRevealColumn
	.type BattleFx_RunRevealColumn, %function
	.thumb_func
BattleFx_RunRevealColumn:
	.incbin "baserom.gba", 0x000f06f4, 0x00000514
	.section .rom.000f0c08, "ax"
	.global Unnamed_080ed408
	.type Unnamed_080ed408, %function
	.thumb_func
Unnamed_080ed408:
	.global BattleEffect_LoadWork
BattleEffect_LoadWork:
	.incbin "baserom.gba", 0x000f0c08, 0x000006b0
	.global Data_080edab8
Data_080edab8:
	.incbin "baserom.gba", 0x000f12b8, 0x00000008
	.global Data_080edac0
Data_080edac0:
	.incbin "baserom.gba", 0x000f12c0, 0x00000030
	.section .rom.000f1648, "ax"
	.incbin "baserom.gba", 0x000f1648, 0x0000046c
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000f1ab4, 0x00000b6a
	.global Data_080eee1e
Data_080eee1e:
	.incbin "baserom.gba", 0x000f261e, 0x0000000c
	.global Data_080eee2a
Data_080eee2a:
	.incbin "baserom.gba", 0x000f262a, 0x0000000c
	.global Data_080eee36
Data_080eee36:
	.incbin "baserom.gba", 0x000f2636, 0x00000008
	.global Data_080eee3e
Data_080eee3e:
	.incbin "baserom.gba", 0x000f263e, 0x00000008
	.global Data_080eee46
Data_080eee46:
	.incbin "baserom.gba", 0x000f2646, 0x00000008
	.global Data_080eee4e
Data_080eee4e:
	.incbin "baserom.gba", 0x000f264e, 0x00000156
	.section .rom.000f2814, "ax"
	.incbin "baserom.gba", 0x000f2814, 0x000007ec
	.section .rom.000f33f0, "ax"
	.global Func_080f03f0
	.type Func_080f03f0, %function
	.thumb_func
Func_080f03f0:
	.incbin "baserom.gba", 0x000f33f0, 0x00000148
	.section .rom.000f37f0, "ax"
	.global Func_080f07f0
Func_080f07f0:
	.incbin "baserom.gba", 0x000f37f0, 0x00000aa8
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f4298, 0x00000d68
	.section .rom.000f5028, "ax"
	.incbin "baserom.gba", 0x000f5028, 0x000006c4
	.section .rom.000f56ec, "ax"
	.global Func_080f26ec
	.type Func_080f26ec, %function
	.thumb_func
Func_080f26ec:
	.incbin "baserom.gba", 0x000f56ec, 0x00000480
	.section .rom.000f5b6c, "ax"
	.global Func_080f2b6c
	.type Func_080f2b6c, %function
	.thumb_func
Func_080f2b6c:
	.incbin "baserom.gba", 0x000f5b6c, 0x00000004
	.section .rom.000f5b70, "ax"
	.global Title_ShowSplashScreen
	.type Title_ShowSplashScreen, %function
	.thumb_func
Title_ShowSplashScreen:
	.incbin "baserom.gba", 0x000f5b70, 0x000001e4
	.section .rom.000f5d54, "ax"
	.global Unnamed_080f2d54
	.type Unnamed_080f2d54, %function
	.thumb_func
Unnamed_080f2d54:
	.incbin "baserom.gba", 0x000f5d54, 0x00000164
	.section .rom.000f6078, "ax"
	.global Unnamed_080f3078
	.type Unnamed_080f3078, %function
	.thumb_func
Unnamed_080f3078:
	.incbin "baserom.gba", 0x000f6078, 0x00000704
	.section .rom.000f68bc, "ax"
	.incbin "baserom.gba", 0x000f68bc, 0x00000744
	.section .rom.000f7168, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x000f7168, 0x00001e98
	.section .rom.000f9440, "ax"
	.incbin "baserom.gba", 0x000f9440, 0x0000106c
	.section .rom.000fa4ac, "ax"
	.global Unnamed_080f7460
	.type Unnamed_080f7460, %function
	.thumb_func
Unnamed_080f7460:
	.incbin "baserom.gba", 0x000fa4ac, 0x00000954
	.section .rom.000fafc4, "ax"
	.incbin "baserom.gba", 0x000fafc4, 0x0000083c
	.section .rom.000fdfa0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000fdfa0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000fe030, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000fe0e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000fe114, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000fe12c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000fe1b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000fe1c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000fe204, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000fe214, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000fe248, 0x00000030
	.section .rom.000fed04, "ax"
	.incbin "baserom.gba", 0x000fed04, 0x00000090
	.section .rom.000fee24, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fee24, 0x00000060
	.section .rom.00186e98, "ax"
	.incbin "baserom.gba", 0x00186e98, 0x00000968
	.section .rom.003217d8, "ax"
	.incbin "baserom.gba", 0x003217d8, 0x00000fd8
	.section .rom.003261e7, "ax"
	.incbin "baserom.gba", 0x003261e7, 0x00000001
	.section .rom.0032c89b, "ax"
	.incbin "baserom.gba", 0x0032c89b, 0x000086f9
	.section .rom.00336fe5, "ax"
	.incbin "baserom.gba", 0x00336fe5, 0x00000003
	.section .rom.003388f5, "ax"
	.incbin "baserom.gba", 0x003388f5, 0x00000003
	.section .rom.0033c3f9, "ax"
	.incbin "baserom.gba", 0x0033c3f9, 0x0000071f
	.section .rom.00340f5a, "ax"
	.incbin "baserom.gba", 0x00340f5a, 0x00000002
	.section .rom.003516ca, "ax"
	.incbin "baserom.gba", 0x003516ca, 0x00000002
	.section .rom.00354cba, "ax"
	.incbin "baserom.gba", 0x00354cba, 0x00000002
	.section .rom.0036075e, "ax"
	.incbin "baserom.gba", 0x0036075e, 0x00000002
	.section .rom.00370e0e, "ax"
	.incbin "baserom.gba", 0x00370e0e, 0x00000002
	.section .rom.003788ae, "ax"
	.incbin "baserom.gba", 0x003788ae, 0x00000002
	.section .rom.0037c0ee, "ax"
	.incbin "baserom.gba", 0x0037c0ee, 0x00000002
	.section .rom.00385522, "ax"
	.incbin "baserom.gba", 0x00385522, 0x00000002
	.section .rom.0039094a, "ax"
	.incbin "baserom.gba", 0x0039094a, 0x00000002
	.section .rom.00398806, "ax"
	.incbin "baserom.gba", 0x00398806, 0x00000002
	.section .rom.0039d296, "ax"
	.incbin "baserom.gba", 0x0039d296, 0x00000002
	.section .rom.003a154a, "ax"
	.incbin "baserom.gba", 0x003a154a, 0x00000002
	.section .rom.003a4eca, "ax"
	.incbin "baserom.gba", 0x003a4eca, 0x00000002
	.section .rom.003b1706, "ax"
	.incbin "baserom.gba", 0x003b1706, 0x00000002
	.section .rom.003c004e, "ax"
	.incbin "baserom.gba", 0x003c004e, 0x00000002
	.section .rom.003c5951, "ax"
	.incbin "baserom.gba", 0x003c5951, 0x00000003
	.section .rom.003c72d3, "ax"
	.incbin "baserom.gba", 0x003c72d3, 0x00000001
	.section .rom.003ca0f1, "ax"
	.incbin "baserom.gba", 0x003ca0f1, 0x00000003
	.section .rom.003cd06e, "ax"
	.incbin "baserom.gba", 0x003cd06e, 0x00000002
	.section .rom.003cd8b5, "ax"
	.incbin "baserom.gba", 0x003cd8b5, 0x00000003
	.section .rom.003cf0cb, "ax"
	.incbin "baserom.gba", 0x003cf0cb, 0x00000001
	.section .rom.003cf6b6, "ax"
	.incbin "baserom.gba", 0x003cf6b6, 0x00000002
	.section .rom.003cfd7a, "ax"
	.incbin "baserom.gba", 0x003cfd7a, 0x00000002
	.section .rom.003d0061, "ax"
	.incbin "baserom.gba", 0x003d0061, 0x00000003
	.section .rom.003d13c7, "ax"
	.incbin "baserom.gba", 0x003d13c7, 0x00000001
	.section .rom.003d17b1, "ax"
	.incbin "baserom.gba", 0x003d17b1, 0x00000003
	.section .rom.003d1b83, "ax"
	.incbin "baserom.gba", 0x003d1b83, 0x00000001
	.section .rom.003d2423, "ax"
	.incbin "baserom.gba", 0x003d2423, 0x00000001
	.section .rom.003d28c6, "ax"
	.incbin "baserom.gba", 0x003d28c6, 0x00000002
	.section .rom.003d3a7f, "ax"
	.incbin "baserom.gba", 0x003d3a7f, 0x00000001
	.section .rom.003d4f42, "ax"
	.incbin "baserom.gba", 0x003d4f42, 0x00000002
	.section .rom.003d5f0b, "ax"
	.incbin "baserom.gba", 0x003d5f0b, 0x00000001
	.section .rom.003d6166, "ax"
	.incbin "baserom.gba", 0x003d6166, 0x00000002
	.section .rom.003d7bc1, "ax"
	.incbin "baserom.gba", 0x003d7bc1, 0x00000003
	.section .rom.003d810d, "ax"
	.incbin "baserom.gba", 0x003d810d, 0x00000003
	.section .rom.003d9e96, "ax"
	.incbin "baserom.gba", 0x003d9e96, 0x00000002
	.section .rom.003da10f, "ax"
	.incbin "baserom.gba", 0x003da10f, 0x00000001
	.section .rom.003da5d6, "ax"
	.incbin "baserom.gba", 0x003da5d6, 0x00000002
	.section .rom.003dc1ab, "ax"
	.incbin "baserom.gba", 0x003dc1ab, 0x00000001
	.section .rom.003ddda9, "ax"
	.incbin "baserom.gba", 0x003ddda9, 0x00000003
	.section .rom.003ddfca, "ax"
	.incbin "baserom.gba", 0x003ddfca, 0x00000002
	.section .rom.003de407, "ax"
	.incbin "baserom.gba", 0x003de407, 0x00000001
	.section .rom.003de519, "ax"
	.incbin "baserom.gba", 0x003de519, 0x00000003
	.section .rom.003dec97, "ax"
	.incbin "baserom.gba", 0x003dec97, 0x00000001
	.section .rom.003df135, "ax"
	.incbin "baserom.gba", 0x003df135, 0x00000003
	.section .rom.003dfe4a, "ax"
	.incbin "baserom.gba", 0x003dfe4a, 0x00000002
	.section .rom.003e0bca, "ax"
	.incbin "baserom.gba", 0x003e0bca, 0x00000002
	.section .rom.003e0f5b, "ax"
	.incbin "baserom.gba", 0x003e0f5b, 0x00000001
	.section .rom.003e2ca2, "ax"
	.incbin "baserom.gba", 0x003e2ca2, 0x00000002
	.section .rom.003e3a79, "ax"
	.incbin "baserom.gba", 0x003e3a79, 0x00000003
	.section .rom.003e42a6, "ax"
	.incbin "baserom.gba", 0x003e42a6, 0x00000002
	.section .rom.003e4931, "ax"
	.incbin "baserom.gba", 0x003e4931, 0x00000003
	.section .rom.003e4aef, "ax"
	.incbin "baserom.gba", 0x003e4aef, 0x00000001
	.section .rom.003e5847, "ax"
	.incbin "baserom.gba", 0x003e5847, 0x00000001
	.section .rom.003e5d93, "ax"
	.incbin "baserom.gba", 0x003e5d93, 0x00000001
	.section .rom.003e616d, "ax"
	.incbin "baserom.gba", 0x003e616d, 0x00000003
	.section .rom.003e651b, "ax"
	.incbin "baserom.gba", 0x003e651b, 0x00000001
	.section .rom.003e84bf, "ax"
	.incbin "baserom.gba", 0x003e84bf, 0x00000001
	.section .rom.003e925b, "ax"
	.incbin "baserom.gba", 0x003e925b, 0x00000001
	.section .rom.003e9477, "ax"
	.incbin "baserom.gba", 0x003e9477, 0x00000001
	.section .rom.003e9773, "ax"
	.incbin "baserom.gba", 0x003e9773, 0x00000001
	.section .rom.003eb921, "ax"
	.incbin "baserom.gba", 0x003eb921, 0x00000003
	.section .rom.003ec6ef, "ax"
	.incbin "baserom.gba", 0x003ec6ef, 0x00000001
	.section .rom.003ed851, "ax"
	.incbin "baserom.gba", 0x003ed851, 0x00000003
	.section .rom.003eddab, "ax"
	.incbin "baserom.gba", 0x003eddab, 0x00000001
	.section .rom.003ef695, "ax"
	.incbin "baserom.gba", 0x003ef695, 0x00000003
	.section .rom.003eff36, "ax"
	.incbin "baserom.gba", 0x003eff36, 0x00000002
	.section .rom.003f0313, "ax"
	.incbin "baserom.gba", 0x003f0313, 0x00000001
	.section .rom.003f059d, "ax"
	.incbin "baserom.gba", 0x003f059d, 0x00000003
	.section .rom.003f0943, "ax"
	.incbin "baserom.gba", 0x003f0943, 0x00000001
	.section .rom.003f0b9e, "ax"
	.incbin "baserom.gba", 0x003f0b9e, 0x00000002
	.section .rom.003f0f56, "ax"
	.incbin "baserom.gba", 0x003f0f56, 0x00000002
	.section .rom.003f243f, "ax"
	.incbin "baserom.gba", 0x003f243f, 0x00000001
	.section .rom.003f3a02, "ax"
	.incbin "baserom.gba", 0x003f3a02, 0x00000a6e
	.section .rom.003f5246, "ax"
	.incbin "baserom.gba", 0x003f5246, 0x00000002
	.section .rom.003f55c9, "ax"
	.incbin "baserom.gba", 0x003f55c9, 0x00000003
	.section .rom.003f6279, "ax"
	.incbin "baserom.gba", 0x003f6279, 0x00000003
	.section .rom.003f6dc1, "ax"
	.incbin "baserom.gba", 0x003f6dc1, 0x00000003
	.section .rom.003f6f5a, "ax"
	.incbin "baserom.gba", 0x003f6f5a, 0x00000002
	.section .rom.003f77e7, "ax"
	.incbin "baserom.gba", 0x003f77e7, 0x00000001
	.section .rom.003f82ae, "ax"
	.incbin "baserom.gba", 0x003f82ae, 0x00000002
	.section .rom.003f87c6, "ax"
	.incbin "baserom.gba", 0x003f87c6, 0x00000002
	.section .rom.003f8dea, "ax"
	.incbin "baserom.gba", 0x003f8dea, 0x00000002
	.section .rom.003f9651, "ax"
	.incbin "baserom.gba", 0x003f9651, 0x00000003
	.section .rom.003f9a75, "ax"
	.incbin "baserom.gba", 0x003f9a75, 0x00000003
	.section .rom.003f9d0f, "ax"
	.incbin "baserom.gba", 0x003f9d0f, 0x00000001
	.section .rom.003fb6ab, "ax"
	.incbin "baserom.gba", 0x003fb6ab, 0x00000001
	.section .rom.003fd422, "ax"
	.incbin "baserom.gba", 0x003fd422, 0x00000002
	.section .rom.003ff397, "ax"
	.incbin "baserom.gba", 0x003ff397, 0x00000001
	.section .rom.003ff867, "ax"
	.incbin "baserom.gba", 0x003ff867, 0x00000001
	.section .rom.003ffef9, "ax"
	.incbin "baserom.gba", 0x003ffef9, 0x00000a37
	.section .rom.00401d01, "ax"
	.incbin "baserom.gba", 0x00401d01, 0x00000003
	.section .rom.004027d2, "ax"
	.incbin "baserom.gba", 0x004027d2, 0x00000002
	.section .rom.0040330f, "ax"
	.incbin "baserom.gba", 0x0040330f, 0x00000001
	.section .rom.0040394f, "ax"
	.incbin "baserom.gba", 0x0040394f, 0x00001589
	.section .rom.00404f39, "ax"
	.incbin "baserom.gba", 0x00404f39, 0x00000003
	.section .rom.004052a7, "ax"
	.incbin "baserom.gba", 0x004052a7, 0x00000001
	.section .rom.00405845, "ax"
	.incbin "baserom.gba", 0x00405845, 0x00000003
	.section .rom.00405b79, "ax"
	.incbin "baserom.gba", 0x00405b79, 0x00000003
	.section .rom.00405eb5, "ax"
	.incbin "baserom.gba", 0x00405eb5, 0x00000003
	.section .rom.00406ece, "ax"
	.incbin "baserom.gba", 0x00406ece, 0x00000002
	.section .rom.0040a4da, "ax"
	.incbin "baserom.gba", 0x0040a4da, 0x00000002
	.section .rom.0040ac7d, "ax"
	.incbin "baserom.gba", 0x0040ac7d, 0x00000003
	.section .rom.0040c00f, "ax"
	.incbin "baserom.gba", 0x0040c00f, 0x00000001
	.section .rom.0040c43f, "ax"
	.incbin "baserom.gba", 0x0040c43f, 0x00000001
	.section .rom.0040e235, "ax"
	.incbin "baserom.gba", 0x0040e235, 0x00000003
	.section .rom.0040eb4a, "ax"
	.incbin "baserom.gba", 0x0040eb4a, 0x00000002
	.section .rom.0041067e, "ax"
	.incbin "baserom.gba", 0x0041067e, 0x00000002
	.section .rom.004116cd, "ax"
	.incbin "baserom.gba", 0x004116cd, 0x00000003
	.section .rom.00411d83, "ax"
	.incbin "baserom.gba", 0x00411d83, 0x00000001
	.section .rom.00412e29, "ax"
	.incbin "baserom.gba", 0x00412e29, 0x00000003
	.section .rom.0042623b, "ax"
	.incbin "baserom.gba", 0x0042623b, 0x00000001
	.section .rom.0042638d, "ax"
	.incbin "baserom.gba", 0x0042638d, 0x00000003
	.section .rom.0042681e, "ax"
	.incbin "baserom.gba", 0x0042681e, 0x00000002
	.section .rom.00426a15, "ax"
	.incbin "baserom.gba", 0x00426a15, 0x00000003
	.section .rom.004280d2, "ax"
	.incbin "baserom.gba", 0x004280d2, 0x00000002
	.section .rom.004295f1, "ax"
	.incbin "baserom.gba", 0x004295f1, 0x00000003
	.section .rom.0042a1bd, "ax"
	.incbin "baserom.gba", 0x0042a1bd, 0x00000003
	.section .rom.0042ad32, "ax"
	.incbin "baserom.gba", 0x0042ad32, 0x00000002
	.section .rom.0042cbdb, "ax"
	.incbin "baserom.gba", 0x0042cbdb, 0x00000001
	.section .rom.0042e369, "ax"
	.incbin "baserom.gba", 0x0042e369, 0x00000003
	.section .rom.0042f81a, "ax"
	.incbin "baserom.gba", 0x0042f81a, 0x00000002
	.section .rom.00431f67, "ax"
	.incbin "baserom.gba", 0x00431f67, 0x00000001
	.section .rom.00433801, "ax"
	.incbin "baserom.gba", 0x00433801, 0x00000003
	.section .rom.00434dca, "ax"
	.incbin "baserom.gba", 0x00434dca, 0x00000002
	.section .rom.0043685a, "ax"
	.incbin "baserom.gba", 0x0043685a, 0x00000002
	.section .rom.00441032, "ax"
	.incbin "baserom.gba", 0x00441032, 0x00000002
	.section .rom.004431e6, "ax"
	.incbin "baserom.gba", 0x004431e6, 0x00000002
	.section .rom.0044734d, "ax"
	.incbin "baserom.gba", 0x0044734d, 0x00000003
	.section .rom.0044a572, "ax"
	.incbin "baserom.gba", 0x0044a572, 0x00000002
	.section .rom.0044bc76, "ax"
	.incbin "baserom.gba", 0x0044bc76, 0x00000002
	.section .rom.0045285e, "ax"
	.incbin "baserom.gba", 0x0045285e, 0x00000002
	.section .rom.0045b1e2, "ax"
	.incbin "baserom.gba", 0x0045b1e2, 0x00000002
	.section .rom.00462cbe, "ax"
	.incbin "baserom.gba", 0x00462cbe, 0x00000002
	.section .rom.00465d52, "ax"
	.incbin "baserom.gba", 0x00465d52, 0x00000002
	.section .rom.00469161, "ax"
	.incbin "baserom.gba", 0x00469161, 0x00000003
	.section .rom.0046b9cd, "ax"
	.incbin "baserom.gba", 0x0046b9cd, 0x00000003
	.section .rom.0046d4be, "ax"
	.incbin "baserom.gba", 0x0046d4be, 0x00000002
	.section .rom.0046e2d6, "ax"
	.incbin "baserom.gba", 0x0046e2d6, 0x00000002
	.section .rom.0046efc1, "ax"
	.incbin "baserom.gba", 0x0046efc1, 0x00000003
	.section .rom.0047847e, "ax"
	.incbin "baserom.gba", 0x0047847e, 0x00000002
	.section .rom.00479189, "ax"
	.incbin "baserom.gba", 0x00479189, 0x00000003
	.section .rom.0047b29d, "ax"
	.incbin "baserom.gba", 0x0047b29d, 0x00000003
	.section .rom.0047bf0f, "ax"
	.incbin "baserom.gba", 0x0047bf0f, 0x00000001
	.section .rom.0047cb27, "ax"
	.incbin "baserom.gba", 0x0047cb27, 0x00000001
	.section .rom.0047d29b, "ax"
	.incbin "baserom.gba", 0x0047d29b, 0x00000001
	.section .rom.0047eb2b, "ax"
	.incbin "baserom.gba", 0x0047eb2b, 0x00000001
	.section .rom.0047f4f9, "ax"
	.incbin "baserom.gba", 0x0047f4f9, 0x00000003
	.section .rom.00480117, "ax"
	.incbin "baserom.gba", 0x00480117, 0x00000001
	.section .rom.004827cb, "ax"
	.incbin "baserom.gba", 0x004827cb, 0x00000001
	.section .rom.00484ede, "ax"
	.incbin "baserom.gba", 0x00484ede, 0x00000002
	.section .rom.00489e33, "ax"
	.incbin "baserom.gba", 0x00489e33, 0x00000001
	.section .rom.0048e15d, "ax"
	.incbin "baserom.gba", 0x0048e15d, 0x00000003
	.section .rom.0048ed4a, "ax"
	.incbin "baserom.gba", 0x0048ed4a, 0x00000002
	.section .rom.004938ff, "ax"
	.incbin "baserom.gba", 0x004938ff, 0x00000001
	.section .rom.00495c69, "ax"
	.incbin "baserom.gba", 0x00495c69, 0x00000003
	.section .rom.0049760b, "ax"
	.incbin "baserom.gba", 0x0049760b, 0x00000001
	.section .rom.0049becd, "ax"
	.incbin "baserom.gba", 0x0049becd, 0x00000003
	.section .rom.0049ff95, "ax"
	.incbin "baserom.gba", 0x0049ff95, 0x00000003
	.section .rom.004a365e, "ax"
	.incbin "baserom.gba", 0x004a365e, 0x00000002
	.section .rom.004ab62f, "ax"
	.incbin "baserom.gba", 0x004ab62f, 0x00000001
	.section .rom.004b293f, "ax"
	.incbin "baserom.gba", 0x004b293f, 0x00000001
	.section .rom.004b78bf, "ax"
	.incbin "baserom.gba", 0x004b78bf, 0x00000001
	.section .rom.004bc26f, "ax"
	.incbin "baserom.gba", 0x004bc26f, 0x0000051d
	.section .rom.004c1b9b, "ax"
	.incbin "baserom.gba", 0x004c1b9b, 0x00000001
	.section .rom.004c1d29, "ax"
	.incbin "baserom.gba", 0x004c1d29, 0x00000003
	.section .rom.004c6d89, "ax"
	.incbin "baserom.gba", 0x004c6d89, 0x00000003
	.section .rom.004cb2f5, "ax"
	.incbin "baserom.gba", 0x004cb2f5, 0x00000003
	.section .rom.004d04b9, "ax"
	.incbin "baserom.gba", 0x004d04b9, 0x00000003
	.section .rom.004d064f, "ax"
	.incbin "baserom.gba", 0x004d064f, 0x00000001
	.section .rom.004d32a3, "ax"
	.incbin "baserom.gba", 0x004d32a3, 0x00000001
	.section .rom.004d9f57, "ax"
	.incbin "baserom.gba", 0x004d9f57, 0x00000001
	.section .rom.004dd092, "ax"
	.incbin "baserom.gba", 0x004dd092, 0x00000002
	.section .rom.004dd213, "ax"
	.incbin "baserom.gba", 0x004dd213, 0x00000001
	.section .rom.004df9cd, "ax"
	.incbin "baserom.gba", 0x004df9cd, 0x00000003
	.section .rom.004e2016, "ax"
	.incbin "baserom.gba", 0x004e2016, 0x00000002
	.section .rom.004e46fa, "ax"
	.incbin "baserom.gba", 0x004e46fa, 0x00000002
	.section .rom.004e576f, "ax"
	.incbin "baserom.gba", 0x004e576f, 0x00000001
	.section .rom.004e78c2, "ax"
	.incbin "baserom.gba", 0x004e78c2, 0x00000002
	.section .rom.004e7a55, "ax"
	.incbin "baserom.gba", 0x004e7a55, 0x00000003
	.section .rom.004ea297, "ax"
	.incbin "baserom.gba", 0x004ea297, 0x00000001
	.section .rom.004ec0ad, "ax"
	.incbin "baserom.gba", 0x004ec0ad, 0x00000003
	.section .rom.004ee78e, "ax"
	.incbin "baserom.gba", 0x004ee78e, 0x00000002
	.section .rom.004ef8d5, "ax"
	.incbin "baserom.gba", 0x004ef8d5, 0x00000003
	.section .rom.004f29e9, "ax"
	.incbin "baserom.gba", 0x004f29e9, 0x00000003
	.section .rom.004f6b51, "ax"
	.incbin "baserom.gba", 0x004f6b51, 0x00000003
	.section .rom.004f814e, "ax"
	.incbin "baserom.gba", 0x004f814e, 0x00000002
	.section .rom.004f9417, "ax"
	.incbin "baserom.gba", 0x004f9417, 0x00000001
	.section .rom.004fc82f, "ax"
	.incbin "baserom.gba", 0x004fc82f, 0x00000001
	.section .rom.004fc9bd, "ax"
	.incbin "baserom.gba", 0x004fc9bd, 0x00000003
	.section .rom.004fe76e, "ax"
	.incbin "baserom.gba", 0x004fe76e, 0x00000002
	.section .rom.005022e5, "ax"
	.incbin "baserom.gba", 0x005022e5, 0x00000003
	.section .rom.005037f3, "ax"
	.incbin "baserom.gba", 0x005037f3, 0x00000001
	.section .rom.005043c7, "ax"
	.incbin "baserom.gba", 0x005043c7, 0x00000001
	.section .rom.005044c5, "ax"
	.incbin "baserom.gba", 0x005044c5, 0x00000003
	.section .rom.005058aa, "ax"
	.incbin "baserom.gba", 0x005058aa, 0x00000002
	.section .rom.00506a2d, "ax"
	.incbin "baserom.gba", 0x00506a2d, 0x00000003
	.section .rom.005073b6, "ax"
	.incbin "baserom.gba", 0x005073b6, 0x00000002
	.section .rom.0050a033, "ax"
	.incbin "baserom.gba", 0x0050a033, 0x00000001
	.section .rom.0050a116, "ax"
	.incbin "baserom.gba", 0x0050a116, 0x00000002
	.section .rom.0050b301, "ax"
	.incbin "baserom.gba", 0x0050b301, 0x00000003
	.section .rom.0050cf89, "ax"
	.incbin "baserom.gba", 0x0050cf89, 0x00000003
	.section .rom.0050d497, "ax"
	.incbin "baserom.gba", 0x0050d497, 0x00000001
	.section .rom.0050d5d7, "ax"
	.incbin "baserom.gba", 0x0050d5d7, 0x00000001
	.section .rom.0050e7f2, "ax"
	.incbin "baserom.gba", 0x0050e7f2, 0x00000002
	.section .rom.0050e94d, "ax"
	.incbin "baserom.gba", 0x0050e94d, 0x00000003
	.section .rom.00512d73, "ax"
	.incbin "baserom.gba", 0x00512d73, 0x00000001
	.section .rom.00514aa2, "ax"
	.incbin "baserom.gba", 0x00514aa2, 0x00000002
	.section .rom.0051581b, "ax"
	.incbin "baserom.gba", 0x0051581b, 0x00000001
	.section .rom.0051594f, "ax"
	.incbin "baserom.gba", 0x0051594f, 0x00000001
	.section .rom.00517dd2, "ax"
	.incbin "baserom.gba", 0x00517dd2, 0x00000002
	.section .rom.00518b4f, "ax"
	.incbin "baserom.gba", 0x00518b4f, 0x00000001
	.section .rom.0051962e, "ax"
	.incbin "baserom.gba", 0x0051962e, 0x00000002
	.section .rom.0051a4a6, "ax"
	.incbin "baserom.gba", 0x0051a4a6, 0x00000002
	.section .rom.0051c57a, "ax"
	.incbin "baserom.gba", 0x0051c57a, 0x00000002
	.section .rom.0051cec1, "ax"
	.incbin "baserom.gba", 0x0051cec1, 0x00000003
	.section .rom.0051db0e, "ax"
	.incbin "baserom.gba", 0x0051db0e, 0x00000002
	.section .rom.0051dd35, "ax"
	.incbin "baserom.gba", 0x0051dd35, 0x00000003
	.section .rom.0051f677, "ax"
	.incbin "baserom.gba", 0x0051f677, 0x00000001
	.section .rom.0051f78b, "ax"
	.incbin "baserom.gba", 0x0051f78b, 0x00000001
	.section .rom.00521151, "ax"
	.incbin "baserom.gba", 0x00521151, 0x00000003
	.section .rom.0052129b, "ax"
	.incbin "baserom.gba", 0x0052129b, 0x00000001
	.section .rom.00523367, "ax"
	.incbin "baserom.gba", 0x00523367, 0x00000001
	.section .rom.0052348f, "ax"
	.incbin "baserom.gba", 0x0052348f, 0x00000001
	.section .rom.00524d17, "ax"
	.incbin "baserom.gba", 0x00524d17, 0x00000001
	.section .rom.00526fb9, "ax"
	.incbin "baserom.gba", 0x00526fb9, 0x00000003
	.section .rom.005270fb, "ax"
	.incbin "baserom.gba", 0x005270fb, 0x00000001
	.section .rom.00529103, "ax"
	.incbin "baserom.gba", 0x00529103, 0x00000001
	.section .rom.0052920e, "ax"
	.incbin "baserom.gba", 0x0052920e, 0x00000002
	.section .rom.0052b1f2, "ax"
	.incbin "baserom.gba", 0x0052b1f2, 0x00000002
	.section .rom.0052c917, "ax"
	.incbin "baserom.gba", 0x0052c917, 0x00000001
	.section .rom.0052d9cb, "ax"
	.incbin "baserom.gba", 0x0052d9cb, 0x00000001
	.section .rom.0052f6b2, "ax"
	.incbin "baserom.gba", 0x0052f6b2, 0x00000002
	.section .rom.0052f80d, "ax"
	.incbin "baserom.gba", 0x0052f80d, 0x00000003
	.section .rom.00531756, "ax"
	.incbin "baserom.gba", 0x00531756, 0x00000002
	.section .rom.005318db, "ax"
	.incbin "baserom.gba", 0x005318db, 0x00000001
	.section .rom.0053442d, "ax"
	.incbin "baserom.gba", 0x0053442d, 0x00000003
	.section .rom.00536af2, "ax"
	.incbin "baserom.gba", 0x00536af2, 0x00000002
	.section .rom.0053924a, "ax"
	.incbin "baserom.gba", 0x0053924a, 0x00000002
	.section .rom.0053aa9d, "ax"
	.incbin "baserom.gba", 0x0053aa9d, 0x00000003
	.section .rom.0053d19d, "ax"
	.incbin "baserom.gba", 0x0053d19d, 0x00000003
	.section .rom.0053ef69, "ax"
	.incbin "baserom.gba", 0x0053ef69, 0x00000003
	.section .rom.005414b3, "ax"
	.incbin "baserom.gba", 0x005414b3, 0x00000001
	.section .rom.0054277e, "ax"
	.incbin "baserom.gba", 0x0054277e, 0x00000002
	.section .rom.0054342f, "ax"
	.incbin "baserom.gba", 0x0054342f, 0x00000001
	.section .rom.00543e99, "ax"
	.incbin "baserom.gba", 0x00543e99, 0x00000003
	.section .rom.00543f8f, "ax"
	.incbin "baserom.gba", 0x00543f8f, 0x00000001
	.section .rom.005459ee, "ax"
	.incbin "baserom.gba", 0x005459ee, 0x00000002
	.section .rom.00546792, "ax"
	.incbin "baserom.gba", 0x00546792, 0x00000002
	.section .rom.005482cb, "ax"
	.incbin "baserom.gba", 0x005482cb, 0x00000001
	.section .rom.0054b1b9, "ax"
	.incbin "baserom.gba", 0x0054b1b9, 0x00000003
	.section .rom.0055299f, "ax"
	.incbin "baserom.gba", 0x0055299f, 0x00000001
	.section .rom.00552a6b, "ax"
	.incbin "baserom.gba", 0x00552a6b, 0x00000001
	.section .rom.0055822d, "ax"
	.incbin "baserom.gba", 0x0055822d, 0x00000003
	.section .rom.0055836f, "ax"
	.incbin "baserom.gba", 0x0055836f, 0x00000001
	.section .rom.0055979d, "ax"
	.incbin "baserom.gba", 0x0055979d, 0x00000003
	.section .rom.0055c4da, "ax"
	.incbin "baserom.gba", 0x0055c4da, 0x00000002
	.section .rom.0055e5d6, "ax"
	.incbin "baserom.gba", 0x0055e5d6, 0x00000002
	.section .rom.00565fe2, "ax"
	.incbin "baserom.gba", 0x00565fe2, 0x00000002
	.section .rom.0056617e, "ax"
	.incbin "baserom.gba", 0x0056617e, 0x00000002
	.section .rom.0056aa19, "ax"
	.incbin "baserom.gba", 0x0056aa19, 0x00000003
	.section .rom.0056cf1a, "ax"
	.incbin "baserom.gba", 0x0056cf1a, 0x00000002
	.section .rom.0057490b, "ax"
	.incbin "baserom.gba", 0x0057490b, 0x00000001
	.section .rom.00574aa6, "ax"
	.incbin "baserom.gba", 0x00574aa6, 0x00000002
	.section .rom.00577655, "ax"
	.incbin "baserom.gba", 0x00577655, 0x00000003
	.section .rom.00579729, "ax"
	.incbin "baserom.gba", 0x00579729, 0x00000003
	.section .rom.0057b782, "ax"
	.incbin "baserom.gba", 0x0057b782, 0x00001cee
	.section .rom.0057ec19, "ax"
	.incbin "baserom.gba", 0x0057ec19, 0x00000003
	.section .rom.005818f5, "ax"
	.incbin "baserom.gba", 0x005818f5, 0x00000003
	.section .rom.005836c7, "ax"
	.incbin "baserom.gba", 0x005836c7, 0x00000001
	.section .rom.00583807, "ax"
	.incbin "baserom.gba", 0x00583807, 0x00000001
	.section .rom.00586861, "ax"
	.incbin "baserom.gba", 0x00586861, 0x00000003
	.section .rom.005897de, "ax"
	.incbin "baserom.gba", 0x005897de, 0x00000002
	.section .rom.0058c0a5, "ax"
	.incbin "baserom.gba", 0x0058c0a5, 0x00000003
	.section .rom.0059300d, "ax"
	.incbin "baserom.gba", 0x0059300d, 0x00000003
	.section .rom.005948b3, "ax"
	.incbin "baserom.gba", 0x005948b3, 0x00000001
	.section .rom.005951b2, "ax"
	.incbin "baserom.gba", 0x005951b2, 0x00000002
	.section .rom.00595f2b, "ax"
	.incbin "baserom.gba", 0x00595f2b, 0x00000001
	.section .rom.0059608b, "ax"
	.incbin "baserom.gba", 0x0059608b, 0x00000001
	.section .rom.00597ce3, "ax"
	.incbin "baserom.gba", 0x00597ce3, 0x00000001
	.section .rom.0059957b, "ax"
	.incbin "baserom.gba", 0x0059957b, 0x00000001
	.section .rom.005996ff, "ax"
	.incbin "baserom.gba", 0x005996ff, 0x00000001
	.section .rom.0059c23b, "ax"
	.incbin "baserom.gba", 0x0059c23b, 0x00000001
	.section .rom.0059e9b5, "ax"
	.incbin "baserom.gba", 0x0059e9b5, 0x00000003
	.section .rom.0059f9a2, "ax"
	.incbin "baserom.gba", 0x0059f9a2, 0x00000002
	.section .rom.005a1063, "ax"
	.incbin "baserom.gba", 0x005a1063, 0x00000001
	.section .rom.005a120d, "ax"
	.incbin "baserom.gba", 0x005a120d, 0x00000003
	.section .rom.005a3fa1, "ax"
	.incbin "baserom.gba", 0x005a3fa1, 0x00000003
	.section .rom.005a8036, "ax"
	.incbin "baserom.gba", 0x005a8036, 0x00000002
	.section .rom.005a8177, "ax"
	.incbin "baserom.gba", 0x005a8177, 0x00000001
	.section .rom.005aa03f, "ax"
	.incbin "baserom.gba", 0x005aa03f, 0x00000001
	.section .rom.005aa1e1, "ax"
	.incbin "baserom.gba", 0x005aa1e1, 0x00000003
	.section .rom.005ac283, "ax"
	.incbin "baserom.gba", 0x005ac283, 0x00000001
	.section .rom.005ad210, "ax"
	.incbin "baserom.gba", 0x005ad210, 0x000022d8
	.section .rom.005b1fd9, "ax"
	.incbin "baserom.gba", 0x005b1fd9, 0x00000003
	.section .rom.005b215e, "ax"
	.incbin "baserom.gba", 0x005b215e, 0x00000002
	.section .rom.005b98ab, "ax"
	.incbin "baserom.gba", 0x005b98ab, 0x00000001
	.section .rom.005b9db5, "ax"
	.incbin "baserom.gba", 0x005b9db5, 0x00000003
	.section .rom.005bb48d, "ax"
	.incbin "baserom.gba", 0x005bb48d, 0x00000003
	.section .rom.005bb631, "ax"
	.incbin "baserom.gba", 0x005bb631, 0x00000003
	.section .rom.005bcf36, "ax"
	.incbin "baserom.gba", 0x005bcf36, 0x00000002
	.section .rom.005be763, "ax"
	.incbin "baserom.gba", 0x005be763, 0x00000001
	.section .rom.005be8e1, "ax"
	.incbin "baserom.gba", 0x005be8e1, 0x00000003
	.section .rom.005c31d5, "ax"
	.incbin "baserom.gba", 0x005c31d5, 0x00000003
	.section .rom.005c421a, "ax"
	.incbin "baserom.gba", 0x005c421a, 0x00000002
	.section .rom.005c503e, "ax"
	.incbin "baserom.gba", 0x005c503e, 0x00000002
	.section .rom.005c6383, "ax"
	.incbin "baserom.gba", 0x005c6383, 0x00000001
	.section .rom.005c64dd, "ax"
	.incbin "baserom.gba", 0x005c64dd, 0x00000003
	.section .rom.005c8a9b, "ax"
	.incbin "baserom.gba", 0x005c8a9b, 0x00000001
	.section .rom.005c8c12, "ax"
	.incbin "baserom.gba", 0x005c8c12, 0x00000002
	.section .rom.005cd505, "ax"
	.incbin "baserom.gba", 0x005cd505, 0x00000003
	.section .rom.005cdf6d, "ax"
	.incbin "baserom.gba", 0x005cdf6d, 0x00000003
	.section .rom.005d05af, "ax"
	.incbin "baserom.gba", 0x005d05af, 0x00000001
	.section .rom.005d0716, "ax"
	.incbin "baserom.gba", 0x005d0716, 0x00000002
	.section .rom.005d185d, "ax"
	.incbin "baserom.gba", 0x005d185d, 0x00000003
	.section .rom.005d35bb, "ax"
	.incbin "baserom.gba", 0x005d35bb, 0x00000001
	.section .rom.005d3722, "ax"
	.incbin "baserom.gba", 0x005d3722, 0x00000002
	.section .rom.005d5fb1, "ax"
	.incbin "baserom.gba", 0x005d5fb1, 0x00000003
	.section .rom.005d60e6, "ax"
	.incbin "baserom.gba", 0x005d60e6, 0x00000002
	.section .rom.005d8d22, "ax"
	.incbin "baserom.gba", 0x005d8d22, 0x00000002
	.section .rom.005dc4c1, "ax"
	.incbin "baserom.gba", 0x005dc4c1, 0x00000003
	.section .rom.005df80f, "ax"
	.incbin "baserom.gba", 0x005df80f, 0x00000001
	.section .rom.005df98b, "ax"
	.incbin "baserom.gba", 0x005df98b, 0x00000001
	.section .rom.005e253a, "ax"
	.incbin "baserom.gba", 0x005e253a, 0x00000002
	.section .rom.005e47ae, "ax"
	.incbin "baserom.gba", 0x005e47ae, 0x00000002
	.section .rom.005e701f, "ax"
	.incbin "baserom.gba", 0x005e701f, 0x00000001
	.section .rom.005ea513, "ax"
	.incbin "baserom.gba", 0x005ea513, 0x00000001
	.section .rom.005ef13a, "ax"
	.incbin "baserom.gba", 0x005ef13a, 0x00000002
	.section .rom.005efdbd, "ax"
	.incbin "baserom.gba", 0x005efdbd, 0x00000003
	.section .rom.005efeff, "ax"
	.incbin "baserom.gba", 0x005efeff, 0x00000001
	.section .rom.005f146f, "ax"
	.incbin "baserom.gba", 0x005f146f, 0x00000001
	.section .rom.005f15da, "ax"
	.incbin "baserom.gba", 0x005f15da, 0x00000002
	.section .rom.005f3335, "ax"
	.incbin "baserom.gba", 0x005f3335, 0x00000003
	.section .rom.005f3477, "ax"
	.incbin "baserom.gba", 0x005f3477, 0x00000001
	.section .rom.005f5c76, "ax"
	.incbin "baserom.gba", 0x005f5c76, 0x00000002
	.section .rom.005f5db7, "ax"
	.incbin "baserom.gba", 0x005f5db7, 0x00000001
	.section .rom.005f77cf, "ax"
	.incbin "baserom.gba", 0x005f77cf, 0x00000001
	.section .rom.005f8d4a, "ax"
	.incbin "baserom.gba", 0x005f8d4a, 0x00000002
	.section .rom.005fa286, "ax"
	.incbin "baserom.gba", 0x005fa286, 0x00000002
	.section .rom.005fa416, "ax"
	.incbin "baserom.gba", 0x005fa416, 0x00000002
	.section .rom.005fc6cf, "ax"
	.incbin "baserom.gba", 0x005fc6cf, 0x00000001
	.section .rom.00600916, "ax"
	.incbin "baserom.gba", 0x00600916, 0x00000002
	.section .rom.0060270d, "ax"
	.incbin "baserom.gba", 0x0060270d, 0x00000003
	.section .rom.006058bb, "ax"
	.incbin "baserom.gba", 0x006058bb, 0x00000001
	.section .rom.00607eef, "ax"
	.incbin "baserom.gba", 0x00607eef, 0x00000001
	.section .rom.0060aa47, "ax"
	.incbin "baserom.gba", 0x0060aa47, 0x00000001
	.section .rom.0060ac0f, "ax"
	.incbin "baserom.gba", 0x0060ac0f, 0x00002905
	.section .rom.00611019, "ax"
	.incbin "baserom.gba", 0x00611019, 0x00000003
	.section .rom.0061258a, "ax"
	.incbin "baserom.gba", 0x0061258a, 0x00000002
	.section .rom.00614db1, "ax"
	.incbin "baserom.gba", 0x00614db1, 0x00000003
	.section .rom.00614f21, "ax"
	.incbin "baserom.gba", 0x00614f21, 0x00000003
	.section .rom.006177e3, "ax"
	.incbin "baserom.gba", 0x006177e3, 0x00000001
	.section .rom.00619fab, "ax"
	.incbin "baserom.gba", 0x00619fab, 0x00000001
	.section .rom.0061b2a5, "ax"
	.incbin "baserom.gba", 0x0061b2a5, 0x00000003
	.section .rom.0061cbeb, "ax"
	.incbin "baserom.gba", 0x0061cbeb, 0x00000001
	.section .rom.0062807e, "ax"
	.incbin "baserom.gba", 0x0062807e, 0x00000002
	.section .rom.0062823e, "ax"
	.incbin "baserom.gba", 0x0062823e, 0x00000002
	.section .rom.0062b5a5, "ax"
	.incbin "baserom.gba", 0x0062b5a5, 0x00000003
	.section .rom.0062da96, "ax"
	.incbin "baserom.gba", 0x0062da96, 0x00000002
	.section .rom.00630877, "ax"
	.incbin "baserom.gba", 0x00630877, 0x00000001
	.section .rom.00631411, "ax"
	.incbin "baserom.gba", 0x00631411, 0x00000003
	.section .rom.00632272, "ax"
	.incbin "baserom.gba", 0x00632272, 0x00000002
	.section .rom.006323ca, "ax"
	.incbin "baserom.gba", 0x006323ca, 0x00000002
	.section .rom.0063a217, "ax"
	.incbin "baserom.gba", 0x0063a217, 0x00000001
	.section .rom.0063a383, "ax"
	.incbin "baserom.gba", 0x0063a383, 0x00000001
	.section .rom.0063d11f, "ax"
	.incbin "baserom.gba", 0x0063d11f, 0x00000001
	.section .rom.0063f2ae, "ax"
	.incbin "baserom.gba", 0x0063f2ae, 0x00000002
	.section .rom.0063f9b6, "ax"
	.incbin "baserom.gba", 0x0063f9b6, 0x00000002
	.section .rom.006405ca, "ax"
	.incbin "baserom.gba", 0x006405ca, 0x00000002
	.section .rom.0065169d, "ax"
	.incbin "baserom.gba", 0x0065169d, 0x00000003
	.section .rom.00651832, "ax"
	.incbin "baserom.gba", 0x00651832, 0x00000002
	.section .rom.00652d42, "ax"
	.incbin "baserom.gba", 0x00652d42, 0x00000002
	.section .rom.00654b27, "ax"
	.incbin "baserom.gba", 0x00654b27, 0x00000001
	.section .rom.00655bba, "ax"
	.incbin "baserom.gba", 0x00655bba, 0x00000002
	.section .rom.00657e2a, "ax"
	.incbin "baserom.gba", 0x00657e2a, 0x00000002
	.section .rom.00657fc1, "ax"
	.incbin "baserom.gba", 0x00657fc1, 0x00000003
	.section .rom.00659f67, "ax"
	.incbin "baserom.gba", 0x00659f67, 0x00000001
	.section .rom.0065ed3e, "ax"
	.incbin "baserom.gba", 0x0065ed3e, 0x00000002
	.section .rom.0066279f, "ax"
	.incbin "baserom.gba", 0x0066279f, 0x00000001
	.section .rom.006654d5, "ax"
	.incbin "baserom.gba", 0x006654d5, 0x00000003
	.section .rom.0066781d, "ax"
	.incbin "baserom.gba", 0x0066781d, 0x00000003
	.section .rom.00668ffb, "ax"
	.incbin "baserom.gba", 0x00668ffb, 0x00000001
	.section .rom.006691ad, "ax"
	.incbin "baserom.gba", 0x006691ad, 0x00000003
	.section .rom.00679b4e, "ax"
	.incbin "baserom.gba", 0x00679b4e, 0x00000002
	.section .rom.00679cad, "ax"
	.incbin "baserom.gba", 0x00679cad, 0x00000003
	.section .rom.0067d407, "ax"
	.incbin "baserom.gba", 0x0067d407, 0x00000001
	.section .rom.0067d56a, "ax"
	.incbin "baserom.gba", 0x0067d56a, 0x00000002
	.section .rom.0068324d, "ax"
	.incbin "baserom.gba", 0x0068324d, 0x00000003
	.section .rom.00685c33, "ax"
	.incbin "baserom.gba", 0x00685c33, 0x00000001
	.section .rom.00687e3e, "ax"
	.incbin "baserom.gba", 0x00687e3e, 0x00000002
	.section .rom.00687f7f, "ax"
	.incbin "baserom.gba", 0x00687f7f, 0x00000001
	.section .rom.00689996, "ax"
	.incbin "baserom.gba", 0x00689996, 0x00000002
	.section .rom.006906fd, "ax"
	.incbin "baserom.gba", 0x006906fd, 0x00000003
	.section .rom.0069087b, "ax"
	.incbin "baserom.gba", 0x0069087b, 0x00000001
	.section .rom.00692825, "ax"
	.incbin "baserom.gba", 0x00692825, 0x00000003
	.section .rom.00694f9e, "ax"
	.incbin "baserom.gba", 0x00694f9e, 0x00000002
	.section .rom.00696805, "ax"
	.incbin "baserom.gba", 0x00696805, 0x00000003
	.section .rom.0069a226, "ax"
	.incbin "baserom.gba", 0x0069a226, 0x00000002
	.section .rom.0069a3ef, "ax"
	.incbin "baserom.gba", 0x0069a3ef, 0x00000001
	.section .rom.006a53d7, "ax"
	.incbin "baserom.gba", 0x006a53d7, 0x00000001
	.section .rom.006a7523, "ax"
	.incbin "baserom.gba", 0x006a7523, 0x00000001
	.section .rom.006a76ad, "ax"
	.incbin "baserom.gba", 0x006a76ad, 0x00000003
	.section .rom.006a9132, "ax"
	.incbin "baserom.gba", 0x006a9132, 0x00000002
	.section .rom.006ac169, "ax"
	.incbin "baserom.gba", 0x006ac169, 0x00000003
	.section .rom.006ac32f, "ax"
	.incbin "baserom.gba", 0x006ac32f, 0x00000001
	.section .rom.006ad263, "ax"
	.incbin "baserom.gba", 0x006ad263, 0x00000001
	.section .rom.006af316, "ax"
	.incbin "baserom.gba", 0x006af316, 0x00000002
	.section .rom.006af4bd, "ax"
	.incbin "baserom.gba", 0x006af4bd, 0x00000003
	.section .rom.006b44e7, "ax"
	.incbin "baserom.gba", 0x006b44e7, 0x00000001
	.section .rom.006b7277, "ax"
	.incbin "baserom.gba", 0x006b7277, 0x00000001
	.section .rom.006b7437, "ax"
	.incbin "baserom.gba", 0x006b7437, 0x00000001
	.section .rom.006b992b, "ax"
	.incbin "baserom.gba", 0x006b992b, 0x00000001
	.section .rom.006b9afd, "ax"
	.incbin "baserom.gba", 0x006b9afd, 0x00000003
	.section .rom.006bbb8e, "ax"
	.incbin "baserom.gba", 0x006bbb8e, 0x00000002
	.section .rom.006bdc8e, "ax"
	.incbin "baserom.gba", 0x006bdc8e, 0x00000002
	.section .rom.006c0842, "ax"
	.incbin "baserom.gba", 0x006c0842, 0x00000002
	.section .rom.006c132f, "ax"
	.incbin "baserom.gba", 0x006c132f, 0x00000001
	.section .rom.006c1502, "ax"
	.incbin "baserom.gba", 0x006c1502, 0x00000002
	.section .rom.006c3e31, "ax"
	.incbin "baserom.gba", 0x006c3e31, 0x00000003
	.section .rom.006c55c9, "ax"
	.incbin "baserom.gba", 0x006c55c9, 0x00000003
	.section .rom.006c683d, "ax"
	.incbin "baserom.gba", 0x006c683d, 0x00000003
	.section .rom.006c8ccd, "ax"
	.incbin "baserom.gba", 0x006c8ccd, 0x00000003
	.section .rom.006cb019, "ax"
	.incbin "baserom.gba", 0x006cb019, 0x00000003
	.section .rom.006cc203, "ax"
	.incbin "baserom.gba", 0x006cc203, 0x00000001
	.section .rom.006cc38a, "ax"
	.incbin "baserom.gba", 0x006cc38a, 0x00000002
	.section .rom.006d08d6, "ax"
	.incbin "baserom.gba", 0x006d08d6, 0x00000002
	.section .rom.006d0a2b, "ax"
	.incbin "baserom.gba", 0x006d0a2b, 0x00000001
	.section .rom.006d0b6b, "ax"
	.incbin "baserom.gba", 0x006d0b6b, 0x00000001
	.section .rom.006d183f, "ax"
	.incbin "baserom.gba", 0x006d183f, 0x00000001
	.section .rom.006d19be, "ax"
	.incbin "baserom.gba", 0x006d19be, 0x00000002
	.section .rom.006d3277, "ax"
	.incbin "baserom.gba", 0x006d3277, 0x00000001
	.section .rom.006d33f9, "ax"
	.incbin "baserom.gba", 0x006d33f9, 0x00000003
	.section .rom.006d4e8b, "ax"
	.incbin "baserom.gba", 0x006d4e8b, 0x00000001
	.section .rom.006d5005, "ax"
	.incbin "baserom.gba", 0x006d5005, 0x00000003
	.section .rom.006d6d43, "ax"
	.incbin "baserom.gba", 0x006d6d43, 0x00000001
	.section .rom.006d6e6f, "ax"
	.incbin "baserom.gba", 0x006d6e6f, 0x00000001
	.section .rom.006d6fe5, "ax"
	.incbin "baserom.gba", 0x006d6fe5, 0x00000003
	.section .rom.006d8e8b, "ax"
	.incbin "baserom.gba", 0x006d8e8b, 0x00000001
	.section .rom.006d9011, "ax"
	.incbin "baserom.gba", 0x006d9011, 0x00000003
	.section .rom.006d9f6f, "ax"
	.incbin "baserom.gba", 0x006d9f6f, 0x00000001
	.section .rom.006db21f, "ax"
	.incbin "baserom.gba", 0x006db21f, 0x00000001
	.section .rom.006db3da, "ax"
	.incbin "baserom.gba", 0x006db3da, 0x00000002
	.section .rom.006dde7e, "ax"
	.incbin "baserom.gba", 0x006dde7e, 0x00000002
	.section .rom.006ddff1, "ax"
	.incbin "baserom.gba", 0x006ddff1, 0x00000003
	.section .rom.006df3d9, "ax"
	.incbin "baserom.gba", 0x006df3d9, 0x00000003
	.section .rom.006e1eb9, "ax"
	.incbin "baserom.gba", 0x006e1eb9, 0x00000003
	.section .rom.006e2063, "ax"
	.incbin "baserom.gba", 0x006e2063, 0x00000001
	.section .rom.006e445e, "ax"
	.incbin "baserom.gba", 0x006e445e, 0x00000002
	.section .rom.006e45ed, "ax"
	.incbin "baserom.gba", 0x006e45ed, 0x00000003
	.section .rom.006e786d, "ax"
	.incbin "baserom.gba", 0x006e786d, 0x00000003
	.section .rom.006e7a4e, "ax"
	.incbin "baserom.gba", 0x006e7a4e, 0x00000002
	.section .rom.006e9fe3, "ax"
	.incbin "baserom.gba", 0x006e9fe3, 0x00000001
	.section .rom.006ea172, "ax"
	.incbin "baserom.gba", 0x006ea172, 0x00000002
	.section .rom.006ecd6a, "ax"
	.incbin "baserom.gba", 0x006ecd6a, 0x00000002
	.section .rom.006ed8b7, "ax"
	.incbin "baserom.gba", 0x006ed8b7, 0x00000001
	.section .rom.006eda1e, "ax"
	.incbin "baserom.gba", 0x006eda1e, 0x00000002
	.section .rom.006f0433, "ax"
	.incbin "baserom.gba", 0x006f0433, 0x00000001
	.section .rom.006f2072, "ax"
	.incbin "baserom.gba", 0x006f2072, 0x00000002
	.section .rom.006f252f, "ax"
	.incbin "baserom.gba", 0x006f252f, 0x00000001
	.section .rom.006f38a7, "ax"
	.incbin "baserom.gba", 0x006f38a7, 0x00000001
	.section .rom.006f4deb, "ax"
	.incbin "baserom.gba", 0x006f4deb, 0x00000001
	.section .rom.006f4f75, "ax"
	.incbin "baserom.gba", 0x006f4f75, 0x00000003
	.section .rom.006f6053, "ax"
	.incbin "baserom.gba", 0x006f6053, 0x00000001
	.section .rom.006f61e1, "ax"
	.incbin "baserom.gba", 0x006f61e1, 0x00000003
	.section .rom.006f7e35, "ax"
	.incbin "baserom.gba", 0x006f7e35, 0x00000003
	.section .rom.006fa817, "ax"
	.incbin "baserom.gba", 0x006fa817, 0x00000001
	.section .rom.006fdd41, "ax"
	.incbin "baserom.gba", 0x006fdd41, 0x00000003
	.section .rom.006ff64b, "ax"
	.incbin "baserom.gba", 0x006ff64b, 0x00000001
	.section .rom.007016f3, "ax"
	.incbin "baserom.gba", 0x007016f3, 0x00000001
	.section .rom.00704147, "ax"
	.incbin "baserom.gba", 0x00704147, 0x00000001
	.section .rom.00705d93, "ax"
	.incbin "baserom.gba", 0x00705d93, 0x00000001
	.section .rom.007071d9, "ax"
	.incbin "baserom.gba", 0x007071d9, 0x00000003
	.section .rom.00709059, "ax"
	.incbin "baserom.gba", 0x00709059, 0x00000003
	.section .rom.0070ac2a, "ax"
	.incbin "baserom.gba", 0x0070ac2a, 0x00000002
	.section .rom.0070d35e, "ax"
	.incbin "baserom.gba", 0x0070d35e, 0x00000002
	.section .rom.0070efa3, "ax"
	.incbin "baserom.gba", 0x0070efa3, 0x00000001
	.section .rom.007103e9, "ax"
	.incbin "baserom.gba", 0x007103e9, 0x00000003
	.section .rom.00711f67, "ax"
	.incbin "baserom.gba", 0x00711f67, 0x00000001
	.section .rom.00713197, "ax"
	.incbin "baserom.gba", 0x00713197, 0x00000001
	.section .rom.007132f1, "ax"
	.incbin "baserom.gba", 0x007132f1, 0x00000003
	.section .rom.007167f7, "ax"
	.incbin "baserom.gba", 0x007167f7, 0x00000001
	.section .rom.00716962, "ax"
	.incbin "baserom.gba", 0x00716962, 0x00000002
	.section .rom.007192b2, "ax"
	.incbin "baserom.gba", 0x007192b2, 0x00001166
	.section .rom.0071bbfd, "ax"
	.incbin "baserom.gba", 0x0071bbfd, 0x00000003
	.section .rom.0071ee43, "ax"
	.incbin "baserom.gba", 0x0071ee43, 0x00000001
	.section .rom.0072173a, "ax"
	.incbin "baserom.gba", 0x0072173a, 0x00000002
	.section .rom.0072aa73, "ax"
	.incbin "baserom.gba", 0x0072aa73, 0x00000001
	.section .rom.0072bda2, "ax"
	.incbin "baserom.gba", 0x0072bda2, 0x00000002
	.section .rom.0072cf9b, "ax"
	.incbin "baserom.gba", 0x0072cf9b, 0x00000001
	.section .rom.0072d0fd, "ax"
	.incbin "baserom.gba", 0x0072d0fd, 0x00000003
	.section .rom.0072fac9, "ax"
	.incbin "baserom.gba", 0x0072fac9, 0x00000003
	.section .rom.00732ff5, "ax"
	.incbin "baserom.gba", 0x00732ff5, 0x00000003
	.section .rom.00737437, "ax"
	.incbin "baserom.gba", 0x00737437, 0x00000001
	.section .rom.007375aa, "ax"
	.incbin "baserom.gba", 0x007375aa, 0x00000002
	.section .rom.0073b80f, "ax"
	.incbin "baserom.gba", 0x0073b80f, 0x00000001
	.section .rom.0073f02a, "ax"
	.incbin "baserom.gba", 0x0073f02a, 0x00000002
	.section .rom.00740a52, "ax"
	.incbin "baserom.gba", 0x00740a52, 0x00000002
	.section .rom.007430ff, "ax"
	.incbin "baserom.gba", 0x007430ff, 0x00000001
	.section .rom.00745163, "ax"
	.incbin "baserom.gba", 0x00745163, 0x00000001
	.section .rom.0074701f, "ax"
	.incbin "baserom.gba", 0x0074701f, 0x00000001
	.section .rom.0074a269, "ax"
	.incbin "baserom.gba", 0x0074a269, 0x00000003
	.section .rom.0074bc2f, "ax"
	.incbin "baserom.gba", 0x0074bc2f, 0x00000001
	.section .rom.0074bda1, "ax"
	.incbin "baserom.gba", 0x0074bda1, 0x00000003
	.section .rom.0074edae, "ax"
	.incbin "baserom.gba", 0x0074edae, 0x00000002
	.section .rom.0074ef66, "ax"
	.incbin "baserom.gba", 0x0074ef66, 0x00000002
	.section .rom.007507d7, "ax"
	.incbin "baserom.gba", 0x007507d7, 0x00000001
	.section .rom.00750956, "ax"
	.incbin "baserom.gba", 0x00750956, 0x00000002
	.section .rom.0075415f, "ax"
	.incbin "baserom.gba", 0x0075415f, 0x00000001
	.section .rom.007542d3, "ax"
	.incbin "baserom.gba", 0x007542d3, 0x00000001
	.section .rom.0075509b, "ax"
	.incbin "baserom.gba", 0x0075509b, 0x00000001
	.section .rom.00755241, "ax"
	.incbin "baserom.gba", 0x00755241, 0x00000003
	.section .rom.0075729c, "ax"
	.incbin "baserom.gba", 0x0075729c, 0x000000e4
	.section .rom.0075a097, "ax"
	.incbin "baserom.gba", 0x0075a097, 0x00000001
	.section .rom.0075a27e, "ax"
	.incbin "baserom.gba", 0x0075a27e, 0x00000002
	.section .rom.0075d51a, "ax"
	.incbin "baserom.gba", 0x0075d51a, 0x00004602
	.section .rom.00761c22, "ax"
	.incbin "baserom.gba", 0x00761c22, 0x00000002
	.section .rom.007637f5, "ax"
	.incbin "baserom.gba", 0x007637f5, 0x00000003
	.section .rom.007652a5, "ax"
	.incbin "baserom.gba", 0x007652a5, 0x00000003
	.section .rom.00765401, "ax"
	.incbin "baserom.gba", 0x00765401, 0x00000003
	.section .rom.0076b647, "ax"
	.incbin "baserom.gba", 0x0076b647, 0x00000001
	.section .rom.0076b74b, "ax"
	.incbin "baserom.gba", 0x0076b74b, 0x00000001
	.section .rom.0076c167, "ax"
	.incbin "baserom.gba", 0x0076c167, 0x00000001
	.section .rom.0076c24d, "ax"
	.incbin "baserom.gba", 0x0076c24d, 0x00000003
	.section .rom.0076c4e9, "ax"
	.incbin "baserom.gba", 0x0076c4e9, 0x00000003
	.section .rom.00770546, "ax"
	.incbin "baserom.gba", 0x00770546, 0x00000002
	.section .rom.00770afa, "ax"
	.incbin "baserom.gba", 0x00770afa, 0x00000002
	.section .rom.00771a8f, "ax"
	.incbin "baserom.gba", 0x00771a8f, 0x00000001
	.section .rom.00773492, "ax"
	.incbin "baserom.gba", 0x00773492, 0x00000002
	.section .rom.00774101, "ax"
	.incbin "baserom.gba", 0x00774101, 0x00000003
	.section .rom.007743a9, "ax"
	.incbin "baserom.gba", 0x007743a9, 0x00000003
	.section .rom.0077522e, "ax"
	.incbin "baserom.gba", 0x0077522e, 0x00000002
	.section .rom.007763bf, "ax"
	.incbin "baserom.gba", 0x007763bf, 0x00000001
	.section .rom.00776587, "ax"
	.incbin "baserom.gba", 0x00776587, 0x00000001
	.section .rom.007768d3, "ax"
	.incbin "baserom.gba", 0x007768d3, 0x0000051d
	.section .rom.0077713b, "ax"
	.incbin "baserom.gba", 0x0077713b, 0x0000051d
	.section .rom.007779a3, "ax"
	.incbin "baserom.gba", 0x007779a3, 0x0000051d
	.section .rom.0077820b, "ax"
	.incbin "baserom.gba", 0x0077820b, 0x0000051d
	.section .rom.00778a73, "ax"
	.incbin "baserom.gba", 0x00778a73, 0x0000051d
	.section .rom.007792db, "ax"
	.incbin "baserom.gba", 0x007792db, 0x0000051d
	.section .rom.00779b43, "ax"
	.incbin "baserom.gba", 0x00779b43, 0x0000051d
	.section .rom.0077a3ab, "ax"
	.incbin "baserom.gba", 0x0077a3ab, 0x00085c55
