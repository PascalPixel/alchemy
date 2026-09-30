.syntax unified
	.thumb
	.global Func_08127038
	.thumb_func
Func_08127038:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	ldr r3, [r3, #92]
	cmp r0, #0
	beq .L_08127060
	movs r1, #158
	lsls r1, r1, #5
	adds r2, r3, r1
	ldr r3, [r2]
	cmp r3, #0
	beq .L_08127060
	movs r3, #0
	str r3, [r2]
	movs r2, #128
	ldr r1, .L_08127064
	lsls r2, r2, #7
	bl ColorBuffer_ScaleThreeQuarters
.L_08127060:
	pop {pc}
	.2byte 0x0000
.L_08127064:
	.4byte 0x06004000
