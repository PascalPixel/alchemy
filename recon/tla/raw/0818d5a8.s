.syntax unified
	.thumb
	.global Func_0818d5a8
	.thumb_func
Func_0818d5a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #92
	str r0, [sp, #68]
	str r1, [sp, #64]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #60]
	ldr r1, [r3, #96]
	str r1, [sp, #56]
	ldr r3, [r3, #100]
	str r3, [sp, #44]
	bl Func_0813ba50
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0818d608
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r1, [sp, #44]
	ldr r0, .L_0818d60c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #60]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0818d610
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r4, [sp, #64]
	cmp r4, #0
	bne .L_0818d628
	b .L_0818d614
	.2byte 0x0000
.L_0818d608:
	.4byte 0x00001010
.L_0818d60c:
	.4byte 0x00000134
.L_0818d610:
	.4byte 0x0000013e
.L_0818d614:
	ldr r5, [sp, #60]
	movs r2, #220
	lsls r2, r2, #6
	adds r1, r5, r2
	ldr r0, .L_0818d980
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	b .L_0818d65c
.L_0818d628:
	ldr r3, [sp, #60]
	movs r4, #220
	lsls r4, r4, #6
	adds r1, r3, r4
	ldr r0, .L_0818d984
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r5, [sp, #60]
	movs r2, #142
	lsls r2, r2, #7
	adds r1, r5, r2
	ldr r0, .L_0818d988
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r3, #174
	lsls r3, r3, #7
	adds r1, r5, r3
	ldr r0, .L_0818d98c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
.L_0818d65c:
	ldr r0, .L_0818d990
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0818d994
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #68]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0818d682
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	b .L_0818d68a
.L_0818d682:
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
.L_0818d68a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r5, [sp, #60]
	movs r0, #239
	lsls r0, r0, #7
	str r3, [sp, #48]
	adds r2, r5, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #64]
	cmp r1, #0
	bne .L_0818d6b0
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r5, r3
	movs r3, #75
	b .L_0818d6bc
.L_0818d6b0:
	ldr r4, [sp, #60]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #132
	adds r2, r4, r5
	movs r3, #50
.L_0818d6bc:
	str r3, [r2]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0818d998
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0818d99c
	movs r0, #0
	movs r2, #128
	mov r8, r0
	movs r1, #0
	lsls r2, r2, #1
.L_0818d6d4:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0818d6d4
	ldr r5, [sp, #68]
	ldr r0, [r5, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r9, r0
	movs r1, #36
	ldrsh r0, [r5, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r2, #0
	str r0, [sp, #40]
	str r2, [sp, #36]
	str r2, [sp, #32]
	mov r3, r9
	ldr r3, [r3, #8]
	mov r4, r9
	str r3, [sp, #28]
	mov r5, sp
	ldr r4, [r4, #16]
	adds r5, #80
	str r4, [sp, #24]
	str r2, [sp, #20]
	str r5, [sp, #16]
	mov r11, r2
.L_0818d714:
	mov r0, r11
	cmp r0, #0
	bne .L_0818d72a
	ldr r1, [sp, #28]
	mov r2, r9
	str r1, [r2, #8]
	str r0, [r2, #12]
	ldr r3, [sp, #24]
	movs r4, #0
	str r3, [r2, #16]
	str r4, [sp, #20]
.L_0818d72a:
	mov r5, r11
	cmp r5, #17
	bgt .L_0818d734
	cmp r5, #0
	bne .L_0818d74c
.L_0818d734:
	ldr r1, [sp, #68]
	ldr r0, [r1, #8]
	ldr r1, [sp, #16]
	bl Func_0815e21c
	ldr r2, [sp, #16]
	ldr r4, [sp, #16]
	ldr r3, [r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r4]
.L_0818d74c:
	mov r3, r11
	subs r3, #2
	cmp r3, #1
	bhi .L_0818d776
	ldr r5, [sp, #16]
	ldr r4, [sp, #60]
	ldr r2, [r5]
	ldr r3, [r5, #4]
	movs r1, #32
	movs r5, #224
	str r1, [sp, #0]
	lsls r5, r5, #3
	movs r1, #64
	str r1, [sp, #4]
	subs r2, #16
	adds r1, r4, r5
	subs r3, #64
	ldr r0, [sp, #56]
	ldr r4, [sp, #48]
	mov lr, r4
	.2byte 0xf800
.L_0818d776:
	mov r2, r11
	subs r2, #4
	cmp r2, #11
	bhi .L_0818d7e0
	lsrs r3, r2, #31
	adds r3, r2, r3
	ldr r0, [sp, #60]
	asrs r3, r3, #1
	lsls r3, r3, #11
	movs r5, #0
	adds r0, r0, r3
	mov r8, r5
	add r7, sp, #80
	mov r10, r0
.L_0818d792:
	mov r1, r8
	lsls r6, r1, #12
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r11
	muls r3, r0
	ldr r5, [r7]
	asrs r3, r3, #16
	adds r0, r6, #0
	adds r5, r5, r3
	bl Trig_Cos
	mov r2, r11
	muls r2, r0
	ldr r3, [r7, #4]
	asrs r2, r2, #16
	adds r3, r3, r2
	mov r2, r11
	subs r3, r3, r2
	movs r2, #32
	subs r5, #16
	str r2, [sp, #0]
	movs r1, #224
	movs r2, #64
	str r2, [sp, #4]
	lsls r1, r1, #3
	adds r2, r5, #0
	movs r5, #1
	subs r3, #64
	ldr r0, [sp, #56]
	add r1, r10
	ldr r4, [sp, #48]
	add r8, r5
	mov lr, r4
	.2byte 0xf800
	mov r0, r8
	cmp r0, #16
	bne .L_0818d792
.L_0818d7e0:
	mov r1, r11
	cmp r1, #4
	bne .L_0818d83c
	mov r2, r9
	adds r2, #90
	movs r3, #0
	strb r3, [r2]
	movs r3, #153
	lsls r3, r3, #8
	mov r2, r9
	adds r3, #153
	str r3, [r2, #72]
	movs r3, #240
	lsls r3, r3, #12
	str r3, [r2, #40]
	ldr r3, [r2, #8]
	cmp r3, #0
	bge .L_0818d80a
	ldr r3, .L_0818d9a0
	str r3, [r2, #36]
	b .L_0818d812
.L_0818d80a:
	movs r3, #128
	lsls r3, r3, #12
	mov r4, r9
	str r3, [r4, #36]
.L_0818d812:
	ldr r5, [sp, #64]
	cmp r5, #1
	bne .L_0818d820
	mov r0, r9
	ldr r3, [r0, #36]
	negs r3, r3
	str r3, [r0, #36]
.L_0818d820:
	mov r0, r9
	movs r1, #2
	bl Object_SetMode
	movs r3, #238
	ldr r1, [sp, #60]
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #4
	str r3, [r2]
	movs r0, #136
	bl Audio_PlayCue
.L_0818d83c:
	mov r4, r11
	cmp r4, #16
	bne .L_0818d89a
	movs r3, #0
	mov r5, r9
	str r3, [r5, #72]
	ldr r0, [sp, #64]
	cmp r0, #0
	bne .L_0818d856
	ldr r1, [sp, #40]
	ldr r3, [r1, #16]
	str r3, [r5, #16]
	b .L_0818d85a
.L_0818d856:
	mov r2, r9
	str r3, [r2, #16]
.L_0818d85a:
	ldr r4, [sp, #40]
	mov r5, r9
	ldr r3, [r4, #16]
	movs r1, #100
	str r3, [r5, #16]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #44]
	ldr r3, [r4, #8]
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #2
	bl Math_Div
	ldr r3, [r5, #8]
	movs r1, #16
	subs r0, r0, r3
	bl Math_Div
	str r0, [sp, #36]
	ldr r1, [sp, #40]
	ldr r3, [r5, #12]
	ldr r0, [r1, #12]
	movs r1, #16
	subs r0, r0, r3
	bl Math_Div
	str r0, [sp, #32]
	mov r0, r9
	bl Object_ResetMotion
.L_0818d89a:
	mov r2, r11
	cmp r2, #17
	bgt .L_0818d8a2
	b .L_0818dc96
.L_0818d8a2:
	mov r3, r9
	ldr r2, [r3, #12]
	cmp r2, #0
	ble .L_0818d8c0
	ldr r3, [r3, #8]
	ldr r4, [sp, #36]
	mov r5, r9
	adds r3, r3, r4
	str r3, [r5, #8]
	ldr r0, [sp, #32]
	adds r3, r2, r0
	str r3, [r5, #12]
	cmp r3, #0
	ble .L_0818d8c0
	b .L_0818d9e6
.L_0818d8c0:
	ldr r1, [sp, #20]
	cmp r1, #0
	beq .L_0818d8c8
	b .L_0818d9e6
.L_0818d8c8:
	movs r3, #0
	movs r2, #1
	mov r4, r9
	str r2, [sp, #20]
	movs r5, #80
	str r3, [r4, #12]
	ldr r7, .L_0818d9a4
	add r5, sp
	mov r8, r3
	mov r10, r5
.L_0818d8dc:
	bl Random16
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #255
	ands r5, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r6, r0, #0
	adds r3, #255
	mov r0, r10
	ands r6, r3
	ldr r3, [r0]
	adds r5, #32
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r0, #4]
	adds r0, r6, #0
	subs r3, #24
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Trig_Sin
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
	movs r1, #1
	movs r2, #128
	adds r3, #32
	add r8, r1
	lsls r2, r2, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r8, r2
	bne .L_0818d8dc
	ldr r3, [sp, #60]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #4
	str r3, [r2]
	ldr r5, [sp, #64]
	cmp r5, #0
	bne .L_0818d9a8
	movs r0, #145
	bl Func_081180e8
	ldr r2, [sp, #68]
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #4
	bl Func_08118088
	ldr r4, [sp, #68]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	b .L_0818d9e6
	.2byte 0x0000
.L_0818d980:
	.4byte 0x0000014c
.L_0818d984:
	.4byte 0x000000e8
.L_0818d988:
	.4byte 0x000000da
.L_0818d98c:
	.4byte 0x000000c1
.L_0818d990:
	.4byte 0x00000148
.L_0818d994:
	.4byte IwramCopyWords
.L_0818d998:
	.4byte Func_08143000
.L_0818d99c:
	.4byte Data_02016018
.L_0818d9a0:
	.4byte 0xfff80000
.L_0818d9a4:
	.4byte Data_02016000
.L_0818d9a8:
	movs r0, #145
	bl Audio_PlayCue
	ldr r0, [sp, #68]
	movs r5, #0
	ldr r3, [r0, #20]
	mov r8, r5
	cmp r3, #0
	beq .L_0818d9e6
	movs r5, #36
.L_0818d9bc:
	ldr r1, [sp, #68]
	ldrsh r0, [r5, r1]
	movs r1, #3
	bl Func_08118088
	ldr r3, [sp, #68]
	movs r1, #7
	ldrsh r0, [r5, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl Func_0814cd48
	ldr r1, [sp, #68]
	movs r0, #1
	ldr r3, [r1, #20]
	add r8, r0
	adds r5, #2
	cmp r8, r3
	bne .L_0818d9bc
.L_0818d9e6:
	ldr r2, [sp, #64]
	cmp r2, #0
	bne .L_0818da1a
	mov r4, r9
	ldr r3, [r4, #12]
	cmp r3, #0
	bgt .L_0818d9f6
	b .L_0818dc96
.L_0818d9f6:
	ldr r5, [sp, #16]
	ldr r4, [sp, #60]
	ldr r2, [r5]
	ldr r3, [r5, #4]
	movs r1, #40
	movs r5, #220
	str r1, [sp, #0]
	lsls r5, r5, #6
	movs r1, #64
	str r1, [sp, #4]
	subs r2, #20
	adds r1, r4, r5
	subs r3, #52
	ldr r0, [sp, #56]
	ldr r4, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	b .L_0818dc96
.L_0818da1a:
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #12]
	movs r0, #1
	bl Func_081969f8
	mov r5, r11
	adds r7, r0, #0
	movs r1, #0
	cmp r5, #40
	bne .L_0818da42
	ldr r0, [sp, #60]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r0, r3
	movs r3, #14
	str r3, [r2]
.L_0818da42:
	ldr r3, [sp, #72]
	ldr r2, .L_0818ddac
	movs r4, #6
	ands r3, r2
	ldr r2, .L_0818ddb0
	orrs r3, r4
	ldr r5, [sp, #60]
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	movs r0, #220
	orrs r3, r2
	lsls r0, r0, #6
	str r3, [sp, #72]
	add r6, sp, #72
	adds r3, r5, r0
	str r3, [r6, #4]
	ldr r3, .L_0818ddb4
	str r4, [r7]
	str r3, [r7, #8]
	str r6, [r7, #16]
	ldr r2, [sp, #12]
	mov r3, r11
	str r2, [r7, #12]
	str r1, [r7, #20]
	cmp r3, #45
	bgt .L_0818dad2
	bl Func_08014de4
	ldr r4, [sp, #16]
	movs r2, #0
	ldr r0, [r4]
	ldr r1, [r4, #4]
	subs r0, #60
	subs r1, #80
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r5, #128
	bl Func_08015160
	lsls r5, r5, #8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_080151e4
	ldr r0, [sp, #68]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0818dab0
	ldr r0, .L_0818ddb8
	bl Func_080150e4
	b .L_0818dabc
.L_0818dab0:
	adds r0, r5, #0
	bl Func_08015068
	ldr r0, .L_0818ddb8
	bl Func_080150e4
.L_0818dabc:
	ldr r0, .L_0818ddbc
	bl Func_0801521c
	ldr r0, .L_0818ddc0
	ldr r1, [sp, #12]
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818dad2:
	mov r2, sp
	movs r1, #6
	adds r2, #72
	strb r1, [r6]
	str r2, [sp, #8]
	strb r1, [r2, #1]
	ldr r4, [sp, #60]
	movs r5, #142
	lsls r5, r5, #7
	adds r3, r4, r5
	str r3, [r2, #4]
	movs r3, #7
	str r3, [r7]
	ldr r3, .L_0818ddc4
	mov r1, r11
	str r3, [r7, #8]
	movs r0, #0
	lsls r1, r1, #3
	mov r8, r0
	mov r10, r1
.L_0818dafa:
	ldr r3, .L_0818ddc8
	mov r2, r8
	ldrb r3, [r3, r2]
	adds r2, r3, #0
	adds r2, #40
	cmp r11, r2
	ble .L_0818dbb2
	mov r4, r11
	subs r3, r2, r4
	lsls r3, r3, #3
	adds r1, r3, #0
	adds r1, #56
	cmp r1, #0
	ble .L_0818db18
	movs r1, #0
.L_0818db18:
	movs r5, #64
	negs r5, r5
	cmp r1, r5
	ble .L_0818dbb2
	ldr r3, .L_0818ddcc
	mov r4, r8
	ldrb r3, [r3, r4]
	mov r0, r11
	subs r2, r0, r2
	muls r2, r3
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r0, #128
	lsls r6, r3, #4
	lsls r0, r0, #8
	str r1, [r7, #20]
	adds r5, r6, r0
	bl Func_08014de4
	movs r2, #63
	mov r3, r10
	ands r3, r2
	strb r3, [r7, #25]
	ldr r1, [sp, #16]
	ldr r3, .L_0818ddd0
	ldr r0, [r1]
	mov r2, r8
	ldrsb r1, [r3, r2]
	subs r0, #60
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	asrs r0, r5, #1
	lsls r1, r5, #2
	cmp r5, #0
	bge .L_0818db70
	movs r3, #128
	lsls r3, r3, #8
	adds r3, #3
	adds r5, r6, r3
.L_0818db70:
	asrs r2, r5, #2
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #2
	bl SceneTransform_ApplyPitch
	movs r3, #1
	mov r4, r8
	ands r3, r4
	cmp r3, #0
	beq .L_0818db94
	lsls r0, r4, #2
	add r0, r11
	lsls r0, r0, #11
	bl Func_08015068
	b .L_0818dba2
.L_0818db94:
	mov r5, r8
	lsls r0, r5, #2
	mov r1, r11
	subs r0, r0, r1
	lsls r0, r0, #11
	bl Func_08015068
.L_0818dba2:
	ldr r0, .L_0818ddd4
	ldr r1, [sp, #12]
	movs r2, #32
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818dbb2:
	movs r3, #1
	add r8, r3
	movs r2, #16
	mov r4, r8
	add r10, r2
	cmp r4, #3
	bne .L_0818dafa
	movs r3, #0
	strb r3, [r7, #25]
	ldr r3, .L_0818ddd8
	movs r0, #174
	str r3, [r7, #8]
	ldr r5, [sp, #60]
	ldr r1, [sp, #8]
	lsls r0, r0, #7
	adds r3, r5, r0
	str r3, [r1, #4]
	movs r2, #7
	add r3, sp, #72
	strb r2, [r3]
	strb r2, [r1, #1]
	ldr r6, [sp, #60]
	movs r2, #0
	mov r8, r2
	mov r10, r11
.L_0818dbe4:
	ldr r3, .L_0818dddc
	mov r4, r8
	ldrb r3, [r3, r4]
	adds r2, r3, #0
	adds r2, #34
	cmp r11, r2
	bne .L_0818dbfc
	ldr r5, [sp, #16]
	ldr r3, [r5]
	str r3, [r6]
	ldr r3, [r5, #4]
	str r3, [r6, #4]
.L_0818dbfc:
	cmp r11, r2
	ble .L_0818dc7a
	mov r0, r11
	subs r3, r2, r0
	lsls r3, r3, #3
	adds r1, r3, #0
	adds r1, #40
	cmp r1, #0
	ble .L_0818dc10
	movs r1, #0
.L_0818dc10:
	movs r3, #64
	negs r3, r3
	cmp r1, r3
	ble .L_0818dc7a
	ldr r3, .L_0818dde0
	mov r5, r8
	ldrb r3, [r3, r5]
	mov r4, r11
	subs r2, r4, r2
	muls r2, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #220
	adds r5, r2, #0
	muls r5, r3
	str r1, [r7, #20]
	movs r3, #7
	mov r1, r10
	ands r3, r1
	lsls r3, r3, #4
	movs r0, #131
	lsls r0, r0, #7
	strb r3, [r7, #24]
	adds r5, r5, r0
	bl Func_08014de4
	ldr r3, .L_0818dde4
	ldr r0, [r6]
	mov r2, r8
	ldrsb r1, [r3, r2]
	subs r0, #60
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Func_08015160
	lsls r1, r5, #1
	adds r2, r5, #0
	adds r0, r5, #0
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #3
	bl SceneTransform_ApplyPitch
	ldr r0, .L_0818dde8
	ldr r1, [sp, #12]
	movs r2, #32
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0818dc7a:
	movs r4, #1
	add r8, r4
	movs r3, #5
	mov r5, r8
	add r10, r3
	adds r6, #28
	cmp r5, #2
	bne .L_0818dbe4
	adds r0, r7, #0
	bl Sys_Free
	ldr r0, [sp, #12]
	bl Sys_Free
.L_0818dc96:
	ldr r0, [sp, #64]
	cmp r0, #1
	bne .L_0818dce0
	mov r1, r11
	cmp r1, #50
	bne .L_0818dce0
	movs r0, #134
	bl Func_081180e8
	ldr r4, [sp, #68]
	movs r2, #0
	ldr r3, [r4, #20]
	mov r8, r2
	cmp r3, #0
	beq .L_0818dce0
	movs r5, #36
.L_0818dcb6:
	ldr r1, [sp, #68]
	movs r3, #128
	lsls r3, r3, #9
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r3, #250
	str r3, [sp, #4]
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #13
	movs r1, #1
	lsls r2, r2, #11
	bl Func_0815f000
	ldr r4, [sp, #68]
	movs r3, #1
	add r8, r3
	ldr r3, [r4, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_0818dcb6
.L_0818dce0:
	ldr r7, .L_0818ddec
	movs r5, #0
	mov r8, r5
.L_0818dce6:
	ldr r3, [r7, #24]
	cmp r3, #0
	ble .L_0818dd52
	subs r3, #1
	movs r2, #128
	adds r0, r7, #0
	str r3, [r7, #24]
	movs r1, #56
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	ldr r6, [r7, #4]
	movs r0, #224
	lsls r0, r0, #15
	cmp r6, r0
	ble .L_0818dd14
	ldr r3, [r7, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #16]
	b .L_0818dd52
.L_0818dd14:
	ldr r5, [r7]
	ldr r1, .L_0818ddf0
	cmp r5, r1
	bhi .L_0818dd52
	cmp r6, #0
	blt .L_0818dd52
	ldr r0, [r7, #24]
	cmp r0, #0
	bge .L_0818dd28
	adds r0, #7
.L_0818dd28:
	asrs r0, r0, #3
	adds r0, #1
	ldr r2, .L_0818ddf4
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #44]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r2, r1
	asrs r2, r5, #16
	subs r2, r2, r3
	asrs r3, r6, #16
	subs r3, r3, r0
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #56]
	ldr r4, [sp, #48]
	mov lr, r4
	.2byte 0xf800
.L_0818dd52:
	movs r5, #1
	movs r0, #128
	add r8, r5
	lsls r0, r0, #1
	adds r7, #28
	cmp r8, r0
	bne .L_0818dce6
	movs r0, #12
	movs r1, #12
	bl Func_08158ce0
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #60]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r4, #1
	add r11, r4
	mov r5, r11
	cmp r5, #88
	beq .L_0818dd8c
	b .L_0818d714
.L_0818dd8c:
	ldr r0, .L_0818ddf8
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0818ddac:
	.4byte 0xffffff00
.L_0818ddb0:
	.4byte 0xffff00ff
.L_0818ddb4:
	.4byte Data_08199340
.L_0818ddb8:
	.4byte 0xffffcc78
.L_0818ddbc:
	.4byte 0x000286a0
.L_0818ddc0:
	.4byte Data_081991e0
.L_0818ddc4:
	.4byte Data_081990d0
.L_0818ddc8:
	.4byte Data_08199e58
.L_0818ddcc:
	.4byte Data_08199e5c
.L_0818ddd0:
	.4byte Data_08199e60
.L_0818ddd4:
	.4byte Data_08199090
.L_0818ddd8:
	.4byte Data_08198ec4
.L_0818dddc:
	.4byte Data_08199e64
.L_0818dde0:
	.4byte Data_08199e68
.L_0818dde4:
	.4byte Data_08199e6c
.L_0818dde8:
	.4byte Data_08198cac
.L_0818ddec:
	.4byte Data_02016000
.L_0818ddf0:
	.4byte 0x007effff
.L_0818ddf4:
	.4byte Data_08197410
.L_0818ddf8:
	.4byte Func_08143000
