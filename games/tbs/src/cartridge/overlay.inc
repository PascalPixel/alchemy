@ The head of an overlay's two stub tables, its entry table and its import
@ table, and of the far-call banks' stub.inc: Thumb code on a word boundary,
@ and the stub. After the head, for an import table alone, the list of the
@ stubs an overlay imports.
	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/cartridge/overlay.s"
@ The stubs an overlay imports, in the order every overlay keeps them:
@ Camelot's four divide routines, then the far-call entries in the order of
@ the main image (games/tbs/src/cartridge/far_call, by bank). The list is here, in
@ the file every stub table includes, and only an import table reads it: an
@ overlay's import table sets `overlay_imports`, then `import.NAME` for each
@ stub its overlay calls, and includes this file: those rows, in this order,
@ are the overlay's table. An entry table and a
@ far-call bank do not set the symbol, and the list is nothing to them.
@ A row is `import NAME, TARGET`: the overlay's code calls NAME, and the
@ stub goes to TARGET. NAME is the name of the routine TARGET reaches, so
@ overlays call the main image's routines by their own names. The exceptions
@ are the divide routines, which overlays call under the compiler's names and
@ the Math_ names, and the few routines whose calls in the maps differ from
@ their prototypes (games/tbs/include/event.h says how).
	.macro import name, target
	.ifdef import.\name
	export \name, \target
	.endif
	.endm
	.ifdef overlay_imports
@ divide routines in IWRAM
	import Math_DivideSigned, IwramSignedDivide
	import __divsi3, IwramSignedDivide
	import __modsi3, IwramSignedRemainder
	import Math_DivideUnsigned, IwramUnsignedDivide
	import __udivsi3, IwramUnsignedDivide
	import Math_RemainderUnsigned, IwramUnsignedRemainder
	import __umodsi3, IwramUnsignedRemainder
@ Runtime bank (far_call/RUNTIME.S)
	import wait, waitFar
	import Task_Add, Task_AddFar
	import Task_Remove, Task_RemoveFar
	import Import_RandomNext, Random16Far
	import ArcTan2, ArcTan2Far
	import FixedSqrt, FixedSqrtFar
	import Trig_Sin, Trig_SinFar
	import Trig_Cos, Trig_CosFar
	import Vector_AddPolarOffset, Vector_AddPolarOffsetFar
	import Irq_SetHandler, Irq_SetHandlerFar
	import HeapSlot_Alloc, HeapSlot_AllocFar
	import HeapSlot_AllocEwram, HeapSlot_AllocEwramFar
	import HeapSlot_Release, HeapSlot_ReleaseFar
	import Heap_Alloc, Heap_AllocFar
	import Heap_AllocEwram, Heap_AllocEwramFar
	import Heap_Release, Heap_ReleaseFar
	import Decompress_ByteLz, Decompress_ByteLzFar
	import Decompress_TaggedLz, Decompress_TaggedLzFar
	import VramCache_FreeEntry, VramCache_FreeEntryFar
	import VramCache_ReleaseVram, VramCache_ReleaseVramFar
	import VramCache_Load, VramCache_LoadFar
	import VramCache_FindFreeEntry, VramCache_FindFreeEntryFar
	import OamMatrix_AllocFromTransform, OamMatrix_AllocFromTransformFar
	import Oam_AddSpriteAtDepth, Oam_AddSpriteAtDepthFar
	import Window_LoadGraphics, Window_LoadGraphicsFar
	import Resource_GetTableEntry, Resource_GetTableEntryFar
	import Stub_NoOpHook, Stub_NoOpHookFar
	import ScreenFade_ToBlack, ScreenFade_ToBlackFar
	import ScreenFade_Wait, ScreenFade_WaitFar
	import Serial_Initialize, Serial_InitializeFar
	import Serial_PollAndTransfer, Serial_PollAndTransferFar
	import IoWriteQueue_Store16, IoWriteQueue_Store16Far
	import IoWriteQueue_Store32, IoWriteQueue_Store32Far
	import Serial_RemoveIrqHandlers, Serial_RemoveIrqHandlersFar
	import Sound_LoadReverbCode, Sound_LoadReverbCodeFar
	import Serial_StartSend, Serial_StartSendFar
	import Serial_StartReceive, Serial_StartReceiveFar
	import Serial_GetActiveTransfers, Serial_GetActiveTransfersFar
	import Task_SetFlags, Task_SetFlagsFar
