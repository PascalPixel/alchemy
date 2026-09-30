.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02002174
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
	.4byte Data_020021a4
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r1, .L_0200808c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008090
	cmp r2, r3
	bne .L_02008064
	ldr r0, .L_02008094
	b .L_0200808a
.L_02008064:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #20
	bne .L_02008076
	ldr r0, .L_02008098
	b .L_0200808a
.L_02008076:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008088
	ldr r0, .L_0200809c
	b .L_0200808a
.L_02008088:
	ldr r0, .L_020080a0
.L_0200808a:
	pop {pc}
.L_0200808c:
	.4byte gPartyState
.L_02008090:
	.4byte 0x0000002e
.L_02008094:
	.4byte Data_02002828
.L_02008098:
	.4byte Data_02002750
.L_0200809c:
	.4byte Data_02002480
.L_020080a0:
	.4byte Data_02002240
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {r5, lr}
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	ldr r5, .L_020080f8
	adds r0, r5, #0
	bl Func_02001e18
	movs r1, #0
	movs r0, #9
	bl Func_02001e20
	bl Func_02001ee8
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020080dc
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001e18
	b .L_020080e8
.L_020080dc:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001e18
.L_020080e8:
	movs r0, #9
	movs r1, #0
	bl Func_02001e30
	bl Func_02001d58
	pop {r5, pc}
	.2byte 0x0000
.L_020080f8:
	.4byte 0x0000189c
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {r5, lr}
	ldr r3, .L_02008130
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200812c
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008134
	movs r0, #1
	movs r1, #22
	bl Func_02001f08
	b .L_020081a8
.L_0200812c:
	.4byte 0xffffc000
.L_02008130:
	.4byte gPartyState
.L_02008134:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008184
	ldr r5, .L_020081ac
	adds r0, r5, #0
	bl Func_02001e18
	movs r1, #0
	movs r0, #22
	bl Func_02001e20
	bl Func_02001ee8
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200816e
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001e18
	b .L_0200817a
.L_0200816e:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001e18
.L_0200817a:
	movs r0, #22
	movs r1, #0
	bl Func_02001e30
	b .L_020081a8
.L_02008184:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02001d10
	cmp r0, #0
	beq .L_0200819a
	ldr r0, .L_020081b0
	bl Func_02001e18
	b .L_020081a0
.L_0200819a:
	ldr r0, .L_020081b4
	bl Func_02001e18
.L_020081a0:
	movs r0, #22
	movs r1, #0
	bl Func_02001e30
.L_020081a8:
	pop {r5, pc}
	.2byte 0x0000
.L_020081ac:
	.4byte 0x00002126
.L_020081b0:
	.4byte 0x00001d5a
.L_020081b4:
	.4byte 0x000018fe
	.section .text.x020081b8,"ax",%progbits
	.global Func_020001b8
	.thumb_func
Func_020001b8:
	push {r5, lr}
	ldr r3, .L_02008204
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008200
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020081e8
	adds r0, r5, #0
	bl Func_02001f00
	b .L_02008230
.L_020081e8:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02001d10
	cmp r0, #0
	beq .L_0200820c
	ldr r0, .L_02008208
	bl Func_02001e18
	b .L_02008228
	.2byte 0x0000
.L_02008200:
	.4byte 0xffffc000
.L_02008204:
	.4byte gPartyState
.L_02008208:
	.4byte 0x00002120
.L_0200820c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008222
	ldr r0, .L_02008234
	bl Func_02001e18
	b .L_02008228
.L_02008222:
	ldr r0, .L_02008238
	bl Func_02001e18
.L_02008228:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001e30
.L_02008230:
	pop {r5, pc}
	.2byte 0x0000
.L_02008234:
	.4byte 0x00001d54
.L_02008238:
	.4byte 0x000018e2
	.section .text.x0200823c,"ax",%progbits
	.global Func_0200023c
	.thumb_func
Func_0200023c:
	push {lr}
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	movs r0, #14
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r0, #14
	movs r1, #6
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #0
	movs r1, #14
	movs r0, #4
	bl Func_02001df8
	ldr r0, .L_02008288
	bl Func_02001e18
	movs r0, #14
	movs r1, #0
	bl Func_02001e30
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02001e38
	bl Func_02001d58
	pop {pc}
	.2byte 0x0000
.L_02008288:
	.4byte 0x000018aa
	.section .text.x0200828c,"ax",%progbits
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl Func_02001d10
	cmp r0, #0
	beq .L_020082bc
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02001d10
	cmp r0, #0
	beq .L_020082ac
	ldr r0, .L_020082f8
	b .L_020082ae
.L_020082ac:
	ldr r0, .L_020082fc
.L_020082ae:
	bl Func_02001e18
	movs r0, #10
	movs r1, #0
	bl Func_02001e30
	b .L_020082f6
