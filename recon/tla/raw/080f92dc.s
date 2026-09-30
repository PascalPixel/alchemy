.syntax unified
	.thumb
	.global Func_080f92dc
	.thumb_func
Func_080f92dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #4
	mov r8, r3
	movs r2, #13
	adds r3, #76
	movs r6, #31
.L_080f92fa:
	ldmia r3!, {r5}
	cmp r5, #0
	beq .L_080f9302
	strb r2, [r5, #5]
.L_080f9302:
	subs r6, #1
	cmp r6, #0
	bge .L_080f92fa
	adds r6, r1, #0
	adds r0, r0, r6
	cmp r6, r0
	bge .L_080f9368
	lsls r2, r6, #2
	adds r3, r2, #0
	adds r3, #76
	mov r1, r8
	ldr r5, [r1, r3]
	cmp r5, #0
	beq .L_080f9368
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldrb r3, [r3]
	subs r3, #1
	cmp r6, r3
	bgt .L_080f9368
	adds r3, r2, r1
	ldr r7, [sp, #32]
	adds r2, r3, #0
	mov r10, r0
	adds r2, #76
.L_080f9336:
	mov r3, r9
	strh r3, [r5, #6]
	strh r7, [r5, #8]
	adds r0, r5, #0
	str r2, [sp, #0]
	bl UiIcon_PrepareObject
	adds r6, #1
	movs r3, #1
	strb r3, [r5, #5]
	adds r7, #16
	ldr r2, [sp, #0]
	cmp r6, r10
	bge .L_080f9368
	adds r2, #4
	ldr r5, [r2]
	cmp r5, #0
	beq .L_080f9368
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldrb r3, [r3]
	subs r3, #1
	cmp r6, r3
	ble .L_080f9336
.L_080f9368:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
