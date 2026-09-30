.syntax unified
	.thumb
	.global Func_08170dac
	.thumb_func
Func_08170dac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #248
	str r0, [sp, #76]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	movs r6, #0
	str r0, [sp, #72]
	movs r0, #1
	ldr r1, [r3, #96]
	str r1, [sp, #68]
	ldr r2, [r3, #48]
	str r2, [sp, #52]
	ldr r3, [r3, #100]
	str r3, [sp, #48]
	bl BattleFx_BeginCanvasLayer
	ldr r3, [sp, #72]
	movs r4, #224
	lsls r4, r4, #3
	adds r1, r3, r4
	ldr r0, .L_08170ee8
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r5, [sp, #72]
	movs r2, #184
	lsls r2, r2, #5
	adds r1, r5, r2
	ldr r0, .L_08170eec
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	movs r2, #0
	ldr r1, [sp, #48]
	ldr r0, .L_08170ef0
	movs r3, #0
	bl Func_08157cf4
	movs r1, #192
	ldr r2, [sp, #48]
	lsls r1, r1, #2
	adds r1, #2
.L_08170e12:
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08170e22
	subs r3, #8
	cmp r3, #0
	bgt .L_08170e20
	movs r3, #1
.L_08170e20:
	strb r3, [r2]
.L_08170e22:
	adds r6, #1
	adds r2, #1
	cmp r6, r1
	bne .L_08170e12
	ldr r3, .L_08170ef4
	movs r5, #128
	lsls r5, r5, #5
	adds r1, r5, r3
	ldr r0, .L_08170ef8
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	adds r1, r5, #0
	ldr r3, .L_08170efc
	ldr r0, .L_08170ef4
	mov lr, r3
	.2byte 0xf800
	ldr r4, .L_08170ef4
	movs r6, #0
	movs r7, #0
	movs r5, #0
	mov r12, r4
.L_08170e50:
	ldr r1, .L_08170f00
	mov r0, r12
	adds r3, r7, r6
	adds r2, r5, r0
	adds r0, r5, r1
	lsls r1, r3, #3
	movs r3, #128
	lsls r3, r3, #5
	add r1, r12
	adds r1, r1, r3
	movs r3, #156
	lsls r3, r3, #1
	adds r3, #255
	movs r4, #0
	adds r2, r2, r3
.L_08170e6e:
	ldrb r3, [r1]
	adds r4, #1
	strb r3, [r0]
	adds r0, #1
	ldrb r3, [r1]
	adds r1, #1
	strb r3, [r2]
	subs r2, #1
	cmp r4, #24
	bne .L_08170e6e
	adds r6, #1
	adds r7, #2
	adds r5, #64
	cmp r6, #48
	bne .L_08170e50
	ldr r0, .L_08170f04
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08170f08
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	movs r0, #132
	ldr r4, [sp, #72]
	lsls r0, r0, #6
	adds r0, #32
	adds r5, r4, r0
	movs r6, #0
.L_08170eac:
	lsls r0, r6, #9
	bl Trig_Sin
	lsls r3, r0, #6
	subs r3, r3, r0
	asrs r3, r3, #16
	movs r2, #0
.L_08170eba:
	adds r2, #1
	strb r3, [r5]
	adds r5, #1
	cmp r2, #32
	bne .L_08170eba
	adds r6, #1
	cmp r6, #32
	bne .L_08170eac
	movs r3, #128
	ldr r2, .L_08170ee4
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	ldr r1, [sp, #76]
	movs r6, #4
	ldr r3, [r1, #4]
	cmp r3, #0
	beq .L_08170f0c
	movs r6, #0
	b .L_08170f0c
	.2byte 0x0000
.L_08170ee4:
	.4byte 0x00001010
.L_08170ee8:
	.4byte 0x00000178
.L_08170eec:
	.4byte 0x00000141
.L_08170ef0:
	.4byte 0x00000134
.L_08170ef4:
	.4byte gMapCellBuffer
.L_08170ef8:
	.4byte 0x0000013a
.L_08170efc:
	.4byte IwramClearWords
.L_08170f00:
	.4byte Data_02010208
.L_08170f04:
	.4byte 0x00000188
.L_08170f08:
	.4byte IwramCopyWords
.L_08170f0c:
	movs r3, #3
	orrs r3, r6
	movs r5, #1
	movs r1, #7
	movs r2, #7
	movs r0, #104
	str r5, [sp, #0]
	bl Func_08196404
	movs r3, #11
	orrs r6, r3
	movs r1, #7
	movs r2, #7
	adds r3, r6, #0
	movs r0, #188
	str r5, [sp, #0]
	bl Func_08196404
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #104]
	adds r3, #188
	ldr r4, [sp, #72]
	ldr r3, [r3]
	movs r5, #238
	lsls r5, r5, #7
	movs r0, #238
	adds r5, #172
	lsls r0, r0, #7
	str r2, [sp, #56]
	str r3, [sp, #60]
	movs r2, #0
	adds r3, r4, r5
	adds r0, #176
	str r2, [r3]
	movs r1, #144
	adds r3, r4, r0
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0817100c
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #72]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08171010
	bl Scheduler_AddOrUpdateCallback
	movs r0, #140
	bl Audio_PlayCue
	movs r3, #238
	ldr r0, [sp, #52]
	ldr r2, [sp, #72]
	lsls r3, r3, #7
	mov r1, sp
	adds r3, #172
	movs r5, #0
	adds r0, #12
	adds r1, #236
	adds r3, r2, r3
	str r5, [sp, #64]
	str r0, [sp, #16]
	str r1, [sp, #24]
	str r3, [sp, #44]
	str r5, [sp, #12]
.L_08170fa6:
	ldr r3, [sp, #64]
	subs r3, #9
	cmp r3, #54
	bhi .L_08170fb6
	ldr r3, .L_08171014
	ldr r4, [sp, #44]
	str r3, [r4]
	b .L_08170fd2
.L_08170fb6:
	ldr r3, [sp, #64]
	subs r3, #64
	cmp r3, #15
	bhi .L_08170fcc
	ldr r5, [sp, #64]
	ldr r0, .L_08171018
	ldr r1, [sp, #44]
	lsls r3, r5, #4
	adds r3, r3, r0
	str r3, [r1]
	b .L_08170fd2
.L_08170fcc:
	ldr r2, [sp, #44]
	movs r3, #0
	str r3, [r2]
.L_08170fd2:
	ldr r4, [sp, #76]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_08170fe2
	ldr r5, [sp, #44]
	ldr r3, [r5]
	negs r3, r3
	str r3, [r5]
.L_08170fe2:
	ldr r0, [sp, #64]
	cmp r0, #79
	ble .L_0817101c
	lsls r1, r0, #1
	ldr r3, .L_08171004
	adds r2, r1, #0
	subs r2, #160
	subs r3, r3, r2
	ldr r2, .L_08171008
	str r1, [sp, #20]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	b .L_08171022
	.2byte 0x0000
.L_08171004:
	.4byte 0x00000010
.L_08171008:
	.4byte 0x00001000
.L_0817100c:
	.4byte Func_0814c928
.L_08171010:
	.4byte Func_08143000
.L_08171014:
	.4byte 0xffffff00
.L_08171018:
	.4byte 0xfffffb00
.L_0817101c:
	ldr r2, [sp, #64]
	lsls r2, r2, #1
	str r2, [sp, #20]
.L_08171022:
	movs r2, #0
	movs r3, #100
	movs r0, #0
	movs r1, #0
	bl Func_08118028
	bl Func_08014de4
	ldr r0, [sp, #52]
	ldr r1, [sp, #16]
	bl Func_080156e8
	ldr r3, [sp, #76]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	str r0, [sp, #40]
	ldr r3, [r0, #8]
	cmp r3, #0
	ble .L_08171056
	ldr r4, .L_08171244
	ldr r5, [sp, #24]
	adds r3, r3, r4
	str r3, [r5]
	b .L_08171060
.L_08171056:
	movs r0, #128
	ldr r1, [sp, #24]
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r1]
.L_08171060:
	ldr r3, .L_08171248
	ldr r2, [sp, #12]
	adds r3, r2, r3
	str r3, [sp, #36]
	cmp r3, #0
	bge .L_0817106e
	b .L_08171176
.L_0817106e:
	mov r4, sp
	adds r4, #104
	ldr r5, .L_0817124c
	str r4, [sp, #28]
	ldr r7, [sp, #24]
	mov r8, r5
	movs r6, #0
	adds r5, r4, #0
.L_0817107e:
	adds r0, r6, #4
	movs r1, #6
	bl __modsi3
	lsls r0, r0, #1
	mov r1, r8
	adds r3, r0, #1
	ldrsb r3, [r1, r3]
	movs r2, #240
	lsls r2, r2, #13
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r4, [sp, #40]
	ldrsb r3, [r1, r0]
	ldr r2, [r4, #16]
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r7, #8]
	adds r1, r5, #0
	adds r0, r7, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	adds r6, #1
	asrs r3, r3, #1
	str r3, [r5]
	adds r5, #12
	cmp r6, #6
	bne .L_0817107e
	movs r6, #0
.L_081710bc:
	lsls r3, r6, #3
	ldr r5, [sp, #36]
	adds r3, r3, r6
	lsls r3, r3, #2
	subs r4, r5, r3
	cmp r4, #36
	ble .L_081710cc
	movs r4, #36
.L_081710cc:
	adds r0, r6, #1
	mov r9, r0
	cmp r4, #0
	blt .L_08171170
	movs r1, #0
	mov r10, r1
	cmp r4, #0
	beq .L_0817116c
	lsls r3, r6, #1
	adds r3, r3, r6
	lsls r3, r3, #2
	adds r2, r0, #0
	mov r8, r3
	ldr r7, [sp, #28]
	movs r3, #4
	str r2, [sp, #32]
	add r3, r8
	mov r11, r3
.L_081710f0:
	ldr r0, [sp, #32]
	movs r1, #6
	str r4, [sp, #8]
	bl __modsi3
	lsls r5, r0, #1
	adds r5, r5, r0
	lsls r5, r5, #2
	mov r0, r8
	ldr r3, [r7, r0]
	ldr r2, [r7, r5]
	movs r1, #224
	subs r2, r2, r3
	lsls r1, r1, #3
	mov r0, r10
	muls r0, r2
	adds r1, #28
	ldr r2, .L_08171250
	mov lr, r2
	.2byte 0xf800
	adds r5, #4
	ldr r2, [r7, r5]
	mov r3, r8
	mov r5, r11
	ldr r6, [r7, r3]
	ldr r3, [r7, r5]
	movs r1, #224
	subs r2, r2, r3
	lsls r1, r1, #3
	adds r6, r6, r0
	adds r1, #28
	mov r0, r10
	muls r0, r2
	ldr r2, .L_08171250
	mov lr, r2
	.2byte 0xf800
	ldr r1, .L_08171254
	movs r2, #8
	ldr r3, [r7, r5]
	subs r2, #2
	ldrh r1, [r1, r2]
	ldr r5, [sp, #48]
	adds r3, r3, r0
	movs r0, #2
	adds r1, r5, r1
	subs r6, r6, r0
	movs r2, #4
	movs r5, #8
	str r2, [sp, #0]
	str r5, [sp, #4]
	subs r3, #4
	ldr r0, [sp, #68]
	adds r2, r6, #0
	ldr r5, [sp, #56]
	mov lr, r5
	.2byte 0xf800
	ldr r4, [sp, #8]
	movs r0, #1
	add r10, r0
	cmp r10, r4
	bne .L_081710f0
	b .L_08171170
.L_0817116c:
	adds r6, #1
	mov r9, r6
.L_08171170:
	mov r6, r9
	cmp r6, #6
	bne .L_081710bc
.L_08171176:
	ldr r2, [sp, #64]
	ldr r1, [sp, #20]
	adds r3, r1, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r10, r3
	cmp r3, #64
	ble .L_0817118c
	movs r3, #64
	mov r10, r3
.L_0817118c:
	mov r4, r10
	movs r6, #0
	cmp r4, #0
	beq .L_08171224
	ldr r5, [sp, #24]
	add r7, sp, #92
	movs r0, #8
	mov r8, r5
	mov r9, r7
	mov r11, r0
.L_081711a0:
	lsls r5, r6, #10
	adds r0, r5, #0
	bl Trig_Cos
	ldr r1, [sp, #40]
	lsls r2, r0, #2
	ldr r3, [r1, #12]
	adds r2, r2, r0
	lsls r2, r2, #3
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	mov r4, r8
	str r3, [r4, #4]
	adds r0, r5, #0
	bl Trig_Sin
	ldr r5, [sp, #40]
	lsls r3, r0, #2
	ldr r2, [r5, #16]
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r2, r2, r3
	mov r0, r8
	mov r1, r9
	str r2, [r0, #8]
	bl Func_0815e1ec
	mov r1, r9
	ldr r3, [r1]
	asrs r3, r3, #1
	str r3, [r1]
	ldr r3, [r1, #8]
	cmp r3, #159
	bgt .L_081711ec
	movs r3, #160
	str r3, [r7, #8]
.L_081711ec:
	movs r2, #136
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	ble .L_081711f8
	str r2, [r7, #8]
.L_081711f8:
	ldr r2, .L_08171254
	mov r3, r11
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #48]
	movs r3, #2
	adds r1, r2, r1
	ldr r2, [r7]
	movs r4, #4
	subs r2, r2, r3
	ldr r3, [r7, #4]
	mov r5, r11
	str r4, [sp, #0]
	subs r3, #4
	str r5, [sp, #4]
	ldr r0, [sp, #68]
	ldr r4, [sp, #60]
	adds r6, #1
	mov lr, r4
	.2byte 0xf800
	cmp r6, r10
	bne .L_081711a0
.L_08171224:
	ldr r5, [sp, #64]
	cmp r5, #71
	bgt .L_0817122c
	b .L_081713b4
.L_0817122c:
	cmp r5, #72
	bne .L_081712ae
	ldr r0, [sp, #24]
	ldr r1, [sp, #72]
	ldr r3, [r0]
	str r3, [r1]
	cmp r3, #0
	ble .L_0817125c
	ldr r3, .L_08171258
	str r3, [r1, #12]
	b .L_08171264
	.2byte 0x0000
.L_08171244:
	.4byte 0xfff80000
.L_08171248:
	.4byte 0xfffffeb0
.L_0817124c:
	.4byte Data_08196e9c
.L_08171250:
	.4byte IwramMulQ16
.L_08171254:
	.4byte Data_08197410
.L_08171258:
	.4byte 0xffe00000
.L_0817125c:
	ldr r2, [sp, #72]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #12]
.L_08171264:
	ldr r3, .L_08171278
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, [sp, #64]
	cmp r3, #72
	bne .L_081712ae
	b .L_0817127c
	.2byte 0x0000
.L_08171278:
	.4byte 0x00000784
.L_0817127c:
	ldr r4, [sp, #72]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r3, r4, r5
	movs r5, #8
	str r5, [r3]
	movs r0, #145
	bl Func_081180e8
	ldr r2, [sp, #76]
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #4
	bl Func_08118088
	ldr r4, [sp, #76]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r2, #5
	movs r3, #0
	str r5, [sp, #0]
	bl Func_0814cd48
.L_081712ae:
	ldr r0, [sp, #72]
	movs r1, #132
	ldr r3, [sp, #72]
	lsls r1, r1, #6
	movs r4, #184
	adds r1, #32
	lsls r4, r4, #5
	ldr r7, [sp, #24]
	add r5, sp, #80
	adds r0, r0, r1
	movs r2, #32
	adds r3, r3, r4
	movs r6, #0
	mov r9, r0
	mov r8, r2
	mov r10, r3
	mov r11, r5
.L_081712d0:
	ldr r0, [sp, #72]
	ldr r3, [r0, #12]
	adds r2, r6, #0
	muls r2, r3
	ldr r3, [r0]
	adds r0, r7, #0
	adds r3, r3, r2
	str r3, [r7]
	movs r3, #240
	lsls r3, r3, #13
	str r3, [r7, #4]
	ldr r1, [sp, #40]
	ldr r3, [r1, #16]
	mov r1, r11
	str r3, [r7, #8]
	bl Func_0815e1ec
	mov r2, r11
	ldr r3, [r2]
	asrs r2, r3, #1
	mov r3, r11
	str r2, [r3]
	ldr r4, [sp, #76]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0817130a
	adds r3, r2, #0
	subs r3, #32
	str r3, [r5]
.L_0817130a:
	cmp r6, #0
	ble .L_0817133a
	ldr r3, [r5, #4]
	mov r0, r8
	ldr r2, [r5]
	subs r3, #32
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r1, r9
	ldr r4, [sp, #56]
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	mov r0, r8
	ldr r2, [r5]
	ldr r3, [r5, #4]
	mov r1, r9
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #60]
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	b .L_0817136e
.L_0817133a:
	ldr r0, [sp, #64]
	cmp r0, #81
	bgt .L_0817136e
	ldr r3, [r5, #4]
	mov r1, r8
	movs r4, #36
	ldr r2, [r5]
	subs r3, #36
	str r1, [sp, #0]
	str r4, [sp, #4]
	mov r1, r10
	ldr r4, [sp, #56]
	ldr r0, [sp, #68]
	mov lr, r4
	.2byte 0xf800
	mov r0, r8
	movs r1, #36
	ldr r2, [r5]
	ldr r3, [r5, #4]
	ldr r4, [sp, #60]
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r0, [sp, #68]
	mov r1, r10
	mov lr, r4
	.2byte 0xf800
.L_0817136e:
	adds r6, #1
	cmp r6, #10
	bne .L_081712d0
	ldr r0, [sp, #72]
	movs r1, #132
	ldr r2, [sp, #64]
	lsls r1, r1, #6
	adds r1, #32
	adds r5, r0, r1
	movs r7, #0
	cmp r2, #79
	ble .L_0817138c
	ldr r3, [sp, #12]
	ldr r4, .L_08171424
	adds r7, r3, r4
.L_0817138c:
	movs r6, #0
.L_0817138e:
	lsls r0, r6, #9
	bl Trig_Sin
	lsls r3, r0, #6
	subs r3, r3, r0
	asrs r3, r3, #16
	subs r3, r3, r7
	cmp r3, #0
	bge .L_081713a2
	movs r3, #0
.L_081713a2:
	movs r2, #0
.L_081713a4:
	adds r2, #1
	strb r3, [r5]
	adds r5, #1
	cmp r2, #32
	bne .L_081713a4
	adds r6, #1
	cmp r6, #32
	bne .L_0817138e
.L_081713b4:
	ldr r5, [sp, #72]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #176
	adds r2, r5, r0
	ldr r3, [r2]
	cmp r3, #0
	bne .L_081713c8
	movs r3, #1
	str r3, [r2]
.L_081713c8:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #72]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #12]
	ldr r5, [sp, #64]
	adds r4, #8
	adds r5, #1
	str r4, [sp, #12]
	str r5, [sp, #64]
	cmp r5, #88
	beq .L_081713fa
	b .L_08170fa6
.L_081713fa:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08171428
	bl Scheduler_RemoveCallback
	ldr r0, .L_0817142c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #248
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08171424:
	.4byte 0xfffffd80
.L_08171428:
	.4byte Func_08143000
.L_0817142c:
	.4byte Func_0814c928