.L_020082bc:
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02001d10
	cmp r0, #0
	beq .L_020082d8
	ldr r0, .L_02008300
	bl Func_02001e18
	movs r0, #10
	movs r1, #0
	bl Func_02001e30
	b .L_020082e6
.L_020082d8:
	ldr r0, .L_02008304
	bl Func_02001e18
	movs r0, #10
	movs r1, #0
	bl Func_02001e30
.L_020082e6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_020082f6:
	pop {pc}
.L_020082f8:
	.4byte 0x0000207d
.L_020082fc:
	.4byte 0x00002084
.L_02008300:
	.4byte 0x00002082
.L_02008304:
	.4byte 0x00002071
	.section .text.x02008308,"ax",%progbits
	.global Func_02000308
	.thumb_func
Func_02000308:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl Func_02001d10
	cmp r0, #0
	beq .L_0200832c
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008328
	ldr r0, .L_02008358
	b .L_0200833a
.L_02008328:
	ldr r0, .L_0200835c
	b .L_0200833a
.L_0200832c:
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008348
	ldr r0, .L_02008360
.L_0200833a:
	bl Func_02001e18
	movs r0, #11
	movs r1, #0
	bl Func_02001e30
	b .L_02008356
.L_02008348:
	ldr r0, .L_02008364
	bl Func_02001e18
	movs r0, #11
	movs r1, #0
	bl Func_02001e30
.L_02008356:
	pop {pc}
.L_02008358:
	.4byte 0x0000207e
.L_0200835c:
	.4byte 0x00002085
.L_02008360:
	.4byte 0x00002083
.L_02008364:
	.4byte 0x00002072
	.section .text.x02008368,"ax",%progbits
	.global Func_02000368
	.thumb_func
Func_02000368:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl Func_02001d10
	cmp r0, #0
	beq .L_0200838c
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008388
	ldr r0, .L_020083bc
	b .L_0200839c
.L_02008388:
	ldr r0, .L_020083c0
	b .L_0200839c
.L_0200838c:
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02001d10
	ldr r3, .L_020083c4
	cmp r0, #0
	beq .L_020083aa
	adds r0, r3, #0
.L_0200839c:
	bl Func_02001e18
	movs r0, #10
	movs r1, #0
	bl Func_02001e30
	b .L_020083b8
.L_020083aa:
	adds r0, r3, #0
	bl Func_02001e18
	movs r0, #10
	movs r1, #0
	bl Func_02001e30
.L_020083b8:
	pop {pc}
	.2byte 0x0000
.L_020083bc:
	.4byte 0x0000207f
.L_020083c0:
	.4byte 0x00002086
.L_020083c4:
	.4byte 0x00002073
	.section .text.x020083c8,"ax",%progbits
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl Func_02001d10
	cmp r0, #0
	beq .L_020083ec
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02001d10
	cmp r0, #0
	beq .L_020083e8
	ldr r0, .L_0200841c
	b .L_020083fc
.L_020083e8:
	ldr r0, .L_02008420
	b .L_020083fc
.L_020083ec:
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02001d10
	ldr r3, .L_02008424
	cmp r0, #0
	beq .L_0200840a
	adds r0, r3, #0
.L_020083fc:
	bl Func_02001e18
	movs r0, #11
	movs r1, #0
	bl Func_02001e30
	b .L_02008418
.L_0200840a:
	adds r0, r3, #0
	bl Func_02001e18
	movs r0, #11
	movs r1, #0
	bl Func_02001e30
.L_02008418:
	pop {pc}
	.2byte 0x0000
.L_0200841c:
	.4byte 0x00002080
.L_02008420:
	.4byte 0x00002087
.L_02008424:
	.4byte 0x00002074
	.section .text.x02008428,"ax",%progbits
	.global Func_02000428
	.thumb_func
Func_02000428:
	push {lr}
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02001d10
	cmp r0, #0
	bne .L_020084a0
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02001d18
	ldr r0, .L_020084bc
	bl Func_02001e18
	movs r1, #12
	movs r2, #0
	negs r1, r1
	movs r0, #14
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	bl Func_02001e30
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #14
	bl Func_02001e48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #14
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r0, #14
	movs r1, #0
	bl Func_02001e30
	b .L_020084b8
.L_020084a0:
	ldr r0, .L_020084c0
	bl Func_02001e18
	movs r0, #14
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r0, #14
	movs r1, #0
	bl Func_02001e30
.L_020084b8:
	pop {pc}
	.2byte 0x0000
.L_020084bc:
	.4byte 0x00002052
.L_020084c0:
	.4byte 0x00002054
	.section .text.x020084c4,"ax",%progbits
	.global Func_020004c4
	.thumb_func
Func_020004c4:
	push {lr}
	ldr r1, .L_02008518
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200851c
	cmp r2, r3
	bne .L_020084dc
	ldr r0, .L_02008520
	b .L_02008514
