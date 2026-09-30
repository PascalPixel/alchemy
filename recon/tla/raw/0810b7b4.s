.syntax unified
	.thumb
	.global Func_0810b7b4
	.thumb_func
Func_0810b7b4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #8]
	str r1, [sp, #4]
	movs r0, #0
	movs r1, #1
	str r0, [sp, #12]
	adds r5, r2, #0
	mov r8, r1
	mov r9, r0
	bl Func_08108148
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #11
	adds r3, r7, r2
	strb r5, [r3]
	movs r1, #12
	movs r5, #2
	movs r2, #14
	movs r3, #8
	movs r0, #16
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r1, #14
	str r0, [r7, #36]
	movs r2, #13
	movs r3, #3
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	mov r10, r0
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #232
	adds r3, r7, r0
	mov r1, r10
	str r1, [r3]
	movs r2, #1
	movs r1, #0
	movs r3, #1
	movs r0, #30
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #236
	adds r3, r7, r2
	mov r11, r0
	movs r1, #128
	movs r6, #0
	ldrh r0, [r3]
	lsls r1, r1, #23
	mov r2, r11
	movs r3, #0
	str r6, [sp, #0]
	bl RenderOutput_CreateFar
	movs r3, #4
	adds r5, r0, #0
	strb r3, [r5, #5]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	strb r6, [r5, #4]
	movs r1, #32
	adds r6, r7, r3
	negs r1, r1
	adds r0, r6, #0
	movs r2, #112
	bl Func_08108aa8
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #5
	adds r3, r7, r0
	mov r1, r8
	str r5, [r6]
	movs r2, #0
	strb r1, [r3]
	mov r0, r10
	movs r1, #2
	bl Func_080f8060
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	adds r5, r7, r2
.L_0810b87e:
	mov r3, r8
	cmp r3, #0
	beq .L_0810b8e8
	ldr r4, [sp, #12]
	movs r0, #153
	lsls r0, r0, #3
	lsls r2, r4, #1
	adds r3, r7, #2
	adds r2, r2, r0
	ldrsh r1, [r3, r2]
	adds r3, r4, #0
	mov r9, r1
	cmp r4, #0
	bge .L_0810b89c
	adds r3, r4, #3
.L_0810b89c:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r4, r3
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	subs r1, #12
	mov r0, r10
	movs r2, #0
	bl Func_08108af0
	movs r3, #3
	mov r1, r8
	strb r3, [r5]
	cmp r1, #2
	bne .L_0810b8d2
	ldr r0, [sp, #12]
	cmp r0, #0
	bge .L_0810b8c4
	adds r0, #3
.L_0810b8c4:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_080f8058
	movs r0, #1
	bl WaitFrames
.L_0810b8d2:
	ldr r1, [sp, #12]
	movs r2, #0
	mov r0, r10
	bl Func_0810928c
	ldr r0, [r7, #36]
	mov r1, r9
	bl Func_0810a004
	movs r2, #0
	mov r8, r2
.L_0810b8e8:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0810b9bc
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_0810b94c
	mov r0, r9
	bl Inventory_CountFar
	cmp r0, #0
	bne .L_0810b90c
	movs r0, #113
	bl Audio_PlayCue
	b .L_0810b87e
.L_0810b90c:
	mov r0, r10
	bl RenderOutput_PrepareForRedrawFar
	movs r0, #112
	bl Audio_PlayCue
	mov r0, r9
	bl Func_0810b9c0
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0810b93e
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #220
	adds r3, r7, r0
	ldr r2, [r3]
	movs r3, #4
	strb r3, [r2, #5]
	movs r1, #2
	movs r3, #12
	strb r3, [r5]
	mov r8, r1
	b .L_0810b87e
.L_0810b93e:
	ldr r3, [sp, #8]
	mov r2, r9
	str r2, [r3]
	ldr r1, [sp, #4]
	movs r6, #0
	str r0, [r1]
	b .L_0810b984
.L_0810b94c:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0810b96c
	movs r0, #113
	bl Audio_PlayCue
	ldr r2, [sp, #8]
	movs r3, #1
	negs r3, r3
	str r3, [r2]
	ldr r0, [sp, #4]
	adds r6, r3, #0
	str r3, [r0]
	b .L_0810b984
.L_0810b96c:
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #4
	adds r3, r7, r1
	movs r1, #0
	ldrsb r1, [r3, r1]
	add r0, sp, #12
	movs r2, #4
	bl Func_08108690
	mov r8, r0
	b .L_0810b87e
.L_0810b984:
	bl Menu_ReleaseEntryObjectsFar
	mov r0, r11
	movs r1, #2
	bl UiWork_FinalizeFar
	mov r0, r10
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r1, #2
	ldr r0, [r7, #36]
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	bl Func_0810824c
	adds r0, r6, #0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810b9bc:
	.4byte gInput
