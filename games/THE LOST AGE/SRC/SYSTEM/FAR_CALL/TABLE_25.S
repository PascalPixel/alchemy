@ Far-call stub table at 0x080c8268: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Event_SetDestinationFar
	.thumb_func
Event_SetDestinationFar:
	overlay_veneer EventRuntime_SetDestination
	.global EventRuntime_SetSavedDestinationFar
	.thumb_func
EventRuntime_SetSavedDestinationFar:
	overlay_veneer EventRuntime_SetSavedDestination
	.global EventRuntime_SetCountdownFar
	.thumb_func
EventRuntime_SetCountdownFar:
	overlay_veneer EventRuntime_SetCountdown
	.global BattleFx_SetWeightedResultFar
	.thumb_func
BattleFx_SetWeightedResultFar:
	overlay_veneer BattleFx_SetWeightedResult
	.global Func_080c8288
	.thumb_func
Func_080c8288:
	overlay_veneer Func_080d2b4c
	.global Party_SetFields1eeAnd1f0Far
	.thumb_func
Party_SetFields1eeAnd1f0Far:
	overlay_veneer Party_SetFields1eeAnd1f0
	.global Party_SetFields1f2And1f4Far
	.thumb_func
Party_SetFields1f2And1f4Far:
	overlay_veneer Party_SetFields1f2And1f4
	.global Field_SetDestinationFar
	.thumb_func
Field_SetDestinationFar:
	overlay_veneer Field_SetDestination
