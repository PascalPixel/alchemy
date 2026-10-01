.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r0, #12
	movs r1, #1
	movs r2, #13
	bl Func_02001a04
	pop {pc}
	.2byte 0x0000
	.section .text.x02008048,"ax",%progbits
	.global Func_02000048
	.thumb_func
Func_02000048:
	push {r5, r6, lr}
	ldr r6, [r0, #80]
	adds r0, #100
	ldrh r5, [r0]
	adds r3, r5, #1
	lsls r5, r5, #16
	asrs r5, r5, #16
	strh r3, [r0]
	lsls r0, r5, #12
	bl Math_Sine
	movs r1, #128
	ldr r3, .L_0200807c
	lsls r1, r1, #1
	mov lr, r3
	.2byte 0xf800
	movs r3, #15
	ands r3, r5
	strh r0, [r6, #18]
	cmp r3, #0
	bne .L_02008078
	movs r0, #155
	bl Func_02001a2c
.L_02008078:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200807c:
	.4byte IwramMulQ16
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {lr}
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r3, #0
	str r3, [r0, #108]
	adds r0, #100
	strh r3, [r0]
	strh r3, [r2, #18]
	pop {pc}
	.section .text.x020080b0,"ax",%progbits
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r2, .L_020080c0
	adds r3, #240
	str r2, [r3]
	bx lr
	.2byte 0x0000
.L_020080c0:
	.4byte 0x02ee0000
	.section .text.x020080c4,"ax",%progbits
	.global Func_020000c4
	.thumb_func
Func_020000c4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #176
	adds r3, #240
	lsls r2, r2, #17
	str r2, [r3]
	bx lr
	.global Data_020000d4
Data_020000d4:
	.4byte 0x00004770
	.section .text.x020080d8,"ax",%progbits
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {lr}
	adds r0, r1, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r2, [r0, #16]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r3, #8
	bne .L_020080fa
	cmp r2, #36
	bne .L_020080fa
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_SetBit
.L_020080fa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200810c
	bl Func_02000838
.L_0200810c:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008110,"ax",%progbits
	.global Func_02000110
	.thumb_func
Func_02000110:
	push {r5, r6, lr}
	sub sp, #12
	movs r3, #5
	movs r2, #13
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #7
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001a24
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200813c
	b .L_020083ae
.L_0200813c:
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_SetBit
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #176
	movs r2, #216
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_0200195c
	movs r0, #12
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_Launch
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r6, #29
.L_02008186:
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r2, #255
	ldr r3, [r5, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bgt .L_020081a4
	movs r2, #160
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #24]
.L_020081a4:
	ldr r3, [r5, #24]
	movs r0, #1
	str r3, [r5, #28]
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_02008186
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #1
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #120
	movs r2, #200
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r2, #200
	movs r1, #136
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #3
	bl Battle_WaitMode0
	ldr r5, .L_020083b4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #0
	movs r2, #16
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #5
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #16
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #3
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_0200195c
	movs r0, #13
	bl Object_GetById
	ldr r3, .L_020083b8
	movs r6, #0
	str r3, [r0, #108]
	movs r0, #13
	bl Object_GetById
	adds r0, #100
	strh r6, [r0]
.L_020083ae:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020083b4:
	.4byte gPartyState
.L_020083b8:
	.4byte Func_02000048
	.section .text.x020083bc,"ax",%progbits
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {lr}
	sub sp, #12
	movs r3, #8
	movs r2, #9
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #9
	movs r1, #9
	movs r2, #2
	movs r3, #3
	bl Func_02001a24
	add sp, #12
	pop {pc}
	.section .text.x020083dc,"ax",%progbits
	.global Func_020003dc
	.thumb_func
Func_020003dc:
	push {lr}
	sub sp, #12
	movs r3, #12
	movs r2, #7
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r2, #5
	movs r3, #2
	str r1, [sp, #8]
	bl Func_02001a24
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x020083fc,"ax",%progbits
	.global Func_020003fc
	.thumb_func
Func_020003fc:
	push {lr}
	sub sp, #12
	movs r3, #14
	movs r2, #11
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #39
	movs r1, #2
	movs r2, #4
	movs r3, #3
	bl Func_02001a24
	add sp, #12
	pop {pc}
	.section .text.x0200841c,"ax",%progbits
	.global Func_0200041c
	.thumb_func
Func_0200041c:
	push {lr}
	sub sp, #12
	movs r3, #21
	movs r2, #9
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #39
	movs r1, #2
	movs r2, #3
	movs r3, #3
	bl Func_02001a24
	add sp, #12
	pop {pc}
	.section .text.x0200843c,"ax",%progbits
	.global Func_0200043c
	.thumb_func
Func_0200043c:
	push {lr}
	sub sp, #12
	movs r3, #25
	movs r2, #8
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r2, #4
	movs r3, #3
	str r1, [sp, #8]
	bl Func_02001a24
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x0200845c,"ax",%progbits
	.global Func_0200045c
	.thumb_func
Func_0200045c:
	push {lr}
	movs r0, #13
	bl Func_02000080
	pop {pc}
	.2byte 0x0000
	.section .text.x02008468,"ax",%progbits
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, r6, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008528
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008528
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_ClearBit
	movs r0, #13
	bl Func_02000080
	movs r1, #135
	movs r2, #184
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200195c
	movs r3, #176
	lsls r3, r3, #13
	str r3, [r5, #12]
	movs r0, #12
	movs r1, #11
	movs r2, #0
	bl ObjectMotion_Launch
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	ldr r1, .L_0200852c
	ldr r2, .L_02008530
	bl ObjectMotion_SetSpeedParameters
	movs r1, #156
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r6, #23
.L_020084e6:
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r2, #255
	ldr r3, [r5, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bgt .L_02008504
	movs r2, #160
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #24]
.L_02008504:
	ldr r3, [r5, #24]
	movs r0, #1
	str r3, [r5, #28]
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_020084e6
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_02008528:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200852c:
	.4byte 0x00023333
.L_02008530:
	.4byte 0x00011999
	.section .text.x02008534,"ax",%progbits
	.global Func_02000534
	.thumb_func
Func_02000534:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008552
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008560
.L_02008552:
	movs r0, #123
	bl Func_02001a2c
	movs r0, #2
	bl Func_020019ec
	b .L_02008614
.L_02008560:
	bl Func_02001914
	movs r0, #0
	bl Func_020019fc
	movs r2, #16
	movs r3, #160
	lsls r3, r3, #7
	movs r0, #11
	movs r1, #0
	negs r2, r2
	bl Func_02001a0c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ae
	ldr r0, .L_02008618
	bl Func_02001994
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	b .L_020085be
.L_020085ae:
	ldr r0, .L_0200861c
	bl Func_02001994
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
.L_020085be:
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #11
	ldr r1, .L_02008620
	bl ObjectMotion_SetSpeedParameters
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02008624
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_020085f4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl ObjectMotion_ResetAndSetPosition
.L_020085f4:
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_0200195c
	movs r2, #208
	movs r0, #4
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	bl Func_0200191c
.L_02008614:
	pop {pc}
	.2byte 0x0000
.L_02008618:
	.4byte 0x00002706
.L_0200861c:
	.4byte 0x00002707
.L_02008620:
	.4byte 0x00013333
.L_02008624:
	.4byte gPartyState
	.section .text.x02008630,"ax",%progbits
	.global Func_02000630
	.thumb_func
Func_02000630:
	push {r5, lr}
	ldr r3, .L_020086ac
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200868e
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #252
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_0200864c:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #224
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_0200864c
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #226
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008676:
	ldr r4, .L_020086b0
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008676
	ldr r3, .L_020086b4
	strh r0, [r3]
.L_0200868e:
	ldr r3, .L_020086ac
	movs r1, #12
	ldr r0, [r3]
	ldr r5, .L_020086b8
	lsrs r0, r0, #2
	bl __umodsi3
	lsls r0, r0, #1
	ldrsh r0, [r5, r0]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #254
	strh r0, [r3]
	pop {r5, pc}
	.2byte 0x0000
.L_020086ac:
	.4byte Data_0300122c
.L_020086b0:
	.4byte 0x05000100
.L_020086b4:
	.4byte 0x05000102
.L_020086b8:
	.4byte Data_02001c30
	.section .text.x020086bc,"ax",%progbits
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #200
	adds r2, #85
	str r2, [r3]
	ldr r0, .L_02008798
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200879c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_020086f2
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_020086fa
.L_020086f2:
	movs r0, #48
	adds r0, #255
	bl GameFlag_SetBit
.L_020086fa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200872a
	movs r1, #136
	movs r2, #146
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_0200195c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200195c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200195c
.L_0200872a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008746
	movs r1, #156
	movs r2, #184
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200195c
.L_02008746:
	ldr r3, .L_0200879c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02008788
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02008788
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008788
	movs r0, #13
	bl Object_GetById
	ldr r3, .L_020087a0
	str r3, [r0, #108]
	movs r0, #13
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
.L_02008788:
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {r5, pc}
.L_02008798:
	.4byte Func_02000630
.L_0200879c:
	.4byte gPartyState
.L_020087a0:
	.4byte Func_02000048
	.section .text.x020087a4,"ax",%progbits
	.global Func_020007a4
	.thumb_func
Func_020007a4:
	push {r5, r6, lr}
	ldr r3, .L_0200882c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #1
	beq .L_020087ce
	cmp r3, #2
	beq .L_020087c0
	cmp r3, #3
	bne .L_020087ca
.L_020087c0:
	movs r0, #150
	lsls r0, r0, #4
	bl GameFlag_ClearBit
	b .L_020087e4
.L_020087ca:
	cmp r3, #4
	bne .L_020087d8
.L_020087ce:
	movs r0, #150
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_020087e4
.L_020087d8:
	cmp r3, #5
	bne .L_020087e4
	movs r0, #150
	lsls r0, r0, #4
	bl GameFlag_SetBit
.L_020087e4:
	movs r0, #150
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008826
	movs r5, #47
	movs r6, #8
	movs r0, #38
	movs r1, #47
	movs r2, #26
	movs r3, #12
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020018f4
	movs r3, #72
	str r3, [sp, #0]
	movs r0, #102
	movs r1, #47
	movs r2, #26
	movs r3, #12
	str r5, [sp, #4]
	bl Func_020018f4
	movs r0, #38
	movs r1, #47
	movs r2, #26
	movs r3, #12
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020018ec
.L_02008826:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_0200882c:
	.4byte gPartyState
	.section .text.x02008830,"ax",%progbits
	.global Func_02000830
	.thumb_func
Func_02000830:
	push {lr}
	bl Func_020019f4
	pop {pc}
	.section .text.x02008838,"ax",%progbits
	.global Func_02000838
	.thumb_func
Func_02000838:
	push {r5, lr}
	bl Func_02001914
	movs r0, #0
	bl Func_020019fc
	ldr r0, .L_020089f8
	bl Func_02001994
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #4
	bl Func_020019c4
	movs r2, #10
	movs r1, #0
	movs r0, #10
	bl Func_020019a4
	bl Func_020019f4
	bl Func_02001914
	movs r0, #0
	bl Func_020019fc
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #4
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #168
	movs r1, #1
	movs r2, #134
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020019e4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #24
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020089fc
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020019cc
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020019a4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008a50
	.2byte 0x0000
.L_020089f8:
	.4byte 0x000026c3
.L_020089fc:
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020019cc
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #10
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_020019a4
	movs r0, #10
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
.L_02008a50:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_020019c4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020019c4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r2, #16
	movs r3, #176
	lsls r3, r3, #8
	movs r1, #16
	negs r2, r2
	movs r0, #11
	bl Func_02001a0c
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_020019c4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #9
	bl Func_020019c4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020019a4
	movs r1, #4
	movs r0, #7
	bl Func_02001964
	movs r0, #7
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
	movs r1, #0
	movs r0, #7
	movs r2, #10
	bl Func_020019a4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #7
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #9
	movs r2, #16
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #0
	negs r2, r2
	str r5, [r0, #24]
	movs r0, #7
	bl Func_02001a0c
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_020019c4
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020019c4
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	movs r2, #0
	adds r0, #10
	bl Func_020019a4
	bl UiWork_FinalizePendingCore
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #7
	bl Func_020019c4
	movs r1, #8
	movs r0, #9
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #10
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020019c4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_020019cc
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020019cc
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r2, #128
	movs r0, #7
	adds r1, r5, #0
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020019a4
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_020019c4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020019c4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020019c4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_020019c4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_020019c4
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020019a4
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_020019c4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020019c4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_020019c4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020019a4
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_020019cc
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_020019c4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r2, #16
	movs r0, #11
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #11
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_020019c4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020019c4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #11
	bl Func_020019c4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020019c4
	movs r1, #132
	movs r2, #55
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020019c4
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	movs r0, #9
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #10
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020019a4
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_020019cc
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020019cc
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #168
	movs r1, #1
	movs r2, #128
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020019e4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_020019c4
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #55
	movs r0, #11
	bl Func_020019c4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020019c4
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r2, #20
	movs r0, #4
	movs r1, #8
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #7
	bl Func_020019c4
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020019a4
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020019a4
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl UiText_OpenMessageAtObject
	ldr r3, .L_02009484
	movs r2, #157
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #4
	bgt .L_0200940c
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200939c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #11
	movs r1, #0
	bl Func_020019a4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020093f8
.L_0200939c:
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_020019c4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #10
	bl Func_020019c4
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_020019c4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
.L_020093f8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_020094f6
.L_0200940c:
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r0, #4
	adds r2, #2
	strh r2, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009488
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #7
	bl Func_020019b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #7
	bl Func_020019c4
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #11
	movs r1, #0
	bl Func_020019a4
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020094e6
.L_02009484:
	.4byte gPartyState
.L_02009488:
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #7
	bl Func_020019b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #40
	movs r0, #11
	bl Func_020019c4
	movs r0, #11
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
.L_020094e6:
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_020019b4
	movs r0, #10
	bl Battle_WaitMode0
.L_020094f6:
	movs r2, #10
	negs r2, r2
	movs r1, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020019a4
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_020019a4
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #7
	bl Func_020019c4
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020019a4
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r2, #32
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #32
	movs r1, #0
	negs r2, r2
	movs r0, #9
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_0200195c
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_0200195c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #168
	movs r1, #1
	movs r2, #134
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020019e4
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #11
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020019c4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #7
	bl Func_020019c4
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020019a4
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_020019c4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #7
	bl Func_020019c4
	movs r2, #0
	movs r1, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #32
	movs r0, #7
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #7
	bl Func_020019c4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020019a4
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #11
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #8
	movs r2, #16
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #10
	movs r0, #11
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_020098ac
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_020098b0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009856
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009856:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200195c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #11
	ldr r1, .L_020098ac
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009894
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl ObjectMotion_ResetAndSetPosition
.L_02009894:
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_0200195c
	bl Func_0200191c
	pop {r5, pc}
	.2byte 0x0000
.L_020098ac:
	.4byte 0x00013333
.L_020098b0:
	.4byte gPartyState
	.section .rodata.x02009a34,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x00000064
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000c9
	.4byte 0x101040c9
	.4byte 0xffffffff
	.4byte 0x102020c8
	.4byte 0xffffffff
	.4byte 0x103040ca
	.4byte 0xffffffff
	.4byte 0x104010c9
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0121
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00003000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00005000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0x005100f4
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00004000
	.4byte 0xffff0121
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000534
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_020000b0
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_020000c4
	.4byte 0x00000602
	.4byte 0xffff0016
	.4byte Func_02000830
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_02000038
	.4byte 0x10008c15
	.4byte 0x09610008
	.4byte Data_020000d4 + 0x1
	.4byte 0x00008c15
	.4byte 0x09610008
	.4byte Func_020000d8
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte Func_02000110
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte Func_020003bc
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte Func_020003dc
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte Func_020003fc
	.4byte 0x50008905
	.4byte 0xffff0022
	.4byte Func_0200041c
	.4byte 0x50008905
	.4byte 0xffff0023
	.4byte Func_0200043c
	.4byte 0x10008715
	.4byte 0x196e000d
	.4byte Func_0200045c
	.4byte 0x00008715
	.4byte 0x196e000d
	.4byte Func_02000468
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001c30
Data_02001c30:
	.4byte 0x62c85f0c
	.4byte 0x6a406684
	.4byte 0x69c06a00
	.4byte 0x69c06980
	.4byte 0x6a406a00
	.4byte 0x62c86684
