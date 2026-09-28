	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer 0x080770d1
	.global BattleFx_RunPageEffectForSlot
	.thumb_func
BattleFx_RunPageEffectForSlot:
	overlay_veneer 0x0808a4f9
