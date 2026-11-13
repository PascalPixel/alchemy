.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Portrait_GetForSpeakerFar
	.thumb_func
Portrait_GetForSpeakerFar:
	overlay_veneer Portrait_GetForSpeaker
	.global FieldMenu_RunFar
	.thumb_func
FieldMenu_RunFar:
	overlay_veneer FieldMenu_Run
	.global Menu_SelectSaveSlotActionFar
	.thumb_func
Menu_SelectSaveSlotActionFar:
	overlay_veneer Menu_SelectSaveSlotAction
	.global Func_080381f8
	.thumb_func
Func_080381f8:
	overlay_veneer Func_0804d7fc
