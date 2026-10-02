.syntax unified
	.thumb
	.global Func_080d3780
	.thumb_func
Func_080d3780:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl ObjectTable_Get
	adds r5, r0, #0
	movs r0, #255
	ands r0, r6
	bl ObjectTable_Get
	cmp r5, #0
	beq .L_080d37d0
	cmp r0, #0
	beq .L_080d37d0
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r6
	str r0, [r5, #104]
	cmp r3, #0
	bne .L_080d37c2
	adds r2, r5, #0
	movs r3, #40
	adds r2, #100
	strh r3, [r2]
	ldr r1, .L_080d37cc
	ldr r3, [r0, #52]
	lsls r3, r3, #1
	str r3, [r5, #52]
	ldr r3, [r0, #48]
	str r3, [r5, #48]
	adds r3, r5, #0
	adds r3, #89
	strb r1, [r3]
.L_080d37c2:
	adds r0, r5, #0
	adds r1, r7, #0
	bl ObjectDispatch_InitializeFar
	b .L_080d37d0
.L_080d37cc:
	.4byte 0x00000000
.L_080d37d0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
