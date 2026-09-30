.syntax unified
	.thumb
	.global Djinn_AddToOwner
	.thumb_func
Djinn_AddToOwner:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r7, r2, #0
	bl Owner_GetState
	movs r3, #140
	lsls r3, r3, #1
	adds r6, r5, r3
	ldrb r4, [r0, r6]
	adds r3, r4, #0
	cmp r3, #9
	bhi .L_080b0ba2
	lsls r3, r5, #2
	adds r1, r3, #0
	adds r1, #248
	ldr r3, [r0, r1]
	movs r2, #1
	lsls r2, r7
	ands r3, r2
	cmp r3, #0
	beq .L_080b0ba8
.L_080b0ba2:
	movs r0, #1
	negs r0, r0
	b .L_080b0bb4
.L_080b0ba8:
	adds r3, r4, #1
	strb r3, [r0, r6]
	ldr r3, [r0, r1]
	orrs r3, r2
	str r3, [r0, r1]
	movs r0, #0
.L_080b0bb4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
