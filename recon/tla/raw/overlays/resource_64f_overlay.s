.syntax unified
	.thumb
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
	.section .text.x020083ac,"ax",%progbits
	.balign 4
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
	.4byte MsgWaahhhItS
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
	.4byte MsgHaThatWas
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
