.syntax unified
	.thumb
	.global Func_08179394
	.thumb_func
Func_08179394:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r11, r0
	ldr r0, [r5, #92]
	sub sp, #172
	str r0, [sp, #64]
	movs r0, #1
	ldr r1, [r5, #96]
	mov r6, r11
	str r1, [sp, #60]
	ldr r2, [r5, #100]
	str r2, [sp, #44]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_081793f4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r0, #188
	movs r1, #7
	str r3, [sp, #48]
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	ldr r2, [r6, #20]
	movs r4, #0
	str r5, [sp, #52]
	mov r8, r4
	cmp r2, #0
	beq .L_08179414
	add r7, sp, #76
	adds r6, r7, #0
	movs r5, #36
	b .L_081793f8
.L_081793f4:
	.4byte 0x00000a10
.L_081793f8:
	mov r1, r11
	ldrsh r0, [r5, r1]
	adds r1, r6, #0
	bl Func_0815e20c
	mov r4, r11
	ldr r2, [r4, #20]
	movs r3, #1
	add r8, r3
	adds r6, #12
	adds r5, #2
	cmp r8, r2
	bne .L_081793f8
	b .L_08179416
.L_08179414:
	add r7, sp, #76
.L_08179416:
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	subs r3, #12
	ldr r1, [r7]
	ldr r3, [r7, r3]
	ldr r0, .L_081797c4
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	str r1, [sp, #40]
	ldr r3, [r7, #4]
	ldr r1, .L_081797c8
	subs r3, #48
	str r3, [sp, #36]
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r6, [sp, #64]
	movs r7, #224
	lsls r7, r7, #3
	adds r1, r6, r7
	ldr r0, .L_081797c8
	movs r2, #64
	movs r3, #64
	bl Func_0816ae40
	ldr r0, .L_081797cc
	ldr r1, [sp, #44]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #142
	lsls r2, r2, #7
	adds r1, r6, r2
	ldr r0, .L_081797d0
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_081797d4
	ldr r1, .L_081797c8
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_081797d8
	ldr r1, .L_081797dc
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r6, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	movs r1, #200
	adds r2, r6, r4
	movs r3, #50
	str r3, [r2]
	ldr r0, .L_081797e0
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r0, [sp, #40]
	mov r7, sp
	movs r6, #0
	adds r7, #68
	lsls r0, r0, #16
	str r6, [sp, #56]
	str r7, [sp, #16]
	str r0, [sp, #12]
.L_081794b6:
	ldr r1, [sp, #56]
	cmp r1, #0
	bne .L_081794f4
	ldr r3, [sp, #64]
	movs r2, #0
	mov r8, r2
	adds r3, #24
	movs r2, #24
.L_081794c6:
	movs r4, #1
	add r8, r4
	mov r6, r8
	str r2, [r3]
	adds r3, #28
	cmp r6, #64
	bne .L_081794c6
	ldr r3, .L_081797e4
	movs r7, #0
	movs r1, #1
	movs r2, #128
	mov r8, r7
	negs r1, r1
	lsls r2, r2, #1
.L_081794e2:
	movs r0, #1
	add r8, r0
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_081794e2
	movs r0, #140
	bl Audio_PlayCue
.L_081794f4:
	ldr r3, [sp, #56]
	subs r3, #40
	cmp r3, #15
	bhi .L_08179502
	ldr r0, .L_081797e8
	bl Func_0815f0a0
.L_08179502:
	ldr r1, [sp, #56]
	cmp r1, #56
	bne .L_0817950e
	movs r0, #144
	bl Audio_PlayCue
.L_0817950e:
	ldr r2, [sp, #56]
	cmp r2, #55
	bgt .L_08179516
	b .L_0817979c
.L_08179516:
	ldr r4, [sp, #36]
	ldr r6, .L_081797ec
	ldr r7, .L_081797f0
	lsls r3, r2, #3
	adds r3, r4, r3
	adds r6, r6, r3
	adds r3, r3, r7
	mov r10, r6
	cmp r3, #7
	bls .L_0817952c
	b .L_08179680
.L_0817952c:
	ldr r1, [sp, #40]
	ldr r2, [sp, #40]
	asrs r1, r1, #31
	lsrs r3, r1, #31
	adds r3, r2, r3
	ldr r6, [sp, #64]
	movs r0, #0
	asrs r3, r3, #1
	mov r8, r0
	mov r9, r1
	lsls r7, r3, #16
.L_08179542:
	mov r4, r8
	negs r3, r4
	cmp r3, #0
	bge .L_0817954c
	adds r3, #3
.L_0817954c:
	asrs r3, r3, #2
	adds r3, #2
	str r3, [r6, #24]
	str r7, [r6]
	bl Random16
	movs r3, #15
	ands r3, r0
	add r3, r10
	adds r3, #16
	lsls r3, r3, #16
	str r3, [r6, #4]
	bl Random16
	mov r1, r11
	ldr r3, [r1, #24]
	ldr r2, .L_081797f4
	lsls r3, r3, #2
	adds r3, #1
	ldrb r5, [r2, r3]
	adds r1, r5, #0
	bl Math_ModU
	lsrs r5, r5, #1
	subs r0, r0, r5
	lsls r0, r0, #12
	str r0, [r6, #12]
	bl Random16
	movs r2, #255
	ands r2, r0
	movs r3, #192
	subs r3, r3, r2
	lsls r3, r3, #10
	str r3, [r6, #16]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r6, #28
	cmp r4, #32
	bne .L_08179542
	mov r7, r11
	ldr r3, [r7, #24]
	ldr r0, .L_081797f4
	lsls r3, r3, #2
	adds r3, #3
	ldrb r3, [r0, r3]
	movs r6, #0
	mov r8, r6
	cmp r3, #0
	beq .L_08179634
	ldr r2, [sp, #40]
	mov r1, r9
	lsrs r3, r1, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r3, r3, #16
	ldr r7, .L_081797f8
	str r3, [sp, #32]
	movs r3, #15
	mov r9, r3
.L_081795c6:
	ldr r4, [sp, #32]
	mov r6, r9
	str r4, [r7]
	bl Random16
	ands r0, r6
	add r0, r10
	adds r0, #24
	lsls r0, r0, #16
	str r0, [r7, #4]
	bl Random16
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	ands r6, r0
	movs r0, #128
	lsls r0, r0, #7
	adds r6, r6, r0
	bl Random16
	movs r5, #255
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	bl Random16
	mov r1, r9
	ands r0, r1
	adds r0, #16
	str r0, [r7, #24]
	mov r4, r11
	ldr r3, [r4, #24]
	ldr r6, .L_081797f4
	lsls r3, r3, #2
	adds r3, #3
	ldrb r3, [r6, r3]
	movs r2, #1
	add r8, r2
	adds r7, #28
	cmp r8, r3
	bne .L_081795c6
.L_08179634:
	movs r0, #145
	bl Func_081180e8
	mov r0, r11
	ldr r3, [r0, #20]
	movs r7, #0
	mov r8, r7
	cmp r3, #0
	beq .L_08179672
	movs r5, #36
.L_08179648:
	mov r1, r11
	ldrsh r0, [r5, r1]
	movs r1, #0
	bl Func_08118088
	mov r3, r11
	ldrsh r0, [r5, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r8
	movs r2, #5
	mov r7, r11
	bl Func_0814cd48
	ldr r3, [r7, #20]
	movs r6, #1
	add r8, r6
	adds r5, #2
	cmp r8, r3
	bne .L_08179648
.L_08179672:
	ldr r0, [sp, #64]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #16
	str r3, [r2]
.L_08179680:
	mov r2, r10
	cmp r2, #52
	bgt .L_081796bc
	ldr r3, [sp, #40]
	movs r4, #24
	lsrs r5, r3, #31
	adds r5, r3, r5
	asrs r5, r5, #1
	movs r6, #64
	adds r2, r5, #0
	str r4, [sp, #0]
	ldr r1, .L_081797c8
	subs r2, #24
	mov r3, r10
	str r4, [sp, #8]
	str r6, [sp, #4]
	ldr r0, [sp, #60]
	ldr r7, [sp, #48]
	mov lr, r7
	.2byte 0xf800
	ldr r4, [sp, #8]
	str r6, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #60]
	ldr r1, .L_081797c8
	adds r2, r5, #0
	mov r3, r10
	ldr r4, [sp, #52]
	mov lr, r4
	.2byte 0xf800
.L_081796bc:
	mov r7, r11
	movs r6, #0
	ldr r2, [r7, #24]
	mov r8, r6
	ldr r6, .L_081797f4
	lsls r3, r2, #2
	ldrb r3, [r6, r3]
	cmp r3, #0
	beq .L_08179730
	ldr r5, [sp, #64]
	adds r7, r6, #0
.L_081796d2:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_0817971a
	cmp r3, #1
	ble .L_0817970e
	adds r1, r3, #0
	cmp r3, #0
	bge .L_081796e4
	adds r1, r3, #3
.L_081796e4:
	ldr r0, [sp, #64]
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r2, #142
	adds r1, r0, r1
	lsls r2, r2, #7
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	subs r2, #16
	subs r3, #32
	ldr r0, [sp, #60]
	ldr r4, [sp, #48]
	mov lr, r4
	.2byte 0xf800
.L_0817970e:
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_081797fc
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
.L_0817971a:
	adds r3, #1
	str r3, [r5, #24]
	mov r1, r11
	ldr r2, [r1, #24]
	movs r0, #1
	lsls r3, r2, #2
	ldrb r3, [r7, r3]
	add r8, r0
	adds r5, #28
	cmp r8, r3
	bne .L_081796d2
.L_08179730:
	movs r3, #0
	mov r8, r3
	lsls r3, r2, #2
	adds r3, #3
	ldrb r3, [r6, r3]
	cmp r3, #0
	beq .L_0817979c
	ldr r5, .L_081797f8
	ldr r7, .L_08179800
.L_08179742:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_0817978c
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r7, r3]
	ldr r6, [sp, #44]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r6, r1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #60]
	ldr r4, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	lsls r2, r2, #5
	movs r1, #60
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	mov r0, r11
	subs r3, #1
	str r3, [r5, #24]
	ldr r6, .L_081797f4
	ldr r2, [r0, #24]
.L_0817978c:
	lsls r3, r2, #2
	adds r3, #3
	ldrb r3, [r6, r3]
	movs r1, #1
	add r8, r1
	adds r5, #28
	cmp r8, r3
	bne .L_08179742
.L_0817979c:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #28]
	movs r0, #1
	bl Func_081969f8
	movs r2, #0
	str r2, [sp, #24]
	movs r2, #7
	str r2, [r0]
	ldr r1, [sp, #16]
	ldr r3, .L_08179804
	str r1, [r0, #16]
	str r3, [r0, #8]
	ldr r3, [sp, #28]
	movs r6, #68
	str r3, [r0, #12]
	ldr r4, [sp, #24]
	b .L_08179808
.L_081797c4:
	.4byte 0x000000ed
.L_081797c8:
	.4byte gMapCellBuffer
.L_081797cc:
	.4byte 0x00000134
.L_081797d0:
	.4byte 0x0000013e
.L_081797d4:
	.4byte 0x0000017f
.L_081797d8:
	.4byte 0x000000c2
.L_081797dc:
	.4byte Data_02011000
.L_081797e0:
	.4byte Func_08143000
.L_081797e4:
	.4byte Data_02015018
.L_081797e8:
	.4byte 0x00000148
.L_081797ec:
	.4byte 0xfffffe20
.L_081797f0:
	.4byte 0xfffffdec
.L_081797f4:
	.4byte Data_08199400
.L_081797f8:
	.4byte Data_02015000
.L_081797fc:
	.4byte 0xfffff000
.L_08179800:
	.4byte Data_08197410
.L_08179804:
	.4byte Data_08199364
.L_08179808:
	add r6, sp
	str r4, [r0, #20]
	str r6, [sp, #16]
	strb r2, [r6]
	strb r2, [r1, #1]
	ldr r3, [sp, #36]
	movs r7, #0
	adds r3, #8
	lsls r3, r3, #16
	str r3, [sp, #20]
	mov r10, r0
	mov r9, r6
	mov r8, r7
.L_08179822:
	mov r0, r11
	ldr r3, [r0, #24]
	ldr r1, .L_0817997c
	lsls r3, r3, #2
	adds r3, #2
	ldrb r3, [r1, r3]
	lsls r6, r3, #10
	lsls r7, r3, #5
	bl Func_08014de4
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	mov r2, r8
	cmp r2, #0
	bne .L_08179860
	ldr r3, [sp, #36]
	ldr r2, .L_08179980
	lsls r1, r3, #16
	ldr r4, [sp, #12]
	ldr r3, .L_08179984
	adds r0, r4, r2
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	b .L_08179872
.L_08179860:
	ldr r1, .L_08179980
	ldr r2, [sp, #20]
	ldr r4, [sp, #12]
	ldr r3, .L_08179984
	adds r0, r4, r1
	adds r1, r2, r3
	movs r2, #0
	bl Func_08015160
.L_08179872:
	movs r0, #250
	lsls r0, r0, #3
	bl SceneTransform_ApplyPitch
	ldr r4, [sp, #56]
	cmp r4, #47
	bgt .L_0817988a
	movs r0, #48
	subs r0, r0, r4
	lsls r0, r0, #9
	bl SceneTransform_ApplyPitch
.L_0817988a:
	mov r0, r8
	cmp r0, #0
	bne .L_081798c8
	ldr r3, [sp, #56]
	subs r3, #16
	adds r5, r7, #0
	muls r5, r3
	cmp r5, r6
	ble .L_0817989e
	adds r5, r6, #0
.L_0817989e:
	ldr r1, [sp, #56]
	cmp r1, #60
	ble .L_081798ac
	lsls r3, r1, #1
	subs r3, #120
	muls r3, r7
	subs r5, r6, r3
.L_081798ac:
	ldr r2, [sp, #64]
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r2, r4
	mov r6, r9
	str r3, [r6, #4]
	ldr r7, [sp, #56]
	negs r3, r7
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #8
	bl Func_08015068
	b .L_081798f0
.L_081798c8:
	ldr r0, [sp, #56]
	lsls r3, r0, #2
	subs r3, #224
	adds r5, r7, #0
	muls r5, r3
	cmp r5, #0
	blt .L_08179914
	movs r2, #128
	lsls r3, r0, #3
	lsls r2, r2, #2
	subs r2, r2, r3
	mov r3, r10
	str r2, [sp, #24]
	str r2, [r3, #20]
	ldr r3, .L_08179988
	mov r4, r9
	movs r1, #128
	str r3, [r4, #4]
	lsls r1, r1, #7
	adds r5, r5, r1
.L_081798f0:
	cmp r5, #0
	ble .L_08179914
	ldr r6, [sp, #24]
	movs r7, #64
	negs r7, r7
	cmp r6, r7
	ble .L_08179914
	adds r0, r5, #0
	bl Func_0801521c
	ldr r0, .L_0817998c
	ldr r1, [sp, #28]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08179914:
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #2
	bne .L_08179822
	mov r0, r10
	bl Sys_Free
	ldr r0, [sp, #28]
	bl Sys_Free
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #64]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #56]
	adds r6, #1
	str r6, [sp, #56]
	cmp r6, #96
	beq .L_08179956
	b .L_081794b6
.L_08179956:
	ldr r0, .L_08179990
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #172
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0817997c:
	.4byte Data_08199400
.L_08179980:
	.4byte 0xff800000
.L_08179984:
	.4byte 0xffc00000
.L_08179988:
	.4byte Data_02011000
.L_0817998c:
	.4byte Data_08199210
.L_08179990:
	.4byte Func_08143000
