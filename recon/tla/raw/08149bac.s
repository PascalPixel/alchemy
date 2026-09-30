.syntax unified
	.thumb
	.global BattleFx_RunSparkGroups
	.thumb_func
BattleFx_RunSparkGroups:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #76
	str r1, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	mov r10, r0
	str r1, [sp, #48]
	ldr r2, [r3, #96]
	str r2, [sp, #44]
	ldr r3, [r3, #100]
	str r3, [sp, #32]
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_08149be6
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r5, #48
	movs r4, #60
	str r4, [sp, #28]
	str r5, [sp, #24]
	b .L_08149c38
.L_08149be6:
	ldr r1, [sp, #52]
	cmp r1, #1
	bne .L_08149c16
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	movs r5, #238
	ldr r4, [sp, #48]
	lsls r5, r5, #7
	movs r1, #238
	movs r2, #60
	movs r3, #64
	adds r5, #180
	lsls r1, r1, #7
	str r2, [sp, #28]
	str r3, [sp, #24]
	adds r2, r4, r5
	movs r3, #24
	adds r1, #184
	str r3, [r2]
	adds r2, r4, r1
	movs r3, #0
	str r3, [r2]
	b .L_08149c38
.L_08149c16:
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	mov r2, r10
	add r5, sp, #64
	ldr r0, [r2, #8]
	adds r1, r5, #0
	bl Func_0815e20c
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #28]
	ldr r3, [r5, #4]
	adds r3, #48
	str r3, [sp, #24]
.L_08149c38:
	ldr r3, .L_08149c74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	mov r3, sp
	adds r3, #56
	adds r1, r3, #0
	movs r0, #0
	str r3, [sp, #20]
	bl Func_08144aac
	ldr r4, [sp, #48]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r4, r5
	ldr r0, .L_08149c78
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_08149c7c
	ldr r1, [sp, #32]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r1, [sp, #52]
	b .L_08149c80
	.2byte 0x0000
.L_08149c74:
	.4byte 0x00001010
.L_08149c78:
	.4byte 0x0000013e
.L_08149c7c:
	.4byte 0x00000134
.L_08149c80:
	cmp r1, #1
	bne .L_08149c9a
	ldr r0, .L_08149fec
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08149ff0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_08149cb4
.L_08149c9a:
	ldr r2, [sp, #52]
	cmp r2, #2
	bne .L_08149cb4
	ldr r0, .L_08149ff4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08149ff0
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08149cb4:
	movs r3, #0
	str r3, [sp, #40]
	mov r4, r10
	ldr r3, [r4, #24]
	ldr r1, .L_08149ff8
	lsls r2, r3, #2
	adds r2, r2, r3
	lsls r2, r2, #1
	adds r2, #2
	ldrh r3, [r1, r2]
	cmp r3, #0
	bne .L_08149cce
	b .L_08149e1e
.L_08149cce:
	movs r5, #0
	str r5, [sp, #8]
.L_08149cd2:
	ldr r2, [sp, #8]
	ldr r3, [sp, #48]
	movs r1, #0
	mov r9, r1
	adds r7, r2, r3
.L_08149cdc:
	mov r4, r9
	lsls r6, r4, #1
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
	mov r5, r9
	negs r3, r3
	str r3, [r7, #4]
	lsrs r3, r5, #31
	add r3, r9
	movs r1, #1
	asrs r3, r3, #1
	add r9, r1
	adds r3, #25
	mov r2, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #16
	bne .L_08149cdc
	mov r4, r10
	ldr r1, [r4, #24]
	movs r3, #0
	ldr r0, .L_08149ff8
	mov r9, r3
	lsls r3, r1, #2
	adds r3, r3, r1
	lsls r3, r3, #1
	ldrh r3, [r0, r3]
	adds r7, r0, #0
	adds r2, r1, #0
	cmp r3, #0
	beq .L_08149dfe
	ldr r5, [sp, #24]
	lsls r5, r5, #16
	mov r11, r5
.L_08149d40:
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r7, r3]
	ldr r1, [sp, #40]
	adds r2, r3, #0
	muls r2, r1
	add r2, r9
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_08149ffc
	lsls r3, r3, #2
	adds r5, r3, r2
	bl Random16
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r0
	adds r3, #32
	mov r8, r3
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	mov r4, r10
	ands r6, r3
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_08149d98
	ldr r3, [r4, #24]
	ldr r1, [sp, #40]
	lsls r2, r3, #2
	adds r2, r2, r3
	adds r2, r2, r1
	lsls r2, r2, #1
	adds r2, #4
	ldrh r3, [r7, r2]
	ldr r2, [sp, #28]
	subs r3, r2, r3
	adds r3, #28
	b .L_08149db0
.L_08149d98:
	mov r4, r10
	ldr r3, [r4, #24]
	ldr r1, [sp, #40]
	lsls r2, r3, #2
	adds r2, r2, r3
	adds r2, r2, r1
	lsls r2, r2, #1
	adds r2, #4
	ldrh r3, [r7, r2]
	ldr r2, [sp, #28]
	adds r3, r2, r3
	subs r3, #28
.L_08149db0:
	lsls r3, r3, #16
	str r3, [r5]
	mov r3, r11
	str r3, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r5, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r5, #24]
	mov r5, r10
	ldr r1, [r5, #24]
	ldr r0, .L_08149ff8
	lsls r3, r1, #2
	adds r3, r3, r1
	lsls r3, r3, #1
	ldrh r3, [r0, r3]
	movs r4, #1
	add r9, r4
	adds r7, r0, #0
	adds r2, r1, #0
	cmp r9, r3
	bne .L_08149d40
.L_08149dfe:
	ldr r2, [sp, #8]
	ldr r4, [sp, #40]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r2, r3
	adds r4, #1
	lsls r3, r1, #2
	str r2, [sp, #8]
	str r4, [sp, #40]
	adds r3, r3, r1
	lsls r3, r3, #1
	adds r3, #2
	ldrh r3, [r0, r3]
	cmp r4, r3
	beq .L_08149e1e
	b .L_08149cd2
.L_08149e1e:
	ldr r5, [sp, #48]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r5, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r5, r3
	movs r1, #200
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0814a000
	bl Scheduler_AddOrUpdateCallback
	movs r4, #0
	str r4, [sp, #36]
	mov r5, r10
	ldr r1, [r5, #24]
	ldr r2, .L_08149ff8
	adds r0, r1, #0
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, #2
	ldrh r3, [r2, r3]
	ldr r4, .L_0814a004
	cmp r3, r4
	bne .L_08149e5e
	b .L_0814a222
.L_08149e5e:
	ldr r5, [sp, #52]
	subs r5, #1
	str r5, [sp, #16]
.L_08149e64:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #48]
	cmp r0, #2
	bne .L_08149e96
	ldr r3, [sp, #36]
	cmp r3, #51
	bgt .L_08149e96
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08149e8a
	ldrh r3, [r2, #54]
	movs r5, #128
	lsls r5, r5, #1
	adds r3, r3, r5
	strh r3, [r2, #54]
	ldr r1, [r4, #24]
	b .L_08149e96
.L_08149e8a:
	ldrh r3, [r2, #54]
	ldr r1, .L_0814a008
	adds r3, r3, r1
	strh r3, [r2, #54]
	mov r2, r10
	ldr r1, [r2, #24]
.L_08149e96:
	cmp r1, #3
	bne .L_08149eae
	ldr r3, [sp, #36]
	cmp r3, #4
	bne .L_08149eae
	movs r1, #128
	ldr r3, .L_0814a00c
	ldr r0, [sp, #44]
	lsls r1, r1, #7
	ldr r2, .L_0814a010
	mov lr, r3
	.2byte 0xf800
.L_08149eae:
	ldr r4, [sp, #16]
	cmp r4, #1
	bhi .L_08149ec8
	ldr r5, [sp, #36]
	cmp r5, #2
	bne .L_08149ee0
	movs r0, #145
	bl Audio_PlayCue
	movs r0, #145
	bl Func_081180e8
	b .L_08149ee0
.L_08149ec8:
	ldr r1, [sp, #36]
	cmp r1, #2
	bne .L_08149ed4
	movs r0, #145
	bl Audio_PlayCue
.L_08149ed4:
	ldr r2, [sp, #36]
	cmp r2, #24
	bne .L_08149ee0
	movs r0, #134
	bl Func_081180e8
.L_08149ee0:
	movs r3, #0
	str r3, [sp, #40]
	mov r4, r10
	ldr r3, [r4, #24]
	ldr r5, .L_08149ff8
	lsls r2, r3, #2
	adds r2, r2, r3
	lsls r2, r2, #1
	adds r2, #2
	ldrh r3, [r5, r2]
	cmp r3, #0
	bne .L_08149efa
	b .L_0814a1e0
.L_08149efa:
	movs r1, #0
	str r1, [sp, #12]
.L_08149efe:
	ldr r2, [sp, #40]
	ldr r3, [sp, #36]
	lsls r2, r2, #3
	mov r11, r2
	cmp r3, r11
	bne .L_08149f18
	ldr r4, [sp, #48]
	movs r5, #238
	lsls r5, r5, #7
	adds r5, #168
	adds r2, r4, r5
	movs r3, #12
	str r3, [r2]
.L_08149f18:
	ldr r1, [sp, #36]
	cmp r1, r11
	bge .L_08149f20
	b .L_0814a086
.L_08149f20:
	mov r3, r11
	adds r3, #2
	cmp r1, r3
	bge .L_08149f9c
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_08149f66
	ldr r2, [r2, #24]
	ldr r4, [sp, #40]
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r3, r3, r4
	ldr r5, .L_08149ff8
	lsls r3, r3, #1
	adds r3, #4
	ldrh r2, [r5, r3]
	movs r3, #32
	str r3, [sp, #0]
	movs r3, #64
	ldr r1, [sp, #28]
	str r3, [sp, #4]
	ldr r3, [sp, #48]
	movs r5, #224
	lsls r5, r5, #3
	subs r2, r1, r2
	adds r1, r3, r5
	ldr r3, [sp, #24]
	adds r2, #12
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	subs r3, #32
	mov lr, r4
	.2byte 0xf800
	b .L_08149f9c
.L_08149f66:
	mov r1, r10
	ldr r2, [r1, #24]
	ldr r4, .L_08149ff8
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [sp, #40]
	ldr r5, [sp, #28]
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, #4
	ldrh r2, [r4, r3]
	movs r3, #32
	str r3, [sp, #0]
	movs r3, #64
	str r3, [sp, #4]
	ldr r3, [sp, #48]
	adds r2, r5, r2
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r3, r5
	ldr r3, [sp, #24]
	subs r2, #44
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	subs r3, #32
	mov lr, r4
	.2byte 0xf800
.L_08149f9c:
	ldr r1, [sp, #36]
	cmp r1, r11
	blt .L_0814a086
	ldr r3, .L_08149ff8
	ldr r4, [sp, #12]
	mov r8, r3
	ldr r1, [sp, #48]
	lsls r3, r4, #3
	subs r3, r3, r4
	movs r2, #0
	lsls r3, r3, #2
	mov r9, r2
	adds r5, r3, r1
.L_08149fb6:
	movs r2, #6
	ldrsh r3, [r5, r2]
	ldr r4, [sp, #24]
	mov r1, r10
	adds r7, r3, r4
	ldr r3, [r1, #4]
	cmp r3, #1
	bne .L_0814a014
	mov r4, r10
	movs r2, #2
	ldrsh r1, [r5, r2]
	ldr r3, [sp, #28]
	ldr r2, [r4, #24]
	adds r1, r1, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [sp, #40]
	mov r4, r8
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, #4
	ldrh r3, [r4, r3]
	subs r1, r1, r3
	adds r6, r1, #0
	adds r6, #28
	b .L_0814a036
	.2byte 0x0000
.L_08149fec:
	.4byte 0x00000148
.L_08149ff0:
	.4byte IwramCopyWords
.L_08149ff4:
	.4byte 0x00000188
.L_08149ff8:
	.4byte Data_081979de
.L_08149ffc:
	.4byte gMapCellBuffer
.L_0814a000:
	.4byte Func_08143000
.L_0814a004:
	.4byte 0x1ffffffb
.L_0814a008:
	.4byte 0xffffff00
.L_0814a00c:
	.4byte IwramFillWords
.L_0814a010:
	.4byte 0x3f3f3f3f
.L_0814a014:
	mov r4, r10
	movs r2, #2
	ldrsh r1, [r5, r2]
	ldr r3, [sp, #28]
	ldr r2, [r4, #24]
	adds r1, r1, r3
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, [sp, #40]
	mov r4, r8
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r3, #4
	ldrh r3, [r4, r3]
	adds r1, r1, r3
	adds r6, r1, #0
	subs r6, #28
.L_0814a036:
	ldr r0, [r5, #24]
	cmp r0, #17
	bhi .L_0814a06c
	movs r1, #3
	bl Math_Div
	ldr r2, .L_0814a248
	ldr r3, [sp, #48]
	ldrb r1, [r2, r0]
	movs r4, #224
	movs r0, #32
	lsls r1, r1, #11
	adds r1, r3, r1
	lsls r4, r4, #3
	str r0, [sp, #0]
	adds r2, r6, #0
	movs r0, #64
	adds r3, r7, #0
	adds r1, r1, r4
	str r0, [sp, #4]
	subs r2, #16
	subs r3, #32
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [r5, #24]
.L_0814a06c:
	cmp r0, #0
	ble .L_0814a074
	subs r3, r0, #1
	b .L_0814a078
.L_0814a074:
	movs r3, #1
	negs r3, r3
.L_0814a078:
	str r3, [r5, #24]
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #28
	cmp r2, #12
	bne .L_08149fb6
.L_0814a086:
	ldr r4, [sp, #36]
	mov r3, r11
	adds r3, #5
	cmp r4, r3
	ble .L_0814a17c
	ldr r5, [sp, #52]
	ldr r7, .L_0814a24c
	cmp r5, #2
	beq .L_0814a09c
	movs r7, #128
	lsls r7, r7, #5
.L_0814a09c:
	movs r1, #0
	mov r2, r10
	mov r9, r1
	ldr r1, [r2, #24]
	ldr r0, .L_0814a250
	lsls r3, r1, #2
	adds r3, r3, r1
	adds r4, r0, #0
	lsls r3, r3, #1
	ldrh r3, [r4, r3]
	adds r2, r1, #0
	cmp r3, #0
	beq .L_0814a17c
.L_0814a0b6:
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r4, r3]
	ldr r4, [sp, #40]
	ldr r5, .L_0814a254
	adds r2, r3, #0
	muls r2, r4
	add r2, r9
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r6, r3, r5
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_0814a168
	movs r1, #60
	adds r2, r7, #0
	adds r0, r6, #0
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #24]
	ldr r2, [r6, #4]
	movs r1, #216
	subs r3, #1
	lsls r1, r1, #15
	str r3, [r6, #24]
	cmp r2, r1
	ble .L_0814a104
	ldr r3, [r6, #16]
	ldr r0, .L_0814a250
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	mov r2, r10
	ldr r1, [r2, #24]
	b .L_0814a168
.L_0814a104:
	ldr r0, [r6]
	ldr r4, .L_0814a258
	cmp r0, r4
	bhi .L_0814a15a
	cmp r2, #0
	blt .L_0814a162
	asrs r2, r2, #16
	asrs r6, r0, #16
	movs r1, #5
	adds r0, r3, #0
	mov r8, r2
	bl Math_Div
	adds r0, #1
	ldr r2, .L_0814a25c
	mov r5, r9
	movs r4, #1
	ands r4, r5
	lsls r5, r0, #1
	subs r3, r5, #2
	ldrh r1, [r2, r3]
	lsrs r3, r0, #31
	ldr r2, [sp, #32]
	str r0, [sp, #0]
	str r5, [sp, #4]
	adds r3, r0, r3
	ldr r5, [sp, #20]
	asrs r3, r3, #1
	subs r6, r6, r3
	lsls r4, r4, #2
	mov r3, r8
	adds r1, r2, r1
	subs r3, r3, r0
	adds r2, r6, #0
	ldr r4, [r4, r5]
	ldr r0, [sp, #44]
	mov r8, r3
	mov lr, r4
	.2byte 0xf800
	mov r2, r10
	ldr r0, .L_0814a250
	ldr r1, [r2, #24]
	b .L_0814a168
.L_0814a15a:
	mov r3, r10
	ldr r0, .L_0814a250
	ldr r1, [r3, #24]
	b .L_0814a168
.L_0814a162:
	mov r4, r10
	ldr r0, .L_0814a250
	ldr r1, [r4, #24]
.L_0814a168:
	lsls r3, r1, #2
	adds r3, r3, r1
	adds r4, r0, #0
	lsls r3, r3, #1
	ldrh r3, [r4, r3]
	movs r5, #1
	add r9, r5
	adds r2, r1, #0
	cmp r9, r3
	bne .L_0814a0b6
.L_0814a17c:
	mov r2, r10
	ldr r3, [r2, #20]
	movs r1, #0
	mov r9, r1
	cmp r3, #0
	beq .L_0814a1be
	mov r6, r11
	adds r6, #6
	movs r5, #36
.L_0814a18e:
	ldr r4, [sp, #36]
	cmp r4, r6
	bne .L_0814a1b4
	mov r1, r10
	movs r3, #10
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r9
	bl Func_0814cd48
	mov r3, r10
	ldrsh r0, [r5, r3]
	movs r1, #4
	bl Func_08118088
	mov r1, r10
	ldr r3, [r1, #20]
.L_0814a1b4:
	movs r2, #1
	add r9, r2
	adds r5, #2
	cmp r9, r3
	bne .L_0814a18e
.L_0814a1be:
	ldr r3, [sp, #12]
	ldr r4, [sp, #40]
	adds r3, #16
	adds r4, #1
	str r3, [sp, #12]
	str r4, [sp, #40]
	mov r5, r10
	ldr r3, [r5, #24]
	ldr r1, .L_0814a250
	lsls r2, r3, #2
	adds r2, r2, r3
	lsls r2, r2, #1
	adds r2, #2
	ldrh r3, [r1, r2]
	cmp r4, r3
	beq .L_0814a1e0
	b .L_08149efe
.L_0814a1e0:
	movs r1, #16
	movs r0, #16
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
	ldr r5, [sp, #36]
	mov r1, r10
	adds r5, #1
	str r5, [sp, #36]
	ldr r2, .L_0814a250
	ldr r0, [r1, #24]
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r3, #2
	ldrh r3, [r2, r3]
	adds r1, r0, #0
	lsls r3, r3, #3
	adds r3, #40
	cmp r5, r3
	beq .L_0814a222
	b .L_08149e64
.L_0814a222:
	ldr r0, .L_0814a260
	bl Scheduler_RemoveCallback
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
.L_0814a248:
	.4byte Data_08197a1a
.L_0814a24c:
	.4byte 0xfffff000
.L_0814a250:
	.4byte Data_081979de
.L_0814a254:
	.4byte gMapCellBuffer
.L_0814a258:
	.4byte 0x007effff
.L_0814a25c:
	.4byte Data_08197410
.L_0814a260:
	.4byte Func_08143000
