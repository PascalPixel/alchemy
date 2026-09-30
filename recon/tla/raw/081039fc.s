.syntax unified
	.thumb
	.global Func_081039fc
	.thumb_func
Func_081039fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #168
	str r0, [sp, #104]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r0, #0
	mov r10, r3
	str r0, [sp, #100]
	mov r0, r10
	adds r0, #240
	movs r1, #1
	movs r2, #0
	movs r3, #2
	str r3, [sp, #88]
	str r0, [sp, #80]
	str r1, [sp, #96]
	str r2, [sp, #92]
	mov r3, r10
	ldr r1, [r0]
	mov r0, sp
	str r1, [sp, #84]
	add r5, sp, #140
	ldr r2, [r3, #20]
	adds r0, #132
	movs r3, #13
	movs r7, #0
	strb r3, [r2, #5]
	str r7, [sp, #132]
	str r0, [sp, #24]
	str r7, [r0, #4]
	adds r0, r5, #0
	bl Party_ListActiveOwnersFar
	mov r1, r10
	movs r3, #28
	ldrsb r3, [r1, r3]
	lsls r3, r3, #1
	ldrh r6, [r5, r3]
	movs r3, #29
	ldrsb r3, [r1, r3]
	adds r0, r6, #0
	lsls r3, r3, #1
	ldrh r5, [r5, r3]
	adds r1, r5, #0
	bl Func_08104e38
	movs r3, #151
	ldr r1, .L_08103a74
	lsls r3, r3, #1
	movs r2, #3
	add r3, r10
	b .L_08103a78
.L_08103a74:
	.4byte 0x000000c8
.L_08103a78:
	subs r2, #1
	strh r1, [r3]
	subs r3, #2
	cmp r2, #0
	bge .L_08103a78
	movs r3, #140
	lsls r3, r3, #1
	add r3, r10
	movs r2, #16
	strh r2, [r3]
	movs r3, #148
	lsls r3, r3, #1
	add r3, r10
	movs r1, #32
	strh r1, [r3]
	cmp r6, r5
	beq .L_08103aaa
	movs r2, #141
	lsls r2, r2, #1
	movs r3, #120
	add r2, r10
	strh r3, [r2]
	adds r3, #178
	add r3, r10
	strh r1, [r3]
.L_08103aaa:
	mov r2, r10
	ldr r0, [r2, #52]
	bl RenderOutput_ClearListFar
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	ldr r0, [sp, #80]
	bl UiWindow_CloseIfOpen
	movs r3, #5
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	ldr r0, [sp, #80]
	movs r3, #30
	movs r1, #0
	movs r2, #0
	bl UiWindow_UpdateOrCreate
	mov r3, sp
	movs r7, #1
	adds r3, #124
	str r7, [sp, #124]
	str r3, [sp, #28]
	str r7, [r3, #4]
	ldr r0, [sp, #104]
	cmp r0, #1
	bls .L_08103ae8
	b .L_08103c5c
.L_08103ae8:
	movs r0, #96
	bl Runtime_BumpAllocateAlternatePool
	movs r5, #166
	lsls r5, r5, #1
	str r0, [sp, #76]
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #0
	movs r3, #128
	str r1, [sp, #72]
	str r1, [sp, #68]
	lsls r3, r3, #2
	adds r3, #22
	add r3, r10
	ldrb r6, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #90
	add r3, r10
	ldrb r3, [r3]
	movs r2, #31
	mov r11, r3
	movs r3, #150
	lsls r3, r3, #2
	add r3, r10
	ldrb r3, [r3]
	mov r8, r2
	mov r9, r0
	mov r0, r8
	ands r0, r3
	movs r3, #182
	lsls r3, r3, #1
	add r3, r10
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #8
	ands r3, r2
	lsls r3, r3, #16
	lsrs r3, r3, #16
	mov r8, r0
	adds r0, r6, #0
	str r3, [sp, #64]
	bl Owner_GetState
	adds r7, r0, #0
	ldr r3, .L_08103e54
	adds r1, r7, #0
	adds r2, r5, #0
	mov r0, r9
	mov lr, r3
	.2byte 0xf800
	mov r1, r11
	adds r0, r6, #0
	mov r2, r8
	bl Djinn_DeactivateFar
	ldr r1, [sp, #104]
	cmp r1, #0
	bne .L_08103ba6
	movs r3, #174
	lsls r3, r3, #1
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	movs r2, #31
	str r3, [sp, #72]
	movs r3, #173
	lsls r3, r3, #1
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	movs r5, #128
	ands r2, r3
	str r2, [sp, #68]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r10
	ldrh r3, [r3]
	lsls r5, r5, #8
	ands r5, r3
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r0, r6, #0
	ldr r1, [sp, #72]
	bl Djinn_AddToOwnerFar
	cmp r5, #0
	beq .L_08103ba6
	adds r0, r6, #0
	ldr r1, [sp, #72]
	ldr r2, [sp, #68]
	bl Djinn_ActivateFar
.L_08103ba6:
	adds r0, r6, #0
	bl Owner_RecalculateStatsFar
	mov r0, sp
	mov r2, sp
	adds r0, #120
	mov r3, r9
	str r0, [sp, #56]
	adds r3, #88
	adds r2, #116
	adds r1, r7, #0
	str r3, [sp, #60]
	str r2, [sp, #52]
	str r2, [sp, #0]
	adds r1, #88
	ldr r2, [sp, #76]
	adds r0, r3, #0
	ldr r3, [sp, #56]
	bl Func_08101860
	movs r2, #166
	str r0, [sp, #124]
	mov r1, r9
	lsls r2, r2, #1
	ldr r5, .L_08103e54
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	movs r3, #140
	lsls r3, r3, #1
	adds r3, #255
	add r3, r10
	ldrb r6, [r3]
	adds r0, r6, #0
	bl Owner_GetState
	movs r2, #166
	adds r7, r0, #0
	adds r1, r7, #0
	mov r0, r9
	lsls r2, r2, #1
	mov lr, r5
	.2byte 0xf800
	ldr r3, [sp, #104]
	cmp r3, #0
	bne .L_08103c0c
	adds r0, r6, #0
	ldr r1, [sp, #72]
	ldr r2, [sp, #68]
	bl Djinn_DeactivateFar
.L_08103c0c:
	adds r0, r6, #0
	mov r1, r11
	mov r2, r8
	bl Djinn_AddToOwnerFar
	ldr r0, [sp, #64]
	cmp r0, #0
	beq .L_08103c26
	adds r0, r6, #0
	mov r1, r11
	mov r2, r8
	bl Djinn_ActivateFar
.L_08103c26:
	adds r0, r6, #0
	bl Owner_RecalculateStatsFar
	ldr r2, [sp, #52]
	adds r1, r7, #0
	str r2, [sp, #0]
	ldr r3, [sp, #56]
	ldr r2, [sp, #76]
	ldr r0, [sp, #60]
	adds r1, #88
	bl Func_08101860
	ldr r3, [sp, #28]
	movs r2, #166
	str r0, [r3, #4]
	mov r1, r9
	lsls r2, r2, #1
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	mov r0, r9
	bl Sys_Free
	ldr r0, [sp, #76]
	bl Sys_Free
	b .L_08103d1a
.L_08103c5c:
	ldr r3, [sp, #104]
	subs r3, #2
	cmp r3, #1
	bhi .L_08103d1a
	movs r0, #96
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #48]
	movs r0, #166
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r10
	ldrb r6, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #90
	add r3, r10
	ldrb r3, [r3]
	mov r9, r0
	mov r11, r3
	movs r3, #150
	lsls r3, r3, #2
	add r3, r10
	ldrb r3, [r3]
	movs r0, #31
	ldr r2, [sp, #104]
	mov r8, r0
	mov r1, r8
	ands r1, r3
	movs r3, #3
	eors r3, r2
	negs r5, r3
	adds r0, r6, #0
	mov r8, r1
	orrs r5, r3
	bl Owner_GetState
	lsrs r5, r5, #31
	subs r5, r7, r5
	movs r2, #166
	adds r7, r0, #0
	adds r1, r7, #0
	lsls r2, r2, #1
	mov r0, r9
	ldr r3, .L_08103e54
	mov lr, r3
	.2byte 0xf800
	adds r0, r6, #0
	mov r1, r11
	mov r2, r8
	bl Djinn_DeactivateFar
	cmp r5, #0
	beq .L_08103cda
	adds r0, r6, #0
	mov r1, r11
	mov r2, r8
	bl Djinn_ActivateFar
.L_08103cda:
	adds r0, r6, #0
	bl Owner_RecalculateStatsFar
	mov r0, r9
	add r2, sp, #108
	adds r1, r7, #0
	add r3, sp, #112
	str r2, [sp, #0]
	adds r1, #88
	ldr r2, [sp, #48]
	adds r0, #88
	bl Func_08101860
	movs r2, #166
	str r0, [sp, #124]
	mov r1, r9
	lsls r2, r2, #1
	ldr r3, .L_08103e54
	adds r0, r7, #0
	mov lr, r3
	.2byte 0xf800
	ldr r0, [sp, #28]
	ldr r3, [sp, #124]
	movs r1, #0
	str r3, [r0, #4]
	mov r0, r9
	str r1, [sp, #88]
	bl Sys_Free
	ldr r0, [sp, #48]
	bl Sys_Free
.L_08103d1a:
	ldr r0, [sp, #124]
	movs r1, #5
	subs r0, #1
	bl Math_Div
	adds r0, #1
	str r0, [sp, #124]
	cmp r0, #0
	bne .L_08103d30
	movs r3, #1
	str r3, [sp, #124]
.L_08103d30:
	ldr r2, [sp, #28]
	movs r1, #5
	ldr r0, [r2, #4]
	subs r0, #1
	bl Math_Div
	ldr r3, [sp, #28]
	adds r0, #1
	str r0, [r3, #4]
	cmp r0, #0
	bne .L_08103d4c
	ldr r0, [sp, #28]
	movs r3, #1
	str r3, [r0, #4]
.L_08103d4c:
	mov r1, r10
	adds r1, #40
	str r1, [sp, #44]
	movs r6, #15
	movs r5, #2
	adds r0, r1, #0
	movs r2, #5
	movs r1, #0
	movs r3, #15
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl UiWindow_UpdateOrCreate
	mov r2, r10
	adds r2, #56
	str r2, [sp, #40]
	movs r3, #15
	adds r0, r2, #0
	movs r1, #15
	movs r2, #5
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl UiWindow_UpdateOrCreate
	ldr r0, [sp, #84]
	bl RenderOutput_RedrawSavedRectFar
	ldr r3, [sp, #104]
	cmp r3, #2
	bne .L_08103d8c
	ldr r0, .L_08103e58
	b .L_08103d94
.L_08103d8c:
	ldr r0, [sp, #104]
	cmp r0, #3
	bne .L_08103dac
	ldr r0, .L_08103e5c
.L_08103d94:
	ldr r1, [sp, #84]
	movs r2, #96
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_08103e60
	ldr r1, [sp, #84]
	movs r2, #96
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	b .L_08103de4
.L_08103dac:
	ldr r1, [sp, #104]
	cmp r1, #0
	bne .L_08103dcc
	ldr r0, .L_08103e64
	ldr r1, [sp, #84]
	movs r2, #128
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_08103e60
	ldr r1, [sp, #84]
	movs r2, #128
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	b .L_08103de4
.L_08103dcc:
	ldr r0, .L_08103e68
	ldr r1, [sp, #84]
	movs r2, #128
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_08103e60
	ldr r1, [sp, #84]
	movs r2, #128
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_08103de4:
	movs r3, #182
	lsls r3, r3, #1
	add r3, r10
	ldrh r6, [r3]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r10
	ldrh r7, [r3]
	ldr r3, [sp, #104]
	movs r2, #0
	mov r8, r2
	cmp r3, #1
	bne .L_08103e02
	movs r0, #1
	mov r8, r0
.L_08103e02:
	movs r1, #128
	lsls r1, r1, #8
	adds r5, r6, #0
	ands r5, r1
	mov r9, r1
	cmp r5, #0
	bne .L_08103e16
	movs r0, #2
	bl Func_080380b8
.L_08103e16:
	mov r3, r8
	lsls r2, r3, #3
	ldr r0, [sp, #84]
	movs r1, #40
	adds r3, r6, #0
	bl Func_08101a54
	ldr r0, [sp, #104]
	cmp r0, #0
	bne .L_08103e6c
	adds r3, r7, #0
	mov r1, r9
	ands r3, r1
	cmp r3, #0
	bne .L_08103e3a
	movs r0, #2
	bl Func_080380b8
.L_08103e3a:
	ldr r0, [sp, #84]
	movs r1, #40
	movs r2, #16
	adds r3, r7, #0
	bl Func_08101a54
	b .L_08103e9e
.L_08103e48:
	movs r7, #1
	b .L_08104490
.L_08103e4c:
	movs r0, #113
	movs r7, #2
	b .L_081043d8
	.2byte 0x0000
.L_08103e54:
	.4byte IwramCopyWords
.L_08103e58:
	.4byte 0x000010d6
.L_08103e5c:
	.4byte 0x000010d5
.L_08103e60:
	.4byte 0x000010f2
.L_08103e64:
	.4byte 0x000010d8
.L_08103e68:
	.4byte 0x000010d7
.L_08103e6c:
	ldr r3, [sp, #104]
	subs r3, #2
	cmp r3, #1
	bhi .L_08103e9e
	cmp r5, #0
	beq .L_08103e7e
	movs r0, #2
	bl Func_080380b8
.L_08103e7e:
	ldr r0, [sp, #84]
	movs r1, #40
	movs r2, #16
	adds r3, r6, #0
	bl Func_08101a54
	movs r1, #242
	movs r3, #0
	lsls r1, r1, #8
	str r3, [sp, #0]
	adds r1, #150
	ldr r0, [sp, #84]
	movs r2, #7
	movs r3, #1
	bl UiWindow_SetTilemapEntryFar
.L_08103e9e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #1
	add r3, r10
	ldr r6, [r3]
	ldr r3, .L_081041c0
	ldr r2, [r3, #4]
	str r2, [sp, #36]
	ldr r3, [r3, #12]
	str r3, [sp, #32]
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #44
	adds r5, r6, r3
	ldr r3, [r5]
	cmp r3, #0
	bne .L_08103ec8
	b .L_081040bc
.L_08103ec8:
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #132
	mov r9, r0
	movs r0, #0
	str r0, [sp, #36]
	str r0, [sp, #32]
	lsls r1, r1, #6
	adds r1, #40
	adds r2, r6, r1
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	ldr r3, [r5]
	subs r3, #5
	cmp r3, #18
	bls .L_08103ef0
	b .L_081040b6
.L_08103ef0:
	ldr r2, .L_081041c4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08103ef8:
	.4byte .L_08103f44
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_08103f6e
	.4byte .L_0810400e
	.4byte .L_08103f44
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_081040b6
	.4byte .L_08103f44
.L_08103f44:
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #40
	adds r2, r6, r3
	ldr r3, [r2]
	cmp r3, #100
	beq .L_08103f54
	b .L_081040b6
.L_08103f54:
	movs r0, #1
	movs r3, #0
	movs r1, #132
	str r0, [sp, #32]
	str r0, [sp, #36]
	lsls r1, r1, #6
	str r3, [r2]
	adds r1, #44
	adds r2, r6, r1
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	b .L_081040b6
.L_08103f6e:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	adds r3, r6, r2
	ldr r3, [r3]
	cmp r3, #60
	beq .L_08103f7e
	b .L_081040b6
.L_08103f7e:
	movs r3, #8
	add r3, r11
	mov r8, r3
	movs r2, #128
	mov r1, r8
	ldr r3, .L_081041c8
	lsls r2, r2, #2
	mov r0, r9
	mov lr, r3
	.2byte 0xf800
	bl Func_08100e5c
	movs r1, #8
	movs r2, #0
	movs r3, #1
	adds r0, #3
	bl UiText_OpenMessageWindowFar
	adds r7, r0, #0
	mov r0, r10
	ldr r3, [r0, #20]
	movs r5, #1
	strb r5, [r3, #5]
	movs r0, #2
	movs r1, #96
	bl Func_080f8ab4
	ldr r3, .L_081041cc
	movs r1, #139
	lsls r1, r1, #2
	adds r3, r3, r1
	strb r5, [r3]
	b .L_08103fc6
.L_08103fc0:
	movs r0, #1
	bl WaitFrames
.L_08103fc6:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_08103fc0
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r2, #128
	mov r1, r9
	ldr r3, .L_081041c8
	lsls r2, r2, #2
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	bl Func_08038290
	movs r1, #132
	lsls r1, r1, #6
	movs r3, #1
	mov r0, r11
	adds r1, #40
	strb r3, [r0, #3]
	movs r2, #0
	str r3, [sp, #96]
	adds r3, r6, r1
	str r2, [r3]
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #44
	adds r2, r6, r3
	movs r3, #11
	str r3, [r2]
	mov r0, r10
	ldr r2, [r0, #20]
	b .L_081040b2
.L_0810400e:
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #40
	adds r3, r6, r1
	ldr r3, [r3]
	cmp r3, #60
	bne .L_081040b6
	movs r2, #8
	add r2, r11
	mov r8, r2
	movs r2, #128
	mov r1, r8
	ldr r3, .L_081041c8
	lsls r2, r2, #2
	mov r0, r9
	mov lr, r3
	.2byte 0xf800
	bl Func_08100e5c
	movs r1, #8
	movs r2, #0
	movs r3, #1
	adds r0, #2
	bl UiText_OpenMessageWindowFar
	adds r7, r0, #0
	mov r0, r10
	ldr r3, [r0, #20]
	movs r5, #1
	strb r5, [r3, #5]
	movs r0, #106
	movs r1, #56
	bl Func_080f8ab4
	ldr r3, .L_081041cc
	movs r1, #139
	lsls r1, r1, #2
	adds r3, r3, r1
	strb r5, [r3]
	b .L_08104064
.L_0810405e:
	movs r0, #1
	bl WaitFrames
.L_08104064:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_0810405e
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r2, #128
	mov r1, r9
	ldr r3, .L_081041c8
	lsls r2, r2, #2
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	bl Func_08038290
	mov r2, r11
	movs r3, #1
	strb r3, [r2, #3]
	movs r0, #1
	bl WaitFrames
	movs r0, #132
	lsls r0, r0, #6
	movs r1, #132
	movs r3, #1
	adds r0, #40
	lsls r1, r1, #6
	str r3, [sp, #96]
	movs r5, #0
	adds r3, r6, r0
	adds r1, #44
	str r5, [r3]
	adds r2, r6, r1
	movs r3, #12
	str r3, [r2]
	mov r3, r10
	ldr r2, [r3, #20]
.L_081040b2:
	movs r3, #13
	strb r3, [r2, #5]
.L_081040b6:
	mov r0, r9
	bl Sys_Free
.L_081040bc:
	ldr r0, [sp, #96]
	cmp r0, #0
	bne .L_081040c4
	b .L_0810422e
.L_081040c4:
	ldr r2, [sp, #104]
	movs r1, #1
	mov r8, r1
	cmp r2, #1
	bhi .L_0810411c
	movs r3, #24
	str r3, [sp, #0]
	ldr r0, [sp, #84]
	movs r3, #224
	movs r1, #128
	movs r2, #16
	bl UiWindow_ClearInteriorTilesFar
	ldr r3, [sp, #88]
	cmp r3, #1
	bne .L_081040f2
	ldr r0, .L_081041d0
	ldr r1, [sp, #84]
	movs r2, #128
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	b .L_0810411c
.L_081040f2:
	ldr r0, [sp, #88]
	movs r2, #2
	eors r2, r0
	negs r3, r2
	orrs r3, r2
	movs r2, #133
	lsls r2, r2, #2
	lsrs r3, r3, #31
	adds r3, r3, r2
	mov r2, r10
	adds r2, #2
	ldrb r0, [r2, r3]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_081041d4
	ldr r1, [sp, #84]
	movs r2, #128
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
.L_0810411c:
	movs r3, #1
	mov r1, r11
	strb r3, [r1, #6]
	mov r2, r10
	ldr r0, [r2, #40]
	bl RenderOutput_PrepareForRedrawFar
	mov r3, r10
	ldr r0, [r3, #56]
	bl RenderOutput_PrepareForRedrawFar
	ldr r0, [sp, #96]
	lsrs r3, r0, #1
	cmp r3, #0
	beq .L_08104140
	movs r0, #1
	bl WaitFrames
.L_08104140:
	ldr r1, [sp, #104]
	cmp r1, #3
	bne .L_0810414e
	movs r2, #0
	movs r7, #0
	mov r8, r2
	b .L_0810415e
.L_0810414e:
	ldr r3, [sp, #104]
	cmp r3, #2
	bne .L_0810415c
	movs r0, #0
	movs r7, #1
	mov r8, r0
	b .L_0810415e
.L_0810415c:
	movs r7, #2
.L_0810415e:
	ldr r1, [sp, #88]
	cmp r1, #0
	beq .L_081041d8
	cmp r1, #1
	beq .L_08104172
	ldr r0, [sp, #104]
	movs r1, #0
	bl Func_08103168
	b .L_08104228
.L_08104172:
	ldr r2, [sp, #104]
	cmp r2, #1
	bne .L_0810417a
	movs r7, #4
.L_0810417a:
	movs r6, #140
	ldr r1, [sp, #88]
	lsls r6, r6, #1
	adds r6, #255
	mov r3, r10
	add r6, r10
	ldr r0, [r3, #40]
	movs r5, #0
	ldrb r3, [r6]
	movs r2, #0
	str r1, [sp, #0]
	movs r1, #0
	str r5, [sp, #4]
	str r7, [sp, #8]
	str r5, [sp, #12]
	str r5, [sp, #16]
	bl Func_08103218
	ldr r1, [sp, #88]
	mov r2, r10
	ldr r0, [r2, #56]
	ldrb r3, [r6]
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r1, [sp, #24]
	str r7, [sp, #8]
	ldr r2, [r1, #4]
	movs r1, #0
	adds r2, #1
	str r2, [sp, #12]
	movs r2, #0
	str r5, [sp, #16]
	bl Func_08103218
	b .L_08104228
.L_081041c0:
	.4byte gInput
.L_081041c4:
	.4byte .L_08103ef8
.L_081041c8:
	.4byte IwramCopyWords
.L_081041cc:
	.4byte gPartyState
.L_081041d0:
	.4byte 0x000010d2
.L_081041d4:
	.4byte 0x000010d1
.L_081041d8:
	ldr r2, [sp, #104]
	cmp r2, #1
	bne .L_081041e4
	movs r3, #0
	movs r7, #1
	mov r8, r3
.L_081041e4:
	ldr r2, [sp, #88]
	movs r5, #151
	mov r1, r10
	lsls r5, r5, #2
	ldr r0, [r1, #40]
	add r5, r10
	mov r1, r8
	ldrb r3, [r5]
	movs r6, #1
	str r2, [sp, #0]
	str r1, [sp, #4]
	str r2, [sp, #12]
	movs r1, #0
	movs r2, #0
	str r7, [sp, #8]
	str r6, [sp, #16]
	bl Func_08103218
	mov r2, r10
	ldr r0, [r2, #56]
	mov r2, r8
	ldrb r3, [r5]
	str r2, [sp, #4]
	ldr r2, [sp, #132]
	ldr r1, [sp, #88]
	adds r2, #1
	str r1, [sp, #0]
	str r2, [sp, #12]
	movs r1, #0
	movs r2, #0
	str r7, [sp, #8]
	str r6, [sp, #16]
	bl Func_08103218
.L_08104228:
	movs r3, #0
	mov r0, r11
	strb r3, [r0, #6]
.L_0810422e:
	ldr r1, [sp, #88]
	cmp r1, #1
	bgt .L_081042d2
	ldr r0, [sp, #28]
	lsls r4, r1, #2
	mov r2, r10
	adds r3, r4, r0
	ldr r7, [r2, #56]
	ldr r2, [r3]
	cmp r2, #1
	ble .L_081042d2
	movs r5, #0
	cmp r5, r2
	bge .L_0810428c
	adds r6, r3, #0
.L_0810424c:
	movs r2, #240
	lsls r2, r2, #8
	adds r2, #49
	adds r1, r5, r2
	cmp r5, #8
	ble .L_0810425e
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #63
.L_0810425e:
	ldr r0, [sp, #24]
	ldr r3, [r4, r0]
	cmp r5, r3
	bne .L_0810426a
	ldr r2, .L_0810451c
	adds r1, r1, r2
.L_0810426a:
	ldr r3, [r6]
	ldrh r2, [r7, #8]
	adds r0, r7, #0
	subs r2, r2, r3
	adds r2, r2, r5
	movs r3, #0
	str r3, [sp, #0]
	subs r2, #2
	subs r3, #1
	str r4, [sp, #20]
	bl UiWindow_SetTilemapEntryFar
	ldr r3, [r6]
	adds r5, #1
	ldr r4, [sp, #20]
	cmp r5, r3
	blt .L_0810424c
.L_0810428c:
	ldr r0, [sp, #28]
	ldrh r2, [r7, #8]
	ldr r3, [r4, r0]
	movs r1, #241
	movs r6, #1
	negs r6, r6
	subs r2, r2, r3
	lsls r1, r1, #8
	movs r5, #0
	adds r0, r7, #0
	adds r3, r6, #0
	adds r1, #40
	subs r2, #3
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	ldrh r2, [r7, #8]
	movs r1, #241
	lsls r1, r1, #8
	adds r1, #41
	subs r2, #2
	adds r0, r7, #0
	adds r3, r6, #0
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	ldrh r3, [r7, #14]
	movs r2, #2
	lsls r3, r3, #16
	asrs r3, r3, #18
	mov r1, r11
	lsls r2, r3
	ldrb r3, [r1, #3]
	orrs r2, r3
	strb r2, [r1, #3]
.L_081042d2:
	ldr r2, [sp, #92]
	movs r1, #60
	adds r2, #1
	adds r0, r2, #0
	str r2, [sp, #92]
	bl __modsi3
	subs r6, r0, #5
	cmp r6, #0
	bge .L_081042e8
	movs r6, #0
.L_081042e8:
	cmp r6, #29
	ble .L_081042ee
	movs r6, #29
.L_081042ee:
	ldr r5, .L_08104520
	movs r0, #0
	adds r1, r5, #0
	bl Func_08105350
	movs r0, #1
	adds r1, r5, #0
	bl Func_08105350
	ldr r3, [sp, #104]
	cmp r3, #1
	bhi .L_08104368
	movs r1, #30
	adds r0, r6, #0
	bl __modsi3
	adds r3, r0, #0
	lsls r0, r3, #4
	adds r0, r0, r3
	lsls r0, r0, #4
	adds r0, r0, r3
	lsls r0, r0, #2
	bl Trig_Sin
	ldr r3, .L_08104524
	adds r1, r0, #0
	movs r0, #16
	mov lr, r3
	.2byte 0xf800
	movs r3, #6
	negs r5, r0
	negs r3, r3
	cmp r5, r3
	bge .L_08104334
	adds r5, r3, #0
.L_08104334:
	cmp r5, #12
	ble .L_0810433a
	movs r5, #12
.L_0810433a:
	adds r0, r6, #0
	movs r1, #35
	bl __modsi3
	lsls r6, r0, #1
	adds r1, r6, #0
	adds r2, r5, #0
	movs r0, #0
	adds r1, #34
	adds r2, #20
	bl Func_08105300
	ldr r0, [sp, #104]
	cmp r0, #0
	bne .L_08104372
	movs r1, #99
	movs r2, #36
	subs r1, r1, r6
	subs r2, r2, r5
	movs r0, #1
	bl Func_08105300
	b .L_08104372
.L_08104368:
	movs r0, #0
	movs r1, #32
	movs r2, #30
	bl Func_08105300
.L_08104372:
	ldr r1, [sp, #96]
	cmp r1, #0
	beq .L_08104386
	movs r2, #0
	ldr r0, [sp, #100]
	movs r1, #2
	str r2, [sp, #96]
	bl Func_08100e28
	str r0, [sp, #100]
.L_08104386:
	ldr r3, [sp, #100]
	movs r1, #16
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #3
	adds r0, #80
	bl Func_080f8a44
	ldr r0, [sp, #92]
	movs r3, #3
	ands r3, r0
	cmp r3, #0
	bne .L_081043c2
	movs r3, #4
	ands r3, r0
	cmp r3, #0
	beq .L_081043b6
	ldr r1, .L_08104528
	ldr r3, .L_0810452c
	ldr r0, .L_08104530
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	b .L_081043c2
.L_081043b6:
	ldr r3, .L_08104534
	ldr r0, .L_08104530
	movs r1, #32
	ldr r2, .L_08104538
	mov lr, r3
	.2byte 0xf800
.L_081043c2:
	ldr r1, [sp, #36]
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq .L_081043e0
	ldr r2, [sp, #100]
	cmp r2, #0
	bne .L_081043d4
	b .L_08103e48
.L_081043d4:
	movs r0, #113
	movs r7, #1
.L_081043d8:
	bl Audio_PlayCue
	negs r7, r7
	b .L_08104490
.L_081043e0:
	ldr r0, [sp, #36]
	movs r3, #8
	ands r3, r0
	cmp r3, #0
	beq .L_081043ec
	b .L_08103e4c
.L_081043ec:
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	bne .L_081043d4
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0810441c
	ldr r1, [sp, #104]
	cmp r1, #1
	bhi .L_08104488
	ldr r0, [sp, #88]
	movs r1, #3
	adds r0, #1
	bl __modsi3
	movs r2, #2
	str r0, [sp, #88]
	movs r0, #111
	str r2, [sp, #96]
	bl Audio_PlayCue
	b .L_08104488
.L_0810441c:
	ldr r0, [sp, #32]
	movs r3, #32
	ands r3, r0
	cmp r3, #0
	beq .L_08104452
	ldr r1, [sp, #88]
	cmp r1, #1
	bgt .L_08104488
	ldr r2, [sp, #24]
	lsls r3, r1, #2
	adds r5, r3, r2
	ldr r0, [r5]
	subs r0, #1
	str r0, [r5]
	ldr r2, [sp, #28]
	ldr r1, [r3, r2]
	bl Func_08100e28
	str r0, [r5]
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080138a8
	movs r3, #1
	str r3, [sp, #96]
	b .L_08104488
.L_08104452:
	ldr r0, [sp, #32]
	movs r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_08104488
	ldr r1, [sp, #88]
	cmp r1, #1
	bgt .L_08104488
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080138a8
	ldr r2, [sp, #88]
	ldr r0, [sp, #24]
	lsls r3, r2, #2
	adds r5, r3, r0
	ldr r0, [r5]
	movs r1, #1
	adds r0, #1
	str r0, [r5]
	ldr r2, [sp, #28]
	str r1, [sp, #96]
	ldr r1, [r3, r2]
	bl Func_08100e28
	str r0, [r5]
.L_08104488:
	movs r0, #1
	bl WaitFrames
	b .L_08103e9e
.L_08104490:
	movs r0, #0
	movs r1, #0
	bl Func_08105350
	movs r0, #1
	movs r1, #0
	bl Func_08105350
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0810453c
	bl Func_080145a8
	movs r5, #192
	lsls r5, r5, #18
	ldr r2, [r5, #60]
	movs r3, #1
	strb r3, [r2, #6]
	ldr r0, [sp, #80]
	movs r1, #1
	bl UiWindow_CloseIfOpen
	movs r0, #1
	bl WaitFrames
	movs r3, #5
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	ldr r0, [sp, #80]
	movs r2, #0
	movs r3, #17
	movs r1, #13
	bl UiWindow_UpdateOrCreate
	movs r1, #1
	ldr r0, [sp, #44]
	bl UiWindow_CloseIfOpen
	movs r1, #1
	ldr r0, [sp, #40]
	bl UiWindow_CloseIfOpen
	mov r3, r10
	ldr r0, [r3, #52]
	bl RenderOutput_RedrawSavedRectFar
	mov r1, r10
	ldr r0, [r1, #44]
	bl RenderOutput_RedrawSavedRectFar
	mov r2, r10
	ldr r0, [r2, #16]
	bl RenderOutput_RedrawSavedRectFar
	ldr r3, [r5, #60]
	movs r6, #0
	strb r6, [r3, #6]
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	add sp, #168
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810451c:
	.4byte 0xfffff000
.L_08104520:
	.4byte 0xffff4000
.L_08104524:
	.4byte IwramMulQ16
.L_08104528:
	.4byte Data_081059b4
.L_0810452c:
	.4byte IwramCopyWords
.L_08104530:
	.4byte 0x060052c0
.L_08104534:
	.4byte IwramFillWords
.L_08104538:
	.4byte 0x44444444
.L_0810453c:
	.4byte Func_08104da8
