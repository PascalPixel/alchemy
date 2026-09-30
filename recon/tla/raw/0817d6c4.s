.syntax unified
	.thumb
	.global Func_0817d6c4
	.thumb_func
Func_0817d6c4:
	push {r5, r6, r7, lr}
	ldr r1, .L_0817d6f0
	movs r7, #0
	movs r6, #0
.L_0817d6cc:
	cmp r7, #14
	ble .L_0817d6f4
	ldr r4, .L_0817d6ec
	movs r2, #0
	adds r0, r6, #0
.L_0817d6d6:
	adds r3, r2, #0
	ands r3, r4
	adds r3, r0, r3
	adds r3, #48
	adds r2, #1
	strh r3, [r1]
	adds r1, #2
	cmp r2, #32
	bne .L_0817d6d6
	b .L_0817d718
	.2byte 0x0000
.L_0817d6ec:
	.4byte 0x0000000f
.L_0817d6f0:
	.4byte 0x0600f800
.L_0817d6f4:
	ldr r5, .L_0817d710
	ldr r4, .L_0817d714
	movs r2, #0
	adds r0, r6, #0
.L_0817d6fc:
	adds r3, r2, #0
	ands r3, r5
	adds r3, r0, r3
	adds r3, r3, r4
	adds r2, #1
	strh r3, [r1]
	adds r1, #2
	cmp r2, #32
	bne .L_0817d6fc
	b .L_0817d718
.L_0817d710:
	.4byte 0x0000000f
.L_0817d714:
	.4byte 0x00000210
.L_0817d718:
	adds r7, #1
	adds r6, #32
	cmp r7, #30
	bne .L_0817d6cc
	pop {r5, r6, r7, pc}
	.2byte 0x0000
