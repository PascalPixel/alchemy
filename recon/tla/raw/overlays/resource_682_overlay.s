.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02002bcc
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_02002bfc
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008060
	ldr r0, .L_02008064
	b .L_02008062
.L_02008060:
	ldr r0, .L_02008068
.L_02008062:
	pop {pc}
.L_02008064:
	.4byte Data_02002efc
.L_02008068:
	.4byte Data_02002c5c
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	ldr r3, [r0, #24]
	ldr r2, .L_0200807c
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	bx lr
.L_0200807c:
	.4byte 0xfffffa00
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	adds r6, r2, #0
	bl Func_02002a44
	movs r0, #0
	bl Func_02002b64
	movs r0, #158
	bl Func_02002bc4
	ldr r5, .L_020080f8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	cmp r6, #0
	bne .L_020080d2
	movs r2, #8
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
	b .L_020080dc
.L_020080d2:
	movs r2, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
.L_020080dc:
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_02002b44
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02002a4c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020080f8:
	.4byte gPartyState
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {r5, r6, lr}
	adds r5, r0, #0
	subs r3, r5, #1
	ldr r6, .L_02008140
	cmp r3, #1
	bhi .L_0200811a
	ldr r3, .L_02008144
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, .L_02008148
	str r3, [r0, #108]
.L_0200811a:
	cmp r5, #7
	bne .L_02008130
	movs r0, #8
	bl Func_020029f4
	adds r0, r6, #0
	movs r1, #7
	movs r2, #1
	bl Func_02000080
	b .L_0200813c
.L_02008130:
	adds r0, r6, #0
	adds r0, #8
	adds r1, r5, #0
	movs r2, #0
	bl Func_02000080
.L_0200813c:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008140:
	.4byte Data_020031fc
.L_02008144:
	.4byte gPartyState
.L_02008148:
	.4byte Func_0200006c
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	movs r1, #196
	movs r2, #142
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #64
	bl Func_02002b5c
	movs r0, #64
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #64
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #12]
	pop {pc}
	.2byte 0x0000
	.section .text.x02008178,"ax",%progbits
	.global Func_02000178
	.thumb_func
Func_02000178:
	push {lr}
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02002b5c
	pop {pc}
	.2byte 0x0000
	.section .text.x02008188,"ax",%progbits
	.global Func_02000188
	.thumb_func
Func_02000188:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #115
	bl Func_020029cc
	bl Func_02002a44
	movs r0, #0
	bl Func_02002b64
	ldr r0, .L_02008428
	bl Func_02002adc
	movs r1, #192
	movs r0, #32
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02002afc
	movs r0, #32
	movs r1, #6
	movs r2, #20
	bl ObjectMotion_Launch
	movs r0, #32
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r0, #176
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #16
	bl Func_02002b34
	bl Func_02002b3c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #32
	bl Func_02002b24
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #33
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #35
	adds r1, #255
	movs r0, #33
	bl Func_02002b1c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #33
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #33
	bl Func_02002b1c
	movs r0, #33
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r0, #32
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #33
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #32
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	adds r1, #204
	adds r2, #102
	movs r0, #33
	bl ObjectMotion_SetSpeedParameters
	movs r0, #32
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #16
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #32
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #33
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #16
	ands r5, r3
	movs r1, #0
	negs r2, r2
	strb r5, [r0]
	movs r0, #33
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #33
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #32
	bl Func_02002b1c
	movs r0, #32
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #176
	movs r2, #0
	movs r0, #32
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #33
	bl Func_02002b04
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #33
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r2, #5
	movs r0, #34
	movs r1, #0
	bl Func_02002aec
	movs r0, #32
	movs r1, #34
	bl Object_LinkObjectAndSetCallback
	movs r0, #33
	movs r1, #34
	bl Object_LinkObjectAndSetCallback
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #34
	ldr r1, .L_0200842c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #32
	movs r0, #34
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #34
	lsls r1, r1, #6
	bl Func_02002b04
	movs r0, #208
	movs r1, #1
	movs r2, #148
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #16
	bl Func_02002b34
	bl Func_02002b3c
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #34
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #34
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r0, #34
	movs r1, #6
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #128
	movs r2, #128
	movs r0, #34
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r2, #48
	movs r0, #34
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #208
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r1, #128
	movs r2, #128
	movs r0, #32
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #33
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r2, #16
	movs r0, #32
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #16
	movs r0, #33
	movs r1, #16
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #32
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #32
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r2, #16
	movs r0, #33
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	bl Func_02002a4c
	pop {r5, pc}
.L_02008428:
	.4byte 0x000025f3
.L_0200842c:
	.4byte 0x00013333
	.section .text.x02008430,"ax",%progbits
	.global Func_02000430
	.thumb_func
Func_02000430:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	adds r6, r0, #0
	str r3, [r6, #28]
	str r3, [r6, #24]
	ldr r0, .L_02008464
	bl Func_02002adc
	adds r0, r5, #0
	movs r1, #0
	bl Func_02002af4
	adds r0, r5, #0
	movs r1, #0
	bl Func_02002b04
	adds r0, r6, #0
	movs r1, #8
	bl Object_SetActionCallback
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008464:
	.4byte 0x000023f4
	.section .text.x02008468,"ax",%progbits
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, r6, lr}
	ldr r5, .L_020084b0
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02002adc
	movs r1, #0
	adds r0, r6, #0
	bl Func_02002ae4
	bl Func_02002bb4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008498
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02002adc
	b .L_020084a4
.L_02008498:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02002adc
.L_020084a4:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02002af4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020084b0:
	.4byte 0x000025c5
	.section .text.x020084b4,"ax",%progbits
	.global Func_020004b4
	.thumb_func
Func_020004b4:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008508
	ldr r5, .L_0200854c
	adds r0, r5, #0
	bl Func_02002adc
	movs r1, #0
	adds r0, r6, #0
	bl Func_02002ae4
	bl Func_02002bb4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020084f2
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02002adc
	b .L_020084fe
.L_020084f2:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02002adc
.L_020084fe:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02002af4
	b .L_02008548
.L_02008508:
	ldr r5, .L_02008550
	adds r0, r5, #0
	bl Func_02002adc
	movs r1, #0
	adds r0, r6, #0
	bl Func_02002ae4
	bl Func_02002bb4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008534
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02002adc
	b .L_02008540
.L_02008534:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02002adc
.L_02008540:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02002af4
.L_02008548:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200854c:
	.4byte 0x000025c8
.L_02008550:
	.4byte 0x000023f0
	.section .text.x02008554,"ax",%progbits
	.global Func_02000554
	.thumb_func
Func_02000554:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #114
	bl Func_020029c4
	cmp r0, #0
	bne .L_02008578
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #114
	bl Func_020029cc
	ldr r0, .L_02008588
	bl Func_02002adc
	b .L_0200857e
.L_02008578:
	ldr r0, .L_0200858c
	bl Func_02002adc
.L_0200857e:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02002af4
	pop {r5, pc}
.L_02008588:
	.4byte 0x000025cf
.L_0200858c:
	.4byte 0x000025d0
	.section .text.x02008590,"ax",%progbits
	.global Func_02000590
	.thumb_func
Func_02000590:
	push {r5, r6, lr}
	ldr r5, .L_020085d8
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02002adc
	movs r1, #0
	adds r0, r6, #0
	bl Func_02002ae4
	bl Func_02002bb4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020085c0
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02002adc
	b .L_020085cc
.L_020085c0:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02002adc
.L_020085cc:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02002af4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085d8:
	.4byte 0x000023f8
	.section .text.x020085dc,"ax",%progbits
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002a44
	movs r0, #0
	bl Func_02002b64
	movs r1, #160
	movs r2, #1
	adds r0, r5, #0
	adds r1, #255
	negs r2, r2
	bl Func_02002a5c
	cmp r0, #0
	bne .L_02008606
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #116
	bl Func_020029d4
.L_02008606:
	bl Func_02002a4c
	pop {r5, pc}
	.section .text.x0200860c,"ax",%progbits
	.global Func_0200060c
	.thumb_func
Func_0200060c:
	push {lr}
	movs r1, #160
	movs r0, #31
	adds r1, #255
	bl Func_02002bbc
	movs r3, #192
	movs r1, #156
	lsls r3, r3, #8
	movs r0, #31
	lsls r1, r1, #17
	ldr r2, .L_02008634
	bl Func_02002aa4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #116
	bl Func_020029cc
	pop {pc}
.L_02008634:
	.4byte 0x02510000
	.section .text.x02008638,"ax",%progbits
	.global Func_02000638
	.thumb_func
Func_02000638:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #34
	bl Func_020029c4
	cmp r0, #0
	bne .L_0200864a
	b .L_02008a5a
.L_0200864a:
	movs r0, #0
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008656
	b .L_02008a5a
.L_02008656:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #117
	bl Func_020029cc
	bl Func_02002a44
	movs r0, #0
	bl Func_02002b64
	ldr r0, .L_02008708
	bl Func_02002adc
	movs r3, #192
	movs r1, #157
	lsls r3, r3, #8
	movs r0, #29
	lsls r1, r1, #17
	ldr r2, .L_0200870c
	bl Func_02002aa4
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #157
	movs r2, #145
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02002afc
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #29
	bl Func_02002b2c
	bl Func_02002b3c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #157
	movs r2, #156
	movs r0, #29
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r1, #0
	movs r0, #29
	bl Func_02002ae4
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008710
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200872c
.L_02008708:
	.4byte 0x00002f82
.L_0200870c:
	.4byte 0x02a70000
.L_02008710:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #29
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
.L_0200872c:
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #16
	movs r2, #0
	movs r0, #30
	bl Func_02002b74
	movs r0, #30
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #29
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #4
	movs r2, #0
	movs r0, #30
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02002afc
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r0, #29
	movs r1, #2
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r0, #30
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #29
	bl Func_02002b1c
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #29
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #29
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_02002b1c
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	bl Func_0200060c
	movs r1, #4
	movs r2, #0
	movs r0, #30
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl Func_02002afc
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #5
	bl Func_02002aec
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02002b1c
	movs r2, #5
	movs r0, #30
	movs r1, #0
	bl Func_02002aec
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #29
	movs r1, #0
	bl Func_02002aec
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #1
	bl Func_02002b2c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #29
	ldr r1, .L_02008a5c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #100
	movs r0, #29
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #30
	movs r0, #4
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008a3c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #30
	bl ObjectMotion_ResetAndSetPosition
.L_02008a3c:
	movs r0, #30
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	bl Func_02002a4c
.L_02008a5a:
	pop {pc}
.L_02008a5c:
	.4byte 0x00013333
	.section .text.x02008a60,"ax",%progbits
	.global Func_02000a60
	.thumb_func
Func_02000a60:
	push {lr}
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008a74
	ldr r0, .L_02008a78
	b .L_02008a76
.L_02008a74:
	ldr r0, .L_02008a7c
.L_02008a76:
	pop {pc}
.L_02008a78:
	.4byte Data_0200b410
.L_02008a7c:
	.4byte Data_0200320c
	.section .text.x02008a80,"ax",%progbits
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	push {lr}
	ldr r3, .L_02008b14
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008b10
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #124
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008a9c:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #96
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008a9c
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #98
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008ac6:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #128
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008ac6
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #130
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008af0:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #160
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008af0
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #162
	strh r0, [r3]
.L_02008b10:
	pop {pc}
	.2byte 0x0000
.L_02008b14:
	.4byte Data_0300122c
	.section .text.x02008b18,"ax",%progbits
	.global Func_02000b18
	.thumb_func
Func_02000b18:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #200
	subs r2, #172
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_02008c00
	bl Func_020029bc
	movs r0, #8
	bl Func_020029fc
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #3
	movs r0, #13
	bl Func_02002b14
	movs r0, #23
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #8
	str r5, [r0, #24]
	movs r0, #23
	bl Object_GetById
	str r5, [r0, #28]
	bl Func_02002ba4
	movs r1, #8
	movs r2, #9
	movs r0, #0
	bl Func_02002bac
	movs r0, #3
	bl Func_020029fc
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008be8
	movs r0, #20
	movs r1, #2
	bl Func_02002b14
	movs r0, #21
	movs r1, #3
	bl Func_02002b14
	movs r0, #22
	movs r1, #3
	bl Func_02002b14
	movs r1, #3
	movs r0, #19
	bl Func_02002b14
	movs r0, #18
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #115
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008be8
	movs r0, #32
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
.L_02008be8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #116
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008bfa
	bl Func_0200060c
.L_02008bfa:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008c00:
	.4byte Func_02000a80
	.section .text.x02008c04,"ax",%progbits
	.global Func_02000c04
	.thumb_func
Func_02000c04:
	push {r5, r6, lr}
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	sub sp, #8
	bl Func_020029c4
	cmp r0, #0
	beq .L_02008c3a
	movs r5, #16
	movs r6, #23
	movs r0, #11
	movs r1, #51
	movs r2, #5
	movs r3, #4
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002a0c
	movs r0, #11
	movs r1, #51
	movs r2, #5
	movs r3, #6
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002a04
.L_02008c3a:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008c40,"ax",%progbits
	.global Func_02000c40
	.thumb_func
Func_02000c40:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r1, #0
	lsls r3, r3, #16
	movs r0, #244
	asrs r7, r3, #16
	lsls r0, r0, #1
	adds r3, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_020029ec
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008c84
	movs r1, #1
	ldr r5, [r6, #80]
	bl Func_020029dc
	ldr r1, .L_02008c8c
	adds r0, r6, #0
	bl Func_020029e4
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #16]
	ldr r1, .L_02008c88
	adds r2, #9
	strh r3, [r2]
	strb r1, [r5, #26]
	strh r7, [r5, #18]
.L_02008c84:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008c88:
	.4byte 0x00000000
.L_02008c8c:
	.4byte Data_02003620
	.section .text.x02008c90,"ax",%progbits
	.global Func_02000c90
	.thumb_func
Func_02000c90:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	adds r5, r1, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #224
	lsls r3, r3, #13
	mov r10, r3
	ldr r1, [r6, #12]
	movs r3, #128
	lsls r3, r3, #5
	mov r11, r3
	movs r3, #15
	adds r5, r0, #0
	ldr r2, [r6, #16]
	add r1, r10
	mov r8, r3
	ldr r0, [r6, #8]
	str r3, [sp, #0]
	mov r3, r11
	bl Func_02000c40
	movs r0, #151
	bl Func_02002bc4
	movs r0, #15
	bl Battle_WaitMode0
	ldr r1, [r5, #12]
	movs r3, #240
	lsls r3, r3, #8
	mov r9, r3
	mov r3, r8
	ldr r2, [r5, #16]
	add r1, r10
	ldr r0, [r5, #8]
	str r3, [sp, #0]
	mov r3, r9
	bl Func_02000c40
	movs r0, #151
	bl Func_02002bc4
	movs r0, #15
	bl Battle_WaitMode0
	ldr r1, [r6, #12]
	mov r3, r8
	ldr r2, [r6, #16]
	add r1, r10
	ldr r0, [r6, #8]
	str r3, [sp, #0]
	mov r3, r11
	bl Func_02000c40
	movs r0, #151
	bl Func_02002bc4
	movs r0, #15
	bl Battle_WaitMode0
	ldr r1, [r5, #12]
	mov r3, r8
	ldr r2, [r5, #16]
	ldr r0, [r5, #8]
	add r1, r10
	str r3, [sp, #0]
	mov r3, r9
	bl Func_02000c40
	movs r0, #151
	bl Func_02002bc4
	movs r0, #15
	bl Battle_WaitMode0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
	.section .text.x02008d4c,"ax",%progbits
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [sp, #32]
	mov r8, r1
	mov r10, r2
	ldr r7, [r3, #108]
	bl ObjectTable_ReadActiveValue
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectTable_ReadActiveValue
	adds r5, r0, #0
	adds r0, r6, #0
	bl BattleFx_GetResourceId
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r1]
	asrs r0, r0, #16
	mov r1, r8
	mov r2, r10
	bl UiText_OpenMessageWindow
	adds r0, r6, #0
	ldr r3, [sp, #28]
	movs r1, #0
	mov r2, r9
	bl Func_02002a2c
	ldr r3, [sp, #52]
	cmp r3, #0
	beq .L_02008db6
	b .L_02008dae
.L_02008da8:
	ldr r0, [sp, #52]
	bl WaitFrames
.L_02008dae:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_02008da8
.L_02008db6:
	adds r0, r5, #0
	bl BattleFx_GetResourceId
	movs r2, #226
	lsls r2, r2, #1
	adds r1, r7, r2
	adds r3, r0, #0
	ldrh r0, [r1]
	lsls r3, r3, #16
	adds r2, r0, #1
	strh r2, [r1]
	lsls r0, r0, #16
	ldr r1, [sp, #36]
	ldr r2, [sp, #40]
	asrs r0, r0, #16
	bl UiText_OpenMessageWindow
	adds r0, r5, #0
	movs r1, #0
	ldr r2, [sp, #44]
	ldr r3, [sp, #48]
	bl Func_02002a2c
	b .L_02008dec
.L_02008de6:
	movs r0, #1
	bl WaitFrames
.L_02008dec:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_02008de6
	movs r0, #1
	bl WaitFrames
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.section .text.x02008e04,"ax",%progbits
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push {r5, r6, lr}
	adds r5, r1, #0
	bl ObjectTable_ReadActiveValue
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectTable_ReadActiveValue
	adds r5, r0, #0
	adds r0, r6, #0
	bl UiWork_FinalizeEntityMatchingLocalizedId
	adds r0, r5, #0
	bl UiWork_FinalizeEntityMatchingLocalizedId
	pop {r5, r6, pc}
	.section .text.x02008e24,"ax",%progbits
	.global Func_02000e24
	.thumb_func
Func_02000e24:
	push {lr}
	movs r0, #151
	lsls r0, r0, #4
	bl Func_020029cc
	pop {pc}
	.section .text.x02008e30,"ax",%progbits
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #151
	lsls r0, r0, #4
	sub sp, #28
	bl Func_020029c4
	cmp r0, #0
	bne .L_02008e4e
	bl .L_0200a99e
.L_02008e4e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #113
	bl Func_020029cc
	bl Func_02002a44
	movs r0, #0
	bl Func_02002b64
	ldr r0, .L_02009160
	bl Func_02002adc
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #25
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #26
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #156
	movs r2, #130
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02002a9c
	movs r1, #228
	movs r2, #154
	movs r0, #26
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02002a9c
	movs r1, #168
	movs r2, #154
	lsls r2, r2, #18
	lsls r1, r1, #16
	movs r0, #27
	bl Func_02002a9c
	movs r0, #78
	bl Func_02002bc4
	movs r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #4
	bl Func_02002b1c
	movs r1, #156
	movs r2, #154
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #156
	movs r1, #1
	movs r2, #150
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #17
	bl Func_02002b34
	bl Func_02002b3c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #39
	bl Func_02002bc4
	movs r1, #152
	movs r2, #144
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #16
	movs r3, #192
	movs r0, #7
	negs r1, r1
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02002b74
	movs r3, #192
	movs r0, #5
	movs r1, #0
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02002b74
	movs r1, #16
	movs r3, #192
	movs r0, #6
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02002b74
	movs r1, #32
	movs r3, #208
	lsls r3, r3, #8
	negs r1, r1
	movs r2, #0
	movs r0, #28
	bl Func_02002b74
	movs r0, #28
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02002b1c
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02002b04
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #0
	movs r0, #28
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #208
	movs r2, #0
	movs r0, #28
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002b04
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02002b1c
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #25
	bl Func_02002ae4
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200901e
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009042
.L_0200901e:
	movs r0, #35
	bl Battle_WaitMode0
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
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
.L_02009042:
	movs r2, #0
	movs r0, #7
	movs r1, #0
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_02002b1c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02002b1c
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #28
	bl Func_02002b1c
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002b04
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #25
	bl Func_02002af4
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #25
	bl Func_02002ae4
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #28
	movs r1, #0
	bl Func_02002b04
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009164
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020091a2
	.2byte 0x0000
.L_02009160:
	.4byte 0x0000243b
.L_02009164:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #6
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02002af4
.L_020091a2:
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02002b04
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #28
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #4
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #7
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #28
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02002b24
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02002afc
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r2, #0
	movs r0, #7
	movs r1, #0
	bl Func_02002afc
	movs r0, #28
	movs r1, #0
	bl Func_02002b04
	movs r1, #172
	movs r2, #154
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #26
	bl ObjectMotion_SetPositionAndReset
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_02002b1c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02002b1c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_02002b1c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02002b1c
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02002b1c
	movs r0, #6
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #4
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #7
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_02002b24
	movs r0, #28
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02002b24
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #8
	bl Func_02002b04
	movs r2, #154
	movs r1, #232
	lsls r2, r2, #2
	movs r0, #27
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02002b1c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02002b1c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02002b1c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_02002b1c
	movs r1, #2
	adds r1, #255
	movs r2, #60
	movs r0, #28
	bl Func_02002b1c
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r2, #0
	movs r0, #27
	movs r1, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r2, #25
	movs r0, #27
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #0
	movs r0, #6
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02002b1c
	movs r1, #4
	movs r2, #50
	adds r1, #255
	movs r0, #5
	bl Func_02002b1c
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002b04
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #28
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #25
	bl Func_02002b04
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02002b1c
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #42
	movs r0, #28
	bl Func_02002b1c
	movs r2, #16
	negs r2, r2
	movs r0, #28
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #25
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #25
	bl Func_02002b1c
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002b04
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #6
	bl Func_02002b1c
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #25
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #7
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #128
	movs r0, #28
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	movs r1, #8
	negs r2, r2
	movs r0, #28
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02002b1c
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #7
	movs r2, #7
	strb r3, [r0]
	negs r1, r1
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02002b04
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #25
	bl Func_02002af4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #28
	bl Func_02002b1c
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b24
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #204
	movs r0, #25
	adds r1, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #25
	movs r1, #0
	bl Func_02002b04
	movs r2, #0
	movs r0, #25
	movs r1, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #25
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #27
	bl Func_02002b1c
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02002b04
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #26
	bl Func_02002b1c
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002b04
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #27
	bl Func_02002b1c
	movs r1, #0
	movs r0, #27
	bl Func_02002b04
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_02002afc
	movs r1, #0
	movs r0, #7
	bl Func_02002b04
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #204
	movs r0, #28
	adds r1, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #28
	bl Func_02002b04
	movs r0, #2
	bl Battle_WaitMode0
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #8
	ands r5, r3
	movs r2, #8
	negs r1, r1
	strb r5, [r0]
	movs r0, #28
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #160
	orrs r6, r3
	strb r6, [r0]
	lsls r1, r1, #7
	movs r0, #28
	bl Func_02002b04
	movs r0, #28
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #4
	adds r1, #255
	movs r2, #42
	movs r0, #26
	bl Func_02002b1c
	movs r0, #26
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r0, #26
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r2, #0
	movs r0, #7
	movs r1, #0
	bl Func_02002afc
	movs r1, #0
	movs r0, #28
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r0, #27
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r0, #27
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #12
	movs r2, #0
	negs r1, r1
	movs r0, #26
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_02002b1c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02002b1c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_02002b1c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02002b1c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #28
	bl Func_02002b1c
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #27
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #12
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #27
	bl Func_02002b1c
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02002b1c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #28
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #26
	bl Func_02002b1c
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002afc
	movs r0, #28
	movs r1, #0
	bl Func_02002b04
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #4
	adds r1, #255
	movs r2, #42
	movs r0, #27
	bl Func_02002b1c
	movs r0, #27
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r0, #27
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #160
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02002afc
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #28
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #26
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002afc
	movs r1, #0
	movs r0, #28
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #27
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #160
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02002afc
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #28
	bl Func_02002b04
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002b04
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #25
	bl Func_02002af4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02002b04
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #28
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002b04
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #6
	bl Func_02002b1c
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02002b04
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #7
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #25
	bl Func_02002b04
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #25
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r0, #27
	movs r1, #0
	bl Func_02002b04
	movs r0, #27
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #28
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #27
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #27
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #27
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #27
	bl Object_LinkObjectAndSetCallback
	movs r0, #28
	movs r1, #27
	bl Object_LinkObjectAndSetCallback
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #27
	movs r1, #20
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #27
	movs r1, #2
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #27
	movs r1, #32
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #32
	movs r0, #27
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002afc
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_02002afc
	movs r1, #0
	movs r0, #28
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #28
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r1, #128
	movs r2, #128
	movs r0, #26
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #26
	movs r1, #0
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #20
	movs r0, #26
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #26
	movs r1, #0
	movs r2, #48
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #28
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_02002a9c
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r0, #28
	movs r1, #25
	bl Object_LinkObjectAndSetCallback
	movs r1, #128
	movs r2, #128
	movs r0, #25
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #24
	movs r0, #25
	movs r1, #22
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	movs r1, #1
	bl Func_02002b2c
	movs r0, #25
	movs r1, #0
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #25
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r1, #176
	movs r0, #25
	lsls r1, r1, #8
	bl Func_02002b04
	movs r0, #152
	movs r1, #1
	movs r2, #162
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #17
	bl Func_02002b34
	bl Func_02002b3c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #28
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #32
	movs r0, #28
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #28
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #28
	bl Func_02002b1c
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #28
	bl Func_02002b1c
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #28
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02002b24
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_02002afc
	movs r1, #0
	movs r0, #7
	bl Func_02002b04
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_02002b04
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r1, #102
	adds r2, #51
	movs r0, #28
	bl ObjectMotion_SetSpeedParameters
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #0
	strb r3, [r0]
	movs r1, #0
	mov r9, r2
	movs r0, #28
	subs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #28
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	bl Func_02002b04
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #6
	bl Func_02002b1c
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #0
	movs r0, #7
	bl Func_02002b04
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #7
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02002b1c
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #7
	bl Func_02002b1c
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r0, #6
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #25
	movs r1, #0
	bl Func_02002af4
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #25
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #25
	bl Func_02002af4
	movs r0, #78
	bl Func_02002bc4
	movs r0, #25
	movs r1, #0
	movs r2, #80
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #25
	bl Func_02002a9c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #152
	movs r1, #1
	movs r2, #156
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #17
	bl Func_02002b34
	bl Func_02002b3c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #3
	bl Func_02002bc4
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #4
	movs r2, #30
	adds r1, #255
	movs r0, #6
	bl Func_02002b1c
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #7
	bl Func_02002b1c
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #6
	bl Func_02002b1c
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r0, #7
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_02002b1c
	movs r1, #192
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #7
	bl Func_02002afc
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #5
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02002b1c
	movs r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #1
	movs r0, #28
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #28
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r0, #28
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r0, #28
	bl Func_02002b04
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r0, #28
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r0, #7
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02002b24
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #6
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #1
	movs r0, #6
	bl Func_02002b04
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #6
	bl Func_02000c90
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02002b1c
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #6
	bl Func_02002b1c
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r3, #14
	str r3, [sp, #12]
	movs r2, #9
	mov r8, r3
	mov r3, r9
	str r2, [sp, #20]
	str r3, [sp, #24]
	movs r5, #8
	movs r3, #18
	movs r6, #6
	mov r10, r2
	movs r0, #5
	movs r2, #5
	movs r1, #5
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r6, [sp, #8]
	str r5, [sp, #16]
	bl Func_02002b0c
	movs r0, #28
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02002b24
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02002b1c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_02002b1c
	mov r2, r8
	str r2, [sp, #12]
	mov r3, r10
	mov r2, r9
	str r3, [sp, #20]
	str r2, [sp, #24]
	movs r0, #5
	movs r1, #8
	movs r2, #5
	movs r3, #18
	str r5, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #16]
	bl Func_02000d4c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #7
	bl Func_02002b04
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a6a8
	movs r1, #6
	movs r0, #5
	bl Func_02000e04
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02002b1c
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a6dc
.L_0200a6a8:
	movs r1, #6
	movs r0, #5
	bl Func_02000e04
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_02002b1c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #28
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02002af4
.L_0200a6dc:
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #6
	bl Func_02002b04
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02002b1c
	movs r1, #8
	movs r2, #45
	adds r1, #255
	movs r0, #6
	bl Func_02002b1c
	movs r0, #28
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r0, #5
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r0, #5
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02002af4
	movs r0, #28
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r0, #6
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r0, #6
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	bl Func_02002af4
	movs r0, #28
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #28
	bl Func_02002af4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #28
	bl Func_02002b1c
	movs r2, #8
	negs r2, r2
	movs r0, #28
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r0, #28
	bl Func_02002b04
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02002b1c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #28
	bl Func_02002b1c
	movs r0, #28
	movs r1, #0
	bl Func_02002af4
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #7
	bl Func_02002af4
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #45
	adds r1, #255
	movs r0, #28
	bl Func_02002b1c
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02002af4
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02002afc
	movs r1, #224
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02002afc
	movs r1, #0
	movs r0, #7
	bl Func_02002b04
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #28
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #5
	ldr r1, .L_0200a9ac
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200a9b0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a8d0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200a8d0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200a9ac
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a90e
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200a90e:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200a9ac
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a94c
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200a94c:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #28
	ldr r1, .L_0200a9ac
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #28
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a98a
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #28
	bl ObjectMotion_ResetAndSetPosition
.L_0200a98a:
	movs r0, #28
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02002a9c
	bl Func_02002a4c
.L_0200a99e:
	add sp, #28
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a9ac:
	.4byte 0x00013333
.L_0200a9b0:
	.4byte gPartyState
	.section .rodata.x0200abcc,"a",%progbits
	.global Data_02002bcc
Data_02002bcc:
	.4byte 0xffff0000
	.4byte 0x00000118
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002bfc
Data_02002bfc:
	.4byte 0x000000b6
	.4byte 0x101010b7
	.4byte 0xffffffff
	.4byte 0x102020b7
	.4byte 0xffffffff
	.4byte 0x103030b7
	.4byte 0xffffffff
	.4byte 0x104040b7
	.4byte 0xffffffff
	.4byte 0x105050b7
	.4byte 0xffffffff
	.4byte 0x106060b7
	.4byte 0xffffffff
	.4byte 0x107010b8
	.4byte 0xffffffff
	.4byte 0x108010b9
	.4byte 0xffffffff
	.4byte 0x10928002
	.4byte 0xffffffff
	.4byte 0x10a070ba
	.4byte 0xffffffff
	.4byte 0x10b130bb
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02002c5c
Data_02002c5c:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00005000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000008
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00010000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff00ff
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00013000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002efc
Data_02002efc:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff01cd
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00005000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00005000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00008000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00013000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001b000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001d000
	.4byte 0xffff00ff
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00018000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00013000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff003f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0003b000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0003d000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200b1e4:
	.2byte 0xffff
.L_0200b1e6:
	.2byte 0x0024
	.4byte 0x00020002
	.4byte 0x00060002
	.4byte 0x00040024
	.4byte 0x00020002
	.4byte 0xffff0006
	.global Data_020031fc
Data_020031fc:
	.4byte .L_0200b1e6
	.4byte 0x0009001d
	.4byte .L_0200b1e4
	.4byte 0x00000000
	.global Data_0200320c
Data_0200320c:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_020000fc
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte Func_020000fc
	.4byte 0x0000c403
	.4byte 0xffff0018
	.4byte 0x004024c0
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_0200014c
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000178
	.4byte 0x00000002
	.4byte 0x0a750064
	.4byte Func_02000638
	.4byte 0x00000002
	.4byte 0x09700013
	.4byte Func_02000e24
	.4byte 0x00000002
	.4byte 0x09710014
	.4byte Func_02000e30
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000023ec
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000023ed
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000023ee
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000023ef
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_020004b4
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000023f3
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000430
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000023f5
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000023f6
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000023f7
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000590
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000023fb
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000023fc
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000023fd
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000023fe
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000023ff
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002400
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002401
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002402
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002403
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002404
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002405
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002423
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000242c
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_020005dc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200b410
Data_0200b410:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_020000fc
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_020000fc
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte Func_020000fc
	.4byte 0x0000c403
	.4byte 0xffff0018
	.4byte 0x004024c0
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_0200014c
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000178
	.4byte 0x00000002
	.4byte 0x0a750064
	.4byte Func_02000638
	.4byte 0x00000002
	.4byte 0x09700013
	.4byte Func_02000e24
	.4byte 0x00000002
	.4byte 0x09710014
	.4byte Func_02000e30
	.4byte 0x00000002
	.4byte 0x0973001a
	.4byte Func_02000188
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025c2
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000025c3
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025c4
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000468
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_020004b4
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000025cb
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000025cc
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000025cd
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000025ce
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_02000554
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x000025d1
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025d2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025d3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025d4
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025d5
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000025d6
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000025d7
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000025d8
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000025d9
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000025da
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000025db
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000025dc
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000268c
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000269c
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_020005dc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003620
Data_02003620:
	.4byte 0x00000026
