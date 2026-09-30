.syntax unified
	.thumb
	.global Func_08109ad8
	.thumb_func
Func_08109ad8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	sub sp, #8
	ldr r6, [r3]
	movs r0, #0
	movs r5, #2
	movs r1, #9
	movs r2, #12
	movs r3, #4
	str r0, [sp, #4]
	str r5, [sp, #0]
	mov r8, r0
	bl UiWindow_CreateFar
	str r0, [r6, #12]
	bl Func_08109188
	movs r1, #12
	movs r2, #14
	movs r3, #8
	movs r0, #16
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r1, #14
	str r0, [r6, #36]
	movs r2, #13
	movs r3, #3
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r1, #192
	lsls r1, r1, #4
	movs r2, #128
	adds r1, #232
	lsls r2, r2, #3
	adds r3, r6, r1
	adds r2, #220
	str r0, [r3]
	adds r3, r6, r2
	ldr r2, [r3]
	movs r3, #4
	strb r3, [r2, #5]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #5
	adds r2, r6, r3
	movs r3, #12
	strb r3, [r2]
	movs r1, #2
	movs r2, #0
	mov r10, r0
	bl Func_080f8060
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #5
	adds r0, r0, r6
	movs r7, #1
	mov r9, r0
.L_08109b5e:
	cmp r7, #0
	beq .L_08109bc4
	ldr r4, [sp, #4]
	movs r1, #153
	lsls r1, r1, #3
	lsls r2, r4, #1
	adds r3, r6, #2
	adds r2, r2, r1
	ldrsh r0, [r3, r2]
	adds r3, r4, #0
	mov r8, r0
	cmp r4, #0
	bge .L_08109b7a
	adds r3, r4, #3
.L_08109b7a:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r4, r3
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	movs r2, #0
	subs r1, #12
	mov r0, r10
	bl Func_08108af0
	movs r3, #3
	mov r2, r9
	strb r3, [r2]
	cmp r7, #2
	bne .L_08109bb0
	ldr r0, [sp, #4]
	cmp r0, #0
	bge .L_08109ba2
	adds r0, #3
.L_08109ba2:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_080f8058
	movs r0, #1
	bl WaitFrames
.L_08109bb0:
	ldr r1, [sp, #4]
	mov r0, r10
	movs r2, #0
	bl Func_0810928c
	ldr r0, [r6, #36]
	mov r1, r8
	bl Func_0810a004
	movs r7, #0
.L_08109bc4:
	ldr r1, .L_08109ca8
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_08109c3e
	movs r0, #1
	bl WaitFrames
	mov r0, r8
	bl Inventory_CountFar
	cmp r0, #0
	bne .L_08109be8
	movs r0, #113
	bl Audio_PlayCue
	b .L_08109b5e
.L_08109be8:
	mov r0, r10
	bl RenderOutput_PrepareForRedrawFar
	movs r0, #112
	bl Audio_PlayCue
	movs r0, #129
	lsls r0, r0, #3
	adds r0, #255
	adds r3, r6, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_08109c0e
	mov r0, r8
	bl Func_08109cac
	b .L_08109c26
.L_08109c0e:
	cmp r3, #3
	bne .L_08109c1a
	mov r0, r8
	bl Func_0810a2d0
	b .L_08109c26
.L_08109c1a:
	mov r0, r8
	bl Func_0810b6c4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08109c72
.L_08109c26:
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #220
	adds r3, r6, r1
	ldr r2, [r3]
	movs r3, #4
	strb r3, [r2, #5]
	movs r3, #12
	mov r2, r9
	strb r3, [r2]
	movs r7, #2
	b .L_08109b5e
.L_08109c3e:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08109c54
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_08109c72
.L_08109c54:
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #4
	adds r3, r6, r0
	movs r1, #0
	ldrsb r1, [r3, r1]
	add r0, sp, #4
	movs r2, #4
	bl Func_08108690
	adds r7, r0, #0
	movs r0, #1
	bl WaitFrames
	b .L_08109b5e
.L_08109c72:
	movs r0, #0
	bl Func_0810bea8
	bl Func_080f8068
	mov r0, r10
	movs r1, #2
	bl UiWork_FinalizeFar
	ldr r0, [r6, #36]
	movs r1, #2
	bl UiWork_FinalizeFar
	ldr r0, [r6, #12]
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08109ca8:
	.4byte gInput
