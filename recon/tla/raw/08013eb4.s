.syntax unified
	.thumb
	.global Func_08013eb4
	.thumb_func
Func_08013eb4:
	ldr r3, .L_08013edc
	movs r4, #0
	strb r4, [r3]
	ldr r2, .L_08013ee0
	ldr r3, .L_08013ed8
	ldr r1, .L_08013ee4
	strh r3, [r2]
	ldr r2, .L_08013ee8
	ldrb r3, [r2]
	strb r3, [r1]
	ldr r3, .L_08013eec
	strb r4, [r2]
	strb r0, [r3]
	ldr r2, .L_08013ef0
	ldrb r3, [r3]
	strb r3, [r2]
	b .L_08013ef4
	.2byte 0x0000
.L_08013ed8:
	.4byte 0x0000003e
.L_08013edc:
	.4byte gBlendBrighten
.L_08013ee0:
	.4byte gBlendLayers
.L_08013ee4:
	.4byte gBlendStartLevel
.L_08013ee8:
	.4byte gBlendTargetLevel
.L_08013eec:
	.4byte gBlendDuration
.L_08013ef0:
	.4byte gBlendFramesLeft
.L_08013ef4:
	bx lr
	.2byte 0x0000
