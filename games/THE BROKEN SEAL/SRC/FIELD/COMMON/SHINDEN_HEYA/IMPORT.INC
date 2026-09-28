	.global IwramUnsignedRemainder
	.thumb_func
IwramUnsignedRemainder:
	overlay_veneer 0x030003e0
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer 0x080000c1
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000101
	overlay_veneer 0x08000119
	overlay_veneer 0x08000121
	.global Runtime_AllocateHeapBlock
	.thumb_func
Runtime_AllocateHeapBlock:
	overlay_veneer 0x08000141
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer 0x08000151
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer 0x080001c9
	.global ObjectDispatch_Initialize
	.thumb_func
ObjectDispatch_Initialize:
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer 0x08009099
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer 0x080090c9
	overlay_veneer 0x08009181
	overlay_veneer 0x080091c1
	overlay_veneer 0x08015011
	overlay_veneer 0x08015019
	overlay_veneer 0x08015079
	.global UiText_ShowCenteredMessage
	.thumb_func
UiText_ShowCenteredMessage:
	overlay_veneer 0x08015211
	overlay_veneer 0x08015229
	overlay_veneer 0x08015231
	overlay_veneer 0x08015239
	.global Ui_PrepareTransferForItem
	.thumb_func
Ui_PrepareTransferForItem:
	overlay_veneer 0x08015251
	overlay_veneer 0x08015401
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer 0x080770d1
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer 0x0808a011
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer 0x0808a019
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer 0x0808a021
	overlay_veneer 0x0808a031
	overlay_veneer 0x0808a071
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	overlay_veneer 0x0808a091
	overlay_veneer 0x0808a099
	overlay_veneer 0x0808a0a1
	overlay_veneer 0x0808a0a9
	overlay_veneer 0x0808a0c1
	overlay_veneer 0x0808a0c9
	overlay_veneer 0x0808a0d1
	overlay_veneer 0x0808a0e9
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer 0x0808a0f1
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer 0x0808a101
	overlay_veneer 0x0808a111
	overlay_veneer 0x0808a119
	overlay_veneer 0x0808a129
	overlay_veneer 0x0808a131
	overlay_veneer 0x0808a139
	overlay_veneer 0x0808a149
	overlay_veneer 0x0808a151
	overlay_veneer 0x0808a159
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a179
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1d1
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a1f1
	overlay_veneer 0x0808a209
	overlay_veneer 0x0808a211
	overlay_veneer 0x0808a219
	overlay_veneer 0x0808a221
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	.global Engine_ColorBufferApplyTarget
	.thumb_func
Engine_ColorBufferApplyTarget:
	overlay_veneer 0x0808a331
	.global Engine_ColorBufferInterpolate
	.thumb_func
Engine_ColorBufferInterpolate:
	overlay_veneer 0x0808a349
	overlay_veneer 0x0808a361
	overlay_veneer 0x0808a371
	overlay_veneer 0x080b0011
	overlay_veneer 0x080f9011
	overlay_veneer 0x080f9079
