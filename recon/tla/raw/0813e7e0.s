.syntax unified
	.thumb
	.global Func_0813e7e0
	.thumb_func
Func_0813e7e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #246
	lsls r1, r1, #7
	mov r11, r0
	adds r1, #124
	movs r0, #92
	sub sp, #16
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	mov r10, r0
	lsls r1, r1, #7
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #36]
	adds r3, #176
	str r1, [sp, #8]
	mov r2, r11
	ldr r6, [r3]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #240
	add r3, r10
	str r2, [r3]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #228
	add r3, r10
	movs r5, #1
	str r5, [r3]
	mov r0, r11
	bl Func_08191530
	bl Func_081434d8
	ldr r2, .L_0813e898
	movs r3, #32
	str r5, [r6, #12]
	strh r3, [r2, #6]
	ldr r1, [sp, #8]
	movs r2, #206
	lsls r2, r2, #3
	adds r3, r1, r2
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #0
	bl Func_08118038
	ldr r3, .L_0813e890
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	movs r3, #100
	movs r0, #0
	bl Func_08118028
	movs r3, #128
	lsls r3, r3, #19
	movs r5, #0
	adds r3, #40
	str r5, [r6, #12]
	movs r2, #128
	str r5, [r3]
	ldr r3, .L_0813e89c
	lsls r2, r2, #19
	adds r2, #44
	str r3, [r2]
	ldr r3, .L_0813e894
	subs r2, #12
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #34
	strh r5, [r3]
	b .L_0813e8a0
.L_0813e890:
	.4byte 0x00000784
.L_0813e894:
	.4byte 0x00000080
.L_0813e898:
	.4byte Data_03001120
.L_0813e89c:
	.4byte 0xfffff000
.L_0813e8a0:
	adds r3, #2
	strh r5, [r3]
	ldr r3, .L_0813e8dc
	adds r2, #6
	strh r3, [r2]
	ldr r1, .L_0813e8e0
	movs r3, #128
	ldr r2, .L_0813e8e4
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_0813e8e8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_0813e8ec
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_0813e8f0
	movs r1, #128
	movs r2, #128
	b .L_0813e8f4
	.2byte 0x0000
.L_0813e8dc:
	.4byte 0x00000100
.L_0813e8e0:
	.4byte 0x000000f0
.L_0813e8e4:
	.4byte 0x00001088
.L_0813e8e8:
	.4byte 0x00003537
.L_0813e8ec:
	.4byte 0x00003f21
.L_0813e8f0:
	.4byte 0x06003800
.L_0813e8f4:
	lsls r1, r1, #1
	lsls r2, r2, #2
	mov r12, r5
	mov r8, r3
	mov r9, r1
	mov lr, r2
	movs r7, #0
	movs r6, #0
.L_0813e904:
	mov r3, r9
	movs r4, #0
	adds r0, r7, r3
	lsls r1, r6, #1
.L_0813e90c:
	adds r3, r0, #0
	orrs r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #4]
	mov r3, r8
	adds r2, r5, r3
	add r3, sp, #4
	ldrh r3, [r3]
	adds r4, #1
	strh r3, [r2]
	add r0, lr
	adds r1, #2
	adds r5, #2
	cmp r4, #8
	bne .L_0813e90c
	movs r2, #1
	movs r1, #128
	add r12, r2
	lsls r1, r1, #5
	mov r3, r12
	adds r7, r7, r1
	adds r6, #8
	cmp r3, #16
	bne .L_0813e904
	movs r1, #128
	ldr r5, .L_0813e9d0
	ldr r0, [sp, #12]
	lsls r1, r1, #7
	mov lr, r5
	.2byte 0xf800
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_0813e9d4
	mov lr, r5
	.2byte 0xf800
	ldr r1, .L_0813e9d8
	ldr r0, .L_0813e9dc
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0813e986
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #238
	adds r3, r3, r1
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
.L_0813e986:
	strh r4, [r0]
	movs r2, #128
	ldr r3, .L_0813e9c8
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0813e9cc
	subs r2, #2
	strh r3, [r2]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0813e9e0
	bl Func_080145a8
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	movs r1, #19
	movs r0, #188
	b .L_0813e9e4
	.2byte 0x0000
.L_0813e9c8:
	.4byte 0x00001010
.L_0813e9cc:
	.4byte 0x00000000
.L_0813e9d0:
	.4byte IwramClearWords
.L_0813e9d4:
	.4byte 0x06004000
.L_0813e9d8:
	.4byte Data_020038e0
.L_0813e9dc:
	.4byte 0x04000208
.L_0813e9e0:
	.4byte Func_08143000
.L_0813e9e4:
	bl Func_081963ec
	movs r2, #240
	ldr r3, .L_0813eb48
	ldr r1, .L_0813eb4c
	lsls r2, r2, #7
	ldr r0, .L_0813eb50
	mov lr, r3
	.2byte 0xf800
	movs r1, #240
	ldr r3, .L_0813eb54
	lsls r1, r1, #7
	ldr r2, .L_0813eb58
	ldr r0, .L_0813eb50
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_0813eb5c
	movs r2, #238
	ldrh r3, [r1, #4]
	lsls r2, r2, #7
	adds r2, #160
	add r2, r10
	str r3, [r2]
	movs r2, #238
	ldrh r3, [r1, #6]
	lsls r2, r2, #7
	adds r2, #164
	add r2, r10
	str r3, [r2]
	ldr r1, [sp, #8]
	movs r2, #206
	lsls r2, r2, #3
	adds r3, r1, r2
	movs r2, #1
	ldrh r1, [r3]
	negs r2, r2
	movs r0, #1
	bl Func_08118040
	movs r2, #31
	negs r2, r2
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_08164b2c
	mov r1, r11
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r3, #30
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #3
	movs r3, #0
	bl Func_0814cd48
	movs r5, #0
	b .L_0813ea80
.L_0813ea56:
	cmp r5, #63
	bgt .L_0813ea6a
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r2, r2, #1
	subs r2, #31
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_08164b2c
.L_0813ea6a:
	bl Func_081434f8
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	add r3, r10
	str r6, [r3]
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0813ea80:
	movs r2, #250
	lsls r2, r2, #7
	adds r2, #96
	cmp r5, r2
	beq .L_0813eaa2
	ldr r1, .L_0813eb60
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_0813ea98
	movs r5, #0
.L_0813ea98:
	ldr r3, [r1]
	movs r6, #1
	ands r3, r6
	cmp r3, #0
	beq .L_0813ea56
.L_0813eaa2:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0813eb64
	bl Func_08014644
	mov r1, r11
	movs r3, #36
	ldrsh r0, [r1, r3]
	movs r3, #1
	negs r3, r3
	movs r2, #0
	str r2, [sp, #0]
	adds r1, r3, #0
	movs r2, #1
	bl Func_0814cd48
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #160
	add r3, r10
	ldr r2, .L_0813eb5c
	ldr r3, [r3]
	movs r1, #206
	strh r3, [r2, #4]
	movs r3, #32
	strh r3, [r2, #6]
	ldr r2, [sp, #8]
	lsls r1, r1, #3
	adds r3, r2, r1
	ldrh r1, [r3]
	movs r2, #0
	movs r0, #2
	bl Func_08118038
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0813eb68
	ldr r0, .L_0813eb6c
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0813eb26
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #234
	adds r3, r3, r1
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
.L_0813eb26:
	strh r4, [r0]
	movs r0, #100
	bl Runtime_ReleaseHeapBlock
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0813eb48:
	.4byte IwramCopyWords
.L_0813eb4c:
	.4byte 0x06008000
.L_0813eb50:
	.4byte gMapCellBuffer
.L_0813eb54:
	.4byte IwramFillWords
.L_0813eb58:
	.4byte 0x01010101
.L_0813eb5c:
	.4byte Data_03001120
.L_0813eb60:
	.4byte gInput
.L_0813eb64:
	.4byte Func_08143000
.L_0813eb68:
	.4byte Data_020038e0
.L_0813eb6c:
	.4byte 0x04000208
