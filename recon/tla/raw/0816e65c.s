.syntax unified
	.thumb
	.global Func_0816e65c
	.thumb_func
Func_0816e65c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	str r0, [sp, #52]
	str r1, [sp, #48]
	movs r5, #192
	lsls r5, r5, #18
	ldr r1, [r5, #92]
	ldr r2, [r5, #96]
	movs r0, #1
	str r2, [sp, #44]
	mov r11, r1
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816e6c0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	ldr r5, [r5, #104]
	adds r2, #132
	add r2, r11
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816e6c4
	str r5, [sp, #36]
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #52]
	ldr r0, [r3, #8]
	b .L_0816e6c8
	.2byte 0x0000
.L_0816e6c0:
	.4byte 0x00000410
.L_0816e6c4:
	.4byte Func_08143000
.L_0816e6c8:
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r1, [sp, #48]
	str r0, [sp, #28]
	cmp r1, #0
	bne .L_0816e73e
	movs r1, #224
	movs r2, #80
	lsls r1, r1, #3
	movs r5, #128
	str r2, [sp, #24]
	ldr r0, .L_0816ea2c
	add r1, r11
	movs r2, #1
	movs r3, #1
	lsls r5, r5, #5
	bl Func_08157cf4
	movs r4, #1
	mov r12, r5
.L_0816e6f2:
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #118
	lsls r2, r4, #1
	adds r2, r2, r3
	movs r3, #4
	subs r3, r3, r4
	movs r1, #240
	lsls r1, r1, #6
	lsls r3, r3, #12
	adds r3, r3, r1
	strh r3, [r2]
	movs r2, #0
	mov r8, r2
	movs r2, #224
	adds r0, r4, #0
	lsls r2, r2, #3
	adds r1, r5, #0
	adds r0, #59
	add r2, r11
.L_0816e71a:
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_0816e724
	strb r3, [r2, r1]
	b .L_0816e726
.L_0816e724:
	strb r0, [r2, r1]
.L_0816e726:
	movs r3, #1
	add r8, r3
	adds r2, #1
	cmp r8, r12
	bne .L_0816e71a
	movs r1, #128
	lsls r1, r1, #5
	adds r4, #1
	adds r5, r5, r1
	cmp r4, #5
	bne .L_0816e6f2
	b .L_0816e752
.L_0816e73e:
	movs r1, #224
	lsls r1, r1, #3
	movs r2, #1
	ldr r0, .L_0816ea30
	add r1, r11
	movs r3, #1
	bl Func_08157cf4
	movs r2, #64
	str r2, [sp, #24]
.L_0816e752:
	movs r0, #212
	bl Audio_PlayCue
	ldr r1, [sp, #24]
	movs r3, #0
	str r3, [sp, #32]
	cmp r1, #0
	bne .L_0816e764
	b .L_0816ec16
.L_0816e764:
	mov r2, sp
	mov r3, sp
	mov r1, sp
	adds r2, #76
	adds r3, #64
	adds r1, #56
	str r2, [sp, #8]
	str r3, [sp, #12]
	str r1, [sp, #16]
.L_0816e776:
	ldr r2, [sp, #32]
	cmp r2, #0
	bne .L_0816e7ee
	ldr r1, [sp, #52]
	add r5, sp, #88
	movs r3, #36
	ldrsh r0, [r1, r3]
	ldr r7, .L_0816ea34
	adds r1, r5, #0
	bl Func_0815e21c
	movs r2, #0
	mov r8, r2
	mov r10, r5
.L_0816e792:
	bl Random16
	adds r6, r0, #0
	bl Random16
	mov r1, r10
	ldr r3, [r1]
	movs r5, #255
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	ands r5, r0
	ldr r3, [r1, #4]
	adds r0, r6, #0
	subs r3, #16
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #128
	add r8, r2
	lsls r3, r3, #1
	adds r7, #28
	cmp r8, r3
	bne .L_0816e792
.L_0816e7ee:
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_0816e80e
	ldr r2, [sp, #32]
	cmp r2, #8
	bne .L_0816e82e
	ldr r3, [sp, #52]
	movs r1, #1
	ldr r0, [r3, #8]
	negs r1, r1
	movs r2, #2
	movs r3, #0
	str r1, [sp, #0]
	bl Func_0814cd48
	b .L_0816e82e
.L_0816e80e:
	ldr r1, [sp, #32]
	cmp r1, #0
	bne .L_0816e82e
	ldr r2, [sp, #52]
	movs r1, #1
	ldr r0, [r2, #8]
	negs r1, r1
	movs r2, #2
	movs r3, #0
	str r1, [sp, #0]
	bl Func_0814cd48
	movs r1, #32
	ldr r0, [sp, #28]
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_0816e82e:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #20]
	movs r0, #1
	bl Func_081969f8
	ldr r3, [sp, #52]
	ldr r1, [sp, #8]
	mov r10, r0
	ldr r0, [r3, #8]
	bl Func_0815e21c
	ldr r2, [sp, #52]
	movs r1, #36
	ldrsh r0, [r2, r1]
	ldr r1, [sp, #12]
	bl Func_0815e21c
	mov r1, r10
	movs r3, #9
	str r3, [r1]
	ldr r2, [sp, #16]
	str r2, [r1, #16]
	ldr r3, [sp, #20]
	movs r2, #0
	str r3, [r1, #12]
	str r2, [r1, #20]
	ldr r3, [sp, #48]
	cmp r3, #0
	beq .L_0816e86e
	b .L_0816e9c6
.L_0816e86e:
	ldr r1, [sp, #16]
	movs r3, #5
	strb r3, [r1]
	movs r3, #7
	strb r3, [r1, #1]
	ldr r3, .L_0816ea38
	mov r2, r10
	str r3, [r2, #8]
	ldr r3, [sp, #32]
	cmp r3, #59
	ble .L_0816e88c
	ldr r1, [sp, #32]
	movs r3, #68
	subs r6, r3, r1
	b .L_0816e890
.L_0816e88c:
	ldr r6, [sp, #32]
	adds r6, #2
.L_0816e890:
	cmp r6, #8
	ble .L_0816e896
	movs r6, #8
.L_0816e896:
	cmp r6, #0
	bgt .L_0816e89c
	b .L_0816eace
.L_0816e89c:
	ldr r2, [sp, #32]
	cmp r2, #38
	ble .L_0816e8bc
	ldr r3, .L_0816ea3c
	lsls r0, r2, #9
	adds r0, r0, r3
	bl Trig_Sin
	cmp r0, #0
	bge .L_0816e8b2
	adds r0, #15
.L_0816e8b2:
	movs r3, #128
	asrs r2, r0, #4
	lsls r3, r3, #7
	subs r0, r3, r2
	b .L_0816e8dc
.L_0816e8bc:
	ldr r1, [sp, #32]
	cmp r1, #15
	ble .L_0816e8da
	ldr r2, .L_0816ea40
	lsls r0, r1, #9
	adds r0, r0, r2
	bl Trig_Cos
	movs r3, #128
	lsls r3, r3, #9
	subs r3, r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r0, r3, #1
	b .L_0816e8dc
.L_0816e8da:
	movs r0, #0
.L_0816e8dc:
	movs r3, #4
	mov r1, r11
	mov r8, r3
	adds r1, #108
.L_0816e8e4:
	ldr r3, [r1]
	movs r2, #1
	negs r2, r2
	add r8, r2
	str r3, [r1, #28]
	mov r3, r8
	subs r1, #28
	cmp r3, #0
	bne .L_0816e8e4
	mov r1, r11
	str r0, [r1, #24]
	ldr r3, [sp, #8]
	movs r2, #4
	mov r7, r11
	mov r8, r2
	mov r9, r3
	adds r7, #112
.L_0816e906:
	mov r1, r8
	cmp r1, #0
	beq .L_0816e916
	ldr r2, [sp, #32]
	mov r3, r8
	adds r3, #38
	cmp r2, r3
	bgt .L_0816e9b8
.L_0816e916:
	ldr r2, [sp, #32]
	mov r1, r8
	lsls r3, r1, #1
	cmp r2, r3
	blt .L_0816e9b8
	ldr r2, [sp, #16]
	lsls r3, r1, #12
	movs r1, #224
	lsls r1, r1, #3
	add r3, r11
	adds r3, r3, r1
	str r3, [r2, #4]
	bl Func_08014de4
	mov r3, r9
	ldr r0, [r3]
	mov r2, r9
	lsrs r3, r0, #31
	ldr r1, [r2, #4]
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	subs r1, #88
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	movs r5, #128
	bl Func_08015160
	lsls r5, r5, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r0, r5, #0
	adds r2, r5, #0
	bl Func_080151e4
	ldr r1, [sp, #52]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816e970
	ldr r0, [r7, #24]
	subs r0, r5, r0
	bl Func_080150e4
	b .L_0816e97c
.L_0816e970:
	ldr r0, [r7, #24]
	movs r2, #128
	lsls r2, r2, #8
	adds r0, r0, r2
	bl Func_080150e4
.L_0816e97c:
	movs r0, #0
	ldr r1, .L_0816ea44
	movs r2, #0
	bl Func_08015160
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #136
	adds r1, r6, #0
	muls r1, r3
	lsls r0, r6, #15
	lsls r2, r6, #13
	bl Func_080151e4
	ldr r1, [sp, #52]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816e9a8
	movs r0, #128
	lsls r0, r0, #8
	bl Func_08015068
.L_0816e9a8:
	ldr r0, .L_0816ea48
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_0816e9b8:
	movs r2, #1
	negs r2, r2
	add r8, r2
	subs r7, #28
	cmp r8, r2
	bne .L_0816e906
	b .L_0816eace
.L_0816e9c6:
	ldr r3, .L_0816ea4c
	mov r1, r10
	str r3, [r1, #8]
	ldr r2, [sp, #16]
	movs r3, #6
	add r6, sp, #56
	strb r3, [r2]
	str r6, [sp, #16]
	strb r3, [r6, #1]
	ldr r3, [sp, #32]
	cmp r3, #0
	bne .L_0816ea66
	ldr r4, [sp, #8]
	ldr r5, [sp, #12]
	movs r1, #0
	mov r8, r1
	mov r0, r11
.L_0816e9e8:
	ldr r3, [r4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r0]
	ldr r3, [r4, #4]
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r0, #4]
	ldr r2, [r5]
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r4]
	asrs r2, r2, #1
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	subs r2, r2, r3
	lsls r2, r2, #11
	str r2, [r0, #12]
	ldr r2, [r4, #4]
	ldr r3, [r5, #4]
	subs r3, r3, r2
	lsls r3, r3, #11
	movs r2, #0
	str r3, [r0, #16]
	str r2, [r0, #8]
	ldr r1, [sp, #52]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816ea54
	ldr r3, .L_0816ea50
	b .L_0816ea58
.L_0816ea2c:
	.4byte 0x000000db
.L_0816ea30:
	.4byte 0x00000100
.L_0816ea34:
	.4byte gMapCellBuffer
.L_0816ea38:
	.4byte Data_0819928c
.L_0816ea3c:
	.4byte 0xffffb200
.L_0816ea40:
	.4byte 0xffffe000
.L_0816ea44:
	.4byte 0xfff00000
.L_0816ea48:
	.4byte Data_081991c0
.L_0816ea4c:
	.4byte Data_08199340
.L_0816ea50:
	.4byte 0xfffff000
.L_0816ea54:
	movs r3, #128
	lsls r3, r3, #5
.L_0816ea58:
	str r3, [r0, #20]
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r0, #28
	cmp r3, #5
	bne .L_0816e9e8
.L_0816ea66:
	ldr r1, [sp, #32]
	cmp r1, #7
	ble .L_0816eace
	movs r3, #224
	lsls r3, r3, #3
	add r3, r11
	str r3, [r6, #4]
	bl Func_08014de4
	mov r2, r11
	ldr r3, .L_0816ec3c
	ldr r0, [r2]
	ldr r1, [r2, #4]
	movs r5, #156
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	lsls r5, r5, #9
	bl Func_08015160
	adds r5, #128
	movs r2, #128
	adds r1, r5, #0
	asrs r0, r5, #1
	lsls r2, r2, #8
	bl Func_080151e4
	mov r3, r11
	ldr r0, [r3, #8]
	bl Func_080150e4
	ldr r1, [sp, #52]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816eab4
	movs r0, #128
	lsls r0, r0, #8
	bl Func_08015068
.L_0816eab4:
	ldr r1, [sp, #20]
	movs r2, #4
	ldr r0, .L_0816ec40
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	mov r0, r11
	movs r1, #64
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
.L_0816eace:
	mov r0, r10
	bl Sys_Free
	ldr r0, [sp, #20]
	bl Sys_Free
	ldr r6, .L_0816ec44
	ldr r2, [sp, #48]
	ldr r1, [sp, #32]
	ldrb r3, [r6, r2]
	cmp r1, r3
	bne .L_0816eb1a
	movs r0, #144
	bl Func_081180e8
	ldr r3, [sp, #52]
	movs r1, #4
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_08118088
	ldr r2, [sp, #52]
	movs r5, #8
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r3, #0
	movs r1, #7
	movs r2, #5
	str r5, [sp, #0]
	bl Func_0814cd48
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	str r5, [r3]
	ldr r1, [sp, #48]
	ldrb r3, [r6, r1]
.L_0816eb1a:
	ldr r2, [sp, #32]
	cmp r2, r3
	blt .L_0816ebda
	ldr r2, .L_0816ec48
	ldr r1, [sp, #48]
	movs r3, #0
	mov r8, r3
	ldrb r3, [r2, r1]
	cmp r3, #0
	beq .L_0816ebda
	ldr r6, [sp, #36]
	ldr r5, .L_0816ec4c
	movs r3, #1
	movs r1, #2
	movs r7, #128
	mov r9, r3
	mov r10, r1
	lsls r7, r7, #5
.L_0816eb3e:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0816ebcc
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r1, #6
	ldrsh r3, [r5, r1]
	mov r1, r9
	str r1, [sp, #0]
	mov r1, r10
	str r1, [sp, #4]
	ldr r0, [sp, #44]
	subs r3, #1
	ldr r1, .L_0816ec50
	mov lr, r6
	.2byte 0xf800
	movs r1, #62
	adds r0, r5, #0
	adds r2, r7, #0
	bl BattleFxKernels_IntegrateVector2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r1, #6
	ldrsh r3, [r5, r1]
	mov r1, r9
	str r1, [sp, #0]
	mov r1, r10
	str r1, [sp, #4]
	subs r3, #1
	ldr r0, [sp, #44]
	ldr r1, .L_0816ec50
	mov lr, r6
	.2byte 0xf800
	movs r1, #62
	adds r0, r5, #0
	adds r2, r7, #0
	bl BattleFxKernels_IntegrateVector2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r1, #6
	ldrsh r3, [r5, r1]
	mov r1, r9
	str r1, [sp, #0]
	mov r1, r10
	subs r3, #1
	str r1, [sp, #4]
	ldr r0, [sp, #44]
	ldr r1, .L_0816ec50
	mov lr, r6
	.2byte 0xf800
	adds r2, r7, #0
	adds r0, r5, #0
	movs r1, #62
	bl BattleFxKernels_IntegrateVector2
	movs r2, #6
	ldrsh r3, [r5, r2]
	cmp r3, #104
	ble .L_0816ebc4
	ldr r3, [r5, #16]
	negs r3, r3
	str r3, [r5, #16]
	ldr r3, [r5, #24]
	subs r3, #8
	str r3, [r5, #24]
.L_0816ebc4:
	ldr r3, [r5, #24]
	ldr r2, .L_0816ec48
	subs r3, #1
	str r3, [r5, #24]
.L_0816ebcc:
	ldr r1, [sp, #48]
	movs r3, #1
	add r8, r3
	ldrb r3, [r2, r1]
	adds r5, #28
	cmp r8, r3
	bne .L_0816eb3e
.L_0816ebda:
	ldr r2, [sp, #48]
	cmp r2, #0
	bne .L_0816ebea
	movs r0, #4
	movs r1, #16
	bl Func_08158ce0
	b .L_0816ebf2
.L_0816ebea:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
.L_0816ebf2:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #32]
	ldr r1, [sp, #24]
	adds r3, #1
	str r3, [sp, #32]
	cmp r3, r1
	beq .L_0816ec16
	b .L_0816e776
.L_0816ec16:
	ldr r0, [sp, #28]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0816ec54
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816ec3c:
	.4byte 0xffc00000
.L_0816ec40:
	.4byte Data_081991e0
.L_0816ec44:
	.4byte Data_08198b96
.L_0816ec48:
	.4byte Data_08198b98
.L_0816ec4c:
	.4byte gMapCellBuffer
.L_0816ec50:
	.4byte Data_08198b9a
.L_0816ec54:
	.4byte Func_08143000
