.syntax unified
	.thumb
	.global Func_081096f8
	.thumb_func
Func_081096f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r1
	movs r1, #128
	sub sp, #16
	mov r11, r0
	lsls r1, r1, #3
	movs r0, #56
	str r2, [sp, #12]
	bl Runtime_AllocateBlock
	movs r2, #1
	str r2, [sp, #8]
	mov r3, r9
	mov r2, r11
	subs r3, r3, r2
	mov r9, r3
	movs r3, #2
	str r3, [sp, #0]
	mov r8, r0
	movs r1, #4
	movs r0, #7
	movs r2, #23
	movs r3, #3
	bl UiWindow_CreateFar
	movs r5, #1
	negs r5, r5
	movs r7, #0
	mov r10, r0
	cmp r0, #0
	bne .L_08109744
	b .L_0810989e
.L_08109744:
	bl Resource_FindFreeEntry
	str r0, [sp, #4]
	cmp r0, #96
	bne .L_08109750
	b .L_0810989e
.L_08109750:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	bl VramBlock_LoadCached
	ldr r5, .L_08109794
	ldr r0, [sp, #4]
	adds r1, r5, #0
	mov r2, r10
	movs r3, #0
	str r7, [sp, #0]
	bl RenderOutput_CreateFar
	adds r1, r5, #0
	mov r2, r10
	movs r3, #32
	ldr r0, [sp, #4]
	str r7, [sp, #0]
	bl RenderOutput_CreateFar
	ldrh r1, [r0, #24]
	ldr r3, .L_08109790
	lsls r2, r1, #22
	lsrs r2, r2, #22
	adds r2, #4
	ands r2, r3
	ldr r3, .L_08109798
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #24]
	b .L_08109864
	.2byte 0x0000
.L_08109790:
	.4byte 0x000003ff
.L_08109794:
	.4byte 0x40004000
.L_08109798:
	.4byte 0xfffffc00
.L_0810979c:
	ldr r2, .L_081098b4
	ldr r3, [r2, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_081097b4
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	str r3, [sp, #8]
	subs r7, #1
.L_081097b4:
	ldr r2, .L_081098b4
	ldr r3, [r2, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_081097cc
	movs r0, #111
	bl Audio_PlayCue
	movs r3, #1
	str r3, [sp, #8]
	adds r7, #1
.L_081097cc:
	ldr r2, [sp, #8]
	cmp r2, #0
	beq .L_0810985e
	mov r2, r9
	movs r3, #0
	adds r0, r7, r2
	mov r1, r9
	str r3, [sp, #8]
	bl Math_Mod
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r7, r0, #0
	adds r3, #212
	ldr r0, .L_081098b8
	mov r1, r8
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r8
	movs r0, #30
	movs r1, #14
	bl Shop_FillSelector
	mov r0, r11
	add r0, r9
	movs r1, #0
	mov r2, r8
	bl Shop_FillSelector
	mov r3, r11
	adds r0, r3, r7
	adds r0, #1
	movs r1, #10
	mov r2, r8
	bl Shop_FillSelector
	mov r0, r11
	movs r1, #2
	mov r2, r8
	bl Shop_FillSelector
	movs r1, #128
	ldr r0, [sp, #4]
	lsls r1, r1, #1
	mov r2, r8
	bl VramBlock_LoadCached
	adds r5, r7, #1
	adds r0, r5, #0
	movs r1, #2
	mov r2, r10
	movs r3, #72
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r2, [sp, #12]
	movs r1, #6
	adds r0, r5, #0
	muls r0, r2
	movs r3, #88
	mov r2, r10
	str r6, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r0, .L_081098bc
	mov r1, r10
	movs r2, #136
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_0810985e:
	movs r0, #1
	bl WaitFrames
.L_08109864:
	ldr r2, .L_081098b4
	ldr r3, [r2, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0810987a
	movs r0, #112
	bl Audio_PlayCue
	adds r5, r7, #1
	b .L_08109890
.L_0810987a:
	ldr r3, .L_081098b4
	ldr r6, [r3, #4]
	movs r3, #2
	ands r6, r3
	cmp r6, #0
	beq .L_0810979c
	movs r0, #113
	bl Audio_PlayCue
	movs r5, #1
	negs r5, r5
.L_08109890:
	movs r0, #1
	bl WaitFrames
	mov r0, r10
	movs r1, #2
	bl UiWork_FinalizeFar
.L_0810989e:
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	adds r0, r5, #0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081098b4:
	.4byte gInput
.L_081098b8:
	.4byte Data_0810c248
.L_081098bc:
	.4byte 0x00001235
