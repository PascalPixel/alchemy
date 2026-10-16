.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	.global Field_RequestBattleFromEncounterTableFar
	.thumb_func
Field_RequestBattleFromEncounterTableFar:
	overlay_veneer Field_RequestBattleFromEncounterTable
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