.L_020084dc:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #20
	bne .L_020084ee
	ldr r0, .L_02008524
	b .L_02008514
.L_020084ee:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008500
	ldr r0, .L_02008528
	b .L_02008514
.L_02008500:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008512
	ldr r0, .L_0200852c
	b .L_02008514
.L_02008512:
	ldr r0, .L_02008530
.L_02008514:
	pop {pc}
	.2byte 0x0000
.L_02008518:
	.4byte gPartyState
.L_0200851c:
	.4byte 0x0000002e
.L_02008520:
	.4byte Data_02002c9c
.L_02008524:
	.4byte Data_02002bb8
.L_02008528:
	.4byte Data_0200308c
.L_0200852c:
	.4byte Data_02002da4
.L_02008530:
	.4byte Data_020028a0
	.section .text.x02008534,"ax",%progbits
	.global Func_02000534
	.thumb_func
Func_02000534:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008554
	movs r1, #222
	movs r2, #128
	movs r0, #26
	lsls r1, r1, #18
	lsls r2, r2, #12
	bl Func_02001db8
	b .L_0200856c
.L_02008554:
	movs r0, #26
	bl Object_GetById
	movs r1, #242
	bl Func_02001d38
	movs r0, #26
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200856c:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008570,"ax",%progbits
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02001d10
	cmp r0, #0
	bne .L_020085b4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #3
	strh r2, [r3]
	adds r0, #17
	bl Func_02001d10
	cmp r0, #0
	bne .L_020085a6
	bl Func_020007dc
	b .L_02008614
.L_020085a6:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r0, #10
	b .L_020085e2
.L_020085b4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02001d10
	cmp r0, #0
	beq .L_020085ec
	movs r3, #224
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
.L_020085e2:
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_02008614
.L_020085ec:
	movs r3, #160
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
.L_02008614:
	pop {pc}
	.2byte 0x0000
.L_02008618:
	.4byte Data_02030000
.L_0200861c:
	.4byte 0x03490000
