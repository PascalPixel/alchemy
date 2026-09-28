	.global Engine_ScheduleCallback
	.thumb_func
Engine_ScheduleCallback:
	overlay_veneer 0x080000d1
	.global Runtime_SetIrqHandler
	.thumb_func
Runtime_SetIrqHandler:
	overlay_veneer 0x08000131
	.global SerialRuntime_PollAndTransfer
	.thumb_func
SerialRuntime_PollAndTransfer:
	overlay_veneer 0x08000309
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer 0x080091a9
	.global Battle_Reset
	.thumb_func
Battle_Reset:
	overlay_veneer 0x0808a019
	.global BattleFx_FinishAction
	.thumb_func
BattleFx_FinishAction:
	overlay_veneer 0x0808a021
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer 0x0808a081
	.global Event_SetValue170
	.thumb_func
Event_SetValue170:
	overlay_veneer 0x0808a249
	.global BattleFx_SetPhaseRequest
	.thumb_func
BattleFx_SetPhaseRequest:
	overlay_veneer 0x0808a259
	.global DisplayTransition_InitializeBattleEffectState
	.thumb_func
DisplayTransition_InitializeBattleEffectState:
	overlay_veneer 0x0808a301
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	overlay_veneer 0x080f9011
