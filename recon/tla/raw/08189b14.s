.syntax unified
	.thumb
	.global Func_08189b14
	.thumb_func
Func_08189b14:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #88
	str r0, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #48]
	movs r0, #1
	ldr r1, [r3, #96]
	str r1, [sp, #44]
	ldr r3, [r3, #100]
	str r3, [sp, #28]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08189b58
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r2, [sp, #52]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08189b5c
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	b .L_08189b64
.L_08189b58:
	.4byte 0x00001010
.L_08189b5c:
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
.L_08189b64:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	ldr r0, .L_08189cd4
	str r3, [sp, #32]
	ldr r1, .L_08189cd8
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #48]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_08189cd8
	movs r2, #64
	movs r3, #64
	bl Func_0816ae40
	ldr r0, .L_08189cdc
	ldr r1, [sp, #28]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r5, [sp, #48]
	movs r6, #142
	lsls r6, r6, #7
	adds r1, r5, r6
	ldr r0, .L_08189ce0
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #174
	lsls r2, r2, #7
	adds r1, r5, r2
	ldr r0, .L_08189ce4
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_08189ce8
	ldr r1, .L_08189cd8
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r5, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	movs r1, #200
	adds r2, r5, r4
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08189cec
	bl Scheduler_AddOrUpdateCallback
	movs r5, #0
	str r5, [sp, #40]
	ldr r6, [sp, #52]
	movs r1, #86
	ldr r0, [r6, #24]
	negs r1, r1
	lsls r3, r0, #4
	cmp r3, r1
	bne .L_08189bf8
	b .L_0818a1b6
.L_08189bf8:
	mov r2, sp
	mov r3, sp
	adds r2, #56
	adds r3, #76
	str r2, [sp, #16]
	str r3, [sp, #24]
.L_08189c04:
	ldr r4, [sp, #40]
	cmp r4, #0
	bne .L_08189c94
	ldr r6, [sp, #52]
	movs r1, #64
	movs r5, #36
	ldrsh r0, [r6, r5]
	add r1, sp
	mov r10, r1
	bl Func_0815e21c
	ldr r3, [r6, #4]
	mov r5, r10
	cmp r3, #0
	bne .L_08189c2c
	mov r2, r10
	ldr r3, [r2]
	ldr r6, [sp, #24]
	adds r3, #80
	b .L_08189c34
.L_08189c2c:
	mov r4, r10
	ldr r3, [r4]
	ldr r6, [sp, #24]
	subs r3, #80
.L_08189c34:
	str r3, [r6]
	ldr r3, [r5, #4]
	movs r0, #0
	subs r3, #80
	str r3, [r6, #4]
	ldr r1, [sp, #48]
	mov r9, r0
	adds r4, r5, #0
	adds r0, r6, #0
	movs r5, #0
.L_08189c48:
	ldr r3, [r0]
	lsls r3, r3, #15
	str r3, [r1]
	ldr r3, [r0, #4]
	lsls r3, r3, #16
	str r3, [r1, #4]
	ldr r2, [r0]
	ldr r3, [r4]
	subs r3, r3, r2
	lsls r3, r3, #11
	str r3, [r1, #12]
	ldr r2, [r0, #4]
	ldr r3, [r4, #4]
	str r5, [r1, #24]
	subs r3, r3, r2
	movs r2, #1
	lsls r3, r3, #12
	add r9, r2
	str r3, [r1, #16]
	mov r3, r9
	adds r1, #28
	cmp r3, #3
	bne .L_08189c48
	ldr r3, .L_08189cf0
	movs r4, #0
	movs r1, #1
	movs r2, #128
	mov r9, r4
	negs r1, r1
	lsls r2, r2, #2
.L_08189c84:
	movs r5, #1
	add r9, r5
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_08189c84
	ldr r6, [sp, #52]
	ldr r0, [r6, #24]
.L_08189c94:
	movs r2, #1
	movs r1, #0
	negs r2, r2
	mov r9, r1
	cmp r0, r2
	bne .L_08189ca2
	b .L_08189e46
.L_08189ca2:
	ldr r3, [sp, #48]
	str r1, [sp, #12]
	str r1, [sp, #8]
	mov r8, r3
.L_08189caa:
	ldr r3, [sp, #12]
	ldr r4, [sp, #40]
	adds r3, #54
	cmp r4, r3
	bne .L_08189cba
	movs r0, #212
	bl Audio_PlayCue
.L_08189cba:
	ldr r3, [sp, #12]
	ldr r5, [sp, #40]
	adds r3, #59
	cmp r5, r3
	bne .L_08189da4
	ldr r6, [sp, #52]
	ldr r3, [r6, #24]
	cmp r9, r3
	bne .L_08189cf4
	movs r0, #144
	bl Func_081180e8
	b .L_08189cfa
.L_08189cd4:
	.4byte 0x000000ec
.L_08189cd8:
	.4byte gMapCellBuffer
.L_08189cdc:
	.4byte 0x00000134
.L_08189ce0:
	.4byte 0x000000e7
.L_08189ce4:
	.4byte 0x000000e9
.L_08189ce8:
	.4byte 0x000000c2
.L_08189cec:
	.4byte Func_08143000
.L_08189cf0:
	.4byte Data_02014018
.L_08189cf4:
	movs r0, #144
	bl Audio_PlayCue
.L_08189cfa:
	ldr r2, [sp, #52]
	movs r3, #0
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r3, #100
	str r3, [sp, #4]
	movs r1, #1
	movs r3, #0
	movs r2, #0
	bl Func_0815f000
	ldr r4, [sp, #52]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r6, #238
	ldr r5, [sp, #48]
	lsls r6, r6, #7
	adds r6, #168
	adds r2, r5, r6
	movs r3, #4
	str r3, [r2]
	ldr r2, [sp, #8]
	ldr r3, .L_0818a040
	movs r1, #64
	movs r0, #0
	add r1, sp
	mov r11, r0
	mov r10, r1
	adds r7, r2, r3
.L_08189d44:
	mov r4, r10
	ldr r3, [r4]
	movs r6, #128
	lsls r3, r3, #15
	str r3, [r7]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r7, #4]
	bl Random16
	movs r1, #192
	lsls r1, r1, #8
	bl __umodsi3
	adds r5, r0, #0
	bl Random16
	lsls r6, r6, #6
	adds r5, r5, r6
	movs r3, #127
	adds r6, r0, #0
	adds r0, r5, #0
	ands r6, r3
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #4
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #3
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r0, #1
	add r11, r0
	adds r3, #16
	mov r1, r11
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #128
	bne .L_08189d44
.L_08189da4:
	ldr r3, [sp, #12]
	ldr r2, [sp, #40]
	adds r3, #50
	cmp r2, r3
	blt .L_08189e22
	mov r5, r8
	movs r4, #2
	ldrsh r3, [r5, r4]
	movs r1, #80
	adds r2, r3, #0
	movs r6, #6
	ldrsh r3, [r5, r6]
	subs r2, #20
	adds r4, r3, #0
	subs r4, #50
	cmp r4, #24
	ble .L_08189dca
	movs r3, #104
	subs r1, r3, r4
.L_08189dca:
	movs r3, #40
	str r3, [sp, #0]
	ldr r3, [sp, #48]
	movs r5, #142
	lsls r5, r5, #7
	str r1, [sp, #4]
	ldr r0, [sp, #44]
	adds r1, r3, r5
	ldr r6, [sp, #32]
	adds r3, r4, #0
	mov lr, r6
	.2byte 0xf800
	mov r0, r8
	ldr r5, [r0, #24]
	cmp r5, #12
	bgt .L_08189e1c
	ldr r3, [r0]
	ldr r0, [r0, #12]
	mov r1, r8
	adds r3, r3, r0
	str r3, [r1]
	ldr r4, [r1, #16]
	ldr r3, [r1, #4]
	adds r3, r3, r4
	str r3, [r1, #4]
	ldr r6, [sp, #24]
	add r1, sp, #64
	ldr r2, [r6]
	ldr r3, [r1]
	subs r3, r3, r2
	lsls r3, r3, #9
	adds r0, r0, r3
	mov r2, r8
	str r0, [r2, #12]
	ldr r3, [r1, #4]
	ldr r2, [r6, #4]
	subs r3, r3, r2
	lsls r3, r3, #10
	adds r4, r4, r3
	mov r3, r8
	str r4, [r3, #16]
.L_08189e1c:
	adds r3, r5, #1
	mov r4, r8
	str r3, [r4, #24]
.L_08189e22:
	ldr r5, [sp, #12]
	ldr r0, [sp, #8]
	movs r1, #224
	lsls r1, r1, #4
	adds r5, #16
	adds r0, r0, r1
	str r5, [sp, #12]
	str r0, [sp, #8]
	ldr r4, [sp, #52]
	movs r2, #1
	ldr r3, [r4, #24]
	movs r6, #28
	add r9, r2
	adds r3, #1
	add r8, r6
	cmp r9, r3
	beq .L_08189e46
	b .L_08189caa
.L_08189e46:
	movs r5, #0
	mov r9, r5
	ldr r5, .L_0818a040
.L_08189e4c:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_08189eaa
	asrs r0, r0, #2
	adds r0, #1
	ldr r2, .L_0818a044
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r6, [sp, #28]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	adds r1, r6, r1
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #44]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r5, #4]
	movs r2, #208
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_08189e98
	ldr r3, [r5, #16]
	str r2, [r5, #4]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
.L_08189e98:
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #7
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_08189eaa:
	movs r6, #1
	movs r0, #128
	add r9, r6
	lsls r0, r0, #2
	adds r5, #28
	cmp r9, r0
	bne .L_08189e4c
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	str r0, [sp, #20]
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0818a048
	ldr r3, [sp, #56]
	movs r1, #7
	ands r3, r2
	ldr r2, .L_0818a04c
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #56]
	ldr r3, [sp, #48]
	ldr r4, [sp, #16]
	adds r2, r3, r2
	ldr r3, .L_0818a050
	str r2, [r4, #4]
	str r3, [r0, #8]
	str r1, [r0]
	str r4, [r0, #16]
	ldr r5, [sp, #20]
	mov r10, r0
	str r5, [r0, #12]
	ldr r3, [sp, #40]
	subs r3, #16
	cmp r3, #91
	bhi .L_08189fac
	ldr r6, [sp, #40]
	ldr r0, [sp, #40]
	ldr r2, .L_0818a054
	lsls r3, r6, #1
	adds r6, r3, #0
	lsls r3, r0, #13
	adds r5, r3, r2
	movs r3, #48
	subs r6, #96
	negs r3, r3
	movs r1, #0
	cmp r6, r3
	ble .L_08189f18
	movs r6, #48
	negs r6, r6
.L_08189f18:
	ldr r4, [sp, #52]
	ldr r0, [sp, #40]
	ldr r3, [r4, #24]
	lsls r2, r3, #4
	adds r3, r2, #0
	adds r3, #60
	cmp r0, r3
	blt .L_08189f30
	subs r3, r0, r2
	lsls r3, r3, #2
	subs r3, #240
	negs r1, r3
.L_08189f30:
	movs r2, #128
	lsls r2, r2, #10
	cmp r5, r2
	ble .L_08189f3c
	movs r5, #128
	lsls r5, r5, #10
.L_08189f3c:
	mov r3, r10
	str r1, [r3, #20]
	bl Func_08014de4
	movs r2, #128
	lsls r2, r2, #8
	movs r1, #128
	adds r0, r2, #0
	lsls r1, r1, #9
	bl Func_080151e4
	ldr r4, [sp, #24]
	lsls r1, r6, #16
	ldr r0, [r4]
	movs r2, #0
	subs r0, #128
	lsls r0, r0, #16
	bl Func_08015160
	movs r0, #178
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	ldr r6, [sp, #52]
	ldr r3, [r6, #4]
	cmp r3, #0
	bne .L_08189f7c
	movs r0, #200
	lsls r0, r0, #5
	bl Func_08015068
	b .L_08189f82
.L_08189f7c:
	ldr r0, .L_0818a058
	bl Func_08015068
.L_08189f82:
	ldr r1, [sp, #40]
	lsls r0, r1, #9
	bl Func_080150e4
	adds r0, r5, #0
	bl Func_0801521c
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r2, #0
	adds r1, r2, #0
	bl Func_080151e4
	ldr r0, .L_0818a05c
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
.L_08189fac:
	ldr r2, .L_0818a060
	ldr r3, [sp, #16]
	movs r6, #1
	str r2, [r3, #4]
	ldr r5, [sp, #52]
	movs r4, #0
	ldr r0, [r5, #24]
	negs r6, r6
	mov r9, r4
	cmp r0, r6
	beq .L_0818a0a0
	ldr r1, [sp, #40]
	ldr r2, .L_0818a064
	movs r7, #128
	lsls r3, r1, #3
	lsls r7, r7, #8
	adds r6, r3, r2
.L_08189fce:
	mov r4, r9
	lsls r3, r4, #4
	ldr r5, [sp, #40]
	adds r2, r3, #0
	adds r2, #50
	cmp r5, r2
	blt .L_0818a094
	subs r3, r5, r3
	adds r2, r3, #0
	movs r3, #64
	subs r1, r3, r6
	subs r2, #50
	cmp r1, #0
	ble .L_08189fec
	movs r1, #0
.L_08189fec:
	movs r3, #64
	negs r3, r3
	cmp r1, r3
	ble .L_0818a094
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r2, r3, #7
	mov r4, r10
	adds r3, r3, r2
	lsls r3, r3, #5
	str r1, [r4, #20]
	adds r5, r3, r7
	bl Func_08014de4
	movs r1, #128
	adds r0, r7, #0
	lsls r1, r1, #9
	adds r2, r7, #0
	bl Func_080151e4
	ldr r1, [sp, #24]
	movs r2, #0
	ldr r0, [r1]
	ldr r1, .L_0818a068
	subs r0, #128
	lsls r0, r0, #16
	bl Func_08015160
	movs r0, #178
	lsls r0, r0, #7
	bl SceneTransform_ApplyPitch
	ldr r2, [sp, #52]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0818a06c
	movs r0, #200
	lsls r0, r0, #5
	bl Func_08015068
	b .L_0818a072
	.2byte 0x0000
.L_0818a040:
	.4byte Data_02014000
.L_0818a044:
	.4byte Data_08197410
.L_0818a048:
	.4byte 0xffffff00
.L_0818a04c:
	.4byte 0xffff00ff
.L_0818a050:
	.4byte Data_08199364
.L_0818a054:
	.4byte 0xfffe0000
.L_0818a058:
	.4byte 0xffffe700
.L_0818a05c:
	.4byte Data_081991e0
.L_0818a060:
	.4byte gMapCellBuffer
.L_0818a064:
	.4byte 0xfffffe70
.L_0818a068:
	.4byte 0xffd00000
.L_0818a06c:
	ldr r0, .L_0818a1d4
	bl Func_08015068
.L_0818a072:
	ldr r3, [sp, #40]
	lsls r0, r3, #9
	bl Func_080150e4
	adds r0, r5, #0
	bl Func_0801521c
	ldr r0, .L_0818a1d8
	ldr r1, [sp, #20]
	movs r2, #4
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	ldr r4, [sp, #52]
	ldr r0, [r4, #24]
.L_0818a094:
	movs r5, #1
	add r9, r5
	adds r3, r0, #1
	subs r6, #128
	cmp r9, r3
	bne .L_08189fce
.L_0818a0a0:
	ldr r6, [sp, #40]
	cmp r6, #49
	bgt .L_0818a172
	ldr r0, [sp, #16]
	movs r3, #5
	strb r3, [r0]
	add r3, sp, #56
	str r3, [sp, #16]
	ldr r1, [sp, #16]
	movs r3, #6
	strb r3, [r1, #1]
	ldr r2, [sp, #48]
	movs r4, #174
	lsls r4, r4, #7
	adds r3, r2, r4
	str r3, [r1, #4]
	ldr r3, .L_0818a1dc
	mov r5, r10
	str r3, [r5, #8]
	ldr r0, [sp, #40]
	ldr r1, [sp, #40]
	movs r6, #0
	lsls r0, r0, #12
	lsls r7, r1, #9
	mov r9, r6
	mov r11, r0
	add r6, sp, #76
	mov r8, r7
.L_0818a0d8:
	movs r3, #0
	mov r2, r10
	str r3, [r2, #20]
	bl Func_08014de4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Func_080151e4
	ldr r0, [r6]
	ldr r1, .L_0818a1e0
	subs r0, #128
	movs r2, #0
	lsls r0, r0, #16
	bl Func_08015160
	movs r0, #142
	lsls r0, r0, #7
	adds r0, #208
	ldr r5, .L_0818a1e4
	bl SceneTransform_ApplyPitch
	adds r0, r7, #0
	bl Func_080150e4
	movs r3, #128
	add r5, r11
	lsls r3, r3, #9
	cmp r5, r3
	ble .L_0818a120
	movs r5, #128
	lsls r5, r5, #9
.L_0818a120:
	ldr r4, [sp, #40]
	cmp r4, #31
	ble .L_0818a12e
	movs r3, #192
	lsls r3, r3, #10
	mov r0, r11
	subs r5, r3, r0
.L_0818a12e:
	mov r0, r8
	bl Trig_Sin
	cmp r0, #0
	bge .L_0818a13a
	adds r0, #7
.L_0818a13a:
	asrs r2, r0, #3
	adds r2, r2, r5
	movs r0, #128
	movs r1, #128
	lsls r2, r2, #1
	lsls r0, r0, #8
	lsls r1, r1, #9
	bl Func_080151e4
	movs r2, #4
	ldr r1, [sp, #20]
	ldr r0, .L_0818a1e8
	bl Func_08196958
	mov r0, r10
	bl Func_08196a7c
	movs r3, #1
	movs r1, #128
	movs r2, #128
	add r9, r3
	lsls r1, r1, #5
	lsls r2, r2, #6
	mov r4, r9
	add r8, r1
	adds r7, r7, r2
	cmp r4, #8
	bne .L_0818a0d8
.L_0818a172:
	mov r0, r10
	bl Sys_Free
	ldr r0, [sp, #20]
	bl Sys_Free
	movs r1, #4
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r6, #240
	ldr r5, [sp, #48]
	lsls r6, r6, #7
	adds r6, #232
	adds r2, r5, r6
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #40]
	ldr r1, [sp, #52]
	adds r0, #1
	str r0, [sp, #40]
	ldr r2, [sp, #40]
	ldr r3, [r1, #24]
	adds r0, r3, #0
	lsls r3, r0, #4
	adds r3, #86
	cmp r2, r3
	beq .L_0818a1b6
	b .L_08189c04
.L_0818a1b6:
	ldr r0, .L_0818a1ec
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0818a1d4:
	.4byte 0xffffe700
.L_0818a1d8:
	.4byte Data_081991e0
.L_0818a1dc:
	.4byte Data_081992b0
.L_0818a1e0:
	.4byte 0xffb00000
.L_0818a1e4:
	.4byte 0xffffc000
.L_0818a1e8:
	.4byte Data_08199200
.L_0818a1ec:
	.4byte Func_08143000