@ Field bank (far_call/FIELD.S)
	import Sprite_StartAnimation, Sprite_StartAnimationFar
	import Sprite_AddAnimation, Sprite_AddAnimationFar
	import SpriteAnimation_Clear, SpriteAnimation_ClearFar
	import Actor_SelectAnimation, Actor_SelectAnimationFar
	import Actor_SetAnimationSpeed, Actor_SetAnimationSpeedFar
	import Actor_StartScript, Actor_StartScriptFar
	import Actor_Create, Actor_CreateFar
	import Actor_Release, Actor_ReleaseFar
	import Actor_StartCamera, Actor_StartCameraFar
	import MapGrid_CenterOnFollowedPoint, MapGrid_CenterOnFollowedPointFar
	import Actor_StartIdle, Actor_StartIdleFar
	import Actor_WaitForScriptEnd, Actor_WaitForScriptEndFar
	import Actor_SetMoveTarget, Actor_SetMoveTargetFar
	import Actor_WaitForMove, Actor_WaitForMoveFar
	import WorldMap_LoadDefaultCellsAndUpdateBlock, WorldMap_LoadDefaultCellsAndUpdateBlockFar
	import MapGrid_PlayMapEdit, MapGrid_PlayMapEditFar
	import MapGrid_CopyCellPictureIndicesRect, MapGrid_CopyCellPictureIndicesRectFar
	import TilesetAnim_Resume, TilesetAnim_ResumeFar
	import TilesetAnim_Pause, TilesetAnim_PauseFar
	import BlendScript_Resume, BlendScript_ResumeFar
	import BlendScript_Suspend, BlendScript_SuspendFar
	import MapGrid_GetTerrainHeight, MapGrid_GetTerrainHeightFar
	import MapGrid_GetCellBehaviorAt, MapGrid_GetCellBehaviorAtFar
	import MapGrid_CopyCellPictureCellsRect, MapGrid_CopyCellPictureCellsRectFar
	import MapGrid_CopyCellAttributeRect, MapGrid_CopyCellAttributeRectFar
	import MapGrid_CopyCellIndexRect, MapGrid_CopyCellIndexRectFar
	import WorldMap_SetWindowCellTile, WorldMap_SetWindowCellTileFar
	import MapGrid_CheckCollision, MapGrid_CheckCollisionFar
	import Actor_SetSpriteFlags, Actor_SetSpriteFlagsFar
	import Actor_SetBlendMode, Actor_SetBlendModeFar
	import MapGrid_SetScreenShake, MapGrid_SetScreenShakeFar
	import MapGrid_WaitForShakeToSettle, MapGrid_WaitForShakeToSettleFar
	import ActorSystem_SuspendProjectionAndShowSprites, ActorSystem_SuspendProjectionAndShowSpritesFar
	import ActorSystem_ResumeProjectionAndHideLayers, ActorSystem_ResumeProjectionAndHideLayersFar
	import Actor_RegisterSpriteMetadata, Actor_RegisterSpriteMetadataFar
	import WorldMap_StartStillPicture, WorldMap_StartStillPictureFar
	import WorldMap_LoadAreaGraphics, WorldMap_LoadAreaGraphicsFar
	import Actor_SetSpriteColorMode, Actor_SetSpriteColorModeFar
	import Sprite_SetAnimationColorModes, Sprite_SetAnimationColorModesFar
	import MapGrid_SetShownLayers, MapGrid_SetShownLayersFar
	import MapGrid_SetCellBehaviorAt, MapGrid_SetCellBehaviorAtFar
