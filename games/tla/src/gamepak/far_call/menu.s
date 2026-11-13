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
	.global FileList_SelectActionFar
	.thumb_func
FileList_SelectActionFar:
	overlay_veneer FileList_SelectAction
	.global Func_080381f8
	.thumb_func
Func_080381f8:
	overlay_veneer IconMenu_ChooseSendOption
