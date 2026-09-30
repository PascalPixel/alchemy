.syntax unified
	.thumb
	.global Func_0810b9c0
	.thumb_func
Func_0810b9c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	str r0, [sp, #28]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r5, #2
	mov r9, r3
	bl Owner_GetState
	movs r1, #1
	str r0, [sp, #16]
	movs r2, #16
	movs r3, #4
	mov r11, r1
	mov r10, r1
	movs r0, #14
	movs r1, #8
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r2, #30
	str r0, [sp, #20]
	movs r3, #3
	movs r0, #0
	movs r1, #5
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r3, #128
	str r0, [sp, #24]
	lsls r3, r3, #3
	adds r3, #220
	add r3, r9
	ldr r2, [r3]
	movs r3, #18
	strb r3, [r2, #5]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	add r2, r9
	movs r3, #12
	strb r3, [r2]
	movs r7, #0
.L_0810ba26:
	mov r2, r10
	cmp r2, #0
	beq .L_0810baa0
	movs r3, #0
	ldr r0, [sp, #28]
	mov r10, r3
	bl Inventory_CountFar
	mov r11, r0
	mov r3, r11
	subs r3, #1
	cmp r7, r3
	ble .L_0810ba42
	adds r7, r3, #0
.L_0810ba42:
	ldr r1, [sp, #16]
	lsls r3, r7, #1
	adds r3, #216
	ldrh r3, [r1, r3]
	ldr r6, .L_0810ba88
	mov r2, r9
	ands r6, r3
	ldr r3, [r2, #36]
	movs r1, #5
	adds r0, r7, #0
	str r3, [sp, #8]
	bl __modsi3
	movs r1, #5
	adds r5, r0, #0
	adds r0, r7, #0
	bl Math_Div
	ldr r3, [sp, #8]
	adds r2, r0, #0
	lsls r5, r5, #4
	lsls r2, r2, #4
	adds r0, r3, #0
	adds r2, #8
	adds r1, r5, #0
	bl Func_08108af0
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #5
	movs r3, #3
	add r2, r9
	strb r3, [r2]
	ldr r0, [sp, #20]
	b .L_0810ba8c
.L_0810ba88:
	.4byte 0x000001ff
.L_0810ba8c:
	ldr r1, [sp, #28]
	adds r2, r7, #0
	bl Func_0810bca0
	ldr r3, .L_0810bc90
	ldr r0, [sp, #24]
	adds r6, r6, r3
	adds r1, r6, #0
	bl Func_08109270
.L_0810baa0:
	movs r0, #1
	bl WaitFrames
	ldr r4, .L_0810bc94
	movs r3, #1
	ldr r2, [r4, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_0810bb30
	movs r0, #1
	bl WaitFrames
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #11
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0810bad4
	ldr r0, [sp, #28]
	adds r1, r7, #0
	bl Func_080ad268
	b .L_0810badc
.L_0810bad4:
	ldr r0, [sp, #28]
	adds r1, r7, #0
	bl Func_080ad270
.L_0810badc:
	cmp r0, #0
	bne .L_0810baea
	movs r0, #112
	bl Audio_PlayCue
	adds r5, r7, #0
	b .L_0810bc6a
.L_0810baea:
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0810bb00
	ldr r0, .L_0810bc98
	movs r1, #8
	movs r2, #1
	movs r3, #0
	bl UiText_OpenMessageWindowFar
	b .L_0810bb14
.L_0810bb00:
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	beq .L_0810bb14
	ldr r0, .L_0810bc9c
	movs r1, #8
	movs r2, #1
	movs r3, #0
	bl UiText_OpenMessageWindowFar
.L_0810bb14:
	movs r0, #113
	bl Audio_PlayCue
	b .L_0810bb22
.L_0810bb1c:
	movs r0, #1
	bl WaitFrames
.L_0810bb22:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_0810bb1c
	bl UiWork_FinalizePendingCoreFar
	b .L_0810ba26
.L_0810bb30:
	ldr r6, [r4, #4]
	movs r2, #2
	ands r6, r2
	cmp r6, #0
	beq .L_0810bb46
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_0810bc6a
.L_0810bb46:
	ldr r3, [r4]
	movs r1, #4
	ands r3, r1
	mov r8, r1
	cmp r3, #0
	beq .L_0810bbce
	ldr r1, [sp, #16]
	lsls r3, r7, #1
	adds r3, #216
	movs r0, #126
	ldrh r5, [r1, r3]
	str r2, [sp, #12]
	str r4, [sp, #4]
	bl Audio_PlayCue
	ldr r2, [sp, #12]
	movs r3, #10
	str r2, [sp, #0]
	movs r1, #9
	movs r2, #16
	movs r0, #0
	bl UiWindow_CreateFar
	adds r1, r5, #0
	mov r10, r0
	bl Func_080f8038
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #9
	add r3, r9
	strb r6, [r3]
	ldr r4, [sp, #4]
	mov r2, r8
	ldr r3, [r4]
	ands r3, r2
	cmp r3, #0
	beq .L_0810bba4
	adds r6, r4, #0
	movs r5, #4
.L_0810bb96:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6]
	ands r3, r5
	cmp r3, #0
	bne .L_0810bb96
.L_0810bba4:
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #9
	add r2, r9
	movs r3, #1
	strb r3, [r2]
	mov r0, r10
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #232
	add r3, r9
	ldr r0, [r3]
	bl RenderOutput_PrepareForRedrawFar
	movs r0, #1
	bl WaitFrames
	b .L_0810bc64
.L_0810bbce:
	ldr r3, [r4, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0810bbf2
	movs r0, #111
	str r4, [sp, #4]
	subs r7, #1
	bl Audio_PlayCue
	mov r1, r11
	adds r0, r7, r1
	bl __modsi3
	ldr r4, [sp, #4]
	movs r2, #1
	adds r7, r0, #0
	mov r10, r2
.L_0810bbf2:
	ldr r3, [r4, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0810bc18
	movs r0, #111
	str r4, [sp, #4]
	adds r7, #1
	bl Audio_PlayCue
	mov r3, r11
	adds r0, r7, r3
	mov r1, r11
	bl __modsi3
	ldr r4, [sp, #4]
	movs r1, #1
	adds r7, r0, #0
	mov r10, r1
.L_0810bc18:
	ldr r3, [r4, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_0810bc3e
	subs r7, #5
	cmp r7, #0
	bge .L_0810bc2a
	adds r7, #15
.L_0810bc2a:
	cmp r7, r11
	blt .L_0810bc34
.L_0810bc2e:
	subs r7, #5
	cmp r7, r11
	bge .L_0810bc2e
.L_0810bc34:
	movs r0, #111
	bl Audio_PlayCue
	movs r2, #1
	mov r10, r2
.L_0810bc3e:
	ldr r3, .L_0810bc94
	movs r2, #128
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_0810bc4c
	b .L_0810ba26
.L_0810bc4c:
	adds r7, #5
	cmp r7, r11
	blt .L_0810bc54
	subs r7, #15
.L_0810bc54:
	cmp r7, #0
	bge .L_0810bc5e
.L_0810bc58:
	adds r7, #5
	cmp r7, #0
	blt .L_0810bc58
.L_0810bc5e:
	movs r0, #111
	bl Audio_PlayCue
.L_0810bc64:
	movs r3, #1
	mov r10, r3
	b .L_0810ba26
.L_0810bc6a:
	ldr r0, [sp, #24]
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r1, #2
	ldr r0, [sp, #20]
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0810bc90:
	.4byte 0x00000092
.L_0810bc94:
	.4byte gInput
.L_0810bc98:
	.4byte 0x00001244
.L_0810bc9c:
	.4byte 0x00001243