@ Window bank (far_call/WINDOW.S)
	import Window_Create, Window_CreateFar
	import Window_Close, Window_CloseFar
	import Message_DecodeRawUnbounded, Message_DecodeRawUnboundedFar
	import Message_OpenWindow, Message_OpenWindowFar
	import FieldMessage_Show, FieldMessage_ShowFar
	import Message_AllTyped, Message_AllTypedFar
	import WindowSprite_PrepareForRedraw, WindowSprite_PrepareForRedrawFar
	import Window_DrawDividerLine, Window_DrawDividerLineFar
	import Text_DrawMessage, Text_DrawMessageFar
	import Text_DrawMessageAsTiles, Text_DrawMessageAsTilesFar
	import Text_DrawString, Text_DrawStringFar
	import Text_DrawStringAtOffset, Text_DrawStringAtOffsetFar
	import Text_DrawStringInWindow, Text_DrawStringInWindowFar
	import Text_DrawNumber, Text_DrawNumberFar
	import Text_DrawNumberAtOffset, Text_DrawNumberAtOffsetFar
	import Text_DrawNumberInWindow, Text_DrawNumberInWindowFar
	import Portrait_OpenWindow, Portrait_OpenWindowFar
	import MessageArgument_ClearAll, MessageArgument_ClearAllFar
	import MessageArgument_Push, MessageArgument_PushFar
	import Message_CloseFinishedWindows, Message_CloseFinishedWindowsFar
	import FileList_SelectAction, FileList_SelectActionFar
	import IconMenu_ChooseSendOption, IconMenu_ChooseSendOptionFar
	import FieldMessage_ShowCentered, FieldMessage_ShowCenteredFar
	import MenuArrow_Create, MenuArrow_CreateFar
	import MenuArrow_SetPosition, MenuArrow_SetPositionFar
	import MenuArrow_Release, MenuArrow_ReleaseFar
	import Icon_DrawItem, Icon_DrawItemFar
	import WindowSprite_RedrawSavedRect, WindowSprite_RedrawSavedRectFar
	import WindowSprite_ClearList, WindowSprite_ClearListFar
	import Window_SetTilemapEntry, Window_SetTilemapEntryFar
	import Save_ToChosenFile, Save_ToChosenFileFar
	import Save_ChooseAndLoadFile, Save_ChooseAndLoadFileFar
	import Save_CopyFile, Save_CopyFileFar
	import Save_EraseSelectedFile, Save_EraseSelectedFileFar
	import Save_ProcessSelectedFile, Save_ProcessSelectedFileFar
	import Save_ScanFileSummaries, Save_ScanFileSummariesFar
	import NameEntry_EditMemberName, NameEntry_EditMemberNameFar
	import DebugBrowser_Icons, DebugBrowser_IconsFar
	import DebugBrowser_Portraits, DebugBrowser_PortraitsFar
	import WindowColor_Apply, WindowColor_ApplyFar
	import YesNoMenu_Run, YesNoMenu_RunFar
	import Menu_SelectCoinsOrMedals, Menu_SelectCoinsOrMedalsFar
	import ColossoMenu_Choose, ColossoMenu_ChooseFar
	import Resource_LoadFixedBlockBIntoFreeSlot, Resource_LoadFixedBlockBIntoFreeSlotFar
	import Graphics_ExpandVramTilesByColorTable, Graphics_ExpandVramTilesByColorTableFar
	import Menu_AnimateSelectionToEntry, Menu_AnimateSelectionToEntryFar
	import Save_CountFilesExcludingSent, Save_CountFilesExcludingSentFar
	import Link_DrawPageArrows, Link_DrawPageArrowsFar
