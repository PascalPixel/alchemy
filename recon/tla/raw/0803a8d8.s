.syntax unified
	.thumb
	.global Func_0803a8d8
	.thumb_func
Func_0803a8d8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0803a8f4
	bl UiWork_ProcessDirectWork
	bl UiWork_ProcessRenderChannels
	bl UiWork_AnimateSpriteSlots
	b .L_0803a8f8
.L_0803a8f4:
	bl UiWork_ProcessRenderChannels
.L_0803a8f8:
	pop {pc}
	.2byte 0x0000
