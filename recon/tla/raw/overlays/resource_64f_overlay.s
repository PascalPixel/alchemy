.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r0, #13
	movs r1, #26
	bl Func_02000a48
	pop {pc}
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	ldr r3, .L_0200806c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008070
	cmp r2, r3
	bne .L_0200805c
	ldr r0, .L_02008074
	b .L_02008068
.L_0200805c:
	ldr r3, .L_02008078
	cmp r2, r3
	bne .L_02008066
	ldr r0, .L_0200807c
	b .L_02008068
.L_02008066:
	ldr r0, .L_02008080
.L_02008068:
	pop {pc}
	.2byte 0x0000
.L_0200806c:
	.4byte gPartyState
.L_02008070:
	.4byte 0x00000007
.L_02008074:
	.4byte Data_02000d98
.L_02008078:
	.4byte 0x00000008
.L_0200807c:
	.4byte Data_02000e40
.L_02008080:
	.4byte Data_02000cd8
	.section .text.x02008084,"ax",%progbits
	.global Func_02000084
	.thumb_func
Func_02000084:
	movs r0, #0
	bx lr
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	ldr r0, .L_0200808c
	bx lr
.L_0200808c:
	.4byte Data_02000ed0
	.section .text.x02008090,"ax",%progbits
	.global Func_02000090
	.thumb_func
Func_02000090:
	push {lr}
	ldr r3, .L_020080b8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080bc
	cmp r2, r3
	bne .L_020080a8
	ldr r0, .L_020080c0
	b .L_020080b4
.L_020080a8:
	ldr r3, .L_020080c4
	cmp r2, r3
	bne .L_020080b2
	ldr r0, .L_020080c8
	b .L_020080b4
.L_020080b2:
	ldr r0, .L_020080cc
.L_020080b4:
	pop {pc}
	.2byte 0x0000
.L_020080b8:
	.4byte gPartyState
.L_020080bc:
	.4byte 0x00000007
.L_020080c0:
	.4byte Data_02000f58
.L_020080c4:
	.4byte 0x00000006
.L_020080c8:
	.4byte Data_02001048
.L_020080cc:
	.4byte Data_02000f10
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {lr}
	ldr r3, .L_020080f8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080fc
	cmp r2, r3
	bne .L_020080e8
	ldr r0, .L_02008100
	b .L_020080f4
.L_020080e8:
	ldr r3, .L_02008104
	cmp r2, r3
	bne .L_020080f2
	ldr r0, .L_02008108
	b .L_020080f4
.L_020080f2:
	ldr r0, .L_0200810c
.L_020080f4:
	pop {pc}
	.2byte 0x0000
.L_020080f8:
	.4byte gPartyState
.L_020080fc:
	.4byte 0x00000007
.L_02008100:
	.4byte Data_020010e4
.L_02008104:
	.4byte 0x00000008
.L_02008108:
	.4byte Data_02001180
.L_0200810c:
	.4byte Data_02001078
	.section .text.x02008110,"ax",%progbits
	.global Func_02000110
	.thumb_func
Func_02000110:
	push {lr}
	ldr r3, .L_02008164
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200812c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #148
	bl GameFlag_SetBit
.L_0200812c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #148
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008160
	movs r1, #228
	movs r2, #156
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020009d0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #8
	movs r2, #0
	bl Func_02000a98
	movs r0, #8
	bl Object_GetById
	movs r1, #8
	bl Object_SetActionCallback
.L_02008160:
	pop {pc}
	.2byte 0x0000
.L_02008164:
	.4byte gPartyState
	.section .text.x02008168,"ax",%progbits
	.global Func_02000168
	.thumb_func
Func_02000168:
	push {r5, lr}
	sub sp, #8
	movs r3, #102
	movs r2, #56
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #108
	movs r1, #38
	bl Func_02000978
	ldr r3, .L_02008258
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200819a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #149
	bl GameFlag_SetBit
