.syntax unified
	.thumb
	.global Func_080d4ccc
	.thumb_func
Func_080d4ccc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	adds r4, r1, #0
	ldr r3, [r3, #108]
	cmp r2, #0
	beq .L_080d4cfe
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080d4cfe
	adds r3, r0, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r4]
	ldr r2, [r4, #8]
	bl Func_080202f0
	cmp r0, #2
	bhi .L_080d4d02
.L_080d4cfe:
	movs r0, #0
	b .L_080d4d04
.L_080d4d02:
	movs r0, #1
.L_080d4d04:
	pop {pc}
	.2byte 0x0000
