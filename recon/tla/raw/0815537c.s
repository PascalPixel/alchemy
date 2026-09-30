.syntax unified
	.thumb
	.global Func_0815537c
	.thumb_func
Func_0815537c:
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
	sub sp, #64
	str r0, [sp, #48]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #44]
	ldr r2, [r5, #100]
	str r2, [sp, #24]
	bl Func_081435e0
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r3, [sp, #48]
	ldr r5, [r5, #104]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_08155620
	movs r2, #1
	movs r3, #0
	str r5, [sp, #28]
	bl Func_08157cf4
	movs r3, #152
	ldr r2, [sp, #48]
	lsls r3, r3, #5
	adds r3, #86
	adds r1, r2, r3
	ldr r0, .L_08155624
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #24]
	movs r3, #0
	ldr r0, .L_08155628
	bl Func_08157cf4
	ldr r3, .L_0815562c
	movs r4, #0
	movs r2, #128
	mov r9, r4
	movs r1, #0
	lsls r2, r2, #3
.L_081553ee:
	movs r0, #1
	add r9, r0
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_081553ee
	ldr r3, [sp, #48]
	movs r1, #0
	movs r2, #1
	mov r9, r1
	negs r2, r2
	adds r3, #24
.L_08155406:
	movs r4, #1
	add r9, r4
	mov r0, r9
	str r2, [r3]
	adds r3, #28
	cmp r0, #64
	bne .L_08155406
	ldr r1, [sp, #48]
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
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08155630
	bl Func_080145a8
	movs r0, #138
	bl Audio_PlayCue
	movs r0, #0
	str r0, [sp, #36]
	mov r1, r11
	ldr r2, [r1, #20]
	movs r4, #40
	lsls r3, r2, #3
	negs r4, r4
	cmp r3, r4
	bne .L_08155450
	b .L_08155814
.L_08155450:
	ldr r0, [sp, #36]
	cmp r0, #24
	bne .L_08155460
	movs r0, #133
	bl Func_081180e8
	mov r1, r11
	ldr r2, [r1, #20]
.L_08155460:
	movs r3, #0
	str r3, [sp, #40]
	cmp r2, #0
	beq .L_0815548e
	ldr r5, .L_08155634
.L_0815546a:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #3
	cmp r0, r3
	bne .L_08155484
	movs r1, #128
	lsls r1, r1, #7
	ldr r2, .L_08155638
	ldr r0, [sp, #44]
	mov lr, r5
	.2byte 0xf800
	mov r1, r11
	ldr r2, [r1, #20]
.L_08155484:
	ldr r3, [sp, #40]
	adds r3, #1
	str r3, [sp, #40]
	cmp r3, r2
	bne .L_0815546a
.L_0815548e:
	movs r4, #0
	str r4, [sp, #40]
	cmp r2, #0
	bne .L_08155498
	b .L_0815575e
.L_08155498:
	mov r0, sp
	adds r0, #52
	movs r1, #36
	str r0, [sp, #20]
	str r1, [sp, #16]
	str r4, [sp, #12]
.L_081554a4:
	ldr r3, [sp, #16]
	ldr r2, [sp, #40]
	mov r1, r11
	ldrsh r0, [r3, r1]
	lsls r2, r2, #3
	ldr r1, [sp, #20]
	mov r8, r2
	bl Func_0815e21c
	ldr r2, [sp, #20]
	ldr r4, [sp, #20]
	ldr r3, [r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r4]
	ldr r0, [sp, #36]
	mov r3, r8
	adds r3, #1
	cmp r0, r3
	bne .L_081554dc
	ldr r1, [sp, #48]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #4
	str r3, [r2]
.L_081554dc:
	ldr r4, [sp, #36]
	mov r3, r8
	adds r3, #4
	cmp r4, r3
	bne .L_08155506
	ldr r1, [sp, #16]
	mov r3, r11
	ldrsh r0, [r1, r3]
	movs r3, #6
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	ldr r3, [sp, #40]
	bl Func_0814cd48
	ldr r4, [sp, #16]
	mov r2, r11
	ldrsh r0, [r4, r2]
	movs r1, #6
	bl Func_08118088
.L_08155506:
	ldr r4, [sp, #36]
	movs r3, #2
	add r3, r8
	mov r10, r3
	cmp r4, r8
	bge .L_08155514
	b .L_0815564e
.L_08155514:
	mov r3, r8
	adds r3, #16
	cmp r4, r3
	blt .L_0815551e
	b .L_08155648
.L_0815551e:
	mov r0, r8
	subs r3, r4, r0
	lsls r5, r3, #6
	cmp r5, #104
	ble .L_0815552a
	movs r5, #104
.L_0815552a:
	mov r3, r11
	ldr r2, [r3, #24]
	ldr r6, .L_0815563c
	lsls r3, r2, #2
	adds r3, #3
	ldrb r3, [r6, r3]
	movs r1, #0
	mov r9, r1
	cmp r3, #0
	beq .L_08155594
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	mov r10, r6
	adds r7, r4, r0
.L_08155546:
	mov r1, r9
	adds r3, r7, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r2, #3
	ands r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r1, r2, #4
	ldr r4, [sp, #20]
	subs r1, r1, r2
	ldr r2, [sp, #48]
	lsls r1, r1, #6
	movs r3, #152
	adds r1, r2, r1
	lsls r3, r3, #5
	ldr r2, [r4]
	adds r3, #86
	adds r1, r1, r3
	movs r3, #24
	subs r2, #12
	str r3, [sp, #0]
	ldr r4, [sp, #28]
	movs r3, #0
	str r5, [sp, #4]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	mov r1, r11
	ldr r2, [r1, #24]
	mov r4, r10
	lsls r3, r2, #2
	adds r3, #3
	ldrb r3, [r4, r3]
	movs r0, #1
	add r9, r0
	cmp r9, r3
	bne .L_08155546
.L_08155594:
	movs r0, #2
	ldr r1, [sp, #36]
	add r0, r8
	mov r10, r0
	cmp r1, r10
	bne .L_0815564e
	movs r3, #0
	mov r9, r3
	lsls r3, r2, #2
	ldrb r3, [r6, r3]
	cmp r3, #0
	beq .L_0815564e
	ldr r4, [sp, #12]
	ldr r0, .L_08155640
	adds r7, r4, r0
.L_081555b2:
	bl Random16
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r6, r0
	bl Random16
	ldr r2, [sp, #20]
	movs r5, #254
	ldr r3, [r2]
	ldr r1, .L_08155644
	lsls r5, r5, #7
	lsls r3, r3, #16
	adds r5, #255
	str r3, [r7]
	ands r5, r0
	movs r3, #208
	adds r5, r5, r1
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #64
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	mov r4, r11
	movs r3, #1
	add r9, r3
	ldr r3, [r4, #24]
	ldr r0, .L_0815563c
	lsls r3, r3, #2
	ldrb r3, [r0, r3]
	adds r7, #28
	cmp r9, r3
	bne .L_081555b2
	b .L_0815564e
	.2byte 0x0000
.L_08155620:
	.4byte 0x00000192
.L_08155624:
	.4byte 0x00000188
.L_08155628:
	.4byte 0x00000134
.L_0815562c:
	.4byte Data_02010018
.L_08155630:
	.4byte Func_08143000
.L_08155634:
	.4byte IwramFillWords
.L_08155638:
	.4byte 0x10101010
.L_0815563c:
	.4byte Data_08198462
.L_08155640:
	.4byte gMapCellBuffer
.L_08155644:
	.4byte 0xffffc000
.L_08155648:
	movs r1, #2
	add r1, r8
	mov r10, r1
.L_0815564e:
	ldr r2, [sp, #36]
	cmp r2, r10
	blt .L_0815573e
	mov r3, r8
	adds r3, #24
	cmp r2, r3
	bge .L_0815573e
	movs r3, #0
	mov r4, r11
	mov r9, r3
	ldr r3, [r4, #24]
	ldr r0, .L_08155834
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r0, r3]
	cmp r3, #0
	beq .L_0815573e
	ldr r7, [sp, #20]
.L_08155672:
	movs r1, #3
	mov r4, r9
	ands r4, r1
	str r4, [sp, #8]
	bl Random16
	mov r2, r11
	ldr r3, [r2, #24]
	ldr r1, .L_08155834
	lsls r3, r3, #2
	adds r3, #2
	ldrb r5, [r1, r3]
	adds r1, r5, #0
	bl Math_ModU
	ldr r2, [r7, #4]
	ldr r4, [sp, #8]
	mov r8, r2
	mov r3, r8
	subs r3, r3, r0
	subs r5, r5, r0
	ldr r0, .L_08155838
	mov r8, r3
	ldrb r3, [r0, r4]
	mov r1, r8
	lsrs r3, r3, #1
	subs r1, r1, r3
	movs r2, #8
	adds r5, #1
	mov r8, r1
	add r8, r2
	mov r10, r0
	bl Random16
	adds r1, r5, #0
	bl Math_ModU
	ldr r6, [r7]
	lsrs r3, r5, #31
	adds r5, r5, r3
	adds r6, r6, r0
	asrs r5, r5, #1
	ldr r4, [sp, #8]
	subs r6, r6, r5
	ldr r5, .L_0815583c
	ldrb r3, [r5, r4]
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl Random16
	ldr r3, .L_08155840
	movs r1, #3
	ands r0, r1
	ldrb r2, [r3, r0]
	mov r0, r11
	movs r3, #3
	orrs r3, r2
	ldr r1, .L_08155844
	ldr r2, [r0, #24]
	movs r0, #188
	ldrb r2, [r1, r2]
	movs r1, #7
	str r2, [sp, #0]
	movs r2, #7
	bl Func_08196404
	ldr r4, [sp, #8]
	ldr r2, .L_08155848
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #48]
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	adds r1, r1, r3
	ldrb r3, [r5, r4]
	mov r0, r10
	str r3, [sp, #0]
	movs r2, #192
	ldrb r3, [r0, r4]
	lsls r2, r2, #18
	str r3, [sp, #4]
	adds r2, #188
	ldr r4, [r2]
	mov r3, r8
	ldr r0, [sp, #44]
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	mov r4, r11
	add r9, r3
	ldr r3, [r4, #24]
	ldr r0, .L_08155834
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r0, r3]
	cmp r9, r3
	bne .L_08155672
.L_0815573e:
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	ldr r4, [sp, #40]
	movs r3, #224
	lsls r3, r3, #4
	adds r2, r2, r3
	adds r1, #2
	adds r4, #1
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r4, [sp, #40]
	mov r0, r11
	ldr r3, [r0, #20]
	cmp r4, r3
	beq .L_0815575e
	b .L_081554a4
.L_0815575e:
	ldr r6, .L_0815584c
	movs r1, #0
	mov r9, r1
.L_08155764:
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_081557d0
	subs r3, #1
	movs r2, #128
	str r3, [r6, #24]
	lsls r2, r2, #5
	adds r0, r6, #0
	movs r1, #60
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #4]
	movs r2, #208
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_08155792
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_081557d0
.L_08155792:
	ldr r2, [r6]
	ldr r4, .L_08155850
	cmp r2, r4
	bhi .L_081557d0
	cmp r3, #0
	blt .L_081557d0
	ldr r4, [r6, #24]
	cmp r4, #0
	bge .L_081557a6
	adds r4, #15
.L_081557a6:
	asrs r4, r4, #4
	adds r4, #1
	ldr r0, .L_08155854
	lsls r5, r4, #1
	subs r1, r5, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #24]
	asrs r2, r2, #16
	adds r1, r0, r1
	lsrs r0, r4, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	asrs r3, r3, #16
	subs r2, r2, r0
	subs r3, r3, r4
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [sp, #28]
	mov lr, r4
	.2byte 0xf800
.L_081557d0:
	movs r0, #1
	movs r1, #128
	add r9, r0
	lsls r1, r1, #3
	adds r6, #28
	cmp r9, r1
	bne .L_08155764
	movs r1, #8
	movs r0, #2
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #48]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #36]
	mov r1, r11
	adds r0, #1
	str r0, [sp, #36]
	ldr r3, [r1, #20]
	adds r2, r3, #0
	lsls r3, r2, #3
	adds r3, #40
	cmp r0, r3
	beq .L_08155814
	b .L_08155450
.L_08155814:
	ldr r0, .L_08155858
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08155834:
	.4byte Data_08198462
.L_08155838:
	.4byte Data_08197498
.L_0815583c:
	.4byte Data_08197492
.L_08155840:
	.4byte Data_0819846e
.L_08155844:
	.4byte Data_08198472
.L_08155848:
	.4byte Data_08197486
.L_0815584c:
	.4byte gMapCellBuffer
.L_08155850:
	.4byte 0x007effff
.L_08155854:
	.4byte Data_08197410
.L_08155858:
	.4byte Func_08143000
