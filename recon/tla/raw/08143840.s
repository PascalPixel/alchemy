.syntax unified
	.thumb
	.global Func_08143840
	.thumb_func
Func_08143840:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #92]
	sub sp, #4
	mov r9, r2
	ldr r2, [r3, #36]
	adds r7, r0, #0
	mov r8, r2
	ldr r2, [r3, #96]
	adds r3, #176
	str r2, [sp, #0]
	ldr r3, [r3]
	mov r10, r3
	bl Func_081434d8
	mov r2, r10
	movs r3, #1
	str r3, [r2, #12]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_08143894
	movs r2, #128
	ldr r6, .L_08143898
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r5, .L_0814389c
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_081438c2
	b .L_081438a0
.L_08143894:
	.4byte 0x00000000
.L_08143898:
	.4byte Data_020038e0
.L_0814389c:
	.4byte 0x04000208
.L_081438a0:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #184
	adds r3, r3, r6
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_081438c2:
	strh r1, [r5]
	ldr r2, .L_0814395c
	movs r3, #0
	mov r11, r3
	movs r3, #32
	strh r3, [r2, #6]
	movs r0, #1
	bl WaitFrames
	movs r3, #206
	lsls r3, r3, #3
	add r3, r8
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #128
	bl Func_08118028 + 0x10
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #180
	add r2, r9
	movs r3, #24
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #184
	add r3, r9
	mov r2, r11
	movs r1, #144
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_08143960
	bl Func_080145a8
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_08143934
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #152
	adds r3, r3, r6
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08143934:
	strh r1, [r5]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_08143958
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #12
	orrs r7, r2
	strh r7, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_08143986
	b .L_08143964
	.2byte 0x0000
.L_08143958:
	.4byte 0x00000784
.L_0814395c:
	.4byte Data_03001120
.L_08143960:
	.4byte Func_08143488
.L_08143964:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #152
	adds r3, r3, r6
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08143986:
	strh r1, [r5]
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #100
	bl Func_08118028
	mov r3, r11
	mov r2, r10
	str r3, [r2, #12]
	movs r0, #1
	bl WaitFrames
	bl Func_0814355c
	ldr r2, .L_081439bc
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #72
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_081439e2
	b .L_081439c0
.L_081439bc:
	.4byte 0x00003537
.L_081439c0:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #238
	adds r3, r3, r6
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_081439e2:
	strh r1, [r5]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_08143a7c
	movs r2, #128
	mov r8, r3
	movs r3, #128
	movs r5, #0
	lsls r2, r2, #1
	lsls r3, r3, #2
	mov r12, r5
	movs r7, #0
	mov r10, r2
	mov lr, r3
	movs r6, #0
.L_08143a02:
	mov r2, r10
	movs r4, #0
	adds r0, r6, r2
	lsls r1, r7, #1
.L_08143a0a:
	adds r3, r0, #0
	orrs r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r11, r3
	mov r3, r8
	adds r2, r5, r3
	adds r4, #1
	mov r3, r11
	strh r3, [r2]
	add r0, lr
	adds r1, #2
	adds r5, #2
	cmp r4, #8
	bne .L_08143a0a
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #5
	add r12, r3
	adds r6, r6, r2
	mov r2, r12
	adds r7, #8
	cmp r2, #16
	bne .L_08143a02
	movs r1, #128
	ldr r0, [sp, #0]
	ldr r5, .L_08143a80
	lsls r1, r1, #7
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_08143a84
	mov lr, r5
	.2byte 0xf800
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	movs r6, #0
	add r3, r9
	str r6, [r3]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	add r3, r9
	str r6, [r3]
	movs r0, #1
	bl WaitFrames
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08143a7c:
	.4byte 0x06003800
.L_08143a80:
	.4byte IwramClearWords
.L_08143a84:
	.4byte 0x06004000
