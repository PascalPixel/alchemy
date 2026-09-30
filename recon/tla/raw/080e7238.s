.syntax unified
	.thumb
	.global Func_080e7238
	.thumb_func
Func_080e7238:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #240
	lsls r1, r1, #5
	adds r1, #144
	movs r0, #92
	sub sp, #32
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #16]
	bl Func_080cdf5c
	bl ObjectTable_Get
	mov r8, r0
	ldr r0, .L_080e74d0
	bl Resource_GetTableEntry
	ldr r1, [sp, #16]
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #3
	adds r1, r5, #0
	ldr r2, [sp, #16]
	str r0, [sp, #8]
	bl VramBlock_LoadCached
	ldr r2, [sp, #16]
	movs r3, #239
	movs r1, #0
	lsls r3, r3, #4
	adds r6, r2, r5
	mov r9, r0
	mov r10, r1
	movs r7, #15
	adds r5, r2, r3
.L_080e7290:
	mov r3, r10
	ands r3, r7
	lsls r3, r3, #1
	add r3, r9
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	adds r3, r7, #0
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r2, #1
	movs r3, #240
	strh r3, [r5, #30]
	add r10, r2
	subs r3, #241
	str r3, [r6, #24]
	mov r3, r10
	adds r5, #40
	adds r6, #28
	cmp r3, #99
	ble .L_080e7290
	movs r0, #200
	bl Audio_PlayCue
	movs r0, #20
	bl WaitFrames
	movs r3, #0
	mov r1, r8
	str r3, [r1, #24]
	str r3, [r1, #28]
	mov r0, r8
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	movs r3, #0
	movs r2, #2
	str r2, [sp, #4]
	str r3, [sp, #12]
	mov r11, r3
	mov r9, r3
.L_080e72fc:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq .L_080e736c
	movs r7, #160
	lsls r7, r7, #15
	mov r10, r1
.L_080e7308:
	bl Random16
	mov r2, r11
	ldr r1, [sp, #16]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r2, #128
	adds r3, r1, r3
	lsls r2, r2, #3
	adds r6, r3, r2
	movs r3, #0
	str r3, [r6, #24]
	mov r1, r8
	ldr r3, [r1, #8]
	adds r5, r0, #0
	str r3, [r6]
	bl Random16
	mov r1, r8
	ldr r2, [r1, #12]
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [r6, #4]
	ldr r3, [r1, #16]
	str r3, [r6, #8]
	bl Random16
	adds r1, r5, #0
	lsls r0, r0, #3
	adds r2, r6, #0
	bl Func_0801489c
	movs r2, #1
	add r11, r2
	mov r0, r11
	movs r1, #100
	str r5, [r6, #12]
	str r7, [r6, #20]
	bl Math_Mod
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r1, r10
	mov r11, r0
	cmp r1, #0
	bne .L_080e7308
.L_080e736c:
	ldr r2, [sp, #16]
	movs r3, #128
	movs r1, #239
	lsls r3, r3, #3
	lsls r1, r1, #4
	adds r6, r2, r3
	adds r7, r2, r1
	movs r2, #99
	add r5, sp, #20
	mov r10, r2
.L_080e7380:
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_080e73d6
	cmp r3, #19
	bhi .L_080e73c8
	ldr r3, [r6]
	ldr r1, [r6, #12]
	ldr r0, [r6, #20]
	str r3, [r5]
	adds r2, r5, #0
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	bl Func_0801489c
	adds r0, r5, #0
	bl Func_080dc390
	ldr r3, [r5]
	adds r0, r7, #0
	str r3, [r7, #12]
	ldr r3, [r5, #8]
	str r3, [r7, #16]
	bl Func_080eb01c
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #4
	adds r3, r3, r1
	str r3, [r6, #12]
	ldr r2, .L_080e74d4
	ldr r3, [r6, #20]
	adds r3, r3, r2
	str r3, [r6, #20]
	ldr r3, [r6, #24]
.L_080e73c8:
	adds r3, #1
	str r3, [r6, #24]
	cmp r3, #20
	bne .L_080e73d6
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
.L_080e73d6:
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r1, r10
	adds r7, #40
	adds r6, #28
	cmp r1, #0
	bge .L_080e7380
	ldr r2, [sp, #12]
	cmp r2, #1
	beq .L_080e741e
	cmp r2, #1
	bgt .L_080e73f6
	cmp r2, #0
	beq .L_080e73fe
	b .L_080e7498
.L_080e73f6:
	ldr r3, [sp, #12]
	cmp r3, #2
	beq .L_080e745c
	b .L_080e7498
.L_080e73fe:
	mov r1, r9
	cmp r1, #20
	bne .L_080e7408
	movs r2, #3
	str r2, [sp, #4]
.L_080e7408:
	mov r3, r9
	cmp r3, #30
	bne .L_080e7498
	movs r3, #1
	movs r1, #5
	movs r2, #1
	negs r3, r3
	str r1, [sp, #4]
	str r2, [sp, #12]
	mov r9, r3
	b .L_080e7498
.L_080e741e:
	mov r1, r8
	ldrh r3, [r1, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	mov r2, r9
	strh r3, [r1, #6]
	lsls r0, r2, #16
	movs r1, #30
	bl __divsi3
	mov r3, r8
	str r0, [r3, #24]
	movs r3, #128
	lsls r3, r3, #9
	cmp r0, r3
	ble .L_080e7444
	mov r1, r8
	str r3, [r1, #24]
.L_080e7444:
	mov r2, r8
	ldr r3, [r2, #24]
	str r3, [r2, #28]
	mov r3, r9
	cmp r3, #40
	bne .L_080e7498
	movs r2, #1
	movs r1, #2
	negs r2, r2
	str r1, [sp, #12]
	mov r9, r2
	b .L_080e7498
.L_080e745c:
	mov r3, r9
	cmp r3, #0
	bne .L_080e7476
	movs r3, #128
	lsls r3, r3, #7
	mov r1, r8
	strh r3, [r1, #6]
	mov r0, r8
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26Far
	movs r2, #3
	str r2, [sp, #4]
.L_080e7476:
	mov r3, r9
	cmp r3, #10
	bne .L_080e7480
	movs r1, #1
	str r1, [sp, #4]
.L_080e7480:
	mov r2, r9
	cmp r2, #20
	bne .L_080e748a
	movs r3, #0
	str r3, [sp, #4]
.L_080e748a:
	mov r1, r9
	cmp r1, #40
	bne .L_080e7498
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	str r2, [sp, #12]
.L_080e7498:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	ldr r1, [sp, #12]
	lsls r2, r2, #2
	movs r3, #1
	adds r2, #255
	add r9, r3
	cmp r1, r2
	beq .L_080e74b0
	b .L_080e72fc
.L_080e74b0:
	ldr r0, [sp, #8]
	bl Func_08014274
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	bl Func_080dc954
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e74d0:
	.4byte 0x000001ef
.L_080e74d4:
	.4byte 0xfffc0000
