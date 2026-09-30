.syntax unified
	.thumb
	.global Func_081665a4
	.thumb_func
Func_081665a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r11, r0
	ldr r0, [r3, #92]
	sub sp, #76
	str r0, [sp, #52]
	mov r2, r11
	ldr r1, [r3, #96]
	str r1, [sp, #48]
	ldr r3, [r3, #100]
	str r3, [sp, #40]
	ldr r0, [r2, #8]
	bl Func_08118088 + 0x10
	ldr r0, [r0]
	str r0, [sp, #36]
	movs r0, #1
	bl Func_081435e0
	ldr r3, .L_08166618
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	mov r5, sp
	strh r3, [r2]
	adds r5, #56
	mov r3, r11
	ldr r0, [r3, #4]
	adds r1, r5, #0
	str r5, [sp, #28]
	bl Func_08144aac
	movs r1, #2
	ldr r0, [sp, #36]
	bl Object_SetMode
	movs r1, #48
	ldr r0, [sp, #36]
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r1, r11
	add r5, sp, #64
	movs r6, #36
	ldrsh r0, [r1, r6]
	adds r1, r5, #0
	bl Func_0815e21c
	mov r2, r11
	ldr r3, [r2, #4]
	b .L_0816661c
	.2byte 0x0000
.L_08166618:
	.4byte 0x00001010
.L_0816661c:
	cmp r3, #0
	bne .L_0816662c
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r3, #24
	b .L_08166636
.L_0816662c:
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r3, #92
.L_08166636:
	str r3, [sp, #32]
	ldr r3, [sp, #52]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r3, r5
	ldr r0, .L_0816699c
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r6, [sp, #52]
	movs r2, #156
	lsls r2, r2, #6
	adds r1, r6, r2
	ldr r0, .L_081669a0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r3, #0
	ldr r0, .L_081669a4
	ldr r1, [sp, #40]
	movs r2, #0
	bl Func_08157cf4
	movs r3, #0
	str r3, [sp, #44]
	str r3, [sp, #8]
	mov r8, r3
.L_08166670:
	ldr r6, [sp, #8]
	ldr r0, [sp, #52]
	movs r5, #0
	mov r10, r5
	adds r7, r6, r0
.L_0816667a:
	mov r1, r10
	lsls r6, r1, #1
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	adds r0, r5, #0
	str r3, [r7]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	mov r2, r10
	negs r3, r3
	str r3, [r7, #4]
	lsrs r3, r2, #31
	add r3, r10
	asrs r3, r3, #1
	adds r3, #25
	str r3, [r7, #24]
	movs r3, #1
	add r10, r3
	mov r5, r10
	adds r7, #28
	cmp r5, #16
	bne .L_0816667a
	mov r1, r8
	ldr r0, .L_081669a8
	lsls r3, r1, #3
	ldr r2, .L_081669ac
	subs r3, r3, r1
	movs r6, #0
	lsls r3, r3, #2
	mov r10, r6
	mov r9, r0
	adds r7, r3, r2
.L_081666d2:
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r6, r0, #0
	adds r3, #255
	ands r6, r3
	mov r3, r11
	ldr r2, [r3, #4]
	ldr r0, [sp, #44]
	lsls r3, r2, #1
	adds r3, r3, r2
	mov r1, r9
	adds r3, r0, r3
	ldr r2, [sp, #32]
	ldrb r3, [r1, r3]
	adds r0, r6, #0
	adds r3, r3, r2
	lsls r3, r3, #16
	str r3, [r7]
	movs r3, #176
	lsls r3, r3, #15
	str r3, [r7, #4]
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r5, #170
	movs r3, #1
	add r10, r3
	lsls r5, r5, #1
	adds r7, #28
	cmp r10, r5
	bne .L_081666d2
	ldr r6, [sp, #8]
	ldr r1, [sp, #44]
	movs r0, #224
	lsls r0, r0, #1
	adds r6, r6, r0
	adds r1, #1
	add r8, r5
	str r6, [sp, #8]
	str r1, [sp, #44]
	cmp r1, #3
	bne .L_08166670
	ldr r3, [sp, #52]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2]
	ldr r6, [sp, #52]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r6, r0
	movs r3, #75
	movs r1, #200
	lsls r1, r1, #4
	str r3, [r2]
	ldr r0, .L_081669b0
	bl Func_080145a8
	movs r1, #0
	mov r9, r1
.L_08166784:
	mov r2, r9
	cmp r2, #4
	bne .L_08166790
	movs r0, #212
	bl Audio_PlayCue
.L_08166790:
	mov r3, r9
	cmp r3, #8
	bne .L_081667a4
	ldr r5, [sp, #52]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #168
	adds r3, r5, r6
	mov r0, r9
	str r0, [r3]
.L_081667a4:
	mov r1, r9
	cmp r1, #18
	bne .L_081667b0
	movs r0, #145
	bl Audio_PlayCue
.L_081667b0:
	mov r2, r9
	cmp r2, #40
	bne .L_081667bc
	movs r0, #134
	bl Func_08118088 + 0x60
.L_081667bc:
	mov r3, r9
	cmp r3, #39
	bgt .L_08166858
	mov r5, r11
	ldr r3, [r5, #4]
	movs r1, #128
	cmp r3, #1
	bne .L_081667fa
	mov r6, r9
	cmp r6, #9
	bgt .L_081667e4
	lsls r3, r6, #2
	add r3, r9
	lsls r3, r3, #1
	adds r2, r3, #0
	lsls r3, r6, #4
	adds r5, r3, #0
	subs r2, #8
	subs r5, #128
	b .L_08166828
.L_081667e4:
	mov r0, r9
	cmp r0, #20
	ble .L_081667f6
	lsls r3, r0, #1
	mov r2, r9
	adds r5, r3, #0
	adds r2, #62
	subs r5, #24
	b .L_08166828
.L_081667f6:
	movs r2, #82
	b .L_08166826
.L_081667fa:
	mov r2, r9
	cmp r2, #9
	bgt .L_08166812
	lsls r3, r2, #2
	add r3, r9
	lsls r3, r3, #1
	mov r5, r9
	subs r2, r1, r3
	lsls r3, r5, #4
	adds r5, r3, #0
	subs r5, #128
	b .L_08166828
.L_08166812:
	mov r6, r9
	cmp r6, #20
	ble .L_08166824
	movs r3, #58
	subs r2, r3, r6
	lsls r3, r6, #1
	adds r5, r3, #0
	subs r5, #24
	b .L_08166828
.L_08166824:
	movs r2, #38
.L_08166826:
	movs r5, #16
.L_08166828:
	adds r3, r5, #0
	adds r3, #128
	cmp r3, #104
	ble .L_08166836
	subs r3, r1, r5
	adds r1, r3, #0
	subs r1, #24
.L_08166836:
	cmp r1, #0
	ble .L_08166858
	ldr r0, [sp, #32]
	movs r3, #64
	str r3, [sp, #0]
	ldr r3, [sp, #52]
	movs r6, #224
	adds r2, r2, r0
	lsls r6, r6, #3
	str r1, [sp, #4]
	subs r2, #32
	adds r1, r3, r6
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_08166858:
	mov r0, r9
	cmp r0, #16
	ble .L_08166864
	ldr r0, .L_081669b4
	bl Func_0815f0a0
.L_08166864:
	movs r1, #0
	movs r2, #22
	movs r3, #16
	str r1, [sp, #44]
	str r2, [sp, #24]
	str r1, [sp, #20]
	str r3, [sp, #16]
	str r1, [sp, #12]
.L_08166874:
	ldr r5, [sp, #44]
	ldr r6, [sp, #16]
	lsls r1, r5, #3
	cmp r9, r6
	bne .L_0816688c
	ldr r0, [sp, #52]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r0, r3
	movs r3, #12
	str r3, [r2]
.L_0816688c:
	ldr r5, [sp, #16]
	cmp r9, r5
	blt .L_0816694c
	adds r3, r1, #0
	adds r3, #18
	cmp r9, r3
	bge .L_081668ca
	mov r6, r11
	ldr r2, [r6, #4]
	ldr r0, [sp, #44]
	ldr r1, .L_081669a8
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r3, r0, r3
	ldrb r2, [r1, r3]
	movs r3, #32
	ldr r1, [sp, #32]
	str r3, [sp, #0]
	movs r3, #64
	str r3, [sp, #4]
	ldr r3, [sp, #52]
	movs r5, #156
	adds r2, r2, r1
	lsls r5, r5, #6
	adds r1, r3, r5
	subs r2, #16
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	movs r3, #56
	mov lr, r4
	.2byte 0xf800
.L_081668ca:
	ldr r1, [sp, #12]
	ldr r0, .L_081669b8
	lsls r3, r1, #3
	ldr r2, [sp, #52]
	subs r3, r3, r1
	movs r6, #0
	lsls r3, r3, #2
	mov r10, r6
	mov r8, r0
	adds r5, r3, r2
.L_081668de:
	mov r0, r11
	ldr r2, [r0, #4]
	movs r3, #6
	ldrsh r7, [r5, r3]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #44]
	movs r6, #2
	ldrsh r1, [r5, r6]
	ldr r6, .L_081669a8
	adds r3, r2, r3
	ldrb r3, [r6, r3]
	ldr r0, [sp, #32]
	adds r1, r1, r3
	adds r6, r1, r0
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_08166932
	movs r1, #3
	bl __divsi3
	mov r2, r8
	ldrb r1, [r2, r0]
	ldr r3, [sp, #52]
	lsls r1, r1, #11
	movs r0, #156
	adds r1, r3, r1
	lsls r0, r0, #6
	adds r1, r1, r0
	movs r0, #32
	str r0, [sp, #0]
	adds r2, r6, #0
	movs r0, #64
	adds r3, r7, #0
	str r0, [sp, #4]
	subs r2, #16
	adds r3, #56
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_08166932:
	cmp r0, #0
	ble .L_0816693a
	subs r3, r0, #1
	b .L_0816693e
.L_0816693a:
	movs r3, #1
	negs r3, r3
.L_0816693e:
	str r3, [r5, #24]
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #28
	cmp r2, #12
	bne .L_081668de
.L_0816694c:
	ldr r3, [sp, #16]
	adds r3, #5
	cmp r9, r3
	ble .L_08166a16
	ldr r7, [sp, #20]
	movs r3, #0
	mov r10, r3
.L_0816695a:
	lsls r3, r7, #4
	adds r3, r7, r3
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #3
	ldr r5, .L_081669ac
	subs r2, r2, r3
	lsls r2, r2, #2
	adds r6, r2, r5
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_08166a0a
	movs r2, #128
	adds r0, r6, #0
	movs r1, #64
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #24]
	ldr r1, [r6, #4]
	movs r0, #216
	subs r3, #1
	lsls r0, r0, #15
	str r3, [r6, #24]
	cmp r1, r0
	ble .L_081669bc
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_08166a0a
.L_0816699c:
	.4byte 0x00000117
.L_081669a0:
	.4byte 0x0000013e
.L_081669a4:
	.4byte 0x00000134
.L_081669a8:
	.4byte Data_08198a92
.L_081669ac:
	.4byte gMapCellBuffer
.L_081669b0:
	.4byte Func_08143000
.L_081669b4:
	.4byte 0x00000184
.L_081669b8:
	.4byte Data_08198a98
.L_081669bc:
	ldr r0, [r6]
	ldr r2, .L_08166aec
	cmp r0, r2
	bhi .L_08166a0a
	cmp r1, #0
	blt .L_08166a0a
	asrs r1, r1, #16
	mov r8, r1
	asrs r6, r0, #16
	movs r1, #5
	adds r0, r3, #0
	bl __divsi3
	ldr r2, .L_08166af0
	adds r0, #1
	lsls r5, r0, #1
	mov r3, r10
	movs r4, #1
	ands r4, r3
	subs r3, r5, #2
	ldrh r1, [r2, r3]
	lsrs r3, r0, #31
	ldr r2, [sp, #40]
	adds r3, r0, r3
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r5, [sp, #28]
	asrs r3, r3, #1
	subs r6, r6, r3
	lsls r4, r4, #2
	mov r3, r8
	subs r3, r3, r0
	adds r1, r2, r1
	ldr r4, [r4, r5]
	ldr r0, [sp, #48]
	adds r2, r6, #0
	mov r8, r3
	mov lr, r4
	.2byte 0xf800
.L_08166a0a:
	movs r6, #1
	movs r0, #128
	add r10, r6
	lsls r0, r0, #1
	cmp r10, r0
	bne .L_0816695a
.L_08166a16:
	mov r2, r11
	ldr r3, [r2, #20]
	movs r1, #0
	mov r10, r1
	cmp r3, #0
	beq .L_08166a6e
	ldr r3, [sp, #24]
	mov r5, r11
	mov r8, r3
	movs r6, #10
	adds r5, #36
.L_08166a2c:
	ldr r0, [sp, #16]
	cmp r9, r0
	bne .L_08166a42
	movs r1, #0
	ldrsh r0, [r5, r1]
	movs r2, #5
	movs r1, #7
	mov r3, r10
	str r6, [sp, #0]
	bl Func_0814cd48
.L_08166a42:
	cmp r9, r8
	bne .L_08166a60
	movs r2, #0
	ldrsh r0, [r5, r2]
	movs r1, #7
	mov r3, r10
	movs r2, #5
	str r6, [sp, #0]
	bl Func_0814cd48
	movs r3, #0
	ldrsh r0, [r5, r3]
	movs r1, #4
	bl Func_08118088
.L_08166a60:
	mov r1, r11
	ldr r3, [r1, #20]
	movs r0, #1
	add r10, r0
	adds r5, #2
	cmp r10, r3
	bne .L_08166a2c
.L_08166a6e:
	ldr r2, [sp, #24]
	ldr r3, [sp, #20]
	ldr r5, [sp, #16]
	ldr r6, [sp, #12]
	ldr r0, [sp, #44]
	adds r2, #8
	adds r3, #5
	adds r5, #8
	adds r6, #16
	adds r0, #1
	str r2, [sp, #24]
	str r3, [sp, #20]
	str r5, [sp, #16]
	str r6, [sp, #12]
	str r0, [sp, #44]
	cmp r0, #2
	beq .L_08166a92
	b .L_08166874
.L_08166a92:
	movs r0, #16
	movs r1, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	ldr r1, [sp, #52]
	lsls r2, r2, #7
	adds r2, #232
	adds r3, r1, r2
	movs r5, #1
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r9, r3
	mov r5, r9
	cmp r5, #80
	beq .L_08166abe
	b .L_08166784
.L_08166abe:
	ldr r0, [sp, #36]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r0, .L_08166af4
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08166aec:
	.4byte 0x007effff
.L_08166af0:
	.4byte Data_08197410
.L_08166af4:
	.4byte Func_08143000
