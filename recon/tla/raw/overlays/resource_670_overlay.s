.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r2, [r0, #80]
	movs r1, #192
	ldrh r3, [r2, #18]
	lsls r1, r1, #3
	adds r3, r3, r1
	strh r3, [r2, #18]
	movs r0, #0
	bx lr
	.section .text.x02008048,"ax",%progbits
	.global Func_02000048
	.thumb_func
Func_02000048:
	ldr r3, [r0, #8]
	str r3, [r0, #68]
	ldr r3, [r0, #12]
	str r3, [r0, #72]
	movs r0, #0
	bx lr
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008098
	movs r2, #1
	ldr r3, [r3]
	adds r7, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02008094
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, .L_0200809c
	adds r3, r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r6, r5, #0
	adds r5, r3, r5
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r3, r3, #16
	ldr r1, [r7, #68]
	ldr r2, [r7, #72]
	lsls r3, r3, #16
	adds r6, r3, r6
	adds r1, r1, r5
	adds r2, r2, r6
	ldr r3, [r7, #16]
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
.L_02008094:
	movs r0, #1
	pop {r5, r6, r7, pc}
.L_02008098:
	.4byte Data_0300122c
.L_0200809c:
	.4byte 0xffff0000
	.section .text.x020080a0,"ax",%progbits
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {lr}
	ldr r3, .L_020080c0
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020080b6
	movs r1, #10
	bl Object_SetPartAttribute
	b .L_020080bc
.L_020080b6:
	movs r1, #0
	bl Object_SetPartAttribute
.L_020080bc:
	pop {pc}
	.2byte 0x0000
.L_020080c0:
	.4byte Data_0300122c
	.section .text.x020080c4,"ax",%progbits
	.global Func_020000c4
	.thumb_func
Func_020000c4:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #98
	ldrb r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_020080da
	adds r3, #255
	strb r3, [r5]
	b .L_020080f0
.L_020080da:
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	adds r3, #10
	strb r3, [r5]
	bl Random16Far
	strh r0, [r6, #6]
.L_020080f0:
	movs r0, #1
	pop {r5, r6, pc}
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	ldr r3, .L_02008118
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200811c
	movs r0, #0
	cmp r2, r3
	bne .L_02008114
	ldr r0, .L_02008120
.L_02008114:
	pop {pc}
	.2byte 0x0000
.L_02008118:
	.4byte gPartyState
.L_0200811c:
	.4byte 0x00000070
.L_02008120:
	.4byte Data_0200635c
	.section .text.x0200812c,"ax",%progbits
	.global Func_0200012c
	.thumb_func
Func_0200012c:
	push {lr}
	ldr r3, .L_02008184
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008188
	cmp r2, r3
	bne .L_02008144
	ldr r0, .L_0200818c
	b .L_02008182
.L_02008144:
	ldr r3, .L_02008190
	cmp r2, r3
	bne .L_0200814e
	ldr r0, .L_02008194
	b .L_02008182
.L_0200814e:
	ldr r3, .L_02008198
	cmp r2, r3
	bne .L_02008158
	ldr r0, .L_0200819c
	b .L_02008182
.L_02008158:
	ldr r3, .L_020081a0
	cmp r2, r3
	bne .L_02008162
	ldr r0, .L_020081a4
	b .L_02008182
.L_02008162:
	ldr r3, .L_020081a8
	cmp r2, r3
	bne .L_02008180
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200817c
	ldr r2, .L_020081ac
	movs r3, #0
	strb r3, [r2, #22]
.L_0200817c:
	ldr r0, .L_020081ac
	b .L_02008182
.L_02008180:
	ldr r0, .L_020081b0
.L_02008182:
	pop {pc}
.L_02008184:
	.4byte gPartyState
.L_02008188:
	.4byte 0x0000006e
.L_0200818c:
	.4byte Data_02006510
.L_02008190:
	.4byte 0x00000071
.L_02008194:
	.4byte Data_020066d8
.L_02008198:
	.4byte 0x0000006f
.L_0200819c:
	.4byte Data_020069a8
.L_020081a0:
	.4byte 0x00000072
.L_020081a4:
	.4byte Data_02006a98
.L_020081a8:
	.4byte 0x00000070
.L_020081ac:
	.4byte Data_02006cd8
.L_020081b0:
	.4byte Data_020064f8
	.section .text.x020081b4,"ax",%progbits
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	push {lr}
	ldr r3, .L_020081f8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020081fc
	cmp r2, r3
	bne .L_020081cc
	ldr r0, .L_02008200
	b .L_020081f6
.L_020081cc:
	ldr r3, .L_02008204
	cmp r2, r3
	bne .L_020081d6
	ldr r0, .L_02008208
	b .L_020081f6
.L_020081d6:
	ldr r3, .L_0200820c
	cmp r2, r3
	bne .L_020081e0
	ldr r0, .L_02008210
	b .L_020081f6
.L_020081e0:
	ldr r3, .L_02008214
	cmp r2, r3
	bne .L_020081ea
	ldr r0, .L_02008218
	b .L_020081f6
.L_020081ea:
	ldr r3, .L_0200821c
	cmp r2, r3
	bne .L_020081f4
	ldr r0, .L_02008220
	b .L_020081f6
.L_020081f4:
	ldr r0, .L_02008224
.L_020081f6:
	pop {pc}
.L_020081f8:
	.4byte gPartyState
.L_020081fc:
	.4byte 0x0000006e
.L_02008200:
	.4byte Data_02006ef4
.L_02008204:
	.4byte 0x00000071
.L_02008208:
	.4byte Data_020070d4
.L_0200820c:
	.4byte 0x0000006f
.L_02008210:
	.4byte Data_02007434
.L_02008214:
	.4byte 0x00000072
.L_02008218:
	.4byte Data_020074dc
.L_0200821c:
	.4byte 0x00000070
.L_02008220:
	.4byte Data_02007698
.L_02008224:
	.4byte Data_02006ee8
	.section .text.x02008228,"ax",%progbits
	.global Func_02000228
	.thumb_func
Func_02000228:
	push {lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_02008248
	bl Func_02005b4c
	movs r1, #0
	movs r0, #15
	bl Func_02005b6c
	bl Func_02005aa4
	pop {pc}
.L_02008248:
	.4byte 0x00002002
	.section .text.x0200824c,"ax",%progbits
	.global Func_0200024c
	.thumb_func
Func_0200024c:
	push {lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_0200826c
	bl Func_02005b4c
	movs r1, #0
	movs r0, #23
	bl Func_02005b6c
	bl Func_02005aa4
	pop {pc}
.L_0200826c:
	.4byte 0x0000200b
	.section .text.x02008270,"ax",%progbits
	.global Func_02000270
	.thumb_func
Func_02000270:
	push {lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_020082bc
	bl Func_02005b4c
	movs r0, #24
	movs r1, #0
	bl Func_02005b64
	ldr r3, .L_020082c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	bl Func_02005b7c
	bl Func_02005aa4
	pop {pc}
	.2byte 0x0000
.L_020082bc:
	.4byte 0x0000200f
.L_020082c0:
	.4byte gPartyState
	.section .text.x020082c4,"ax",%progbits
	.global Func_020002c4
	.thumb_func
Func_020002c4:
	push {lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_020082e4
	bl Func_02005b4c
	movs r1, #0
	movs r0, #12
	bl Func_02005b6c
	bl Func_02005aa4
	pop {pc}
.L_020082e4:
	.4byte 0x0000201d
	.section .text.x020082e8,"ax",%progbits
	.global Func_020002e8
	.thumb_func
Func_020002e8:
	push {lr}
	ldr r3, .L_02008310
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #190
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	adds r2, #255
	adds r3, r3, r2
	ldr r2, .L_02008314
	lsls r3, r3, #16
	movs r0, #1
	cmp r3, r2
	bls .L_0200830e
	movs r0, #0
.L_0200830e:
	pop {pc}
.L_02008310:
	.4byte gPartyState
.L_02008314:
	.4byte 0x3ffe0000
	.section .text.x02008318,"ax",%progbits
	.global Func_02000318
	.thumb_func
Func_02000318:
	push {lr}
	bl Func_020002e8
	cmp r0, #0
	beq .L_0200832c
	movs r0, #17
	movs r1, #20
	bl Func_02005c84
	b .L_02008348
.L_0200832c:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_0200834c
	bl Func_02005b4c
	movs r0, #20
	movs r1, #0
	bl Func_02005b64
	bl Func_02005aa4
.L_02008348:
	pop {pc}
	.2byte 0x0000
.L_0200834c:
	.4byte 0x00002030
	.section .text.x02008350,"ax",%progbits
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {lr}
	bl Func_020002e8
	cmp r0, #0
	beq .L_02008364
	movs r0, #18
	movs r1, #21
	bl Func_02005c84
	b .L_02008380
.L_02008364:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_02008384
	bl Func_02005b4c
	movs r0, #21
	movs r1, #0
	bl Func_02005b64
	bl Func_02005aa4
.L_02008380:
	pop {pc}
	.2byte 0x0000
.L_02008384:
	.4byte 0x00002032
	.section .text.x02008388,"ax",%progbits
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {lr}
	bl Func_020002e8
	cmp r0, #0
	beq .L_0200839c
	movs r0, #19
	movs r1, #22
	bl Func_02005c84
	b .L_020083b8
.L_0200839c:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_020083bc
	bl Func_02005b4c
	movs r0, #22
	movs r1, #0
	bl Func_02005b64
	bl Func_02005aa4
.L_020083b8:
	pop {pc}
	.2byte 0x0000
.L_020083bc:
	.4byte 0x00002034
	.section .text.x020083c0,"ax",%progbits
	.global Func_020003c0
	.thumb_func
Func_020003c0:
	push {lr}
	bl Func_020002e8
	cmp r0, #0
	beq .L_020083d4
	movs r0, #6
	movs r1, #23
	bl Func_02005c94
	b .L_020083f0
.L_020083d4:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_020083f4
	bl Func_02005b4c
	movs r0, #23
	movs r1, #0
	bl Func_02005b64
	bl Func_02005aa4
.L_020083f0:
	pop {pc}
	.2byte 0x0000
.L_020083f4:
	.4byte 0x00002036
	.section .text.x020083f8,"ax",%progbits
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	push {lr}
	bl Func_020002e8
	cmp r0, #0
	beq .L_0200840a
	movs r0, #25
	bl Func_02005c8c
	b .L_02008426
.L_0200840a:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_02008428
	bl Func_02005b4c
	movs r0, #25
	movs r1, #0
	bl Func_02005b64
	bl Func_02005aa4
.L_02008426:
	pop {pc}
.L_02008428:
	.4byte 0x0000203a
	.section .text.x0200842c,"ax",%progbits
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {lr}
	movs r0, #142
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008476
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r3, .L_020084d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #36
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #36
	bl Func_02005c8c
	movs r1, #176
	movs r0, #36
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02005aa4
	b .L_020084d0
.L_02008476:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_020084d8
	bl Func_02005b4c
	movs r2, #40
	movs r0, #36
	movs r1, #0
	bl Func_02005b5c
	movs r0, #36
	movs r1, #0
	bl Func_02005b64
	ldr r3, .L_020084d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #36
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #36
	bl Func_02005c8c
	movs r1, #176
	movs r0, #36
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #142
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02005aa4
.L_020084d0:
	pop {pc}
	.2byte 0x0000
.L_020084d4:
	.4byte gPartyState
.L_020084d8:
	.4byte 0x00001efc
	.section .text.x020084dc,"ax",%progbits
	.global Func_020004dc
	.thumb_func
Func_020004dc:
	push {lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_020084fc
	bl Func_02005b4c
	movs r0, #16
	movs r1, #0
	bl Func_02005b64
	bl Func_02005aa4
	pop {pc}
.L_020084fc:
	.4byte 0x00002028
	.section .text.x02008500,"ax",%progbits
	.global Func_02000500
	.thumb_func
Func_02000500:
	push {lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r0, .L_02008520
	bl Func_02005b4c
	movs r0, #16
	movs r1, #0
	bl Func_02005b64
	bl Func_02005aa4
	pop {pc}
.L_02008520:
	.4byte 0x0000202a
	.section .text.x02008524,"ax",%progbits
	.global Func_02000524
	.thumb_func
Func_02000524:
	push {lr}
	ldr r3, .L_02008558
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200855c
	cmp r2, r3
	beq .L_0200853e
	ldr r3, .L_02008560
	cmp r2, r3
	bne .L_02008548
.L_0200853e:
	movs r1, #15
	movs r2, #2
	bl Func_02000590
	b .L_02008556
.L_02008548:
	ldr r3, .L_02008564
	cmp r2, r3
	bne .L_02008556
	movs r1, #26
	movs r2, #2
	bl Func_02000590
.L_02008556:
	pop {pc}
.L_02008558:
	.4byte gPartyState
.L_0200855c:
	.4byte 0x0000006f
.L_02008560:
	.4byte 0x00000072
.L_02008564:
	.4byte 0x00000070
	.section .text.x02008568,"ax",%progbits
	.global Func_02000568
	.thumb_func
Func_02000568:
	push {lr}
	movs r0, #1
	bl Func_02000524
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x0200857c,"ax",%progbits
	.global Func_0200057c
	.thumb_func
Func_0200057c:
	push {lr}
	movs r0, #0
	bl Func_02000524
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008590,"ax",%progbits
	.global Func_02000590
	.thumb_func
Func_02000590:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r2, #0
	movs r5, #0
	mov r8, r0
	adds r7, r1, #0
	cmp r5, r6
	bcs .L_020085c8
.L_020085a2:
	adds r0, r7, r5
	bl Object_GetById
	mov r3, r8
	adds r0, #35
	adds r1, r5, #1
	cmp r3, #0
	beq .L_020085ba
	ldrb r2, [r0]
	movs r3, #239
	ands r3, r2
	b .L_020085c0
.L_020085ba:
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
.L_020085c0:
	strb r3, [r0]
	adds r5, r1, #0
	cmp r5, r6
	bcc .L_020085a2
.L_020085c8:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020085d0,"ax",%progbits
	.global Func_020005d0
	.thumb_func
Func_020005d0:
	push {lr}
	ldr r0, .L_020085dc
	bl Func_02005c6c
	pop {pc}
	.2byte 0x0000
.L_020085dc:
	.4byte Data_02006324
	.section .text.x020085e0,"ax",%progbits
	.global Func_020005e0
	.thumb_func
Func_020005e0:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #34
	movs r1, #33
	movs r2, #23
	movs r3, #18
	bl Func_02005a64
	movs r3, #23
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #33
	movs r2, #1
	movs r3, #1
	movs r0, #34
	bl Func_02005a6c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x02008618,"ax",%progbits
	.global Func_02000618
	.thumb_func
Func_02000618:
	push {lr}
	cmp r0, #1
	bne .L_02008622
	bl Func_020005e0
.L_02008622:
	pop {pc}
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #34
	movs r2, #16
	movs r3, #7
	bl Func_02005a64
	movs r3, #16
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #34
	movs r2, #1
	movs r3, #1
	movs r0, #36
	bl Func_02005a6c
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200865c,"ax",%progbits
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	push {lr}
	cmp r0, #1
	bne .L_02008666
	bl Func_02000624
.L_02008666:
	pop {pc}
	.section .text.x02008668,"ax",%progbits
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #34
	movs r2, #16
	movs r3, #11
	bl Func_02005a64
	movs r3, #16
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #34
	movs r2, #1
	movs r3, #1
	movs r0, #38
	bl Func_02005a6c
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x020086a0,"ax",%progbits
	.global Func_020006a0
	.thumb_func
Func_020006a0:
	push {lr}
	cmp r0, #1
	bne .L_020086aa
	bl Func_02000668
.L_020086aa:
	pop {pc}
	.section .text.x020086ac,"ax",%progbits
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #36
	movs r2, #14
	movs r3, #9
	bl Func_02005a64
	movs r3, #14
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #36
	movs r2, #1
	movs r3, #1
	movs r0, #36
	bl Func_02005a6c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x020086e4,"ax",%progbits
	.global Func_020006e4
	.thumb_func
Func_020006e4:
	push {lr}
	cmp r0, #1
	bne .L_020086ee
	bl Func_020006ac
.L_020086ee:
	pop {pc}
	.section .text.x020086f0,"ax",%progbits
	.global Func_020006f0
	.thumb_func
Func_020006f0:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #36
	movs r2, #18
	movs r3, #9
	bl Func_02005a64
	movs r3, #18
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #36
	movs r2, #1
	movs r3, #1
	movs r0, #38
	bl Func_02005a6c
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x02008728,"ax",%progbits
	.global Func_02000728
	.thumb_func
Func_02000728:
	push {lr}
	cmp r0, #1
	bne .L_02008732
	bl Func_020006f0
.L_02008732:
	pop {pc}
	.section .text.x02008734,"ax",%progbits
	.global Func_02000734
	.thumb_func
Func_02000734:
	push {r5, r6, r7, lr}
	movs r0, #11
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_020087c4
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #18
	bne .L_020087c4
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #11
	bl Object_GetById
	adds r7, r6, #0
	movs r3, #3
	adds r0, #85
	adds r7, #85
	strb r3, [r0]
	strb r3, [r7]
.L_0200876c:
	movs r0, #1
	bl WaitFrames
	ldr r5, [r6, #40]
	cmp r5, #0
	bne .L_0200876c
	movs r0, #188
	bl Func_02005c9c
	movs r0, #10
	bl WaitFrames
	strb r5, [r7]
	movs r0, #11
	bl Object_GetById
	movs r3, #6
	adds r0, #85
	movs r2, #18
	strb r5, [r0]
	movs r1, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #5
	movs r2, #1
	movs r3, #1
	bl Func_02005a6c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #234
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087c0
	bl Func_02001318
.L_020087c0:
	bl Func_02005aa4
.L_020087c4:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x020087c8,"ax",%progbits
	.global Func_020007c8
	.thumb_func
Func_020007c8:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005bbc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #152
	movs r0, #153
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Func_02005ba4
	movs r0, #176
	movs r1, #1
	movs r2, #196
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	ldr r5, .L_0200895c
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #196
	ldr r0, [r5]
	movs r1, #72
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #196
	ldr r0, [r5]
	movs r1, #88
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #10
	lsls r1, r1, #7
	movs r0, #7
	bl ObjectMotion_ArmCallback
	ldr r0, .L_02008960
	bl Func_02005b4c
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	ldr r1, [r5]
	movs r0, #26
	bl Func_02005b1c
	ldr r1, [r5]
	movs r0, #5
	bl Func_02005b1c
	ldr r1, [r5]
	movs r0, #6
	bl Func_02005b1c
	movs r0, #1
	bl WaitFrames
	movs r0, #26
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #5
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #6
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #6
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02008964
	movs r0, #26
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02008968
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200896c
	movs r0, #6
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088fe
	bl Func_02000978
	b .L_02008902
.L_020088fe:
	bl Func_02000b2c
.L_02008902:
	bl Func_02000d74
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #26
	ldr r1, .L_02008970
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02008970
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #6
	ldr r1, .L_02008970
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02008974
	movs r0, #5
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #26
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #243
	bl GameFlag_SetBit
	bl Func_02005aa4
	pop {r5, pc}
	.2byte 0x0000
.L_0200895c:
	.4byte gPartyState
.L_02008960:
	.4byte 0x00001e6a
.L_02008964:
	.4byte Data_02005d18
.L_02008968:
	.4byte Data_02005d5c
.L_0200896c:
	.4byte Data_02005da0
.L_02008970:
	.4byte 0x00019999
.L_02008974:
	.4byte Data_02005e54
	.section .text.x02008978,"ax",%progbits
	.global Func_02000978
	.thumb_func
Func_02000978:
	push {r5, lr}
	ldr r5, .L_02008b24
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #20
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	ldr r0, .L_02008b28
	bl Func_02005b4c
	movs r1, #0
	movs r0, #6
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020089d8
	movs r1, #128
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008a06
.L_020089d8:
	movs r1, #128
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #5
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02005b64
.L_02008a06:
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	ldr r5, .L_02008b24
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #20
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02005b94
	movs r0, #6
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #4
	movs r2, #10
	adds r1, #255
	movs r0, #5
	bl Func_02005b94
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #4
	adds r1, #255
	movs r2, #10
	movs r0, #7
	bl Func_02005b94
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	pop {r5, pc}
.L_02008b24:
	.4byte gPartyState
.L_02008b28:
	.4byte 0x00001e6c
	.section .text.x02008b2c,"ax",%progbits
	.global Func_02000b2c
	.thumb_func
Func_02000b2c:
	push {r5, lr}
	ldr r0, .L_02008c88
	bl Func_02005b4c
	movs r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	ldr r5, .L_02008c8c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02005b9c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r2, #20
	movs r0, #26
	movs r1, #0
	bl Func_02005b5c
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #6
	bl Func_02005b94
	movs r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02005b94
	movs r0, #6
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #4
	movs r2, #10
	adds r1, #255
	movs r0, #5
	bl Func_02005b94
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #4
	adds r1, #255
	movs r2, #10
	movs r0, #7
	bl Func_02005b94
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	pop {r5, pc}
.L_02008c88:
	.4byte 0x00001e8c
.L_02008c8c:
	.4byte gPartyState
	.section .text.x02008c90,"ax",%progbits
	.global Func_02000c90
	.thumb_func
Func_02000c90:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r5, .L_02008d64
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02008d68
	adds r1, #204
	bl Func_02005ba4
	movs r0, #140
	movs r1, #1
	movs r2, #176
	movs r3, #1
	lsls r2, r2, #15
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02008d6c
	bl Func_02005b4c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02005b7c
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02008d70
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008d30
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02008d30:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #7
	bl Func_02005b0c
	movs r0, #131
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_SetBit
	movs r0, #7
	movs r1, #0
	bl Event_PrepareObjectAndApplyValue
	bl Func_02005aa4
	pop {r5, pc}
	.2byte 0x0000
.L_02008d64:
	.4byte gPartyState
.L_02008d68:
	.4byte 0x00026666
.L_02008d6c:
	.4byte 0x00001efb
.L_02008d70:
	.4byte 0x00019999
	.section .text.x02008d74,"ax",%progbits
	.global Func_02000d74
	.thumb_func
Func_02000d74:
	push {r5, r6, lr}
	bl Func_020032c8
	movs r0, #0
	bl Func_02005c9c
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009184
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009188
	adds r1, #153
	bl Func_02005ba4
	movs r0, #132
	movs r1, #1
	movs r2, #194
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #20
	movs r0, #17
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #26
	bl Func_02005c9c
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #27
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	movs r2, #192
	movs r3, #172
	lsls r2, r2, #13
	lsls r3, r3, #17
	ldr r1, .L_0200918c
	bl Object_SetPositionAndResetMotion
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005ba4
	movs r0, #132
	movs r1, #1
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	movs r0, #17
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #187
	lsls r2, r2, #1
	movs r0, #17
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r6, .L_02009190
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r6, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r5, .L_02009194
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	bl Func_020032e0
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #0
	movs r0, #17
	bl Func_02005b64
	movs r0, #11
	bl Func_02005c9c
	bl Func_02003610
	movs r0, #160
	bl Battle_WaitMode0
	movs r2, #80
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r1, #1
	movs r0, #1
	bl Func_02003640
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #80
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #161
	bl Func_02005c9c
	movs r1, #1
	movs r0, #0
	bl Func_02003640
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b9c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #20
	movs r0, #17
	bl Func_02005b5c
	adds r0, r6, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009198
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008f7c
	ldr r3, [r3, #12]
	movs r2, #144
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_02008f7c
.L_02008f52:
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_02009198
	ldr r1, .L_0200919c
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #144
	ldr r3, [r3, #12]
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_02008f52
.L_02008f7c:
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009188
	adds r1, #153
	bl Func_02005ba4
	movs r0, #132
	movs r1, #128
	movs r2, #210
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #17
	ldr r1, .L_020091a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #196
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #17
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #148
	bl Func_02005c9c
	movs r0, #11
	bl Func_02005c9c
	bl Func_020032b0
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #176
	movs r1, #128
	movs r2, #196
	movs r3, #1
	lsls r2, r2, #17
	lsls r1, r1, #14
	lsls r0, r0, #15
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_02005b9c
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_020091a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #193
	movs r0, #7
	movs r1, #152
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	movs r0, #7
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r2, #178
	movs r0, #7
	movs r1, #124
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #20
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	ldr r5, .L_02009184
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #184
	lsls r2, r2, #1
	movs r1, #104
	movs r0, #26
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02005ba4
	movs r0, #222
	movs r1, #1
	movs r2, #178
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #15
	bl Motion_CamBounds
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #26
	movs r1, #1
	bl Object_SetModeById
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_020091a4
	.2byte 0x0000
.L_02009184:
	.4byte gPartyState
.L_02009188:
	.4byte 0x0004cccc
.L_0200918c:
	.4byte 0x01090000
.L_02009190:
	.4byte Func_02002eb0
.L_02009194:
	.4byte Func_02002ccc
.L_02009198:
	.4byte Data_02007950
.L_0200919c:
	.4byte 0xffffc000
.L_020091a0:
	.4byte 0x00019999
.L_020091a4:
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #26
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020091f4
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #26
	bl Func_02005b64
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009216
.L_020091f4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #192
	adds r3, #1
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	bl Func_02005c7c
.L_02009216:
	pop {r5, r6, pc}
	.section .text.x02009218,"ax",%progbits
	.global Func_02001218
	.thumb_func
Func_02001218:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009230
	movs r0, #5
	bl Func_02005bc4
	b .L_0200930a
.L_02009230:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r5, .L_0200930c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r1, [r5]
	movs r0, #7
	bl Func_02005b1c
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #7
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl ObjectMotion_SetActionVariant
	movs r0, #1
	bl WaitFrames
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02009310
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #222
	lsls r2, r2, #1
	movs r0, #7
	movs r1, #62
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #7
	bl Func_02005b7c
	ldr r0, .L_02009314
	bl Func_02005b4c
	movs r1, #0
	movs r0, #7
	bl Func_02005b64
	movs r0, #7
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r2, #210
	strb r3, [r0]
	movs r1, #56
	movs r0, #7
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #172
	movs r0, #7
	movs r1, #40
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r2, #255
	movs r1, #80
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	bl Func_02005bc4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_ClearBit
	movs r0, #131
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #7
	bl Party_RemoveOwnerRestored
	bl Func_02005aa4
.L_0200930a:
	pop {r5, pc}
.L_0200930c:
	.4byte gPartyState
.L_02009310:
	.4byte 0x00019999
.L_02009314:
	.4byte 0x00001efa
	.section .text.x02009318,"ax",%progbits
	.global Func_02001318
	.thumb_func
Func_02001318:
	push {r5, lr}
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	ldr r0, .L_02009724
	bl Func_02005b4c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005bbc
	movs r1, #204
	movs r3, #0
	adds r0, #85
	lsls r1, r1, #7
	strb r3, [r0]
	adds r1, #102
	ldr r0, .L_02009728
	bl Func_02005ba4
	movs r0, #192
	movs r1, #1
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_0200972c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02005b9c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #7
	bl Func_02005b6c
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005ba4
	movs r0, #192
	movs r1, #1
	movs r2, #186
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #153
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009730
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_02009730
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009734
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r0, [r5]
	ldr r1, .L_02009738
	bl Object_SetActionCallbackAndRefreshById
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #224
	movs r2, #186
	movs r0, #26
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02005b0c
	movs r1, #224
	movs r2, #186
	movs r0, #5
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02005b0c
	movs r1, #224
	movs r2, #186
	lsls r2, r2, #17
	lsls r1, r1, #14
	movs r0, #6
	bl Func_02005b0c
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0200973c
	movs r0, #26
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009740
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009744
	movs r0, #6
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #6
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #26
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #6
	bl Func_02005b94
	movs r1, #208
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #26
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #6
	bl Func_02005b94
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #0
	bl Func_02005b7c
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	b .L_02009748
	.2byte 0x0000
.L_02009724:
	.4byte 0x00001eae
.L_02009728:
	.4byte 0x00033333
.L_0200972c:
	.4byte gPartyState
.L_02009730:
	.4byte 0x00013333
.L_02009734:
	.4byte Data_02005eec
.L_02009738:
	.4byte Data_02005e88
.L_0200973c:
	.4byte Data_02005f14
.L_02009740:
	.4byte Data_02005f58
.L_02009744:
	.4byte Data_02005f9c
.L_02009748:
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #6
	bl Func_02005b94
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	adds r0, #26
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200980e
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009834
.L_0200980e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	adds r3, #2
	movs r1, #3
	strh r3, [r2]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
.L_02009834:
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #26
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #176
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009cb8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #26
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #26
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #131
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02005b94
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_02005b94
	movs r1, #128
	movs r2, #20
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #20
	movs r0, #7
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #7
	bl Func_02005b94
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #176
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02005b7c
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #40
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #6
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02005b9c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #6
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #176
	movs r2, #10
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #0
	movs r0, #26
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r2, #20
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #26
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02009cbc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #184
	lsls r2, r2, #1
	b .L_02009cc0
	.2byte 0x0000
.L_02009cb8:
	.4byte gPartyState
.L_02009cbc:
	.4byte 0x00019999
.L_02009cc0:
	movs r0, #7
	movs r1, #72
	bl ObjectMotion_SetPositionAndReset
	movs r1, #129
	movs r0, #26
	lsls r1, r1, #1
	bl Func_02005b9c
	movs r2, #174
	movs r0, #7
	movs r1, #40
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #166
	movs r0, #7
	movs r1, #40
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #26
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #176
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_02005b94
	movs r1, #128
	movs r2, #40
	movs r0, #7
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #176
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b9c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	adds r0, #26
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009e9c
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #20
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #3
	strh r3, [r2]
	b .L_02009f10
.L_02009e9c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #8
	adds r3, #1
	strh r3, [r2]
	adds r1, #255
	movs r2, #20
	movs r0, #26
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r0, #6
	movs r1, #0
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02005b9c
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02005c7c
.L_02009f10:
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	ldr r5, .L_0200a048
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #144
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #7
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #175
	lsls r2, r2, #1
	movs r0, #7
	movs r1, #40
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #7
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200a04c
	movs r0, #5
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #26
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_SetBit
	movs r0, #7
	movs r1, #1
	bl Event_PrepareObjectAndApplyValue
	bl Func_02005a8c
	pop {r5, pc}
	.2byte 0x0000
.L_0200a048:
	.4byte gPartyState
.L_0200a04c:
	.4byte Data_02005fe0
	.section .text.x0200a050,"ax",%progbits
	.global Func_02002050
	.thumb_func
Func_02002050:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200a06a
	bl .L_0200aac8
.L_0200a06a:
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r5, .L_0200a470
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #172
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #208
	bl ObjectMotion_SetPositionAndReset
	movs r1, #154
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #200
	bl ObjectMotion_SetPositionAndReset
	movs r1, #145
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #188
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #7
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, [r5]
	movs r0, #5
	bl Func_02005b1c
	ldr r1, [r5]
	movs r0, #6
	bl Func_02005b1c
	ldr r1, [r5]
	movs r0, #26
	bl Func_02005b1c
	ldr r1, [r5]
	movs r0, #7
	bl Func_02005b1c
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0200a474
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a478
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a47c
	movs r0, #26
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a480
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	ldr r0, .L_0200a484
	bl Func_02005b4c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #152
	lsls r1, r1, #7
	adds r1, #204
	ldr r0, .L_0200a488
	bl Func_02005ba4
	bl Func_02005bbc
	adds r0, #85
	strb r6, [r0]
	movs r1, #1
	movs r0, #200
	movs r2, #180
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #174
	movs r1, #1
	movs r2, #224
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #134
	movs r1, #1
	movs r2, #240
	negs r1, r1
	lsls r2, r2, #15
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #136
	movs r1, #1
	movs r2, #180
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Func_02005b94
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #131
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	ldr r0, [r5]
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200a48c
	adds r1, #102
	bl Func_02005ba4
	movs r0, #134
	movs r1, #1
	movs r2, #146
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #26
	ldr r1, .L_0200a48c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #145
	movs r0, #26
	lsls r1, r1, #1
	movs r2, #116
	bl ObjectMotion_SetPositionAndReset
	movs r1, #134
	movs r0, #26
	lsls r1, r1, #1
	movs r2, #109
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #40
	movs r0, #26
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #134
	movs r1, #1
	movs r2, #188
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r0, #26
	movs r1, #218
	movs r2, #156
	bl ObjectMotion_SetPositionAndReset
	movs r2, #40
	movs r1, #0
	movs r0, #26
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #26
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #156
	movs r1, #248
	movs r0, #26
	bl ObjectMotion_SetPositionAndReset
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #26
	bl Object_LinkObjectAndSetCallback
	movs r1, #26
	movs r0, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #26
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #248
	movs r2, #188
	movs r0, #26
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #26
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #7
	movs r0, #26
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02005b94
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_02005b94
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #140
	movs r2, #188
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	b .L_0200a490
.L_0200a470:
	.4byte gPartyState
.L_0200a474:
	.4byte Data_02006014
.L_0200a478:
	.4byte Data_02006058
.L_0200a47c:
	.4byte Data_0200609c
.L_0200a480:
	.4byte Data_020060e0
.L_0200a484:
	.4byte 0x00001f00
.L_0200a488:
	.4byte 0x00026666
.L_0200a48c:
	.4byte 0x00013333
.L_0200a490:
	movs r1, #1
	movs r0, #26
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #26
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #238
	ands r6, r3
	movs r2, #188
	strb r6, [r0]
	movs r0, #26
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #26
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r2, r3
	strb r2, [r0]
	movs r1, #0
	movs r0, #26
	mov r8, r2
	bl UiText_OpenMessageAtObject
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_02005b94
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_0200a4fa:
	ldr r3, .L_0200a548
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a522
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a54c
.L_0200a522:
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	subs r3, #2
	strh r3, [r2]
	movs r1, #0
	bl UiText_OpenMessageAtObject
	b .L_0200a4fa
	.2byte 0x0000
.L_0200a548:
	.4byte gPartyState
.L_0200a54c:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_0200384c
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	ldr r5, .L_0200a960
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02005b94
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_02005b94
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02005b94
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Func_02005b9c
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r2, #20
	movs r0, #26
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	bl Func_02005b7c
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r1, [r5]
	movs r0, #5
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #6
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #26
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #134
	movs r1, #1
	movs r2, #164
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #140
	movs r2, #120
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Func_02005b7c
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_0200384c
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r2, #142
	ldr r0, [r5]
	movs r1, #232
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [r5]
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_SetBit
	bl Func_0200384c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	movs r1, #148
	movs r2, #142
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [r5]
	bl Func_02005b7c
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_0200384c
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #134
	movs r1, #1
	movs r2, #188
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #142
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #188
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #8
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #26
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Func_02005b94
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #208
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_02005b9c
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #26
	bl Func_02005b94
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #26
	movs r1, #0
	movs r2, #20
	bl Func_02005b5c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #20
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	bl Func_02005b64
	movs r2, #10
	movs r0, #26
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #134
	movs r1, #1
	movs r2, #164
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #148
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #140
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #154
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #40
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #26
	bl Func_02005b94
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #134
	movs r1, #1
	movs r2, #188
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #6
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #40
	movs r0, #26
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	bl Func_02005b7c
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #129
	movs r0, #26
	lsls r1, r1, #1
	bl Func_02005b9c
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #5
	b .L_0200a964
.L_0200a960:
	.4byte gPartyState
.L_0200a964:
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #20
	movs r0, #26
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r2, #10
	movs r0, #26
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #8
	movs r2, #0
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_02005b94
	movs r0, #26
	movs r1, #0
	bl Func_02005b7c
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02005b94
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02005b94
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02005b94
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_02005b94
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200aad0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200aad0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #26
	ldr r1, .L_0200aad0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_0200aad0
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200aad4
	movs r0, #5
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #6
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #26
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #7
	adds r1, r5, #0
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_SetBit
	bl Func_02005aa4
.L_0200aac8:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200aad0:
	.4byte 0x00013333
.L_0200aad4:
	.4byte Data_02006108
	.section .text.x0200aad8,"ax",%progbits
	.global Func_02002ad8
	.thumb_func
Func_02002ad8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #144
	ldr r5, [r3]
	ldr r3, .L_0200aaf4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	str r0, [r5, #24]
	pop {r5, pc}
.L_0200aaf4:
	.4byte gPartyState
	.section .text.x0200aaf8,"ax",%progbits
	.global Func_02002af8
	.thumb_func
Func_02002af8:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #144
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #24]
	bx lr
	.2byte 0x0000
	.section .text.x0200ab08,"ax",%progbits
	.global Func_02002b08
	.thumb_func
Func_02002b08:
	push {lr}
	cmp r0, #31
	ble .L_0200ab10
	movs r0, #31
.L_0200ab10:
	cmp r0, #0
	bge .L_0200ab16
	movs r0, #0
.L_0200ab16:
	pop {pc}
	.section .text.x0200ab18,"ax",%progbits
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r3, .L_0200ab70
	lsls r1, r1, #1
	mov r10, r0
	ldrsh r6, [r3, r1]
	movs r0, #248
	lsls r6, r6, #16
	lsls r0, r0, #13
	ands r0, r6
	ldr r3, .L_0200ab6c
	asrs r0, r0, #16
	adds r0, r0, r2
	lsrs r5, r6, #21
	lsls r0, r0, #16
	lsrs r6, r6, #26
	ands r5, r3
	ands r6, r3
	asrs r0, r0, #16
	asrs r3, r2, #1
	asrs r2, r2, #2
	adds r5, r5, r3
	adds r6, r6, r2
	bl Func_02002b08
	mov r8, r0
	mov r2, r8
	lsls r5, r5, #16
	asrs r5, r5, #16
	lsls r2, r2, #16
	asrs r2, r2, #16
	adds r0, r5, #0
	mov r8, r2
	bl Func_02002b08
	lsls r6, r6, #16
	asrs r6, r6, #16
	adds r5, r0, #0
	adds r0, r6, #0
	b .L_0200ab74
.L_0200ab6c:
	.4byte 0x0000001f
.L_0200ab70:
	.4byte Data_02007948
.L_0200ab74:
	bl Func_02002b08
	lsls r5, r5, #16
	mov r3, r10
	asrs r5, r5, #16
	lsls r0, r0, #16
	lsls r3, r3, #1
	lsls r5, r5, #5
	asrs r0, r0, #6
	orrs r0, r5
	mov r10, r3
	movs r2, #160
	mov r3, r8
	orrs r3, r0
	lsls r2, r2, #19
	add r10, r2
	mov r8, r3
	mov r0, r8
	mov r2, r10
	strh r0, [r2]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.section .text.x0200aba4,"ax",%progbits
	.global Func_02002ba4
	.thumb_func
Func_02002ba4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r3, .L_0200ac00
	lsls r1, r1, #1
	mov r10, r0
	ldrsh r6, [r3, r1]
	movs r0, #248
	lsls r6, r6, #16
	lsls r0, r0, #13
	ands r0, r6
	asrs r0, r0, #16
	ldr r3, .L_0200abfc
	adds r0, r0, r2
	adds r0, #4
	lsrs r5, r6, #21
	subs r2, #4
	lsrs r6, r6, #26
	lsls r0, r0, #16
	ands r5, r3
	ands r6, r3
	asrs r0, r0, #16
	asrs r3, r2, #1
	asrs r2, r2, #2
	adds r5, r5, r3
	adds r6, r6, r2
	bl Func_02002b08
	mov r8, r0
	mov r2, r8
	lsls r5, r5, #16
	asrs r5, r5, #16
	lsls r2, r2, #16
	asrs r2, r2, #16
	adds r0, r5, #0
	mov r8, r2
	bl Func_02002b08
	lsls r6, r6, #16
	asrs r6, r6, #16
	adds r5, r0, #0
	adds r0, r6, #0
	b .L_0200ac04
.L_0200abfc:
	.4byte 0x0000001f
.L_0200ac00:
	.4byte Data_02007948
.L_0200ac04:
	bl Func_02002b08
	lsls r5, r5, #16
	mov r3, r10
	asrs r5, r5, #16
	lsls r0, r0, #16
	lsls r3, r3, #1
	lsls r5, r5, #5
	asrs r0, r0, #6
	orrs r0, r5
	mov r10, r3
	movs r2, #160
	mov r3, r8
	orrs r3, r0
	lsls r2, r2, #19
	add r10, r2
	mov r8, r3
	mov r0, r8
	mov r2, r10
	strh r0, [r2]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.section .text.x0200ac34,"ax",%progbits
	.global Func_02002c34
	.thumb_func
Func_02002c34:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200acc4
	movs r1, #173
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200acc4
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200acc4
	ldr r3, .L_0200acc8
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200acc4
	bl Random16Far
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r2, r2, #1
	lsrs r2, r2, #16
	subs r2, #5
	movs r1, #0
	movs r0, #9
	bl Func_02002b18
	bl Random16Far
	adds r2, r0, #0
	lsls r2, r2, #3
	lsrs r2, r2, #16
	subs r2, #4
	movs r1, #1
	movs r0, #87
	bl Func_02002b18
	bl Random16Far
	adds r2, r0, #0
	lsls r2, r2, #2
	lsrs r2, r2, #16
	subs r2, #2
	movs r1, #2
	movs r0, #103
	bl Func_02002b18
	bl Random16Far
	adds r2, r0, #0
	lsls r2, r2, #2
	lsrs r2, r2, #16
	subs r2, #2
	movs r0, #119
	movs r1, #3
	bl Func_02002b18
.L_0200acc4:
	pop {pc}
	.2byte 0x0000
.L_0200acc8:
	.4byte Data_0300122c
	.section .text.x0200accc,"ax",%progbits
	.global Func_02002ccc
	.thumb_func
Func_02002ccc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200ad5e
	movs r1, #173
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200ad5e
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200ad5e
	ldr r3, .L_0200ad60
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200ad5e
	bl Random16Far
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r2, r2, #1
	lsrs r2, r2, #16
	subs r2, #5
	movs r1, #0
	movs r0, #9
	bl Func_02002b18
	bl Random16Far
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r2, r2, #1
	lsrs r2, r2, #16
	subs r2, #3
	movs r1, #1
	movs r0, #7
	bl Func_02002b18
	bl Random16Far
	adds r2, r0, #0
	lsls r2, r2, #2
	lsrs r2, r2, #16
	subs r2, #2
	movs r1, #2
	movs r0, #6
	bl Func_02002b18
	bl Random16Far
	adds r2, r0, #0
	lsls r2, r2, #3
	lsrs r2, r2, #16
	subs r2, #4
	movs r0, #76
	movs r1, #3
	bl Func_02002ba4
.L_0200ad5e:
	pop {pc}
.L_0200ad60:
	.4byte Data_0300122c
	.section .text.x0200ad64,"ax",%progbits
	.global Func_02002d64
	.thumb_func
Func_02002d64:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200adda
	movs r1, #173
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200adda
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200adda
	ldr r3, .L_0200addc
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200adda
	bl Random16Far
	adds r2, r0, #0
	lsls r2, r2, #2
	lsrs r2, r2, #16
	subs r2, #2
	movs r1, #0
	movs r0, #169
	bl Func_02002b18
	bl Random16Far
	adds r2, r0, #0
	lsls r2, r2, #1
	lsrs r2, r2, #16
	subs r2, #1
	movs r1, #1
	movs r0, #167
	bl Func_02002b18
	bl Random16Far
	adds r2, r0, #0
	lsrs r2, r2, #16
	movs r0, #166
	movs r1, #2
	bl Func_02002b18
.L_0200adda:
	pop {pc}
.L_0200addc:
	.4byte Data_0300122c
	.section .text.x0200ade0,"ax",%progbits
	.global Func_02002de0
	.thumb_func
Func_02002de0:
	push {r5, r6, r7, lr}
	movs r0, #234
	movs r2, #144
	movs r3, #172
	adds r0, #255
	ldr r1, .L_0200ae58
	lsls r2, r2, #14
	lsls r3, r3, #17
	ldr r5, .L_0200ae5c
	bl Func_02005a34
	movs r7, #0
	str r0, [r5]
	cmp r0, #0
	beq .L_0200ae56
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	adds r2, r0, #0
	adds r2, #92
	strb r7, [r3]
	movs r1, #193
	movs r3, #1
	strb r3, [r2]
	lsls r1, r1, #3
	strb r7, [r6, #26]
	strb r7, [r6, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #242
	bl Func_02005a84
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_0200ae56:
	pop {r5, r6, r7, pc}
.L_0200ae58:
	.4byte 0x01090000
.L_0200ae5c:
	.4byte Data_02007950
	.section .text.x0200ae60,"ax",%progbits
	.global Func_02002e60
	.thumb_func
Func_02002e60:
	push {lr}
	ldr r3, .L_0200aeac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #13
	cmp r3, r2
	ble .L_0200ae8e
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #30
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	b .L_0200aeaa
.L_0200ae8e:
	movs r0, #29
	bl Object_GetById
	movs r1, #240
	movs r3, #168
	lsls r1, r1, #15
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #30
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200aeaa:
	pop {pc}
.L_0200aeac:
	.4byte gPartyState
	.section .text.x0200aeb0,"ax",%progbits
	.global Func_02002eb0
	.thumb_func
Func_02002eb0:
	push {r5, lr}
	ldr r5, .L_0200aed8
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200aed4
	movs r0, #27
	bl Object_GetById
	ldr r4, [r5]
	ldr r2, [r0, #12]
	movs r3, #192
	lsls r3, r3, #12
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	adds r0, r4, #0
	bl Object_SetPositionAndResetMotion
.L_0200aed4:
	pop {r5, pc}
	.2byte 0x0000
.L_0200aed8:
	.4byte Data_02007950
	.section .text.x0200aedc,"ax",%progbits
	.global Func_02002edc
	.thumb_func
Func_02002edc:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200af0c
	adds r3, #15
.L_0200af0c:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200af34,"ax",%progbits
	.global Func_02002f34
	.thumb_func
Func_02002f34:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200afd4
	ldr r7, [r3]
	movs r3, #3
	ands r7, r3
	cmp r7, #0
	bne .L_0200afe8
	movs r0, #183
	movs r1, #132
	movs r2, #128
	movs r3, #158
	lsls r0, r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Func_02005a34
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200afe8
	bl Random16Far
	movs r3, #192
	lsls r0, r0, #13
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	str r0, [r6, #68]
	bl Random16Far
	movs r3, #128
	lsls r0, r0, #16
	lsrs r0, r0, #16
	lsls r3, r3, #11
	subs r3, r3, r0
	str r3, [r6, #76]
	bl Random16Far
	ldr r3, .L_0200afd8
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, .L_0200afd0
	adds r5, r6, #0
	adds r2, r6, #0
	adds r2, #85
	adds r5, #100
	strh r0, [r5]
	movs r1, #0
	strb r3, [r2]
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #2
	bl Func_02005a24
	ldr r1, .L_0200afdc
	adds r0, r6, #0
	bl Func_02005a2c
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, .L_0200afe0
	strh r7, [r5]
	str r7, [r6, #48]
	b .L_0200afe4
.L_0200afd0:
	.4byte 0x00000000
.L_0200afd4:
	.4byte Data_0300122c
.L_0200afd8:
	.4byte 0xffff8000
.L_0200afdc:
	.4byte Data_02005ca4
.L_0200afe0:
	.4byte Func_02002edc
.L_0200afe4:
	str r7, [r6, #52]
	str r3, [r6, #108]
.L_0200afe8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200afec,"ax",%progbits
	.global Func_02002fec
	.thumb_func
Func_02002fec:
	push {lr}
	adds r1, r0, #0
	adds r1, #100
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r2, #160
	ldr r3, [r0, #24]
	lsls r2, r2, #3
	adds r2, #30
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldrh r3, [r1]
	adds r3, #2
	strh r3, [r1]
	ldr r3, [r0, #104]
	subs r3, #1
	str r3, [r0, #104]
	cmp r3, #0
	bne .L_0200b02e
	bl Func_02005a3c
.L_0200b02e:
	pop {pc}
	.section .text.x0200b030,"ax",%progbits
	.global Func_02003030
	.thumb_func
Func_02003030:
	push {r5, r6, lr}
	ldr r3, .L_0200b0b0
	ldr r6, [r3]
	movs r3, #63
	ands r6, r3
	cmp r6, #0
	bne .L_0200b0ac
	movs r0, #30
	movs r1, #176
	movs r2, #128
	movs r3, #157
	adds r0, #255
	lsls r1, r1, #15
	lsls r2, r2, #13
	lsls r3, r3, #17
	bl Func_02005a34
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200b0ac
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #15
	strh r6, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #13
	ldr r1, [r5, #80]
	negs r3, r3
	ldrb r2, [r1, #9]
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, .L_0200b0b4
	adds r0, r5, #0
	movs r1, #5
	str r3, [r5, #108]
	bl Func_02005a24
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
.L_0200b0ac:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b0b0:
	.4byte Data_0300122c
.L_0200b0b4:
	.4byte Func_02002fec
	.section .text.x0200b0b8,"ax",%progbits
	.global Func_020030b8
	.thumb_func
Func_020030b8:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #100
	movs r1, #0
	ldrsh r7, [r5, r1]
	ldrh r3, [r5]
	cmp r7, #0
	beq .L_0200b0d0
	subs r3, #1
	strh r3, [r5]
	b .L_0200b0fe
.L_0200b0d0:
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	adds r3, #10
	strh r3, [r5]
	adds r2, r6, #0
	adds r2, #102
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #0
	beq .L_0200b0f6
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #24]
	strh r7, [r2]
	b .L_0200b0fe
.L_0200b0f6:
	ldr r3, .L_0200b100
	str r3, [r6, #24]
	movs r3, #1
	strh r3, [r2]
.L_0200b0fe:
	pop {r5, r6, r7, pc}
.L_0200b100:
	.4byte 0xfffe8000
	.section .text.x0200b104,"ax",%progbits
	.global Func_02003104
	.thumb_func
Func_02003104:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r0, #131
	adds r3, r2, #0
	lsls r0, r0, #1
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_02005a34
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200b17e
	adds r3, r6, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #28]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	adds r2, r6, #0
	adds r3, #10
	adds r2, #100
	strh r3, [r2]
	adds r3, r6, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_0200b180
	ldr r1, [r6, #80]
	str r3, [r6, #108]
	ldrb r2, [r1, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #5
	bl Func_02005a24
	adds r0, r6, #0
	movs r1, #1
	bl Animation_SetStateFlags
.L_0200b17e:
	pop {r5, r6, pc}
.L_0200b180:
	.4byte Func_020030b8
	.section .text.x0200b184,"ax",%progbits
	.global Func_02003184
	.thumb_func
Func_02003184:
	push {r5, lr}
	ldr r3, .L_0200b1c4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b1c8
	subs r2, #2
	movs r5, #220
	strh r3, [r2]
	lsls r5, r5, #17
	movs r1, #160
	movs r2, #225
	adds r0, r5, #0
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02003104
	movs r1, #184
	movs r2, #219
	adds r0, r5, #0
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02003104
	movs r1, #208
	movs r2, #213
	adds r0, r5, #0
	lsls r1, r1, #14
	lsls r2, r2, #17
	b .L_0200b1cc
	.2byte 0x0000
.L_0200b1c4:
	.4byte 0x00000c08
.L_0200b1c8:
	.4byte 0x00003f10
.L_0200b1cc:
	bl Func_02003104
	movs r1, #232
	movs r2, #207
	adds r0, r5, #0
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02003104
	movs r1, #128
	movs r2, #201
	lsls r1, r1, #15
	lsls r2, r2, #17
	adds r0, r5, #0
	bl Func_02003104
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b1f0,"ax",%progbits
	.global Func_020031f0
	.thumb_func
Func_020031f0:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02005b8c
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b220,"ax",%progbits
	.global Func_02003220
	.thumb_func
Func_02003220:
	push {lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b240
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b248
	bl Func_0200057c
	b .L_0200b248
.L_0200b240:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200b248:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b24c,"ax",%progbits
	.global Func_0200324c
	.thumb_func
Func_0200324c:
	push {r5, r6, r7, lr}
	movs r7, #0
.L_0200b250:
	adds r6, r7, #0
	adds r6, #15
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	adds r5, #85
	adds r0, r6, #0
	adds r7, #1
	movs r6, #0
	bl Func_020031f0
	strb r6, [r5]
	cmp r7, #1
	bls .L_0200b250
	movs r0, #15
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #12]
	movs r0, #16
	bl Object_GetById
	str r6, [r0, #12]
	bl Func_02003220
	pop {r5, r6, r7, pc}
	.section .text.x0200b288,"ax",%progbits
	.global Func_02003288
	.thumb_func
Func_02003288:
	push {r5, r6, r7, lr}
	movs r7, #0
.L_0200b28c:
	adds r6, r7, #0
	adds r6, #26
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_020031f0
	adds r5, #85
	movs r3, #0
	adds r7, #1
	strb r3, [r5]
	cmp r7, #1
	bls .L_0200b28c
	bl Func_02003220
	pop {r5, r6, r7, pc}
	.section .text.x0200b2b0,"ax",%progbits
	.global Func_020032b0
	.thumb_func
Func_020032b0:
	push {r5, lr}
	movs r5, #0
.L_0200b2b4:
	adds r0, r5, #0
	adds r0, #18
	movs r1, #5
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #7
	bls .L_0200b2b4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b2c8,"ax",%progbits
	.global Func_020032c8
	.thumb_func
Func_020032c8:
	push {r5, lr}
	movs r5, #0
.L_0200b2cc:
	adds r0, r5, #0
	adds r0, #18
	movs r1, #1
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #7
	bls .L_0200b2cc
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b2e0,"ax",%progbits
	.global Func_020032e0
	.thumb_func
Func_020032e0:
	push {lr}
	movs r0, #143
	movs r1, #1
	bl Func_02005c14
	movs r1, #27
	movs r0, #17
	bl Func_02005c1c
	bl Func_02005c34
	movs r0, #1
	bl Field_DispatchTypeHandler
	bl Func_02005c24
	bl Func_02005c2c
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b308,"ax",%progbits
	.global Func_02003308
	.thumb_func
Func_02003308:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r2, #99
	adds r2, r2, r6
	ldrb r3, [r2]
	sub sp, #12
	movs r1, #0
	mov r8, r2
	cmp r3, #0
	beq .L_0200b342
	adds r3, r6, #0
	adds r2, r6, #0
	adds r3, #100
	adds r2, #102
	movs r1, #0
	ldrsh r2, [r2, r1]
	movs r4, #0
	ldrsh r3, [r3, r4]
	adds r3, r3, r2
	lsls r0, r3, #3
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r1, r3, #1
.L_0200b342:
	ldr r3, [r6, #56]
	mov r5, sp
	str r3, [r5]
	adds r7, r6, #0
	ldr r3, [r6, #60]
	adds r7, #100
	str r3, [r5, #4]
	ldr r3, [r6, #64]
	str r3, [r5, #8]
	adds r3, r6, #0
	ldr r0, [r6, #76]
	adds r3, #102
	movs r4, #0
	ldrsh r2, [r3, r4]
	movs r4, #0
	ldrsh r3, [r7, r4]
	lsls r0, r0, #16
	adds r0, r0, r1
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #8
	adds r1, r1, r2
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	str r3, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	adds r5, r6, #0
	str r3, [r6, #16]
	adds r5, #98
	ldrb r3, [r5]
	cmp r3, #7
	bls .L_0200b38c
	b .L_0200b4f2
.L_0200b38c:
	ldr r2, .L_0200b4fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200b394:
	.4byte .L_0200b3b4
	.4byte .L_0200b3f8
	.4byte .L_0200b42c
	.4byte .L_0200b44a
	.4byte .L_0200b468
	.4byte .L_0200b482
	.4byte .L_0200b4b0
	.4byte .L_0200b4e4
.L_0200b3b4:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #4
	strh r3, [r7]
	str r0, [r6, #76]
	ldr r2, [r6, #80]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r2, #18]
	movs r1, #192
	ldr r3, [r6, #28]
	lsls r1, r1, #3
	ldr r2, [r6, #24]
	adds r1, #102
	adds r3, r3, r1
	str r3, [r6, #28]
	movs r3, #192
	adds r2, r2, r1
	lsls r3, r3, #9
	str r2, [r6, #24]
	cmp r2, r3
	bgt .L_0200b3e6
	b .L_0200b4f2
.L_0200b3e6:
	ldr r3, [r6, #80]
	ldrh r3, [r3, #18]
	cmp r3, #0
	beq .L_0200b3f0
	b .L_0200b4f2
.L_0200b3f0:
	mov r4, r8
	strb r0, [r5]
	strb r0, [r4]
	b .L_0200b4f2
.L_0200b3f8:
	ldrh r3, [r7]
	movs r1, #128
	adds r3, #4
	strh r3, [r7]
	lsls r1, r1, #5
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	ldr r3, [r6, #80]
	ldrh r3, [r3, #18]
	cmp r3, #0
	bne .L_0200b4f2
	mov r2, r8
	ldrb r3, [r2]
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_0200b4f2
	movs r0, #220
	bl Func_02005c9c
	movs r3, #2
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b42c:
	ldrh r3, [r7]
	movs r4, #128
	adds r3, #1
	strh r3, [r7]
	lsls r4, r4, #11
	ldr r3, [r6, #60]
	movs r1, #200
	adds r3, r3, r4
	lsls r1, r1, #15
	str r3, [r6, #60]
	cmp r3, r1
	ble .L_0200b4f2
	movs r3, #3
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b44a:
	ldrh r3, [r7]
	mov r2, r8
	adds r3, #1
	strh r3, [r7]
	ldr r3, [r6, #76]
	adds r3, #2
	str r3, [r6, #76]
	movs r3, #10
	strb r3, [r2]
	ldr r3, [r6, #76]
	cmp r3, #40
	ble .L_0200b4f2
	movs r3, #4
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b468:
	ldrh r3, [r7]
	mov r4, r8
	adds r3, #1
	strh r3, [r7]
	ldrb r3, [r4]
	adds r3, #255
	strb r3, [r4]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_0200b4f2
	movs r3, #5
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b482:
	ldrh r3, [r7]
	movs r1, #136
	adds r3, #1
	strh r3, [r7]
	lsls r1, r1, #16
	ldr r3, [r6, #76]
	subs r3, #2
	str r3, [r6, #76]
	ldr r3, [r6, #60]
	cmp r3, r1
	bge .L_0200b4a0
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r6, #60]
.L_0200b4a0:
	ldr r3, [r6, #76]
	cmp r3, #1
	bgt .L_0200b4f2
	movs r3, #0
	str r3, [r6, #76]
	movs r3, #6
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b4b0:
	ldrh r3, [r7]
	movs r4, #128
	adds r3, #4
	strh r3, [r7]
	lsls r4, r4, #5
	ldr r2, [r6, #80]
	ldr r1, .L_0200b500
	ldrh r3, [r2, #18]
	adds r3, r3, r4
	strh r3, [r2, #18]
	ldr r2, [r6, #24]
	ldr r3, [r6, #28]
	adds r2, r2, r1
	adds r3, r3, r1
	movs r1, #144
	lsls r1, r1, #5
	str r2, [r6, #24]
	str r3, [r6, #28]
	cmp r2, r1
	bge .L_0200b4f2
	movs r3, #100
	str r3, [r6, #56]
	str r3, [r6, #64]
	movs r3, #7
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b4e4:
	ldr r3, [r6, #80]
	ldrb r0, [r3, #16]
	bl Resource_ResetEntry
	adds r0, r6, #0
	bl Func_02005a3c
.L_0200b4f2:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b4fc:
	.4byte .L_0200b394
.L_0200b500:
	.4byte 0xfffff000
	.section .text.x0200b504,"ax",%progbits
	.global Func_02003504
	.thumb_func
Func_02003504:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #17
	bl Object_GetById
	mov r8, r0
	movs r0, #0
	mov r11, r0
	movs r0, #154
	bl Func_02005c9c
	movs r2, #0
	mov r10, r2
.L_0200b528:
	movs r0, #17
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r3, #1
	add r10, r3
	mov r0, r10
	cmp r0, #7
	bls .L_0200b528
	movs r0, #209
	bl Func_02005c9c
	movs r2, #0
	mov r10, r2
	mov r9, r2
.L_0200b562:
	mov r3, r8
	ldr r2, [r3, #12]
	movs r0, #192
	lsls r0, r0, #13
	adds r2, r2, r0
	movs r0, #209
	lsls r0, r0, #1
	ldr r1, [r3, #8]
	adds r0, #255
	ldr r3, [r3, #16]
	bl Func_02005a34
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b5f4
	mov r1, r11
	ldr r0, [r7, #80]
	bl Func_02005c44
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	ldr r1, [r7, #80]
	mov r11, r0
	ldrb r3, [r1, #9]
	movs r0, #13
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005a24
	adds r3, r7, #0
	adds r3, #100
	movs r1, #180
	strh r5, [r3]
	lsls r1, r1, #1
	str r5, [r7, #76]
	mov r0, r9
	bl IwramUnsignedDivideEntry
	ldr r6, .L_0200b5ec
	adds r3, r7, #0
	adds r3, #102
	strh r0, [r3]
	subs r3, #4
	strb r6, [r3]
	mov r2, r8
	ldr r3, [r2, #8]
	movs r0, #192
	str r3, [r7, #56]
	lsls r0, r0, #13
	ldr r3, [r2, #12]
	adds r3, r3, r0
	str r3, [r7, #60]
	ldr r3, [r2, #16]
	str r3, [r7, #64]
	ldr r3, .L_0200b5f0
	str r3, [r7, #108]
	b .L_0200b5f4
.L_0200b5ec:
	.4byte 0x00000000
.L_0200b5f0:
	.4byte Func_02003308
.L_0200b5f4:
	movs r3, #1
	movs r2, #176
	add r10, r3
	lsls r2, r2, #13
	mov r0, r10
	add r9, r2
	cmp r0, #15
	bls .L_0200b562
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200b610,"ax",%progbits
	.global Func_02003610
	.thumb_func
Func_02003610:
	push {lr}
	ldr r0, .L_0200b638
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	bl Func_02005bb4
	movs r0, #40
	bl Battle_WaitMode0
	ldr r3, .L_0200b63c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200b634
	bl Func_02003504
.L_0200b634:
	pop {pc}
	.2byte 0x0000
.L_0200b638:
	.4byte Func_02002ccc
.L_0200b63c:
	.4byte Data_02007950
	.section .text.x0200b640,"ax",%progbits
	.global Func_02003640
	.thumb_func
Func_02003640:
	push {r5, lr}
	sub sp, #8
	adds r5, r1, #0
	cmp r0, #0
	beq .L_0200b6ac
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #37
	movs r2, #79
	movs r3, #15
	movs r0, #49
	bl Func_02005a64
	movs r0, #12
	bl Object_GetById
	movs r1, #253
	movs r2, #128
	lsls r1, r1, #16
	lsls r2, r2, #14
	ldr r3, .L_0200b6d8
	bl Object_SetPositionAndResetMotion
	movs r0, #13
	bl Object_GetById
	movs r2, #128
	ldr r1, .L_0200b6dc
	lsls r2, r2, #14
	ldr r3, .L_0200b6d8
	bl Object_SetPositionAndResetMotion
	cmp r5, #0
	beq .L_0200b69a
	movs r0, #12
	movs r1, #0
	bl Object_SetModeById
	movs r0, #13
	movs r1, #0
	bl Object_SetModeById
	b .L_0200b6d4
.L_0200b69a:
	movs r0, #12
	movs r1, #1
	bl Object_SetModeById
	movs r0, #13
	movs r1, #1
	bl Object_SetModeById
	b .L_0200b6d4
.L_0200b6ac:
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #37
	movs r2, #79
	movs r3, #15
	bl Func_02005a64
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
.L_0200b6d4:
	add sp, #8
	pop {r5, pc}
.L_0200b6d8:
	.4byte 0x01210000
.L_0200b6dc:
	.4byte 0x01130000
	.section .text.x0200b6e0,"ax",%progbits
	.global Func_020036e0
	.thumb_func
Func_020036e0:
	push {r5, lr}
	sub sp, #8
	adds r5, r1, #0
	cmp r0, #0
	beq .L_0200b7a6
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #41
	movs r1, #38
	movs r2, #79
	movs r3, #16
	bl Func_02005a64
	movs r3, #16
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #21
	movs r2, #1
	movs r3, #1
	bl Func_02005a6c
	cmp r5, #0
	beq .L_0200b72a
	movs r0, #14
	bl Object_GetById
	movs r1, #132
	movs r2, #128
	movs r3, #154
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
.L_0200b72a:
	cmp r5, #6
	bhi .L_0200b7d6
	ldr r2, .L_0200b7dc
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200b738:
	.4byte .L_0200b754
	.4byte .L_0200b760
	.4byte .L_0200b76a
	.4byte .L_0200b774
	.4byte .L_0200b77e
	.4byte .L_0200b788
	.4byte .L_0200b78e
.L_0200b754:
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	b .L_0200b7d6
.L_0200b760:
	movs r0, #14
	movs r1, #0
	bl Object_SetModeById
	b .L_0200b7d6
.L_0200b76a:
	movs r0, #14
	movs r1, #1
	bl Object_SetModeById
	b .L_0200b7d6
.L_0200b774:
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	b .L_0200b7d6
.L_0200b77e:
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	b .L_0200b7d6
.L_0200b788:
	movs r0, #14
	movs r1, #6
	b .L_0200b792
.L_0200b78e:
	movs r0, #14
	movs r1, #7
.L_0200b792:
	bl Object_SetModeById
	movs r0, #14
	bl Object_GetById
	ldr r2, .L_0200b7e0
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	b .L_0200b7d6
.L_0200b7a6:
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #45
	movs r1, #38
	movs r2, #79
	movs r3, #16
	bl Func_02005a64
	movs r3, #16
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #22
	movs r2, #1
	movs r3, #1
	bl Func_02005a6c
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
.L_0200b7d6:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200b7dc:
	.4byte .L_0200b738
.L_0200b7e0:
	.4byte 0xffde0000
	.section .text.x0200b7e4,"ax",%progbits
	.global Func_020037e4
	.thumb_func
Func_020037e4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #196
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200b84a
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b810
	bl Func_02000624
	b .L_0200b84a
.L_0200b810:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b824
	bl Func_02000668
	b .L_0200b84a
.L_0200b824:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b838
	bl Func_020006ac
	b .L_0200b84a
.L_0200b838:
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b84a
	bl Func_020006f0
.L_0200b84a:
	pop {pc}
	.section .text.x0200b84c,"ax",%progbits
	.global Func_0200384c
	.thumb_func
Func_0200384c:
	push {r5, lr}
	movs r0, #136
	movs r1, #1
	bl Func_02005c14
	ldr r5, .L_0200b890
	movs r1, #144
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200b894
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #1
	ldr r0, [r3]
	negs r1, r1
	bl Func_02005c1c
	bl Func_02005c34
	movs r0, #1
	bl Field_DispatchTypeHandler
	bl Func_02005c24
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	bl Func_02005c2c
	pop {r5, pc}
	.2byte 0x0000
.L_0200b890:
	.4byte Func_020037e4
.L_0200b894:
	.4byte gPartyState
	.section .text.x0200b898,"ax",%progbits
	.global Func_02003898
	.thumb_func
Func_02003898:
	push {lr}
	ldr r3, .L_0200b8d0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b8d4
	cmp r2, r3
	bne .L_0200b8b4
	ldr r0, .L_0200b8d8
	bl Scheduler_RemoveCallbackFar
	b .L_0200b8ce
.L_0200b8b4:
	ldr r3, .L_0200b8dc
	cmp r2, r3
	bne .L_0200b8c2
	ldr r0, .L_0200b8e0
	bl Scheduler_RemoveCallbackFar
	b .L_0200b8ce
.L_0200b8c2:
	ldr r3, .L_0200b8e4
	cmp r2, r3
	bne .L_0200b8ce
	ldr r0, .L_0200b8e8
	bl Scheduler_RemoveCallbackFar
.L_0200b8ce:
	pop {pc}
.L_0200b8d0:
	.4byte gPartyState
.L_0200b8d4:
	.4byte 0x00000071
.L_0200b8d8:
	.4byte Func_02002c34
.L_0200b8dc:
	.4byte 0x00000072
.L_0200b8e0:
	.4byte Func_02002ccc
.L_0200b8e4:
	.4byte 0x0000007d
.L_0200b8e8:
	.4byte Func_02002d64
	.section .text.x0200b8ec,"ax",%progbits
	.global Func_020038ec
	.thumb_func
Func_020038ec:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b910
	bl Func_02003898
.L_0200b910:
	movs r0, #123
	bl Func_02005c9c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02005bc4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b930,"ax",%progbits
	.global Func_02003930
	.thumb_func
Func_02003930:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Func_020038ec
	pop {pc}
	.section .text.x0200b948,"ax",%progbits
	.global Func_02003948
	.thumb_func
Func_02003948:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200ba00
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	ldr r0, [r6]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r6]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0200b99e
	adds r3, #15
.L_0200b99e:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	adds r1, r1, r0
	ldr r3, [r5, #16]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02005a54
	adds r0, r5, #0
	bl Func_02005a5c
	movs r3, #192
	movs r0, #128
	lsls r3, r3, #8
	lsls r0, r0, #2
	strh r3, [r5, #6]
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b9d4
	bl Func_02003898
.L_0200b9d4:
	ldr r1, .L_0200ba04
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_02005c9c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02005bc4
	pop {r5, r6, r7, pc}
.L_0200ba00:
	.4byte gPartyState
.L_0200ba04:
	.4byte Data_02005cd0
	.section .text.x0200ba08,"ax",%progbits
	.global Func_02003a08
	.thumb_func
Func_02003a08:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r5, .L_0200bac0
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #133
	ldr r3, [r3, #108]
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r6, r0, #0
	ldr r0, [r5]
	mov r8, r1
	mov r9, r3
	bl Object_GetById
	mov r10, r0
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	mov r2, r10
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	mov r2, r8
	ldr r0, [r5]
	adds r1, r6, #0
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02005b7c
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	movs r1, #13
	bl Object_SetModeById
	lsls r6, r6, #16
	mov r3, r8
	lsls r3, r3, #16
	ldr r2, .L_0200bac4
	adds r1, r6, #0
	mov r0, r10
	mov r8, r3
	bl Func_02005a54
	mov r0, r10
	bl Func_02005a5c
	movs r1, #10
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #123
	bl Func_02005c9c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	add r9, r2
	mov r2, r9
	movs r3, #0
	ldrsh r0, [r2, r3]
	bl Func_02005bc4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200bac0:
	.4byte gPartyState
.L_0200bac4:
	.4byte 0xfff40000
	.section .text.x0200bac8,"ax",%progbits
	.global Func_02003ac8
	.thumb_func
Func_02003ac8:
	push {lr}
	ldr r3, .L_0200bb18
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r4, #42
	bne .L_0200baf4
	cmp r1, #23
	bne .L_0200baf4
	ldr r3, .L_0200bb1c
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200bb14
.L_0200baf4:
	cmp r4, #43
	bne .L_0200bb08
	cmp r1, #22
	bne .L_0200bb08
	ldr r3, .L_0200bb1c
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200bb14
.L_0200bb08:
	movs r0, #170
	movs r1, #176
	lsls r0, r0, #2
	lsls r1, r1, #1
	bl Func_02003a08
.L_0200bb14:
	pop {pc}
	.2byte 0x0000
.L_0200bb18:
	.4byte gPartyState
.L_0200bb1c:
	.4byte gInput
	.section .text.x0200bb20,"ax",%progbits
	.global Func_02003b20
	.thumb_func
Func_02003b20:
	push {lr}
	ldr r3, .L_0200bb80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r1, r3, #20
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #38
	bne .L_0200bb5e
	cmp r1, #23
	bne .L_0200bb4c
	ldr r3, .L_0200bb84
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200bb7e
.L_0200bb4c:
	cmp r1, #25
	bne .L_0200bb72
	ldr r3, .L_0200bb84
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200bb72
	b .L_0200bb7e
.L_0200bb5e:
	cmp r1, #24
	bne .L_0200bb72
	cmp r3, #39
	bne .L_0200bb72
	ldr r3, .L_0200bb84
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200bb7e
.L_0200bb72:
	movs r0, #196
	movs r1, #152
	lsls r0, r0, #1
	lsls r1, r1, #2
	bl Func_02003a08
.L_0200bb7e:
	pop {pc}
.L_0200bb80:
	.4byte gPartyState
.L_0200bb84:
	.4byte gInput
	.section .text.x0200bb88,"ax",%progbits
	.global Func_02003b88
	.thumb_func
Func_02003b88:
	push {lr}
	ldr r3, .L_0200bbe8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r1, r3, #20
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #54
	bne .L_0200bbc6
	cmp r1, #42
	bne .L_0200bbb4
	ldr r3, .L_0200bbec
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200bbe6
.L_0200bbb4:
	cmp r1, #44
	bne .L_0200bbda
	ldr r3, .L_0200bbec
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200bbda
	b .L_0200bbe6
.L_0200bbc6:
	cmp r1, #43
	bne .L_0200bbda
	cmp r3, #55
	bne .L_0200bbda
	ldr r3, .L_0200bbec
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200bbe6
.L_0200bbda:
	movs r0, #174
	movs r1, #216
	lsls r0, r0, #2
	lsls r1, r1, #2
	bl Func_02003a08
.L_0200bbe6:
	pop {pc}
.L_0200bbe8:
	.4byte gPartyState
.L_0200bbec:
	.4byte gInput
	.section .text.x0200bbf0,"ax",%progbits
	.global Func_02003bf0
	.thumb_func
Func_02003bf0:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc04
	bl Func_02003898
.L_0200bc04:
	movs r0, #188
	movs r1, #144
	lsls r0, r0, #1
	lsls r1, r1, #1
	bl Func_02003a08
	pop {pc}
	.2byte 0x0000
	.section .text.x0200bc14,"ax",%progbits
	.global Func_02003c14
	.thumb_func
Func_02003c14:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200bc7c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r5, #0
	b .L_0200bc3e
.L_0200bc3c:
	adds r5, #1
.L_0200bc3e:
	cmp r5, #159
	bhi .L_0200bc54
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, .L_0200bc80
	movs r2, #11
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200bc3c
.L_0200bc54:
	ldr r0, .L_0200bc7c
	bl Scheduler_RemoveCallbackFar
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #218
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #32
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #11
	bl Func_02005bc4
	pop {r5, pc}
	.2byte 0x0000
.L_0200bc7c:
	.4byte Func_02002d64
.L_0200bc80:
	.4byte gInput
	.section .text.x0200bc84,"ax",%progbits
	.global Func_02003c84
	.thumb_func
Func_02003c84:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #212
	bl Func_02005c9c
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02005bd4
	movs r0, #1
	bl Func_02005be4
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005bd4
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	mov r10, r2
	movs r2, #218
	lsls r2, r2, #1
	movs r6, #1
	str r6, [r3, r2]
	mov r8, r2
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r5, .L_0200bde8
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #60
	bl WaitFrames
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	movs r0, #212
	bl Func_02005c9c
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02005bd4
	movs r0, #1
	bl Func_02005be4
	movs r0, #1
	bl WaitFrames
	movs r0, #146
	movs r1, #1
	movs r2, #248
	lsls r2, r2, #16
	movs r3, #0
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005a4c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005bd4
	movs r0, #1
	bl Func_02005be4
	movs r0, #1
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #60
	bl WaitFrames
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	movs r0, #212
	bl Func_02005c9c
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02005bd4
	movs r0, #1
	bl Func_02005be4
	movs r0, #1
	bl WaitFrames
	movs r0, #248
	movs r1, #1
	movs r2, #134
	lsls r2, r2, #18
	movs r3, #0
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02005a4c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005bd4
	movs r0, #1
	bl Func_02005be4
	movs r0, #1
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #180
	bl WaitFrames
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	mov r2, r10
	ldr r3, [r2, #108]
	mov r2, r8
	str r6, [r3, r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	bl Func_02005bc4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0200bde8:
	.4byte Func_02002d64
	.section .text.x0200bdec,"ax",%progbits
	.global Func_02003dec
	.thumb_func
Func_02003dec:
	push {r5, r6, lr}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005bbc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r0, .L_0200c0b8
	bl Func_02005b4c
	movs r1, #0
	movs r0, #17
	bl Func_02005b64
	movs r0, #78
	bl Func_02005c9c
	ldr r6, .L_0200c0bc
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r6, r6, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #200
	ldr r0, [r6]
	movs r1, #86
	adds r2, #255
	bl ObjectMotion_SetPositionAndReset
	movs r1, #172
	lsls r1, r1, #15
	ldr r2, .L_0200c0c0
	movs r0, #26
	bl Func_02005b0c
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #200
	movs r0, #26
	movs r1, #72
	adds r2, #255
	bl ObjectMotion_SetPositionAndReset
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200c0c4
	adds r1, #204
	bl Func_02005ba4
	movs r0, #132
	movs r1, #128
	movs r2, #210
	lsls r2, r2, #17
	movs r3, #1
	lsls r1, r1, #13
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005ba4
	movs r0, #132
	movs r1, #128
	movs r2, #160
	movs r3, #1
	lsls r1, r1, #14
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #80
	bl Battle_WaitMode0
	movs r2, #40
	movs r0, #17
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c0c8
	adds r1, #153
	bl Func_02005ba4
	movs r0, #132
	movs r1, #128
	movs r2, #200
	movs r3, #1
	lsls r1, r1, #14
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #20
	movs r0, #17
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #11
	bl Func_02005c9c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_SetBit
	movs r0, #148
	bl Func_02005c9c
	movs r0, #18
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #11
	str r5, [r0, #40]
	movs r0, #5
	bl WaitFrames
	movs r0, #19
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl WaitFrames
	movs r0, #20
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl WaitFrames
	movs r0, #21
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl WaitFrames
	movs r0, #22
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl WaitFrames
	movs r0, #23
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl WaitFrames
	movs r0, #24
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl WaitFrames
	movs r0, #25
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #20
	bl WaitFrames
	movs r1, #208
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #17
	bl ObjectMotion_ArmCallback
	bl Func_020032b0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #6
	lsls r1, r1, #3
	adds r0, #51
	adds r1, #102
	bl Func_02005ba4
	movs r0, #132
	movs r2, #200
	lsls r2, r2, #17
	movs r3, #1
	movs r1, #0
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #200
	bl Battle_WaitMode0
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c0c8
	adds r1, #153
	bl Func_02005ba4
	movs r0, #222
	movs r1, #128
	movs r2, #228
	movs r3, #1
	lsls r0, r0, #15
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r2, #0
	movs r0, #26
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #20
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r6]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200c09a
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #26
	bl ObjectMotion_ResetAndSetPosition
.L_0200c09a:
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_SetBit
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c0b8:
	.4byte 0x00001e65
.L_0200c0bc:
	.4byte gPartyState
.L_0200c0c0:
	.4byte 0x01c70000
.L_0200c0c4:
	.4byte 0x00026666
.L_0200c0c8:
	.4byte 0x0004cccc
	.section .text.x0200c0cc,"ax",%progbits
	.global Func_020040cc
	.thumb_func
Func_020040cc:
	push {r5, r6, lr}
	ldr r5, .L_0200c280
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005bbc
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r5, .L_0200c284
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #78
	bl Func_02005c9c
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02005ba4
	movs r0, #132
	movs r1, #1
	movs r2, #194
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	ldr r0, .L_0200c288
	bl Func_02005b4c
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #26
	bl Func_02005c9c
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #27
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #27
	bl Object_GetById
	movs r2, #192
	movs r3, #172
	lsls r2, r2, #13
	lsls r3, r3, #17
	ldr r1, .L_0200c28c
	bl Object_SetPositionAndResetMotion
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005ba4
	movs r0, #132
	movs r1, #1
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	movs r0, #17
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #187
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #17
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c290
	bl Scheduler_AddOrUpdateCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	bl Func_020032e0
	movs r1, #144
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	movs r0, #17
	bl Func_02005b64
	movs r0, #11
	bl Func_02005c9c
	bl Func_02003610
	movs r0, #160
	bl Battle_WaitMode0
	movs r2, #80
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r1, #1
	movs r0, #1
	bl Func_02003640
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #40
	movs r0, #17
	bl Func_02005b5c
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #86
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	bl Func_02005bc4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c280:
	.4byte gPartyState
.L_0200c284:
	.4byte Func_02002ccc
.L_0200c288:
	.4byte 0x00001f27
.L_0200c28c:
	.4byte 0x01090000
.L_0200c290:
	.4byte Func_02002eb0
	.section .text.x0200c294,"ax",%progbits
	.global Func_02004294
	.thumb_func
Func_02004294:
	push {r5, r6, lr}
	ldr r5, .L_0200c40c
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005bbc
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #27
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #27
	bl Object_GetById
	movs r2, #192
	movs r3, #172
	lsls r2, r2, #13
	lsls r3, r3, #17
	ldr r1, .L_0200c410
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0200c414
	ldr r2, [r5]
	cmp r2, #0
	beq .L_0200c32a
	ldr r3, [r2, #12]
	movs r1, #128
	lsls r1, r1, #14
	adds r3, r3, r1
	str r3, [r2, #12]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
.L_0200c32a:
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c418
	bl Scheduler_AddOrUpdateCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #161
	bl Func_02005c9c
	movs r1, #1
	movs r0, #0
	bl Func_02003640
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b9c
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_0200c41c
	bl Func_02005b4c
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200c3ac
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	b .L_0200c3a2
.L_0200c382:
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_0200c414
	ldr r1, .L_0200c420
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl WaitFrames
	ldr r3, [r5]
.L_0200c3a2:
	movs r2, #144
	ldr r3, [r3, #12]
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_0200c382
.L_0200c3ac:
	movs r0, #17
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #255
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #11
	bl Func_02005c9c
	movs r0, #80
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #129
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #11
	bl Func_02005bc4
	pop {r5, r6, pc}
.L_0200c40c:
	.4byte gPartyState
.L_0200c410:
	.4byte 0x01090000
.L_0200c414:
	.4byte Data_02007950
.L_0200c418:
	.4byte Func_02002ccc
.L_0200c41c:
	.4byte 0x00001f2b
.L_0200c420:
	.4byte 0xffffc000
	.section .text.x0200c424,"ax",%progbits
	.global Func_02004424
	.thumb_func
Func_02004424:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	ldr r5, .L_0200c518
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Func_020032b0
	ldr r5, .L_0200c51c
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r3, #192
	lsls r3, r3, #18
	mov r10, r3
	movs r6, #129
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r6, r6, #1
	lsls r2, r2, #1
	adds r6, #255
	str r6, [r3, r2]
	mov r8, r2
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	ldr r0, .L_0200c520
	bl Func_02005b4c
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #160
	movs r0, #17
	lsls r1, r1, #7
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005bdc
	movs r1, #0
	movs r0, #0
	bl Func_02005bd4
	mov r2, r10
	ldr r3, [r2, #108]
	mov r2, r8
	str r6, [r3, r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #252
	bl GameFlag_SetBit
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #12
	bl Func_02005bc4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c518:
	.4byte gPartyState
.L_0200c51c:
	.4byte Func_02002ccc
.L_0200c520:
	.4byte 0x00001f45
	.section .text.x0200c524,"ax",%progbits
	.global Func_02004524
	.thumb_func
Func_02004524:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005bbc
	movs r3, #0
	adds r0, #85
	ldr r5, .L_0200c5c4
	strb r3, [r0]
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c5c8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	ldr r5, .L_0200c5cc
	ldr r2, [r5]
	cmp r2, #0
	beq .L_0200c59e
	ldr r3, [r2, #12]
	movs r1, #128
	lsls r1, r1, #14
	adds r3, r3, r1
	str r3, [r2, #12]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
.L_0200c59e:
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	pop {r5, pc}
.L_0200c5c4:
	.4byte gPartyState
.L_0200c5c8:
	.4byte Func_02002ccc
.L_0200c5cc:
	.4byte Data_02007950
	.section .text.x0200c5d0,"ax",%progbits
	.global Func_020045d0
	.thumb_func
Func_020045d0:
	push {r5, lr}
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c7dc
	adds r1, #153
	bl Func_02005ba4
	movs r0, #132
	movs r2, #200
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005bb4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #19
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #20
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #21
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #22
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #23
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #24
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #40
	movs r0, #25
	movs r1, #6
	bl ObjectMotion_Launch
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	bl Func_02005b7c
	movs r2, #0
	movs r0, #20
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #20
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r2, #0
	movs r0, #23
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005b64
	ldr r5, .L_0200c7e0
	movs r0, #18
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #19
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #20
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #21
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #22
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #23
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #24
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #25
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #17
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #17
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #17
	bl Func_02005b64
	movs r0, #18
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #19
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #20
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #21
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #22
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #23
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #24
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #25
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	movs r2, #0
	adds r1, #255
	movs r0, #17
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r0, #17
	movs r1, #4
	movs r2, #10
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	pop {r5, pc}
.L_0200c7dc:
	.4byte 0x0004cccc
.L_0200c7e0:
	.4byte Data_020061a8
	.section .text.x0200c7e4,"ax",%progbits
	.global Func_020047e4
	.thumb_func
Func_020047e4:
	push {r5, lr}
	bl Func_02004524
	movs r0, #161
	bl Func_02005c9c
	movs r1, #1
	movs r0, #0
	bl Func_02003640
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_0200c890
	bl Func_02005b4c
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	ldr r3, .L_0200c894
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200c850
	adds r1, r2, #0
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	movs r1, #144
	ldr r3, [r2, #12]
	lsls r1, r1, #14
	cmp r3, r1
	ble .L_0200c850
.L_0200c826:
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_0200c894
	ldr r1, .L_0200c898
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #144
	ldr r3, [r3, #12]
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_0200c826
.L_0200c850:
	movs r0, #17
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #252
	bl GameFlag_SetBit
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #12
	bl Func_02005bc4
	pop {r5, pc}
.L_0200c890:
	.4byte 0x00001f49
.L_0200c894:
	.4byte Data_02007950
.L_0200c898:
	.4byte 0xffffc000
	.section .text.x0200c89c,"ax",%progbits
	.global Func_0200489c
	.thumb_func
Func_0200489c:
	push {r5, lr}
	bl Func_02004524
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r0, #12
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #8
	bl WaitFrames
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #8
	bl WaitFrames
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b94
	ldr r0, .L_0200c96c
	bl Func_02005b4c
	movs r1, #0
	movs r0, #17
	bl Func_02005b64
	movs r0, #161
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005c9c
	ldr r5, .L_0200c970
	movs r0, #12
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #13
	bl Object_SetActionCallbackAndRefreshById
	ldr r5, .L_0200c974
	movs r0, #12
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #13
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl Object_GetById
	movs r1, #12
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #12
	bl Object_SetPartAttribute
	movs r0, #120
	bl Battle_WaitMode0
	bl Func_020045d0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #252
	bl GameFlag_SetBit
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #12
	bl Func_02005bc4
	pop {r5, pc}
	.2byte 0x0000
.L_0200c96c:
	.4byte 0x00001f4c
.L_0200c970:
	.4byte Data_0200614c
.L_0200c974:
	.4byte Data_0200613c
	.section .text.x0200c978,"ax",%progbits
	.global Func_02004978
	.thumb_func
Func_02004978:
	push {r5, lr}
	bl Func_02004524
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r0, #12
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #8
	bl WaitFrames
	movs r0, #12
	bl Object_GetById
	ldr r5, .L_0200ca1c
	str r5, [r0, #108]
	movs r0, #13
	bl Object_GetById
	movs r1, #128
	str r5, [r0, #108]
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #17
	bl Func_02005b94
	ldr r0, .L_0200ca20
	bl Func_02005b4c
	movs r2, #20
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r1, #0
	movs r0, #1
	bl Func_020036e0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #162
	bl Func_02005c9c
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ca24
	bl Scheduler_AddOrUpdateCallback
	movs r0, #120
	bl Battle_WaitMode0
	bl Func_020045d0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #252
	bl GameFlag_SetBit
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #12
	bl Func_02005bc4
	pop {r5, pc}
.L_0200ca1c:
	.4byte Func_020000a0
.L_0200ca20:
	.4byte 0x00001f54
.L_0200ca24:
	.4byte Func_02002f34
	.section .text.x0200ca28,"ax",%progbits
	.global Func_02004a28
	.thumb_func
Func_02004a28:
	push {r5, r6, r7, lr}
	movs r0, #14
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004524
	movs r1, #1
	movs r0, #17
	bl ObjectMotion_SetActionVariant
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r0, #12
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #8
	bl WaitFrames
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #8
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #17
	bl Func_02005b94
	ldr r0, .L_0200cce4
	bl Func_02005b4c
	movs r2, #20
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r1, #0
	movs r0, #1
	bl Func_020036e0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #233
	bl Func_02005c9c
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r6, #28]
	movs r0, #14
	bl Object_GetById
	movs r1, #132
	movs r2, #192
	movs r3, #154
	lsls r1, r1, #17
	lsls r2, r2, #13
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r5, #0
.L_0200cae8:
	ldr r3, [r6, #28]
	movs r1, #128
	lsls r1, r1, #3
	adds r3, r3, r1
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #63
	bls .L_0200cae8
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02005c9c
	ldr r3, .L_0200cce8
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200cb48
	adds r1, r2, #0
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	movs r1, #144
	ldr r3, [r2, #12]
	lsls r1, r1, #14
	cmp r3, r1
	ble .L_0200cb48
.L_0200cb2a:
	ldr r5, .L_0200cce8
	ldr r1, .L_0200ccec
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #144
	ldr r3, [r3, #12]
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_0200cb2a
.L_0200cb48:
	movs r0, #232
	bl Func_02005c9c
	ldr r3, .L_0200cce8
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200cbb2
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r5, #0
.L_0200cb5e:
	ldr r7, .L_0200cce8
	movs r1, #192
	ldr r2, [r7]
	lsls r1, r1, #11
	ldr r3, [r2, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r2, #12]
	ldr r2, .L_0200ccf0
	ldr r3, [r6, #28]
	ldr r1, .L_0200ccec
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	adds r5, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #3
	bls .L_0200cb5e
	ldr r0, [r7]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	movs r5, #0
.L_0200cb96:
	ldr r3, [r6, #28]
	ldr r2, .L_0200ccf0
	ldr r1, .L_0200ccec
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #3
	bls .L_0200cb96
.L_0200cbb2:
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl Func_02005b0c
	movs r0, #0
	movs r1, #0
	bl Func_020036e0
	movs r1, #1
	movs r0, #1
	bl Func_02003640
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #0
	bl Func_02003640
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r1, #0
	movs r0, #1
	bl Func_020036e0
	movs r0, #132
	bl Func_02005c9c
	movs r0, #14
	bl Object_GetById
	movs r1, #132
	movs r2, #192
	movs r3, #154
	lsls r1, r1, #17
	lsls r2, r2, #13
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r5, #0
.L_0200cc1c:
	ldr r3, [r6, #28]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r3, r1
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #7
	bls .L_0200cc1c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b9c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #17
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #160
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r0, #17
	movs r2, #0
	bl Func_02005b0c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #87
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #251
	bl GameFlag_SetBit
	movs r0, #13
	bl Func_02005bc4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cce4:
	.4byte 0x00001f5c
.L_0200cce8:
	.4byte Data_02007950
.L_0200ccec:
	.4byte 0xffff8000
.L_0200ccf0:
	.4byte 0xffffe000
	.section .text.x0200ccf4,"ax",%progbits
	.global Func_02004cf4
	.thumb_func
Func_02004cf4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	movs r0, #192
	movs r1, #1
	negs r1, r1
	ldr r2, .L_0200d024
	movs r3, #0
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0200d028
	movs r2, #133
	lsls r2, r2, #2
	movs r6, #192
	adds r5, r5, r2
	lsls r6, r6, #8
	movs r1, #185
	movs r2, #159
	lsls r1, r1, #17
	lsls r2, r2, #18
	adds r3, r6, #0
	ldr r0, [r5]
	movs r7, #192
	bl Func_02005b14
	lsls r7, r7, #18
	bl Func_02005a4c
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	movs r0, #0
	movs r1, #0
	mov r8, r2
	bl Func_02005bdc
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #2
	bl Func_02005bd4
	ldr r3, [r7, #108]
	movs r2, #218
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #60
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r0, #40]
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005bd4
	movs r0, #40
	bl Func_02005be4
	movs r0, #60
	bl WaitFrames
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	ldr r0, .L_0200d02c
	bl Func_02005b4c
	movs r0, #10
	movs r1, #0
	bl Func_02005b64
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02005b64
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	ldr r0, [r5]
	adds r1, r6, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Func_02005b64
	movs r0, #9
	movs r1, #0
	bl Func_02005b7c
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Func_02005b64
	movs r2, #20
	mov r1, r8
	movs r0, #9
	bl Func_02005b94
	movs r0, #9
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Func_02005b64
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Func_02005b64
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Func_02005b64
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_0200d030
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #182
	movs r2, #144
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #80
	adds r1, #255
	movs r0, #9
	bl Func_02005b94
	movs r0, #9
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #9
	movs r1, #0
	bl Func_02005b64
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02005b64
	movs r0, #9
	movs r1, #0
	bl Func_02005b64
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	bl Func_02005b64
	movs r0, #10
	movs r1, #0
	bl Func_02005b7c
	movs r0, #10
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02005b7c
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #0
	bl Func_02005b64
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200cfea
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #10
	bl ObjectMotion_ResetAndSetPosition
.L_0200cfea:
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	ldr r1, [r7, #108]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r3, #214
	lsls r3, r3, #1
	movs r0, #254
	adds r2, r1, r3
	lsls r0, r0, #3
	adds r3, #93
	str r3, [r2]
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02005aa4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200d024:
	.4byte 0x02590000
.L_0200d028:
	.4byte gPartyState
.L_0200d02c:
	.4byte 0x00001fee
.L_0200d030:
	.4byte 0x00019999
	.section .text.x0200d034,"ax",%progbits
	.global Func_02005034
	.thumb_func
Func_02005034:
	push {lr}
	ldr r3, .L_0200d08c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200d090
	cmp r2, r3
	bne .L_0200d04e
	bl Func_02005114
	b .L_0200d088
.L_0200d04e:
	ldr r3, .L_0200d094
	cmp r2, r3
	bne .L_0200d05a
	bl Func_0200516c
	b .L_0200d088
.L_0200d05a:
	ldr r3, .L_0200d098
	cmp r2, r3
	bne .L_0200d066
	bl Func_020054e0
	b .L_0200d088
.L_0200d066:
	ldr r3, .L_0200d09c
	cmp r2, r3
	bne .L_0200d072
	bl Func_02005594
	b .L_0200d088
.L_0200d072:
	ldr r3, .L_0200d0a0
	cmp r2, r3
	bne .L_0200d07e
	bl Func_020057e0
	b .L_0200d088
.L_0200d07e:
	ldr r3, .L_0200d0a4
	cmp r2, r3
	bne .L_0200d088
	bl Func_02005898
.L_0200d088:
	movs r0, #0
	pop {pc}
.L_0200d08c:
	.4byte gPartyState
.L_0200d090:
	.4byte 0x0000006e
.L_0200d094:
	.4byte 0x00000071
.L_0200d098:
	.4byte 0x0000006f
.L_0200d09c:
	.4byte 0x00000072
.L_0200d0a0:
	.4byte 0x0000007d
.L_0200d0a4:
	.4byte 0x00000070
	.section .text.x0200d0ac,"ax",%progbits
	.global Func_020050ac
	.thumb_func
Func_020050ac:
	push {r5, lr}
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	bl Func_02005c04
	ldr r0, .L_0200d110
	bl Func_02005c64
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	bl Func_02005c54
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #233
	movs r2, #12
	movs r3, #13
	movs r0, #0
	bl Func_02005c5c
	movs r0, #22
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #23
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	pop {r5, pc}
	.2byte 0x0000
.L_0200d110:
	.4byte Data_02006324
	.section .text.x0200d114,"ax",%progbits
	.global Func_02005114
	.thumb_func
Func_02005114:
	push {lr}
	movs r1, #176
	movs r2, #157
	lsls r2, r2, #17
	lsls r1, r1, #15
	movs r0, #11
	bl Func_02005b0c
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #28]
	bl Func_020050ac
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_ClearBit
	movs r0, #25
	bl Func_02005b8c
	movs r0, #25
	movs r1, #2
	bl Object_SetModeById
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d168
	bl Scheduler_AddOrUpdateCallback
	bl Func_02003184
	pop {pc}
.L_0200d168:
	.4byte Func_02003030
	.section .text.x0200d16c,"ax",%progbits
	.global Func_0200516c
	.thumb_func
Func_0200516c:
	push {r5, r6, r7, lr}
	movs r0, #11
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d26a
	adds r3, r7, #0
	adds r3, #85
	strb r0, [r3]
	movs r3, #168
	lsls r3, r3, #14
	ldr r1, .L_0200d2bc
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	movs r0, #11
	adds r3, r3, r1
	str r3, [r7, #16]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #1
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_0200d2c0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #19
	bne .L_0200d26a
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d26a
	movs r0, #7
	bl Owner_GetState
	adds r5, r0, #0
	ldrh r1, [r5, #52]
	ldrh r3, [r5, #54]
	strh r1, [r5, #56]
	strh r3, [r5, #58]
	lsls r1, r1, #16
	asrs r1, r1, #16
	lsls r0, r1, #14
	bl Engine_MathDivide
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0200d1f8
	movs r3, #0
	cmp r0, #0
	blt .L_0200d1f8
	adds r3, r0, #0
.L_0200d1f8:
	strh r3, [r5, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200d20c
	movs r2, #56
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_0200d20c
	movs r3, #1
	strh r3, [r5, #20]
.L_0200d20c:
	movs r3, #58
	ldrsh r0, [r5, r3]
	movs r2, #54
	ldrsh r1, [r5, r2]
	lsls r0, r0, #14
	bl Engine_MathDivide
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0200d22a
	movs r3, #0
	cmp r0, #0
	blt .L_0200d22a
	adds r3, r0, #0
.L_0200d22a:
	strh r3, [r5, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200d23e
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_0200d23e
	movs r3, #1
	strh r3, [r5, #22]
.L_0200d23e:
	movs r2, #50
	adds r2, #255
	movs r1, #160
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #131
	strb r2, [r3]
	lsls r0, r0, #4
	adds r3, r5, r1
	strb r2, [r3]
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_ClearBit
	movs r0, #7
	bl Party_RemoveOwnerRestored
.L_0200d26a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #18
	ldr r2, .L_0200d2c4
	ldrh r3, [r3]
	ldr r5, .L_0200d2b8
	strh r3, [r2]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #174
	ldrh r3, [r3]
	movs r6, #0
	strh r3, [r2, #2]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #206
	ldrh r3, [r3]
	strh r3, [r2, #4]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #238
	ldrh r3, [r3]
	strh r3, [r2, #6]
	bl Func_020050ac
	movs r0, #30
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #31
	b .L_0200d2c8
	.2byte 0x0000
.L_0200d2b8:
	.4byte 0x00000000
.L_0200d2bc:
	.4byte 0xffd60000
.L_0200d2c0:
	.4byte gPartyState
.L_0200d2c4:
	.4byte Data_02007948
.L_0200d2c8:
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #32
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #33
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #34
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #35
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #29
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #29
	str r6, [r7, #12]
	bl Func_020031f0
	movs r0, #30
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d364
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r1, #151
	movs r2, #172
	movs r0, #21
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02005b0c
.L_0200d364:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_SetBit
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d388
	bl Scheduler_AddOrUpdateCallback
	bl Func_02005aa4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d388:
	.4byte Func_02002c34
	.section .text.x0200d38c,"ax",%progbits
	.global Func_0200538c
	.thumb_func
Func_0200538c:
	push {lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #251
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d3ba
	bl Func_02002de0
.L_0200d3ba:
	bl Func_0200324c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d3d6
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200d3d6:
	ldr r0, .L_0200d4d8
	bl Func_02005c64
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #234
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d438
	movs r0, #11
	bl Object_GetById
	movs r1, #208
	movs r3, #148
	lsls r1, r1, #15
	movs r2, #0
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r3, #6
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #5
	movs r1, #18
	movs r2, #1
	movs r3, #1
	bl Func_02005a6c
	movs r0, #10
	bl WaitFrames
.L_0200d438:
	ldr r3, .L_0200d4dc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d462
	bl Func_020005e0
.L_0200d462:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d472
	bl Func_02000624
.L_0200d472:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d484
	bl Func_02000668
.L_0200d484:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d496
	bl Func_020006ac
.L_0200d496:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d4a8
	bl Func_020006f0
.L_0200d4a8:
	movs r0, #12
	bl Func_02005b8c
	movs r0, #13
	bl Func_02005b8c
	movs r0, #14
	bl Func_02005b8c
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #13
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_0200d4d8:
	.4byte Data_02006324
.L_0200d4dc:
	.4byte gPartyState
	.section .text.x0200d4e0,"ax",%progbits
	.global Func_020054e0
	.thumb_func
Func_020054e0:
	push {lr}
	bl Func_0200538c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #251
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d504
	movs r0, #1
	movs r1, #0
	bl Func_02003640
	movs r0, #1
	movs r1, #5
	bl Func_020036e0
.L_0200d504:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200d58c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200d528
	movs r0, #0
	movs r1, #0
	bl Func_02003640
	movs r0, #0
	movs r1, #0
	b .L_0200d54a
.L_0200d528:
	cmp r3, #98
	bne .L_0200d53a
	movs r0, #1
	movs r1, #0
	bl Func_02003640
	movs r0, #1
	movs r1, #0
	b .L_0200d54a
.L_0200d53a:
	cmp r3, #97
	bne .L_0200d55e
	movs r0, #1
	movs r1, #0
	bl Func_02003640
	movs r0, #1
	movs r1, #2
.L_0200d54a:
	bl Func_020036e0
	ldr r3, .L_0200d590
	movs r1, #0
	ldr r0, [r3]
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	b .L_0200d588
.L_0200d55e:
	cmp r3, #96
	bne .L_0200d588
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r0, #1
	movs r1, #4
	bl Func_020036e0
	ldr r3, .L_0200d590
	movs r1, #0
	ldr r0, [r3]
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #14
	movs r1, #4
	bl Object_SetModeById
.L_0200d588:
	pop {pc}
	.2byte 0x0000
.L_0200d58c:
	.4byte gPartyState
.L_0200d590:
	.4byte Data_02007950
	.section .text.x0200d594,"ax",%progbits
	.global Func_02005594
	.thumb_func
Func_02005594:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #18
	ldrh r3, [r3]
	ldr r2, .L_0200d7cc
	strh r3, [r2]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #14
	ldrh r3, [r3]
	strh r3, [r2, #2]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #12
	ldrh r3, [r3]
	strh r3, [r2, #4]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #152
	ldrh r3, [r3]
	strh r3, [r2, #6]
	bl Func_0200538c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #251
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d600
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r0, #1
	movs r1, #4
	bl Func_020036e0
.L_0200d600:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200d7d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #10
	cmp r3, #6
	bhi .L_0200d666
	ldr r2, .L_0200d7d4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200d620:
	.4byte .L_0200d63c
	.4byte .L_0200d642
	.4byte .L_0200d648
	.4byte .L_0200d64e
	.4byte .L_0200d654
	.4byte .L_0200d65a
	.4byte .L_0200d660
.L_0200d63c:
	bl Func_020040cc
	b .L_0200d7c8
.L_0200d642:
	bl Func_02004294
	b .L_0200d7c8
.L_0200d648:
	bl Func_02004424
	b .L_0200d7c8
.L_0200d64e:
	bl Func_020047e4
	b .L_0200d7c8
.L_0200d654:
	bl Func_0200489c
	b .L_0200d7c8
.L_0200d65a:
	bl Func_02004978
	b .L_0200d7c8
.L_0200d660:
	bl Func_02004a28
	b .L_0200d7c8
.L_0200d666:
	movs r0, #17
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #26
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #5
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #6
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d6a8
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	b .L_0200d6c8
.L_0200d6a8:
	movs r0, #131
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d6c8
	movs r3, #128
	movs r1, #152
	movs r2, #176
	lsls r3, r3, #6
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02005b14
.L_0200d6c8:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d6e8
	movs r0, #1
	movs r1, #6
	bl Func_020036e0
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
.L_0200d6e8:
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #144
	ldr r0, .L_0200d7d8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r5, #0
.L_0200d6fc:
	adds r0, r5, #0
	adds r0, #18
	bl Object_GetById
	adds r5, #1
	adds r0, #92
	movs r3, #2
	strb r3, [r0]
	cmp r5, #7
	bls .L_0200d6fc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d732
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d746
	bl Func_020032b0
	b .L_0200d746
.L_0200d732:
	ldr r3, .L_0200d7d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_0200d746
	bl Func_02003dec
.L_0200d746:
	bl Func_02005aa4
	ldr r3, .L_0200d7d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200d768
	movs r0, #0
	movs r1, #0
	bl Func_02003640
	movs r0, #0
	movs r1, #0
	b .L_0200d78a
.L_0200d768:
	cmp r3, #98
	bne .L_0200d77a
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r0, #1
	movs r1, #0
	b .L_0200d78a
.L_0200d77a:
	cmp r3, #97
	bne .L_0200d79e
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r0, #1
	movs r1, #4
.L_0200d78a:
	bl Func_020036e0
	ldr r3, .L_0200d7dc
	movs r1, #0
	ldr r0, [r3]
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	b .L_0200d7c8
.L_0200d79e:
	cmp r3, #96
	bne .L_0200d7c8
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r0, #1
	movs r1, #4
	bl Func_020036e0
	ldr r3, .L_0200d7dc
	movs r1, #0
	ldr r0, [r3]
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #14
	movs r1, #5
	bl Object_SetModeById
.L_0200d7c8:
	pop {r5, pc}
	.2byte 0x0000
.L_0200d7cc:
	.4byte Data_02007948
.L_0200d7d0:
	.4byte gPartyState
.L_0200d7d4:
	.4byte .L_0200d620
.L_0200d7d8:
	.4byte Func_02002ccc
.L_0200d7dc:
	.4byte Data_02007950
	.section .text.x0200d7e0,"ax",%progbits
	.global Func_020057e0
	.thumb_func
Func_020057e0:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	ldr r3, .L_0200d880
	ldr r2, .L_0200d884
	ldrh r3, [r3]
	movs r1, #241
	strh r3, [r2]
	ldr r3, .L_0200d888
	lsls r1, r1, #1
	ldrh r3, [r3]
	strh r3, [r2, #2]
	ldr r3, .L_0200d88c
	ldrh r3, [r3]
	strh r3, [r2, #4]
	ldr r3, .L_0200d890
	adds r2, r3, r1
	movs r1, #0
	ldrsh r2, [r2, r1]
	cmp r2, #10
	bne .L_0200d83e
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	bl Func_02003c84
	b .L_0200d87c
.L_0200d83e:
	cmp r2, #11
	bne .L_0200d866
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	bl Func_02003c14
	b .L_0200d87c
.L_0200d866:
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d894
	bl Scheduler_AddOrUpdateCallback
	bl Func_02005aa4
.L_0200d87c:
	pop {r5, pc}
	.2byte 0x0000
.L_0200d880:
	.4byte 0x05000152
.L_0200d884:
	.4byte Data_02007948
.L_0200d888:
	.4byte 0x0500014e
.L_0200d88c:
	.4byte 0x0500014c
.L_0200d890:
	.4byte gPartyState
.L_0200d894:
	.4byte Func_02002d64
	.section .text.x0200d898,"ax",%progbits
	.global Func_02005898
	.thumb_func
Func_02005898:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Func_02003288
	ldr r3, .L_0200d984
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_0200d90e
	movs r0, #254
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d90a
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d8f6
	movs r3, #176
	movs r1, #217
	lsls r3, r3, #8
	movs r0, #8
	lsls r1, r1, #17
	ldr r2, .L_0200d988
	bl Func_02005b14
	b .L_0200d90e
.L_0200d8f6:
	movs r3, #176
	movs r1, #182
	movs r2, #144
	lsls r3, r3, #8
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005b14
	b .L_0200d90e
.L_0200d90a:
	bl Func_02004cf4
.L_0200d90e:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d930
	movs r0, #16
	bl Object_GetById
	ldr r2, .L_0200d98c
	ldr r3, [r0, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, .L_0200d990
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
.L_0200d930:
	movs r1, #0
	movs r0, #16
	bl Func_02005c74
	movs r0, #16
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r3, #0
	strh r3, [r2, #18]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	ldr r3, [r0, #8]
	movs r2, #224
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r1, #6
	movs r0, #16
	bl Object_SetModeById
	ldr r3, .L_0200d984
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #14
	bne .L_0200d982
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_SetBit
.L_0200d982:
	pop {pc}
.L_0200d984:
	.4byte gPartyState
.L_0200d988:
	.4byte 0x027a0000
.L_0200d98c:
	.4byte 0xfff20000
.L_0200d990:
	.4byte 0xfff80000
	.section .text.x0200d994,"ax",%progbits
	.global Func_02005994
	.thumb_func
Func_02005994:
	push {lr}
	bl Func_02005bcc
	pop {pc}
	.section .rodata.x0200dca4,"a",%progbits
	.global Data_02005ca4
Data_02005ca4:
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02005cd0
Data_02005cd0:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02005d18
Data_02005d18:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00620000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005d5c
Data_02005d5c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005da0
Data_02005da0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00440000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000800
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02005e54
Data_02005e54:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005e88
Data_02005e88:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01540000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01640000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005eec
Data_02005eec:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005f14
Data_02005f14:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005f58
Data_02005f58:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005f9c
Data_02005f9c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00240000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005fe0
Data_02005fe0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01740000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006014
Data_02006014:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x012a0000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006058
Data_02006058:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200609c
Data_0200609c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x012a0000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020060e0
Data_020060e0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00de0000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006108
Data_02006108:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x011c0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200613c
Data_0200613c:
	.4byte 0x0000002e
	.4byte Func_02000048
	.4byte 0x0000002e
	.4byte Func_02000054
	.global Data_0200614c
Data_0200614c:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020061a8
Data_020061a8:
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte Func_020000c4
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x012a0000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02006324
Data_02006324:
	.4byte 0x02010008
	.4byte 0x0000ffff
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200635c
Data_0200635c:
	.4byte 0x00280190
	.4byte 0x01a00170
	.4byte 0x01800038
	.4byte 0x000effff
	.4byte 0x00280070
	.4byte 0x00800270
	.4byte 0x02800038
	.4byte 0x000fffff
	.4byte 0x00280180
	.4byte 0x01900370
	.4byte 0x03800038
	.4byte 0x0010ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x0000006e
	.4byte 0x0011b002
	.4byte 0x0020106f
	.4byte 0x00301070
	.4byte 0x00406070
	.4byte 0x00505070
	.4byte 0x00604070
	.4byte 0x00708070
	.4byte 0x0080a070
	.4byte 0x0090206f
	.4byte 0x00a0506f
	.4byte 0x00000071
	.4byte 0x0011b002
	.4byte 0x00201072
	.4byte 0x00301070
	.4byte 0x00406070
	.4byte 0x00505070
	.4byte 0x00604070
	.4byte 0x00708070
	.4byte 0x0080a070
	.4byte 0x00902072
	.4byte 0x00a05072
	.4byte 0x0000006f
	.4byte 0x0010206e
	.4byte 0x0020906e
	.4byte 0x0030107b
	.4byte 0x00403078
	.4byte 0x0050a06e
	.4byte 0x00000072
	.4byte 0x00102071
	.4byte 0x00209071
	.4byte 0x0030107b
	.4byte 0x00403078
	.4byte 0x0050a071
	.4byte 0x00a02079
	.4byte 0x00b03079
	.4byte 0x00c01079
	.4byte 0x00d05078
	.4byte 0x00000070
	.4byte 0x1010306e
	.4byte 0x0000086b
	.4byte 0x10103071
	.4byte 0xffffffff
	.4byte 0x10203070
	.4byte 0xffffffff
	.4byte 0x10302070
	.4byte 0xffffffff
	.4byte 0x1040606e
	.4byte 0x0000086b
	.4byte 0x10406071
	.4byte 0xffffffff
	.4byte 0x1050506e
	.4byte 0x0000086b
	.4byte 0x10505071
	.4byte 0xffffffff
	.4byte 0x1060406e
	.4byte 0x0000086b
	.4byte 0x10604071
	.4byte 0xffffffff
	.4byte 0x1070f070
	.4byte 0xffffffff
	.4byte 0x1080706e
	.4byte 0x0000086b
	.4byte 0x10807071
	.4byte 0xffffffff
	.4byte 0x1090e070
	.4byte 0xffffffff
	.4byte 0x10a0806e
	.4byte 0x0000086b
	.4byte 0x10a08071
	.4byte 0xffffffff
	.4byte 0x10b0c070
	.4byte 0xffffffff
	.4byte 0x10c0b070
	.4byte 0xffffffff
	.4byte 0x10d10070
	.4byte 0xffffffff
	.4byte 0x10e09070
	.4byte 0xffffffff
	.4byte 0x10f07070
	.4byte 0xffffffff
	.4byte 0x1100d070
	.4byte 0xffffffff
	.4byte 0x0000007d
	.4byte 0x00a10072
	.4byte 0x00b0a072
	.4byte 0x000001ff
	.global Data_020064f8
Data_020064f8:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006510
Data_02006510:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00024000
	.4byte 0xffff0183
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0183
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0073
	.4byte 0x00000003
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x00e40000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x0000d000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x01200000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00008000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01120000
	.4byte 0x0000b000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x003a0000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00013000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0001b000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0001d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00013000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x011a0000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00015000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01ae0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00023000
	.4byte 0xffff0170
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020066d8
Data_020066d8:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0001d000
	.4byte 0xffff008d
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00720000
	.4byte 0x0001d000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001d000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x01240000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x0001b000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0001d000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x0001b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01160000
	.4byte 0x0001d000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x009e0000
	.4byte 0x0001b000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x0001d000
	.4byte 0xffff008b
	.4byte 0x00000001
	.4byte 0x01460000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0001d000
	.4byte 0xffff008c
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01340000
	.4byte 0x0001d000
	.4byte 0xffff008d
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x00920000
	.4byte 0x0001b000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x008a0000
	.4byte 0x0001d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x008e0000
	.4byte 0x0001d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x0001d000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b50000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00f20000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01120000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01520000
	.4byte 0x01024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01720000
	.4byte 0x01024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x003c0000
	.4byte 0x0002b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020069a8
Data_020069a8:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006a98
Data_02006a98:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff018b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01024000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0003d000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x0003b000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x0003b000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x01a60000
	.4byte 0x0003b000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01960000
	.4byte 0x0003b000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00820000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x00022000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006cd8
Data_02006cd8:
	.4byte 0xffff001b
	.4byte 0x00000001
	.4byte 0x016e0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00025000
	.4byte 0xffff001c
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00003000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x0002d000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00005000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000d000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x0000d000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x02c60000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00003000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00023000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00015000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x006a0000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00018000
	.4byte 0xffff0053
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00010000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00003000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00003000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00003000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00540000
	.4byte 0x00000000
	.4byte 0x035a0000
	.4byte 0x00003000
	.4byte 0xffff0087
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x036e0000
	.4byte 0x00005000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01760000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00003000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006ee8
Data_02006ee8:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006ef4
Data_02006ef4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_02003948
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte Func_02002ad8
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte Func_02002af8
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte Func_020005d0
	.4byte 0x00008515
	.4byte 0x09e9000c
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte Func_02005994
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte Func_02005994
	.4byte 0x10009a15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002001
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002011
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000228
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002012
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002005
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002013
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002006
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002014
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002007
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002015
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002008
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002016
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002009
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002017
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000200a
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002018
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_0200024c
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002019
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000200e
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x0000201a
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte Func_02000270
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000201b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020070d4
Data_020070d4:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_020038ec
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_020038ec
	.4byte 0x0000c602
	.4byte 0x02090003
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0x02090004
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0x02090005
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0x02090006
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0x02090007
	.4byte Func_02003948
	.4byte 0x0000c602
	.4byte 0x02090008
	.4byte Func_02003948
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte Func_020038ec
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020038ec
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte Func_02002ad8
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte Func_02002af8
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte Func_020005d0
	.4byte 0x00008515
	.4byte 0x09e9000c
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte Func_02005994
	.4byte 0x00000000
	.4byte 0x08fd0014
	.4byte 0x00001e35
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001f9d
	.4byte 0x00008d15
	.4byte 0x08fd0014
	.4byte 0x00001e39
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001fa9
	.4byte 0x00000000
	.4byte 0x08fd000e
	.4byte 0x00001e36
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001f97
	.4byte 0x00008d15
	.4byte 0x08fd000e
	.4byte 0x00001e3a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001fa3
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001e37
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001e3b
	.4byte 0x00000000
	.4byte 0x08fd0015
	.4byte 0x00001e3d
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001f9e
	.4byte 0x00008d15
	.4byte 0x08fd0015
	.4byte 0x00001e41
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001faa
	.4byte 0x00000000
	.4byte 0x08fd0011
	.4byte 0x00001e3e
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001f9a
	.4byte 0x00008d15
	.4byte 0x08fd0011
	.4byte 0x00001e42
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001fa6
	.4byte 0x00000000
	.4byte 0x08fd0016
	.4byte 0x00001e40
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f9f
	.4byte 0x00008d15
	.4byte 0x08fd0016
	.4byte 0x00001e44
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001fab
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001e46
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001e4a
	.4byte 0x00000000
	.4byte 0x08fd000f
	.4byte 0x00001e47
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001f98
	.4byte 0x00008d15
	.4byte 0x08fd000f
	.4byte 0x00001e4b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001fa4
	.4byte 0x00000000
	.4byte 0x08fd0019
	.4byte 0x00001e48
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001fa2
	.4byte 0x00008d15
	.4byte 0x08fd0019
	.4byte 0x00001e4c
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001fae
	.4byte 0x00000000
	.4byte 0x08fd0012
	.4byte 0x00001e4d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001f9b
	.4byte 0x00008d15
	.4byte 0x08fd0012
	.4byte 0x00001e51
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001fa7
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001e4e
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e52
	.4byte 0x00000000
	.4byte 0x08fd0010
	.4byte 0x00001e55
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001f99
	.4byte 0x00008d15
	.4byte 0x08fd0010
	.4byte 0x00001e58
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001fa5
	.4byte 0x00000000
	.4byte 0x08fd0017
	.4byte 0x00001e57
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001fa0
	.4byte 0x00008d15
	.4byte 0x08fd0017
	.4byte 0x00001e5b
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001fac
	.4byte 0x00000000
	.4byte 0x08fd0013
	.4byte 0x00001e5d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001f9c
	.4byte 0x00008d15
	.4byte 0x08fd0013
	.4byte 0x00001e61
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001fa8
	.4byte 0x00000000
	.4byte 0x08fd0018
	.4byte 0x00001e5f
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001fa1
	.4byte 0x00008d15
	.4byte 0x08fd0018
	.4byte 0x00001e63
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001fad
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte Func_0200042c
	.4byte 0x00008d15
	.4byte 0xffff0024
	.4byte 0x00001eff
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007434
Data_02007434:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_020038ec
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_020038ec
	.4byte 0x00004602
	.4byte 0x186a0003
	.4byte Func_02003bf0
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02003930
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte Func_020038ec
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte Func_020005d0
	.4byte 0x00008c15
	.4byte 0x09ea000b
	.4byte Func_02000734
	.4byte 0x00000009
	.4byte 0x09ea0000
	.4byte Func_02000734
	.4byte 0x50008805
	.4byte 0x086a000a
	.4byte Func_02000618
	.4byte 0x50008805
	.4byte 0x0204000b
	.4byte Func_0200065c
	.4byte 0x50008805
	.4byte 0x0205000c
	.4byte Func_020006a0
	.4byte 0x50008805
	.4byte 0x0206000d
	.4byte Func_020006e4
	.4byte 0x50008805
	.4byte 0x0207000e
	.4byte Func_02000728
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020074dc
Data_020074dc:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_020038ec
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_020038ec
	.4byte 0x00004602
	.4byte 0x186a0003
	.4byte Func_02003bf0
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02003930
	.4byte 0x00000002
	.4byte 0x08f30005
	.4byte Func_020038ec
	.4byte 0x00000002
	.4byte 0x092f0005
	.4byte Func_02001218
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte Func_020038ec
	.4byte 0x00001815
	.4byte 0x02010008
	.4byte Func_020005d0
	.4byte 0x00008c15
	.4byte 0x09ea000b
	.4byte Func_02000734
	.4byte 0x00000009
	.4byte 0x09ea0000
	.4byte Func_02000734
	.4byte 0x50008805
	.4byte 0x086a000a
	.4byte Func_02000618
	.4byte 0x50008805
	.4byte 0x0204000b
	.4byte Func_0200065c
	.4byte 0x50008805
	.4byte 0x0205000c
	.4byte Func_020006a0
	.4byte 0x50008805
	.4byte 0x0206000d
	.4byte Func_020006e4
	.4byte 0x50008805
	.4byte 0x0207000e
	.4byte Func_02000728
	.4byte 0x00000002
	.4byte 0x08f30014
	.4byte Func_020007c8
	.4byte 0x00000002
	.4byte 0x092e0016
	.4byte Func_02000c90
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x00001eac
	.4byte 0x00008d15
	.4byte 0xffff0007
	.4byte 0x00001ead
	.4byte 0x00000002
	.4byte 0x08f50015
	.4byte Func_02002050
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001f87
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001f88
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001f89
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001f8a
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f8b
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001f8c
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001f8d
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001f8e
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001f8f
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001f90
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001f91
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001f92
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001f93
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001f94
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001f95
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001f96
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007698
Data_02007698:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000202
	.4byte 0xffff0007
	.4byte Func_02003ac8
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000202
	.4byte 0xffff0009
	.4byte Func_02003b20
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000202
	.4byte 0xffff000d
	.4byte Func_02003b88
	.4byte 0x00000000
	.4byte 0x08ee0008
	.4byte 0x00001ff5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001fff
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ffd
	.4byte 0x00000000
	.4byte 0x08ee0009
	.4byte 0x00001ff9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002000
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ffe
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000201c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002021
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020002c4
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002022
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002020
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002023
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002024
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002026
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002025
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002027
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_020004dc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte Func_02000500
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte Func_020004dc
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte Func_02000500
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002029
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000202b
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000202c
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000202e
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000202d
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000202f
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_02000318
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte Func_02000318
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002031
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_02000350
	.4byte 0x00000003
	.4byte 0xffff0015
	.4byte Func_02000350
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002033
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000388
	.4byte 0x00000003
	.4byte 0xffff0016
	.4byte Func_02000388
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002035
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte Func_020003c0
	.4byte 0x00000003
	.4byte 0xffff0017
	.4byte Func_020003c0
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002038
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00002037
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00002039
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte Func_020003f8
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte Func_020003f8
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000203b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global Data_02007948
Data_02007948:
	.space 0x00000008
	.global Data_02007950
Data_02007950:
