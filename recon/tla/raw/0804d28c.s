.syntax unified
	.thumb
	.global Func_0804d28c
	.thumb_func
Func_0804d28c:
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
	ldr r6, [r3]
	movs r2, #1
	adds r5, r6, #0
	movs r3, #12
	adds r5, #140
	adds r7, r1, #0
	mov r9, r2
	mov r10, r3
	strh r0, [r5]
	cmp r7, r0
	bge .L_0804d2ba
	subs r2, #2
	mov r9, r2
.L_0804d2ba:
	mov r8, r0
	movs r3, #146
	adds r3, r3, r6
	mov r11, r3
	b .L_0804d2d6
.L_0804d2c4:
	ldrh r3, [r5]
	movs r0, #111
	add r3, r9
	strh r3, [r5]
	bl Audio_PlayCue
	movs r2, #0
	mov r10, r2
	add r8, r9
.L_0804d2d6:
	ldr r0, [r6, #120]
	bl RenderOutput_PrepareForRedraw
	mov r2, r11
	movs r3, #0
	ldrsh r0, [r2, r3]
	cmp r0, #0
	beq .L_0804d2ee
	movs r2, #0
	ldrsh r3, [r5, r2]
	adds r0, r0, r3
	b .L_0804d2fa
.L_0804d2ee:
	movs r2, #0
	ldrsh r3, [r5, r2]
	adds r3, #132
	ldrb r2, [r6, r3]
	ldr r3, .L_0804d33c
	adds r0, r2, r3
.L_0804d2fa:
	movs r3, #0
	ldr r1, [r6, #120]
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r0, .L_0804d340
	subs r3, r1, r7
	adds r2, r3, #0
	cmp r3, #0
	bge .L_0804d314
	subs r2, r7, r1
.L_0804d314:
	ldrb r0, [r0, r2]
	add r0, r10
	bl WaitFrames
	cmp r8, r7
	bne .L_0804d2c4
	movs r0, #48
	bl WaitFrames
	movs r0, #112
	bl Audio_PlayCue
	adds r0, r7, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804d33c:
	.4byte 0x0000003a
.L_0804d340:
	.4byte Data_0805f89f
