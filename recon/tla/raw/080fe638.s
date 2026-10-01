.syntax unified
	.thumb
	.global Func_080fe638
	.thumb_func
Func_080fe638:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #36
	movs r0, #0
	str r0, [sp, #28]
	movs r0, #112
	str r3, [sp, #32]
	bl Audio_PlayCue
	ldr r1, [sp, #32]
	movs r5, #13
	adds r1, #240
	str r1, [sp, #24]
	movs r6, #1
	ldr r0, [r1]
	bl RenderOutput_RedrawSavedRectFar
	ldr r2, [sp, #24]
	ldr r0, .L_080fe6ec
	ldr r1, [r2]
	movs r3, #16
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, [sp, #24]
	movs r2, #96
	ldr r1, [r3]
	ldr r0, .L_080fe6f0
	negs r2, r2
	movs r3, #132
	bl UiText_DrawResourceFar
	ldr r0, [sp, #32]
	movs r1, #188
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3]
	movs r2, #190
	lsls r2, r2, #1
	strb r5, [r3, #5]
	adds r3, r0, r2
	ldr r3, [r3]
	movs r0, #0
	strb r5, [r3, #5]
	bl Func_081054cc
	ldr r0, [sp, #32]
	movs r1, #184
	ldr r3, [r0, #20]
	lsls r1, r1, #1
	strb r5, [r3, #5]
	adds r3, r0, r1
	ldr r3, [r3]
	movs r2, #148
	lsls r2, r2, #1
	strb r5, [r3, #5]
	adds r1, r0, r2
	ldr r5, .L_080fe6e4
	movs r2, #140
	ldr r4, .L_080fe6e8
	lsls r2, r2, #2
	adds r3, r0, r2
	movs r0, #32
	movs r2, #3
.L_080fe6ca:
	subs r2, #1
	strh r0, [r3]
	strh r5, [r3, #8]
	adds r0, #56
	strh r4, [r1]
	adds r3, #2
	adds r1, #2
	cmp r2, #0
	bge .L_080fe6ca
	movs r0, #1
	bl WaitFrames
	b .L_080fe826
.L_080fe6e4:
	.4byte 0x00000038
.L_080fe6e8:
	.4byte 0x0000001a
.L_080fe6ec:
	.4byte 0x00001046
.L_080fe6f0:
	.4byte 0x00001047
.L_080fe6f4:
	cmp r6, #0
	beq .L_080fe7c6
	ldr r3, [sp, #32]
	ldr r3, [r3, #40]
	mov r8, r3
	mov r0, r8
	bl RenderOutput_RedrawSavedRectFar
	movs r0, #1
	bl WaitFrames
	movs r3, #11
	str r3, [sp, #0]
	mov r0, r8
	movs r1, #0
	movs r2, #11
	movs r3, #28
	bl UiWindow_DrawDividerLineFar
	movs r3, #28
	str r3, [sp, #0]
	mov r0, r8
	movs r1, #2
	movs r2, #1
	ldr r3, [sp, #28]
	bl Menu_DrawPageIndicator
	ldr r0, [sp, #28]
	movs r4, #0
	lsls r0, r0, #3
	str r0, [sp, #20]
	str r4, [sp, #16]
	str r4, [sp, #12]
	str r4, [sp, #8]
.L_080fe738:
	ldr r1, [sp, #8]
	ldr r2, [sp, #12]
	ldr r3, [sp, #16]
	movs r7, #0
	mov r11, r1
	mov r9, r2
	mov r10, r3
.L_080fe746:
	ldr r1, [sp, #20]
	ldr r2, [sp, #28]
	str r4, [sp, #4]
	adds r0, r1, r2
	adds r0, r7, r0
	movs r1, #18
	adds r0, #7
	bl Math_Mod
	mov r3, r11
	adds r6, r3, r0
	adds r0, r6, #0
	adds r0, #48
	bl GameFlag_Test
	ldr r4, [sp, #4]
	cmp r0, #0
	beq .L_080fe79a
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #1
	adds r1, r4, r2
	movs r3, #0
	adds r5, r7, #1
	mov r2, r9
	str r3, [sp, #0]
	mov r0, r8
	adds r2, #1
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntryFar
	ldr r0, .L_080fe7fc
	lsls r3, r7, #3
	mov r2, r10
	adds r0, r6, r0
	adds r3, #8
	mov r1, r8
	adds r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	ldr r4, [sp, #4]
	b .L_080fe79c
.L_080fe79a:
	adds r5, r7, #1
.L_080fe79c:
	adds r7, r5, #0
	cmp r7, #8
	ble .L_080fe746
	ldr r3, [sp, #16]
	ldr r0, [sp, #12]
	ldr r1, [sp, #8]
	adds r3, #56
	adds r0, #7
	adds r1, #20
	adds r4, #1
	str r3, [sp, #16]
	str r0, [sp, #12]
	str r1, [sp, #8]
	cmp r4, #3
	ble .L_080fe738
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	movs r3, #1
	strb r3, [r2, #3]
	movs r6, #0
.L_080fe7c6:
	ldr r0, .L_080fe800
	bl Link_DrawShiftedTilePairFar
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080fe804
	movs r2, #7
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_080fe834
	ldr r3, [r1, #12]
	movs r2, #48
	ands r3, r2
	cmp r3, #0
	beq .L_080fe826
	ldr r3, [r1, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080fe808
	ldr r2, [sp, #28]
	adds r2, #1
	str r2, [sp, #28]
	b .L_080fe80e
	.2byte 0x0000
.L_080fe7fc:
	.4byte 0x000006d3
.L_080fe800:
	.4byte 0x06002500
.L_080fe804:
	.4byte gInput
.L_080fe808:
	ldr r3, [sp, #28]
	subs r3, #1
	str r3, [sp, #28]
.L_080fe80e:
	ldr r0, [sp, #28]
	movs r1, #2
	adds r0, #2
	bl Math_Mod
	str r0, [sp, #28]
	movs r0, #111
	bl Audio_PlayCue
	bl Runtime_SetMainState19
	movs r6, #1
.L_080fe826:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fe834
	b .L_080fe6f4
.L_080fe834:
	ldr r1, [sp, #32]
	ldr r0, [r1, #40]
	bl RenderOutput_RedrawSavedRectFar
	ldr r2, [sp, #24]
	ldr r0, [r2]
	bl RenderOutput_ClearListFar
	ldr r1, [sp, #32]
	movs r2, #140
	ldr r0, .L_080fe854
	lsls r2, r2, #2
	adds r3, r1, r2
	movs r1, #130
	movs r2, #3
	b .L_080fe858
.L_080fe854:
	.4byte 0x00000080
.L_080fe858:
	subs r2, #1
	strh r1, [r3]
	strh r0, [r3, #8]
	adds r1, #32
	adds r3, #2
	cmp r2, #0
	bge .L_080fe858
	ldr r0, [sp, #32]
	movs r1, #184
	ldr r3, [r0, #20]
	movs r2, #1
	lsls r1, r1, #1
	strb r2, [r3, #5]
	adds r3, r0, r1
	ldr r3, [r3]
	movs r0, #1
	strb r2, [r3, #5]
	bl Func_081054cc
	movs r0, #113
	bl Audio_PlayCue
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
