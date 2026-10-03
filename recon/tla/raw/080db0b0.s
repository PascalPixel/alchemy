.syntax unified
	.thumb
	.global Func_080db0b0
	.thumb_func
Func_080db0b0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #0
	sub sp, #20
	mov r9, r0
	bl Func_080cdf5c
	bl Object_GetById
	mov r8, r0
	adds r0, r5, #0
	bl Object_GetById
	mov r1, r8
	cmp r1, #0
	bne .L_080db0de
	b .L_080db428
.L_080db0de:
	cmp r0, #0
	bne .L_080db0e4
	b .L_080db428
.L_080db0e4:
	movs r2, #85
	add r2, r8
	ldrb r3, [r2]
	mov r11, r2
	str r3, [sp, #8]
	ldr r4, [r1, #80]
	str r4, [sp, #4]
	ldr r2, [r1, #8]
	ldr r3, [r0, #8]
	cmp r2, r3
	bge .L_080db0fe
	movs r0, #1
	mov r9, r0
.L_080db0fe:
	adds r0, r5, #0
	bl Func_080dae70
	adds r7, r0, #0
	cmp r7, #0
	bne .L_080db10c
	b .L_080db428
.L_080db10c:
	mov r2, r9
	lsls r2, r2, #15
	ldr r1, [r7, #4]
	str r2, [sp, #0]
	ldr r3, [r7, #16]
	eors r3, r2
	movs r2, #128
	lsls r2, r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080db138
	adds r5, r1, #0
	cmp r5, #0
	blt .L_080db136
.L_080db128:
	str r5, [r7, #4]
	movs r0, #1
	subs r5, #64
	bl WaitFrames
	cmp r5, #0
	bge .L_080db128
.L_080db136:
	movs r1, #0
.L_080db138:
	movs r3, #192
	adds r5, r1, #0
	lsls r3, r3, #3
	cmp r5, r3
	bgt .L_080db15e
	ldr r4, [sp, #0]
	movs r0, #128
	lsls r0, r0, #7
	adds r6, r4, r0
.L_080db14a:
	str r5, [r7, #4]
	str r6, [r7, #16]
	movs r0, #1
	bl WaitFrames
	movs r1, #192
	adds r5, #64
	lsls r1, r1, #3
	cmp r5, r1
	ble .L_080db14a
.L_080db15e:
	mov r4, r9
	ldr r3, [r7, #16]
	movs r2, #1
	eors r2, r4
	movs r0, #255
	movs r4, #128
	lsls r0, r0, #8
	lsls r1, r2, #15
	lsls r4, r4, #7
	ands r3, r0
	adds r2, r1, r4
	cmp r3, r2
	beq .L_080db194
	movs r2, #192
	lsls r2, r2, #3
	mov r10, r2
	adds r5, r1, r4
	adds r6, r0, #0
.L_080db182:
	mov r3, r10
	str r3, [r7, #4]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #16]
	ands r3, r6
	cmp r3, r5
	bne .L_080db182
.L_080db194:
	mov r4, r8
	mov r0, r9
	ldr r6, [r4, #80]
	movs r5, #1
	cmp r0, #0
	beq .L_080db1ae
	movs r1, #162
	mov r10, r1
	subs r5, #2
	b .L_080db1b2
.L_080db1a8:
	movs r3, #0
	strh r3, [r6, #18]
	b .L_080db424
.L_080db1ae:
	movs r2, #146
	mov r10, r2
.L_080db1b2:
	movs r3, #1
	str r3, [r7, #20]
	mov r0, r8
	movs r1, #32
	bl Object_SetMode
	mov r3, r10
	lsls r3, r3, #16
	mov r10, r3
.L_080db1c4:
	movs r3, #1
	mov r4, r9
	eors r3, r4
	movs r0, #128
	lsls r3, r3, #15
	lsls r0, r0, #7
	adds r3, r3, r0
	str r3, [r7, #16]
	ldr r3, .L_080db320
	ldr r2, [r3]
	movs r3, #63
	ands r2, r3
	cmp r2, #18
	bhi .L_080db25c
	lsls r3, r2, #2
	ldr r2, .L_080db324
	ldr r3, [r3, r2]
	mov pc, r3
.L_080db1e8:
	.4byte .L_080db234
	.4byte .L_080db244
	.4byte .L_080db244
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db25c
	.4byte .L_080db234
	.4byte .L_080db244
	.4byte .L_080db244
.L_080db234:
	movs r3, #196
	lsls r3, r3, #3
	str r3, [r7, #4]
	movs r3, #232
	lsls r3, r3, #12
	str r3, [r7, #8]
	lsls r3, r5, #10
	b .L_080db28a
.L_080db244:
	ldr r3, [r7, #4]
	movs r1, #128
	adds r3, #16
	str r3, [r7, #4]
	ldr r3, [r7, #8]
	lsls r1, r1, #7
	adds r3, r3, r1
	str r3, [r7, #8]
	lsls r2, r5, #9
	ldrh r3, [r6, #18]
	adds r3, r3, r2
	b .L_080db28a
.L_080db25c:
	ldr r3, [r7, #4]
	movs r2, #192
	lsls r2, r2, #3
	cmp r3, r2
	ble .L_080db26c
	subs r3, #32
	str r3, [r7, #4]
	b .L_080db26e
.L_080db26c:
	str r2, [r7, #4]
.L_080db26e:
	ldr r3, [r7, #8]
	movs r2, #224
	lsls r2, r2, #12
	cmp r3, r2
	ble .L_080db27e
	ldr r4, .L_080db328
	adds r3, r3, r4
	str r3, [r7, #8]
.L_080db27e:
	ldrh r2, [r6, #18]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080db28c
	lsls r3, r5, #7
	subs r3, r2, r3
.L_080db28a:
	strh r3, [r6, #18]
.L_080db28c:
	ldr r1, .L_080db32c
	mov r0, r10
	ldr r2, [r1, #4]
	lsrs r3, r0, #16
	ands r2, r3
	cmp r2, #0
	bne .L_080db1a8
	ldr r3, [r1, #4]
	cmp r3, #0
	beq .L_080db314
	strh r2, [r6, #18]
	movs r1, #6
	str r2, [r7, #20]
	mov r0, r8
	bl Object_SetMode
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Audio_PlayCue
	mov r0, r8
	movs r1, #7
	bl Object_SetMode
	movs r3, #192
	mov r1, r8
	lsls r3, r3, #10
	str r3, [r1, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r1, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r1, #40]
	mov r3, r11
	ldrb r2, [r3]
	movs r3, #126
	ands r3, r2
	mov r4, r11
	strb r3, [r4]
	movs r1, #0
	mov r0, r8
	bl ObjectDispatch_SetSingleChildField26Far
	movs r0, #5
	bl WaitFrames
	mov r0, r8
	movs r1, #16
	bl Object_SetMode
	ldr r5, .L_080db31c
	mov r0, r11
	strb r5, [r0]
	ldr r3, [r7, #24]
	movs r2, #130
	ldr r3, [r3]
	lsls r2, r2, #15
	ldr r1, [r3, #4]
	mov r10, r2
	str r1, [sp, #16]
	ldr r3, [r3, #8]
	str r3, [sp, #12]
	movs r3, #8
	mov r9, r3
	b .L_080db3ae
.L_080db314:
	movs r0, #1
	bl WaitFrames
	b .L_080db1c4
.L_080db31c:
	.4byte 0x00000000
.L_080db320:
	.4byte gFrameCount
.L_080db324:
	.4byte .L_080db1e8
.L_080db328:
	.4byte 0xfffff800
.L_080db32c:
	.4byte gInput
.L_080db330:
	mov r4, r9
	cmp r4, #0
	beq .L_080db34c
	ldr r3, [r7, #8]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	movs r0, #160
	movs r2, #1
	str r3, [r7, #8]
	lsls r0, r0, #7
	negs r2, r2
	add r10, r0
	add r9, r2
.L_080db34c:
	movs r3, #128
	lsls r3, r3, #4
	str r3, [r7, #4]
	movs r3, #1
	str r3, [r7, #20]
	ldr r0, [r7, #16]
	ldr r3, .L_080db438
	adds r0, r0, r3
	bl Trig_Sin
	ldr r1, [r7, #4]
	ldr r5, .L_080db43c
	lsls r3, r1, #2
	adds r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	adds r6, r0, #0
	bl Trig_Sin
	mov r1, r10
	mov lr, r5
	.2byte 0xf800
	ldr r4, [sp, #16]
	mov r1, r8
	subs r0, r4, r0
	str r0, [r1, #8]
	adds r0, r6, #0
	bl Trig_Cos
	mov r1, r10
	mov lr, r5
	.2byte 0xf800
	ldr r2, [sp, #12]
	mov r3, r8
	subs r0, r2, r0
	str r0, [r3, #12]
	ldr r3, [r3, #8]
	mov r4, r8
	str r3, [r4, #56]
	str r0, [r4, #60]
	ldr r0, [sp, #4]
	strh r6, [r0, #18]
	movs r0, #1
	bl WaitFrames
.L_080db3ae:
	ldr r3, [r7, #16]
	ldr r1, [sp, #0]
	movs r2, #255
	movs r4, #224
	lsls r2, r2, #8
	lsls r4, r4, #6
	ands r3, r2
	adds r2, r1, r4
	cmp r3, r2
	bne .L_080db330
	movs r3, #1
	str r3, [r7, #20]
	mov r0, r8
	movs r1, #7
	bl Object_SetMode
	movs r0, #2
	ldr r3, [sp, #8]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	mov r1, r11
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #11
	mov r2, r8
	str r3, [r2, #40]
	movs r5, #7
.L_080db3e6:
	ldr r3, [r7, #8]
	ldr r4, .L_080db440
	movs r0, #1
	adds r3, r3, r4
	str r3, [r7, #8]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_080db3e6
	movs r3, #224
	lsls r3, r3, #12
	str r3, [r7, #8]
	ldr r0, [sp, #4]
	movs r3, #0
	strh r3, [r0, #18]
	movs r1, #6
	mov r0, r8
	bl Object_SetMode
	mov r0, r8
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26Far
	add r1, sp, #8
	ldrb r1, [r1]
	mov r2, r11
	strb r1, [r2]
	movs r0, #6
	bl WaitFrames
.L_080db424:
	movs r3, #1
	str r3, [r7, #20]
.L_080db428:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080db438:
	.4byte 0xfffff060
.L_080db43c:
	.4byte IwramMulQ16
.L_080db440:
	.4byte 0xfffff000
