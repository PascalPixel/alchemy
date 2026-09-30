.syntax unified
	.thumb
	.global Func_080d41fc
	.thumb_func
Func_080d41fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	mov r8, r2
	adds r6, r1, #0
	ldr r5, [sp, #40]
	ldr r7, [sp, #60]
	mov r10, r3
	bl ObjectTable_ReadActiveValue
	mov r11, r0
	adds r0, r5, #0
	bl ObjectTable_ReadActiveValue
	mov r9, r0
	mov r0, r11
	bl Func_080d1eac
	movs r1, #226
	lsls r1, r1, #1
	add r1, r10
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r1]
	asrs r0, r0, #16
	mov r2, r8
	adds r1, r6, #0
	bl UiText_OpenMessageWindowFar
	movs r1, #0
	mov r8, r0
	ldr r2, [sp, #0]
	mov r0, r11
	ldr r3, [sp, #36]
	bl Func_080380f8
	cmp r7, #0
	beq .L_080d4270
	b .L_080d4268
.L_080d4262:
	adds r0, r7, #0
	bl WaitFrames
.L_080d4268:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080d4262
.L_080d4270:
	mov r0, r9
	bl Func_080d1eac
	movs r1, #226
	lsls r1, r1, #1
	add r1, r10
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	strh r2, [r1]
	lsls r0, r0, #16
	ldr r1, [sp, #44]
	ldr r2, [sp, #48]
	asrs r0, r0, #16
	bl UiText_OpenMessageWindowFar
	movs r1, #0
	adds r7, r0, #0
	ldr r2, [sp, #52]
	mov r0, r9
	ldr r3, [sp, #56]
	bl Func_080380f8
	b .L_080d42a8
.L_080d42a2:
	movs r0, #1
	bl WaitFrames
.L_080d42a8:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080d42a2
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080d432c
	movs r2, #129
	ldr r3, [r1, #4]
	lsls r2, r2, #2
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	bne .L_080d42d8
	adds r6, r1, #0
	adds r5, r2, #0
.L_080d42ca:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #4]
	ands r3, r5
	cmp r3, #0
	beq .L_080d42ca
.L_080d42d8:
	movs r0, #1
	bl WaitFrames
	mov r0, r11
	bl UiWork_FinalizeEntityMatchingLocalizedIdFar
	mov r0, r9
	bl UiWork_FinalizeEntityMatchingLocalizedIdFar
	bl Func_08038140
	movs r0, #1
	bl WaitFrames
	b .L_080d42fc
.L_080d42f6:
	movs r0, #1
	bl WaitFrames
.L_080d42fc:
	mov r0, r8
	bl UiWork_IsCompleteFar + 0x8
	cmp r0, #0
	beq .L_080d42f6
	b .L_080d430e
.L_080d4308:
	movs r0, #1
	bl WaitFrames
.L_080d430e:
	adds r0, r7, #0
	bl UiWork_IsCompleteFar + 0x8
	cmp r0, #0
	beq .L_080d4308
	movs r0, #1
	bl WaitFrames
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d432c:
	.4byte gInput
