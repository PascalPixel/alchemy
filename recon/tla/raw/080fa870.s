.syntax unified
	.thumb
	.global Func_080fa870
	.thumb_func
Func_080fa870:
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
	ldr r6, [r3]
	sub sp, #16
	movs r3, #29
	ldrsb r3, [r6, r3]
	movs r2, #139
	str r3, [sp, #12]
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r6, r2
	ldrb r3, [r3]
	ldr r1, [r6, #36]
	mov r9, r3
	movs r3, #0
	str r3, [sp, #8]
	str r3, [sp, #4]
	mov r8, r1
	movs r3, #12
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #13
	movs r3, #17
	mov r10, r0
	mov r0, r8
	bl UiWindow_SetBounds
	ldr r0, [r6, #36]
	bl RenderOutput_RedrawSavedRectFar
	movs r3, #28
	ldrsb r3, [r6, r3]
	movs r1, #129
	lsls r1, r1, #2
	lsls r3, r3, #1
	adds r3, r3, r1
	ldrh r0, [r6, r3]
	bl Owner_GetState
	movs r1, #144
	ldr r0, .L_080fa8e4
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	movs r5, #2
	mov r11, r2
	b .L_080fab36
.L_080fa8e4:
	.4byte Func_080fabe0
.L_080fa8e8:
	lsls r3, r5, #24
	asrs r7, r3, #24
	cmp r7, #0
	bne .L_080fa8f2
	b .L_080faab4
.L_080fa8f2:
	ldr r3, [r6, #36]
	movs r1, #129
	mov r8, r3
	ldr r3, [sp, #12]
	lsls r1, r1, #2
	lsls r3, r3, #1
	adds r3, r3, r1
	ldrh r0, [r6, r3]
	bl Owner_GetState
	ldr r3, [r6, #16]
	ldr r4, [sp, #12]
	ldrh r2, [r3, #12]
	lsls r1, r4, #1
	adds r1, r1, r4
	ldr r5, [r6, #24]
	adds r2, r2, r1
	ldr r3, .L_080fa93c
	lsls r2, r2, #3
	subs r2, #2
	strh r2, [r5, #6]
	ands r2, r3
	mov r3, r11
	ands r2, r3
	ldr r1, .L_080fa940
	ldrh r3, [r5, #22]
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #22]
	cmp r7, #2
	bne .L_080fa952
	adds r0, r4, #0
	cmp r4, #0
	bge .L_080fa944
	adds r0, r4, #3
	b .L_080fa944
	.2byte 0x0000
.L_080fa93c:
	.4byte 0x0000ffff
.L_080fa940:
	.4byte 0xfffffe00
.L_080fa944:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	movs r0, #1
	bl WaitFrames
.L_080fa952:
	ldr r1, [sp, #12]
	ldr r0, [r6, #16]
	mov r2, r9
	bl Func_08104d5c
	mov r1, r10
	cmp r1, #1
	bne .L_080faa20
	ldr r3, [sp, #12]
	movs r2, #129
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r0, [r6, r3]
	movs r1, #1
	bl Func_080fae2c
	movs r3, #9
	str r3, [sp, #0]
	mov r0, r8
	movs r1, #0
	movs r2, #9
	movs r3, #16
	bl UiWindow_DrawDividerLineFar
	movs r3, #80
	str r3, [sp, #0]
	movs r2, #72
	movs r3, #120
	mov r0, r8
	movs r1, #0
	bl UiWindow_ClearInteriorTilesFar
	movs r3, #28
	ldrsb r3, [r6, r3]
	ldr r2, [sp, #12]
	cmp r2, r3
	beq .L_080faa00
	movs r1, #129
	lsls r3, r2, #1
	lsls r1, r1, #2
	movs r2, #182
	adds r3, r3, r1
	lsls r2, r2, #1
	ldrh r0, [r6, r3]
	adds r3, r6, r2
	ldrh r3, [r3]
	mov r1, r11
	ands r1, r3
	bl Func_080fad48
	ldr r3, [sp, #12]
	movs r1, #129
	lsls r3, r3, #1
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r5, r0, #0
	ldrh r0, [r6, r3]
	bl Func_080fad1c
	cmp r0, #15
	bne .L_080fa9d8
	cmp r5, #0
	bne .L_080fa9dc
	movs r2, #0
	ldr r0, .L_080fabcc
	b .L_080fa9f6
.L_080fa9d8:
	cmp r5, #0
	beq .L_080fa9f2
.L_080fa9dc:
	movs r3, #72
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #2
	mov r2, r8
	movs r3, #8
	bl UiText_DrawNumberInWindowFar
	movs r2, #32
	ldr r0, .L_080fabd0
	b .L_080fa9f6
.L_080fa9f2:
	movs r2, #16
	ldr r0, .L_080fabd4
.L_080fa9f6:
	mov r1, r8
	movs r3, #72
	bl UiText_DrawCharacterAtOffsetFar
	ldr r2, [sp, #12]
.L_080faa00:
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #22
	adds r3, r6, r1
	subs r1, #174
	ldrb r0, [r3]
	adds r3, r6, r1
	ldrh r1, [r3]
	lsls r3, r2, #1
	movs r2, #129
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r3, [r6, r3]
	movs r2, #0
	bl Func_080fae8c
.L_080faa20:
	mov r3, r10
	cmp r3, #0
	bne .L_080faab2
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrh r3, [r3]
	mov r0, r11
	ands r0, r3
	bl ItemMenu_IsSpecial
	cmp r0, #0
	beq .L_080faa56
	ldr r3, [sp, #12]
	movs r2, #129
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	subs r2, #156
	ldrh r1, [r6, r3]
	adds r3, r6, r2
	ldrh r2, [r3]
	ldr r0, [r6, #40]
	movs r3, #8
	bl Func_080f8170
	b .L_080faa72
.L_080faa56:
	ldr r3, [sp, #12]
	movs r1, #129
	lsls r1, r1, #2
	lsls r3, r3, #1
	movs r2, #180
	adds r3, r3, r1
	lsls r2, r2, #1
	ldrh r1, [r6, r3]
	adds r3, r6, r2
	ldrh r2, [r3]
	ldr r0, [r6, #40]
	movs r3, #0
	bl Func_080f8170
.L_080faa72:
	movs r0, #82
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080faaaa
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_080faaaa
	ldr r0, [r6, #48]
	bl RenderOutput_RedrawSavedRectFar
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrh r3, [r3]
	mov r0, r11
	ands r0, r3
	ldr r3, .L_080fabd8
	movs r2, #0
	adds r0, r0, r3
	ldr r1, [r6, #48]
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #1
	str r2, [sp, #4]
	b .L_080faab2
.L_080faaaa:
	movs r0, #82
	adds r0, #255
	bl GameFlag_ClearBit
.L_080faab2:
	movs r5, #0
.L_080faab4:
	ldr r2, [sp, #12]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080faabe
	adds r3, r2, #3
.L_080faabe:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #3
	movs r1, #16
	subs r0, #10
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080fabdc
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080fab12
	mov r3, r10
	cmp r3, #1
	bne .L_080faafc
	movs r2, #28
	ldrsb r2, [r6, r2]
	ldr r3, [sp, #12]
	cmp r3, r2
	bne .L_080faafc
	movs r0, #114
	bl Audio_PlayCue
	b .L_080fab36
.L_080faafc:
	movs r0, #112
	bl Audio_PlayCue
	ldr r3, [sp, #12]
	movs r1, #129
	lsls r3, r3, #1
	lsls r1, r1, #2
	adds r3, r3, r1
	ldrb r3, [r6, r3]
	str r3, [sp, #8]
	b .L_080fab44
.L_080fab12:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fab28
	movs r0, #113
	bl Audio_PlayCue
	movs r2, #255
	str r2, [sp, #8]
	b .L_080fab44
.L_080fab28:
	add r0, sp, #12
	mov r1, r9
	movs r2, #4
	bl Func_08104c00
	lsls r0, r0, #24
	lsrs r5, r0, #24
.L_080fab36:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fab44
	b .L_080fa8e8
.L_080fab44:
	ldr r5, [r6, #24]
	movs r7, #13
	adds r0, r5, #0
	bl UiIcon_PrepareObject
	strb r7, [r5, #5]
	bl Func_080fac58
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #12]
	movs r2, #129
	strb r3, [r6, #29]
	ldr r3, [sp, #12]
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r3, [r6, r3]
	movs r1, #140
	str r3, [r6, #8]
	ldr r3, [sp, #12]
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r6, r3]
	adds r1, #255
	adds r3, r6, r1
	strb r2, [r3]
	movs r3, #28
	ldrsb r3, [r6, r3]
	str r3, [sp, #12]
	movs r2, #30
	ldrsb r2, [r6, r2]
	mov r9, r2
	cmp r3, #0
	bge .L_080fab90
	adds r3, #3
.L_080fab90:
	asrs r0, r3, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	ldr r0, [r6, #16]
	ldr r1, [sp, #12]
	mov r2, r9
	bl Func_08104d5c
	movs r1, #188
	lsls r1, r1, #1
	adds r3, r6, r1
	ldr r3, [r3]
	movs r2, #190
	lsls r2, r2, #1
	strb r7, [r3, #5]
	adds r3, r6, r2
	ldr r3, [r3]
	strb r7, [r3, #5]
	ldr r3, [sp, #8]
	add sp, #16
	lsls r0, r3, #24
	asrs r0, r0, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fabcc:
	.4byte 0x0000105f
.L_080fabd0:
	.4byte 0x0000105e
.L_080fabd4:
	.4byte 0x00001060
.L_080fabd8:
	.4byte 0x00000092
.L_080fabdc:
	.4byte gInput
