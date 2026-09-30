.syntax unified
	.thumb
	.global Func_081693d0
	.thumb_func
Func_081693d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #116
	str r0, [sp, #68]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #96]
	ldr r3, .L_08169448
	str r0, [sp, #64]
	movs r0, #0
	ldr r1, [r5, #92]
	mov r8, r3
	str r1, [sp, #60]
	movs r6, #200
	ldr r2, [r5, #100]
	lsls r6, r6, #4
	str r2, [sp, #56]
	bl Func_081435e0
	bl Func_0813ba50
	ldr r2, .L_08169440
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r1, r6, #0
	ldr r0, .L_0816944c
	bl Func_080145a8
	ldr r2, .L_08169450
	movs r0, #2
	movs r1, #0
	bl Func_08013438
	ldr r4, [sp, #60]
	movs r7, #239
	lsls r7, r7, #7
	adds r2, r4, r7
	movs r3, #0
	str r3, [r2]
	adds r1, r6, #0
	ldr r0, .L_08169454
	bl Func_080145a8
	ldr r3, .L_08169444
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	b .L_08169458
.L_08169440:
	.4byte 0x00000000
.L_08169444:
	.4byte 0x00002137
.L_08169448:
	.4byte gMapCellBuffer
.L_0816944c:
	.4byte Func_081693b0
.L_08169450:
	.4byte Func_081693c0
.L_08169454:
	.4byte Func_08143000
.L_08169458:
	strh r3, [r2]
	movs r0, #1
	movs r1, #0
	bl Func_08163c2c
	ldr r3, .L_081694a0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #64
	strh r3, [r2]
	ldr r0, [sp, #60]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #240
	adds r3, r0, r1
	ldr r0, [r3]
	bl Func_0814cc4c
	ldr r3, [r5, #36]
	movs r2, #237
	lsls r2, r2, #3
	adds r2, #255
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	ldr r0, .L_081694a4
	bl Resource_GetTableEntry
	movs r2, #128
	adds r5, r0, #0
	ldr r6, .L_081694a8
	adds r1, r5, #0
	lsls r2, r2, #2
	ldr r0, .L_081694ac
	b .L_081694b0
	.2byte 0x0000
.L_081694a0:
	.4byte 0x0000f0f0
.L_081694a4:
	.4byte 0x00000099
.L_081694a8:
	.4byte IwramCopyWords
.L_081694ac:
	.4byte 0x05000200
.L_081694b0:
	mov lr, r6
	.2byte 0xf800
	movs r3, #128
	lsls r3, r3, #2
	adds r5, r5, r3
	adds r0, r5, #0
	mov r1, r8
	bl Func_0801587c
	mov r9, r6
	ldr r5, .L_081695ac
	ldr r6, [sp, #60]
	movs r0, #238
	lsls r0, r0, #7
	movs r1, #13
	movs r4, #0
	adds r0, #220
	negs r1, r1
	mov r11, r5
	mov r10, r4
	adds r5, r6, r0
	mov r7, r8
	adds r6, r1, #0
.L_081694de:
	movs r1, #32
	ldr r2, .L_081695b0
	movs r3, #0
	movs r0, #32
	bl Func_0815b290
	ldrb r3, [r0, #9]
	movs r2, #4
	ands r3, r6
	orrs r3, r2
	strb r3, [r0, #9]
	ldrb r3, [r0, #16]
	stmia r5!, {r0}
	lsls r3, r3, #2
	add r3, r11
	ldrh r0, [r3, #2]
	ldr r2, .L_081695b4
	adds r1, r7, #0
	adds r0, r0, r2
	movs r2, #128
	lsls r2, r2, #3
	mov lr, r9
	.2byte 0xf800
	movs r4, #1
	movs r3, #128
	add r10, r4
	lsls r3, r3, #3
	mov r0, r10
	adds r7, r7, r3
	cmp r0, #8
	bne .L_081694de
	ldr r2, .L_081695b8
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_081695bc
	movs r0, #1
	movs r2, #0
	bl Func_08118028 + 0x18
	ldr r0, .L_081695c0
	ldr r1, [sp, #56]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r2, [sp, #60]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_081695c4
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r5, #170
	ldr r4, [sp, #60]
	lsls r5, r5, #7
	adds r5, #32
	adds r1, r4, r5
	ldr r0, .L_081695c8
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r3, .L_0816959c
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_081695a0
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_081695a4
	adds r2, #50
	strh r3, [r2]
	ldr r1, .L_081695cc
	ldr r3, .L_081695a8
	movs r7, #140
	movs r0, #200
	subs r2, #2
	movs r6, #1
	lsls r7, r7, #17
	lsls r0, r0, #16
	strh r3, [r2]
	str r6, [sp, #52]
	str r7, [sp, #48]
	str r0, [sp, #44]
	str r1, [sp, #40]
	ldr r3, .L_081695d0
	movs r5, #192
	ldrh r3, [r3, #4]
	b .L_081695d4
	.2byte 0x0000
.L_0816959c:
	.4byte 0x00007741
.L_081695a0:
	.4byte 0x00000080
.L_081695a4:
	.4byte 0x00001010
.L_081695a8:
	.4byte 0x00003f44
.L_081695ac:
	.4byte ResourceTableEntries
.L_081695b0:
	.4byte 0x80002000
.L_081695b4:
	.4byte 0x06010000
.L_081695b8:
	.4byte gCameraSceneParameters
.L_081695bc:
	.4byte 0x00000074
.L_081695c0:
	.4byte 0x00000134
.L_081695c4:
	.4byte 0x0000012f
.L_081695c8:
	.4byte 0x00000137
.L_081695cc:
	.4byte 0xfffc0000
.L_081695d0:
	.4byte Data_03001120
.L_081695d4:
	lsls r5, r5, #18
	str r3, [sp, #36]
	adds r3, r5, #0
	adds r3, #176
	ldr r3, [r3]
	ldr r2, .L_08169654
	str r3, [sp, #32]
	ldr r3, [sp, #60]
	movs r4, #239
	lsls r4, r4, #7
	mov r11, r2
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r6, [sp, #60]
	movs r7, #238
	lsls r7, r7, #7
	adds r7, #132
	adds r2, r6, r7
	movs r3, #75
	str r3, [r2]
	ldr r0, [sp, #52]
	ldr r1, [sp, #32]
	str r0, [r1, #16]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r1, #19
	movs r0, #188
	str r3, [sp, #80]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r2, sp
	adds r2, #80
	str r2, [sp, #20]
	str r3, [r2, #4]
	ldr r3, .L_08169650
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	movs r3, #0
	mov r10, r3
	ldr r3, .L_08169658
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #3
.L_0816963c:
	movs r4, #1
	add r10, r4
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_0816963c
	ldr r7, [sp, #60]
	movs r5, #0
	mov r10, r5
	b .L_0816965c
.L_08169650:
	.4byte 0x00000784
.L_08169654:
	.4byte 0xfff80000
.L_08169658:
	.4byte Data_02010018
.L_0816965c:
	bl Random16
	movs r6, #252
	lsls r6, r6, #6
	adds r6, #255
	ands r6, r0
	movs r0, #128
	lsls r0, r0, #8
	adds r6, r6, r0
	bl Random16
	movs r5, #127
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #255
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #2
	str r3, [r7]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r2, r3, #2
	ldr r3, [r7]
	str r2, [r7, #4]
	negs r3, r3
	cmp r3, #0
	bge .L_0816969e
	adds r3, #31
.L_0816969e:
	asrs r3, r3, #5
	str r3, [r7, #12]
	negs r3, r2
	cmp r3, #0
	bge .L_081696aa
	adds r3, #31
.L_081696aa:
	movs r1, #1
	asrs r3, r3, #5
	add r10, r1
	str r3, [r7, #16]
	mov r2, r10
	movs r3, #0
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #32
	bne .L_0816965c
	ldr r7, .L_081697d0
	mov r10, r3
.L_081696c2:
	bl Random16
	movs r3, #255
	adds r6, r0, #0
	ands r6, r3
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
	movs r4, #128
	lsls r4, r4, #16
	asrs r3, r3, #3
	adds r3, r3, r4
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r5, #192
	asrs r3, r3, #4
	lsls r5, r5, #15
	adds r3, r3, r5
	str r3, [r7, #4]
	bl Random16
	movs r3, #248
	lsls r3, r3, #5
	adds r3, #255
	ldr r6, .L_081697d4
	ands r3, r0
	negs r3, r3
	adds r3, r3, r6
	lsls r3, r3, #5
	str r3, [r7, #12]
	bl Random16
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r3, r0
	ldr r0, .L_081697d8
	negs r3, r3
	adds r3, r3, r0
	lsls r3, r3, #5
	movs r1, #1
	movs r2, #178
	str r3, [r7, #16]
	add r10, r1
	movs r3, #0
	lsls r2, r2, #2
	str r3, [r7, #24]
	adds r7, #28
	cmp r10, r2
	bne .L_081696c2
	mov r8, r3
.L_08169742:
	mov r3, r8
	cmp r3, #24
	bne .L_0816974e
	movs r0, #162
	bl Audio_PlayCue
.L_0816974e:
	mov r4, r8
	cmp r4, #76
	bne .L_0816975a
	movs r0, #164
	bl Audio_PlayCue
.L_0816975a:
	mov r5, r8
	cmp r5, #154
	bne .L_08169766
	movs r0, #142
	bl Audio_PlayCue
.L_08169766:
	mov r6, r8
	cmp r6, #222
	bne .L_08169772
	movs r0, #145
	bl Audio_PlayCue
.L_08169772:
	mov r7, r8
	cmp r7, #15
	bhi .L_081697c0
	mov r0, r8
	ldr r7, .L_081697d0
	cmp r0, #1
	bne .L_081697a8
	ldr r5, .L_081697dc
	movs r3, #63
	adds r6, r5, #0
	adds r6, #128
.L_08169788:
	str r3, [sp, #8]
	bl Random16
	ldr r3, [sp, #8]
	ands r0, r3
	strb r0, [r5]
	adds r5, #1
	cmp r5, r6
	bne .L_08169788
	ldr r3, .L_081697cc
	movs r1, #200
	strh r3, [r7]
	ldr r0, .L_081697e0
	lsls r1, r1, #4
	bl Func_080145a8
.L_081697a8:
	ldr r1, [sp, #52]
	ldrh r3, [r7]
	mov r2, r8
	adds r3, r3, r1
	adds r1, #3
	strh r3, [r7]
	str r1, [sp, #52]
	cmp r2, #15
	bne .L_081697c0
	ldr r0, .L_081697e0
	bl Func_08014644
.L_081697c0:
	mov r3, r8
	cmp r3, #21
	bgt .L_081697c8
	b .L_08169900
.L_081697c8:
	ldr r3, .L_081697e4
	b .L_081697e8
.L_081697cc:
	.4byte 0x00000000
.L_081697d0:
	.4byte gMapCellBuffer
.L_081697d4:
	.4byte 0xfffff800
.L_081697d8:
	.4byte 0xffffe800
.L_081697dc:
	.4byte Data_02010002
.L_081697e0:
	.4byte Func_0814cbcc
.L_081697e4:
	.4byte Data_08196e8c
.L_081697e8:
	ldr r4, [r3, #4]
	ldr r3, [r3]
	str r3, [sp, #72]
	str r4, [sp, #76]
	mov r4, r8
	cmp r4, #83
	bgt .L_08169844
	cmp r4, #48
	ble .L_0816980c
	ldr r5, [sp, #40]
	lsls r3, r5, #4
	subs r3, r3, r5
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_08169808
	adds r3, #63
.L_08169808:
	asrs r3, r3, #6
	str r3, [sp, #40]
.L_0816980c:
	mov r6, r11
	lsls r3, r6, #4
	subs r3, r3, r6
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_0816981a
	adds r3, #63
.L_0816981a:
	ldr r7, [sp, #44]
	movs r0, #132
	asrs r3, r3, #6
	lsls r0, r0, #16
	mov r11, r3
	cmp r7, r0
	ble .L_0816982e
	ldr r1, .L_08169a6c
	add r11, r1
	b .L_08169834
.L_0816982e:
	movs r2, #128
	lsls r2, r2, #6
	add r11, r2
.L_08169834:
	ldr r3, [sp, #48]
	ldr r5, [sp, #44]
	ldr r4, [sp, #40]
	add r5, r11
	adds r3, r3, r4
	str r3, [sp, #48]
	str r5, [sp, #44]
	b .L_081698a4
.L_08169844:
	mov r6, r8
	cmp r6, #170
	bgt .L_081698a4
	cmp r6, #107
	bgt .L_08169858
	ldr r7, [sp, #40]
	movs r0, #128
	lsls r0, r0, #6
	adds r7, r7, r0
	str r7, [sp, #40]
.L_08169858:
	ldr r1, [sp, #40]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_08169866
	adds r3, #63
.L_08169866:
	ldr r2, [sp, #48]
	asrs r3, r3, #6
	adds r2, r2, r3
	str r3, [sp, #40]
	mov r3, r8
	str r2, [sp, #48]
	cmp r3, #117
	bgt .L_081698a4
	ldr r4, [sp, #44]
	movs r5, #138
	lsls r5, r5, #16
	cmp r4, r5
	ble .L_08169886
	ldr r6, .L_08169a6c
	add r11, r6
	b .L_0816988c
.L_08169886:
	movs r7, #128
	lsls r7, r7, #6
	add r11, r7
.L_0816988c:
	mov r0, r11
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_0816989a
	adds r3, #63
.L_0816989a:
	ldr r1, [sp, #44]
	asrs r3, r3, #6
	mov r11, r3
	add r1, r11
	str r1, [sp, #44]
.L_081698a4:
	movs r3, #0
	add r5, sp, #100
	str r3, [r5, #12]
	str r3, [r5, #4]
	mov r10, r3
	ldr r2, [sp, #60]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #220
	add r7, sp, #72
	adds r6, r2, r3
.L_081698ba:
	movs r2, #3
	mov r4, r10
	ands r2, r4
	ldr r0, [sp, #48]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r1, .L_08169a70
	lsls r3, r3, #20
	adds r3, r3, r0
	adds r3, r3, r1
	str r3, [r5]
	mov r3, r10
	cmp r4, #0
	bge .L_081698d8
	adds r3, #3
.L_081698d8:
	asrs r3, r3, #2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, [sp, #44]
	ldr r4, .L_08169a74
	lsls r2, r2, #20
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r5, #8]
	ldmia r6!, {r0}
	adds r1, r5, #0
	adds r2, r7, #0
	movs r3, #0
	bl Func_08020010
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #8
	bne .L_081698ba
.L_08169900:
	mov r2, r8
	cmp r2, #76
	bne .L_08169988
	ldr r7, .L_08169a78
	movs r3, #0
	mov r10, r3
.L_0816990c:
	ldr r3, [r7, #24]
	movs r4, #1
	negs r4, r4
	cmp r3, r4
	bne .L_0816997a
	bl Random16
	movs r3, #255
	adds r6, r0, #0
	ands r6, r3
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
	movs r0, #128
	lsls r0, r0, #15
	asrs r3, r3, #3
	adds r3, r3, r0
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r1, #192
	lsls r1, r1, #15
	asrs r3, r3, #4
	adds r3, r3, r1
	str r3, [r7, #4]
	bl Random16
	movs r2, #63
	ands r2, r0
	movs r3, #32
	subs r3, r3, r2
	lsls r3, r3, #12
	str r3, [r7, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	negs r3, r3
	subs r3, #16
	lsls r3, r3, #13
	str r3, [r7, #16]
	movs r3, #0
	str r3, [r7, #24]
.L_0816997a:
	movs r2, #1
	movs r3, #128
	add r10, r2
	lsls r3, r3, #3
	adds r7, #28
	cmp r10, r3
	bne .L_0816990c
.L_08169988:
	mov r4, r8
	cmp r4, #152
	bne .L_081699b2
	ldr r2, .L_08169a7c
	movs r0, #2
	movs r1, #96
	bl Func_08013438
	movs r6, #238
	ldr r5, [sp, #60]
	lsls r6, r6, #7
	movs r7, #238
	adds r6, #180
	lsls r7, r7, #7
	adds r2, r5, r6
	movs r3, #24
	adds r7, #184
	str r3, [r2]
	adds r2, r5, r7
	movs r3, #0
	str r3, [r2]
.L_081699b2:
	movs r0, #152
	negs r0, r0
	add r0, r8
	mov r9, r0
	cmp r0, #87
	bhi .L_08169a30
	ldr r6, [sp, #60]
	movs r1, #0
	mov r10, r1
.L_081699c4:
	mov r3, r10
	cmp r3, #0
	bge .L_081699cc
	adds r3, #3
.L_081699cc:
	asrs r2, r3, #2
	adds r3, r2, #0
	adds r3, #152
	cmp r8, r3
	blt .L_08169a24
	adds r3, #32
	cmp r8, r3
	bge .L_08169a24
	mov r5, r10
	movs r0, #3
	ands r0, r5
	movs r3, #2
	ldrsh r2, [r6, r3]
	adds r0, #5
	movs r4, #6
	ldrsh r3, [r6, r4]
	ldr r4, .L_08169a80
	lsls r5, r0, #1
	subs r1, r5, #2
	ldrh r1, [r4, r1]
	ldr r7, [sp, #60]
	movs r4, #170
	lsls r4, r4, #7
	adds r4, #32
	adds r2, #112
	adds r3, #62
	adds r1, r7, r1
	subs r2, r2, r0
	subs r3, r3, r0
	adds r1, r1, r4
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r6]
	ldr r3, [r6, #12]
	adds r2, r2, r3
	str r2, [r6]
	ldr r3, [r6, #16]
	ldr r2, [r6, #4]
	adds r2, r2, r3
	str r2, [r6, #4]
.L_08169a24:
	movs r5, #1
	add r10, r5
	mov r7, r10
	adds r6, #28
	cmp r7, #32
	bne .L_081699c4
.L_08169a30:
	mov r0, r8
	cmp r0, #222
	bne .L_08169aae
	ldr r5, [sp, #60]
	movs r1, #0
	mov r10, r1
	movs r6, #15
.L_08169a3e:
	bl Random16
	ands r0, r6
	subs r0, #8
	str r0, [r5]
	bl Random16
	ldr r3, [r5]
	ands r0, r6
	subs r0, #8
	str r0, [r5, #4]
	cmp r3, #0
	bge .L_08169a5c
	subs r3, #4
	b .L_08169a5e
.L_08169a5c:
	adds r3, #4
.L_08169a5e:
	str r3, [r5]
	ldr r3, [r5, #4]
	cmp r3, #0
	bge .L_08169a84
	subs r3, #4
	b .L_08169a86
	.2byte 0x0000
.L_08169a6c:
	.4byte 0xffffe000
.L_08169a70:
	.4byte 0xffe00000
.L_08169a74:
	.4byte 0xffe80000
.L_08169a78:
	.4byte gMapCellBuffer
.L_08169a7c:
	.4byte Func_081693c0
.L_08169a80:
	.4byte Data_08197424
.L_08169a84:
	adds r3, #4
.L_08169a86:
	str r3, [r5, #4]
	ldr r2, [r5, #4]
	ldr r1, [r5]
	adds r2, #52
	movs r3, #6
	str r2, [r5, #4]
	negs r3, r3
	str r2, [r5, #16]
	movs r2, #1
	str r3, [r5, #12]
	add r10, r2
	movs r3, #0
	adds r1, #100
	str r3, [r5, #24]
	mov r3, r10
	str r1, [r5]
	str r1, [r5, #8]
	adds r5, #28
	cmp r3, #64
	bne .L_08169a3e
.L_08169aae:
	mov r4, r8
	cmp r4, #221
	ble .L_08169b14
	movs r5, #0
	mov r10, r5
	ldr r7, .L_08169e14
	ldr r5, [sp, #60]
	movs r6, #2
.L_08169abe:
	mov r0, r10
	lsrs r3, r0, #31
	add r3, r10
	asrs r3, r3, #1
	adds r3, #222
	cmp r8, r3
	blt .L_08169b08
	ldrh r1, [r7]
	ldr r2, [sp, #60]
	movs r3, #170
	lsls r3, r3, #7
	adds r1, r2, r1
	adds r3, #32
	ldr r2, [r5]
	adds r1, r1, r3
	ldr r3, [r5, #4]
	subs r2, #1
	subs r3, #1
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #64]
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
	cmp r3, #0
	bge .L_08169b08
	ldr r3, [r5, #8]
	str r2, [r5, #4]
	str r3, [r5]
.L_08169b08:
	movs r4, #1
	add r10, r4
	mov r0, r10
	adds r5, #28
	cmp r0, #64
	bne .L_08169abe
.L_08169b14:
	mov r1, r8
	cmp r1, #175
	ble .L_08169b88
	movs r6, #5
	cmp r1, #221
	ble .L_08169b2c
	mov r3, r8
	subs r3, #223
	movs r6, #60
	cmp r3, #1
	bls .L_08169b2c
	movs r6, #40
.L_08169b2c:
	cmp r6, #60
	ble .L_08169b32
	movs r6, #60
.L_08169b32:
	movs r2, #0
	mov r10, r2
	cmp r6, #0
	beq .L_08169b88
	lsls r7, r6, #1
.L_08169b3c:
	mov r5, r10
	adds r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r1, r3, #1
	lsls r3, r1, #2
	subs r4, r7, r3
	cmp r4, #11
	bgt .L_08169b50
	movs r4, #12
.L_08169b50:
	cmp r4, #63
	ble .L_08169b56
	movs r4, #63
.L_08169b56:
	movs r3, #1
	mov r0, r10
	ands r3, r0
	cmp r3, #0
	beq .L_08169b62
	negs r1, r1
.L_08169b62:
	lsrs r2, r1, #31
	adds r0, r1, #0
	adds r2, r1, r2
	adds r1, #1
	lsrs r3, r1, #31
	adds r1, r1, r3
	asrs r1, r1, #1
	asrs r2, r2, #1
	movs r3, #63
	subs r3, r3, r1
	adds r0, #48
	adds r2, #111
	movs r1, #0
	mov r10, r5
	str r4, [sp, #0]
	bl Func_08143eb4
	cmp r10, r6
	bne .L_08169b3c
.L_08169b88:
	mov r1, r9
	cmp r1, #39
	bhi .L_08169bc6
	lsrs r3, r1, #31
	add r3, r9
	asrs r3, r3, #1
	adds r4, r3, #1
	cmp r4, #4
	ble .L_08169b9c
	movs r4, #4
.L_08169b9c:
	ldr r2, .L_08169e14
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #60]
	movs r3, #170
	str r0, [sp, #0]
	str r0, [sp, #4]
	lsls r3, r3, #7
	ldr r5, [sp, #20]
	adds r1, r2, r1
	adds r3, #32
	adds r1, r1, r3
	movs r2, #108
	movs r3, #60
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r0, [sp, #64]
	ldr r4, [r5, #4]
	mov lr, r4
	.2byte 0xf800
.L_08169bc6:
	mov r2, r8
	subs r2, #192
	cmp r2, #7
	bhi .L_08169c06
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	adds r4, r3, #3
	cmp r4, #6
	ble .L_08169bdc
	movs r4, #6
.L_08169bdc:
	ldr r2, .L_08169e14
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r6, [sp, #60]
	str r0, [sp, #0]
	str r0, [sp, #4]
	movs r7, #170
	ldr r0, [sp, #20]
	lsls r7, r7, #7
	adds r1, r6, r1
	adds r7, #32
	movs r2, #108
	movs r3, #60
	subs r2, r2, r4
	subs r3, r3, r4
	adds r1, r1, r7
	ldr r4, [r0, #4]
	ldr r0, [sp, #64]
	mov lr, r4
	.2byte 0xf800
.L_08169c06:
	mov r1, r8
	cmp r1, #199
	ble .L_08169c84
	mov r5, r8
	subs r5, #200
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08169c1a
	mov r3, r8
	subs r3, #197
.L_08169c1a:
	asrs r6, r3, #2
	adds r4, r6, #5
	cmp r4, #8
	ble .L_08169c24
	movs r4, #8
.L_08169c24:
	ldr r7, .L_08169e14
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r7, r3]
	ldr r2, [sp, #60]
	movs r3, #170
	str r0, [sp, #0]
	str r0, [sp, #4]
	lsls r3, r3, #7
	ldr r0, [sp, #20]
	adds r1, r2, r1
	adds r3, #32
	adds r1, r1, r3
	movs r2, #108
	movs r3, #60
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r4, [r0, #4]
	ldr r0, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	mov r1, r8
	cmp r1, #213
	bgt .L_08169c84
	adds r4, r6, #1
	cmp r4, #4
	ble .L_08169c5c
	movs r4, #4
.L_08169c5c:
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r7, r3]
	ldr r2, [sp, #60]
	movs r3, #170
	str r0, [sp, #0]
	str r0, [sp, #4]
	lsls r3, r3, #7
	ldr r5, [sp, #20]
	adds r1, r2, r1
	adds r3, #32
	adds r1, r1, r3
	movs r2, #100
	movs r3, #52
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r0, [sp, #64]
	ldr r4, [r5, #4]
	mov lr, r4
	.2byte 0xf800
.L_08169c84:
	mov r3, r8
	subs r3, #214
	cmp r3, #7
	bhi .L_08169cc2
	adds r1, r3, #0
	cmp r1, #0
	bge .L_08169c96
	mov r1, r8
	subs r1, #211
.L_08169c96:
	asrs r3, r1, #2
	cmp r3, #2
	ble .L_08169c9e
	movs r3, #2
.L_08169c9e:
	lsls r1, r3, #3
	adds r1, r1, r3
	ldr r6, [sp, #60]
	movs r3, #48
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #20]
	lsls r1, r1, #8
	movs r7, #176
	adds r1, r6, r1
	lsls r7, r7, #4
	ldr r4, [r0, #4]
	adds r1, r1, r7
	ldr r0, [sp, #64]
	movs r2, #63
	movs r3, #18
	mov lr, r4
	.2byte 0xf800
.L_08169cc2:
	mov r1, r8
	cmp r1, #221
	ble .L_08169d20
	mov r3, r8
	subs r3, #222
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r5, r3, #0
	cmp r3, #0
	bge .L_08169cda
	adds r5, r3, #3
.L_08169cda:
	asrs r5, r5, #2
	lsls r5, r5, #2
	subs r5, r3, r5
	adds r5, #3
	ldr r2, [sp, #60]
	lsls r1, r5, #3
	adds r1, r1, r5
	lsls r1, r1, #8
	movs r3, #176
	adds r1, r2, r1
	lsls r3, r3, #4
	movs r6, #48
	adds r1, r1, r3
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #80]
	ldr r0, [sp, #64]
	movs r2, #72
	movs r3, #30
	mov lr, r4
	.2byte 0xf800
	cmp r5, #5
	bne .L_08169d20
	ldr r5, [sp, #60]
	str r6, [sp, #0]
	str r6, [sp, #4]
	movs r6, #232
	lsls r6, r6, #5
	ldr r4, [sp, #80]
	ldr r0, [sp, #64]
	adds r1, r5, r6
	movs r2, #66
	movs r3, #22
	mov lr, r4
	.2byte 0xf800
.L_08169d20:
	mov r7, r8
	cmp r7, #27
	ble .L_08169da6
	ldr r0, [sp, #20]
	ldr r5, .L_08169e18
	ldr r0, [r0, #4]
	movs r1, #0
	mov r9, r0
	mov r10, r1
	movs r7, #3
.L_08169d34:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_08169d98
	movs r1, #3
	mov r0, r10
	bl Math_Mod
	ldr r2, .L_08169e1c
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #56]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #64]
	mov lr, r9
	.2byte 0xf800
	ldr r2, .L_08169e20
	mov r3, r10
	ands r3, r7
	lsls r3, r3, #2
	ldr r2, [r2, r3]
	adds r0, r5, #0
	movs r1, #62
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [r5, #16]
	cmp r3, #0
	ble .L_08169d98
	movs r0, #6
	ldrsh r3, [r5, r0]
	cmp r3, #104
	ble .L_08169d98
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_08169d98:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #3
	adds r5, #28
	cmp r10, r2
	bne .L_08169d34
.L_08169da6:
	ldr r4, [sp, #60]
	movs r5, #224
	lsls r5, r5, #3
	movs r3, #0
	adds r6, r4, r5
	mov r10, r3
	movs r5, #32
.L_08169db4:
	mov r7, r10
	mov r2, r8
	lsls r1, r7, #5
	cmp r2, #0
	bge .L_08169dc0
	adds r2, #3
.L_08169dc0:
	ldr r0, [sp, #20]
	movs r3, #31
	str r5, [sp, #0]
	str r5, [sp, #4]
	asrs r2, r2, #2
	ands r2, r3
	adds r2, r1, r2
	subs r2, #32
	ldr r4, [r0, #4]
	adds r1, r6, #0
	ldr r0, [sp, #64]
	movs r3, #88
	mov lr, r4
	.2byte 0xf800
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #5
	bne .L_08169db4
	ldr r3, [sp, #60]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	movs r5, #1
	adds r2, r3, r4
	add r8, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	mov r6, r8
	bl WaitFrames
	cmp r6, #244
	beq .L_08169e2e
	cmp r6, #16
	bgt .L_08169e0a
	b .L_08169742
.L_08169e0a:
	ldr r3, .L_08169e24
	movs r2, #3
	ldr r3, [r3, #12]
	ands r3, r2
	b .L_08169e28
.L_08169e14:
	.4byte Data_08197424
.L_08169e18:
	.4byte gMapCellBuffer
.L_08169e1c:
	.4byte Data_08197410
.L_08169e20:
	.4byte Data_08198aec
.L_08169e24:
	.4byte gInput
.L_08169e28:
	cmp r3, #0
	bne .L_08169e2e
	b .L_08169742
.L_08169e2e:
	add r7, sp, #36
	ldr r3, .L_08169eac
	ldrh r7, [r7]
	movs r2, #0
	strh r7, [r3, #4]
	ldr r0, [sp, #32]
	movs r1, #0
	str r2, [r0, #16]
	movs r0, #2
	bl Func_08013438
	bl Func_0814cca8
	bl Func_08014c4c
	movs r3, #128
	movs r1, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08169eb0
	adds r1, #160
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08169eb4
	movs r2, #160
	ldrh r3, [r3]
	lsls r2, r2, #19
	adds r2, #188
	strh r3, [r2]
	ldr r0, .L_08169eb8
	bl Func_08014644
	ldr r3, .L_08169ea8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #64
	strh r3, [r2]
	ldr r1, [sp, #60]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #75
	str r3, [r2]
	ldr r3, .L_08169ebc
	movs r5, #0
	movs r2, #128
	mov r10, r5
	movs r1, #0
	lsls r2, r2, #3
	b .L_08169ec0
	.2byte 0x0000
.L_08169ea8:
	.4byte 0x000000f0
.L_08169eac:
	.4byte Data_03001120
.L_08169eb0:
	.4byte 0x05000200
.L_08169eb4:
	.4byte 0x050001e8
.L_08169eb8:
	.4byte Func_081693b0
.L_08169ebc:
	.4byte Data_02010018
.L_08169ec0:
	movs r6, #1
	add r10, r6
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_08169ec0
	ldr r5, [sp, #60]
	movs r7, #0
	mov r10, r7
	movs r6, #0
	movs r7, #31
.L_08169ed6:
	bl Random16
	ands r0, r7
	adds r0, #32
	str r0, [r5]
	movs r0, #1
	add r10, r0
	mov r1, r10
	str r6, [r5, #4]
	str r6, [r5, #24]
	adds r5, #28
	cmp r1, #16
	bne .L_08169ed6
	ldr r4, [sp, #68]
	movs r2, #0
	ldr r3, [r4, #20]
	mov r10, r2
	cmp r3, #0
	beq .L_08169f26
	ldr r5, [sp, #60]
	add r6, sp, #88
	movs r7, #36
.L_08169f02:
	ldr r1, [sp, #68]
	ldrsh r0, [r7, r1]
	adds r1, r6, #0
	bl Func_0815e20c
	ldr r3, [r6]
	adds r7, #2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5]
	ldr r4, [sp, #68]
	movs r3, #1
	add r10, r3
	ldr r3, [r4, #20]
	adds r5, #28
	cmp r10, r3
	bne .L_08169f02
.L_08169f26:
	ldr r5, [sp, #60]
	movs r6, #224
	lsls r6, r6, #3
	movs r2, #1
	movs r3, #1
	ldr r0, .L_08169f80
	adds r1, r5, r6
	bl Func_08157cf4
	movs r0, #195
	lsls r0, r0, #1
	bl Audio_PlayCue
	ldr r3, .L_08169f78
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_08169f7c
	adds r2, #50
	strh r3, [r2]
	movs r7, #0
	mov r8, r7
.L_08169f54:
	mov r0, r8
	cmp r0, #96
	bne .L_08169f60
	movs r0, #134
	bl Func_08118088 + 0x60
.L_08169f60:
	ldr r2, [sp, #60]
	movs r3, #224
	ldr r4, [sp, #20]
	lsls r3, r3, #3
	adds r3, r2, r3
	str r3, [sp, #28]
	str r4, [sp, #16]
	movs r1, #0
	mov r10, r1
	adds r7, r2, #0
	b .L_08169f84
	.2byte 0x0000
.L_08169f78:
	.4byte 0x00000080
.L_08169f7c:
	.4byte 0x00001010
.L_08169f80:
	.4byte 0x00000130
.L_08169f84:
	mov r6, r10
	lsls r5, r6, #4
	adds r3, r5, #7
	cmp r8, r3
	bne .L_08169f94
	movs r0, #154
	bl Audio_PlayCue
.L_08169f94:
	cmp r8, r5
	bge .L_08169f9a
	b .L_0816a0c4
.L_08169f9a:
	ldr r3, [r7, #24]
	cmp r3, #31
	ble .L_08169fa2
	b .L_0816a0c4
.L_08169fa2:
	mov r1, r8
	lsls r0, r1, #4
	movs r1, #104
	bl Math_Mod
	movs r6, #104
	ldr r3, [r7, #4]
	movs r4, #17
	adds r5, r0, #0
	ldr r2, [r7]
	ldr r0, [sp, #16]
	str r6, [sp, #4]
	str r4, [sp, #0]
	adds r3, r3, r5
	ldr r1, [sp, #28]
	ldr r4, [r0, #4]
	subs r2, #8
	ldr r0, [sp, #64]
	subs r3, #216
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #4]
	movs r1, #17
	adds r3, r3, r5
	subs r5, r6, r5
	ldr r2, [r7]
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r5, [sp, #16]
	ldr r1, [sp, #28]
	ldr r4, [r5, #4]
	subs r2, #8
	subs r3, #112
	ldr r0, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	movs r1, #34
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #65
	str r1, [sp, #4]
	ldr r6, [sp, #60]
	ldr r4, [r5, #4]
	movs r5, #208
	lsls r5, r5, #4
	adds r5, #232
	subs r3, #65
	subs r2, #17
	ldr r0, [sp, #64]
	adds r1, r6, r5
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #4]
	cmp r3, #111
	bgt .L_0816a018
	adds r3, #16
	str r3, [r7, #4]
	b .L_0816a01e
.L_0816a018:
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_0816a01e:
	ldr r3, [r7, #4]
	cmp r3, #111
	ble .L_0816a0c4
	ldr r0, [r7]
	movs r6, #0
	str r0, [sp, #24]
	ldr r3, [r7, #24]
	mov r11, r6
	cmp r3, #7
	bgt .L_0816a040
	ldr r1, [sp, #60]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	adds r2, r1, r3
	movs r3, #4
	str r3, [r2]
.L_0816a040:
	ldr r2, .L_0816a208
	movs r4, #0
	mov r9, r4
.L_0816a046:
	ldr r3, [r2, #24]
	cmp r3, #0
	bne .L_0816a0b6
	str r2, [sp, #12]
	bl Random16
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #255
	ands r6, r0
	bl Random16
	movs r5, #254
	lsls r5, r5, #7
	ldr r1, [sp, #24]
	adds r5, #255
	ldr r2, [sp, #12]
	ands r5, r0
	ldr r0, .L_0816a20c
	lsls r3, r1, #16
	movs r4, #112
	adds r5, r5, r0
	str r3, [r2]
	lsls r3, r4, #16
	str r3, [r2, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	ldr r2, [sp, #12]
	asrs r3, r3, #7
	str r3, [r2, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	ldr r2, [sp, #12]
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r2, #16]
	bl Random16
	movs r5, #1
	movs r3, #7
	ldr r2, [sp, #12]
	ands r3, r0
	add r11, r5
	adds r3, #32
	mov r6, r11
	str r3, [r2, #24]
	cmp r6, #4
	beq .L_0816a0c4
.L_0816a0b6:
	movs r0, #1
	movs r1, #128
	add r9, r0
	lsls r1, r1, #3
	adds r2, #28
	cmp r9, r1
	bne .L_0816a046
.L_0816a0c4:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #5
	beq .L_0816a0d2
	b .L_08169f84
.L_0816a0d2:
	ldr r5, [sp, #68]
	movs r4, #0
	ldr r3, [r5, #20]
	mov r10, r4
	cmp r3, #0
	beq .L_0816a10e
	movs r5, #36
	movs r6, #8
.L_0816a0e2:
	cmp r8, r6
	bne .L_0816a102
	ldr r7, [sp, #68]
	movs r3, #8
	ldrsh r0, [r5, r7]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #7
	mov r3, r10
	bl Func_0814cd48
	ldrsh r0, [r5, r7]
	movs r1, #2
	bl Func_08118088
	ldr r3, [r7, #20]
.L_0816a102:
	movs r4, #1
	add r10, r4
	adds r5, #2
	adds r6, #16
	cmp r10, r3
	bne .L_0816a0e2
.L_0816a10e:
	ldr r6, .L_0816a208
	movs r5, #0
	mov r10, r5
.L_0816a114:
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_0816a18c
	subs r3, #1
	movs r2, #128
	str r3, [r6, #24]
	adds r0, r6, #0
	movs r1, #60
	lsls r2, r2, #7
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #4]
	asrs r7, r3, #16
	mov r12, r7
	cmp r7, #120
	ble .L_0816a142
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_0816a18c
.L_0816a142:
	ldr r2, [r6]
	cmp r2, #0
	blt .L_0816a18c
	asrs r7, r2, #16
	cmp r7, #126
	bgt .L_0816a18c
	cmp r3, #0
	blt .L_0816a18c
	ldr r0, [r6, #24]
	cmp r0, #0
	bge .L_0816a15a
	adds r0, #7
.L_0816a15a:
	asrs r0, r0, #3
	adds r0, #3
	ldr r2, .L_0816a210
	lsls r5, r0, #1
	mov r1, r10
	subs r3, r5, #2
	movs r4, #1
	ands r4, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #56]
	str r0, [sp, #0]
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r2, r7, r2
	mov r7, r12
	subs r3, r7, r0
	str r5, [sp, #4]
	ldr r0, [sp, #20]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	ldr r0, [sp, #64]
	mov lr, r4
	.2byte 0xf800
.L_0816a18c:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #3
	adds r6, #28
	cmp r10, r2
	bne .L_0816a114
	movs r0, #4
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #60]
	lsls r4, r4, #7
	adds r4, #232
	movs r5, #1
	adds r2, r3, r4
	add r8, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	mov r6, r8
	bl WaitFrames
	cmp r6, #144
	beq .L_0816a1c6
	b .L_08169f54
.L_0816a1c6:
	ldr r0, [sp, #60]
	movs r1, #238
	lsls r1, r1, #7
	movs r7, #0
	adds r1, #220
	mov r10, r7
	adds r5, r0, r1
.L_0816a1d4:
	ldmia r5!, {r0}
	bl Func_08020040 + 0x8
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #8
	bne .L_0816a1d4
	ldr r0, .L_0816a214
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816a208:
	.4byte gMapCellBuffer
.L_0816a20c:
	.4byte 0xffffc000
.L_0816a210:
	.4byte Data_08197410
.L_0816a214:
	.4byte Func_08143000
