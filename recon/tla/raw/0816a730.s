.syntax unified
	.thumb
	.global Func_0816a730
	.thumb_func
Func_0816a730:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	str r0, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #96]
	ldr r2, [r3, #92]
	str r4, [sp, #16]
	movs r0, #0
	mov r10, r2
	ldr r5, [r3, #100]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816a794
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	adds r1, r5, #0
	ldr r0, .L_0816a798
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_0816a79c
	add r1, r10
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #182
	lsls r1, r1, #4
	ldr r0, .L_0816a7a0
	add r1, r10
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	b .L_0816a7a4
.L_0816a794:
	.4byte 0x00000100
.L_0816a798:
	.4byte 0x00000134
.L_0816a79c:
	.4byte 0x00000113
.L_0816a7a0:
	.4byte 0x00000184
.L_0816a7a4:
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r10
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816a9f8
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #20]
	add r5, sp, #32
	movs r2, #36
	ldrsh r0, [r3, r2]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r2, [r5]
	movs r1, #128
	movs r3, #64
	lsls r1, r1, #19
	subs r3, r3, r2
	adds r1, #40
	lsls r3, r3, #8
	str r3, [r1]
	movs r4, #0
	mov r8, r4
	mov r7, r10
.L_0816a7e2:
	bl Random16
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r5, r0, #0
	adds r3, #255
	ands r5, r3
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r7]
	movs r3, #224
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #128
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	movs r2, #1
	ands r3, r0
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #16
	bne .L_0816a7e2
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r10
	movs r3, #8
	str r3, [r2]
	mov r2, sp
	movs r4, #0
	adds r2, #24
	str r2, [sp, #12]
	str r4, [sp, #8]
	mov r9, r4
.L_0816a854:
	mov r3, r9
	cmp r3, #10
	bne .L_0816a88c
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	movs r4, #8
	add r3, r10
	str r4, [r3]
	movs r0, #212
	bl Func_081180e8
	ldr r3, [sp, #20]
	movs r1, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_08118088
	ldr r2, [sp, #20]
	movs r3, #8
	movs r4, #36
	ldrsh r0, [r2, r4]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_0816a88c:
	mov r4, r9
	cmp r4, #7
	ble .L_0816a91c
	cmp r4, #31
	bgt .L_0816a8a2
	lsls r3, r4, #1
	add r3, r9
	lsls r3, r3, #2
	adds r5, r3, #0
	subs r5, #96
	b .L_0816a8aa
.L_0816a8a2:
	ldr r2, [sp, #8]
	movs r3, #136
	lsls r3, r3, #1
	subs r5, r3, r2
.L_0816a8aa:
	cmp r5, #0
	ble .L_0816a91c
	cmp r5, #80
	ble .L_0816a8b8
	movs r5, #80
	movs r7, #1
	b .L_0816a8ba
.L_0816a8b8:
	movs r7, #0
.L_0816a8ba:
	movs r3, #0
	movs r4, #112
	mov r8, r3
	movs r6, #50
	mov r11, r4
.L_0816a8c4:
	mov r2, r8
	cmp r2, #0
	bne .L_0816a8da
	movs r0, #104
	movs r1, #7
	movs r2, #7
	movs r3, #3
	str r7, [sp, #0]
	bl Func_08196404
	b .L_0816a8e8
.L_0816a8da:
	movs r0, #104
	movs r1, #7
	movs r2, #7
	movs r3, #7
	str r7, [sp, #0]
	bl Func_08196404
.L_0816a8e8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #224
	mov r12, r3
	str r3, [sp, #24]
	mov r4, r11
	movs r3, #14
	lsls r1, r1, #3
	adds r2, r6, #0
	str r3, [sp, #0]
	str r5, [sp, #4]
	subs r3, r4, r5
	ldr r0, [sp, #16]
	add r1, r10
	mov lr, r12
	.2byte 0xf800
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r6, #14
	cmp r3, #2
	bne .L_0816a8c4
.L_0816a91c:
	ldr r4, [sp, #20]
	ldr r1, [sp, #12]
	ldr r0, [r4, #4]
	bl Func_08144aac
	movs r2, #0
	mov r8, r2
	mov r5, r10
.L_0816a92c:
	mov r4, r8
	lsrs r3, r4, #31
	add r3, r8
	asrs r3, r3, #1
	adds r3, #8
	cmp r9, r3
	blt .L_0816a988
	ldr r0, [r5, #24]
	cmp r0, #28
	bgt .L_0816a988
	movs r1, #3
	bl __divsi3
	movs r2, #2
	ldrsh r4, [r5, r2]
	movs r3, #6
	ldrsh r6, [r5, r3]
	cmp r0, #6
	ble .L_0816a954
	movs r0, #6
.L_0816a954:
	ldr r3, .L_0816a9fc
	lsls r2, r0, #1
	ldrh r1, [r3, r2]
	movs r3, #182
	lsls r3, r3, #4
	add r1, r10
	adds r1, r1, r3
	ldr r3, .L_0816aa00
	ldrh r0, [r3, r2]
	lsrs r3, r0, #1
	subs r2, r4, r3
	str r0, [sp, #0]
	subs r3, r6, r3
	str r0, [sp, #4]
	ldr r4, [sp, #24]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #24]
	adds r0, r5, #0
	adds r3, #1
	str r3, [r5, #24]
	movs r1, #62
	ldr r2, .L_0816aa04
	bl BattleFxKernels_IntegrateVector2
.L_0816a988:
	movs r4, #1
	add r8, r4
	mov r2, r8
	adds r5, #28
	cmp r2, #16
	bne .L_0816a92c
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	mov r3, r9
	cmp r3, #7
	bgt .L_0816a9b0
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	b .L_0816a9b8
.L_0816a9b0:
	movs r0, #16
	movs r1, #16
	bl Func_08158ce0
.L_0816a9b8:
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r10
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #8]
	movs r2, #1
	add r9, r2
	adds r4, #6
	mov r3, r9
	str r4, [sp, #8]
	cmp r3, #54
	beq .L_0816a9e0
	b .L_0816a854
.L_0816a9e0:
	ldr r0, .L_0816a9f8
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816a9f8:
	.4byte Func_08143000
.L_0816a9fc:
	.4byte Data_08198afc
.L_0816aa00:
	.4byte Data_08198b0a
.L_0816aa04:
	.4byte 0xffffe000
