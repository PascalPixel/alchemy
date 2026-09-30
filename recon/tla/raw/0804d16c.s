.syntax unified
	.thumb
	.global Menu_RunResourceSelectionLoop
	.thumb_func
Menu_RunResourceSelectionLoop:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #232
	ldr r3, [r3]
	movs r1, #140
	mov r8, r3
	add r1, r8
	mov r10, r1
	mov r2, r10
	movs r3, #146
	strh r0, [r2]
	add r3, r8
	mov r11, r3
.L_0804d194:
	mov r1, r8
	ldr r0, [r1, #120]
	bl RenderOutput_PrepareForRedraw
	mov r3, r11
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_0804d1bc
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	adds r0, r0, r3
	b .L_0804d1cc
.L_0804d1b0:
	movs r0, #113
	bl Audio_PlayCue
	movs r0, #1
	negs r0, r0
	b .L_0804d278
.L_0804d1bc:
	mov r0, r10
	movs r2, #0
	ldrsh r3, [r0, r2]
	mov r1, r8
	adds r3, #132
	ldrb r2, [r1, r3]
	ldr r3, .L_0804d284
	adds r0, r2, r3
.L_0804d1cc:
	mov r2, r8
	ldr r1, [r2, #120]
	movs r3, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	movs r3, #142
	ldr r6, .L_0804d288
	add r3, r8
	mov r7, r10
	mov r9, r3
.L_0804d1e2:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r6, #4]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_0804d26c
	ldr r2, [r6, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	bne .L_0804d1b0
	ldr r2, [r6, #4]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	bne .L_0804d1b0
	ldr r2, [r6, #12]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	bne .L_0804d21a
	ldr r5, [r6, #12]
	movs r3, #64
	ands r5, r3
	cmp r5, #0
	beq .L_0804d238
.L_0804d21a:
	movs r0, #111
	bl Audio_PlayCue
	ldrh r3, [r7]
	subs r3, #1
	strh r3, [r7]
	lsls r3, r3, #16
	cmp r3, #0
	bge .L_0804d194
	mov r0, r9
	ldrh r3, [r0]
	mov r1, r10
	subs r3, #1
	strh r3, [r1]
	b .L_0804d194
.L_0804d238:
	ldr r2, [r6, #12]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	bne .L_0804d24c
	ldr r2, [r6, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_0804d1e2
.L_0804d24c:
	movs r0, #111
	bl Audio_PlayCue
	ldrh r3, [r7]
	mov r1, r9
	adds r3, #1
	strh r3, [r7]
	lsls r3, r3, #16
	movs r0, #0
	ldrsh r2, [r1, r0]
	asrs r3, r3, #16
	cmp r3, r2
	blt .L_0804d194
	mov r2, r10
	strh r5, [r2]
	b .L_0804d194
.L_0804d26c:
	movs r0, #112
	bl Audio_PlayCue
	mov r1, r10
	movs r3, #0
	ldrsh r0, [r1, r3]
.L_0804d278:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804d284:
	.4byte 0x0000003a
.L_0804d288:
	.4byte gInput
