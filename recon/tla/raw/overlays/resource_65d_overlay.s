.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000764
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {lr}
	ldr r3, .L_0200805c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008060
	movs r0, #0
	cmp r2, r3
	bne .L_02008058
	ldr r0, .L_02008064
.L_02008058:
	pop {pc}
	.2byte 0x0000
.L_0200805c:
	.4byte gPartyState
.L_02008060:
	.4byte 0x00000038
.L_02008064:
	.4byte Data_02000794
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	ldr r0, .L_0200806c
	bx lr
.L_0200806c:
	.4byte Data_020007c4
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {lr}
	ldr r3, .L_0200808c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008090
	cmp r2, r3
	bne .L_02008088
	ldr r0, .L_02008094
	b .L_0200808a
.L_02008088:
	ldr r0, .L_02008098
.L_0200808a:
	pop {pc}
.L_0200808c:
	.4byte gPartyState
.L_02008090:
	.4byte 0x00000037
.L_02008094:
	.4byte Data_02000fa0
.L_02008098:
	.4byte Data_020010c0
	.section .text.x0200809c,"ax",%progbits
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	movs r5, #8
.L_020080b0:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_020080c2
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_020080c2:
	adds r5, #1
	cmp r5, #63
	bls .L_020080b0
	movs r2, #170
	lsls r2, r2, #1
	adds r7, r6, r2
	movs r3, #0
	ldrsh r5, [r7, r3]
	movs r0, #158
	bl Func_020006f0
	subs r5, #1
	ldr r0, .L_02008144
	lsls r4, r5, #3
	adds r3, r4, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r4]
	bl Func_02000610
	ldr r3, .L_02008148
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	ldr r0, [r6]
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r6]
	bl Object_SetModeById
	cmp r5, #4
	beq .L_0200812c
	movs r2, #8
	ldr r0, [r6]
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #10
	bl Battle_WaitMode0
.L_0200812c:
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl Func_020006b8
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02000650
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008144:
	.4byte Data_0200073c
.L_02008148:
	.4byte gPartyState
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	movs r0, #18
	movs r1, #2
	movs r2, #8
	bl Func_020006d8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {lr}
	ldr r3, .L_02008178
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200817c
	cmp r2, r3
	bne .L_02008174
	ldr r0, .L_02008180
	b .L_02008176
.L_02008174:
	ldr r0, .L_02008184
.L_02008176:
	pop {pc}
.L_02008178:
	.4byte gPartyState
.L_0200817c:
	.4byte 0x00000038
.L_02008180:
	.4byte Data_020012ac
.L_02008184:
	.4byte Data_020010d8
	.section .text.x02008188,"ax",%progbits
	.global Func_02000188
	.thumb_func
Func_02000188:
	push {r5, lr}
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	ldr r5, .L_020081dc
	adds r0, r5, #0
	bl Func_02000698
	movs r1, #0
	movs r0, #9
	bl Func_020006a0
	bl Func_020006e8
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020081c0
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000698
	b .L_020081cc
.L_020081c0:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000698
.L_020081cc:
	movs r0, #9
	movs r1, #0
	bl Func_020006a8
	bl Func_02000650
	pop {r5, pc}
	.2byte 0x0000
.L_020081dc:
	.4byte 0x0000191f
	.section .text.x020081e0,"ax",%progbits
	.global Func_020001e0
	.thumb_func
Func_020001e0:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #5
	movs r1, #49
	movs r2, #5
	movs r3, #41
	bl Func_02000618
	movs r3, #4
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #48
	movs r2, #3
	movs r3, #2
	movs r0, #4
	bl Func_02000630
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #91
	bl Func_02000608
	add sp, #8
	pop {pc}
	.section .text.x02008218,"ax",%progbits
	.global Func_02000218
	.thumb_func
