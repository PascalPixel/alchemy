.syntax unified
	.thumb
	.global Func_081811d4
	.thumb_func
Func_081811d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r10, r0
	ldr r0, [r3, #92]
	sub sp, #68
	str r0, [sp, #52]
	movs r0, #0
	ldr r3, [r3, #96]
	movs r5, #7
	mov r8, r3
	bl Func_08143a88
	ldr r3, .L_08181218
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0818121c
	adds r2, #50
	strh r3, [r2]
	movs r1, #0
	mov r11, r1
	movs r0, #0
.L_08181210:
	movs r1, #0
	subs r4, r0, #3
	b .L_08181220
	.2byte 0x0000
.L_08181218:
	.4byte 0x00000100
.L_0818121c:
	.4byte 0x00000410
.L_08181220:
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08181238
	cmp r1, #7
	bgt .L_0818123c
	cmp r4, #3
	bhi .L_08181256
	adds r3, r0, #0
	adds r2, r1, #0
	adds r3, #9
	b .L_0818125a
.L_08181238:
	cmp r1, #7
	bgt .L_08181248
.L_0818123c:
	cmp r0, #11
	bgt .L_08181256
	adds r2, r1, #0
	ands r2, r5
	adds r3, r0, #0
	b .L_0818125a
.L_08181248:
	cmp r4, #3
	bhi .L_08181256
	adds r2, r1, #0
	adds r3, r0, #0
	ands r2, r5
	adds r3, #9
	b .L_0818125a
.L_08181256:
	movs r2, #0
	movs r3, #0
.L_0818125a:
	lsls r3, r3, #3
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r2, r3, #1
	lsls r2, r2, #8
	orrs r2, r3
	ldr r3, .L_081812d0
	lsls r2, r2, #16
	add r3, r11
	asrs r2, r2, #16
	strh r2, [r3]
	adds r1, #1
	movs r3, #2
	add r11, r3
	cmp r1, #16
	bne .L_08181220
	adds r0, #1
	cmp r0, #16
	bne .L_08181210
	ldr r3, .L_081812d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #44
	str r3, [r2]
	ldr r1, .L_081812c0
	movs r3, #128
	ldr r2, .L_081812c4
	lsls r3, r3, #19
	adds r3, #64
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r3, .L_081812c8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_081812cc
	adds r2, #2
	strh r3, [r2]
	ldr r4, [sp, #52]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r4, r2
	movs r3, #1
	b .L_081812d8
	.2byte 0x0000
.L_081812c0:
	.4byte 0x000000f0
.L_081812c4:
	.4byte 0x00002888
.L_081812c8:
	.4byte 0x00003737
.L_081812cc:
	.4byte 0x00002033
.L_081812d0:
	.4byte 0x06003800
.L_081812d4:
	.4byte 0xffffd800
.L_081812d8:
	movs r2, #1
	ldr r0, .L_081813a4
	bl Func_08157cf4
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #39
	movs r0, #188
	str r3, [sp, #40]
	bl Func_081963ec
	ldr r4, [sp, #52]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	adds r5, #188
	lsls r1, r1, #7
	adds r2, r4, r0
	ldr r5, [r5]
	movs r3, #1
	adds r1, #132
	str r3, [r2]
	adds r2, r4, r1
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_081813a8
	str r5, [sp, #44]
	bl Func_080145a8
	movs r2, #172
	lsls r2, r2, #15
	ldr r1, [sp, #52]
	str r2, [sp, #36]
	movs r2, #146
	movs r3, #128
	mov r0, sp
	lsls r2, r2, #7
	lsls r3, r3, #13
	movs r4, #0
	adds r0, #56
	adds r2, r1, r2
	str r3, [sp, #32]
	str r4, [sp, #48]
	str r0, [sp, #12]
	str r2, [sp, #20]
.L_08181340:
	movs r3, #192
	lsls r3, r3, #18
	mov r4, r10
	ldr r5, [r3, #48]
	ldr r1, [sp, #12]
	movs r3, #36
	ldrsh r0, [r4, r3]
	bl Func_0815e20c
	ldr r0, [sp, #48]
	cmp r0, #31
	bgt .L_0818136c
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08181366
	ldrh r3, [r5, #54]
	subs r3, #128
	b .L_0818136a
.L_08181366:
	ldrh r3, [r5, #54]
	adds r3, #128
.L_0818136a:
	strh r3, [r5, #54]
.L_0818136c:
	ldr r2, [sp, #48]
	cmp r2, #108
	blt .L_08181384
	movs r4, #124
	ldr r1, .L_081813a0
	movs r3, #128
	subs r2, r4, r2
	lsls r3, r3, #19
	subs r2, #1
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08181384:
	ldr r0, [sp, #48]
	cmp r0, #0
	bne .L_08181422
	mov r1, r10
	ldr r6, [r1, #4]
	cmp r6, #0
	bne .L_081813ac
	movs r2, #172
	movs r3, #128
	lsls r2, r2, #15
	lsls r3, r3, #13
	str r2, [sp, #36]
	str r3, [sp, #32]
	b .L_081813b4
.L_081813a0:
	.4byte 0x00001000
.L_081813a4:
	.4byte 0x000000b0
.L_081813a8:
	.4byte Func_08143000
.L_081813ac:
	ldr r4, .L_0818146c
	ldr r0, .L_08181470
	str r4, [sp, #36]
	str r0, [sp, #32]
.L_081813b4:
	movs r1, #0
	mov r11, r1
	b .L_081813be
.L_081813ba:
	mov r2, r10
	ldr r6, [r2, #4]
.L_081813be:
	mov r4, r11
	lsls r3, r4, #3
	ldr r0, [sp, #52]
	subs r3, r3, r4
	lsls r3, r3, #2
	adds r5, r0, r3
	cmp r6, #0
	bne .L_081813da
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #12
	b .L_081813e2
.L_081813da:
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5]
	ldr r3, .L_08181474
.L_081813e2:
	str r3, [r5, #12]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r5, #4]
	mov r3, r10
	ldr r2, [r3, #24]
	ldr r1, .L_08181478
	lsls r3, r2, #3
	subs r3, r3, r2
	add r3, r11
	ldrb r3, [r1, r3]
	cmp r3, #0
	beq .L_081813fe
	movs r3, #1
.L_081813fe:
	str r3, [r5, #8]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	ldr r4, .L_0818147c
	adds r3, #255
	ands r3, r0
	movs r0, #1
	adds r3, r3, r4
	add r11, r0
	str r3, [r5, #16]
	mov r1, r11
	movs r3, #0
	str r3, [r5, #20]
	str r3, [r5, #24]
	cmp r1, #7
	bne .L_081813ba
.L_08181422:
	ldr r2, [sp, #48]
	cmp r2, #24
	bgt .L_08181480
	ldr r4, [sp, #36]
	movs r3, #128
	asrs r0, r4, #16
	lsls r3, r3, #19
	negs r2, r0
	adds r3, #40
	lsls r2, r2, #8
	str r2, [r3]
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0818145a
	ldr r3, .L_08181468
	adds r2, r0, #0
	adds r2, #120
	lsls r2, r2, #8
	orrs r2, r3
	movs r1, #128
	movs r3, #128
	lsls r1, r1, #19
	lsls r3, r3, #19
	adds r1, #64
	adds r3, #66
	strh r2, [r1]
	b .L_0818149c
.L_0818145a:
	movs r3, #128
	lsls r3, r3, #19
	adds r2, r0, #0
	adds r3, #64
	adds r2, #240
	b .L_08181498
	.2byte 0x0000
.L_08181468:
	.4byte 0x000000f0
.L_0818146c:
	.4byte 0xffaa0000
.L_08181470:
	.4byte 0xfff00000
.L_08181474:
	.4byte 0xfff40000
.L_08181478:
	.4byte Data_081995f3
.L_0818147c:
	.4byte 0xffff8000
.L_08181480:
	ldr r4, [sp, #36]
	movs r2, #128
	asrs r3, r4, #16
	lsls r2, r2, #19
	negs r3, r3
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	ldr r2, .L_081814b4
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #64
.L_08181498:
	strh r2, [r3]
	adds r3, #2
.L_0818149c:
	strh r2, [r3]
	ldr r1, [sp, #32]
	ldr r0, [sp, #36]
	movs r3, #52
	muls r3, r1
	subs r0, r0, r1
	str r0, [sp, #36]
	cmp r3, #0
	bge .L_081814b8
	adds r3, #63
	b .L_081814b8
	.2byte 0x0000
.L_081814b4:
	.4byte 0x000000f0
.L_081814b8:
	asrs r3, r3, #6
	str r3, [sp, #32]
	ldr r3, [sp, #48]
	movs r2, #0
	str r2, [sp, #28]
	cmp r3, #105
	ble .L_081814cc
	movs r4, #1
	str r4, [sp, #28]
	b .L_081814e2
.L_081814cc:
	ldr r0, [sp, #48]
	cmp r0, #21
	ble .L_081814e2
	subs r0, #22
	movs r1, #12
	bl Math_Mod
	cmp r0, #3
	ble .L_081814e2
	movs r1, #1
	str r1, [sp, #28]
.L_081814e2:
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_08181520
	ldr r3, [sp, #28]
	cmp r3, #0
	bne .L_08181502
	ldr r4, [sp, #52]
	movs r3, #88
	movs r2, #224
	lsls r2, r2, #3
	str r3, [sp, #0]
	movs r3, #96
	adds r1, r4, r2
	str r3, [sp, #4]
	b .L_08181512
.L_08181502:
	movs r3, #88
	ldr r2, [sp, #52]
	str r3, [sp, #0]
	movs r3, #96
	str r3, [sp, #4]
	movs r3, #160
	lsls r3, r3, #6
	adds r1, r2, r3
.L_08181512:
	mov r0, r8
	movs r2, #32
	movs r3, #0
	ldr r4, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	b .L_08181560
.L_08181520:
	ldr r0, [sp, #28]
	cmp r0, #0
	bne .L_08181544
	movs r3, #88
	ldr r2, [sp, #52]
	str r3, [sp, #0]
	movs r3, #96
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	mov r0, r8
	movs r2, #8
	movs r3, #0
	ldr r4, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	b .L_08181560
.L_08181544:
	movs r3, #88
	ldr r2, [sp, #52]
	str r3, [sp, #0]
	movs r3, #96
	str r3, [sp, #4]
	movs r3, #160
	lsls r3, r3, #6
	adds r1, r2, r3
	mov r0, r8
	movs r2, #8
	movs r3, #0
	ldr r4, [sp, #44]
	mov lr, r4
	.2byte 0xf800
.L_08181560:
	ldr r3, [sp, #40]
	ldr r4, [sp, #44]
	ldr r1, [sp, #52]
	movs r2, #146
	movs r0, #0
	lsls r2, r2, #7
	adds r2, #204
	str r3, [sp, #24]
	str r4, [sp, #16]
	str r0, [sp, #8]
	ldr r5, [sp, #52]
	adds r1, r1, r2
	mov r11, r0
	mov r9, r1
.L_0818157c:
	ldr r3, [sp, #8]
	ldr r0, [sp, #48]
	adds r3, #22
	cmp r0, r3
	bge .L_08181594
	cmp r0, #21
	ble .L_0818158c
	b .L_08181846
.L_0818158c:
	mov r1, r11
	cmp r1, #0
	beq .L_08181594
	b .L_08181846
.L_08181594:
	ldr r3, [sp, #8]
	ldr r2, [sp, #48]
	adds r3, #26
	cmp r2, r3
	blt .L_081815b6
	ldr r4, [r5, #24]
	cmp r4, #0
	bne .L_081815b8
	ldr r3, [r5]
	ldr r2, [r5, #12]
	subs r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	b .L_081815b8
.L_081815b6:
	ldr r4, [r5, #24]
.L_081815b8:
	cmp r4, #4
	ble .L_081815be
	b .L_08181846
.L_081815be:
	cmp r4, #0
	ble .L_081815c8
	adds r3, r4, #1
	str r3, [r5, #24]
	adds r4, r3, #0
.L_081815c8:
	ldr r3, [r5, #8]
	cmp r3, #1
	bne .L_081816b4
	cmp r4, #0
	bne .L_081816ba
	mov r3, r10
	ldr r6, [r3, #4]
	movs r7, #0
	cmp r6, #0
	bne .L_081815f2
	ldr r1, [sp, #12]
	ldr r0, [r5]
	ldr r2, [r1]
	asrs r3, r0, #16
	lsrs r1, r2, #31
	adds r2, r2, r1
	asrs r2, r2, #1
	adds r2, #16
	cmp r3, r2
	bge .L_08181608
	b .L_08181606
.L_081815f2:
	ldr r1, [sp, #12]
	ldr r0, [r5]
	ldr r2, [r1]
	asrs r3, r0, #16
	lsrs r1, r2, #31
	adds r2, r2, r1
	asrs r2, r2, #1
	adds r2, #48
	cmp r3, r2
	ble .L_08181608
.L_08181606:
	movs r7, #1
.L_08181608:
	cmp r7, #1
	bne .L_081816c0
	ldr r3, [sp, #52]
	movs r4, #238
	lsls r4, r4, #7
	adds r4, #168
	adds r2, r3, r4
	mov r0, r11
	movs r3, #4
	str r3, [r2]
	cmp r0, #6
	bne .L_08181634
	mov r2, r10
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #4
	bl Func_08118088
	movs r0, #134
	bl Func_08118088 + 0x60
	b .L_0818165c
.L_08181634:
	mov r4, r10
	mov r1, r11
	movs r3, #36
	ldrsh r0, [r4, r3]
	lsls r3, r1, #2
	add r3, r11
	lsls r3, r3, #1
	adds r3, #110
	str r3, [sp, #4]
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #10
	movs r1, #1
	lsls r3, r3, #12
	str r2, [sp, #0]
	bl Func_0815f000
	movs r0, #133
	bl Audio_PlayCue
.L_0818165c:
	mov r3, r10
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	movs r2, #5
	bl Func_0814cd48
	movs r3, #1
	str r3, [r5, #24]
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08181696
	ldr r0, [sp, #12]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r1, #2
	ldrsh r2, [r5, r1]
	asrs r3, r3, #1
	subs r3, r3, r2
	str r3, [r5]
	adds r0, r3, #0
	ldr r6, [r4, #4]
	movs r4, #1
	b .L_081816c0
.L_08181696:
	ldr r2, [sp, #12]
	mov r0, r10
	ldr r3, [r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r4, #2
	ldrsh r2, [r5, r4]
	asrs r3, r3, #1
	subs r3, r3, r2
	adds r3, #128
	str r3, [r5]
	movs r4, #1
	ldr r6, [r0, #4]
	adds r0, r3, #0
	b .L_081816c0
.L_081816b4:
	mov r1, r10
	ldr r6, [r1, #4]
	b .L_081816be
.L_081816ba:
	mov r2, r10
	ldr r6, [r2, #4]
.L_081816be:
	ldr r0, [r5]
.L_081816c0:
	cmp r6, #0
	bne .L_08181784
	cmp r4, #0
	bne .L_08181738
	movs r4, #6
	ldrsh r3, [r5, r4]
	asrs r2, r0, #16
	movs r1, #12
	movs r0, #51
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #24]
	mov r1, r9
	subs r2, #111
	adds r3, #24
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #6
	ldrsh r3, [r5, r1]
	movs r4, #51
	movs r0, #12
	str r4, [sp, #0]
	str r0, [sp, #4]
	mov r1, r9
	ldr r4, [sp, #24]
	adds r2, #17
	adds r3, #96
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #6
	ldrsh r3, [r5, r1]
	movs r4, #17
	movs r0, #12
	ldr r1, [sp, #20]
	subs r2, #128
	adds r3, #24
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #24]
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	movs r1, #6
	ldrsh r3, [r5, r1]
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r4, #17
	movs r0, #12
	str r4, [sp, #0]
	str r0, [sp, #4]
	adds r3, #96
	mov r0, r8
	ldr r1, [sp, #20]
	b .L_0818177c
.L_08181738:
	ldr r2, [sp, #56]
	movs r1, #51
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	adds r2, r2, r0
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r4, #12
	subs r2, #111
	adds r3, #24
	str r1, [sp, #0]
	str r4, [sp, #4]
	mov r1, r9
	ldr r4, [sp, #24]
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	ldr r2, [sp, #56]
	movs r1, #51
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r5]
	asrs r2, r2, #1
	adds r2, r2, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r4, #12
	str r1, [sp, #0]
	str r4, [sp, #4]
	adds r2, #17
	adds r3, #96
	mov r0, r8
	mov r1, r9
.L_0818177c:
	ldr r4, [sp, #24]
	mov lr, r4
	.2byte 0xf800
	b .L_08181846
.L_08181784:
	cmp r4, #0
	bne .L_081817fe
	asrs r2, r0, #16
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r1, #51
	movs r4, #12
	str r1, [sp, #0]
	str r4, [sp, #4]
	mov r1, r9
	ldr r4, [sp, #16]
	adds r3, #24
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #6
	ldrsh r3, [r5, r1]
	movs r4, #51
	movs r0, #12
	str r4, [sp, #0]
	str r0, [sp, #4]
	mov r1, r9
	ldr r4, [sp, #16]
	subs r2, #128
	adds r3, #96
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #6
	ldrsh r3, [r5, r1]
	movs r4, #17
	movs r0, #12
	ldr r1, [sp, #20]
	adds r2, #51
	adds r3, #24
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #16]
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #6
	ldrsh r3, [r5, r1]
	movs r4, #17
	movs r0, #12
	str r4, [sp, #0]
	str r0, [sp, #4]
	subs r2, #77
	adds r3, #96
	mov r0, r8
	ldr r1, [sp, #20]
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	b .L_08181846
.L_081817fe:
	ldr r2, [sp, #56]
	movs r1, #51
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	adds r2, r2, r0
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r4, #12
	adds r3, #24
	str r1, [sp, #0]
	str r4, [sp, #4]
	mov r1, r9
	ldr r4, [sp, #16]
	mov r0, r8
	mov lr, r4
	.2byte 0xf800
	ldr r2, [sp, #56]
	movs r1, #51
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r5]
	asrs r2, r2, #1
	adds r2, r2, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r4, #12
	str r1, [sp, #0]
	str r4, [sp, #4]
	subs r2, #128
	adds r3, #96
	mov r0, r8
	mov r1, r9
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
.L_08181846:
	ldr r0, [sp, #8]
	movs r1, #1
	add r11, r1
	adds r0, #12
	mov r2, r11
	str r0, [sp, #8]
	adds r5, #28
	cmp r2, #7
	beq .L_0818185a
	b .L_0818157c
.L_0818185a:
	ldr r3, [sp, #28]
	cmp r3, #0
	bne .L_081818a6
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08181888
	movs r3, #14
	str r3, [sp, #0]
	movs r3, #13
	ldr r2, [sp, #52]
	str r3, [sp, #4]
	movs r3, #152
	lsls r3, r3, #7
	adds r3, #48
	adds r1, r2, r3
	mov r0, r8
	movs r2, #88
	movs r3, #29
	ldr r4, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	b .L_081818a6
.L_08181888:
	movs r3, #14
	str r3, [sp, #0]
	movs r3, #13
	ldr r2, [sp, #52]
	str r3, [sp, #4]
	movs r3, #152
	lsls r3, r3, #7
	adds r3, #48
	adds r1, r2, r3
	mov r0, r8
	movs r2, #26
	movs r3, #29
	ldr r4, [sp, #44]
	mov lr, r4
	.2byte 0xf800
.L_081818a6:
	bl Func_081434f8
	movs r0, #1
	movs r1, #4
	bl Func_08158ce0
	movs r1, #240
	ldr r0, [sp, #52]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #48]
	adds r2, #1
	str r2, [sp, #48]
	cmp r2, #124
	beq .L_081818d2
	b .L_08181340
.L_081818d2:
	ldr r2, .L_081818fc
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #68
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r0, .L_08181900
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #68
	b .L_08181904
	.2byte 0x0000
.L_081818fc:
	.4byte 0x00001088
.L_08181900:
	.4byte Func_08143000
.L_08181904:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
