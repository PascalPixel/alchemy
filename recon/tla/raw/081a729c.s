.syntax unified
	.thumb
	.global Func_081a729c
	.thumb_func
Func_081a729c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r0
	movs r0, #128
	lsls r0, r0, #1
	sub sp, #8
	adds r0, #255
	movs r1, #0
	str r1, [sp, #4]
	mov r8, r0
	movs r1, #112
	movs r0, #172
	bl Runtime_AllocateBlock
	mov r11, r0
	bl Func_080144c0
	movs r0, #1
	bl Func_08013ef8
	bl Func_08014b70
	bl Func_08014368
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_081a7310
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_081a7314
	subs r2, #12
	strh r3, [r2]
	add r2, sp, #4
	ldr r3, .L_081a7318
	ldrh r2, [r2]
	ldr r0, .L_081a731c
	strh r2, [r3, #10]
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #112
	b .L_081a7320
	.2byte 0x0000
.L_081a7310:
	.4byte 0x00000681
.L_081a7314:
	.4byte 0x00001440
.L_081a7318:
	.4byte Data_03001120
.L_081a731c:
	.4byte 0x00000016
.L_081a7320:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_081a7424
	movs r3, #224
	lsls r3, r3, #1
	adds r4, r4, r3
	adds r1, r5, #0
	adds r0, r4, #0
	bl Func_0801587c
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_081a7428
	ldr r2, .L_081a742c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_081a7430
	movs r3, #208
	lsls r3, r3, #1
	movs r7, #0
.L_081a734c:
	movs r6, #0
.L_081a734e:
	adds r2, r3, #0
	movs r0, #128
	lsls r3, r2, #16
	lsls r0, r0, #9
	adds r3, r3, r0
	adds r6, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r6, #29
	bls .L_081a734e
	mov r2, r8
	strh r2, [r1]
	mov r0, r8
	adds r1, #2
	adds r7, #1
	strh r0, [r1]
	adds r1, #2
	cmp r7, #19
	bls .L_081a734c
	ldr r3, .L_081a7434
	movs r7, #0
	movs r2, #0
.L_081a737c:
	adds r7, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r7, #3
	bls .L_081a737c
	movs r3, #128
	movs r1, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081a7434
	adds r1, #16
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_08014bac
	bl Func_08014b70
	mov r1, r9
	cmp r1, #0
	beq .L_081a73f2
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_081a7438
	bl Func_0801591c
	mov r5, r11
	adds r5, #16
	movs r7, #0
.L_081a73c8:
	bl Resource_FindFreeEntry
	lsls r2, r7, #8
	lsrs r2, r2, #1
	adds r2, r6, r2
	movs r1, #128
	bl VramBlock_LoadCached
	adds r2, r5, #0
	movs r3, #0
	stmia r2!, {r3}
	ldr r3, .L_081a743c
	adds r7, #1
	stmia r2!, {r3}
	adds r5, #12
	str r0, [r2]
	cmp r7, #4
	bls .L_081a73c8
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
.L_081a73f2:
	movs r0, #30
	bl Func_08013f3c
	bl Blend_WaitForTransition
	ldr r3, .L_081a7420
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r2, #165
	lsls r2, r2, #2
	mov r3, r9
	mov r10, r2
	cmp r3, #0
	beq .L_081a7440
	movs r0, #168
	lsls r0, r0, #5
	adds r0, #24
	mov r10, r0
	b .L_081a7440
.L_081a741a:
	movs r1, #1
	str r1, [sp, #4]
	b .L_081a74e4
.L_081a7420:
	.4byte 0x00001540
.L_081a7424:
	.4byte gMapCellBuffer
.L_081a7428:
	.4byte 0x06006800
.L_081a742c:
	.4byte 0x84002580
.L_081a7430:
	.4byte 0x06003000
.L_081a7434:
	.4byte Data_03001120
.L_081a7438:
	.4byte Data_081a8288
.L_081a743c:
	.4byte 0x40004000
.L_081a7440:
	movs r7, #0
	cmp r7, r10
	bcs .L_081a74e4
.L_081a7446:
	cmp r7, #120
	bls .L_081a74d4
	mov r2, r9
	cmp r2, #0
	beq .L_081a74bc
	ldr r3, .L_081a7498
	mov r5, r11
	adds r5, #16
	movs r6, #0
	movs r4, #80
	mov r8, r3
.L_081a745c:
	ldr r3, .L_081a7494
	adds r2, r4, #0
	ands r2, r3
	ldrh r3, [r5, #6]
	mov r0, r8
	ands r3, r0
	orrs r3, r2
	strh r3, [r5, #6]
	movs r3, #124
	strb r3, [r5, #4]
	adds r0, r5, #0
	movs r1, #0
	str r4, [sp, #0]
	bl Func_080140d8
	ldr r4, [sp, #0]
	adds r6, #1
	adds r4, #32
	adds r5, #12
	cmp r6, #2
	bls .L_081a745c
	movs r1, #60
	adds r0, r7, #0
	bl Math_ModU
	ldr r2, .L_081a749c
	b .L_081a74a0
	.2byte 0x0000
.L_081a7494:
	.4byte 0x000001ff
.L_081a7498:
	.4byte 0xfffffe00
.L_081a749c:
	.4byte Data_081a873e
.L_081a74a0:
	ldr r3, .L_081a74cc
	ldrb r1, [r2, r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r3, #16
	subs r3, r3, r1
	movs r0, #128
	lsls r3, r3, #8
	lsls r0, r0, #19
	adds r3, r3, r1
	adds r0, #82
	strh r3, [r0]
.L_081a74bc:
	ldr r3, .L_081a74d0
	movs r2, #9
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_081a741a
	b .L_081a74d4
	.2byte 0x0000
.L_081a74cc:
	.4byte 0x00002f50
.L_081a74d0:
	.4byte gInput
.L_081a74d4:
	bl Random16
	adds r7, #1
	movs r0, #1
	bl WaitFrames
	cmp r7, r10
	bcc .L_081a7446
.L_081a74e4:
	movs r0, #172
	bl Runtime_ReleaseHeapBlock
	ldr r2, .L_081a7504
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #4]
	add sp, #8
	b .L_081a7508
.L_081a7504:
	.4byte 0x00000000
.L_081a7508:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.global Data_081a7514
Data_081a7514:
	.4byte 0x00004770