Func_02000218:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl Func_02000600
	cmp r0, #0
	bne .L_020082ee
	movs r0, #0
	bl Func_02000620
	movs r0, #1
	bl Func_02000620
	movs r0, #2
	bl Func_02000620
	movs r5, #16
	movs r0, #56
	movs r1, #46
	movs r2, #30
	movs r3, #46
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000618
	movs r0, #56
	movs r1, #174
	movs r2, #30
	movs r3, #174
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000618
	movs r2, #15
	movs r3, #23
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r2
	movs r0, #28
	movs r1, #23
	movs r2, #16
	movs r3, #16
	bl Func_02000628
	movs r3, #21
	movs r2, #88
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #2
	bl Func_02000628
	movs r3, #89
	str r3, [sp, #4]
	movs r6, #17
	movs r0, #28
	movs r1, #90
	movs r2, #4
	movs r3, #4
	str r6, [sp, #0]
	bl Func_02000628
	movs r5, #91
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #1
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl Func_02000628
	movs r3, #19
	str r3, [sp, #0]
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02000628
	mov r3, r8
	str r3, [sp, #0]
	movs r5, #92
	movs r0, #28
	movs r1, #92
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02000628
	movs r0, #30
	movs r1, #92
	movs r2, #2
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000628
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02000608
.L_020082ee:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020082f8,"ax",%progbits
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	movs r0, #0
	bx lr
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #16
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	ldr r5, .L_02008430
	strb r3, [r0]
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #240
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008434
	cmp r2, r3
	bne .L_02008414
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02000600
	cmp r0, #0
	beq .L_02008426
	movs r0, #0
	bl Func_02000620
	movs r0, #1
	bl Func_02000620
	movs r0, #2
	bl Func_02000620
	movs r5, #16
	movs r0, #56
	movs r1, #46
	movs r2, #30
	movs r3, #46
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000618
	movs r0, #56
	movs r1, #174
	movs r2, #30
	movs r3, #174
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000618
	movs r2, #15
	movs r3, #23
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r2
	movs r0, #28
	movs r1, #23
	movs r2, #16
	movs r3, #16
	bl Func_02000628
	movs r3, #21
	movs r2, #88
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #2
	bl Func_02000628
	movs r3, #89
	str r3, [sp, #4]
	movs r6, #17
	movs r0, #28
	movs r1, #90
	movs r2, #4
	movs r3, #4
	str r6, [sp, #0]
	bl Func_02000628
	movs r5, #91
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #1
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl Func_02000628
	movs r3, #19
	str r3, [sp, #0]
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02000628
	mov r3, r8
	movs r5, #92
	str r3, [sp, #0]
	movs r0, #28
	movs r1, #92
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02000628
	movs r0, #30
	movs r1, #92
	movs r2, #2
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000628
	b .L_02008426
.L_02008414:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #91
	bl Func_02000600
	cmp r0, #0
	beq .L_02008426
	bl Func_020001e0
.L_02008426:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008430:
	.4byte gPartyState
.L_02008434:
	.4byte 0x00000038
	.section .text.x02008438,"ax",%progbits
	.global Func_02000438
	.thumb_func
Func_02000438:
	push {r5, r6, lr}
	ldr r3, .L_020084e4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020084e8
	sub sp, #8
	cmp r2, r3
	bne .L_02008454
	movs r6, #12
	movs r5, #17
	b .L_02008458
.L_02008454:
	movs r6, #3
	movs r5, #9
.L_02008458:
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_020006b0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #1
	movs r3, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #14
	movs r0, #96
	movs r1, #14
	movs r2, #72
	bl Func_02000618
	movs r2, #4
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #4
	movs r1, #13
	bl Object_SetModeById
	adds r2, r5, #0
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #10
	movs r0, #4
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #123
	bl Func_020006f0
	adds r0, r6, #0
	bl Func_020006b8
	bl Func_02000650
	add sp, #8
	pop {r5, r6, pc}
.L_020084e4:
	.4byte gPartyState
.L_020084e8:
	.4byte 0x00000037
	.section .text.x020084ec,"ax",%progbits
	.global Func_020004ec
	.thumb_func
Func_020004ec:
	push {lr}
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	movs r2, #14
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	bl Func_02000438
	pop {pc}
	.section .text.x02008514,"ax",%progbits
	.global Func_02000514
	.thumb_func
Func_02000514:
	push {lr}
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	movs r1, #2
	movs r2, #6
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #14
	movs r2, #8
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	bl Func_02000438
	pop {pc}
	.2byte 0x0000
	.section .text.x02008544,"ax",%progbits
	.global Func_02000544
	.thumb_func
Func_02000544:
	push {lr}
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_020006b0
	bl Func_02000438
	pop {pc}
	.2byte 0x0000
	.section .text.x02008564,"ax",%progbits
	.global Func_02000564
	.thumb_func
Func_02000564:
	push {r5, lr}
	movs r0, #4
	bl Object_GetById
	movs r3, #10
	ldrsh r5, [r0, r3]
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	adds r3, r5, #0
	cmp r5, #0
	bge .L_02008582
	adds r3, #15
.L_02008582:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r5, r3
	cmp r3, #7
	bgt .L_020085aa
	movs r1, #4
	movs r2, #4
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #4
	movs r2, #8
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	b .L_020085c2
.L_020085aa:
	movs r2, #4
	movs r0, #4
	movs r1, #4
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #8
	movs r0, #4
	movs r1, #4
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
.L_020085c2:
	movs r1, #212
	movs r0, #4
	lsls r1, r1, #1
	movs r2, #99
	bl ObjectMotion_SetPositionAndReset
	bl Func_02000438
	pop {r5, pc}
	.section .text.x020085d4,"ax",%progbits
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	push {lr}
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	movs r2, #4
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #212
	movs r0, #4
	lsls r1, r1, #1
	movs r2, #99
	bl ObjectMotion_SetPositionAndReset
	bl Func_02000438
	pop {pc}
	.2byte 0x0000
	.section .rodata.x020086f8,"a",%progbits
.L_020086f8:
	.4byte 0x00320051
	.4byte 0x00020001
	.4byte 0x00500006
	.4byte 0x00010032
	.4byte 0x00060002
	.2byte 0xffff
.L_0200870e:
	.2byte 0x004f
	.4byte 0x00010032
	.4byte 0x00060002
	.4byte 0x0032004e
	.4byte 0x00020001
	.4byte 0xffff0006
.L_02008724:
	.4byte 0x0034004e
	.4byte 0x00020002
	.4byte 0x004e0006
	.4byte 0x00020035
	.4byte 0x00060002
	.4byte 0x0000ffff
	.global Data_0200073c
Data_0200073c:
	.4byte .L_020086f8
	.4byte 0x001d0054
	.4byte .L_020086f8
	.4byte 0x0022004d
	.4byte .L_0200870e
	.4byte 0x00260056
	.4byte .L_020086f8
	.4byte 0x0029004b
	.4byte .L_02008724
	.4byte 0x001b0045
	.global Data_02000764
Data_02000764:
	.4byte 0xffff0000
	.4byte 0x000001c8
	.4byte 0x40000248
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000794
Data_02000794:
	.4byte 0x001c0044
	.4byte 0x004c0194
	.4byte 0x019c0024
	.4byte 0x0001ffff
	.4byte 0x001c00f4
	.4byte 0x00fc0064
	.4byte 0x006c0024
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020007c4
Data_020007c4:
	.4byte 0x00000037
	.4byte 0x10101039
	.4byte 0xffffffff
	.4byte 0x10202039
	.4byte 0xffffffff
	.4byte 0x10303039
	.4byte 0xffffffff
	.4byte 0x10404039
	.4byte 0xffffffff
	.4byte 0x1050103a
	.4byte 0xffffffff
	.4byte 0x1060e002
	.4byte 0xffffffff
	.4byte 0x10709037
	.4byte 0xffffffff
	.4byte 0x1080a037
	.4byte 0xffffffff
	.4byte 0x10907037
	.4byte 0xffffffff
	.4byte 0x10a08037
	.4byte 0xffffffff
	.4byte 0x10b04038
	.4byte 0xffffffff
	.4byte 0x10c01038
	.4byte 0xffffffff
	.4byte 0x00000038
	.4byte 0x1010c037
	.4byte 0xffffffff
	.4byte 0x10203038
	.4byte 0xffffffff
	.4byte 0x10302038
	.4byte 0xffffffff
	.4byte 0x1040b037
	.4byte 0xffffffff
	.4byte 0x000001ff
.L_02008850:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_02008bf8:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.global Data_02000fa0
Data_02000fa0:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00014000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00034000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000002
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0000c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00015000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x0001e000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x0001a000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00012000
	.4byte 0xffff00cb
	.4byte .L_02008850
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0xffff0051
	.4byte .L_02008bf8
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00008000
	.4byte 0x006000f5
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020010c0
Data_020010c0:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020010d8
Data_020010d8:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200009c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200009c
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_0200009c
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_0200009c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200009c
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte Func_02000544
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte Func_020004ec
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte Func_02000514
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000191e
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000188
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001922
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001923
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001924
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001925
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001926
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001927
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001938
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001939
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001928
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001929
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000192a
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000192b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000192c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000192d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000192e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000192f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000193a
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000193b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_0200014c
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303e
	.4byte 0x50008805
	.4byte 0xffff0032
	.4byte Func_020001e0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020012ac
Data_020012ac:
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte Func_02000544
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte Func_020004ec
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte Func_02000514
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte Func_02000564
	.4byte 0x0000c602
	.4byte 0xffff0022
	.4byte Func_020005d4
	.4byte 0x50008a05
	.4byte Data_02000000 + 0x3c
	.4byte Func_02000218
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
