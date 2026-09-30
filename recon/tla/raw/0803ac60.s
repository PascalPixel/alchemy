.syntax unified
	.thumb
	.global Func_0803ac60
	.thumb_func
Func_0803ac60:
	push {r5, r6, r7, lr}
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r1, #0
	adds r7, r2, #0
	ldr r4, [r3, #60]
	cmp r0, #0
	bne .L_0803ac94
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #66
	adds r1, r4, r3
	ldrh r3, [r1]
	movs r2, #244
	lsls r2, r2, #4
	lsls r3, r3, #1
	adds r3, r3, r2
	adds r0, r4, r2
	ldr r2, .L_0803acbc
	strh r2, [r4, r3]
	ldr r2, .L_0803acc0
	ldrh r3, [r1]
	adds r3, #1
	ands r3, r2
	strh r3, [r1]
.L_0803ac94:
	movs r1, #14
	ldrsh r3, [r5, r1]
	movs r1, #12
	ldrsh r2, [r5, r1]
	adds r3, r3, r6
	adds r3, #1
	adds r2, r2, r7
	lsls r3, r3, #5
	adds r3, r3, r2
	movs r2, #160
	adds r1, r3, #1
	lsls r2, r2, #2
	cmp r1, r2
	bcs .L_0803acd0
	ldr r3, .L_0803acc4
	lsls r1, r1, #1
	adds r2, r1, r3
	adds r1, r4, r1
	movs r3, #7
	b .L_0803acc8
.L_0803acbc:
	.4byte 0x00000000
.L_0803acc0:
	.4byte 0x000001ff
.L_0803acc4:
	.4byte 0x06002000
.L_0803acc8:
	adds r1, #8
	ands r3, r7
	bl Func_080416cc
.L_0803acd0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