.L_02008620:
	.4byte 0x036d0000
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {r5, r6, lr}
	movs r6, #192
	lsls r6, r6, #18
	ldr r3, [r6, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	ldr r5, .L_0200879c
	adds r2, #255
	str r2, [r3]
	adds r2, #11
	adds r3, r5, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	bl Func_02001e80
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087a0
	cmp r2, r3
	bne .L_020086e6
	ldr r3, [r6, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #3
	strh r2, [r3]
	adds r0, #33
	bl Func_02001d10
	cmp r0, #0
	bne .L_0200869a
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_020086b2
.L_0200869a:
	movs r0, #29
	bl Object_GetById
	movs r1, #3
	bl Func_02001e10
	movs r0, #30
	bl Object_GetById
	movs r1, #5
	bl Func_02001e10
.L_020086b2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02001d10
	cmp r0, #0
	beq .L_020086e6
	movs r3, #64
	movs r5, #16
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #110
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02001d28
	movs r3, #0
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #46
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02001d20
.L_020086e6:
	ldr r5, .L_0200879c
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #20
	bne .L_020086fa
	bl Func_02000570
.L_020086fa:
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087a4
	cmp r2, r3
	bne .L_0200876a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl Func_02001d10
	cmp r0, #0
	beq .L_02008724
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_0200876a
.L_02008724:
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02001ef0
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #8
	ldrb r2, [r1, #26]
	movs r0, #10
	orrs r3, r2
	strb r3, [r1, #26]
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #3
	ldrb r2, [r1, #17]
	movs r0, #10
	ands r3, r2
	movs r2, #32
	orrs r3, r2
	strb r3, [r1, #17]
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r3, #1
	strb r3, [r2, #25]
.L_0200876a:
	bl Func_02000534
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02001d10
	cmp r0, #0
	beq .L_0200878a
	movs r0, #33
	bl Object_GetById
	movs r1, #2
	bl Func_02001e10
	b .L_02008796
.L_0200878a:
	movs r0, #28
	bl Object_GetById
	movs r1, #2
	bl Func_02001e10
.L_02008796:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_0200879c:
	.4byte gPartyState
.L_020087a0:
	.4byte 0x0000002d
.L_020087a4:
	.4byte 0x0000002e
	.section .text.x020087a8,"ax",%progbits
	.global Func_020007a8
	.thumb_func
Func_020007a8:
	movs r0, #0
	bx lr
	.section .text.x020087ac,"ax",%progbits
	.global Func_020007ac
	.thumb_func
Func_020007ac:
	push {lr}
	ldr r2, [r0, #80]
	cmp r0, #0
	beq .L_020087c0
	cmp r2, #0
	beq .L_020087c0
	ldrh r3, [r2, #18]
	ldr r1, .L_020087c4
	adds r3, r3, r1
	strh r3, [r2, #18]
.L_020087c0:
	movs r0, #0
	pop {pc}
.L_020087c4:
	.4byte 0xfffff800
	.section .text.x020087c8,"ax",%progbits
	.global Func_020007c8
	.thumb_func
Func_020007c8:
	push {lr}
	ldr r2, [r0, #80]
	cmp r0, #0
	beq .L_020087d8
	cmp r2, #0
	beq .L_020087d8
	movs r3, #0
	strh r3, [r2, #18]
.L_020087d8:
	movs r0, #0
	pop {pc}
	.section .text.x020087dc,"ax",%progbits
	.global Func_020007dc
	.thumb_func
Func_020007dc:
	push {r5, r6, lr}
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #200
	movs r2, #222
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001db8
	movs r0, #204
	movs r1, #1
	movs r2, #212
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl Func_02001e60
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	movs r0, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, .L_02008c24
	bl Func_02001e18
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r2, #192
	movs r1, #198
	lsls r2, r2, #2
	movs r0, #4
	lsls r1, r1, #1
	adds r2, #101
	bl ObjectMotion_SetPositionAndReset
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #25
	movs r2, #19
	movs r0, #11
	bl Func_02001ec8
	movs r0, #11
	bl Object_RefreshSelectorById
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02001e38
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02001e38
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl Func_02001e38
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #244
	movs r1, #1
	movs r2, #205
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #17
	bl Func_02001e60
	bl Func_02001e68
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001e48
	movs r2, #10
	movs r1, #0
	movs r0, #9
	bl Func_02001e28
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001e28
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_02001e28
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	bl Func_02001e40
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_02001e48
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r5, #2
.L_0200894e:
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #134
	bl Func_02001f10
	subs r5, #1
	movs r0, #9
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	cmp r5, #0
	bge .L_0200894e
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #7
	bl Func_02001e48
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_02001e48
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02001e48
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r2, #0
	movs r1, #9
	movs r0, #10
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r2, #0
	movs r1, #10
	movs r0, #9
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #10
	movs r1, #0
	movs r0, #9
	bl Func_02001e28
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_02001e48
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02001e40
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_02001e28
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02001e38
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #0
	movs r2, #10
	movs r0, #7
	bl Func_02001e28
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	movs r0, #7
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #24
	movs r1, #1
	bl Func_02001e90
	movs r0, #7
	movs r1, #8
	bl Func_02001e98
	bl Func_02001eb0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001e48
	movs r0, #9
	movs r1, #7
	movs r2, #0
	bl Func_02001df8
	movs r2, #0
	movs r1, #7
	movs r0, #10
	bl Func_02001df8
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02008c28
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #1
	bl Func_02001e88
	bl Func_02001ea0
	bl Func_02001ea8
	movs r1, #0
	movs r2, #10
	movs r0, #11
	bl Func_02001e28
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #9
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #9
	str r5, [r0, #24]
	movs r0, #9
	bl Object_GetById
	str r5, [r0, #28]
	movs r0, #4
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #8
	strb r3, [r0]
	movs r2, #0
	movs r0, #9
	negs r1, r1
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r1, .L_02008c2c
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #9
	bl Object_RefreshSelectorById
	movs r0, #127
	bl Func_02001f10
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #0
	orrs r3, r6
	strb r3, [r0]
	movs r1, #6
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #3
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #9
	bl Func_02001ef0
	movs r0, #9
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02001e48
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #20
	bl Battle_WaitMode0
	b .L_02008c30
	.2byte 0x0000
.L_02008c24:
	.4byte 0x000018b4
.L_02008c28:
	.4byte Data_0200349c
.L_02008c2c:
	.4byte Data_02003418
.L_02008c30:
	movs r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #16
	ands r5, r3
	movs r2, #16
	strb r5, [r0]
	negs r1, r1
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #20
	movs r1, #6
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	ldr r5, .L_02008ed4
	orrs r3, r6
	strb r3, [r0]
	adds r1, r5, #0
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #4
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r0, #11
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_02001e28
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #10
	movs r1, #7
	bl Func_02001df8
	movs r0, #4
	movs r1, #10
	bl Object_LinkObjectAndSetCallback
	movs r0, #11
	movs r1, #10
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_02001e48
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r1, #16
	movs r0, #10
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #1
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r2, #10
	movs r0, #7
	bl Func_02001e28
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	movs r2, #8
	negs r1, r1
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r1, #0
	movs r0, #10
	bl Func_02001e28
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	adds r1, r5, #0
	orrs r6, r3
	strb r6, [r0]
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #11
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #10
	bl Object_RefreshSelectorById
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02001db8
	movs r0, #8
	movs r1, #6
	bl Object_SetModeById
	movs r0, #8
	movs r1, #0
	bl Func_02001e08
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl Func_02001e38
	movs r0, #8
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #4
	movs r1, #1
	bl Func_02001e58
	bl Func_02001e68
	movs r2, #0
	movs r0, #11
	movs r1, #4
	bl Object_LinkPair
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #11
	bl Func_02001e20
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008e40
	movs r0, #11
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl Func_02001e28
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008e64
.L_02008e40:
	movs r0, #11
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #11
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
.L_02008e64:
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl Func_02001e28
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008eb2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl ObjectMotion_ResetAndSetPosition
.L_02008eb2:
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl Func_02001db8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #17
	bl Func_02001d18
	bl Func_02001d58
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008ed4:
	.4byte Data_020033c8
	.section .text.x02008ed8,"ax",%progbits
	.global Func_02000ed8
	.thumb_func
Func_02000ed8:
	push {lr}
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	ldr r0, .L_02008f08
	bl Func_02001e18
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02001e38
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	bl Func_02001d58
	pop {pc}
	.2byte 0x0000
.L_02008f08:
	.4byte 0x000018cf
	.section .text.x02008f0c,"ax",%progbits
	.global Func_02000f0c
	.thumb_func
Func_02000f0c:
	push {lr}
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	ldr r0, .L_02008fdc
	bl Func_02001e18
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001e28
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #7
	bl Func_02001e28
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	bl Func_02001d58
	pop {pc}
.L_02008fdc:
	.4byte 0x000018d0
	.section .text.x02008fe0,"ax",%progbits
	.global Func_02000fe0
	.thumb_func
Func_02000fe0:
	push {lr}
	movs r0, #24
	movs r1, #1
	bl Func_02001e90
	movs r1, #8
	movs r0, #7
	bl Func_02001e98
	bl Func_02001eb0
	movs r0, #1
	bl Func_02001e88
	bl Func_02001ea0
	bl Func_02001ea8
	pop {pc}
	.2byte 0x0000
	.section .text.x02009008,"ax",%progbits
	.global Func_02001008
	.thumb_func
Func_02001008:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl Func_02001d18
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02001d18
	movs r0, #224
	lsls r0, r0, #1
	bl PartyInventory_Remove
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #11
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #205
	movs r2, #109
	movs r0, #11
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #190
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_02009074
	ldr r0, [r0, #8]
	cmp r0, #0
	bge .L_0200906a
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_0200906a:
	asrs r1, r0, #16
	movs r2, #95
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
.L_02009074:
	movs r1, #181
	movs r2, #95
	movs r0, #4
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02001e40
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, .L_020092f4
	bl Func_02001e18
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #3
	movs r0, #11
	bl Func_02001e48
	movs r1, #189
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #109
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #11
	movs r2, #0
	movs r0, #10
	bl Object_LinkPair
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02001e48
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r2, #0
	movs r1, #11
	movs r0, #10
	bl Func_02001df8
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02001e48
	movs r0, #25
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #25
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_020092f8
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020092fc
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, .L_02009300
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Object_RefreshSelectorById
	movs r0, #120
	bl Battle_WaitMode0
	ldr r1, .L_02009304
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #11
	movs r0, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, .L_02009308
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200930c
	movs r0, #25
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Object_RefreshSelectorById
	movs r0, #11
	bl Object_RefreshSelectorById
	movs r0, #25
	bl Object_RefreshSelectorById
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r0, #25
	movs r1, #2
	movs r2, #10
	bl Func_02001ec0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #4
	adds r3, #1
	strh r3, [r2]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02001e38
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	bl Func_02001d58
	pop {pc}
	.2byte 0x0000
.L_020092f4:
	.4byte 0x00002075
.L_020092f8:
	.4byte Data_02001f18
.L_020092fc:
	.4byte Data_02001f4c
.L_02009300:
	.4byte Data_02001f90
.L_02009304:
	.4byte Data_02001fc4
.L_02009308:
	.4byte Data_02002054
.L_0200930c:
	.4byte Data_020020e4
	.section .text.x02009310,"ax",%progbits
	.global Func_02001310
	.thumb_func
Func_02001310:
	push {lr}
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02001d18
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	movs r0, #4
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #190
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_0200934c
	ldr r0, [r0, #8]
	cmp r0, #0
	bge .L_02009342
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_02009342:
	asrs r1, r0, #16
	movs r2, #95
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
.L_0200934c:
	movs r1, #181
	movs r2, #95
	movs r0, #4
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02001e40
	ldr r0, .L_02009448
	bl Func_02001e18
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl Func_02001df8
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r0, #10
	movs r1, #0
	movs r2, #5
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #3
	movs r0, #11
	bl Func_02001e48
	movs r1, #4
	movs r2, #0
	movs r0, #11
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #5
	movs r0, #11
	bl Func_02001e28
	ldr r0, .L_0200944c
	bl Func_02001e18
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02001e48
	movs r1, #0
	movs r0, #10
	bl Func_02001e40
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #5
	bl Func_02001e28
	movs r1, #11
	movs r2, #0
	movs r0, #10
	bl Object_LinkPair
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #10
	bl Func_02001e48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #5
	bl Func_02001e28
	movs r2, #0
	movs r1, #4
	movs r0, #11
	bl Func_02001df8
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_02001e28
	bl Func_02001d58
	pop {pc}
.L_02009448:
	.4byte 0x00002075
.L_0200944c:
	.4byte 0x00002081
	.section .text.x02009450,"ax",%progbits
	.global Func_02001450
	.thumb_func
Func_02001450:
	push {lr}
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	ldr r0, .L_0200975c
	bl Func_02001e18
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_02001e48
	movs r0, #28
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r0, #28
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r0, #15
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r1, #176
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02001e38
	movs r0, #18
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r0, #14
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r0, #15
	movs r1, #4
	movs r2, #0
	bl Func_02001df8
	movs r1, #4
	movs r2, #0
	movs r0, #16
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #223
	lsls r1, r1, #1
	movs r0, #4
	adds r1, #255
	movs r2, #93
	bl ObjectMotion_SetPositionAndReset
	movs r1, #32
	movs r0, #4
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	movs r3, #128
	movs r0, #5
	negs r1, r1
	movs r2, #32
	lsls r3, r3, #8
	bl Func_02001ec8
	movs r1, #16
	movs r3, #160
	movs r0, #31
	negs r1, r1
	movs r2, #0
	lsls r3, r3, #7
	bl Func_02001ec8
	movs r1, #8
	movs r3, #192
	movs r0, #6
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #7
	bl Func_02001ec8
	movs r1, #24
	movs r3, #192
	lsls r3, r3, #7
	negs r1, r1
	movs r2, #16
	movs r0, #7
	bl Func_02001ec8
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_02001e48
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_02001e48
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #27
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #27
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_02001e48
	movs r0, #27
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #4
	movs r2, #0
	movs r0, #7
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02001e38
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #6
	movs r2, #20
	movs r0, #18
	bl ObjectMotion_Launch
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_02001e28
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #6
	bl Func_02001e40
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_02001e48
	movs r1, #0
	movs r0, #28
	bl Func_02001e20
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009690
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_02001e48
	movs r2, #10
	movs r0, #27
	movs r1, #0
	bl Func_02001e28
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02001e40
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_02001e48
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_02001e28
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #3
	strh r3, [r2]
	b .L_02009700
.L_02009690:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_02001e48
	bl Func_02001ef8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #27
	adds r3, #3
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02001e40
	movs r1, #4
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001e28
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02001e50
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
.L_02009700:
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #31
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #129
	bl Func_02001d10
	cmp r0, #0
	beq .L_02009760
	bl Func_02001910
	b .L_02009764
	.2byte 0x0000
.L_0200975c:
	.4byte 0x00002093
.L_02009760:
	bl Func_02001a20
.L_02009764:
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02001e38
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02001e40
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02001e38
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02001e38
	movs r1, #192
	movs r0, #31
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02001e38
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02001e38
	movs r2, #0
	movs r1, #0
	movs r0, #7
	bl Func_02001e38
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #31
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #31
	ldr r1, .L_0200990c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #31
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02009832
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #31
	bl ObjectMotion_ResetAndSetPosition
.L_02009832:
	movs r0, #31
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200990c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02009870
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009870:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200990c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_020098ae
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_020098ae:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200990c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_020098ec
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_020098ec:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl Func_02001db8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #137
	bl Func_02001d18
	bl Func_02001d58
	pop {pc}
.L_0200990c:
	.4byte 0x00013333
	.section .text.x02009910,"ax",%progbits
	.global Func_02001910
	.thumb_func
Func_02001910:
	push {lr}
	ldr r0, .L_02009a1c
	bl Func_02001e18
	movs r0, #28
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #31
	movs r2, #0
	movs r0, #4
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02001e38
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #31
	bl Func_02001e38
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_02001e48
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_02001e48
	movs r2, #10
	movs r0, #27
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_02001e48
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_02001e28
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	bl Func_02001e40
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	pop {pc}
	.2byte 0x0000
.L_02009a1c:
	.4byte 0x000020a2
	.section .text.x02009a20,"ax",%progbits
	.global Func_02001a20
	.thumb_func
Func_02001a20:
	push {lr}
	ldr r0, .L_02009d0c
	bl Func_02001e18
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_02001e28
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	bl Func_02001e40
	movs r1, #4
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #5
	movs r2, #0
	movs r0, #6
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_02001e28
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	bl Func_02001e40
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_02001e48
	movs r0, #7
	movs r1, #17
	movs r2, #0
	bl Func_02001df8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #7
	bl Func_02001e48
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #31
	bl Func_02001e48
	movs r0, #31
	movs r1, #7
	movs r2, #0
	bl Func_02001df8
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r2, #0
	movs r1, #31
	movs r0, #7
	bl Func_02001df8
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r0, #4
	movs r1, #7
	movs r2, #0
	bl Func_02001df8
	movs r0, #6
	movs r1, #7
	movs r2, #0
	bl Func_02001df8
	movs r1, #7
	movs r2, #0
	movs r0, #5
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_02001e48
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #6
	movs r2, #0
	movs r0, #7
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #4
	bl Func_02001e48
	movs r1, #4
	movs r2, #0
	movs r0, #7
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_02001e48
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_02001e28
	movs r1, #4
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	bl Func_02001e40
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #31
	movs r2, #0
	movs r0, #7
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	movs r2, #0
	movs r0, #7
	bl Func_02001df8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #7
	bl Func_02001e28
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #129
	bl Func_02001d18
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_02001e48
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_02001e28
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02001e40
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02001e38
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_02001e48
	movs r2, #10
	movs r0, #27
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_02001e48
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02001e38
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #17
	movs r1, #0
	bl Func_02001e28
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	bl Func_02001e40
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	pop {pc}
	.2byte 0x0000
.L_02009d0c:
	.4byte 0x000020aa
	.section .rodata.x02009f18,"a",%progbits
	.global Data_02001f18
Data_02001f18:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01640000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00490000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02001f4c
Data_02001f4c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01530000
	.4byte 0x006f0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000011
	.global Data_02001f90
Data_02001f90:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01640000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00490000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02001fc4
Data_02001fc4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x006d0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002054
Data_02002054:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x00610000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_020020e4
Data_020020e4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00180000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x00610000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002174
Data_02002174:
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
	.global Data_020021a4
Data_020021a4:
	.4byte 0x0000002d
	.4byte 0x1010202c
	.4byte 0xffffffff
	.4byte 0x1020302c
	.4byte 0xffffffff
	.4byte 0x1030402d
	.4byte 0xffffffff
	.4byte 0x1040302d
	.4byte 0xffffffff
	.4byte 0x1050702c
	.4byte 0xffffffff
	.4byte 0x1060702d
	.4byte 0xffffffff
	.4byte 0x1070602d
	.4byte 0xffffffff
	.4byte 0x1080402c
	.4byte 0xffffffff
	.4byte 0x1090802c
	.4byte 0xffffffff
	.4byte 0x10a0502c
	.4byte 0xffffffff
	.4byte 0x10b0c02d
	.4byte 0xffffffff
	.4byte 0x10c0b02d
	.4byte 0xffffffff
	.4byte 0x10d0e02d
	.4byte 0xffffffff
	.4byte 0x10e0d02d
	.4byte 0xffffffff
	.4byte 0x10f0602c
	.4byte 0xffffffff
	.4byte 0x1140902c
	.4byte 0xffffffff
	.4byte 0x0000002e
	.4byte 0x1010102c
	.4byte 0xffffffff
	.4byte 0x1020a02c
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02002240
Data_02002240:
	.4byte 0xffff0049
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00016000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001a000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x02670000
	.4byte 0x00000000
	.4byte 0x01370000
	.4byte 0x00015000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x022c0000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x0001e000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0001a000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x02240000
	.4byte 0x00012000
	.4byte 0xffff00d1
	.4byte 0x00000003
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02060000
	.4byte 0x0001e000
	.4byte 0x006200f5
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02a40000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00018000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02da0000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte Field_Map008 + 0x25db
	.4byte 0x00000001
	.4byte 0x034f0000
	.4byte 0x00000000
	.4byte 0x02c70000
	.4byte 0x00012000
	.4byte Field_Map008 + 0x25db
	.4byte 0x00000001
	.4byte 0x038a0000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002480
Data_02002480:
	.4byte 0xffff0049
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00016000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0000e000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000a000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x026c0000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000e000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x0001e000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0001a000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x02240000
	.4byte 0x00012000
	.4byte 0xffff00d1
	.4byte 0x00000003
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02060000
	.4byte 0x0001e000
	.4byte 0x006200f5
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x02660000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00002000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x027c0000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00005000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02a40000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00018000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02da0000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002750
Data_02002750:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x020a0000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte Field_Map008 + 0x24f3
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x016a0000
	.4byte 0x00000000
	.4byte 0x034f0000
	.4byte 0x0002c000
	.4byte 0x18ff00bb
	.4byte 0x00000001
	.4byte 0x018a0000
	.4byte 0x00000000
	.4byte 0x035f0000
	.4byte 0x0001a000
	.4byte 0x18ff00bb
	.4byte 0x00000001
	.4byte 0x018c0000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002828
Data_02002828:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00010000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00014000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x01640000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00024000
	.4byte 0x18ab00ba
	.4byte 0x00000001
	.4byte 0x01650000
	.4byte 0x00000000
	.4byte 0x00830000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020028a0
Data_020028a0:
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
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000189b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020000a4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000018a1
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte 0x000018a2
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000018a5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000018a6
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000018a9
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000018ab
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000018ac
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000018ad
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000018ae
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001b8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000018e3
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000018e4
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000018ff
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001900
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001901
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001902
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x00002f59
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00002f5a
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000189f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000018a0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000018a3
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000018a4
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000018a7
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000018a8
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000018af
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000018b0
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000018b1
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000018b2
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000018b3
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000018e5
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000018e6
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000018e7
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001903
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001904
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001905
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001906
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001907
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x00002f5b
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x00002f5c
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403043
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403044
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403045
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403046
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x00403047
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002bb8
Data_02002bb8:
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000000
	.4byte 0x18ff0009
	.4byte 0x00002114
	.4byte 0x00000000
	.4byte 0x18ff000a
	.4byte 0x00002115
	.4byte 0x0000c400
	.4byte 0x18ff000d
	.4byte 0x00002051
	.4byte 0x0000a400
	.4byte 0x18ff000d
	.4byte 0x00002051
	.4byte 0x00008400
	.4byte 0x18ff000d
	.4byte 0x00002051
	.4byte 0x00000000
	.4byte 0x18ff000e
	.4byte Func_02000428
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001d48
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001d49
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte Func_02000ed8
	.4byte 0x00008d15
	.4byte 0x18ff0009
	.4byte 0x00002116
	.4byte 0x00008d15
	.4byte 0x18ff000a
	.4byte 0x00002117
	.4byte 0x00008d15
	.4byte 0x18ff000d
	.4byte 0x00002067
	.4byte 0x00008d15
	.4byte 0x1211000e
	.4byte 0x00002068
	.4byte 0x00008d15
	.4byte 0x18ff040e
	.4byte Func_02000428
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001d4a
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d4b
	.4byte 0x00008d15
	.4byte 0xffff0007
	.4byte Func_02000f0c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002c9c
Data_02002c9c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte Field_Map008 + 0x24f4
	.4byte 0x00001897
	.4byte 0x00000000
	.4byte 0x08ff0008
	.4byte 0x00001d2e
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002069
	.4byte 0x00000000
	.4byte Field_Map008 + 0x24f5
	.4byte 0x00001898
	.4byte 0x00000000
	.4byte 0x08ff0009
	.4byte 0x00001d2f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000206a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000018df
	.4byte 0x00000000
	.4byte 0x08ff000b
	.4byte 0x00001d51
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000211d
	.4byte 0x00008d15
	.4byte Field_Map008 + 0x24f4
	.4byte 0x00001899
	.4byte 0x00008d15
	.4byte 0x08ff0008
	.4byte 0x00001d30
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000206b
	.4byte 0x00008d15
	.4byte Field_Map008 + 0x24f5
	.4byte 0x0000189a
	.4byte 0x00008d15
	.4byte 0x08ff0009
	.4byte 0x00001d31
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000206c
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000018e1
	.4byte 0x00008d15
	.4byte 0x08ff000b
	.4byte 0x00001d53
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000211f
	.4byte 0x00000173
	.4byte 0xffff00cd
	.4byte 0x00403048
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002da4
Data_02002da4:
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
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001d32
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001d33
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001d36
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001d37
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001d3a
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001d3b
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001d3e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001d3f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001d40
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001d41
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001d42
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001b8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d55
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001d56
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001d5b
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001d5c
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00001d5d
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00001d5e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001d34
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001d35
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d38
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001d39
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001d3c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d3d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001d43
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001d44
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001d45
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001d46
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001d47
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001d57
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001d58
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001d59
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001d5f
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001d60
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001d61
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001d62
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001d63
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403043
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403044
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403045
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403046
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x00403047
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200308c
Data_0200308c:
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
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x0989001e
	.4byte Func_02001450
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000206d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000206e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200028c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000308
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002088
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002089
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x000020bf
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x000020c0
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000020c1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000020c2
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000020c3
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000020c4
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000020c5
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001b8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002121
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002122
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002129
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000212a
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte 0x0000212b
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte 0x0000212c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000206f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002070
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte Func_02000368
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte Func_020003c8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000208a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000208b
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x000020c6
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x000020c7
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000020c8
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000020c9
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000020ca
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000020cb
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000020cc
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002123
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002124
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002125
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000212d
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x0000212e
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000212f
	.4byte 0x00008d15
	.4byte 0xffff0020
	.4byte 0x00002130
	.4byte 0x00008d15
	.4byte 0xffff0021
	.4byte 0x00002131
	.4byte 0x0001c014
	.4byte 0x098a000a
	.4byte Func_02001008
	.4byte 0x0001c114
	.4byte 0x098a000a
	.4byte Func_02001310
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403043
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403044
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403045
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403046
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x00403047
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020033c8
Data_020033c8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02003418
Data_02003418:
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200349c
Data_0200349c:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000064
	.4byte 0x00000000
	.4byte 0x00000024
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00030000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00050000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00050000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
