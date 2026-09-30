.syntax unified
	.thumb
	.global Func_08025608
	.thumb_func
Func_08025608:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	ldr r2, [r6, #104]
	ldr r3, [r2, #48]
	str r3, [r6, #48]
	ldr r3, [r2, #52]
	str r3, [r6, #52]
	ldr r3, [r6, #8]
	ldr r1, [r2, #8]
	ldr r2, [r2, #16]
	subs r1, r1, r3
	ldr r3, [r6, #16]
	mov r8, r1
	subs r2, r2, r3
	mov r10, r2
	asrs r3, r1, #16
	asrs r2, r2, #16
	adds r0, r3, #0
	muls r0, r3
	adds r3, r2, #0
	muls r3, r2
	adds r0, r0, r3
	ldr r3, .L_08025694
	mov lr, r3
	.2byte 0xf800
	adds r7, r0, #0
	cmp r7, #16
	ble .L_08025682
	adds r5, r7, #0
	subs r5, #16
	mov r0, r8
	muls r0, r5
	adds r1, r7, #0
	bl Math_Div
	adds r1, r7, #0
	mov r8, r0
	mov r0, r10
	muls r0, r5
	bl Math_Div
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	add r1, r8
	adds r3, r3, r0
	ldr r2, [r6, #12]
	adds r0, r6, #0
	bl Object_SetMoveTarget
	adds r0, r6, #0
	movs r1, #2
	bl ObjectDispatch_ApplyArgumentToChildren
	ldrh r3, [r6, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r6, #4]
	b .L_0802568c
.L_08025682:
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_ApplyArgumentToChildren
	movs r0, #0
.L_0802568c:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08025694:
	.4byte IwramFillWords + 0x74
