.syntax unified
	.thumb
	.global Func_0813fa88
	.thumb_func
Func_0813fa88:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	movs r1, #16
	movs r0, #92
	sub sp, #8
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	lsls r1, r1, #7
	mov r9, r0
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #36]
	adds r3, #176
	ldr r3, [r3]
	mov r8, r3
	bl Func_0813ba50
	mov r2, r8
	movs r3, #1
	str r3, [r2, #12]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0813fae8
	movs r2, #128
	ldr r6, .L_0813faec
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r5, .L_0813faf0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0813fb16
	b .L_0813faf4
.L_0813fae8:
	.4byte 0x00000000
.L_0813faec:
	.4byte Data_020038e0
.L_0813faf0:
	.4byte 0x04000208
.L_0813faf4:
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
.L_0813fb16:
	strh r1, [r5]
	ldr r2, .L_0813fb88
	movs r3, #32
	strh r3, [r2, #6]
	movs r0, #1
	bl WaitFrames
	movs r2, #206
	lsls r2, r2, #3
	adds r3, r7, r2
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #0
	bl Func_08118038
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0813fb62
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #230
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
.L_0813fb62:
	strh r1, [r5]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_0813fb84
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #12
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0813fbae
	b .L_0813fb8c
	.2byte 0x0000
.L_0813fb84:
	.4byte 0x00000784
.L_0813fb88:
	.4byte Data_03001120
.L_0813fb8c:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	movs r2, #230
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
.L_0813fbae:
	strh r1, [r5]
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #100
	bl Func_08118028
	mov r2, r8
	movs r3, #0
	str r3, [r2, #12]
	movs r0, #1
	bl WaitFrames
	bl Func_0814355c
	ldr r3, .L_0813fbec
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0813fbf0
	adds r2, #40
	strh r3, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0813fc16
	b .L_0813fbf4
	.2byte 0x0000
.L_0813fbec:
	.4byte 0x00000100
.L_0813fbf0:
	.4byte 0x00003537
.L_0813fbf4:
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
.L_0813fc16:
	strh r1, [r5]
	movs r5, #0
	ldr r3, .L_0813fd0c
	movs r2, #128
	mov r8, r3
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #2
	mov r12, r5
	mov r10, r2
	mov lr, r3
	movs r7, #0
	movs r6, #0
.L_0813fc30:
	mov r2, r10
	movs r4, #0
	adds r0, r7, r2
	lsls r1, r6, #1
.L_0813fc38:
	adds r3, r0, #0
	orrs r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #0]
	mov r3, r8
	adds r2, r5, r3
	mov r3, sp
	ldrh r3, [r3]
	adds r4, #1
	strh r3, [r2]
	add r0, lr
	adds r1, #2
	adds r5, #2
	cmp r4, #8
	bne .L_0813fc38
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #5
	add r12, r3
	adds r7, r7, r2
	mov r2, r12
	adds r6, #8
	cmp r2, #16
	bne .L_0813fc30
	movs r1, #128
	ldr r0, [sp, #4]
	ldr r5, .L_0813fd10
	lsls r1, r1, #7
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_0813fd14
	mov lr, r5
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	mov r3, r11
	cmp r3, #4
	bhi .L_0813fcb8
	ldr r2, .L_0813fd18
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0813fc94:
	.4byte .L_0813fca8
	.4byte .L_0813fcac
	.4byte .L_0813fcb0
	.4byte .L_0813fcb4
	.4byte .L_0813fcb8
.L_0813fca8:
	ldr r0, .L_0813fd1c
	b .L_0813fcba
.L_0813fcac:
	ldr r0, .L_0813fd20
	b .L_0813fcba
.L_0813fcb0:
	ldr r0, .L_0813fd24
	b .L_0813fcba
.L_0813fcb4:
	ldr r0, .L_0813fd28
	b .L_0813fcba
.L_0813fcb8:
	ldr r0, .L_0813fd2c
.L_0813fcba:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0813fd30
	movs r2, #128
	lsls r0, r0, #19
	movs r6, #144
	mov lr, r3
	.2byte 0xf800
	movs r5, #0
	mov r2, r9
	movs r3, #24
	lsls r6, r6, #3
	str r3, [r2, #8]
	str r5, [r2, #12]
	adds r1, r6, #0
	ldr r0, .L_0813fd34
	bl Scheduler_AddOrUpdateCallback
	mov r3, r9
	movs r1, #200
	str r5, [r3, #4]
	lsls r1, r1, #4
	ldr r0, .L_0813fd38
	bl Scheduler_AddOrUpdateCallback
	mov r2, r9
	str r5, [r2]
	adds r1, r6, #0
	ldr r0, .L_0813fd3c
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0813fd0c:
	.4byte 0x06003800
.L_0813fd10:
	.4byte IwramClearWords
.L_0813fd14:
	.4byte 0x06004000
.L_0813fd18:
	.4byte .L_0813fc94
.L_0813fd1c:
	.4byte 0x0000018c
.L_0813fd20:
	.4byte 0x00000193
.L_0813fd24:
	.4byte 0x00000178
.L_0813fd28:
	.4byte 0x0000018f
.L_0813fd2c:
	.4byte 0x00000182
.L_0813fd30:
	.4byte IwramCopyWords
.L_0813fd34:
	.4byte Func_0813fa44
.L_0813fd38:
	.4byte Func_0813f89c
.L_0813fd3c:
	.4byte Func_0813f8d4
