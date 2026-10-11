.syntax unified
	.thumb
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
	bl GameFlag_Test
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
	bl UiText_OpenMessageAtObject
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
	.4byte MsgScene659No06
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	.ifdef TLA_EDITION_JA
	push {lr}
	ldr r3, .L_ja_02008148
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3, #0]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_ja_02008144
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_ja_0200812c
	movs r0, #1
	movs r1, #22
	bl Func_02001f08
	b .L_ja_02008174
.L_ja_0200812c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008150
	ldr r0, .L_ja_0200814c
	bl Func_02001e18
	b .L_ja_0200816c
	.2byte 0x0000
.L_ja_02008144:
	.4byte 0xffffc000
.L_ja_02008148:
	.4byte gPartyState
.L_ja_0200814c:
	.4byte 0x00002253
.L_ja_02008150:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008166
	ldr r0, .L_ja_02008178
	bl Func_02001e18
	b .L_ja_0200816c
.L_ja_02008166:
	ldr r0, .L_ja_0200817c
	bl Func_02001e18
.L_ja_0200816c:
	movs r0, #22
	movs r1, #0
	bl Func_02001e30
.L_ja_02008174:
	pop {pc}
	.2byte 0x0000
.L_ja_02008178:
	.4byte 0x00001e6c
.L_ja_0200817c:
	.4byte 0x00001a0f
	.else
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
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008184
	ldr r5, .L_020081ac
	adds r0, r5, #0
	bl Func_02001e18
	movs r1, #0
	movs r0, #22
	bl UiText_OpenMessageAtObject
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
	bl GameFlag_Test
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
	.4byte MsgScene659No155
.L_020081b0:
	.4byte MsgScene659No87
.L_020081b4:
	.4byte MsgScene659No39
	.endif
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
	bl GameFlag_Test
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
	.4byte MsgScene659No149
.L_0200820c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
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
	.4byte MsgScene659No81
.L_02008238:
	.4byte MsgScene659No33
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
	bl ObjectMotion_SetAngleToward
	movs r0, #14
	movs r1, #6
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #0
	movs r1, #14
	movs r0, #4
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_02008288
	bl Func_02001e18
	movs r0, #14
	movs r1, #0
	bl Func_02001e30
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02001d58
	pop {pc}
	.2byte 0x0000
.L_02008288:
	.4byte MsgScene659No18
	.section .text.x0200828c,"ax",%progbits
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082bc
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	.4byte MsgScene659No111
.L_020082fc:
	.4byte MsgScene659No118
.L_02008300:
	.4byte MsgScene659No116
.L_02008304:
	.4byte MsgScene659No106
	.section .text.x02008308,"ax",%progbits
	.global Func_02000308
	.thumb_func
Func_02000308:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200832c
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	.4byte MsgScene659No112
.L_0200835c:
	.4byte MsgScene659No119
.L_02008360:
	.4byte MsgScene659No117
.L_02008364:
	.4byte MsgScene659No107
	.section .text.x02008368,"ax",%progbits
	.global Func_02000368
	.thumb_func
Func_02000368:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200838c
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	.4byte MsgScene659No113
.L_020083c0:
	.4byte MsgScene659No120
.L_020083c4:
	.4byte MsgScene659No108
	.section .text.x020083c8,"ax",%progbits
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083ec
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	.4byte MsgScene659No114
.L_02008420:
	.4byte MsgScene659No121
.L_02008424:
	.4byte MsgScene659No109
	.section .text.x02008428,"ax",%progbits
	.global Func_02000428
	.thumb_func
Func_02000428:
	push {lr}
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084a0
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
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
	bl ObjectMotion_SetAngleToward
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
	bl ObjectMotion_SetAngleToward
	movs r0, #14
	movs r1, #0
	bl Func_02001e30
.L_020084b8:
	pop {pc}
	.2byte 0x0000
.L_020084bc:
	.4byte MsgUhhhnnnUhhhnnn2
.L_020084c0:
	.4byte MsgScene659No97
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
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008500
	ldr r0, .L_02008528
	b .L_02008514
.L_02008500:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	bl ObjectDispatch_SetSingleChildFlags
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
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	.4byte gEwramHeap
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
	bl GameFlag_Test
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
	bl Object_SetPartAttribute
	movs r0, #30
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
.L_020086b2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
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
	bl GameFlag_Test
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
	bl ObjectDispatch_SetSingleChildFlags
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
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200878a
	movs r0, #33
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
	b .L_02008796
.L_0200878a:
	movs r0, #28
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
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
	.section .text.x02009008,"ax",%progbits
	.balign 4
	.global Func_02001008
	.thumb_func
Func_02001008:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_SetBit
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_SetBit
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
	bl ObjectMotion_SetAngleToward
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
	bl ObjectMotion_ArmCallback
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
	bl ObjectMotion_SetAngleToward
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
	bl ObjectMotion_SetAngleToward
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
	bl ObjectMotion_ArmCallback
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
	bl ObjectMotion_ArmCallback
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
	.4byte MsgScene659No110
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
