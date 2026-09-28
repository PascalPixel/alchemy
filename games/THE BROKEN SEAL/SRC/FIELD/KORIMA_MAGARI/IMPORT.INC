	.global IwramSignedDivide
	.thumb_func
IwramSignedDivide:
	overlay_veneer 0x03000380
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000d9
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000129
	overlay_veneer 0x08000131
	overlay_veneer 0x08000149
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer 0x08009081
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer 0x080090c9
	overlay_veneer 0x080090e1
	overlay_veneer 0x08009129
	overlay_veneer 0x08009151
	overlay_veneer 0x08009159
	.global Map_GetTerrainHeightFar
	.thumb_func
Map_GetTerrainHeightFar:
	overlay_veneer 0x080091a9
	overlay_veneer 0x080091b9
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer 0x080091c1
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer 0x080091e1
	overlay_veneer 0x08015041
	overlay_veneer 0x080770c1
	overlay_veneer 0x0808a011
	overlay_veneer 0x0808a019
	overlay_veneer 0x0808a021
	overlay_veneer 0x0808a081
	overlay_veneer 0x0808a091
	overlay_veneer 0x0808a0b9
	overlay_veneer 0x0808a0e1
	overlay_veneer 0x0808a0e9
	overlay_veneer 0x0808a0f1
	overlay_veneer 0x0808a101
	overlay_veneer 0x0808a209
	overlay_veneer 0x0808a211
	overlay_veneer 0x0808a219
	.global Field_TryJumpForward
	.thumb_func
Field_TryJumpForward:
	overlay_veneer 0x0808a2a1
	overlay_veneer 0x0808a331
	overlay_veneer 0x0808a339
	overlay_veneer 0x080f9011
