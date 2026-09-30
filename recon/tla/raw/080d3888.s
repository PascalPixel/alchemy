.syntax unified
	.thumb
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	push {r5, r6, lr}
	adds r5, r1, #0
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080d38d0
	adds r3, r6, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #1
	bne .L_080d38d0
	ldr r1, [r6, #80]
	movs r2, #13
	ldrb r0, [r1, #9]
	movs r3, #3
	negs r2, r2
	ands r5, r3
	adds r3, r2, #0
	lsls r4, r5, #2
	ands r3, r0
	orrs r3, r4
	strb r3, [r1, #9]
	adds r1, #37
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r4
	strb r2, [r1]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_080d38d0:
	pop {r5, r6, pc}
	.2byte 0x0000
