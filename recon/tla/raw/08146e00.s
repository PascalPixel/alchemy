.syntax unified
	.thumb
	.global Func_08146e00
	.thumb_func
Func_08146e00:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #364
	str r0, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	movs r6, #224
	str r0, [sp, #96]
	movs r0, #0
	ldr r1, [r3, #48]
	lsls r6, r6, #3
	str r1, [sp, #92]
	movs r7, #239
	ldr r2, [r3, #92]
	lsls r7, r7, #7
	str r2, [sp, #88]
	ldr r3, [r3, #100]
	str r3, [sp, #80]
	bl BattleFx_BeginCanvasLayer
	mov r3, sp
	adds r3, #112
	adds r1, r3, #0
	movs r0, #0
	str r3, [sp, #76]
	bl Func_08144aac
	ldr r5, [sp, #88]
	ldr r0, .L_08147114
	adds r1, r5, r6
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r0, .L_08147118
	ldr r1, [sp, #80]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	movs r0, #238
	lsls r0, r0, #7
	adds r2, r5, r7
	movs r3, #2
	adds r0, #132
	str r3, [r2]
	movs r1, #200
	adds r2, r5, r0
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0814711c
	bl Scheduler_AddOrUpdateCallback
	ldr r5, [sp, #100]
	movs r3, #0
	str r3, [sp, #60]
	str r3, [sp, #64]
	mov r9, r3
	ldr r3, [r5, #20]
	movs r1, #128
	movs r2, #160
	lsls r1, r1, #16
	lsls r2, r2, #14
	str r1, [sp, #68]
	str r2, [sp, #72]
	cmp r3, #0
	beq .L_08146f1c
	movs r7, #166
	movs r6, #172
	lsls r7, r7, #1
	add r6, sp
	add r7, sp
	movs r0, #36
	mov r11, r6
	mov r10, r7
	add r2, sp, #300
	add r7, sp, #268
	movs r4, #0
	add r6, sp, #204
	mov r8, r0
.L_08146eae:
	ldr r5, [sp, #100]
	mov r1, r8
	ldrsh r0, [r1, r5]
	str r2, [sp, #12]
	str r4, [sp, #8]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	ldr r4, [sp, #8]
	ldr r3, [r5, #8]
	mov r0, r11
	str r3, [r6]
	ldr r3, [r5, #16]
	str r3, [r6, #4]
	adds r6, #8
	ldrh r3, [r5, #6]
	str r3, [r4, r0]
	ldr r1, [r5, #16]
	ldr r0, [r5, #8]
	bl ArcTan2
	ldr r4, [sp, #8]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	mov r1, r10
	str r0, [r4, r1]
	ldr r3, [r5, #8]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r5, #16]
	asrs r3, r3, #8
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_08147120
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #8]
	ldr r2, [sp, #12]
	movs r3, #0
	asrs r0, r0, #7
	str r0, [r4, r2]
	str r3, [r4, r7]
	str r3, [r5, #72]
	ldr r0, [sp, #100]
	movs r3, #2
	add r8, r3
	ldr r3, [r0, #20]
	movs r5, #1
	add r9, r5
	adds r4, #4
	cmp r9, r3
	bne .L_08146eae
.L_08146f1c:
	ldr r5, [sp, #88]
	movs r1, #0
	mov r9, r1
	movs r6, #0
.L_08146f24:
	movs r3, #120
	str r3, [r5, #8]
	str r6, [r5, #4]
	bl Random16
	str r6, [r5, #16]
	str r6, [r5, #12]
	bl Random16
	movs r3, #63
	movs r2, #1
	ands r3, r0
	add r9, r2
	str r3, [r5, #24]
	mov r3, r9
	adds r5, #28
	cmp r3, #64
	bne .L_08146f24
	ldr r6, [sp, #92]
	movs r5, #0
	adds r6, #12
	str r5, [sp, #84]
	str r6, [sp, #40]
.L_08146f52:
	ldr r3, .L_08147124
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_08146f7a
	ldr r7, [sp, #84]
	cmp r7, #159
	bgt .L_08146f6a
	movs r0, #160
	str r0, [sp, #84]
	b .L_08146f7a
.L_08146f6a:
	ldr r1, [sp, #84]
	movs r2, #197
	lsls r2, r2, #1
	cmp r1, r2
	bgt .L_08146f7a
	movs r3, #140
	adds r3, #255
	str r3, [sp, #84]
.L_08146f7a:
	bl Func_08014de4
	ldr r0, [sp, #92]
	ldr r1, [sp, #40]
	bl Graphics_PrepareTransferInIwramWork
	ldr r5, [sp, #84]
	cmp r5, #16
	bne .L_08146f92
	movs r0, #141
	bl Audio_PlayCue
.L_08146f92:
	ldr r6, [sp, #84]
	movs r7, #128
	lsls r7, r7, #1
	cmp r6, r7
	bne .L_08146fa2
	movs r0, #140
	bl Audio_PlayCue
.L_08146fa2:
	ldr r0, [sp, #84]
	movs r1, #167
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_08146fb2
	movs r0, #212
	bl Audio_PlayCue
.L_08146fb2:
	ldr r2, [sp, #84]
	movs r3, #92
	adds r3, #255
	cmp r2, r3
	bne .L_08146fc2
	movs r0, #212
	bl Audio_PlayCue
.L_08146fc2:
	ldr r5, [sp, #84]
	movs r6, #104
	adds r6, #255
	cmp r5, r6
	bne .L_08146fd2
	movs r0, #212
	bl Audio_PlayCue
.L_08146fd2:
	ldr r7, [sp, #84]
	movs r0, #186
	lsls r0, r0, #1
	cmp r7, r0
	bne .L_08146fe2
	movs r0, #212
	bl Audio_PlayCue
.L_08146fe2:
	ldr r2, [sp, #100]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r9, r1
	cmp r3, #0
	bne .L_08146ff0
	b .L_08147146
.L_08146ff0:
	movs r5, #36
	str r5, [sp, #16]
	movs r4, #0
.L_08146ff6:
	ldr r0, [sp, #84]
	mov r6, r9
	lsls r7, r6, #4
	cmp r0, r7
	bgt .L_08147002
	b .L_08147134
.L_08147002:
	lsls r6, r6, #1
	str r6, [sp, #56]
	ldr r3, [sp, #100]
	ldr r1, [sp, #16]
	str r4, [sp, #8]
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r4, [sp, #8]
	add r6, sp, #332
	ldr r5, [r0]
	ldr r0, [r6, r4]
	mov r8, r4
	bl Trig_Sin
	ldr r4, [sp, #8]
	add r2, sp, #300
	ldr r3, [r2, r4]
	mov r10, r2
	muls r3, r0
	asrs r3, r3, #1
	str r3, [r5, #8]
	str r2, [sp, #12]
	ldr r0, [r6, r4]
	bl Trig_Cos
	ldr r2, [sp, #12]
	ldr r4, [sp, #8]
	ldr r3, [r2, r4]
	muls r3, r0
	asrs r3, r3, #1
	str r3, [r5, #16]
	ldr r0, [sp, #84]
	cmp r0, #159
	bgt .L_0814707e
	adds r3, r7, #0
	adds r3, #16
	cmp r0, r3
	ble .L_08147058
	add r7, sp, #268
	ldr r3, [r7, r4]
	adds r3, #48
	str r3, [r7, r4]
.L_08147058:
	ldr r3, [r2, r4]
	cmp r3, #31
	bgt .L_08147068
	ldr r3, [r5, #12]
	movs r1, #192
	lsls r1, r1, #11
	adds r3, r3, r1
	b .L_08147070
.L_08147068:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
.L_08147070:
	str r3, [r5, #12]
	ldr r3, [r5, #12]
	movs r2, #248
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_0814707e
	str r2, [r5, #12]
.L_0814707e:
	ldr r3, [sp, #84]
	movs r7, #245
	lsls r7, r7, #1
	cmp r3, r7
	bgt .L_081470b4
	add r7, sp, #268
	ldr r2, [r6, r4]
	ldr r3, [r7, r4]
	movs r0, #128
	adds r2, r2, r3
	lsls r0, r0, #9
	str r2, [r6, r4]
	cmp r2, r0
	ble .L_081470a2
	ldr r1, .L_08147128
	adds r3, r2, r1
	mov r2, r8
	str r3, [r6, r2]
.L_081470a2:
	mov r3, r8
	ldr r2, [r7, r3]
	cmp r2, #0
	bge .L_081470ac
	adds r2, #3
.L_081470ac:
	ldrh r3, [r5, #6]
	asrs r2, r2, #2
	adds r3, r3, r2
	strh r3, [r5, #6]
.L_081470b4:
	ldr r6, [sp, #84]
	movs r7, #140
	adds r7, #255
	cmp r6, r7
	bne .L_081470c6
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r5, #72]
.L_081470c6:
	ldr r6, .L_0814712c
	movs r0, #0
	mov r11, r0
.L_081470cc:
	ldrh r3, [r6]
	ldr r1, [sp, #84]
	adds r6, #2
	cmp r1, r3
	bne .L_081470f4
	movs r3, #0
	str r3, [r5, #40]
	ldr r3, [sp, #56]
	ldr r2, [sp, #100]
	adds r3, #36
	ldrsh r0, [r2, r3]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r9
	str r4, [sp, #8]
	bl Func_0814cd48
	ldr r4, [sp, #8]
.L_081470f4:
	movs r0, #1
	add r11, r0
	mov r1, r11
	cmp r1, #4
	bne .L_081470cc
	mov r2, r10
	ldr r3, [r2, r4]
	cmp r3, #16
	ble .L_08147130
	subs r3, #2
	mov r5, r8
	str r3, [r2, r5]
	ldr r6, [sp, #100]
	ldr r3, [r6, #20]
	b .L_08147134
	.2byte 0x0000
.L_08147114:
	.4byte 0x00000143
.L_08147118:
	.4byte 0x00000134
.L_0814711c:
	.4byte Func_08143000
.L_08147120:
	.4byte IwramFillWords + 0x74
.L_08147124:
	.4byte gInput
.L_08147128:
	.4byte 0xffff0000
.L_0814712c:
	.4byte Data_08197950
.L_08147130:
	ldr r7, [sp, #100]
	ldr r3, [r7, #20]
.L_08147134:
	ldr r0, [sp, #16]
	movs r1, #1
	adds r0, #2
	add r9, r1
	adds r4, #4
	str r0, [sp, #16]
	cmp r9, r3
	beq .L_08147146
	b .L_08146ff6
.L_08147146:
	ldr r3, [sp, #84]
	subs r3, #16
	cmp r3, #143
	bhi .L_081471a0
	ldr r2, [sp, #84]
	lsls r3, r2, #1
	adds r5, r3, #0
	subs r5, #32
	cmp r5, #48
	ble .L_0814715c
	movs r5, #48
.L_0814715c:
	ldr r0, [sp, #84]
	cmp r0, #0
	bge .L_08147164
	adds r0, #3
.L_08147164:
	movs r2, #3
	ldr r1, .L_08147330
	negs r2, r2
	asrs r0, r0, #2
	bl Math_UnsignedMulHighAdd
	movs r3, #48
	subs r2, r3, r5
	lsls r1, r0, #1
	lsls r3, r2, #1
	adds r1, r1, r0
	adds r3, r3, r2
	ldr r6, [sp, #88]
	lsls r3, r3, #4
	lsls r1, r1, #10
	adds r1, r1, r3
	movs r7, #224
	movs r0, #48
	adds r1, r6, r1
	lsls r7, r7, #3
	movs r3, #112
	str r0, [sp, #0]
	adds r1, r1, r7
	subs r3, r3, r5
	str r5, [sp, #4]
	ldr r4, [sp, #112]
	ldr r0, [sp, #96]
	movs r2, #32
	mov lr, r4
	.2byte 0xf800
.L_081471a0:
	ldr r3, [sp, #84]
	subs r3, #48
	cmp r3, #111
	bhi .L_081471f6
	ldr r1, [sp, #84]
	lsls r3, r1, #1
	adds r5, r3, #0
	subs r5, #96
	cmp r5, #64
	ble .L_081471b6
	movs r5, #64
.L_081471b6:
	ldr r0, [sp, #84]
	cmp r0, #0
	bge .L_081471be
	adds r0, #3
.L_081471be:
	movs r2, #3
	ldr r1, .L_08147330
	negs r2, r2
	asrs r0, r0, #2
	bl Math_UnsignedMulHighAdd
	movs r3, #64
	subs r3, r3, r5
	lsls r1, r0, #1
	lsls r2, r3, #1
	adds r1, r1, r0
	adds r2, r2, r3
	lsls r2, r2, #4
	lsls r1, r1, #10
	adds r1, r1, r2
	ldr r2, [sp, #88]
	movs r6, #224
	adds r1, r2, r1
	lsls r6, r6, #3
	movs r7, #48
	adds r1, r1, r6
	str r7, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #112]
	ldr r0, [sp, #96]
	movs r2, #32
	mov lr, r4
	.2byte 0xf800
.L_081471f6:
	ldr r0, [sp, #84]
	subs r0, #160
	mov r11, r0
	cmp r0, #239
	bhi .L_08147268
	ldr r3, [sp, #84]
	cmp r3, #0
	bge .L_08147208
	adds r3, #3
.L_08147208:
	asrs r3, r3, #2
	ldr r1, .L_08147330
	mov r8, r3
	movs r2, #3
	negs r2, r2
	mov r0, r8
	mov r10, r1
	mov r9, r2
	bl Math_UnsignedMulHighAdd
	ldr r3, [sp, #88]
	lsls r1, r0, #1
	adds r1, r1, r0
	movs r5, #48
	lsls r1, r1, #10
	movs r6, #224
	str r5, [sp, #0]
	adds r1, r3, r1
	lsls r6, r6, #3
	movs r5, #64
	ldr r4, [sp, #112]
	movs r3, #0
	adds r1, r1, r6
	str r5, [sp, #4]
	ldr r0, [sp, #96]
	movs r2, #8
	mov lr, r4
	.2byte 0xf800
	mov r1, r10
	mov r2, r9
	mov r0, r8
	bl Math_UnsignedMulHighAdd
	lsls r1, r0, #1
	ldr r7, [sp, #88]
	adds r1, r1, r0
	lsls r1, r1, #10
	movs r0, #48
	adds r1, r7, r1
	str r0, [sp, #0]
	adds r1, r1, r6
	str r5, [sp, #4]
	ldr r4, [sp, #112]
	ldr r0, [sp, #96]
	movs r2, #8
	movs r3, #64
	mov lr, r4
	.2byte 0xf800
.L_08147268:
	ldr r1, [sp, #84]
	cmp r1, #159
	bgt .L_0814734e
	movs r3, #160
	ldr r6, [sp, #88]
	movs r2, #0
	add r3, sp
	mov r9, r2
	mov r10, r3
.L_0814727a:
	ldr r5, [r6, #24]
	mov r8, r5
	cmp r5, #0
	bne .L_0814733c
	ldr r0, [r6, #16]
	bl Trig_Sin
	ldr r3, [r6, #8]
	mov r5, r10
	muls r3, r0
	str r3, [r5]
	ldr r3, [r6, #4]
	str r3, [r5, #4]
	ldr r0, [r6, #16]
	bl Trig_Cos
	ldr r3, [r6, #8]
	add r7, sp, #148
	muls r3, r0
	adds r1, r7, #0
	str r3, [r5, #8]
	adds r0, r5, #0
	bl Func_0815e1ec
	ldr r3, [r7]
	ldr r0, .L_08147334
	asrs r2, r3, #1
	str r2, [r7]
	ldr r3, [r6, #4]
	cmp r3, r0
	bgt .L_081472d4
	ldr r3, [r7, #4]
	ldr r5, [sp, #88]
	movs r7, #172
	movs r1, #16
	lsls r7, r7, #6
	str r1, [sp, #0]
	str r1, [sp, #4]
	subs r2, #12
	subs r3, #12
	ldr r4, [sp, #112]
	ldr r0, [sp, #96]
	adds r1, r5, r7
	mov lr, r4
	.2byte 0xf800
.L_081472d4:
	ldr r3, [r6, #8]
	cmp r3, #24
	ble .L_081472de
	subs r3, #4
	str r3, [r6, #8]
.L_081472de:
	ldr r3, [r6, #12]
	ldr r2, [r6, #16]
	lsls r3, r3, #1
	movs r0, #128
	adds r2, r2, r3
	lsls r0, r0, #9
	str r2, [r6, #16]
	cmp r2, r0
	ble .L_081472f6
	ldr r1, .L_08147338
	adds r3, r2, r1
	str r3, [r6, #16]
.L_081472f6:
	ldr r3, [r6, #12]
	movs r2, #128
	adds r3, #50
	lsls r2, r2, #5
	str r3, [r6, #12]
	cmp r3, r2
	ble .L_08147306
	str r2, [r6, #12]
.L_08147306:
	ldr r3, [r6, #16]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	ldr r2, [r6, #4]
	str r3, [r6, #16]
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #14
	str r2, [r6, #4]
	cmp r2, r3
	ble .L_08147342
	mov r5, r8
	movs r3, #100
	str r5, [r6, #4]
	str r3, [r6, #8]
	str r5, [r6, #16]
	str r5, [r6, #12]
	b .L_08147342
	.2byte 0x0000
.L_08147330:
	.4byte 0x55555556
.L_08147334:
	.4byte 0x003fffff
.L_08147338:
	.4byte 0xffff0000
.L_0814733c:
	mov r3, r8
	subs r3, #1
	str r3, [r6, #24]
.L_08147342:
	movs r7, #1
	add r9, r7
	mov r0, r9
	adds r6, #28
	cmp r0, #24
	bne .L_0814727a
.L_0814734e:
	ldr r1, [sp, #84]
	cmp r1, #160
	beq .L_08147356
	b .L_08147474
.L_08147356:
	ldr r5, .L_08147644
	movs r0, #1
	adds r1, r5, #0
	movs r2, #0
	bl Func_08118040
	adds r1, r5, #0
	movs r0, #1
	movs r2, #8
	bl Func_08118038
	movs r5, #240
	ldr r2, [sp, #88]
	lsls r5, r5, #7
	adds r5, #240
	adds r3, r2, r5
	ldr r0, [r3]
	bl Func_0814cc4c
	movs r1, #128
	lsls r1, r1, #2
	movs r0, #9
	adds r1, #134
	movs r2, #2
	bl Func_08152404
	ldr r6, [sp, #88]
	movs r7, #244
	lsls r7, r7, #6
	ldr r0, .L_08147648
	adds r1, r6, r7
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	ldr r3, .L_0814764c
	movs r0, #0
	movs r2, #128
	mov r9, r0
	movs r1, #0
	lsls r2, r2, #2
.L_081473a8:
	movs r5, #1
	add r9, r5
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_081473a8
	ldr r7, [sp, #100]
	movs r6, #0
	ldr r3, [r7, #20]
	mov r9, r6
	cmp r3, #0
	beq .L_081473e8
	movs r6, #36
.L_081473c2:
	ldr r1, [sp, #100]
	ldrsh r0, [r6, r1]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #8
	lsls r3, r3, #16
	str r3, [r5, #12]
	ldr r5, [sp, #100]
	movs r3, #1
	add r9, r3
	ldr r3, [r5, #20]
	adds r6, #2
	cmp r9, r3
	bne .L_081473c2
.L_081473e8:
	ldr r2, .L_08147650
	movs r3, #72
	str r3, [r2, #12]
	ldr r7, [sp, #88]
	movs r6, #0
	mov r9, r6
.L_081473f4:
	bl Random16
	movs r3, #127
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	movs r5, #254
	lsls r5, r5, #7
	adds r5, #255
	ands r5, r0
	movs r0, #159
	lsls r0, r0, #8
	adds r0, #255
	adds r5, r5, r0
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	adds r0, r5, #0
	str r3, [r7]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	str r3, [r7, #4]
	bl Random16
	movs r1, #200
	bl Math_ModU
	movs r1, #1
	subs r0, #100
	add r9, r1
	lsls r0, r0, #16
	movs r3, #0
	mov r2, r9
	str r0, [r7, #8]
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #32
	bne .L_081473f4
	ldr r5, [sp, #100]
	mov r9, r3
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_08147474
	movs r4, #184
	lsls r4, r4, #5
	add r0, sp, #300
	movs r5, #16
	add r1, sp, #268
	adds r4, #112
	movs r2, #0
.L_08147462:
	str r5, [r2, r0]
	str r4, [r2, r1]
	ldr r7, [sp, #100]
	movs r6, #1
	ldr r3, [r7, #20]
	add r9, r6
	adds r2, #4
	cmp r9, r3
	bne .L_08147462
.L_08147474:
	ldr r0, [sp, #84]
	cmp r0, #159
	bgt .L_0814747c
	b .L_08147a18
.L_0814747c:
	ldr r3, .L_08147654
	mov r1, r11
	ldr r4, [r3, #4]
	ldr r3, [r3]
	movs r5, #16
	str r3, [sp, #104]
	str r4, [sp, #108]
	cmp r1, #64
	bgt .L_08147490
	movs r5, #32
.L_08147490:
	mov r2, r11
	lsls r0, r2, #7
	bl Trig_Sin
	ldr r3, [sp, #68]
	lsls r0, r0, #5
	asrs r0, r0, #6
	subs r0, r3, r0
	mov r6, r11
	str r0, [sp, #68]
	lsls r0, r6, #9
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	ldr r7, [sp, #72]
	ldr r5, [sp, #60]
	ldr r2, [sp, #64]
	asrs r3, r3, #6
	adds r3, r7, r3
	ldr r6, [sp, #64]
	adds r2, r2, r3
	str r3, [sp, #72]
	lsrs r3, r5, #31
	adds r3, r3, r5
	asrs r3, r3, #1
	ldr r0, [sp, #60]
	str r3, [sp, #60]
	lsrs r3, r6, #31
	adds r3, r3, r6
	asrs r3, r3, #1
	ldr r1, [sp, #68]
	str r3, [sp, #64]
	movs r3, #128
	lsls r3, r3, #9
	add r4, sp, #104
	str r3, [sp, #104]
	str r2, [sp, #72]
	str r3, [r4, #4]
	add r2, sp, #120
	movs r3, #0
	adds r0, r0, r1
	str r3, [r2, #12]
	ldr r7, [sp, #88]
	str r0, [sp, #68]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #220
	adds r5, r7, r0
	movs r7, #255
	mov r9, r3
	adds r6, r2, #0
	lsls r7, r7, #16
.L_081474fa:
	ldr r3, .L_08147658
	mov r1, r9
	ldrb r3, [r3, r1]
	ldr r2, [sp, #68]
	lsls r3, r3, #16
	movs r0, #160
	adds r3, r3, r2
	lsls r0, r0, #14
	adds r3, r3, r0
	str r3, [r6]
	ldr r3, .L_0814765c
	lsls r2, r1, #6
	ldrb r3, [r3, r1]
	ldr r1, [sp, #72]
	adds r3, r3, r2
	lsls r3, r3, #16
	adds r3, r3, r1
	adds r2, r4, #0
	str r7, [r6, #4]
	str r3, [r6, #8]
	ldmia r5!, {r0}
	movs r3, #0
	adds r1, r6, #0
	str r4, [sp, #8]
	bl Render_ApplyProjectedPlacementFar
	movs r3, #1
	movs r2, #128
	add r9, r3
	lsls r2, r2, #15
	mov r0, r9
	adds r7, r7, r2
	ldr r4, [sp, #8]
	cmp r0, #9
	bne .L_081474fa
	ldr r1, [sp, #84]
	cmp r1, #255
	bgt .L_08147548
	b .L_08147a18
.L_08147548:
	movs r3, #128
	add r5, sp, #136
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #8]
	str r2, [r5]
	str r2, [r5, #4]
	mov r10, r2
	bl Func_08014de4
	adds r0, r5, #0
	bl SceneTransform_ApplyPosition
	ldr r3, [sp, #84]
	movs r5, #74
	adds r5, #255
	cmp r3, r5
	ble .L_0814756e
	b .L_081476be
.L_0814756e:
	movs r6, #0
	mov r9, r6
	ldr r6, [sp, #88]
.L_08147574:
	ldr r3, [r6]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r6, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r6, #8]
	adds r0, r0, r2
	asrs r3, r3, #8
	adds r7, r3, #0
	muls r7, r3
	adds r3, r7, #0
	adds r0, r0, r3
	ldr r3, .L_08147660
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #9
	mov r8, r0
	cmp r0, #0
	beq .L_0814766c
	add r7, sp, #148
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r7]
	ldr r0, [sp, #68]
	asrs r3, r3, #17
	asrs r2, r0, #17
	adds r3, r3, r2
	adds r3, #32
	ldr r5, [sp, #72]
	str r3, [r7]
	movs r1, #6
	ldrsh r3, [r7, r1]
	asrs r2, r5, #16
	adds r3, r3, r2
	subs r3, #4
	str r3, [r7, #4]
	movs r0, #10
	ldrsh r3, [r7, r0]
	str r3, [r7, #8]
	cmp r3, #169
	bgt .L_081475d4
	movs r3, #170
	str r3, [r7, #8]
.L_081475d4:
	ldr r0, [r7, #8]
	movs r3, #175
	lsls r3, r3, #1
	cmp r0, r3
	ble .L_081475e2
	str r3, [r7, #8]
	adds r0, r3, #0
.L_081475e2:
	ldr r1, .L_08147664
	subs r0, #170
	bl Math_UnsignedMulHigh
	movs r3, #6
	subs r4, r3, r0
	ldr r2, .L_08147668
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #80]
	lsrs r3, r4, #31
	adds r1, r2, r1
	ldr r2, [r7]
	adds r3, r4, r3
	asrs r3, r3, #1
	ldr r5, [sp, #76]
	subs r2, r2, r3
	ldr r3, [r7, #4]
	str r0, [sp, #4]
	str r4, [sp, #0]
	subs r3, r3, r4
	ldr r0, [sp, #96]
	ldr r4, [r5, #4]
	mov lr, r4
	.2byte 0xf800
	ldr r5, [r6]
	mov r1, r8
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6]
	ldr r5, [r6, #4]
	mov r1, r8
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6, #4]
	ldr r5, [r6, #8]
	mov r1, r8
	adds r0, r5, #0
	bl Math_Div
	subs r5, r5, r0
	str r5, [r6, #8]
	b .L_08147670
	.2byte 0x0000
.L_08147644:
	.4byte 0x00000045
.L_08147648:
	.4byte 0x0000014b
.L_0814764c:
	.4byte Data_02010018
.L_08147650:
	.4byte gCameraSceneParameters
.L_08147654:
	.4byte Data_08196df8
.L_08147658:
	.4byte Data_0819793e
.L_0814765c:
	.4byte Data_08197947
.L_08147660:
	.4byte IwramFillWords + 0x74
.L_08147664:
	.4byte 0x071c71c8
.L_08147668:
	.4byte Data_08197410
.L_0814766c:
	movs r7, #1
	add r10, r7
.L_08147670:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r6, #28
	cmp r1, #32
	beq .L_0814767e
	b .L_08147574
.L_0814767e:
	mov r2, r10
	cmp r2, #0
	ble .L_081476be
	movs r1, #10
	mov r0, r10
	bl Math_Div
	ldr r2, .L_081479e4
	adds r4, r0, #1
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #80]
	ldr r5, [sp, #68]
	adds r1, r3, r1
	ldr r6, [sp, #72]
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	asrs r2, r5, #17
	ldr r7, [sp, #76]
	subs r2, r2, r3
	asrs r3, r6, #16
	subs r3, r3, r4
	str r0, [sp, #4]
	str r4, [sp, #0]
	adds r2, #32
	subs r3, #4
	ldr r4, [r7, #4]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
.L_081476be:
	movs r0, #0
	mov r11, r0
	mov r10, r0
.L_081476c4:
	ldr r2, .L_081479e8
	mov r1, r11
	lsls r3, r1, #1
	ldrh r3, [r2, r3]
	ldr r5, [sp, #84]
	cmp r5, r3
	bne .L_08147742
	ldr r7, .L_081479ec
	movs r6, #0
	mov r9, r6
	add r7, r10
	mov r8, r6
.L_081476dc:
	bl Random16
	movs r6, #127
	ands r6, r0
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #16
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #128
	mov r0, r8
	lsls r3, r3, #10
	str r3, [r7, #16]
	str r0, [r7]
	str r0, [r7, #4]
	str r0, [r7, #8]
	bl Random16
	movs r3, #15
	movs r1, #1
	ands r3, r0
	add r9, r1
	adds r3, #64
	mov r2, r9
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #96
	bne .L_081476dc
	ldr r2, .L_081479e8
.L_08147742:
	ldrh r3, [r2]
	ldr r5, [sp, #84]
	cmp r5, r3
	blt .L_081477ea
	ldr r5, .L_081479ec
	movs r6, #0
	mov r9, r6
.L_08147750:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_081477dc
	ldr r3, .L_081479f0
	add r1, sp, #148
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	add r7, sp, #148
	ldr r3, [r7]
	asrs r3, r3, #17
	adds r3, #32
	str r3, [r7]
	movs r0, #6
	ldrsh r3, [r7, r0]
	adds r3, #56
	str r3, [r7, #4]
	movs r1, #10
	ldrsh r3, [r7, r1]
	str r3, [r7, #8]
	cmp r3, #169
	bgt .L_08147780
	movs r3, #170
	str r3, [r7, #8]
.L_08147780:
	ldr r0, [r7, #8]
	movs r3, #175
	lsls r3, r3, #1
	cmp r0, r3
	ble .L_0814778e
	str r3, [r7, #8]
	adds r0, r3, #0
.L_0814778e:
	ldr r1, .L_081479f4
	subs r0, #170
	bl Math_UnsignedMulHigh
	movs r3, #3
	subs r4, r3, r0
	ldr r2, .L_081479e4
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #80]
	lsrs r3, r4, #31
	adds r1, r2, r1
	ldr r2, [r7]
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r7, #4]
	str r4, [sp, #0]
	subs r3, r3, r4
	str r0, [sp, #4]
	ldr r4, [sp, #112]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r5]
	ldr r3, [r5, #12]
	adds r2, r2, r3
	str r2, [r5]
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_081477dc:
	movs r3, #1
	movs r6, #128
	add r9, r3
	lsls r6, r6, #2
	adds r5, #28
	cmp r9, r6
	bne .L_08147750
.L_081477ea:
	movs r0, #1
	movs r7, #224
	add r11, r0
	lsls r7, r7, #4
	mov r1, r11
	add r10, r7
	cmp r1, #4
	beq .L_081477fc
	b .L_081476c4
.L_081477fc:
	movs r2, #0
	mov r9, r2
	str r2, [sp, #28]
	ldr r2, [sp, #68]
	movs r3, #212
	lsls r3, r3, #3
	str r3, [sp, #32]
	ldr r6, .L_081479f8
	ldr r5, [sp, #84]
	movs r7, #165
	asrs r3, r2, #17
	lsls r7, r7, #1
	ldr r0, [sp, #88]
	adds r3, #32
	movs r1, #210
	str r7, [sp, #20]
	str r3, [sp, #52]
	adds r6, r5, r6
	lsls r1, r1, #3
	str r6, [sp, #24]
	adds r6, r0, r1
.L_08147826:
	ldr r3, [sp, #84]
	ldr r5, [sp, #20]
	cmp r3, r5
	bne .L_08147872
	ldr r0, [sp, #72]
	ldr r7, [sp, #52]
	asrs r3, r0, #16
	subs r3, #4
	str r7, [r6]
	str r3, [r6, #4]
	str r7, [r6, #12]
	str r3, [r6, #16]
	ldr r5, [sp, #88]
	movs r7, #230
	movs r1, #0
	lsls r7, r7, #2
	mov r11, r1
	movs r2, #4
	adds r3, r5, r7
.L_0814784c:
	movs r0, #1
	add r11, r0
	mov r1, r11
	str r2, [r3]
	adds r3, #28
	cmp r1, #28
	bne .L_0814784c
	ldr r3, .L_081479fc
	ldr r5, [sp, #88]
	movs r7, #238
	movs r2, #128
	lsls r7, r7, #7
	lsls r2, r2, #12
	adds r7, #168
	str r2, [sp, #60]
	str r3, [sp, #64]
	adds r2, r5, r7
	movs r3, #8
	str r3, [r2]
.L_08147872:
	ldr r0, [sp, #84]
	ldr r1, [sp, #20]
	cmp r0, r1
	bge .L_0814787c
	b .L_081479bc
.L_0814787c:
	ldr r2, [sp, #24]
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r4, r3, #1
	cmp r4, #2
	ble .L_0814788a
	movs r4, #2
.L_0814788a:
	ldr r2, .L_08147a00
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #88]
	ldr r2, [r6]
	adds r1, r3, r1
	ldr r3, .L_08147a04
	ldr r7, [sp, #76]
	ldrb r0, [r3, r4]
	ldr r3, [r6, #4]
	movs r5, #224
	subs r3, r3, r0
	ldr r0, .L_08147a08
	lsls r5, r5, #3
	ldrb r0, [r0, r4]
	adds r1, r1, r5
	str r0, [sp, #0]
	ldr r0, .L_08147a0c
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r4, [r7, #4]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6]
	subs r3, #8
	str r3, [r6]
	ldr r3, [r6, #4]
	adds r3, #2
	str r3, [r6, #4]
	ldr r3, [sp, #20]
	ldr r0, [sp, #84]
	adds r3, #8
	cmp r0, r3
	bge .L_081479bc
	ldr r0, .L_08147a10
	bl Func_080150e4
	ldr r0, .L_08147a14
	bl Func_08015068
	ldr r3, [sp, #28]
	movs r1, #0
	ldr r5, [sp, #32]
	ldr r0, [sp, #88]
	mov r11, r1
	movs r2, #160
	str r1, [sp, #36]
	str r3, [sp, #48]
	movs r1, #224
	add r7, sp, #148
	add r2, sp
	lsls r1, r1, #2
	str r5, [sp, #44]
	mov r8, r2
	mov r10, r7
	adds r5, r0, r1
.L_081478fc:
	mov r2, r8
	movs r3, #0
	str r3, [r2]
	ldr r0, [sp, #36]
	bl Trig_Cos
	ldr r3, [r5, #24]
	muls r3, r0
	mov r0, r8
	str r3, [r0, #4]
	ldr r0, [sp, #36]
	bl Trig_Sin
	ldr r3, [r5, #24]
	mov r1, r8
	muls r3, r0
	str r3, [r1, #8]
	mov r0, r8
	ldr r3, [r5, #24]
	mov r1, r10
	adds r3, #2
	str r3, [r5, #24]
	bl Func_0815e1ec
	movs r1, #192
	ldr r0, [sp, #48]
	lsls r1, r1, #3
	mov r3, r10
	adds r1, #156
	ldr r2, [r3]
	adds r3, r0, r1
	ldr r0, [sp, #88]
	asrs r2, r2, #17
	ldr r3, [r0, r3]
	mov r1, r10
	adds r2, r2, r3
	str r2, [r1]
	movs r3, #6
	ldrsh r2, [r1, r3]
	ldr r1, [sp, #44]
	ldr r3, [r0, r1]
	mov r1, r10
	adds r2, r2, r3
	mov r3, r10
	str r2, [r3, #4]
	movs r0, #10
	ldrsh r3, [r3, r0]
	str r3, [r1, #8]
	cmp r3, #169
	bgt .L_08147964
	movs r3, #170
	str r3, [r7, #8]
.L_08147964:
	ldr r0, [r7, #8]
	movs r3, #175
	lsls r3, r3, #1
	cmp r0, r3
	ble .L_08147972
	str r3, [r7, #8]
	adds r0, r3, #0
.L_08147972:
	ldr r1, .L_081479f4
	subs r0, #170
	bl Math_UnsignedMulHigh
	movs r3, #3
	subs r4, r3, r0
	ldr r2, .L_081479e4
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #80]
	lsrs r3, r4, #31
	adds r1, r2, r1
	ldr r2, [r7]
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r7, #4]
	str r4, [sp, #0]
	subs r3, r3, r4
	str r0, [sp, #4]
	ldr r4, [sp, #112]
	ldr r0, [sp, #96]
	mov lr, r4
	.2byte 0xf800
	movs r0, #144
	ldr r3, [sp, #36]
	lsls r0, r0, #4
	movs r1, #1
	adds r0, #36
	add r11, r1
	adds r3, r3, r0
	mov r2, r11
	str r3, [sp, #36]
	adds r5, #28
	cmp r2, #28
	bne .L_081478fc
.L_081479bc:
	ldr r3, [sp, #32]
	ldr r5, [sp, #28]
	ldr r7, [sp, #24]
	ldr r0, [sp, #20]
	movs r1, #1
	add r9, r1
	adds r3, #28
	adds r5, #28
	subs r7, #12
	adds r0, #12
	mov r2, r9
	str r3, [sp, #32]
	str r5, [sp, #28]
	str r7, [sp, #24]
	str r0, [sp, #20]
	adds r6, #28
	cmp r2, #4
	beq .L_081479e2
	b .L_08147826
.L_081479e2:
	b .L_08147a18
.L_081479e4:
	.4byte Data_08197410
.L_081479e8:
	.4byte Data_08197950
.L_081479ec:
	.4byte gMapCellBuffer
.L_081479f0:
	.4byte IwramTransformVector
.L_081479f4:
	.4byte 0x02d82d83
.L_081479f8:
	.4byte 0xfffffeb6
.L_081479fc:
	.4byte 0xfffe0000
.L_08147a00:
	.4byte Data_08197962
.L_08147a04:
	.4byte Data_0819795e
.L_08147a08:
	.4byte Data_08197958
.L_08147a0c:
	.4byte Data_0819795b
.L_08147a10:
	.4byte 0xfffff800
.L_08147a14:
	.4byte 0xfffff000
.L_08147a18:
	bl Func_081434f8
	movs r5, #240
	ldr r3, [sp, #88]
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #84]
	movs r7, #200
	adds r6, #1
	lsls r7, r7, #1
	str r6, [sp, #84]
	cmp r6, r7
	beq .L_08147a42
	bl .L_08146f52
.L_08147a42:
	movs r0, #134
	bl Func_081180e8
	ldr r1, [sp, #100]
	movs r0, #0
	ldr r3, [r1, #20]
	mov r9, r0
	cmp r3, #0
	beq .L_08147a80
	add r6, sp, #172
	add r5, sp, #204
	movs r7, #36
.L_08147a5a:
	ldr r2, [sp, #100]
	ldrsh r0, [r7, r2]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r2, [r0]
	movs r0, #1
	str r3, [r2, #8]
	add r9, r0
	ldr r3, [r5, #4]
	adds r7, #2
	str r3, [r2, #16]
	adds r5, #8
	ldmia r6!, {r3}
	strh r3, [r2, #6]
	ldr r1, [sp, #100]
	ldr r3, [r1, #20]
	cmp r9, r3
	bne .L_08147a5a
.L_08147a80:
	ldr r2, .L_08147acc
	movs r3, #120
	str r3, [r2, #12]
	bl Func_0814cca8
	movs r6, #238
	ldr r3, [sp, #88]
	lsls r6, r6, #7
	movs r2, #0
	adds r6, #220
	mov r9, r2
	adds r5, r3, r6
.L_08147a98:
	movs r7, #1
	ldmia r5!, {r0}
	add r9, r7
	bl ResourceObject_ReleaseFar
	mov r0, r9
	cmp r0, #9
	bne .L_08147a98
	ldr r0, .L_08147ad0
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #364
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08147acc:
	.4byte gCameraSceneParameters
.L_08147ad0:
	.4byte Func_08143000
