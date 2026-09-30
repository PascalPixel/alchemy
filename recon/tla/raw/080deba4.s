.syntax unified
	.thumb
	.global Func_080deba4
	.thumb_func
Func_080deba4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldr r3, [r0, #8]
	ldr r1, [r0, #16]
	adds r0, #34
	ldrb r2, [r0]
	adds r0, r3, #0
	bl Func_080dbca8
	cmp r0, #0
	bne .L_080debd4
	movs r3, #179
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #128
	lsls r3, r3, #6
	adds r3, #146
	strh r3, [r2]
.L_080debd4:
	pop {r5, pc}
	.2byte 0x0000