@ Game bank (far_call/GAME.S)
	import Djinn_GetRecoveryTable, Djinn_GetRecoveryTableFar
	import Unit_Get, Unit_GetFar
	import Unit_Calculate, Unit_CalculateFar
	import Item_GetDirect, Item_GetDirectFar
	import Inventory_AddItem, Inventory_AddItemFar
	import PartyInventory_Add, PartyInventory_AddFar
	import Inventory_Find, Inventory_FindFar
	import PartyInventory_FindMember, PartyInventory_FindMemberFar
	import PartyInventory_Remove, PartyInventory_RemoveFar
	import Inventory_Equip, Inventory_EquipFar
	import Ability_GetDirect, Ability_Get
	import MainRoutineWork_InitializeDefaults, MainRoutineWork_InitializeDefaultsFar
	import GameFlag_Get, GameFlag_GetFar
	import GameFlag_Set, GameFlag_SetFar
	import Import_GameFlagSet, GameFlag_SetFar
	import GameFlag_Clear, GameFlag_ClearFar
	import GameFlag_GetByte, GameFlag_GetByteFar
	import GameFlag_SetByte, GameFlag_SetByteFar
	import Unit_AddHp, Unit_AddHpFar
	import Unit_AddPp, Unit_AddPpFar
	import Unit_RecalculateGauges, Unit_RecalculateGaugesFar
	import Party_CountMembers, Party_CountMembersFar
	import Party_AddMember, Party_AddMemberFar
	import Party_ListMembers, Party_ListMembersFar
	import Party_RemoveMember, Party_RemoveMemberFar
	import Djinn_Give, Djinn_GiveFar
	import Djinn_Set, Djinn_SetFar
	import DjinnRecovery_AddStandby, DjinnRecovery_AddStandbyFar
	import Djinn_Transfer, Djinn_TransferFar
	import Unit_LevelUpTo, Unit_LevelUpToFar
	import Party_AdjustCoins, Party_AdjustCoinsFar
	import Item_AdjustCounter, Item_AdjustCounterFar
	import Inventory_Count, Inventory_CountFar
	import PartyInventory_HasSpace, PartyInventory_HasSpaceFar
	import Party_RestoreHpPp, Party_RestoreHpPpFar
	import Party_RestorePpOfLitElements, Party_RestorePpOfLitElementsFar
	import Party_StartChildhood, Party_StartChildhoodFar
	import Party_EndChildhood, Party_EndChildhoodFar
	import Inventory_Discard, Inventory_DiscardFar
	import Party_GetAverageLevel, Party_GetAverageLevelFar
	import PartyInventory_CountItem, PartyInventory_CountItemFar
	import PartyInventory_Discard, PartyInventory_DiscardFar
	import BuildStamp_GetTime, BuildStamp_GetTimeFar
	import PartyInventory_CountFreeSlots, PartyInventory_CountFreeSlotsFar
	import InventorySnapshot_Save, InventorySnapshot_SaveFar
	import InventorySnapshot_Restore, InventorySnapshot_RestoreFar