.L_0200819a:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #149
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200822a
	movs r2, #230
	movs r0, #13
	ldr r1, .L_0200825c
	lsls r2, r2, #18
	bl Func_020009d0
	movs r1, #156
	movs r0, #14
	lsls r1, r1, #18
	ldr r2, .L_02008260
	bl Func_020009d0
	movs r1, #158
	movs r2, #230
	lsls r2, r2, #18
	lsls r1, r1, #18
	movs r0, #15
	bl Func_020009d0
	movs r0, #13
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #192
	strh r3, [r0, #6]
	lsls r1, r1, #7
	movs r0, #13
	bl Func_02000a90
	movs r0, #14
	bl Object_GetById
	movs r5, #0
	strh r5, [r0, #6]
	movs r1, #0
	movs r0, #14
	bl Func_02000a90
	movs r0, #15
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r1, #0
	movs r0, #15
	bl Func_02000a90
	movs r0, #13
	bl Object_GetById
	movs r5, #1
	adds r0, #89
	strb r5, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #89
	strb r5, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #89
	strb r5, [r0]
.L_0200822a:
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_02008258:
	.4byte gPartyState
.L_0200825c:
	.4byte 0x024e0000
.L_02008260:
	.4byte 0x03ab0000
	.section .text.x02008264,"ax",%progbits
	.global Func_02000264
	.thumb_func
Func_02000264:
	push {lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008286
	ldr r3, .L_02008288
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_02008286
	bl Func_02000800
.L_02008286:
	pop {pc}
.L_02008288:
	.4byte gPartyState
	.section .text.x0200828c,"ax",%progbits
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	str r2, [r3]
	ldr r3, .L_020082d0
	adds r2, #224
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020082d4
	cmp r2, r3
	bne .L_020082b6
	bl Func_02000110
	b .L_020082cc
.L_020082b6:
	ldr r3, .L_020082d8
	cmp r2, r3
	bne .L_020082c2
	bl Func_02000168
	b .L_020082cc
.L_020082c2:
	ldr r3, .L_020082dc
	cmp r2, r3
	bne .L_020082cc
	bl Func_02000264
.L_020082cc:
	movs r0, #0
	pop {pc}
.L_020082d0:
	.4byte gPartyState
.L_020082d4:
	.4byte 0x00000006
.L_020082d8:
	.4byte 0x00000007
.L_020082dc:
	.4byte 0x00000008
	.section .text.x020082e0,"ax",%progbits
	.global Func_020002e0
	.thumb_func
Func_020002e0:
	push {lr}
	sub sp, #8
	movs r3, #102
	movs r2, #56
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #38
	movs r2, #1
	movs r3, #1
	movs r0, #108
	bl Func_02000978
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008308,"ax",%progbits
	.global Func_02000308
	.thumb_func
Func_02000308:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	sub sp, #8
	movs r2, #0
	ldrsh r6, [r3, r2]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #158
	bl Func_02000aa0
	movs r5, #2
	movs r1, #36
	movs r2, #71
	movs r3, #8
	movs r0, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000970
	movs r0, #4
	bl WaitFrames
	movs r3, #8
	movs r1, #36
	movs r2, #71
	movs r0, #68
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000970
	movs r0, #4
	bl WaitFrames
	movs r2, #16
	movs r1, #3
	negs r2, r2
	movs r0, #0
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #123
	bl Func_02000aa0
	adds r0, r6, #0
	bl Func_02000a38
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008378,"ax",%progbits
	.global Func_02000378
	.thumb_func
Func_02000378:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	ldr r3, .L_020083a4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #4
	str r2, [r3]
	movs r0, #123
	bl Func_02000aa0
	adds r0, r5, #0
	bl Func_02000a38
	pop {r5, pc}
.L_020083a4:
	.4byte gPartyState
	.section .text.x020083a8,"ax",%progbits
	.global Func_020003a8
	.thumb_func
Func_020003a8:
	movs r0, #0
	bx lr
	.section .text.x020083ac,"ax",%progbits
	.global Func_020003ac
	.thumb_func
Func_020003ac:
	push {r5, r6, lr}
	bl Func_02000988
	movs r0, #0
	bl Func_02000a70
	ldr r0, .L_02008484
	bl Func_02000a00
	ldr r6, .L_02008488
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r6, r3
	movs r1, #236
	movs r2, #160
	ldr r0, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #228
	movs r1, #1
	movs r2, #140
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02000a30
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #20
	movs r0, #8
	bl ObjectMotion_Launch
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_02000a08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #8
	bl Func_02000a18
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_02000a08
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_02000a08
	movs r1, #228
	movs r2, #156
	lsls r2, r2, #1
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, .L_0200848c
	movs r1, #99
	bl Party_SetFields1eeAnd1f0
	ldr r0, .L_02008490
	movs r1, #98
	bl Party_SetFields1f2And1f4
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r6, r6, r3
	movs r3, #2
	strb r3, [r6]
	movs r0, #9
	movs r1, #1
	bl Func_02000a40
	bl Func_02000990
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008484:
	.4byte 0x00001609
.L_02008488:
	.4byte gPartyState
.L_0200848c:
	.4byte 0x00000006
.L_02008490:
	.4byte 0x00000004
	.section .text.x02008494,"ax",%progbits
	.global Func_02000494
	.thumb_func
Func_02000494:
	push {r5, r6, lr}
	bl Func_02000988
	movs r0, #0
	bl Func_02000a70
	ldr r0, .L_020085d8
	bl Func_02000a00
	ldr r6, .L_020085dc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r6, r3
	movs r2, #4
	movs r1, #0
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	ldr r0, [r5]
	movs r2, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #154
	movs r2, #228
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020009d0
	movs r1, #150
	movs r2, #230
	movs r0, #13
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #154
	movs r2, #228
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020009d0
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #158
	movs r2, #230
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #154
	movs r2, #228
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020009d0
	movs r1, #154
	movs r2, #230
	lsls r2, r2, #2
	movs r0, #14
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_02000a08
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_02000a08
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_02000a08
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r0, .L_020085e0
	movs r1, #99
	bl Party_SetFields1eeAnd1f0
	ldr r0, .L_020085e4
	movs r1, #98
	bl Party_SetFields1f2And1f4
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r6, r6, r3
	movs r3, #2
	strb r3, [r6]
	movs r0, #9
	movs r1, #2
	bl Func_02000a40
	bl Func_02000990
	pop {r5, r6, pc}
.L_020085d8:
	.4byte 0x0000160d
.L_020085dc:
	.4byte gPartyState
.L_020085e0:
	.4byte 0x00000007
.L_020085e4:
	.4byte 0x00000004
	.section .text.x020085e8,"ax",%progbits
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	push {r5, lr}
	movs r0, #8
	movs r2, #0
	movs r1, #4
	bl ObjectMotion_Launch
	ldr r5, .L_02008644
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008614
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_02008614:
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02008648
	movs r1, #99
	bl Party_SetFields1eeAnd1f0
	ldr r0, .L_0200864c
	movs r1, #98
	bl Party_SetFields1f2And1f4
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r5, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #9
	movs r1, #3
	bl Func_02000a40
	bl Func_02000990
	pop {r5, pc}
.L_02008644:
	.4byte gPartyState
.L_02008648:
	.4byte 0x00000008
.L_0200864c:
	.4byte 0x00000004
	.section .text.x02008650,"ax",%progbits
	.global Func_02000650
	.thumb_func
Func_02000650:
	push {r5, lr}
	bl Func_02000988
	movs r0, #0
	bl Func_02000a70
	movs r1, #236
	movs r2, #240
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #8
	bl Func_020009d0
	movs r0, #16
	bl Battle_WaitMode0
	ldr r5, .L_02008718
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02000a18
	movs r0, #8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02000a30
	movs r1, #4
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_Launch
	movs r0, #16
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #240
	movs r2, #236
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #2
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #248
	movs r2, #232
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #244
	movs r2, #228
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #248
	movs r2, #224
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020005e8
	pop {r5, pc}
	.2byte 0x0000
.L_02008718:
	.4byte gPartyState
	.section .text.x0200871c,"ax",%progbits
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	push {r5, lr}
	bl Func_02000988
	movs r0, #0
	bl Func_02000a70
	movs r1, #236
	movs r2, #240
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #8
	bl Func_020009d0
	movs r0, #16
	bl Battle_WaitMode0
	ldr r5, .L_020087fc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02000a18
	movs r0, #16
	bl Battle_WaitMode0
	movs r1, #244
	movs r2, #220
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #16
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02000a30
	movs r1, #4
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_Launch
	movs r0, #16
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #244
	movs r2, #240
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #2
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #252
	movs r2, #236
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #130
	movs r2, #232
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #132
	movs r2, #228
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020005e8
	pop {r5, pc}
.L_020087fc:
	.4byte gPartyState
	.section .text.x02008800,"ax",%progbits
	.global Func_02000800
	.thumb_func
Func_02000800:
	push {r5, lr}
	bl Func_02000988
	movs r0, #0
	bl Func_02000a70
	ldr r0, .L_020088c4
	bl Func_02000a00
	movs r1, #128
	movs r2, #216
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009d0
	ldr r5, .L_020088c8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #132
	movs r2, #216
	ldr r0, [r5]
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009d0
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_02000a08
	movs r1, #3
	ldr r0, [r5]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_02000a08
	movs r1, #3
	ldr r0, [r5]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #150
	bl GameFlag_SetBit
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_020088a6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_020088a6:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_020009d0
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000990
	pop {r5, pc}
	.2byte 0x0000
.L_020088c4:
	.4byte 0x00001613
.L_020088c8:
	.4byte gPartyState
	.section .text.x020088cc,"ax",%progbits
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {r5, lr}
	bl Func_02000988
	movs r0, #0
	bl Func_02000a70
	ldr r0, .L_02008950
	bl Func_02000a00
	movs r1, #16
	movs r3, #0
	negs r1, r1
	movs r2, #0
	movs r0, #16
	bl Func_02000a78
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #16
	movs r1, #0
	bl Func_02000a08
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02008954
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008928
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl ObjectMotion_ResetAndSetPosition
.L_02008928:
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_020009d0
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	bl Func_02000990
	pop {r5, pc}
	.2byte 0x0000
.L_02008950:
	.4byte 0x00001616
.L_02008954:
	.4byte gPartyState
	.section .rodata.x02008aa8,"a",%progbits
	.4byte 0x80010000
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
	.4byte 0x80010000
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
	.4byte 0x00000008
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x80010000
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
	.4byte 0x80010000
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
	.4byte 0x00000008
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global Data_02000cd8
Data_02000cd8:
	.4byte 0xffff0000
	.4byte 0x00000098
	.4byte 0x400001f8
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0001
	.4byte 0x00000098
	.4byte 0xc0000208
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000118
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0x40000130
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0004
	.4byte 0x00000168
	.4byte 0x00000128
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0xffff0005
	.4byte 0x00000298
	.4byte 0x800001a8
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0xffff0063
	.4byte 0x000001d8
	.4byte 0x80000138
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000d98
Data_02000d98:
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000130
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x00000188
	.4byte 0x80000170
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0003
	.4byte 0x00000028
	.4byte 0x00000390
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0004
	.4byte 0x000002a8
	.4byte 0x80000388
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0005
	.4byte 0x00000268
	.4byte 0x40000398
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0063
	.4byte 0x00000268
	.4byte 0x40000398
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000e40
Data_02000e40:
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc0000108
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000b8
	.4byte 0x400000a8
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0003
	.4byte 0x00000118
	.4byte 0x40000158
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0xffff0004
	.4byte 0x00000298
	.4byte 0xc0000268
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0xffff0063
	.4byte 0x00000210
	.4byte 0x400001b0
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000ed0
Data_02000ed0:
	.4byte 0x00000006
	.4byte 0x00201007
	.4byte 0x00404007
	.4byte 0x00502004
	.4byte 0x00000007
	.4byte 0x00102006
	.4byte 0x00203007
	.4byte 0x00302007
	.4byte 0x00404006
	.4byte 0x00501008
	.4byte 0x00000008
	.4byte 0x00105007
	.4byte 0x00203008
	.4byte 0x00302008
	.4byte 0x0040b009
	.4byte 0x000001ff
	.global Data_02000f10
Data_02000f10:
	.4byte 0xffff00f7
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000f58
Data_02000f58:
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00025000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00022000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00022000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00022000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001048
Data_02001048:
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001078
Data_02001078:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02000308
	.4byte 0x00000002
	.4byte WorldMap_TilesA3 + 0x7d8
	.4byte Func_020003ac
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte Func_020008cc
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000160c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020010e4
Data_020010e4:
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
	.4byte 0x00000002
	.4byte Resource_Data1B7 + 0x1e1
	.4byte Func_02000494
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_020008cc
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001610
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001611
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001612
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000038
	.4byte 0x00000013
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001180
Data_02001180:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000378
	.4byte 0x00000002
	.4byte Ui_Icons + 0x44be
	.4byte Func_02000650
	.4byte 0x00000002
	.4byte Ui_Icons + 0x44bf
	.4byte Func_0200071c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
