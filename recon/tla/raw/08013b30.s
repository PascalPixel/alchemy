.syntax unified
	.thumb
	.global Sound_LoadPresetParameters
	.thumb_func
Sound_LoadPresetParameters:
	push {lr}
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.4byte 0x00004770
	.4byte 0x00004770
