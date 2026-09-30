.syntax unified
	.thumb
	.global Func_080d4580
	.thumb_func
Func_080d4580:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	movs r1, #213
	adds r6, r0, #0
	lsls r1, r1, #4
	movs r0, #108
	bl Runtime_AllocateBlock
	movs r3, #230
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r5, [r0]
	cmp r6, #0
	beq .L_080d45b6
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_InitFromTable4WithArgumentFar
	str r6, [r5, #104]
	cmp r7, #0
	bne .L_080d45b6
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	ldr r3, [r6, #12]
	str r3, [r5, #12]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
.L_080d45b6:
	pop {r5, r6, r7, pc}
