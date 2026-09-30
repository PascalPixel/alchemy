.syntax unified
	.thumb
	.global Func_08158dc8
	.thumb_func
Func_08158dc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #324
	movs r3, #192
	str r0, [sp, #120]
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #176
	ldr r2, [r2]
	movs r5, #248
	str r2, [sp, #116]
	lsls r5, r5, #5
	ldr r0, [r3, #92]
	movs r6, #0
	str r0, [sp, #112]
	movs r0, #128
	ldr r1, [r3, #96]
	lsls r0, r0, #6
	str r1, [sp, #108]
	mov r9, r6
	ldr r3, [r3, #100]
	movs r6, #192
	str r3, [sp, #104]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_08158e44
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r2, [sp, #112]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_08158e48
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r4, [sp, #112]
	ldr r0, .L_08158e4c
	adds r1, r4, r5
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	lsls r6, r6, #2
	ldr r0, .L_08158e50
	ldr r1, [sp, #104]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r5, #0
	movs r0, #64
	b .L_08158e54
	.2byte 0x0000
.L_08158e44:
	.4byte 0x00000100
.L_08158e48:
	.4byte 0x00000180
.L_08158e4c:
	.4byte 0x00000136
.L_08158e50:
	.4byte 0x00000134
.L_08158e54:
	adds r6, #2
.L_08158e56:
	ldr r2, [sp, #112]
	ldr r1, [sp, #104]
	adds r3, r5, r2
	movs r2, #184
	lsls r2, r2, #6
	adds r2, #16
	movs r4, #0
	mov r12, r0
	adds r3, r3, r2
.L_08158e68:
	ldrb r2, [r1]
	adds r1, #1
	cmp r2, r12
	ble .L_08158e72
	mov r2, r12
.L_08158e72:
	cmp r2, #0
	bge .L_08158e78
	movs r2, #0
.L_08158e78:
	adds r4, #1
	strb r2, [r3]
	adds r3, #1
	cmp r4, r6
	bne .L_08158e68
	movs r3, #1
	add r9, r3
	adds r5, r5, r4
	mov r4, r9
	subs r0, #7
	cmp r4, #8
	bne .L_08158e56
	bl Func_0813ba50
	ldr r2, .L_08158ed0
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r3, .L_08158ed4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r5, [sp, #112]
	movs r6, #238
	lsls r6, r6, #7
	movs r0, #238
	adds r6, #144
	lsls r0, r0, #7
	movs r2, #238
	adds r3, r5, r6
	adds r0, #148
	movs r6, #0
	lsls r2, r2, #7
	movs r4, #238
	str r6, [r3]
	movs r1, #2
	adds r3, r5, r0
	adds r2, #152
	lsls r4, r4, #7
	adds r4, #156
	b .L_08158ed8
.L_08158ed0:
	.4byte 0x00000000
.L_08158ed4:
	.4byte 0x00002784
.L_08158ed8:
	str r1, [r3]
	adds r3, r5, r2
	movs r2, #1
	str r2, [r3]
	adds r3, r5, r4
	str r6, [r3]
	ldr r5, [sp, #116]
	mov r10, r1
	str r2, [r5, #16]
	movs r5, #200
	lsls r5, r5, #4
	adds r1, r5, #0
	ldr r0, .L_08158f60
	bl Scheduler_AddOrUpdateCallback
	adds r1, r5, #0
	ldr r0, .L_08158f64
	bl Scheduler_AddOrUpdateCallback
	ldr r0, [sp, #112]
	movs r1, #239
	lsls r1, r1, #7
	adds r0, r0, r1
	str r6, [r0]
	mov r8, r0
	movs r1, #0
	movs r0, #0
	bl Func_08163c2c
	ldr r1, .L_08158f68
	movs r0, #1
	movs r2, #0
	bl Func_08118040
	ldr r2, .L_08158f6c
	ldr r5, .L_08158f54
	movs r6, #128
	movs r3, #240
	lsls r6, r6, #19
	str r3, [r2, #16]
	adds r6, #32
	movs r0, #0
	movs r1, #1
	bl Func_08163c2c
	strh r5, [r6]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r2, #128
	str r3, [sp, #92]
	ldr r3, .L_08158f58
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_08158f5c
	adds r2, #82
	strh r5, [r6]
	b .L_08158f70
.L_08158f54:
	.4byte 0x00000080
.L_08158f58:
	.4byte 0x00007741
.L_08158f5c:
	.4byte 0x0000100f
.L_08158f60:
	.4byte Func_0813baec
.L_08158f64:
	.4byte Func_08143000
.L_08158f68:
	.4byte 0x00000074
.L_08158f6c:
	.4byte gCameraSceneParameters
.L_08158f70:
	strh r3, [r2]
	ldr r3, .L_08158fa8
	subs r2, #2
	strh r3, [r2]
	mov r2, r10
	mov r3, r8
	str r2, [r3]
	ldr r4, [sp, #112]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #132
	movs r0, #146
	adds r2, r4, r5
	movs r3, #75
	lsls r0, r0, #1
	str r3, [r2]
	add r0, sp
	movs r2, #68
	adds r2, #255
	str r0, [sp, #44]
	movs r6, #0
	add r2, sp
	mov r9, r6
	adds r5, r0, #0
	movs r6, #63
	movs r1, #3
	adds r4, r2, #0
	b .L_08158fac
.L_08158fa8:
	.4byte 0x00003f44
.L_08158fac:
	strb r1, [r0]
	strb r1, [r4]
	subs r4, #1
	ldrb r3, [r0]
	cmp r3, #63
	bls .L_08158fba
	strb r6, [r5]
.L_08158fba:
	ldrb r3, [r2]
	cmp r3, #63
	bls .L_08158fc2
	strb r6, [r2]
.L_08158fc2:
	movs r3, #1
	add r9, r3
	mov r3, r9
	adds r5, #1
	adds r0, #1
	subs r2, #1
	adds r1, #8
	cmp r3, #16
	bne .L_08158fac
	ldr r3, .L_08159008
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r0, .L_0815900c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08159010
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	ldr r6, .L_08159014
	mov r5, sp
	movs r4, #0
	adds r5, #124
	str r4, [sp, #100]
	str r5, [sp, #80]
	str r6, [sp, #24]
	str r4, [sp, #20]
	str r4, [sp, #16]
	b .L_081594e2
	.2byte 0x0000
.L_08159008:
	.4byte 0x00000784
.L_0815900c:
	.4byte 0x00000184
.L_08159010:
	.4byte IwramCopyWords
.L_08159014:
	.4byte 0xfffff460
.L_08159018:
	ldr r1, [sp, #100]
	movs r0, #0
	movs r5, #63
	mov r9, r0
	ands r5, r1
.L_08159022:
	cmp r5, #15
	bgt .L_0815902e
	ldr r0, .L_081592b8
	bl Func_0815f0a0
	b .L_0815904c
.L_0815902e:
	cmp r5, #31
	bgt .L_0815903a
	ldr r0, .L_081592bc
	bl Func_0815f0a0
	b .L_0815904c
.L_0815903a:
	cmp r5, #47
	bgt .L_08159046
	ldr r0, .L_081592c0
	bl Func_0815f0a0
	b .L_0815904c
.L_08159046:
	ldr r0, .L_081592c4
	bl Func_0815f0a0
.L_0815904c:
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #2
	bne .L_08159022
	ldr r4, [sp, #100]
	cmp r4, #150
	bne .L_08159074
	ldr r5, [sp, #112]
	movs r6, #239
	lsls r6, r6, #7
	adds r2, r5, r6
	movs r3, #1
	movs r0, #238
	str r3, [r2]
	lsls r0, r0, #7
	ldr r3, .L_081592c8
	adds r0, #132
	adds r2, r5, r0
	b .L_0815908a
.L_08159074:
	ldr r1, [sp, #112]
	movs r2, #239
	movs r5, #238
	lsls r2, r2, #7
	lsls r5, r5, #7
	adds r3, r1, r2
	mov r4, r9
	adds r5, #132
	str r4, [r3]
	adds r2, r1, r5
	movs r3, #75
.L_0815908a:
	str r3, [r2]
	movs r0, #255
	movs r1, #192
	ldr r3, .L_081592cc
	lsls r1, r1, #8
	lsls r0, r0, #17
	mov lr, r3
	.2byte 0xf800
	adds r1, r0, #0
	movs r0, #255
	lsls r0, r0, #17
	ldr r2, .L_081592d0
	bl Camera_StoreSceneParameters
	bl Func_08014de4
	ldr r6, [sp, #100]
	cmp r6, #128
	ble .L_08159130
	subs r6, #128
	cmp r6, #22
	ble .L_081590b8
	movs r6, #20
.L_081590b8:
	add r0, sp, #136
	movs r3, #0
	str r3, [r0]
	negs r3, r6
	lsls r3, r3, #17
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r0, #8]
	asrs r3, r6, #2
	adds r3, #2
	str r3, [sp, #88]
	cmp r3, #8
	ble .L_081590d8
	movs r1, #8
	str r1, [sp, #88]
.L_081590d8:
	movs r5, #128
	bl SceneTransform_ApplyPosition
	lsls r5, r5, #8
	ldr r0, .L_081592d4
	bl SceneTransform_ApplyPitch
	adds r0, r5, #0
	bl Func_080150e4
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
	lsls r0, r6, #12
	bl Func_08015068
	ldr r2, [sp, #100]
	cmp r2, #150
	ble .L_08159116
	ldr r4, [sp, #16]
	ldr r5, .L_081592d8
	ldr r0, [sp, #80]
	adds r3, r4, r2
	lsls r3, r3, #11
	adds r3, r3, r5
	movs r6, #5
	str r3, [r0]
	str r3, [r0, #4]
	str r3, [r0, #8]
	str r6, [sp, #88]
	b .L_0815912a
.L_08159116:
	lsls r3, r6, #1
	ldr r0, [sp, #80]
	adds r3, r3, r6
	movs r2, #128
	lsls r3, r3, #10
	lsls r2, r2, #9
	subs r2, r2, r3
	str r2, [r0]
	str r2, [r0, #4]
	str r2, [r0, #8]
.L_0815912a:
	bl Func_080151ac
	b .L_08159158
.L_08159130:
	add r0, sp, #136
	movs r3, #0
	str r3, [r0]
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #17
	str r3, [r0, #8]
	bl SceneTransform_ApplyPosition
	ldr r0, .L_081592d4
	bl SceneTransform_ApplyPitch
	ldr r0, [sp, #100]
	lsls r5, r0, #8
	adds r0, r5, #0
	bl Func_080150e4
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
.L_08159158:
	ldr r1, [sp, #100]
	cmp r1, #149
	ble .L_08159160
	b .L_08159284
.L_08159160:
	mov r3, sp
	movs r2, #0
	adds r3, #148
	mov r9, r2
	str r3, [sp, #48]
	ldr r2, .L_081592dc
	add r6, sp, #280
	adds r5, r3, #0
	movs r7, #0
.L_08159172:
	ldrsh r3, [r7, r2]
	adds r1, r5, #0
	subs r3, #96
	lsls r3, r3, #16
	str r3, [r6]
	movs r3, #0
	str r3, [r6, #4]
	adds r3, r7, #2
	ldrsh r3, [r3, r2]
	adds r0, r6, #0
	subs r3, #96
	lsls r3, r3, #16
	str r3, [r6, #8]
	str r2, [sp, #12]
	bl Func_0815e1ec
	ldr r3, [r5]
	adds r7, #4
	asrs r3, r3, #17
	adds r3, #64
	str r3, [r5]
	movs r1, #6
	ldrsh r3, [r5, r1]
	ldr r2, [sp, #12]
	adds r3, #60
	str r3, [r5, #4]
	movs r3, #1
	add r9, r3
	mov r4, r9
	adds r5, #12
	cmp r4, #6
	bne .L_08159172
	ldr r0, [sp, #20]
	movs r5, #0
	movs r6, #4
	str r6, [sp, #36]
	str r5, [sp, #32]
	str r0, [sp, #28]
	mov r9, r5
.L_081591c0:
	ldr r1, [sp, #28]
	ldr r2, .L_081592e0
	adds r1, r1, r2
	mov r8, r1
	cmp r1, #48
	ble .L_081591d0
	movs r3, #48
	mov r8, r3
.L_081591d0:
	mov r4, r8
	cmp r4, #0
	blt .L_08159268
	mov r5, r8
	movs r4, #0
	cmp r5, #0
	beq .L_08159268
	ldr r6, [sp, #88]
	ldr r1, [sp, #88]
	asrs r6, r6, #31
	ldr r0, [sp, #36]
	str r6, [sp, #40]
	ldr r7, [sp, #32]
	ldr r6, [sp, #48]
	lsls r1, r1, #1
	mov r11, r0
	mov r10, r1
.L_081591f2:
	adds r3, r7, #0
	adds r3, #12
	ldr r2, [r6, r3]
	ldr r3, [r6, r7]
	movs r1, #160
	subs r2, r2, r3
	lsls r1, r1, #3
	str r4, [sp, #8]
	adds r0, r4, #0
	muls r0, r2
	adds r1, #85
	ldr r2, .L_081592e4
	mov lr, r2
	.2byte 0xf800
	ldr r5, [r6, r7]
	adds r3, r7, #0
	adds r5, r5, r0
	adds r3, #16
	mov r0, r11
	ldr r2, [r6, r3]
	ldr r3, [r6, r0]
	ldr r4, [sp, #8]
	movs r1, #160
	subs r2, r2, r3
	lsls r1, r1, #3
	adds r0, r4, #0
	muls r0, r2
	adds r1, #85
	ldr r2, .L_081592e4
	mov lr, r2
	.2byte 0xf800
	mov r1, r11
	ldr r3, [r6, r1]
	mov r2, r10
	adds r3, r3, r0
	ldr r0, .L_081592e8
	subs r2, #2
	ldrh r1, [r0, r2]
	ldr r2, [sp, #104]
	ldr r0, [sp, #40]
	adds r1, r2, r1
	lsrs r2, r0, #31
	ldr r0, [sp, #88]
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r5, r5, r2
	mov r2, r10
	subs r3, r3, r0
	str r0, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #108]
	adds r2, r5, #0
	ldr r5, [sp, #92]
	mov lr, r5
	.2byte 0xf800
	ldr r4, [sp, #8]
	adds r4, #1
	cmp r4, r8
	bne .L_081591f2
.L_08159268:
	ldr r6, [sp, #36]
	ldr r0, [sp, #32]
	ldr r1, [sp, #28]
	movs r2, #1
	add r9, r2
	adds r6, #24
	adds r0, #24
	subs r1, #48
	mov r3, r9
	str r6, [sp, #36]
	str r0, [sp, #32]
	str r1, [sp, #28]
	cmp r3, #3
	bne .L_081591c0
.L_08159284:
	ldr r4, [sp, #100]
	cmp r4, #179
	ble .L_0815928c
	b .L_081593ae
.L_0815928c:
	movs r5, #0
	cmp r4, #155
	ble .L_08159296
	adds r5, r4, #0
	subs r5, #156
.L_08159296:
	cmp r5, #7
	ble .L_0815929c
	movs r5, #7
.L_0815929c:
	ldr r6, [sp, #100]
	cmp r6, #139
	bgt .L_081592ec
	movs r0, #188
	movs r1, #3
	bl Func_081963ec
	movs r0, #192
	lsls r0, r0, #18
	adds r0, #188
	ldr r0, [r0]
	str r0, [sp, #76]
	b .L_081592fe
	.2byte 0x0000
.L_081592b8:
	.4byte 0x00000184
.L_081592bc:
	.4byte 0x00000154
.L_081592c0:
	.4byte 0x00000150
.L_081592c4:
	.4byte 0x00000152
.L_081592c8:
	.4byte 0x1a1a1a1a
.L_081592cc:
	.4byte IwramRatioMulQ14
.L_081592d0:
	.4byte 0x7fff0000
.L_081592d4:
	.4byte 0xfffff000
.L_081592d8:
	.4byte 0xfff26c00
.L_081592dc:
	.4byte Data_08196e2c
.L_081592e0:
	.4byte 0xffffff00
.L_081592e4:
	.4byte IwramMulQ16
.L_081592e8:
	.4byte Data_08197410
.L_081592ec:
	movs r1, #19
	movs r0, #188
	bl Func_081963ec
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #188
	ldr r1, [r1]
	str r1, [sp, #76]
.L_081592fe:
	ldr r2, [sp, #84]
	mov r8, r2
	cmp r2, #128
	ble .L_0815930a
	movs r3, #128
	mov r8, r3
.L_0815930a:
	movs r4, #0
	mov r6, r8
	mov r9, r4
	cmp r6, #0
	beq .L_081593a8
	lsls r3, r5, #1
	adds r3, r3, r5
	ldr r7, [sp, #88]
	lsls r3, r3, #7
	adds r3, r3, r5
	movs r0, #140
	lsls r3, r3, #1
	adds r7, #1
	lsls r0, r0, #1
	str r3, [sp, #72]
	add r0, sp
	lsls r1, r7, #1
	mov r10, r0
	add r6, sp, #148
	mov r11, r1
.L_08159332:
	mov r2, r9
	lsls r5, r2, #9
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r4, r10
	lsls r3, r3, #5
	str r3, [r4]
	adds r0, r5, #0
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #5
	negs r3, r3
	mov r5, r10
	str r3, [r5, #8]
	adds r1, r6, #0
	mov r0, r10
	bl Func_0815e1ec
	ldr r2, [r6]
	movs r0, #6
	ldrsh r3, [r6, r0]
	asrs r2, r2, #17
	ldr r0, .L_081595cc
	adds r2, #64
	adds r3, #60
	mov r1, r11
	str r2, [r6]
	str r3, [r6, #4]
	subs r1, #2
	ldrh r1, [r0, r1]
	ldr r4, [sp, #72]
	ldr r5, [sp, #112]
	movs r0, #184
	adds r1, r4, r1
	lsls r0, r0, #6
	adds r1, r5, r1
	adds r0, #16
	adds r1, r1, r0
	lsrs r0, r7, #31
	adds r0, r7, r0
	asrs r0, r0, #1
	mov r4, r11
	subs r2, r2, r0
	subs r3, r3, r7
	str r7, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #108]
	ldr r5, [sp, #76]
	mov lr, r5
	.2byte 0xf800
	movs r0, #1
	add r9, r0
	cmp r9, r8
	bne .L_08159332
.L_081593a8:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_081593ae:
	ldr r3, [sp, #100]
	subs r3, #151
	cmp r3, #16
	bhi .L_081594b6
	ldr r1, [sp, #24]
	ldr r2, [sp, #100]
	movs r4, #0
	mov r8, r1
	cmp r2, #151
	ble .L_081593c6
	adds r4, r2, #0
	subs r4, #152
.L_081593c6:
	cmp r4, #15
	ble .L_081593cc
	movs r4, #15
.L_081593cc:
	movs r3, #1
	ldr r5, [sp, #44]
	mov r9, r3
	lsls r3, r4, #1
	negs r3, r3
	adds r7, r3, #0
	adds r3, r4, r5
	adds r6, r4, #0
	adds r3, #1
	adds r7, #30
	adds r6, #49
	mov r10, r3
.L_081593e4:
	mov r0, r9
	adds r3, r4, r0
	cmp r3, #15
	bgt .L_08159406
	movs r2, #1
	movs r3, #16
	subs r3, r3, r0
	str r2, [sp, #4]
	str r4, [sp, #8]
	str r7, [sp, #0]
	ldr r0, [sp, #108]
	mov r1, r10
	adds r2, r6, #0
	ldr r5, [sp, #92]
	mov lr, r5
	.2byte 0xf800
	ldr r4, [sp, #8]
.L_08159406:
	movs r0, #1
	add r9, r0
	mov r1, r9
	subs r7, #2
	adds r6, #1
	add r10, r0
	cmp r1, #10
	bne .L_081593e4
	movs r2, #0
	mov r3, r8
	mov r9, r2
	cmp r3, #0
	beq .L_08159452
	lsls r2, r4, #1
	movs r3, #32
	adds r6, r4, #0
	subs r3, r3, r2
	adds r6, #48
	mov r10, r3
	movs r7, #1
.L_0815942e:
	ldr r2, [sp, #44]
	mov r3, r9
	mov r5, r10
	adds r1, r2, r4
	str r5, [sp, #0]
	str r4, [sp, #8]
	adds r3, #16
	str r7, [sp, #4]
	ldr r0, [sp, #108]
	adds r2, r6, #0
	ldr r5, [sp, #92]
	mov lr, r5
	.2byte 0xf800
	movs r0, #1
	add r9, r0
	ldr r4, [sp, #8]
	cmp r9, r8
	bne .L_0815942e
.L_08159452:
	movs r1, #19
	movs r0, #188
	bl Func_081963ec
	movs r6, #96
	movs r1, #32
	str r1, [sp, #0]
	str r6, [sp, #4]
	movs r2, #192
	ldr r5, [sp, #24]
	lsls r2, r2, #18
	ldr r3, [sp, #112]
	adds r2, #188
	ldr r4, [r2]
	movs r2, #224
	subs r5, #56
	lsls r2, r2, #3
	ldr r0, [sp, #108]
	mov r8, r1
	adds r1, r3, r2
	movs r2, #32
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	mov r3, r8
	str r3, [sp, #0]
	str r6, [sp, #4]
	ldr r2, [sp, #112]
	movs r6, #192
	lsls r6, r6, #18
	movs r3, #224
	lsls r3, r3, #3
	adds r6, #188
	adds r1, r2, r3
	ldr r4, [r6]
	ldr r0, [sp, #108]
	movs r2, #64
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_081594b6:
	ldr r4, [sp, #112]
	movs r5, #240
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r4, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #24]
	ldr r0, [sp, #20]
	ldr r1, [sp, #16]
	ldr r2, [sp, #100]
	adds r6, #20
	adds r0, #4
	adds r1, #2
	adds r2, #1
	str r6, [sp, #24]
	str r0, [sp, #20]
	str r1, [sp, #16]
	str r2, [sp, #100]
.L_081594e2:
	ldr r3, [sp, #100]
	cmp r3, #170
	beq .L_08159520
	ldr r5, [sp, #16]
	movs r4, #2
	str r4, [sp, #88]
	str r5, [sp, #84]
	cmp r3, #16
	bne .L_081594fa
	movs r0, #140
	bl Audio_PlayCue
.L_081594fa:
	ldr r6, [sp, #100]
	cmp r6, #132
	bne .L_08159506
	movs r0, #131
	bl Audio_PlayCue
.L_08159506:
	ldr r0, [sp, #100]
	cmp r0, #151
	bne .L_08159512
	movs r0, #145
	bl Audio_PlayCue
.L_08159512:
	ldr r3, .L_081595d0
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	bne .L_08159520
	b .L_08159018
.L_08159520:
	ldr r1, [sp, #116]
	movs r3, #0
	str r3, [r1, #16]
	ldr r0, .L_081595d4
	bl Scheduler_RemoveCallback
	bl Func_0814cca8
	movs r4, #240
	ldr r2, [sp, #112]
	lsls r4, r4, #7
	adds r4, #240
	adds r3, r2, r4
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #130
	movs r0, #9
	movs r2, #1
	bl Func_08152404
	ldr r0, .L_081595d8
	ldr r1, .L_081595dc
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r5, [sp, #112]
	movs r6, #224
	lsls r6, r6, #3
	adds r1, r5, r6
	ldr r0, .L_081595e0
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #206
	lsls r2, r2, #7
	adds r1, r5, r2
	movs r3, #0
	movs r2, #1
	ldr r0, .L_081595e4
	bl Func_08157cf4
	ldr r0, .L_081595e8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_081595ec
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #0
	ldr r1, [sp, #104]
	movs r3, #0
	ldr r0, .L_081595f0
	bl Func_08157cf4
	ldr r3, .L_081595c8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r4, #128
	movs r3, #160
	movs r5, #0
	lsls r3, r3, #15
	lsls r4, r4, #15
	str r3, [sp, #68]
	movs r2, #192
	str r4, [sp, #64]
	str r5, [sp, #60]
	str r5, [sp, #56]
	ldr r3, .L_081595f4
	lsls r2, r2, #2
	mov r9, r5
	movs r1, #0
	adds r2, #142
	b .L_081595f8
	.2byte 0x0000
.L_081595c8:
	.4byte 0x00001010
.L_081595cc:
	.4byte Data_08197410
.L_081595d0:
	.4byte gInput
.L_081595d4:
	.4byte Func_0813baec
.L_081595d8:
	.4byte 0x00000192
.L_081595dc:
	.4byte gMapCellBuffer
.L_081595e0:
	.4byte 0x00000194
.L_081595e4:
	.4byte 0x00000128
.L_081595e8:
	.4byte 0x00000193
.L_081595ec:
	.4byte IwramCopyWords
.L_081595f0:
	.4byte 0x00000135
.L_081595f4:
	.4byte Data_02010c70
.L_081595f8:
	movs r6, #1
	add r9, r6
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_081595f8
	ldr r5, [sp, #112]
	movs r0, #0
	mov r9, r0
.L_0815960a:
	bl Random16
	movs r3, #31
	ands r3, r0
	subs r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #63
	ands r3, r0
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	movs r1, #1
	ands r3, r0
	add r9, r1
	negs r3, r3
	mov r2, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #16
	bne .L_0815960a
	ldr r3, [sp, #112]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r5, [sp, #112]
	movs r6, #238
	lsls r6, r6, #7
	adds r6, #132
	adds r2, r5, r6
	movs r3, #50
	movs r0, #145
	str r3, [r2]
	bl Audio_PlayCue
	movs r0, #0
	str r0, [sp, #100]
.L_0815965c:
	ldr r1, [sp, #100]
	cmp r1, #20
	bne .L_08159674
	ldr r2, [sp, #112]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_081599d8
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
.L_08159674:
	ldr r4, [sp, #100]
	subs r4, #80
	str r4, [sp, #52]
	cmp r4, #59
	bhi .L_0815968e
	ldr r5, [sp, #100]
	movs r3, #7
	ands r3, r5
	cmp r3, #0
	bne .L_0815968e
	movs r0, #134
	bl Audio_PlayCue
.L_0815968e:
	ldr r6, [sp, #100]
	cmp r6, #120
	bne .L_0815969a
	movs r0, #134
	bl Func_081180e8
.L_0815969a:
	movs r0, #8
	movs r4, #0
	mov r10, r0
.L_081596a0:
	ldr r1, [sp, #100]
	lsls r2, r4, #7
	cmp r1, r10
	bge .L_081596aa
	b .L_081597b2
.L_081596aa:
	adds r3, r2, #0
	adds r3, #17
	cmp r1, r3
	bge .L_081597b2
	movs r3, #112
	mov r11, r3
	adds r3, r2, #0
	adds r5, r2, #0
	adds r3, #9
	adds r5, #12
	cmp r1, r3
	blt .L_081596e6
	cmp r1, r5
	bge .L_081596ec
	ldr r2, [sp, #112]
	movs r3, #48
	str r3, [sp, #0]
	movs r3, #224
	mov r6, r11
	lsls r3, r3, #3
	adds r1, r2, r3
	str r6, [sp, #4]
	str r4, [sp, #8]
	ldr r0, [sp, #108]
	movs r2, #36
	movs r3, #0
	ldr r6, [sp, #92]
	mov lr, r6
	.2byte 0xf800
	ldr r4, [sp, #8]
.L_081596e6:
	ldr r0, [sp, #100]
	cmp r0, r5
	blt .L_08159716
.L_081596ec:
	ldr r1, [sp, #100]
	mov r3, r10
	adds r3, #8
	cmp r1, r3
	bge .L_08159716
	movs r3, #48
	ldr r2, [sp, #112]
	str r3, [sp, #0]
	movs r3, #112
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #5
	adds r1, r2, r3
	str r4, [sp, #8]
	ldr r0, [sp, #108]
	movs r2, #36
	movs r3, #0
	ldr r5, [sp, #92]
	mov lr, r5
	.2byte 0xf800
	ldr r4, [sp, #8]
.L_08159716:
	ldr r6, [sp, #100]
	mov r3, r10
	adds r3, #2
	cmp r6, r3
	bne .L_081597b2
	ldr r7, .L_081599dc
	movs r0, #0
	mov r8, r0
	mov r9, r0
.L_08159728:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_08159794
	str r4, [sp, #8]
	bl Random16
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r5, #254
	ldr r1, .L_081599e0
	lsls r5, r5, #7
	movs r2, #60
	adds r5, #255
	lsls r3, r2, #16
	ands r5, r0
	mov r0, r11
	adds r5, r5, r1
	str r3, [r7]
	lsls r3, r0, #16
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r1, #1
	movs r2, #128
	adds r3, #32
	add r8, r1
	lsls r2, r2, #2
	str r3, [r7, #24]
	ldr r4, [sp, #8]
	cmp r8, r2
	beq .L_081597a4
.L_08159794:
	movs r5, #192
	movs r3, #1
	lsls r5, r5, #2
	add r9, r3
	adds r5, #142
	adds r7, #28
	cmp r9, r5
	bne .L_08159728
.L_081597a4:
	ldr r6, [sp, #112]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #168
	adds r2, r6, r0
	movs r3, #8
	str r3, [r2]
.L_081597b2:
	movs r1, #128
	adds r4, #1
	add r10, r1
	cmp r4, #1
	beq .L_081597be
	b .L_081596a0
.L_081597be:
	ldr r2, [sp, #100]
	cmp r2, #10
	bne .L_081597f8
	ldr r4, [sp, #120]
	movs r3, #0
	mov r9, r3
	ldr r3, [r4, #20]
	cmp r3, #0
	beq .L_081597f8
	movs r5, #36
.L_081597d2:
	ldr r6, [sp, #120]
	movs r3, #8
	ldrsh r0, [r5, r6]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r9
	bl Func_0814cd48
	ldrsh r0, [r5, r6]
	movs r1, #4
	bl Func_08118088
	movs r3, #1
	add r9, r3
	ldr r3, [r6, #20]
	adds r5, #2
	cmp r9, r3
	bne .L_081597d2
.L_081597f8:
	ldr r4, [sp, #52]
	cmp r4, #63
	bls .L_08159800
	b .L_08159a60
.L_08159800:
	ldr r6, .L_081599e4
	movs r5, #0
	mov r9, r5
	movs r5, #80
.L_08159808:
	ldr r0, [sp, #100]
	cmp r0, r5
	bne .L_0815981a
	movs r1, #128
	ldr r0, [sp, #108]
	lsls r1, r1, #7
	ldr r2, .L_081599e8
	mov lr, r6
	.2byte 0xf800
.L_0815981a:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #8
	cmp r2, #6
	bne .L_08159808
	movs r3, #0
	movs r4, #3
	movs r5, #80
	mov r9, r3
	mov r11, r4
	mov r10, r5
.L_08159832:
	ldr r0, [sp, #100]
	mov r6, r9
	lsls r3, r6, #1
	cmp r0, r10
	bge .L_0815983e
	b .L_08159974
.L_0815983e:
	adds r3, #82
	cmp r0, r3
	bge .L_0815989e
	bl Random16
	mov r1, r11
	adds r6, r0, #0
	ands r6, r1
	bl Random16
	mov r2, r11
	adds r5, r0, #0
	movs r1, #19
	movs r0, #188
	ands r5, r2
	bl Func_081963ec
	movs r1, #3
	mov r0, r9
	bl __modsi3
	lsls r1, r0, #2
	adds r1, r1, r0
	ldr r3, [sp, #112]
	lsls r1, r1, #4
	adds r1, r1, r0
	lsls r1, r1, #6
	adds r1, r3, r1
	movs r3, #72
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #192
	movs r4, #224
	lsls r0, r0, #18
	lsls r4, r4, #3
	adds r0, #188
	subs r6, #3
	adds r5, #32
	adds r1, r1, r4
	adds r2, r6, #0
	ldr r4, [r0]
	adds r3, r5, #0
	ldr r0, [sp, #108]
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
.L_0815989e:
	ldr r1, [sp, #100]
	cmp r1, r10
	bne .L_08159974
	ldr r7, .L_081599dc
	movs r2, #0
	mov r8, r2
	movs r4, #0
.L_081598ac:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_08159914
	str r4, [sp, #8]
	bl Random16
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r5, r0, #0
	adds r3, #255
	ands r5, r3
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r7]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r5, r8
	ldr r4, [sp, #8]
	cmp r5, #16
	beq .L_08159922
.L_08159914:
	movs r6, #192
	lsls r6, r6, #2
	adds r4, #1
	adds r6, #142
	adds r7, #28
	cmp r4, r6
	bne .L_081598ac
.L_08159922:
	ldr r0, [sp, #112]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r3, r0, r1
	movs r2, #8
	str r2, [r3]
	ldr r2, [sp, #120]
	movs r4, #0
	ldr r3, [r2, #20]
	cmp r3, #0
	beq .L_08159974
	ldr r6, .L_081599ec
	movs r5, #36
.L_0815993e:
	str r4, [sp, #8]
	bl Random16
	ldr r2, [sp, #120]
	ldr r4, [sp, #8]
	mov r3, r11
	ands r0, r3
	ldrb r1, [r6, r0]
	ldrsh r0, [r5, r2]
	movs r3, #4
	str r3, [sp, #0]
	movs r2, #5
	adds r3, r4, #0
	bl Func_0814cd48
	ldr r1, [sp, #120]
	ldrsh r0, [r5, r1]
	movs r1, #4
	bl Func_08118088
	ldr r0, [sp, #120]
	ldr r4, [sp, #8]
	ldr r3, [r0, #20]
	adds r4, #1
	adds r5, #2
	cmp r4, r3
	bne .L_0815993e
.L_08159974:
	movs r2, #1
	add r9, r2
	movs r1, #2
	mov r3, r9
	add r10, r1
	cmp r3, #18
	beq .L_08159984
	b .L_08159832
.L_08159984:
	bl Random16
	movs r6, #7
	ands r6, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #8
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #16
	adds r3, #72
	adds r0, r5, #0
	mov r11, r3
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	ldr r4, [sp, #112]
	movs r5, #206
	asrs r3, r3, #16
	movs r2, #32
	lsls r5, r5, #7
	subs r2, r2, r3
	adds r0, r4, r5
	mov r1, r11
	movs r3, #24
	mov r10, r2
	bl Func_0818caa8
	ldr r7, .L_081599dc
	movs r6, #0
	mov r8, r6
	mov r9, r6
	b .L_081599f0
.L_081599d8:
	.4byte 0x00000124
.L_081599dc:
	.4byte Data_02010c58
.L_081599e0:
	.4byte 0xffffc000
.L_081599e4:
	.4byte IwramFillWords
.L_081599e8:
	.4byte 0x10101010
.L_081599ec:
	.4byte Data_0819855e
.L_081599f0:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_08159a50
	bl Random16
	movs r5, #63
	ands r5, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r6, r0, #0
	adds r3, #255
	mov r0, r11
	mov r1, r10
	ands r6, r3
	lsls r3, r0, #16
	str r3, [r7]
	lsls r3, r1, #16
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r2, #1
	adds r3, #16
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	cmp r3, #4
	beq .L_08159a60
.L_08159a50:
	movs r5, #192
	movs r4, #1
	lsls r5, r5, #2
	add r9, r4
	adds r5, #142
	adds r7, #28
	cmp r9, r5
	bne .L_081599f0
.L_08159a60:
	movs r0, #188
	movs r1, #31
	bl Func_081963ec
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	ldr r5, .L_08159c84
	movs r6, #0
	mov r8, r3
	mov r9, r6
.L_08159a78:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_08159ae0
	subs r3, #1
	str r3, [r5, #24]
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #4]
	movs r0, #240
	lsls r0, r0, #15
	cmp r3, r0
	ble .L_08159aa4
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	b .L_08159ae0
.L_08159aa4:
	ldr r2, [r5]
	ldr r1, .L_08159c88
	cmp r2, r1
	bhi .L_08159ae0
	cmp r3, #0
	blt .L_08159ae0
	ldr r0, [r5, #24]
	asrs r6, r2, #16
	asrs r7, r3, #16
	cmp r0, #0
	bge .L_08159abc
	adds r0, #15
.L_08159abc:
	asrs r0, r0, #4
	adds r0, #1
	ldr r2, .L_08159c8c
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #104]
	subs r3, r7, r0
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	str r0, [sp, #0]
	subs r2, r6, r2
	str r4, [sp, #4]
	ldr r0, [sp, #108]
	mov lr, r8
	.2byte 0xf800
.L_08159ae0:
	movs r4, #192
	movs r3, #1
	lsls r4, r4, #2
	add r9, r3
	adds r4, #142
	adds r5, #28
	cmp r9, r4
	bne .L_08159a78
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #100]
	cmp r5, #15
	ble .L_08159b4e
	cmp r5, #32
	bne .L_08159b0a
	ldr r0, .L_08159c90
	movs r6, #128
	lsls r6, r6, #11
	str r6, [sp, #60]
	str r0, [sp, #56]
.L_08159b0a:
	ldr r1, [sp, #100]
	cmp r1, #31
	ble .L_08159b44
	ldr r2, [sp, #68]
	ldr r3, [sp, #60]
	ldr r6, [sp, #60]
	ldr r4, [sp, #64]
	ldr r5, [sp, #56]
	adds r2, r2, r3
	lsls r3, r3, #4
	subs r3, r3, r6
	adds r4, r4, r5
	lsls r3, r3, #2
	str r2, [sp, #68]
	str r4, [sp, #64]
	cmp r3, #0
	bge .L_08159b2e
	adds r3, #63
.L_08159b2e:
	ldr r0, [sp, #56]
	asrs r3, r3, #6
	str r3, [sp, #60]
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_08159b40
	adds r3, #63
.L_08159b40:
	asrs r3, r3, #6
	str r3, [sp, #56]
.L_08159b44:
	movs r0, #0
	ldr r1, [sp, #68]
	ldr r2, [sp, #64]
	bl Func_0816442c
.L_08159b4e:
	ldr r3, [sp, #100]
	subs r3, #16
	cmp r3, #127
	bhi .L_08159c02
	ldr r2, [sp, #68]
	movs r1, #0
	asrs r2, r2, #17
	mov r9, r1
	mov r11, r2
	movs r7, #3
.L_08159b62:
	mov r4, r9
	ands r4, r7
	str r4, [sp, #8]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	ldr r3, .L_08159c94
	ldr r4, [sp, #8]
	adds r6, r0, #0
	mov r10, r3
	ldrb r3, [r3, r4]
	lsls r6, r6, #3
	asrs r6, r6, #16
	lsrs r3, r3, #1
	adds r0, r5, #0
	add r6, r11
	subs r6, r6, r3
	bl Trig_Cos
	lsls r5, r0, #2
	ldr r4, [sp, #8]
	adds r5, r5, r0
	ldr r0, .L_08159c98
	lsls r5, r5, #3
	ldrb r3, [r0, r4]
	asrs r5, r5, #16
	lsrs r3, r3, #1
	subs r5, r5, r3
	mov r8, r0
	bl Random16
	ldr r3, .L_08159c9c
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #1
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #188
	bl Func_08196404
	ldr r4, [sp, #8]
	ldr r2, .L_08159ca0
	lsls r3, r4, #1
	mov r0, r10
	ldrh r1, [r2, r3]
	ldrb r3, [r0, r4]
	ldr r2, .L_08159ca4
	str r3, [sp, #0]
	adds r1, r1, r2
	mov r2, r8
	ldrb r3, [r2, r4]
	adds r5, #56
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r4, [r3]
	ldr r0, [sp, #108]
	adds r3, r5, #0
	adds r2, r6, #0
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r4, #1
	add r9, r4
	mov r5, r9
	cmp r5, #3
	bne .L_08159b62
.L_08159c02:
	ldr r6, [sp, #100]
	cmp r6, #31
	bgt .L_08159c12
	movs r0, #8
	movs r1, #16
	bl Func_08158ce0
	b .L_08159c1a
.L_08159c12:
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
.L_08159c1a:
	bl Func_081434f8
	movs r1, #240
	ldr r0, [sp, #112]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #100]
	adds r2, #1
	str r2, [sp, #100]
	cmp r2, #128
	beq .L_08159c3e
	b .L_0815965c
.L_08159c3e:
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_08159ca8
	bl Scheduler_RemoveCallback
	movs r0, #0
	ldr r1, [sp, #68]
	ldr r2, [sp, #64]
	bl Func_0816467c
	movs r6, #238
	ldr r4, [sp, #112]
	lsls r6, r6, #7
	movs r3, #0
	adds r6, #220
	mov r9, r3
	adds r5, r4, r6
.L_08159c62:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r0, #1
	add r9, r0
	mov r1, r9
	cmp r1, #9
	bne .L_08159c62
	bl Func_08143bb8
	add sp, #324
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08159c84:
	.4byte Data_02010c58
.L_08159c88:
	.4byte 0x007effff
.L_08159c8c:
	.4byte Data_08197410
.L_08159c90:
	.4byte 0xffffc000
.L_08159c94:
	.4byte Data_08197492
.L_08159c98:
	.4byte Data_08197498
.L_08159c9c:
	.4byte Data_08198562
.L_08159ca0:
	.4byte Data_08197486
.L_08159ca4:
	.4byte gMapCellBuffer
.L_08159ca8:
	.4byte Func_08143000
