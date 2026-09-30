.syntax unified
	.thumb
	.global Func_080fd2e8
	.thumb_func
Func_080fd2e8:
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
	sub sp, #4
	movs r3, #29
	ldrsb r3, [r6, r3]
	movs r1, #139
	str r3, [sp, #0]
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r6, r1
	ldrb r3, [r3]
	subs r1, #17
	mov r8, r3
	movs r3, #28
	ldrsb r3, [r6, r3]
	movs r2, #0
	lsls r3, r3, #1
	adds r3, r3, r1
	mov r11, r0
	ldrh r0, [r6, r3]
	mov r9, r2
	mov r10, r2
	bl Owner_GetState
	ldr r3, [sp, #0]
	movs r1, #16
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #3
	subs r0, #10
	movs r7, #2
	bl Func_080f8ab4
	b .L_080fd49a
.L_080fd33e:
	cmp r7, #0
	bne .L_080fd344
	b .L_080fd430
.L_080fd344:
	ldr r3, [sp, #0]
	movs r2, #129
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r0, [r6, r3]
	bl Owner_GetState
	ldr r4, [sp, #0]
	ldr r5, [r6, #24]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_080fd360
	adds r3, r4, #3
.L_080fd360:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r4, r3
	lsls r1, r3, #1
	adds r1, r1, r3
	ldr r3, [r6, #16]
	ldrh r2, [r3, #12]
	ldr r3, .L_080fd390
	adds r2, r2, r1
	lsls r2, r2, #3
	subs r2, #2
	strh r2, [r5, #6]
	ands r2, r3
	ldr r3, .L_080fd394
	ldr r1, .L_080fd398
	ands r2, r3
	ldrh r3, [r5, #22]
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #22]
	mov r3, r11
	cmp r3, #0
	bne .L_080fd430
	b .L_080fd39c
.L_080fd390:
	.4byte 0x0000ffff
.L_080fd394:
	.4byte 0x000001ff
.L_080fd398:
	.4byte 0xfffffe00
.L_080fd39c:
	movs r1, #129
	lsls r3, r4, #1
	lsls r1, r1, #2
	adds r3, r3, r1
	ldrh r1, [r6, r3]
	ldr r0, [r6, #40]
	movs r2, #0
	movs r3, #0
	bl Func_080f8170
	ldr r3, [sp, #0]
	movs r2, #129
	lsls r3, r3, #1
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r1, [r6, r3]
	adds r0, r6, #0
	bl Func_080f88c4
	cmp r7, #2
	bne .L_080fd3dc
	ldr r0, [sp, #0]
	cmp r0, #0
	bge .L_080fd3ce
	adds r0, #3
.L_080fd3ce:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	movs r0, #1
	bl WaitFrames
.L_080fd3dc:
	ldr r0, [r6, #16]
	ldr r1, [sp, #0]
	mov r2, r8
	bl Func_08104d5c
	movs r0, #82
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fd428
	mov r3, r10
	cmp r3, #0
	bne .L_080fd428
	ldr r0, [r6, #48]
	bl RenderOutput_RedrawSavedRectFar
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrh r3, [r3]
	ldr r0, .L_080fd420
	movs r2, #0
	ands r0, r3
	ldr r3, .L_080fd424
	ldr r1, [r6, #48]
	adds r0, r0, r3
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r2, #1
	mov r10, r2
	b .L_080fd430
	.2byte 0x0000
.L_080fd420:
	.4byte 0x00003fff
.L_080fd424:
	.4byte 0x00000885
.L_080fd428:
	movs r0, #82
	adds r0, #255
	bl GameFlag_ClearBit
.L_080fd430:
	ldr r4, [sp, #0]
	adds r3, r4, #0
	cmp r4, #0
	bge .L_080fd43a
	adds r3, r4, #3
.L_080fd43a:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r4, r3
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #3
	movs r1, #16
	subs r0, #10
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080fd528
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080fd476
	movs r0, #112
	bl Audio_PlayCue
	ldr r3, [sp, #0]
	movs r1, #129
	lsls r3, r3, #1
	lsls r1, r1, #2
	adds r3, r3, r1
	ldrh r3, [r6, r3]
	mov r9, r3
	b .L_080fd4a8
.L_080fd476:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fd48e
	movs r0, #113
	bl Audio_PlayCue
	movs r2, #1
	negs r2, r2
	mov r9, r2
	b .L_080fd4a8
.L_080fd48e:
	mov r0, sp
	mov r1, r8
	movs r2, #4
	bl Func_08104c00
	adds r7, r0, #0
.L_080fd49a:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fd4a8
	b .L_080fd33e
.L_080fd4a8:
	ldr r5, [r6, #24]
	movs r7, #13
	adds r0, r5, #0
	bl UiIcon_PrepareObject
	strb r7, [r5, #5]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #0]
	movs r2, #129
	strb r3, [r6, #29]
	ldr r3, [sp, #0]
	lsls r2, r2, #2
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r3, [r6, r3]
	movs r1, #140
	str r3, [r6, #8]
	ldr r3, [sp, #0]
	lsls r1, r1, #1
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrh r2, [r6, r3]
	adds r1, #255
	adds r3, r6, r1
	strb r2, [r3]
	movs r3, #28
	ldrsb r3, [r6, r3]
	str r3, [sp, #0]
	movs r2, #30
	ldrsb r2, [r6, r2]
	mov r8, r2
	cmp r3, #0
	bge .L_080fd4f0
	adds r3, #3
.L_080fd4f0:
	asrs r0, r3, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	ldr r0, [r6, #16]
	ldr r1, [sp, #0]
	mov r2, r8
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
	mov r0, r9
	strb r7, [r3, #5]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fd528:
	.4byte gInput
