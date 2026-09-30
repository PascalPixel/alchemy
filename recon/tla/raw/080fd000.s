.syntax unified
	.thumb
	.global Func_080fd000
	.thumb_func
Func_080fd000:
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
	sub sp, #24
	movs r3, #28
	ldrsb r3, [r7, r3]
	movs r2, #0
	str r3, [sp, #20]
	mov r8, r0
	movs r1, #30
	ldrsb r1, [r7, r1]
	str r2, [sp, #12]
	str r1, [sp, #16]
	str r2, [sp, #8]
	add r2, sp, #12
	ldrb r2, [r2]
	movs r1, #155
	lsls r1, r1, #2
	adds r3, r7, r1
	strb r2, [r3]
	ldr r3, [sp, #20]
	adds r5, r7, #0
	lsls r3, r3, #1
	ldrh r0, [r3, r0]
	bl Owner_GetState
	adds r5, #36
	movs r3, #10
	movs r6, #2
	mov r11, r0
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #13
	movs r2, #3
	movs r3, #17
	str r6, [sp, #4]
	bl UiWindow_UpdateOrCreate
	cmp r0, #0
	beq .L_080fd068
	ldr r1, [r5]
	adds r0, r7, #0
	bl Func_080fa3d4
.L_080fd068:
	adds r5, r7, #0
	movs r3, #4
	adds r5, #44
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #13
	movs r2, #13
	movs r3, #17
	str r6, [sp, #4]
	bl UiWindow_UpdateOrCreate
	cmp r0, #0
	beq .L_080fd09e
	ldr r3, [sp, #12]
	ldr r2, [r5]
	movs r1, #0
	str r3, [sp, #0]
	movs r0, #2
	movs r3, #0
	bl RenderOutput_CreateFar + 0x8
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r7, r1
	str r0, [r3]
	movs r3, #13
	strb r3, [r0, #5]
.L_080fd09e:
	movs r2, #155
	ldr r3, .L_080fd290
	lsls r2, r2, #2
	adds r2, r2, r7
	mov r10, r2
	mov r9, r3
	b .L_080fd240
.L_080fd0ac:
	cmp r6, #0
	beq .L_080fd150
	ldr r3, [sp, #20]
	mov r1, r8
	lsls r3, r3, #1
	ldrh r0, [r3, r1]
	ldr r5, [r7, #40]
	bl Owner_GetState
	ldr r3, [sp, #20]
	mov r2, r8
	lsls r3, r3, #1
	mov r11, r0
	ldrh r0, [r3, r2]
	bl Func_080fd294
	ldr r3, [sp, #20]
	mov r2, r8
	lsls r3, r3, #1
	ldrh r1, [r3, r2]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_080f8170
	ldr r3, [sp, #20]
	mov r2, r8
	lsls r3, r3, #1
	ldrh r1, [r3, r2]
	ldr r0, [r7, #44]
	bl Func_080fd55c
	ldr r3, [sp, #20]
	mov r2, r8
	lsls r3, r3, #1
	ldrh r1, [r3, r2]
	adds r0, r7, #0
	bl Func_080f88c4
	cmp r6, #2
	bne .L_080fd114
	ldr r0, [sp, #20]
	cmp r0, #0
	bge .L_080fd106
	adds r0, #3
.L_080fd106:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_08104ef8
	movs r0, #1
	bl WaitFrames
.L_080fd114:
	ldr r0, [r7, #16]
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	bl Func_08104d5c
	movs r0, #82
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fd148
	ldr r3, [sp, #8]
	cmp r3, #0
	bne .L_080fd148
	ldr r0, [r7, #48]
	bl RenderOutput_ClearListFar
	ldr r0, [r7, #48]
	bl RenderOutput_RedrawSavedRectFar
	ldr r0, [r7, #48]
	bl UiText_DrawWorkValueWithLabel
	movs r1, #1
	str r1, [sp, #8]
	b .L_080fd150
.L_080fd148:
	movs r0, #82
	adds r0, #255
	bl GameFlag_ClearBit
.L_080fd150:
	ldr r2, [sp, #20]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080fd15a
	adds r3, r2, #3
.L_080fd15a:
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
	mov r1, r9
	ldr r3, [r1, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080fd1a6
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080fd19e
	movs r0, #112
	bl Audio_PlayCue
	ldr r2, [sp, #20]
	mov r1, r8
	lsls r3, r2, #1
	ldrh r3, [r3, r1]
	str r3, [sp, #12]
	b .L_080fd250
.L_080fd19e:
	movs r0, #114
	bl Audio_PlayCue
	ldr r1, .L_080fd290
.L_080fd1a6:
	mov r2, r9
	ldr r3, [r2, #4]
	movs r0, #128
	lsls r0, r0, #2
	ands r3, r0
	cmp r3, #0
	bne .L_080fd1c0
	ldr r3, [r1, #4]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080fd21a
.L_080fd1c0:
	ldr r3, [sp, #20]
	mov r2, r8
	lsls r3, r3, #1
	ldrh r3, [r3, r2]
	str r3, [sp, #12]
	ldr r3, [r1, #4]
	ands r3, r0
	cmp r3, #0
	beq .L_080fd1da
	movs r3, #1
	mov r1, r10
	strb r3, [r1]
	b .L_080fd1e0
.L_080fd1da:
	movs r3, #2
	mov r2, r10
	strb r3, [r2]
.L_080fd1e0:
	movs r0, #64
	bl Runtime_BumpAllocate
	adds r6, r0, #0
	adds r1, r6, #0
	movs r2, #1
	mov r0, r11
	bl Func_080fd6f0
	adds r5, r0, #0
	lsls r5, r5, #24
	lsrs r5, r5, #24
	lsls r5, r5, #24
	adds r0, r6, #0
	asrs r5, r5, #24
	bl Sys_Free
	cmp r5, #0
	bne .L_080fd212
	mov r3, r10
	strb r5, [r3]
	movs r0, #114
	bl Audio_PlayCue
	b .L_080fd21a
.L_080fd212:
	movs r0, #112
	bl Audio_PlayCue
	b .L_080fd24e
.L_080fd21a:
	mov r1, r9
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fd234
	movs r0, #113
	bl Audio_PlayCue
	movs r2, #1
	negs r2, r2
	str r2, [sp, #12]
	b .L_080fd24e
.L_080fd234:
	add r0, sp, #20
	ldr r1, [sp, #16]
	movs r2, #4
	bl Func_08104c00
	adds r6, r0, #0
.L_080fd240:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080fd24e
	b .L_080fd0ac
.L_080fd24e:
	ldr r2, [sp, #20]
.L_080fd250:
	strb r2, [r7, #28]
	ldr r3, [sp, #20]
	mov r1, r8
	lsls r3, r3, #1
	ldrh r3, [r3, r1]
	str r3, [r7, #8]
	ldr r3, [sp, #20]
	lsls r3, r3, #1
	ldrh r2, [r3, r1]
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #22
	adds r3, r7, r1
	strb r2, [r3]
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r3, [r3]
	movs r2, #13
	subs r1, #154
	strb r2, [r3, #5]
	adds r3, r7, r1
	ldr r3, [r3]
	strb r2, [r3, #5]
	ldr r0, [sp, #12]
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fd290:
	.4byte gInput
