.syntax unified
	.thumb
	.global Blend_SetDarkenTarget16
	.thumb_func
Blend_SetDarkenTarget16:
	ldr r2, .L_08013e98
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_08013e9c
	ldr r3, .L_08013e94
	ldr r1, .L_08013ea0
	strh r3, [r2]
	ldr r2, .L_08013ea4
	ldrb r3, [r2]
	strb r3, [r1]
	movs r3, #16
	strb r3, [r2]
	ldr r3, .L_08013ea8
	ldr r2, .L_08013eac
	strb r0, [r3]
	ldrb r3, [r3]
	strb r3, [r2]
	b .L_08013eb0
.L_08013e94:
	.4byte 0x0000003e
.L_08013e98:
	.4byte gBlendBrighten
.L_08013e9c:
	.4byte gBlendLayers
.L_08013ea0:
	.4byte gBlendStartLevel
.L_08013ea4:
	.4byte gBlendTargetLevel
.L_08013ea8:
	.4byte gBlendDuration
.L_08013eac:
	.4byte gBlendFramesLeft
.L_08013eb0:
	bx lr
	.2byte 0x0000
