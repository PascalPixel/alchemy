.syntax unified
	.thumb
	.global Func_08178710
	.thumb_func
Func_08178710:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #164
	str r0, [sp, #84]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #80]
	movs r0, #1
	ldr r1, [r3, #96]
	str r1, [sp, #76]
	ldr r2, [r3, #48]
	str r2, [sp, #60]
	ldr r3, [r3, #100]
	str r3, [sp, #56]
	bl Func_081435e0
	ldr r4, [sp, #84]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0817874e
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	b .L_08178756
.L_0817874e:
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
.L_08178756:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r2, #128
	str r3, [sp, #68]
	ldr r3, .L_0817879c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r7, [sp, #80]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r7, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r7, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_081787a0
	bl Func_080145a8
	movs r3, #0
	movs r4, #0
	str r3, [sp, #104]
	str r4, [sp, #108]
	str r3, [sp, #96]
	str r4, [sp, #100]
	ldr r2, [sp, #84]
	ldr r0, [r2, #8]
	b .L_081787a4
.L_0817879c:
	.4byte 0x00001010
.L_081787a0:
	.4byte Func_08143000
.L_081787a4:
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r1, #48
	str r0, [sp, #52]
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r1, #2
	ldr r0, [sp, #52]
	bl Object_SetMode
	ldr r3, [sp, #84]
	mov r4, sp
	adds r4, #136
	ldr r0, [r3, #8]
	adds r1, r4, #0
	str r4, [sp, #48]
	bl Func_0815e21c
	ldr r1, [sp, #84]
	mov r2, sp
	adds r2, #124
	movs r7, #36
	ldrsh r0, [r1, r7]
	adds r1, r2, #0
	str r2, [sp, #44]
	bl Func_0815e21c
	ldr r3, [sp, #80]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_0817885c
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_08178860
	ldr r1, [sp, #56]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r7, [sp, #80]
	movs r2, #142
	lsls r2, r2, #7
	adds r1, r7, r2
	movs r3, #1
	movs r2, #1
	ldr r0, .L_08178864
	bl Func_08157cf4
	ldr r0, .L_08178868
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817886c
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_08178858
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r0, .L_08178870
	movs r2, #32
	movs r3, #32
	movs r1, #4
	bl Func_08178680
	movs r3, #0
	str r3, [sp, #64]
	ldr r4, [sp, #84]
	movs r7, #60
	ldr r2, [r4, #24]
	negs r7, r7
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	cmp r3, r7
	bne .L_0817884e
	b .L_08178ea4
.L_0817884e:
	ldr r0, [sp, #60]
	adds r0, #12
	str r0, [sp, #32]
	b .L_08178874
	.2byte 0x0000
.L_08178858:
	.4byte 0x00000080
.L_0817885c:
	.4byte 0x000000f4
.L_08178860:
	.4byte 0x00000134
.L_08178864:
	.4byte 0x000000e8
.L_08178868:
	.4byte 0x00000130
.L_0817886c:
	.4byte IwramCopyWords
.L_08178870:
	.4byte Data_02012000
.L_08178874:
	bl Func_08014de4
	ldr r1, [sp, #32]
	ldr r0, [sp, #60]
	bl Func_080156e8
	ldr r1, [sp, #64]
	cmp r1, #0
	bne .L_081788a8
	add r3, sp, #148
	str r1, [r3]
	str r1, [r3, #4]
	str r1, [r3, #8]
	str r1, [r3, #12]
	ldr r3, [sp, #80]
	movs r2, #0
	mov r10, r2
	adds r3, #24
	subs r2, #1
.L_0817889a:
	movs r4, #1
	add r10, r4
	mov r7, r10
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_0817889a
.L_081788a8:
	ldr r2, [sp, #84]
	movs r3, #1
	ldr r1, [r2, #24]
	movs r0, #0
	negs r3, r3
	mov r10, r0
	cmp r1, r3
	bne .L_081788ba
	b .L_081789ec
.L_081788ba:
	movs r4, #28
	str r4, [sp, #20]
	mov r11, r0
.L_081788c0:
	ldr r7, [sp, #64]
	ldr r0, [sp, #20]
	cmp r7, r0
	beq .L_081788ca
	b .L_081789d4
.L_081788ca:
	ldr r2, [sp, #84]
	add r5, sp, #112
	movs r1, #36
	ldrsh r0, [r2, r1]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r7, [sp, #80]
	movs r3, #0
	mov r9, r3
	mov r8, r5
	add r7, r11
.L_081788e2:
	mov r4, r8
	ldr r3, [r4]
	movs r5, #255
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r4, #4]
	subs r3, #16
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	adds r6, r0, #0
	bl Random16
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
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
	movs r0, #1
	add r9, r0
	adds r3, #16
	mov r1, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #32
	bne .L_081788e2
	movs r1, #128
	ldr r3, .L_08178990
	ldr r2, .L_08178994
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	movs r4, #238
	ldr r3, [sp, #80]
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2]
	ldr r7, [sp, #64]
	cmp r7, #28
	bne .L_08178998
	ldr r0, [sp, #84]
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_0817896e
	movs r0, #144
	bl Func_081180e8
	b .L_08178974
.L_0817896e:
	movs r0, #144
	bl Audio_PlayCue
.L_08178974:
	ldr r2, [sp, #84]
	movs r3, #128
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r2, #150
	str r2, [sp, #4]
	movs r2, #128
	lsls r3, r3, #11
	movs r1, #1
	lsls r2, r2, #10
	str r3, [sp, #0]
	bl Func_0815f000
	b .L_081789bc
.L_08178990:
	.4byte IwramFillWords
.L_08178994:
	.4byte 0x3f3f3f3f
.L_08178998:
	movs r0, #144
	bl Func_081180e8
	ldr r4, [sp, #84]
	movs r2, #128
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #224
	lsls r3, r3, #11
	str r3, [sp, #0]
	movs r3, #200
	str r3, [sp, #4]
	movs r3, #128
	movs r1, #1
	lsls r2, r2, #10
	lsls r3, r3, #11
	bl Func_0815f000
.L_081789bc:
	ldr r1, [sp, #84]
	movs r3, #8
	movs r7, #36
	ldrsh r0, [r1, r7]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	ldr r2, [sp, #84]
	ldr r1, [r2, #24]
.L_081789d4:
	ldr r3, [sp, #20]
	movs r4, #224
	adds r3, #10
	movs r7, #1
	str r3, [sp, #20]
	lsls r4, r4, #2
	add r10, r7
	adds r3, r1, #1
	add r11, r4
	cmp r10, r3
	beq .L_081789ec
	b .L_081788c0
.L_081789ec:
	ldr r6, .L_08178d44
	ldr r5, [sp, #80]
	movs r0, #0
	mov r10, r0
.L_081789f4:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08178a38
	asrs r0, r0, #3
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #56]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #76]
	ldr r4, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08178a38:
	movs r7, #1
	add r10, r7
	mov r0, r10
	adds r5, #28
	cmp r0, #64
	bne .L_081789f4
	ldr r1, [sp, #64]
	cmp r1, #15
	bgt .L_08178a4c
	b .L_08178e6c
.L_08178a4c:
	movs r3, #44
	adds r4, r1, #0
	adds r1, r4, #0
	muls r1, r3
	ldr r7, .L_08178d48
	add r2, sp, #148
	adds r3, r1, r7
	mov r11, r2
	str r3, [r2]
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #182
	cmp r3, r2
	ble .L_08178a6c
	mov r0, r11
	str r2, [r0]
.L_08178a6c:
	ldr r3, [sp, #64]
	cmp r3, #31
	ble .L_08178a80
	ldr r4, .L_08178d4c
	mov r7, r11
	adds r3, r1, r4
	str r3, [r7, #4]
	cmp r3, r2
	ble .L_08178a80
	str r2, [r7, #4]
.L_08178a80:
	ldr r0, [sp, #64]
	cmp r0, #29
	ble .L_08178aa0
	movs r3, #44
	muls r3, r0
	ldr r1, .L_08178d50
	mov r2, r11
	adds r3, r3, r1
	str r3, [r2, #8]
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #182
	cmp r3, r2
	ble .L_08178aa0
	mov r3, r11
	str r2, [r3, #8]
.L_08178aa0:
	ldr r4, [sp, #64]
	cmp r4, #45
	ble .L_08178abe
	movs r3, #44
	muls r3, r4
	ldr r7, .L_08178d54
	movs r2, #192
	lsls r2, r2, #2
	adds r3, r3, r7
	mov r0, r11
	adds r2, #182
	str r3, [r0, #12]
	cmp r3, r2
	ble .L_08178abe
	str r2, [r0, #12]
.L_08178abe:
	movs r1, #0
	str r1, [sp, #24]
	mov r9, r1
.L_08178ac4:
	ldr r2, [sp, #48]
	ldr r3, [r2]
	cmp r3, #0
	bge .L_08178ace
	adds r3, #3
.L_08178ace:
	ldr r4, [sp, #44]
	asrs r0, r3, #2
	ldr r3, [r4]
	cmp r3, #0
	bge .L_08178ada
	adds r3, #3
.L_08178ada:
	asrs r3, r3, #2
	mov r7, r11
	ldr r2, [r7, #4]
	subs r4, r0, r3
	ldr r3, [r7]
	subs r3, r3, r2
	mov r1, r9
	muls r1, r3
	lsls r3, r2, #5
	adds r3, r3, r2
	adds r1, r1, r3
	mov r8, r1
	cmp r4, #0
	bge .L_08178af8
	negs r4, r4
.L_08178af8:
	adds r0, r4, #0
	movs r1, #3
	str r4, [sp, #8]
	bl Math_Div
	ldr r1, [sp, #24]
	str r0, [sp, #40]
	adds r6, r1, #0
	adds r6, #16
	ldr r4, [sp, #8]
	cmp r6, #32
	ble .L_08178b12
	movs r6, #32
.L_08178b12:
	movs r0, #162
	lsls r0, r0, #7
	adds r0, #32
	adds r3, r0, #0
	muls r3, r6
	cmp r3, #0
	bge .L_08178b28
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_08178b28:
	asrs r6, r3, #16
	lsrs r3, r3, #31
	adds r3, r6, r3
	ldr r0, [sp, #40]
	asrs r3, r3, #1
	negs r3, r3
	movs r7, #0
	mov r10, r7
	adds r2, r0, r3
	adds r7, r4, r3
	lsls r3, r1, #2
	ldr r1, .L_08178d58
	adds r5, r3, r1
.L_08178b42:
	mov r0, r8
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r4, [sp, #8]
	asrs r3, r3, #16
	subs r3, r3, r4
	strb r3, [r5]
	mov r0, r8
	bl Trig_Sin
	ldr r2, [sp, #12]
	adds r7, r7, r6
	adds r3, r2, #0
	muls r3, r0
	movs r0, #1
	asrs r3, r3, #16
	negs r3, r3
	add r10, r0
	strb r3, [r5, #1]
	mov r1, r10
	movs r3, #0
	strb r3, [r5, #2]
	adds r2, r2, r6
	adds r5, #4
	ldr r4, [sp, #8]
	cmp r1, #2
	bne .L_08178b42
	mov r2, r9
	cmp r2, #32
	bne .L_08178baa
	mov r0, r8
	bl Trig_Cos
	ldr r4, [sp, #8]
	adds r3, r4, #0
	muls r3, r0
	asrs r3, r3, #16
	subs r3, r3, r4
	mov r0, r8
	str r3, [sp, #104]
	bl Trig_Sin
	ldr r4, [sp, #40]
	adds r3, r4, #0
	muls r3, r0
	asrs r3, r3, #16
	negs r3, r3
	str r3, [sp, #96]
.L_08178baa:
	ldr r7, [sp, #24]
	movs r0, #1
	add r9, r0
	adds r7, #2
	mov r1, r9
	str r7, [sp, #24]
	cmp r1, #33
	bne .L_08178ac4
	movs r2, #0
	str r2, [sp, #16]
	mov r9, r2
.L_08178bc0:
	ldr r4, [sp, #48]
	ldr r3, [r4]
	cmp r3, #0
	bge .L_08178bca
	adds r3, #3
.L_08178bca:
	ldr r7, [sp, #44]
	asrs r0, r3, #2
	ldr r3, [r7]
	cmp r3, #0
	bge .L_08178bd6
	adds r3, #3
.L_08178bd6:
	asrs r3, r3, #2
	subs r4, r0, r3
	mov r0, r11
	ldr r2, [r0, #12]
	ldr r3, [r0, #8]
	subs r3, r3, r2
	mov r1, r9
	muls r1, r3
	lsls r3, r2, #5
	adds r3, r3, r2
	adds r1, r1, r3
	mov r8, r1
	cmp r4, #0
	bge .L_08178bf4
	negs r4, r4
.L_08178bf4:
	lsls r0, r4, #5
	movs r1, #36
	str r4, [sp, #8]
	bl Math_Div
	mov r1, r9
	lsls r6, r1, #2
	str r0, [sp, #36]
	ldr r4, [sp, #8]
	cmp r6, #24
	ble .L_08178c0c
	movs r6, #24
.L_08178c0c:
	movs r2, #162
	lsls r2, r2, #7
	adds r2, #32
	adds r3, r2, #0
	muls r3, r6
	cmp r3, #0
	bge .L_08178c22
	movs r7, #255
	lsls r7, r7, #8
	adds r7, #255
	adds r3, r3, r7
.L_08178c22:
	asrs r6, r3, #16
	lsrs r3, r3, #31
	adds r3, r6, r3
	ldr r1, [sp, #36]
	asrs r3, r3, #1
	negs r3, r3
	movs r0, #0
	mov r10, r0
	adds r2, r1, r3
	adds r7, r4, r3
	ldr r0, .L_08178d5c
	ldr r3, [sp, #16]
	adds r5, r3, r0
.L_08178c3c:
	mov r0, r8
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	ldr r4, [sp, #8]
	asrs r3, r3, #16
	subs r3, r3, r4
	strb r3, [r5]
	mov r0, r8
	bl Trig_Sin
	ldr r2, [sp, #12]
	movs r1, #0
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r5, #1]
	movs r3, #1
	add r10, r3
	mov r0, r10
	strb r1, [r5, #2]
	adds r2, r2, r6
	adds r7, r7, r6
	adds r5, #4
	ldr r4, [sp, #8]
	cmp r0, #2
	bne .L_08178c3c
	mov r1, r9
	cmp r1, #32
	bne .L_08178ca0
	mov r0, r8
	bl Trig_Cos
	ldr r4, [sp, #8]
	adds r3, r4, #0
	muls r3, r0
	asrs r3, r3, #16
	subs r3, r3, r4
	mov r0, r8
	str r3, [sp, #108]
	bl Trig_Sin
	ldr r2, [sp, #36]
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	str r3, [sp, #100]
.L_08178ca0:
	ldr r3, [sp, #16]
	movs r4, #1
	add r9, r4
	adds r3, #8
	mov r7, r9
	str r3, [sp, #16]
	cmp r7, #33
	bne .L_08178bc0
	movs r0, #1
	bl Func_081969f8
	adds r6, r0, #0
	ldr r0, .L_08178d60
	movs r2, #0
	movs r3, #6
	add r7, sp, #88
	str r3, [r6]
	strb r2, [r6, #25]
	str r7, [r6, #16]
	str r2, [r6, #20]
	str r0, [r6, #12]
	mov r10, r2
	ldr r2, [sp, #84]
	subs r3, #7
	ldr r1, [r2, #24]
	cmp r1, r3
	bne .L_08178cd8
	b .L_08178e66
.L_08178cd8:
	ldr r0, .L_08178d58
	ldr r4, [sp, #48]
	str r0, [sp, #28]
	movs r2, #0
	mov r8, r4
	mov r9, r2
.L_08178ce4:
	mov r3, r9
	adds r3, #4
	mov r4, r11
	mov r0, r9
	ldr r2, [r4, r3]
	ldr r3, [r4, r0]
	cmp r2, r3
	blt .L_08178cf6
	b .L_08178e4c
.L_08178cf6:
	movs r1, #6
	strb r1, [r7]
	movs r3, #5
	add r7, sp, #88
	strb r3, [r7, #1]
	ldr r2, [sp, #80]
	ldr r0, .L_08178d64
	movs r4, #224
	lsls r4, r4, #3
	adds r3, r2, r4
	str r3, [r7, #4]
	str r0, [r6, #8]
	ldr r1, [sp, #64]
	movs r3, #127
	lsls r2, r1, #3
	bics r3, r2
	strb r3, [r6, #24]
	bl Func_08014de4
	ldr r2, [sp, #84]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08178d6c
	mov r3, r8
	ldr r0, [r3]
	mov r4, r8
	lsrs r3, r0, #31
	ldr r1, [r4, #4]
	ldr r2, .L_08178d68
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #68
	lsls r1, r1, #16
	adds r1, r1, r2
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
	b .L_08178d8a
.L_08178d44:
	.4byte Data_08197410
.L_08178d48:
	.4byte 0xfffffd40
.L_08178d4c:
	.4byte 0xfffffa80
.L_08178d50:
	.4byte 0xfffffad8
.L_08178d54:
	.4byte 0xfffff818
.L_08178d58:
	.4byte gMapCellBuffer
.L_08178d5c:
	.4byte Data_02010108
.L_08178d60:
	.4byte Data_02011000
.L_08178d64:
	.4byte Data_02012000
.L_08178d68:
	.4byte 0xffa00000
.L_08178d6c:
	mov r3, r8
	ldr r0, [r3]
	mov r4, r8
	lsrs r3, r0, #31
	ldr r1, [r4, #4]
	ldr r2, .L_08178ecc
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #52
	lsls r1, r1, #16
	adds r1, r1, r2
	lsls r0, r0, #16
	movs r2, #0
	bl Func_08015160
.L_08178d8a:
	movs r5, #128
	lsls r5, r5, #8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_080151e4
	ldr r4, [sp, #84]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_08178da8
	adds r0, r5, #0
	bl Func_08015068
.L_08178da8:
	movs r3, #220
	lsls r3, r3, #6
	adds r3, #112
	mov r0, r10
	muls r0, r3
	ldr r1, .L_08178ed0
	adds r0, r0, r1
	bl Func_080150e4
	movs r0, #128
	lsls r0, r0, #11
	bl Func_0801521c
	movs r2, #66
	ldr r0, [sp, #28]
	ldr r1, .L_08178ed4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	mov r2, r10
	ldr r4, [sp, #64]
	lsls r3, r2, #4
	adds r3, #40
	cmp r4, r3
	bge .L_08178e48
	add r0, sp, #88
	adds r3, r0, #0
	movs r7, #6
	strb r7, [r0]
	adds r7, r3, #0
	ldr r3, .L_08178ed8
	movs r2, #0
	movs r1, #6
	strb r2, [r6, #24]
	strb r1, [r7, #1]
	str r3, [r6, #8]
	ldr r2, [sp, #80]
	movs r4, #142
	lsls r4, r4, #7
	adds r3, r2, r4
	mov r0, r10
	lsls r2, r0, #2
	str r3, [r7, #4]
	add r3, sp, #104
	ldr r0, [r2, r3]
	add r3, sp, #96
	ldr r1, [r2, r3]
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	mov r1, r9
	mov r2, r11
	ldr r3, [r1, r2]
	movs r0, #192
	lsls r0, r0, #6
	lsls r3, r3, #5
	adds r0, #96
	subs r0, r0, r3
	bl Func_080150e4
	movs r0, #162
	lsls r0, r0, #7
	adds r0, #32
	bl Func_0801521c
	ldr r1, .L_08178ed4
	ldr r0, .L_08178edc
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	ldr r3, [sp, #84]
	ldr r1, [r3, #24]
	b .L_08178e4c
.L_08178e48:
	ldr r4, [sp, #84]
	ldr r1, [r4, #24]
.L_08178e4c:
	ldr r0, [sp, #28]
	movs r2, #132
	movs r3, #8
	lsls r2, r2, #1
	movs r4, #1
	adds r0, r0, r2
	add r9, r3
	add r10, r4
	adds r3, r1, #1
	str r0, [sp, #28]
	cmp r10, r3
	beq .L_08178e66
	b .L_08178ce4
.L_08178e66:
	adds r0, r6, #0
	bl Sys_Free
.L_08178e6c:
	bl Func_081434f8
	movs r1, #2
	movs r0, #2
	bl Func_08158ce0
	movs r0, #240
	ldr r7, [sp, #80]
	lsls r0, r0, #7
	adds r0, #232
	adds r2, r7, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #64]
	ldr r3, [sp, #84]
	adds r1, #1
	str r1, [sp, #64]
	ldr r2, [r3, #24]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, #60
	cmp r1, r3
	beq .L_08178ea4
	b .L_08178874
.L_08178ea4:
	ldr r0, [sp, #52]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08178ee0
	bl Func_08014644
	bl Func_08143bb8
	add sp, #164
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08178ecc:
	.4byte 0xffa00000
.L_08178ed0:
	.4byte 0xfffff000
.L_08178ed4:
	.4byte Data_02011000
.L_08178ed8:
	.4byte Data_08199340
.L_08178edc:
	.4byte Data_081991e0
.L_08178ee0:
	.4byte Func_08143000