@ Overworld bank (far_call/OVERWORLD.S)
	import Event_Wait, Event_WaitFar
	import Event_Start, Event_StartFar
	import Event_End, Event_EndFar
	import Event_CallWithLastActiveActorId, Event_CallWithLastActiveActorIdFar
	import Scene_AssignViewFlags, Scene_AssignViewFlagsFar
	import Event_SetTriggers, Event_SetTriggersFar
	import Event_PrepareActorAndApplyValue, Event_PrepareActorAndApplyValueFar
	import Event_PrepareTwoActorsAndApply, Event_PrepareTwoActorsAndApplyFar
	import Party_RemoveMemberRestored, Party_RemoveMemberRestoredFar
	import PartyInventory_GiveItem, PartyInventory_GiveItemFar
	import Event_AskYesNoWithActorAnimation, Event_AskYesNoWithActorAnimationFar
	import Party_LearnAbility, Party_LearnAbilityFar
	import Actor_GetById, Actor_GetByIdFar
	import Actor_ReleaseById, Actor_ReleaseByIdFar
	import Actor_SetSpeedParameters, Actor_SetSpeedParametersFar
	import Actor_SetMovementTypeById, Actor_SetMovementTypeByIdFar
	import Actor_RefreshSelectorById, Actor_RefreshSelectorByIdFar
	import Actor_Stop, Actor_StopFar
	import Actor_SetMovementTypeAndWaitById, Actor_SetMovementTypeAndWaitByIdFar
	import Actor_ResetAndSetPosition, Actor_ResetAndSetPositionFar
	import Actor_SetPositionAndCommit, Actor_SetPositionAndCommitFar
	import Actor_ResetAndSetPositionInMode2, Actor_ResetAndSetPositionInMode2Far
	import Actor_SetPositionAndReset, Actor_SetPositionAndResetFar
	import Actor_CenterOnColumn, Actor_CenterOnColumnFar
	import Actor_OffsetPositionAndResetMotion, Actor_OffsetPositionAndResetMotionFar
	import Actor_WaitForMoveById, Actor_WaitForMoveByIdFar
	import Import_ActorSetPosition, Actor_SetHorizontalPositionWithTerrainFar
	import Import_ActorSetAnimation, Actor_SetModeByIdFar
	import Actor_SetAnimationSpeedById, Actor_SetAnimationSpeedByIdFar
	import Actor_SetAnimationAndWait, Actor_SetAnimationAndWaitFar
	import Actor_WaitForAnimationChange, Actor_WaitForAnimationChangeFar
	import Actor_Jump, Actor_JumpFar
	import Actor_StartRepeatedMotion, Actor_StartRepeatedMotionFar
	import Actor_RunRepeatedMotion, Actor_RunRepeatedMotionFar
	import Actor_JumpDownLedge, Actor_JumpDownLedgeFar
	import Actor_SetAngleToward, Actor_SetAngleTowardFar
	import Actor_TurnToFaceEachOther, Actor_TurnToFaceEachOtherFar
	import Actor_ConfigureSpriteColor, Actor_ConfigureSpriteColorFar
	import Actor_SetSpriteColor, Actor_SetSpriteColorFar
	import Actor_SetTargetAndCallback, Actor_SetTargetAndCallbackFar
	import Event_SetMessageId, Event_SetMessageIdFar
	import FieldMessage_ShowAtActor, FieldMessage_ShowAtActorFar
	import Event_ShowMessageAndWait, Event_ShowMessageAndWaitFar
	import Event_ShowMessageThenWait, Event_ShowMessageThenWaitFar
	import Event_AskYesNoAndShowResponse, Event_AskYesNoAndShowResponseFar
	import Actor_ArmCallback, Actor_ArmCallbackFar
	import Import_EventShowTwoMessagesAndWait, Event_ShowTwoMessagesAndWaitFar
	import Message_SetVoiceFromActor, Message_SetVoiceFromActorFar
	import Actor_SetSpritePriority, Actor_SetSpritePriorityFar
	import FieldEffect_SpawnAttached, FieldEffect_SpawnAttachedFar
	import FieldEffect_SetAttached, FieldEffect_SetAttachedFar
	import Actor_CopyGraphics, Actor_CopyGraphicsFar
	import FieldCamera_FollowActor, FieldCamera_FollowActorFar
	import FieldCamera_SetSpeedParameters, FieldCamera_SetSpeedParametersFar
	import FieldCamera_MoveWithinMapBounds, FieldCamera_MoveWithinMapBoundsFar
	import FieldCamera_WaitForArrival, FieldCamera_WaitForArrivalFar
	import Actor_PlaceCurrentWithinCameraBounds, Actor_PlaceCurrentWithinCameraBoundsFar
	import FieldCamera_GetActor, FieldCamera_GetActorFar
	import MapGrid_SetCameraBounds, MapGrid_SetCameraBoundsFar
	import Event_SetDestination, Event_SetDestinationFar
	import Import_EventSetExitRequest, Event_SetExitRequestFar
	import Field_RequestBattleFromEncounterTable, Field_RequestBattleFromEncounterTableFar
	import Field_RequestDjinnBattle, Field_RequestDjinnBattleFar
	import Event_SetAfterBattleDestination, Event_SetAfterBattleDestinationFar
	import Event_SetDefeatDestination, Event_SetDefeatDestinationFar
	import Event_LeaveFor, Event_LeaveForFar
	import Field_TryJumpForward, Field_TryJumpForwardFar
	import Field_ClimbDown, Field_ClimbDownFar
	import Field_ClimbUp, Field_ClimbUpFar
	import Leader_StopMoving, Leader_StopMovingFar
	import Weather_StartRain, Weather_StartRainFar
	import Weather_StartLightning, Weather_StartLightningFar
	import Weather_StopLightning, Weather_StopLightningFar
	import Weather_LastLightning, Weather_LastLightningFar
	import Weather_ResumeLightning, Weather_ResumeLightningFar
	import Weather_StartLightningFlash, Weather_StartLightningFlashFar
	import FieldEffect_InitializeSparkles, FieldEffect_InitializeSparklesFar
	import ScreenTransition_InitializeBattleEffectState, ScreenTransition_InitializeBattleEffectStateFar
	import PaletteFade_SetTarget, PaletteFade_SetTargetFar
	import PaletteFade_SetSource, PaletteFade_SetSourceFar
	import PaletteFade_StartInterpolation, PaletteFade_StartInterpolationFar
	import Event_StartScreenTransition, Event_StartScreenTransitionFar
	import Event_FinishScreenTransition, Event_FinishScreenTransitionFar
	import Event_WaitTransitionFrames, Event_WaitTransitionFramesFar
	import LineScroll_InitWave, LineScroll_InitWaveFar
	import LineScroll_Stop, LineScroll_StopFar
	import FieldEffect_PlayCueAndStartEmitterOnTarget, FieldEffect_PlayCueAndStartEmitterOnTargetFar
	import ItemDisplay_ShowPicture, ItemDisplay_ShowPictureFar
	import ItemDisplay_ShowItem, ItemDisplay_ShowItemFar
	import TreasureSpot_SetActorPosition, TreasureSpot_SetActorPositionFar
	import TreasureSpot_MoveLeaderAway, TreasureSpot_MoveLeaderAwayFar
	import WorldMap_Freeze, WorldMap_FreezeFar
	import WorldMap_Unfreeze, WorldMap_UnfreezeFar
	import Actor_Lookup, Actor_LookupFar
	import FieldEffect_StartFadeOverlay, FieldEffect_StartFadeOverlayFar
	import FieldPsynergy_RunHandler, FieldPsynergy_RunHandlerFar
	import FieldPsynergy_LoadResources, FieldPsynergy_LoadResourcesFar
	import FieldPsynergy_SetupActorPair, FieldPsynergy_SetupActorPairFar
	import FieldPsynergy_StopCasterUpdate, FieldPsynergy_StopCasterUpdateFar
	import FieldPsynergy_End, FieldPsynergy_EndFar
	import FieldPsynergy_StartCast, FieldPsynergy_StartCastFar
	import Field_EndBusyAndReveal, Field_EndBusyAndRevealFar
	import FieldPsynergy_StartMindReadWave, FieldPsynergy_StartMindReadWaveFar
	import FieldPsynergy_FinishWaveAndReleaseHeapBlock, FieldPsynergy_FinishWaveAndReleaseHeapBlockFar
	import FieldPsynergy_RestoreMindReadPalette, FieldPsynergy_RestoreMindReadPaletteFar
	import Sprite_ShareVramCacheEntry, Sprite_ShareVramCacheEntryFar
	import Party_AddPp, Party_AddPpFar
	import FieldYesNo_Ask, FieldYesNo_AskFar
	import Scene_PlayMusic, Scene_PlayMusicFar
	import Djinn_JoinParty, Djinn_JoinPartyFar
	import FieldParticle_HasPendingTarget, FieldParticle_HasPendingTargetFar
	import FieldParticle_SetActorMode, FieldParticle_SetActorModeFar
	import FieldParticle_SetCallback, FieldParticle_SetCallbackFar
	import FieldParticle_Initialize, FieldParticle_InitializeFar
	import FieldEffect_ClearOwnedSlot, FieldEffect_ClearOwnedSlotFar
	import Camera_WorldToScreen, Camera_WorldToScreenFar
	import Event_ClearInvalidPackedValues, Event_ClearInvalidPackedValuesFar
	import FieldEffect_InitializeSlots, FieldEffect_InitializeSlotsFar
	import FieldEffect_ClearActiveSlotsAndScheduleUpdates, FieldEffect_ClearActiveSlotsAndScheduleUpdatesFar
	import Actor_LaunchFromLeader, Actor_LaunchFromLeaderFar
	import Actor_OffsetPositionAndReset, Actor_OffsetPositionAndResetFar
	import Actor_CommitPositionAndActivate, Actor_CommitPositionAndActivateFar
	import Actor_WatchAndSetScript, Actor_WatchAndSetScriptFar
	import FieldPsynergy_HaltActor, FieldPsynergy_HaltActorFar
	import Actor_Snapshot, Save_CaptureActorsFar
	import Actor_Restore, Actor_RestoreFar
	import FieldEffect_ScheduleRatioTransition, FieldEffect_ScheduleRatioTransitionFar
	import Event_WaitForZoomTransition, Event_WaitForZoomTransitionFar
	import Actor_SetMovementType, Actor_SetMovementTypeFar
	import Scene_QueueMusic, Scene_QueueMusicFar
	import Scene_PlayQueuedMusic, Scene_PlayQueuedMusicFar
