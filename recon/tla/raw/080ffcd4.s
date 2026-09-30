.syntax unified
	.thumb
	.global Func_080ffcd4
	.thumb_func
Func_080ffcd4:
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
	ldr r7, [r3]
	sub sp, #48
	ldr r2, [r7, #40]
	movs r3, #0
	str r3, [sp, #16]
	mov r10, r2
	movs r3, #5
	movs r2, #1
	adds r0, r7, #0
	movs r4, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r3, #30
	mov r11, r2
	adds r0, #48
	movs r2, #0
	mov r8, r4
	str r4, [sp, #4]
	bl UiWindow_UpdateOrCreate
	adds r3, r7, #0
	adds r3, #240
	str r3, [sp, #12]
	adds r0, r7, #0
	ldr r1, [r3]
	bl Func_080fa3d4
	ldr r5, .L_080ffedc
	movs r6, #24
	negs r6, r6
	adds r0, r5, #0
	mov r1, r10
	movs r2, #0
	adds r3, r6, #0
	subs r5, #2
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r5, #0
	mov r1, r10
	movs r2, #64
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	bl Func_080ff850
	movs r4, #226
	lsls r4, r4, #1
	adds r4, r4, r7
	mov r9, r4
	b .L_080ffeaa
.L_080ffd4c:
	mov r2, r8
	add r6, sp, #20
	cmp r2, #0
	beq .L_080ffdba
	cmp r2, #2
	bne .L_080ffd96
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #22
	adds r3, r7, r4
	ldrb r0, [r3]
	bl Owner_GetState
	mov r1, r9
	movs r2, #0
	bl Func_080fd6f0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	add r6, sp, #20
	strb r0, [r3]
	movs r1, #0
	adds r0, r6, #0
	bl Func_080ff7b4
	bl ItemMenu_HideAllIcons
	mov r0, r9
	bl Func_080fd6b0
	movs r3, #1
	movs r0, #1
	mov r11, r3
	bl WaitFrames
	b .L_080ffd98
.L_080ffd96:
	add r6, sp, #20
.L_080ffd98:
	mov r4, r11
	cmp r4, #0
	beq .L_080ffdac
	movs r2, #0
	mov r11, r2
	mov r0, r10
	movs r1, #0
	adds r2, r6, #0
	bl PsynergyMenu_DrawListPage
.L_080ffdac:
	mov r0, r10
	movs r1, #0
	adds r2, r6, #0
	bl PsynergyMenu_DrawRangePage
	movs r3, #0
	mov r8, r3
.L_080ffdba:
	movs r0, #1
	bl WaitFrames
	add r3, sp, #28
	ldr r1, [r6, #20]
	movs r0, #0
	str r3, [sp, #0]
	movs r2, #5
	add r3, sp, #36
	bl Func_080f8f9c
	ldr r1, [r6, #16]
	adds r5, r0, #0
	lsls r1, r1, #4
	adds r1, #60
	movs r0, #55
	bl Func_080f8a44
	cmp r5, #1
	bne .L_080ffde8
	movs r4, #1
	mov r8, r4
	mov r11, r4
.L_080ffde8:
	cmp r5, #0
	bne .L_080ffdf0
	movs r2, #1
	mov r8, r2
.L_080ffdf0:
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	bne .L_080ffdfc
	movs r4, #0
	mov r8, r4
.L_080ffdfc:
	ldr r5, .L_080ffee0
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080ffe14
	movs r0, #112
	bl Audio_PlayCue
	movs r2, #1
	str r2, [sp, #16]
	b .L_080ffeb8
.L_080ffe14:
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ffe2c
	movs r0, #113
	bl Audio_PlayCue
	movs r3, #1
	negs r3, r3
	str r3, [sp, #16]
	b .L_080ffeb8
.L_080ffe2c:
	ldr r3, [r5, #12]
	movs r1, #128
	lsls r1, r1, #1
	ands r3, r1
	cmp r3, #0
	bne .L_080ffe44
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ffeaa
.L_080ffe44:
	movs r0, #111
	str r1, [sp, #8]
	bl Audio_PlayCue
	movs r0, #28
	ldrsb r0, [r7, r0]
	movs r4, #129
	lsls r3, r0, #1
	lsls r4, r4, #2
	adds r3, r3, r4
	ldrh r3, [r7, r3]
	movs r2, #153
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r6, #24]
	ldr r1, [sp, #8]
	strb r2, [r7, r3]
	ldr r3, [r5, #12]
	ands r3, r1
	cmp r3, #0
	beq .L_080ffe72
	adds r0, #1
	b .L_080ffe74
.L_080ffe72:
	subs r0, #1
.L_080ffe74:
	movs r4, #139
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r7, r4
	ldrb r1, [r3]
	adds r0, r0, r1
	bl Math_Mod
	movs r3, #129
	lsls r2, r0, #1
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrh r3, [r7, r2]
	movs r4, #128
	str r3, [r7, #8]
	ldrh r1, [r7, r2]
	lsls r4, r4, #2
	adds r4, #22
	adds r3, r7, r4
	strb r1, [r3]
	strb r0, [r7, #28]
	adds r0, r7, #0
	ldrh r1, [r7, r2]
	bl Func_080f88c4
	movs r2, #2
	mov r8, r2
.L_080ffeaa:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ffeb8
	b .L_080ffd4c
.L_080ffeb8:
	ldr r0, [r7, #48]
	bl RenderOutput_PrepareForRedrawFar
	mov r0, r10
	bl RenderOutput_RedrawSavedRectFar
	ldr r3, [sp, #12]
	ldr r0, [r3]
	bl RenderOutput_ClearListFar
	ldr r0, [sp, #16]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ffedc:
	.4byte 0x00001037
.L_080ffee0:
	.4byte gInput
