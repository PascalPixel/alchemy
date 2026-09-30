.syntax unified
	.thumb
	.global Func_08166b10
	.thumb_func
Func_08166b10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #64
	str r0, [sp, #48]
	str r1, [sp, #44]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	str r0, [sp, #40]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #36]
	ldr r2, [r5, #100]
	str r2, [sp, #20]
	bl Func_081435e0
	ldr r3, .L_08166b78
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r4, [sp, #48]
	add r6, sp, #52
	movs r3, #36
	ldrsh r0, [r4, r3]
	adds r1, r6, #0
	bl Func_0815e21c
	ldr r3, [r6]
	movs r1, #19
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r0, #104
	str r3, [sp, #16]
	bl Func_081963ec
	ldr r6, [r5, #104]
	movs r0, #188
	movs r1, #35
	str r6, [sp, #24]
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	ldr r0, [sp, #44]
	b .L_08166b7c
.L_08166b78:
	.4byte 0x00001010
.L_08166b7c:
	str r5, [sp, #28]
	cmp r0, #2
	beq .L_08166b96
	ldr r2, [sp, #40]
	movs r3, #170
	lsls r3, r3, #7
	adds r3, #32
	adds r1, r2, r3
	ldr r0, .L_08166c08
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
.L_08166b96:
	ldr r4, [sp, #40]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r4, r5
	ldr r0, .L_08166c0c
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r2, #160
	ldr r6, [sp, #40]
	lsls r2, r2, #5
	adds r2, #208
	adds r1, r6, r2
	ldr r0, .L_08166c10
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r3, #0
	ldr r0, .L_08166c14
	ldr r1, [sp, #20]
	movs r2, #0
	bl Func_08157cf4
	ldr r3, [sp, #44]
	cmp r3, #1
	bne .L_08166c18
	movs r4, #0
	mov r8, r4
	ldr r4, .L_08166c04
	movs r0, #160
	lsls r0, r0, #19
	movs r5, #31
.L_08166bda:
	ldrh r3, [r0]
	adds r2, r5, #0
	ands r2, r3
	lsls r3, r3, #16
	lsrs r1, r3, #21
	lsrs r3, r3, #26
	ands r3, r4
	lsls r2, r2, #10
	lsls r3, r3, #5
	movs r6, #1
	ands r1, r4
	orrs r2, r3
	add r8, r6
	orrs r2, r1
	mov r1, r8
	strh r2, [r0]
	adds r0, #2
	cmp r1, #64
	bne .L_08166bda
	b .L_08166c18
	.2byte 0x0000
.L_08166c04:
	.4byte 0x0000001f
.L_08166c08:
	.4byte 0x00000118
.L_08166c0c:
	.4byte 0x00000146
.L_08166c10:
	.4byte 0x0000013e
.L_08166c14:
	.4byte 0x00000134
.L_08166c18:
	ldr r3, [sp, #40]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r5, [sp, #40]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #132
	adds r2, r5, r6
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_08166e10
	lsls r1, r1, #4
	bl Func_080145a8
	ldr r7, [sp, #40]
	movs r0, #0
	mov r8, r0
.L_08166c42:
	mov r1, r8
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
	mov r2, r8
	negs r3, r3
	str r3, [r7, #4]
	lsrs r3, r2, #31
	add r3, r8
	asrs r3, r3, #1
	adds r3, #25
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r7, #28
	cmp r4, #32
	bne .L_08166c42
	ldr r3, .L_08166e14
	movs r5, #0
	movs r1, #1
	movs r2, #171
	mov r8, r5
	negs r1, r1
	lsls r2, r2, #2
.L_08166c94:
	movs r6, #1
	add r8, r6
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_08166c94
	ldr r1, [sp, #16]
	ldr r7, .L_08166e18
	lsls r1, r1, #16
	str r1, [sp, #8]
	movs r0, #0
	mov r8, r0
.L_08166cac:
	bl Random16
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	ldr r2, [sp, #8]
	adds r5, r0, #0
	adds r3, #255
	ands r5, r3
	movs r3, #176
	lsls r3, r3, #15
	str r2, [r7]
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
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
	movs r4, #170
	movs r3, #1
	add r8, r3
	lsls r4, r4, #1
	adds r7, #28
	cmp r8, r4
	bne .L_08166cac
	movs r5, #0
	str r5, [sp, #32]
.L_08166d0e:
	ldr r6, [sp, #44]
	cmp r6, #0
	bne .L_08166d24
	ldr r3, [sp, #32]
	subs r3, #41
	cmp r3, #22
	bhi .L_08166d48
	ldr r0, .L_08166e1c
	bl Func_0815f0a0
	b .L_08166d48
.L_08166d24:
	ldr r0, [sp, #44]
	cmp r0, #1
	bne .L_08166d48
	ldr r1, [sp, #32]
	cmp r1, #61
	bne .L_08166d48
	ldr r0, .L_08166e20
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08166e24
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	bl Func_0815b634
.L_08166d48:
	ldr r2, [sp, #32]
	cmp r2, #8
	bne .L_08166d5a
	ldr r4, [sp, #40]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r3, r4, r5
	str r2, [r3]
.L_08166d5a:
	ldr r6, [sp, #32]
	cmp r6, #48
	bne .L_08166d6e
	ldr r0, [sp, #40]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r0, r1
	movs r3, #8
	str r3, [r2]
.L_08166d6e:
	ldr r2, [sp, #32]
	cmp r2, #60
	bne .L_08166d82
	ldr r3, [sp, #40]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	movs r3, #16
	str r3, [r2]
.L_08166d82:
	ldr r5, [sp, #32]
	cmp r5, #4
	bne .L_08166d8e
	movs r0, #212
	bl Audio_PlayCue
.L_08166d8e:
	ldr r6, [sp, #32]
	cmp r6, #32
	bne .L_08166d9a
	movs r0, #164
	bl Audio_PlayCue
.L_08166d9a:
	ldr r0, [sp, #32]
	cmp r0, #60
	bne .L_08166db4
	ldr r1, [sp, #44]
	cmp r1, #1
	bne .L_08166dae
	movs r0, #144
	bl Func_08118088 + 0x60
	b .L_08166db4
.L_08166dae:
	movs r0, #145
	bl Func_08118088 + 0x60
.L_08166db4:
	ldr r2, [sp, #32]
	cmp r2, #55
	ble .L_08166e3e
	ldr r4, .L_08166e28
	ldr r5, [sp, #40]
	movs r3, #0
	mov r8, r3
	mov r10, r4
.L_08166dc4:
	movs r0, #2
	ldrsh r3, [r5, r0]
	ldr r1, [sp, #16]
	ldr r0, [r5, #24]
	movs r6, #6
	ldrsh r7, [r5, r6]
	adds r6, r3, r1
	cmp r0, #17
	bhi .L_08166e08
	movs r1, #3
	bl __divsi3
	mov r2, r10
	ldrb r1, [r2, r0]
	ldr r3, [sp, #40]
	movs r4, #160
	movs r0, #32
	lsls r1, r1, #11
	lsls r4, r4, #5
	adds r1, r3, r1
	adds r4, #208
	adds r2, r6, #0
	str r0, [sp, #0]
	adds r3, r7, #0
	movs r0, #64
	str r0, [sp, #4]
	adds r1, r1, r4
	subs r2, #16
	adds r3, #48
	ldr r0, [sp, #36]
	ldr r6, [sp, #24]
	mov lr, r6
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_08166e08:
	cmp r0, #0
	ble .L_08166e2c
	subs r3, r0, #1
	b .L_08166e30
.L_08166e10:
	.4byte Func_08143000
.L_08166e14:
	.4byte Data_02010018
.L_08166e18:
	.4byte Data_02014ad0
.L_08166e1c:
	.4byte 0x00000184
.L_08166e20:
	.4byte 0x00000188
.L_08166e24:
	.4byte IwramCopyWords
.L_08166e28:
	.4byte Data_08198a9e
.L_08166e2c:
	movs r3, #1
	negs r3, r3
.L_08166e30:
	str r3, [r5, #24]
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #16
	bne .L_08166dc4
.L_08166e3e:
	ldr r2, [sp, #32]
	cmp r2, #28
	bne .L_08166ec6
	ldr r7, .L_08167158
	movs r3, #0
	movs r4, #63
	mov r8, r3
	mov r10, r4
.L_08166e4e:
	ldr r3, [r7, #24]
	movs r5, #1
	negs r5, r5
	cmp r3, r5
	bne .L_08166eb8
	bl Random16
	adds r6, r0, #0
	mov r0, r10
	ands r6, r0
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
	ldr r1, [sp, #8]
	asrs r3, r3, #3
	adds r3, r3, r1
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r2, #192
	lsls r2, r2, #15
	asrs r3, r3, #2
	adds r3, r3, r2
	str r3, [r7, #4]
	bl Random16
	mov r3, r10
	ands r0, r3
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r4, r10
	ands r0, r4
	negs r0, r0
	subs r0, #8
	lsls r0, r0, #13
	movs r3, #0
	str r0, [r7, #16]
	str r3, [r7, #24]
.L_08166eb8:
	movs r5, #1
	movs r6, #128
	add r8, r5
	lsls r6, r6, #1
	adds r7, #28
	cmp r8, r6
	bne .L_08166e4e
.L_08166ec6:
	ldr r0, [sp, #32]
	subs r0, #32
	str r0, [sp, #12]
	cmp r0, #31
	bhi .L_08166fb4
	ldr r7, .L_08167158
	movs r1, #0
	movs r2, #63
	mov r9, r1
	mov r8, r1
	mov r10, r2
.L_08166edc:
	ldr r3, [r7, #24]
	movs r4, #1
	negs r4, r4
	cmp r3, r4
	bne .L_08166f50
	bl Random16
	adds r6, r0, #0
	bl Random16
	movs r3, #255
	mov r5, r10
	lsls r3, r3, #8
	ands r6, r5
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	ldr r0, [sp, #8]
	asrs r3, r3, #3
	adds r3, r3, r0
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r1, #192
	lsls r1, r1, #15
	asrs r3, r3, #2
	adds r3, r3, r1
	str r3, [r7, #4]
	bl Random16
	mov r2, r10
	ands r0, r2
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r3, r10
	ands r0, r3
	negs r0, r0
	movs r4, #1
	subs r0, #8
	add r9, r4
	lsls r0, r0, #13
	movs r3, #0
	mov r5, r9
	str r0, [r7, #16]
	str r3, [r7, #24]
	cmp r5, #16
	beq .L_08166f5e
.L_08166f50:
	movs r6, #1
	movs r0, #171
	add r8, r6
	lsls r0, r0, #2
	adds r7, #28
	cmp r8, r0
	bne .L_08166edc
.L_08166f5e:
	ldr r1, [sp, #12]
	cmp r1, #31
	bhi .L_08166fb4
	ldr r2, [sp, #32]
	ldr r1, [sp, #24]
	ldr r4, [sp, #40]
	lsls r0, r2, #4
	ldr r2, .L_0816715c
	movs r5, #224
	lsls r5, r5, #3
	adds r4, r4, r5
	movs r3, #104
	mov r11, r1
	adds r0, r0, r2
	movs r1, #104
	mov r10, r4
	mov r8, r3
	ldr r6, [sp, #16]
	bl Math_Mod
	subs r6, #17
	adds r5, r0, #0
	mov r9, r6
	mov r4, r8
	movs r6, #34
	movs r3, #4
	subs r3, r3, r5
	mov r1, r10
	mov r2, r9
	str r6, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	mov lr, r11
	.2byte 0xf800
	movs r3, #108
	subs r3, r3, r5
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #36]
	mov r1, r10
	mov r2, r9
	mov lr, r11
	.2byte 0xf800
.L_08166fb4:
	ldr r5, [sp, #32]
	cmp r5, #71
	bgt .L_081670b0
	ldr r5, .L_08167158
	movs r6, #0
	mov r8, r6
.L_08166fc0:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_081670a2
	mov r0, r8
	movs r1, #3
	bl Math_Mod
	ldr r3, [r5, #16]
	adds r4, r0, #2
	cmp r3, #0
	ble .L_08166fd8
	adds r4, #2
.L_08166fd8:
	ldr r0, [sp, #32]
	cmp r0, #68
	ble .L_08166fe4
	cmp r4, #5
	bgt .L_08166fe4
	movs r4, #6
.L_08166fe4:
	ldr r1, [sp, #32]
	cmp r1, #70
	ble .L_08166ff0
	cmp r4, #6
	bgt .L_08166ff0
	movs r4, #7
.L_08166ff0:
	ldr r2, [sp, #32]
	cmp r2, #72
	ble .L_08166ffc
	cmp r4, #7
	bgt .L_08166ffc
	movs r4, #8
.L_08166ffc:
	ldr r3, [sp, #32]
	cmp r3, #74
	ble .L_08167008
	cmp r4, #8
	bgt .L_08167008
	movs r4, #9
.L_08167008:
	ldr r6, [sp, #32]
	cmp r6, #76
	ble .L_08167010
	movs r4, #10
.L_08167010:
	ldr r2, .L_08167160
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r4, [sp, #0]
	subs r3, r3, r4
	str r0, [sp, #4]
	ldr r4, [sp, #24]
	ldr r0, [sp, #36]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5]
	ldr r2, [r5, #12]
	ldr r1, [r5, #16]
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r5, #4]
	adds r3, r3, r1
	str r3, [r5, #4]
	ldr r6, [sp, #32]
	cmp r6, #80
	ble .L_08167056
	ldr r0, .L_08167164
	adds r3, r1, r0
	b .L_08167064
.L_08167056:
	ldr r3, .L_08167168
	movs r2, #3
	mov r4, r8
	ands r2, r4
	lsls r2, r2, #2
	ldr r3, [r3, r2]
	adds r3, r1, r3
.L_08167064:
	str r3, [r5, #16]
	ldr r2, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_08167074
	adds r3, #63
.L_08167074:
	ldr r2, [r5, #16]
	asrs r3, r3, #6
	str r3, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r2, r3, #1
	cmp r2, #0
	bge .L_08167086
	adds r2, #63
.L_08167086:
	ldr r3, [r5, #24]
	asrs r2, r2, #6
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
	cmp r2, #0
	ble .L_081670a2
	movs r6, #6
	ldrsh r3, [r5, r6]
	cmp r3, #108
	ble .L_081670a2
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_081670a2:
	movs r0, #1
	movs r1, #171
	add r8, r0
	lsls r1, r1, #1
	adds r5, #28
	cmp r8, r1
	bne .L_08166fc0
.L_081670b0:
	ldr r2, [sp, #32]
	cmp r2, #95
	bgt .L_0816711a
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	subs r2, #18
	movs r1, #120
	cmp r3, #60
	ble .L_081670ca
	ldr r5, .L_0816716c
	lsls r3, r3, #3
	adds r4, r3, r5
	b .L_081670ee
.L_081670ca:
	ldr r6, [sp, #32]
	cmp r6, #32
	ble .L_081670de
	ldr r0, [sp, #12]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r4, r3, #0
	adds r4, #16
	b .L_081670ee
.L_081670de:
	ldr r3, [sp, #32]
	cmp r3, #9
	bgt .L_081670ec
	lsls r3, r3, #4
	adds r4, r3, #0
	subs r4, #128
	b .L_081670ee
.L_081670ec:
	movs r4, #16
.L_081670ee:
	adds r3, r4, #0
	adds r3, #120
	cmp r3, #108
	ble .L_081670fc
	subs r3, r1, r4
	adds r1, r3, #0
	subs r1, #12
.L_081670fc:
	cmp r1, #0
	ble .L_0816711a
	ldr r5, [sp, #40]
	movs r6, #170
	lsls r6, r6, #7
	movs r3, #36
	adds r6, #32
	str r3, [sp, #0]
	str r1, [sp, #4]
	adds r3, r4, #0
	ldr r0, [sp, #36]
	adds r1, r5, r6
	ldr r4, [sp, #28]
	mov lr, r4
	.2byte 0xf800
.L_0816711a:
	ldr r5, [sp, #32]
	cmp r5, #59
	ble .L_081671c0
	ldr r7, .L_08167170
	movs r6, #0
	mov r8, r6
.L_08167126:
	ldr r3, [r7, #24]
	cmp r3, #0
	ble .L_081671b2
	movs r2, #128
	adds r0, r7, #0
	movs r1, #64
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r7, #24]
	ldr r6, [r7, #4]
	movs r1, #216
	subs r0, r3, #1
	lsls r1, r1, #15
	str r0, [r7, #24]
	cmp r6, r1
	ble .L_08167174
	ldr r3, [r7, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #16]
	b .L_081671b2
	.2byte 0x0000
.L_08167158:
	.4byte gMapCellBuffer
.L_0816715c:
	.4byte 0xffffff00
.L_08167160:
	.4byte Data_08197410
.L_08167164:
	.4byte 0xffff8000
.L_08167168:
	.4byte Data_08198aa4
.L_0816716c:
	.4byte 0xfffffe3e
.L_08167170:
	.4byte Data_02014ad0
.L_08167174:
	ldr r5, [r7]
	ldr r2, .L_08167288
	cmp r5, r2
	bhi .L_081671b2
	cmp r6, #0
	blt .L_081671b2
	movs r1, #5
	bl __divsi3
	ldr r2, .L_0816728c
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #20]
	asrs r5, r5, #16
	adds r1, r3, r1
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	asrs r6, r6, #16
	subs r5, r5, r3
	subs r6, r6, r0
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	adds r2, r5, #0
	adds r3, r6, #0
	ldr r4, [sp, #24]
	mov lr, r4
	.2byte 0xf800
.L_081671b2:
	movs r5, #1
	movs r6, #170
	add r8, r5
	lsls r6, r6, #1
	adds r7, #28
	cmp r8, r6
	bne .L_08167126
.L_081671c0:
	ldr r0, [sp, #32]
	cmp r0, #68
	bne .L_081671fc
	ldr r2, [sp, #48]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r8, r1
	cmp r3, #0
	beq .L_081671fc
	movs r5, #36
.L_081671d4:
	ldr r3, [sp, #48]
	movs r1, #7
	ldrsh r0, [r5, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl Func_0814cd48
	ldr r6, [sp, #48]
	ldrsh r0, [r5, r6]
	movs r1, #7
	bl Func_08118088
	ldr r3, [r6, #20]
	movs r2, #1
	add r8, r2
	adds r5, #2
	cmp r8, r3
	bne .L_081671d4
.L_081671fc:
	ldr r3, [sp, #32]
	cmp r3, #9
	bne .L_08167224
	ldr r5, [sp, #48]
	movs r3, #8
	movs r4, #36
	ldrsh r0, [r5, r4]
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r1, #128
	ldr r3, .L_08167290
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	ldr r2, .L_08167294
	mov lr, r3
	.2byte 0xf800
.L_08167224:
	ldr r6, [sp, #32]
	cmp r6, #60
	bne .L_08167238
	movs r1, #128
	ldr r3, .L_08167290
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	ldr r2, .L_08167294
	mov lr, r3
	.2byte 0xf800
.L_08167238:
	movs r0, #16
	movs r1, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #40]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #32]
	adds r2, #1
	str r2, [sp, #32]
	cmp r2, #102
	beq .L_08167264
	b .L_08166d0e
.L_08167264:
	ldr r0, .L_08167298
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
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
.L_08167288:
	.4byte 0x007effff
.L_0816728c:
	.4byte Data_08197410
.L_08167290:
	.4byte IwramFillWords
.L_08167294:
	.4byte 0x3f3f3f3f
.L_08167298:
	.4byte Func_08143000
