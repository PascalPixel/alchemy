@ Far-call stub table heading the battle effect code at 0x080c9000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_EffectTable
FarCall_EffectTable:
	.global Func_080c9000
	.thumb_func
Func_080c9000:
	overlay_veneer BattleEffect_LoadWork
	.global BattleFx_DispatchByIdRangeFar
	.thumb_func
BattleFx_DispatchByIdRangeFar:
	overlay_veneer BattleFx_DispatchByIdRange
	.global BattleFx_InitializeModeFar
	.thumb_func
BattleFx_InitializeModeFar:
	overlay_veneer BattleFx_InitializeMode
	.global BattleFx_DispatchModeFar
	.thumb_func
BattleFx_DispatchModeFar:
	overlay_veneer BattleFx_DispatchMode
	.global Func_080c9020
	.thumb_func
Func_080c9020:
	overlay_veneer Unnamed_080cb7f8
	.global BattleEffect_RunTileAndPaletteAnimationFar
	.thumb_func
BattleEffect_RunTileAndPaletteAnimationFar:
	overlay_veneer BattleEffect_RunTileAndPaletteAnimation
	.global Func_080c9030
	.thumb_func
Func_080c9030:
	overlay_veneer Func_080cc5d8
	.global BattlePresentation_PrepareSceneFar
	.thumb_func
BattlePresentation_PrepareSceneFar:
	overlay_veneer BattlePresentation_PrepareScene
	.global BattleFx_ScheduleCallbacksAndReleaseBlocksFar
	.thumb_func
BattleFx_ScheduleCallbacksAndReleaseBlocksFar:
	overlay_veneer BattleFx_ScheduleCallbacksAndReleaseBlocks