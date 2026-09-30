.syntax unified
	.thumb
	.global Func_080d7b04
	.thumb_func
Func_080d7b04:
	push {r5, r6, r7, lr}
	ldr r3, .L_080d7c00
	movs r1, #133
	adds r5, r0, #0
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r7, r5, #0
	ldr r0, [r3]
	adds r7, #64
	sub sp, #12
	bl Object_GetById
	movs r2, #0
	ldrsb r2, [r7, r2]
	cmp r2, #0
	bne .L_080d7b3c
	ldrh r3, [r5, #60]
	adds r3, #1
	strh r3, [r5, #60]
	ldrh r3, [r5, #62]
	adds r3, #1
	strh r3, [r5, #62]
	movs r1, #56
	ldrsh r3, [r5, r1]
	cmp r3, #60
	bne .L_080d7bd8
	strh r2, [r5, #56]
	b .L_080d7bc4
.L_080d7b3c:
	cmp r2, #1
	bne .L_080d7b50
	ldrh r3, [r5, #62]
	adds r3, #1
	strh r3, [r5, #62]
	movs r2, #56
	ldrsh r3, [r5, r2]
	cmp r3, #40
	bne .L_080d7bd8
	b .L_080d7bc0
.L_080d7b50:
	cmp r2, #2
	bne .L_080d7ba8
	ldrh r3, [r5, #62]
	mov r6, sp
	adds r3, #1
	strh r3, [r5, #62]
	movs r1, #160
	ldr r3, [r0, #8]
	lsls r1, r1, #13
	str r3, [r6]
	ldr r3, [r0, #12]
	adds r3, r3, r1
	str r3, [r6, #4]
	ldr r3, [r0, #16]
	adds r0, r6, #0
	str r3, [r6, #8]
	bl Camera_WorldToScreen
	ldr r3, [r6]
	ldr r2, [r5, #20]
	subs r3, r3, r2
	cmp r3, #0
	bge .L_080d7b80
	adds r3, #7
.L_080d7b80:
	asrs r3, r3, #3
	adds r3, r2, r3
	str r3, [r5, #20]
	ldr r2, [r5, #24]
	ldr r3, [r6, #8]
	subs r3, r3, r2
	cmp r3, #0
	bge .L_080d7b92
	adds r3, #7
.L_080d7b92:
	asrs r3, r3, #3
	adds r3, r2, r3
	str r3, [r5, #24]
	movs r2, #56
	ldrsh r3, [r5, r2]
	cmp r3, #40
	bne .L_080d7bda
	movs r3, #0
	strh r3, [r5, #56]
	ldrb r3, [r7]
	b .L_080d7bc8
.L_080d7ba8:
	cmp r2, #3
	bne .L_080d7bce
	ldrh r3, [r5, #60]
	subs r3, #1
	strh r3, [r5, #60]
	ldrh r3, [r5, #62]
	adds r3, #1
	strh r3, [r5, #62]
	movs r1, #56
	ldrsh r3, [r5, r1]
	cmp r3, #60
	bne .L_080d7bd8
.L_080d7bc0:
	movs r3, #0
	strh r3, [r5, #56]
.L_080d7bc4:
	ldrb r3, [r7]
	mov r6, sp
.L_080d7bc8:
	adds r3, #1
	strb r3, [r7]
	b .L_080d7bda
.L_080d7bce:
	cmp r2, #4
	bne .L_080d7bd8
	adds r0, r5, #0
	bl BattleFx_ClearOwnedSlot
.L_080d7bd8:
	mov r6, sp
.L_080d7bda:
	ldr r3, [r5, #20]
	str r3, [r6]
	ldr r3, [r5, #24]
	str r3, [r6, #8]
	movs r2, #60
	ldrsh r0, [r5, r2]
	movs r3, #62
	ldrsh r1, [r5, r3]
	lsls r0, r0, #16
	lsls r1, r1, #11
	adds r2, r6, #0
	bl Vector_AddPolarOffset
	ldr r3, [r6]
	add sp, #12
	str r3, [r5, #4]
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	pop {r5, r6, r7, pc}
.L_080d7c00:
	.4byte gPartyState
