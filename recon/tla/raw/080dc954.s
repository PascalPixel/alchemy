.syntax unified
	.thumb
	.global Func_080dc954
	.thumb_func
Func_080dc954:
	push {lr}
	movs r1, #192
	lsls r1, r1, #18
	ldr r3, [r1, #108]
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsb r2, [r3, r2]
	cmp r2, #3
	bne .L_080dc974
	ldr r3, [r1, #32]
	movs r1, #151
	lsls r1, r1, #4
	adds r3, r3, r1
	strb r2, [r3]
.L_080dc974:
	pop {pc}
	.2byte 0x0000
