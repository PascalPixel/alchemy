.syntax unified
	.thumb
	.global Trade_RemoveOffer
	.thumb_func
Trade_RemoveOffer:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	movs r1, #0
	mov r10, r2
	mov r9, r1
	movs r3, #0
	cmp r0, #7
	bls .L_080b0d72
	movs r3, #1
.L_080b0d72:
	adds r0, r3, #0
	bl Trade_GetOfferState
	movs r1, #148
	adds r3, r0, #0
	lsls r1, r1, #1
	movs r2, #8
	adds r2, r2, r3
	adds r7, r3, r1
	mov r12, r2
	ldr r2, [r7]
	movs r4, #0
	adds r0, #9
	movs r5, #0
	mov r1, r12
	cmp r9, r2
	bge .L_080b0df8
	ldrb r3, [r1]
	mov r6, r9
	mov lr, r3
	cmp r8, lr
	bne .L_080b0dae
	ldrb r3, [r0]
	cmp r10, r3
	bne .L_080b0dae
	subs r3, r2, #1
	movs r1, #1
	str r3, [r7]
	mov r9, r1
	b .L_080b0dd2
.L_080b0dae:
	ldr r2, [r7]
	adds r4, #1
	adds r0, #4
	adds r1, #4
	adds r5, #4
	cmp r4, r2
	bge .L_080b0df8
	ldrb r3, [r1]
	adds r6, r5, #0
	cmp r8, r3
	bne .L_080b0dae
	ldrb r3, [r0]
	cmp r10, r3
	bne .L_080b0dae
	subs r3, r2, #1
	str r3, [r7]
	movs r2, #1
	mov r9, r2
.L_080b0dd2:
	movs r3, #144
	lsls r3, r3, #1
	add r3, r12
	ldr r3, [r3]
	cmp r4, r3
	bge .L_080b0df8
	movs r2, #144
	lsls r2, r2, #1
	add r2, r12
	b .L_080b0de8
.L_080b0de6:
	lsls r6, r4, #2
.L_080b0de8:
	mov r1, r12
	adds r3, r6, #4
	ldr r3, [r1, r3]
	adds r4, #1
	str r3, [r1, r6]
	ldr r3, [r2]
	cmp r4, r3
	blt .L_080b0de6
.L_080b0df8:
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