@ Menu bank (far_call/MENU.S)
	import PartyMemberChooser_Open, PartyMemberChooser_OpenFar
	import DjinnMenu_Run, DjinnMenu_RunFar
	import ItemMenu_DrawItemDetails, ItemMenu_DrawItemDetailsFar
@ Shop bank (far_call/SHOP.S)
	import Shop_OpenDebugShop, Shop_OpenDebugShopFar
	import Shop_Run, Shop_RunFar
	import Sanctum_Run, Sanctum_RunFar
	import Inn_CheckIn, Inn_CheckInFar
	import Shop_SaveAndTintFieldPalette, Shop_SaveAndTintFieldPaletteFar
	import Shop_RestoreFieldPalette, Shop_RestoreFieldPaletteFar
	import ItemPicker_Run, ItemPicker_RunFar
	import Sound_WaitForJingle, Sound_WaitForJingleFar
	import Inn_PlaySleep, Inn_PlaySleepFar
@ Battle bank (far_call/BATTLE.S)
	import DjinnRecovery_CountDownTwenty, DjinnRecovery_CountDownTwentyFar
	import DebugParty_LoadPreset, DebugParty_LoadPresetFar
@ Credits bank (far_call/CREDITS.S)
	import Import_CreditsRun, Credits_RunFar
@ Title bank (far_call/TITLE.S)
	import Title_ShowIntro, Title_ShowIntroFar
	import Title_ShowNintendoLogo, Title_ShowNintendoLogoFar
	import Import_TitleShowCamelotLogo, Title_ShowCamelotLogoFar
@ Sound bank (far_call/SOUND.S)
	import Sound_Play, Sound_PlayFar
	import Sound_SetBgmTempo, Sound_SetBgmTempoFar
	import Sound_GetBgmRestoreDelay, Sound_GetBgmRestoreDelayFar
	import Sound_WaitForBgmRestore, Sound_WaitForBgmRestoreFar
	.endif
