.syntax unified
	.thumb
	.global Func_080fc6bc
	.thumb_func
Func_080fc6bc:
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
	ldr r7, [r3]
	str r1, [sp, #32]
	str r1, [sp, #20]
	add r4, sp, #32
	ldrb r4, [r4]
	movs r2, #152
	lsls r2, r2, #2
	adds r3, r7, r2
	strb r4, [r3]
	adds r5, r7, #0
	movs r3, #14
	str r3, [sp, #0]
	adds r5, #56
	movs r3, #2
	str r3, [sp, #4]
	mov r11, r0
	movs r1, #13
	adds r0, r5, #0
	movs r2, #3
	movs r3, #17
	bl UiWindow_UpdateOrCreate
	ldr r5, [r5]
	movs r1, #0
	str r5, [sp, #36]
	str r1, [sp, #24]
	movs r2, #40
	mov r3, r11
	add r2, sp
	lsls r3, r3, #1
	adds r4, r7, #2
	mov r8, r2
	str r3, [sp, #8]
	str r4, [sp, #12]
	b .L_080fc9e0
.L_080fc71a:
	movs r0, #173
	bl Audio_PlayCue
	mov r1, r8
	ldr r3, [r1, #24]
	movs r2, #226
	lsls r3, r3, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	str r3, [sp, #32]
	movs r3, #1
	str r3, [sp, #24]
	b .L_080fc9e0
.L_080fc736:
	movs r0, #113
	bl Audio_PlayCue
	movs r4, #1
	str r5, [sp, #32]
	str r4, [sp, #24]
	b .L_080fc9e0
.L_080fc744:
	mov r2, r8
	ldr r1, [r2, #16]
	movs r0, #98
	lsls r1, r1, #4
	adds r1, #36
	bl Func_080f8a44
	mov r3, r10
	cmp r3, #0
	beq .L_080fc820
	ldr r1, [sp, #20]
	mov r2, r9
	lsls r3, r1, #1
	adds r3, #216
	ldrh r3, [r2, r3]
	movs r4, #0
	mov r10, r4
	cmp r3, #0
	beq .L_080fc774
	lsls r3, r1, #2
	adds r3, #76
	ldr r0, [r7, r3]
	bl UiIcon_PrepareObject
.L_080fc774:
	ldr r3, [sp, #28]
	cmp r3, #0
	beq .L_080fc7a0
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	ldr r0, [sp, #36]
	mov r2, r8
	bl Func_080fc608
	mov r4, r11
	cmp r4, #0
	bne .L_080fc79c
	ldr r0, .L_080fc838
	ldr r1, [sp, #36]
	movs r2, #0
	movs r3, #88
	bl UiText_DrawCharacterAtOffsetFar
.L_080fc79c:
	movs r1, #0
	str r1, [sp, #28]
.L_080fc7a0:
	add r1, sp, #68
	ldr r0, [sp, #36]
	mov r2, r8
	bl Func_080fc558
	ldr r3, [sp, #8]
	movs r4, #182
	lsls r4, r4, #1
	mov r1, r8
	adds r2, r3, r4
	ldr r3, [r1, #24]
	movs r5, #226
	lsls r5, r5, #1
	lsls r3, r3, #1
	adds r3, r3, r5
	ldrh r3, [r7, r3]
	strh r3, [r7, r2]
	ldr r2, [sp, #12]
	movs r3, #133
	lsls r3, r3, #2
	add r3, r11
	ldrb r3, [r2, r3]
	ldr r1, [r1, #24]
	adds r0, r3, #0
	movs r2, #0
	bl Func_080fae8c
	mov r3, r8
	ldr r2, [r3, #24]
	lsls r3, r2, #1
	adds r3, r3, r5
	ldrh r3, [r7, r3]
	cmp r3, #0
	beq .L_080fc7f6
	lsls r3, r2, #2
	adds r3, #76
	ldr r0, [r7, r3]
	movs r3, #9
	movs r2, #0
	strb r3, [r0, #5]
	movs r3, #250
	strh r2, [r0, #12]
	strb r3, [r0, #15]
.L_080fc7f6:
	movs r4, #139
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r7, r4
	ldrb r3, [r3]
	movs r5, #0
	cmp r5, r3
	bcs .L_080fc820
	adds r6, r7, r4
.L_080fc808:
	lsls r3, r5, #2
	adds r3, #248
	ldr r0, [r7, r3]
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	ldrb r3, [r6]
	cmp r5, r3
	bcc .L_080fc808
.L_080fc820:
	ldr r3, .L_080fc83c
	ldr r2, [r3]
	movs r3, #31
	ands r2, r3
	cmp r2, #0
	bne .L_080fc880
	ldr r6, .L_080fc834
	movs r5, #0
	b .L_080fc840
	.2byte 0x0000
.L_080fc834:
	.4byte 0x000001ff
.L_080fc838:
	.4byte 0x000010ba
.L_080fc83c:
	.4byte gFrameCount
.L_080fc840:
	movs r1, #172
	lsls r3, r5, #1
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrsh r0, [r7, r3]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_080fc876
	mov r4, r8
	ldr r3, [r4, #24]
	adds r1, #108
	lsls r3, r3, #1
	adds r3, r3, r1
	ldrh r3, [r7, r3]
	adds r1, r6, #0
	ands r1, r3
	bl Item_CanOwnerEquip
	cmp r0, #0
	beq .L_080fc876
	lsls r3, r5, #2
	adds r3, #248
	ldr r0, [r7, r3]
	movs r1, #3
	bl Animation_ApplyChildArgumentFar
.L_080fc876:
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #3
	bls .L_080fc840
.L_080fc880:
	movs r0, #1
	bl WaitFrames
	mov r2, r8
	ldr r2, [r2, #24]
	mov r3, r8
	str r2, [sp, #20]
	add r2, sp, #48
	ldr r1, [r3, #20]
	movs r0, #0
	str r2, [sp, #0]
	add r3, sp, #56
	movs r2, #5
	bl Func_080f8f9c
	cmp r0, #1
	bne .L_080fc8a8
	movs r4, #1
	str r4, [sp, #28]
	mov r10, r4
.L_080fc8a8:
	cmp r0, #0
	bne .L_080fc8b0
	movs r1, #1
	mov r10, r1
.L_080fc8b0:
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .L_080fc8bc
	movs r2, #0
	mov r10, r2
.L_080fc8bc:
	ldr r1, .L_080fcab4
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_080fc8dc
	mov r4, r8
	ldr r3, [r4, #24]
	movs r2, #226
	lsls r3, r3, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r7, r3]
	cmp r3, #0
	beq .L_080fc8dc
	b .L_080fc71a
.L_080fc8dc:
	ldr r2, [r1, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080fc8e8
	b .L_080fc736
.L_080fc8e8:
	ldr r2, [r1, #12]
	adds r3, #254
	ands r2, r3
	cmp r2, #0
	bne .L_080fc8fe
	ldr r2, [r1, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080fc9d2
.L_080fc8fe:
	mov r3, r11
	cmp r3, #1
	bne .L_080fc912
	movs r0, #114
	bl Audio_PlayCue
	movs r0, #1
	bl WaitFrames
	b .L_080fc9d2
.L_080fc912:
	movs r0, #111
	bl Audio_PlayCue
	movs r1, #133
	ldr r2, [sp, #12]
	mov r4, r11
	lsls r1, r1, #2
	adds r0, r4, r1
	ldrb r3, [r2, r0]
	movs r4, #153
	lsls r4, r4, #2
	adds r3, r3, r4
	mov r4, r8
	ldr r2, [r4, #24]
	movs r4, #139
	strb r2, [r7, r3]
	mov r2, r11
	adds r2, #28
	str r2, [sp, #16]
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r7, r4
	ldrb r3, [r3]
	ldrsb r5, [r7, r2]
	ldr r6, [sp, #12]
	mov r9, r3
	mov r10, r0
.L_080fc948:
	ldr r3, .L_080fcab4
	movs r2, #128
	ldr r3, [r3, #12]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080fc95a
	adds r5, #1
	b .L_080fc95c
.L_080fc95a:
	subs r5, #1
.L_080fc95c:
	mov r1, r9
	adds r0, r5, r1
	bl __modsi3
	movs r3, #129
	adds r5, r0, #0
	lsls r3, r3, #2
	lsls r2, r5, #1
	adds r2, r2, r3
	ldrh r3, [r7, r2]
	mov r4, r10
	str r3, [r7, #8]
	ldrh r3, [r7, r2]
	strb r3, [r6, r4]
	ldr r1, [sp, #16]
	strb r5, [r7, r1]
	ldrb r0, [r6, r4]
	bl Owner_GetState
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	movs r2, #0
	bl ItemMenu_Collect
	movs r3, #133
	lsls r3, r3, #2
	strb r0, [r7, r3]
	lsls r0, r0, #24
	cmp r0, #0
	beq .L_080fc948
	adds r0, r5, #0
	cmp r5, #0
	bge .L_080fc9a2
	adds r0, r5, #3
.L_080fc9a2:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	ldr r0, [r7, #16]
	adds r1, r5, #0
	mov r2, r9
	bl Func_08104d5c
	movs r4, #188
	lsls r4, r4, #1
	adds r3, r7, r4
	ldr r3, [r3]
	movs r1, #190
	movs r2, #13
	lsls r1, r1, #1
	strb r2, [r3, #5]
	adds r3, r7, r1
	ldr r3, [r3]
	movs r0, #1
	strb r2, [r3, #5]
	bl WaitFrames
	b .L_080fc9e0
.L_080fc9d2:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fc9e0
	b .L_080fc744
.L_080fc9e0:
	ldr r2, [sp, #24]
	cmp r2, #0
	bne .L_080fca46
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fca46
	ldr r1, [sp, #12]
	movs r6, #133
	mov r4, r11
	lsls r6, r6, #2
	adds r3, r4, r6
	ldrb r0, [r1, r3]
	bl Owner_GetState
	movs r2, #226
	lsls r2, r2, #1
	adds r5, r7, r2
	adds r1, r5, #0
	movs r2, #0
	mov r9, r0
	bl ItemMenu_Collect
	movs r1, #0
	strb r0, [r7, r6]
	adds r0, r5, #0
	bl ItemMenu_DrawIcons
	movs r4, #134
	lsls r4, r4, #2
	adds r3, r7, r4
	ldr r2, [r3]
	movs r3, #13
	strb r3, [r2, #5]
	mov r1, r11
	mov r0, r8
	bl Func_080fc4c0
	mov r2, r8
	ldr r1, [r2, #24]
	movs r0, #98
	lsls r1, r1, #4
	adds r1, #36
	bl Func_080f8a44
	movs r3, #1
	mov r10, r3
	str r3, [sp, #28]
	b .L_080fc9d2
.L_080fca46:
	movs r3, #96
	str r3, [sp, #0]
	ldr r0, [sp, #36]
	movs r1, #0
	movs r2, #88
	movs r3, #120
	bl UiWindow_ClearInteriorTilesFar
	ldr r0, [r7, #72]
	bl UiIcon_PrepareObject
	ldr r4, [sp, #8]
	movs r1, #180
	lsls r1, r1, #1
	adds r2, r4, r1
	mov r4, r8
	ldr r3, [r4, #24]
	movs r0, #168
	strh r3, [r7, r2]
	ldr r1, [sp, #12]
	movs r3, #133
	lsls r3, r3, #2
	add r3, r11
	ldrb r3, [r1, r3]
	movs r2, #153
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r4, #24]
	movs r1, #182
	strb r2, [r7, r3]
	ldr r4, [sp, #8]
	add r2, sp, #32
	ldrh r2, [r2]
	lsls r1, r1, #1
	adds r3, r4, r1
	strh r2, [r7, r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080fca9e
	movs r3, #1
	negs r3, r3
	str r3, [sp, #32]
.L_080fca9e:
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #32]
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fcab4:
	.4byte gInput
