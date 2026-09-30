.syntax unified
	.thumb
	.global Func_08127308
	.thumb_func
Func_08127308:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	sub sp, #216
	mov r9, r0
	movs r0, #1
	str r3, [sp, #4]
	mov r11, r1
	adds r5, r2, #0
	bl WaitFrames
	ldr r0, [sp, #4]
	movs r1, #206
	lsls r1, r1, #3
	adds r3, r0, r1
	movs r2, #0
	ldrh r1, [r3]
	movs r0, #1
	bl Func_0812628c
	movs r1, #128
	ldr r3, .L_0812755c
	lsls r1, r1, #7
	ldr r0, .L_08127560
	mov lr, r3
	.2byte 0xf800
	movs r1, #220
	movs r0, #128
	lsls r1, r1, #6
	lsls r0, r0, #19
	adds r1, #65
	bl Func_08013ba4
	movs r0, #128
	movs r1, #224
	lsls r0, r0, #19
	lsls r1, r1, #3
	adds r0, #12
	adds r1, #132
	bl Func_08013ba4
	movs r0, #128
	movs r1, #252
	lsls r0, r0, #19
	lsls r1, r1, #6
	adds r1, #68
	adds r0, #80
	bl Func_08013ba4
	movs r0, #1
	bl WaitFrames
	movs r2, #240
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #64
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #136
	adds r3, #4
	strh r2, [r3]
	movs r2, #63
	adds r3, #4
	strh r2, [r3]
	movs r2, #17
	adds r3, #2
	strh r2, [r3]
	cmp r5, #0
	bne .L_08127488
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #19
	lsls r1, r1, #5
	adds r1, #14
	adds r0, #82
	bl Func_08013ba4
	mov r0, r11
	bl Func_08127068
	movs r3, #192
	ldr r2, [sp, #4]
	lsls r3, r3, #3
	adds r3, #108
	adds r3, r2, r3
	ldr r0, .L_08127564
	str r3, [sp, #0]
	movs r1, #0
	movs r7, #0
	add r6, sp, #204
	mov r8, r0
	mov r10, r1
.L_081273d2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #92]
	cmp r7, #24
	bgt .L_081273fe
	ldr r0, [sp, #0]
	movs r2, #128
	mov r3, r10
	lsls r2, r2, #9
	subs r2, r2, r3
	str r2, [r0]
	ldr r1, [sp, #4]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #108
	adds r0, r1, r3
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #192
	movs r3, #128
	bl ColorBuffer_Scale
.L_081273fe:
	adds r1, r6, #0
	mov r0, r9
	bl Func_0811c2b4
	ldr r3, [r6]
	movs r1, #152
	movs r2, #64
	lsls r1, r1, #5
	adds r1, #196
	subs r3, r2, r3
	adds r0, r5, r1
	lsls r3, r3, #8
	str r3, [r0]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #200
	adds r2, r5, r3
	ldr r3, [r6, #4]
	movs r1, #64
	subs r3, r1, r3
	ldr r1, .L_08127568
	lsls r3, r3, #8
	str r3, [r2]
	mov r2, r8
	ldrh r3, [r2]
	adds r4, r3, #0
	mov r3, r8
	strh r3, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0812745c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	stmia r3!, {r0}
	strh r2, [r1]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #2
	str r2, [r3]
.L_0812745c:
	mov r3, r8
	strh r4, [r3]
	movs r0, #152
	lsls r0, r0, #5
	adds r0, #204
	adds r2, r5, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #68
	adds r7, #1
	add r10, r1
	cmp r7, #44
	ble .L_081273d2
	mov r0, r11
	bl Func_081272b0
	b .L_0812754e
.L_08127488:
	cmp r5, #1
	bne .L_08127508
	mov r0, r11
	bl Func_08138038
	ldr r0, .L_08127568
	movs r2, #8
	ldr r5, .L_08127564
	add r2, sp
	movs r3, #64
	mov r8, r2
	add r6, sp, #192
	mov r11, r3
	mov r10, r0
	movs r7, #39
.L_081274a6:
	adds r1, r6, #0
	mov r0, r9
	bl Func_0811c2b4
	ldr r3, [r6]
	mov r1, r11
	subs r3, r1, r3
	lsls r3, r3, #8
	str r3, [sp, #8]
	mov r2, r8
	ldr r3, [r6, #4]
	subs r3, r1, r3
	lsls r3, r3, #8
	str r3, [r2, #4]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	mov r3, r10
	ldrh r2, [r3]
	cmp r2, #31
	bgt .L_081274f4
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	add r3, r10
	mov r0, r10
	adds r3, #4
	strh r2, [r0]
	mov r2, r8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #2
	str r2, [r3]
.L_081274f4:
	strh r1, [r5]
	movs r0, #1
	subs r7, #1
	bl WaitFrames
	cmp r7, #0
	bge .L_081274a6
	bl Func_08138040
	b .L_0812754e
.L_08127508:
	cmp r5, #2
	bne .L_0812752e
	add r0, sp, #104
	movs r3, #0
	str r3, [r0, #28]
	mov r3, r11
	mov r1, r9
	str r3, [r0]
	mov r2, r9
	movs r3, #1
	str r6, [r0, #24]
	str r1, [r0, #8]
	strh r2, [r0, #36]
	str r1, [r0, #12]
	str r3, [r0, #20]
	str r3, [r0, #16]
	bl Func_08138020
	b .L_0812754e
.L_0812752e:
	add r0, sp, #16
	movs r3, #0
	str r3, [r0, #28]
	str r3, [r0, #24]
	mov r3, r9
	mov r2, r9
	mov r1, r11
	strh r3, [r0, #36]
	movs r3, #1
	str r1, [r0]
	str r2, [r0, #8]
	str r2, [r0, #12]
	str r3, [r0, #20]
	str r3, [r0, #16]
	bl Func_08138030
.L_0812754e:
	add sp, #216
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0812755c:
	.4byte IwramClearWords
.L_08127560:
	.4byte 0x06004000
.L_08127564:
	.4byte 0x04000208
.L_08127568:
	.4byte Data_020038e0
