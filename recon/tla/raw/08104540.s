.syntax unified
	.thumb
	.global Func_08104540
	.thumb_func
Func_08104540:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #88
	movs r1, #0
	adds r3, #220
	movs r0, #1
	ldr r3, [r3]
	str r1, [sp, #56]
	str r0, [sp, #60]
	mov r10, r3
	ldr r2, [r3, #20]
	movs r3, #13
	strb r3, [r2, #5]
	ldr r1, .L_08104580
	mov r2, sp
	movs r3, #151
	adds r2, #80
	movs r7, #0
	lsls r3, r3, #1
	str r2, [sp, #24]
	str r7, [sp, #80]
	add r3, r10
	str r7, [r2, #4]
	movs r2, #3
	b .L_08104584
.L_08104580:
	.4byte 0x000000c8
.L_08104584:
	subs r2, #1
	strh r1, [r3]
	subs r3, #2
	cmp r2, #0
	bge .L_08104584
	mov r3, r10
	ldr r0, [r3, #52]
	bl RenderOutput_ClearListFar
	movs r0, #1
	bl WaitFrames
	mov r0, sp
	adds r0, #72
	movs r7, #1
	str r0, [sp, #28]
	str r7, [sp, #72]
	str r7, [r0, #4]
	movs r0, #96
	bl Runtime_BumpAllocateAlternatePool
	adds r5, r0, #0
	movs r0, #166
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	add r3, r10
	adds r6, r0, #0
	ldrb r0, [r3]
	bl Owner_GetState
	adds r1, r0, #0
	add r2, sp, #64
	adds r1, #88
	str r2, [sp, #0]
	add r3, sp, #68
	adds r2, r5, #0
	adds r0, r1, #0
	bl Func_08101860
	ldr r1, [sp, #28]
	str r0, [sp, #72]
	str r0, [r1, #4]
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r5, #0
	bl Sys_Free
	ldr r0, [sp, #72]
	movs r1, #6
	subs r0, #1
	bl __divsi3
	adds r0, #1
	str r0, [sp, #72]
	cmp r0, #0
	bne .L_08104600
	str r7, [sp, #72]
.L_08104600:
	ldr r2, [sp, #28]
	movs r1, #6
	ldr r0, [r2, #4]
	subs r0, #1
	bl __divsi3
	ldr r3, [sp, #28]
	adds r0, #1
	str r0, [r3, #4]
	cmp r0, #0
	bne .L_08104618
	str r7, [r3, #4]
.L_08104618:
	mov r0, r10
	movs r5, #2
	adds r0, #40
	movs r6, #15
	movs r1, #0
	movs r2, #5
	movs r3, #15
	str r5, [sp, #4]
	str r0, [sp, #52]
	str r6, [sp, #0]
	bl UiWindow_UpdateOrCreate
	mov r1, r10
	adds r1, #56
	str r1, [sp, #48]
	movs r3, #15
	adds r0, r1, #0
	movs r2, #5
	movs r1, #15
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl UiWindow_UpdateOrCreate
	mov r2, r10
	adds r2, #240
	str r2, [sp, #44]
	ldr r0, [r2]
	bl RenderOutput_RedrawSavedRectFar
	mov r3, r10
	ldr r0, [r3, #16]
	bl RenderOutput_RedrawSavedRectFar
	ldr r5, .L_08104904
	mov r0, r10
	ldr r1, [r0, #16]
	movs r2, #0
	adds r0, r5, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #2
	mov r2, r10
	ldr r1, [r2, #16]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #151
	mov r1, sp
	lsls r2, r2, #2
	ldr r0, [sp, #24]
	adds r1, #72
	add r2, r10
	str r1, [sp, #20]
	str r2, [sp, #32]
	movs r3, #0
	mov r9, r3
	mov r11, r0
.L_08104690:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	ldr r1, [sp, #60]
	mov r8, r3
	ldr r3, .L_08104908
	ldr r0, [r3, #4]
	str r0, [sp, #40]
	ldr r3, [r3, #12]
	str r3, [sp, #36]
	cmp r1, #0
	beq .L_08104708
	mov r3, r8
	movs r2, #1
	strb r2, [r3, #6]
	mov r1, r10
	ldr r0, [r1, #40]
	bl RenderOutput_PrepareForRedrawFar
	mov r2, r10
	ldr r0, [r2, #56]
	bl RenderOutput_PrepareForRedrawFar
	ldr r1, [sp, #32]
	mov r2, r9
	mov r3, r10
	ldr r0, [r3, #40]
	ldrb r3, [r1]
	str r2, [sp, #0]
	str r2, [sp, #4]
	str r2, [sp, #12]
	movs r1, #3
	movs r2, #1
	str r1, [sp, #8]
	str r2, [sp, #16]
	movs r1, #0
	movs r2, #0
	bl Func_08103218
	ldr r1, [sp, #32]
	mov r2, r9
	mov r3, r10
	ldr r0, [r3, #56]
	ldrb r3, [r1]
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r2, [sp, #80]
	movs r1, #3
	adds r2, #1
	str r2, [sp, #12]
	movs r2, #1
	str r1, [sp, #8]
	str r2, [sp, #16]
	movs r1, #0
	movs r2, #0
	bl Func_08103218
	mov r3, r9
	mov r0, r8
	strb r3, [r0, #6]
.L_08104708:
	ldr r2, [sp, #28]
	movs r1, #0
	ldr r3, [r1, r2]
	cmp r3, #1
	ble .L_081047aa
	mov r0, r10
	movs r5, #0
	ldr r6, [r0, #56]
	cmp r5, r3
	bge .L_08104760
	adds r7, r2, #0
	adds r7, #0
.L_08104720:
	movs r2, #240
	lsls r2, r2, #8
	adds r2, #49
	adds r1, r5, r2
	cmp r5, #8
	ble .L_08104732
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #63
.L_08104732:
	ldr r2, [sp, #24]
	movs r0, #0
	ldr r3, [r0, r2]
	cmp r5, r3
	bne .L_08104740
	ldr r3, .L_0810490c
	adds r1, r1, r3
.L_08104740:
	ldr r3, [r7]
	ldrh r2, [r6, #8]
	mov r0, r9
	subs r2, r2, r3
	adds r2, r2, r5
	movs r3, #1
	str r0, [sp, #0]
	negs r3, r3
	subs r2, #2
	adds r0, r6, #0
	bl UiWindow_SetTilemapEntryFar
	ldr r3, [r7]
	adds r5, #1
	cmp r5, r3
	blt .L_08104720
.L_08104760:
	ldr r0, [sp, #28]
	movs r1, #0
	ldr r3, [r1, r0]
	ldrh r2, [r6, #8]
	mov r1, r9
	str r1, [sp, #0]
	movs r5, #1
	movs r1, #241
	negs r5, r5
	subs r2, r2, r3
	lsls r1, r1, #8
	adds r0, r6, #0
	adds r3, r5, #0
	subs r2, #3
	adds r1, #40
	bl UiWindow_SetTilemapEntryFar
	ldrh r2, [r6, #8]
	movs r1, #241
	mov r3, r9
	lsls r1, r1, #8
	str r3, [sp, #0]
	subs r2, #2
	adds r0, r6, #0
	adds r1, #41
	adds r3, r5, #0
	bl UiWindow_SetTilemapEntryFar
	ldrh r3, [r6, #14]
	movs r2, #2
	lsls r3, r3, #16
	asrs r3, r3, #18
	mov r0, r8
	lsls r2, r3
	ldrb r3, [r0, #3]
	orrs r2, r3
	strb r2, [r0, #3]
.L_081047aa:
	ldr r1, [sp, #56]
	adds r1, #1
	str r1, [sp, #56]
	adds r0, r1, #0
	movs r1, #60
	bl Math_Mod
	subs r0, #5
	movs r2, #200
	movs r0, #0
	movs r1, #32
	bl Func_08105300
	ldr r2, [sp, #60]
	cmp r2, #0
	beq .L_081047ce
	movs r3, #0
	str r3, [sp, #60]
.L_081047ce:
	ldr r3, [sp, #56]
	movs r0, #3
	ands r3, r0
	cmp r3, #0
	bne .L_081047fc
	ldr r1, [sp, #56]
	movs r3, #4
	ands r3, r1
	cmp r3, #0
	beq .L_081047f0
	ldr r3, .L_08104910
	ldr r0, .L_08104914
	ldr r1, .L_08104918
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	b .L_081047fc
.L_081047f0:
	ldr r3, .L_0810491c
	ldr r0, .L_08104914
	movs r1, #32
	ldr r2, .L_08104920
	mov lr, r3
	.2byte 0xf800
.L_081047fc:
	ldr r2, [sp, #40]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_0810480c
	movs r0, #113
	movs r7, #2
	b .L_0810481e
.L_0810480c:
	movs r3, #129
	ldr r0, [sp, #40]
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r0
	cmp r3, #0
	beq .L_08104826
	movs r0, #113
	movs r7, #1
.L_0810481e:
	bl Audio_PlayCue
	negs r7, r7
	b .L_08104888
.L_08104826:
	ldr r1, [sp, #36]
	movs r3, #32
	ands r3, r1
	cmp r3, #0
	beq .L_08104854
	mov r2, r11
	ldr r0, [r2]
	subs r0, #1
	str r0, [r2]
	ldr r3, [sp, #20]
	ldr r1, [r3]
	bl Func_08100e28
	mov r1, r11
	str r0, [r1]
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080138a8
	movs r2, #1
	str r2, [sp, #60]
	b .L_08104880
.L_08104854:
	ldr r0, [sp, #36]
	movs r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_08104880
	mov r1, r11
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	ldr r2, [sp, #20]
	ldr r1, [r2]
	bl Func_08100e28
	mov r3, r11
	str r0, [r3]
	movs r0, #111
	bl Audio_PlayCue
	bl Func_080138a8
	movs r0, #1
	str r0, [sp, #60]
.L_08104880:
	movs r0, #1
	bl WaitFrames
	b .L_08104690
.L_08104888:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_08104924
	bl Func_080145a8
	movs r5, #192
	lsls r5, r5, #18
	ldr r2, [r5, #60]
	movs r3, #1
	strb r3, [r2, #6]
	ldr r0, [sp, #44]
	movs r1, #1
	bl UiWindow_CloseIfOpen
	movs r0, #1
	bl WaitFrames
	movs r3, #5
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	ldr r0, [sp, #44]
	movs r3, #17
	movs r2, #0
	movs r1, #13
	bl UiWindow_UpdateOrCreate
	movs r1, #1
	ldr r0, [sp, #52]
	bl UiWindow_CloseIfOpen
	movs r1, #1
	ldr r0, [sp, #48]
	bl UiWindow_CloseIfOpen
	mov r1, r10
	ldr r0, [r1, #52]
	bl RenderOutput_RedrawSavedRectFar
	mov r2, r10
	ldr r0, [r2, #44]
	bl RenderOutput_RedrawSavedRectFar
	mov r3, r10
	ldr r0, [r3, #16]
	bl RenderOutput_RedrawSavedRectFar
	ldr r3, [r5, #60]
	movs r6, #0
	strb r6, [r3, #6]
	movs r0, #1
	bl WaitFrames
	adds r0, r7, #0
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08104904:
	.4byte 0x000010db
.L_08104908:
	.4byte gInput
.L_0810490c:
	.4byte 0xfffff000
.L_08104910:
	.4byte IwramCopyWords
.L_08104914:
	.4byte 0x060052c0
.L_08104918:
	.4byte Data_081059b4
.L_0810491c:
	.4byte IwramFillWords
.L_08104920:
	.4byte 0x44444444
.L_08104924:
	.4byte Func_08104da8
