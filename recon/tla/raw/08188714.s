.syntax unified
	.thumb
	.global Func_08188714
	.thumb_func
Func_08188714:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #104
	str r0, [sp, #56]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #52]
	movs r0, #0
	ldr r3, [r3, #96]
	str r3, [sp, #48]
	bl Func_081435e0
	ldr r2, [sp, #52]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08188798
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r3, #0
	ldr r0, .L_0818879c
	ldr r1, .L_081887a0
	movs r2, #0
	bl Func_08157cf4
	ldr r4, [sp, #56]
	ldr r3, [r4, #24]
	cmp r3, #1
	bne .L_081887ac
	ldr r0, .L_081887a4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081887a8
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r0, #238
	ldr r5, [sp, #52]
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r5, r0
	movs r3, #50
	str r3, [r2]
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	movs r2, #128
	ldr r3, .L_08188794
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	b .L_081887cc
.L_08188794:
	.4byte 0x00001010
.L_08188798:
	.4byte 0x0000013e
.L_0818879c:
	.4byte 0x000000c2
.L_081887a0:
	.4byte gMapCellBuffer
.L_081887a4:
	.4byte 0x00000149
.L_081887a8:
	.4byte IwramCopyWords
.L_081887ac:
	ldr r1, [sp, #52]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #75
	str r3, [r2]
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
.L_081887cc:
	ldr r5, [sp, #52]
	movs r0, #239
	lsls r0, r0, #7
	adds r2, r5, r0
	movs r3, #2
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08188af0
	str r3, [sp, #36]
	bl Func_080145a8
	ldr r1, [sp, #56]
	ldr r0, [r1, #8]
	bl Func_08118088 + 0x10
	ldr r3, [sp, #56]
	adds r5, r0, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_08118088 + 0x10
	ldr r0, [r0]
	ldr r5, [r5]
	str r0, [sp, #32]
	movs r4, #90
	ldr r6, [r5, #8]
	ldr r3, [r0, #8]
	mov r8, r4
	subs r3, r3, r6
	mov r0, r8
	muls r0, r3
	movs r1, #100
	bl __divsi3
	mov r9, r5
	ldr r5, [sp, #32]
	adds r6, r6, r0
	mov r0, r9
	ldr r3, [r5, #16]
	ldr r5, [r0, #16]
	movs r1, #100
	subs r3, r3, r5
	mov r0, r8
	muls r0, r3
	bl __divsi3
	ldr r1, [sp, #56]
	adds r5, r5, r0
	ldr r0, [r1, #8]
	bl Func_08118070
	ldr r3, [sp, #56]
	str r0, [sp, #28]
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_08118070
	ldr r1, [sp, #56]
	str r0, [sp, #24]
	mov r2, sp
	adds r2, #92
	movs r4, #36
	ldrsh r0, [r1, r4]
	adds r1, r2, #0
	str r2, [sp, #20]
	bl Func_0815e21c
	mov r0, r9
	bl Object_ResetMotion
	adds r3, r5, #0
	movs r2, #0
	mov r0, r9
	adds r1, r6, #0
	bl Object_SetPosition
	movs r1, #2
	mov r0, r9
	bl Object_SetMode
	mov r3, r9
	movs r2, #1
	adds r3, #88
	strb r2, [r3]
	adds r3, #2
	str r3, [sp, #16]
	strb r2, [r3]
	movs r3, #128
	mov r4, r9
	lsls r3, r3, #10
	str r3, [r4, #52]
	movs r3, #128
	lsls r3, r3, #12
	movs r0, #20
	str r3, [r4, #48]
	bl WaitFrames
	movs r0, #56
	movs r1, #92
	movs r5, #0
	negs r0, r0
	negs r1, r1
	str r5, [sp, #44]
	str r0, [sp, #12]
	str r1, [sp, #8]
.L_081888a6:
	ldr r2, [sp, #44]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	cmp r2, #0
	bne .L_081888ca
	ldr r3, [sp, #32]
	movs r2, #240
	lsls r2, r2, #12
	str r2, [r3, #40]
	ldr r4, [sp, #32]
	movs r3, #145
	lsls r3, r3, #8
	adds r3, #235
	mov r0, r9
	str r3, [r4, #72]
	str r2, [r0, #40]
	str r3, [r0, #72]
.L_081888ca:
	ldr r1, [sp, #44]
	cmp r1, #11
	bne .L_081888f0
	ldr r2, [sp, #32]
	mov r4, r9
	ldr r3, [r2, #28]
	negs r3, r3
	str r3, [r2, #28]
	ldr r3, [r4, #28]
	negs r3, r3
	str r3, [r4, #28]
	ldr r0, [sp, #28]
	ldr r3, [r4, #12]
	adds r3, r3, r0
	str r3, [r4, #12]
	ldr r1, [sp, #24]
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
.L_081888f0:
	ldr r2, [sp, #44]
	cmp r2, #54
	bne .L_08188950
	ldr r4, [sp, #56]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #10
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r0, [sp, #32]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r0, #72]
	movs r3, #160
	mov r1, r9
	lsls r3, r3, #11
	str r3, [r1, #40]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #81
	str r3, [r1, #72]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r1, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r1, #48]
	ldr r2, [sp, #16]
	movs r3, #0
	strb r3, [r2]
	mov r0, r9
	bl Object_ResetMotion
	mov r4, r9
	ldr r3, [r4, #16]
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Object_SetPosition
.L_08188950:
	ldr r0, [sp, #56]
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_08188a1e
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Func_080156e8
	mov r1, r9
	ldr r3, [r1, #8]
	add r0, sp, #80
	str r3, [r0]
	movs r2, #68
	ldr r3, [r1, #12]
	add r2, sp
	str r3, [r0, #4]
	mov r10, r2
	ldr r3, [r1, #16]
	mov r1, r10
	str r3, [r0, #8]
	bl Func_0815e1ec
	mov r4, r10
	ldr r3, [r4]
	asrs r2, r3, #1
	str r2, [r4]
	ldr r3, [sp, #44]
	subs r3, #54
	cmp r3, #1
	bhi .L_081889b0
	ldr r3, [r4, #4]
	ldr r5, [sp, #52]
	movs r1, #32
	movs r4, #224
	str r1, [sp, #0]
	lsls r4, r4, #3
	movs r1, #64
	str r1, [sp, #4]
	subs r2, #16
	adds r1, r5, r4
	subs r3, #16
	ldr r0, [sp, #48]
	ldr r5, [sp, #36]
	mov lr, r5
	.2byte 0xf800
.L_081889b0:
	ldr r0, [sp, #12]
	cmp r0, #11
	bls .L_081889b8
	b .L_08188af8
.L_081889b8:
	lsrs r3, r0, #31
	adds r3, r0, r3
	ldr r2, [sp, #52]
	asrs r3, r3, #1
	lsls r3, r3, #11
	ldr r7, [sp, #8]
	movs r1, #0
	adds r2, r2, r3
	mov r8, r1
	mov r11, r2
.L_081889cc:
	mov r3, r8
	lsls r6, r3, #12
	adds r0, r6, #0
	bl Trig_Sin
	mov r4, r10
	adds r3, r7, #0
	muls r3, r0
	ldr r5, [r4]
	asrs r3, r3, #16
	adds r0, r6, #0
	adds r5, r5, r3
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r0, [sp, #44]
	movs r2, #32
	subs r5, #16
	str r2, [sp, #0]
	asrs r3, r3, #16
	movs r2, #64
	movs r1, #224
	subs r3, r3, r0
	str r2, [sp, #4]
	lsls r1, r1, #3
	adds r2, r5, #0
	movs r5, #1
	adds r3, #100
	ldr r0, [sp, #48]
	add r1, r11
	ldr r4, [sp, #36]
	add r8, r5
	mov lr, r4
	.2byte 0xf800
	mov r0, r8
	cmp r0, #16
	bne .L_081889cc
	ldr r1, [sp, #56]
	ldr r3, [r1, #24]
	b .L_08188afc
.L_08188a1e:
	ldr r2, [sp, #44]
	cmp r2, #55
	ble .L_08188afc
	cmp r2, #56
	bne .L_08188a8e
	ldr r4, [sp, #20]
	ldr r7, [sp, #52]
	movs r3, #0
	mov r8, r3
	mov r10, r4
.L_08188a32:
	mov r5, r10
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r5, #4]
	movs r5, #255
	subs r3, #16
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	mov r0, r8
	asrs r3, r3, #5
	str r3, [r7, #16]
	negs r3, r0
	cmp r3, #0
	bge .L_08188a7e
	adds r3, #3
.L_08188a7e:
	movs r1, #1
	add r8, r1
	asrs r3, r3, #2
	mov r2, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #32
	bne .L_08188a32
.L_08188a8e:
	ldr r5, [sp, #52]
	movs r3, #0
	mov r8, r3
.L_08188a94:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_08188ad8
	adds r1, r3, #0
	cmp r3, #0
	bge .L_08188aa2
	adds r1, r3, #3
.L_08188aa2:
	ldr r4, [sp, #52]
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r0, #224
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r1, r4, r1
	lsls r0, r0, #3
	movs r4, #6
	ldrsh r3, [r5, r4]
	adds r1, r1, r0
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #48]
	ldr r4, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #62
	ldr r2, .L_08188af4
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_08188ad8:
	movs r0, #1
	add r8, r0
	adds r3, #1
	mov r1, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #16
	bne .L_08188a94
	ldr r2, [sp, #56]
	ldr r3, [r2, #24]
	b .L_08188afc
	.2byte 0x0000
.L_08188af0:
	.4byte Func_08143000
.L_08188af4:
	.4byte 0xffffe000
.L_08188af8:
	ldr r4, [sp, #56]
	ldr r3, [r4, #24]
.L_08188afc:
	cmp r3, #1
	bne .L_08188bca
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #1
	bl Func_081969f8
	adds r5, r0, #0
	ldr r0, [sp, #44]
	cmp r0, #55
	ble .L_08188bbe
	ldr r1, [sp, #12]
	movs r2, #128
	lsls r3, r1, #13
	lsls r2, r2, #7
	adds r6, r3, r2
	ldr r3, [sp, #44]
	movs r0, #0
	cmp r3, #63
	ble .L_08188b32
	ldr r4, [sp, #44]
	movs r3, #64
	subs r3, r3, r4
	lsls r0, r3, #2
.L_08188b32:
	movs r1, #64
	negs r1, r1
	cmp r0, r1
	ble .L_08188bbe
	ldr r3, [sp, #60]
	ldr r2, .L_08188c94
	movs r1, #7
	ands r3, r2
	ldr r2, .L_08188c98
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #60]
	ldr r3, .L_08188c9c
	ldr r4, .L_08188ca0
	add r2, sp, #60
	str r3, [r5, #8]
	mov r3, r10
	str r1, [r5]
	str r2, [r5, #16]
	str r3, [r5, #12]
	str r4, [r2, #4]
	str r0, [r5, #20]
	movs r7, #128
	movs r0, #0
	mov r8, r0
	lsls r7, r7, #6
.L_08188b6c:
	bl Func_08014de4
	ldr r1, [sp, #20]
	movs r2, #0
	ldr r0, [r1]
	movs r1, #128
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	lsls r0, r0, #16
	lsls r1, r1, #14
	bl Func_08015160
	adds r1, r6, #0
	adds r2, r6, #0
	adds r0, r6, #0
	bl Func_080151e4
	adds r0, r7, #0
	bl Func_080150e4
	ldr r0, .L_08188ca4
	bl Func_08015024
	movs r2, #4
	ldr r0, .L_08188ca8
	mov r1, r10
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	movs r3, #1
	movs r2, #128
	add r8, r3
	lsls r2, r2, #7
	mov r4, r8
	adds r7, r7, r2
	cmp r4, #2
	bne .L_08188b6c
.L_08188bbe:
	adds r0, r5, #0
	bl Sys_Free
	mov r0, r10
	bl Sys_Free
.L_08188bca:
	ldr r5, [sp, #44]
	cmp r5, #64
	bne .L_08188bf8
	ldr r0, [sp, #32]
	mov r1, r9
	ldr r3, [r0, #28]
	negs r3, r3
	str r3, [r0, #28]
	ldr r3, [r1, #28]
	negs r3, r3
	str r3, [r1, #28]
	ldr r2, [sp, #28]
	ldr r3, [r1, #12]
	subs r3, r3, r2
	str r3, [r1, #12]
	ldr r4, [sp, #24]
	ldr r3, [r0, #12]
	movs r1, #0
	subs r3, r3, r4
	str r3, [r0, #12]
	mov r0, r9
	bl Object_SetMode
.L_08188bf8:
	ldr r5, [sp, #44]
	cmp r5, #54
	bne .L_08188c14
	ldr r0, [sp, #56]
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_08188c0e
	movs r0, #134
	bl Func_08118088 + 0x60
	b .L_08188c14
.L_08188c0e:
	movs r0, #145
	bl Func_08118088 + 0x60
.L_08188c14:
	ldr r1, [sp, #44]
	cmp r1, #0
	bne .L_08188c2e
	movs r0, #136
	bl Audio_PlayCue
	ldr r3, [sp, #52]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #6
	str r3, [r2]
.L_08188c2e:
	ldr r5, [sp, #44]
	cmp r5, #53
	bne .L_08188c42
	ldr r0, [sp, #52]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #6
	str r3, [r2]
.L_08188c42:
	movs r1, #16
	movs r0, #16
	bl Func_08158ce0
	movs r4, #240
	ldr r3, [sp, #52]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r5, [sp, #12]
	ldr r0, [sp, #8]
	ldr r1, [sp, #44]
	adds r5, #1
	adds r0, #2
	adds r1, #1
	str r5, [sp, #12]
	str r0, [sp, #8]
	str r1, [sp, #44]
	cmp r1, #96
	beq .L_08188c76
	b .L_081888a6
.L_08188c76:
	ldr r0, .L_08188cac
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08188c94:
	.4byte 0xffffff00
.L_08188c98:
	.4byte 0xffff00ff
.L_08188c9c:
	.4byte Data_08199364
.L_08188ca0:
	.4byte gMapCellBuffer
.L_08188ca4:
	.4byte 0xfffff000
.L_08188ca8:
	.4byte Data_08199210
.L_08188cac:
	.4byte Func_08143000
