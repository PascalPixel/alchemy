.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_02008074
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008078
	cmp r2, r3
	bne .L_02008064
	ldr r0, .L_0200807c
	b .L_02008070
.L_02008064:
	ldr r3, .L_02008080
	cmp r2, r3
	bne .L_0200806e
	ldr r0, .L_02008084
	b .L_02008070
.L_0200806e:
	ldr r0, .L_02008088
.L_02008070:
	pop {pc}
	.2byte 0x0000
.L_02008074:
	.4byte gPartyState
.L_02008078:
	.4byte 0x0000008d
.L_0200807c:
	.4byte Data_02000ca4
.L_02008080:
	.4byte 0x0000008e
.L_02008084:
	.4byte Data_02000dac
.L_02008088:
	.4byte Data_02000c8c
	.section .text.x02008204,"ax",%progbits
	.global Func_02000204
	.thumb_func
Func_02000204:
	push {lr}
	ldr r3, .L_0200822c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008230
	cmp r2, r3
	bne .L_0200821c
	ldr r0, .L_02008234
	b .L_02008228
.L_0200821c:
	ldr r3, .L_02008238
	cmp r2, r3
	bne .L_02008226
	ldr r0, .L_0200823c
	b .L_02008228
.L_02008226:
	ldr r0, .L_02008240
.L_02008228:
	pop {pc}
	.2byte 0x0000
.L_0200822c:
	.4byte gPartyState
.L_02008230:
	.4byte 0x0000008d
.L_02008234:
	.4byte Data_02000ea8
.L_02008238:
	.4byte 0x0000008e
.L_0200823c:
	.4byte Data_02000fe0
.L_02008240:
	.4byte Data_02000e84
	.section .text.x02008244,"ax",%progbits
	.global Func_02000244
	.thumb_func
Func_02000244:
	push {r5, lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r5, .L_02008298
	adds r0, r5, #0
	bl Func_02000a18
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	bl Func_02000a58
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200827c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000a18
	b .L_02008288
.L_0200827c:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000a18
.L_02008288:
	movs r0, #9
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	pop {r5, pc}
	.2byte 0x0000
.L_02008298:
	.4byte MsgScene677No10
	.section .text.x0200829c,"ax",%progbits
	.global Func_0200029c
	.thumb_func
Func_0200029c:
	push {r5, lr}
	ldr r3, .L_020082d4
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
	ldr r2, .L_020082d0
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020082d8
	movs r0, #20
	adds r1, r5, #0
	bl Func_02000a68
	b .L_020082f4
	.2byte 0x0000
.L_020082d0:
	.4byte 0xffffc000
.L_020082d4:
	.4byte gPartyState
.L_020082d8:
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_020082f8
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
.L_020082f4:
	pop {r5, pc}
	.2byte 0x0000
.L_020082f8:
	.4byte MsgScene677No21
	.section .text.x020082fc,"ax",%progbits
	.global Func_020002fc
	.thumb_func
Func_020002fc:
	push {r5, lr}
	ldr r3, .L_02008334
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
	ldr r2, .L_02008330
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008338
	movs r0, #21
	adds r1, r5, #0
	bl Func_02000a68
	b .L_02008354
	.2byte 0x0000
.L_02008330:
	.4byte 0xffffc000
.L_02008334:
	.4byte gPartyState
.L_02008338:
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_02008358
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
.L_02008354:
	pop {r5, pc}
	.2byte 0x0000
.L_02008358:
	.4byte MsgScene677No23
	.section .text.x0200835c,"ax",%progbits
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {r5, lr}
	ldr r3, .L_02008394
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
	ldr r2, .L_02008390
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008398
	movs r0, #22
	adds r1, r5, #0
	bl Func_02000a68
	b .L_020083b4
	.2byte 0x0000
.L_02008390:
	.4byte 0xffffc000
.L_02008394:
	.4byte gPartyState
.L_02008398:
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_020083b8
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
.L_020083b4:
	pop {r5, pc}
	.2byte 0x0000
.L_020083b8:
	.4byte MsgScene677No25
	.section .text.x020083bc,"ax",%progbits
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, lr}
	ldr r3, .L_020083fc
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
	ldr r2, .L_020083f8
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008400
	movs r0, #7
	adds r1, r5, #0
	bl Func_02000a78
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	b .L_0200841c
.L_020083f8:
	.4byte 0xffffc000
.L_020083fc:
	.4byte gPartyState
.L_02008400:
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_02008420
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
.L_0200841c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008420:
	.4byte MsgScene677No27
	.section .text.x02008424,"ax",%progbits
	.global Func_02000424
	.thumb_func
Func_02000424:
	push {r5, lr}
	ldr r3, .L_02008458
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
	ldr r2, .L_02008454
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200845c
	adds r0, r5, #0
	bl Func_02000a70
	b .L_02008478
.L_02008454:
	.4byte 0xffffc000
.L_02008458:
	.4byte gPartyState
.L_0200845c:
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_0200847c
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
.L_02008478:
	pop {r5, pc}
	.2byte 0x0000
.L_0200847c:
	.4byte MsgScene65ENo27
	.section .text.x02008480,"ax",%progbits
	.global Func_02000480
	.thumb_func
Func_02000480:
	push {r5, lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	movs r1, #2
	movs r0, #14
	bl ObjectMotion_SetVariantCallback
	ldr r5, .L_020084dc
	adds r0, r5, #0
	bl Func_02000a18
	movs r1, #0
	movs r0, #14
	bl UiText_OpenMessageAtObject
	bl Func_02000a58
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020084c0
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000a18
	b .L_020084cc
.L_020084c0:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000a18
.L_020084cc:
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	pop {r5, pc}
	.2byte 0x0000
.L_020084dc:
	.4byte MsgScene677No31
	.section .text.x020084e0,"ax",%progbits
	.global Func_020004e0
	.thumb_func
Func_020004e0:
	push {r5, lr}
	movs r5, #174
	adds r5, #255
.L_020084e6:
	adds r0, r5, #0
	bl PartyInventory_FindOwner
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_02008510
	movs r3, #182
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	ble .L_020084e6
	movs r5, #162
	adds r5, #255
.L_02008502:
	adds r0, r5, #0
	bl PartyInventory_FindOwner
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02008514
.L_02008510:
	movs r0, #1
	b .L_02008520
.L_02008514:
	movs r3, #172
	adds r5, #1
	adds r3, #255
	cmp r5, r3
	ble .L_02008502
	movs r0, #0
.L_02008520:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008524,"ax",%progbits
	.global Func_02000524
	.thumb_func
Func_02000524:
	.ifdef TLA_EDITION_JA
	push {r5, lr}
	bl Func_020004e0
	cmp r0, #0
	beq .L_ja_02008538
	movs r0, #16
	adds r0, #255
	bl GameFlag_SetBit
	b .L_ja_02008556
.L_ja_02008538:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02008556
	bl Func_02000a90
	cmp r0, #0
	bne .L_ja_02008556
	movs r0, #16
	adds r0, #255
	bl GameFlag_ClearBit
.L_ja_02008556:
	movs r0, #16
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02008598
	ldr r5, .L_ja_02008760
	adds r0, r5, #0
	bl Func_02000a18
	movs r1, #0
	movs r0, #14
	bl UiText_OpenMessageAtObject
	bl Func_02000a58
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_ja_0200858a
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	b .L_ja_020085b4
.L_ja_0200858a:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000a18
	b .L_ja_020085b8
.L_ja_02008598:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020085aa
	ldr r0, .L_ja_02008764
	b .L_ja_020085b4
.L_ja_020085aa:
	bl Func_02000a90
	cmp r0, #0
	beq .L_ja_020085c2
	ldr r0, .L_ja_02008768
.L_ja_020085b4:
	bl Func_02000a18
.L_ja_020085b8:
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
	b .L_ja_0200875c
.L_ja_020085c2:
	ldr r5, .L_ja_0200876c
	adds r0, r5, #0
	bl Func_02000a18
	movs r1, #0
	movs r0, #14
	bl UiText_OpenMessageAtObject
	bl Func_02000a58
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_ja_020085e2
	b .L_ja_0200874e
.L_ja_020085e2:
	adds r0, r5, #1
	bl Func_02000a18
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02000a60
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	ldr r5, .L_ja_02008770
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #244
	movs r2, #204
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r5, #0]
	bl ObjectMotion_ResetAndSetPosition
	ldr r0, [r5, #0]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5, #0]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #252
	movs r2, #204
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r1, [r5, #0]
	movs r0, #14
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #14
	bl Func_02000a80
	bl Func_02000a90
	cmp r0, #0
	beq .L_ja_02008718
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #14
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #142
	movs r2, #204
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #142
	movs r2, #180
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #158
	movs r2, #173
	movs r0, #16
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	bl Object_SetModeById
	movs r1, #150
	movs r2, #180
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	bl Object_SetModeById
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl Object_GetById
	movs r3, #129
	adds r0, #89
	strb r3, [r0, #0]
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	adds r0, #35
	strb r3, [r0, #0]
	movs r1, #1
	movs r0, #14
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0, #0]
	movs r5, #1
	orrs r5, r3
	strb r5, [r0, #0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	b .L_ja_0200875c
.L_ja_02008718:
	movs r1, #236
	movs r2, #199
	adds r1, #255
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #14
	movs r1, #0
	bl Func_02000a60
	b .L_ja_0200875c
.L_ja_0200874e:
	adds r0, r5, #2
	bl Func_02000a18
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
.L_ja_0200875c:
	pop {r5, pc}
	.2byte 0x0000
.L_ja_02008760:
	.4byte 0x000012f9
.L_ja_02008764:
	.4byte 0x000012ff
.L_ja_02008768:
	.4byte 0x00001300
.L_ja_0200876c:
	.4byte 0x000012fc
.L_ja_02008770:
	.4byte gPartyState
	.else
	push {r5, lr}
	bl Func_020004e0
	cmp r0, #0
	beq .L_02008538
	movs r0, #16
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008556
.L_02008538:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008556
	bl Func_02000a90
	cmp r0, #0
	bne .L_02008556
	movs r0, #16
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008556:
	movs r0, #16
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008598
	ldr r5, .L_02008768
	adds r0, r5, #0
	bl Func_02000a18
	movs r1, #0
	movs r0, #14
	bl UiText_OpenMessageAtObject
	bl Func_02000a58
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200858a
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	b .L_020085b4
.L_0200858a:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000a18
	b .L_020085b8
.L_02008598:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085aa
	ldr r0, .L_0200876c
	b .L_020085b4
.L_020085aa:
	bl Func_02000a90
	cmp r0, #0
	beq .L_020085c2
	ldr r0, .L_02008770
.L_020085b4:
	bl Func_02000a18
.L_020085b8:
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
	b .L_02008766
.L_020085c2:
	ldr r5, .L_02008774
	adds r0, r5, #0
	bl Func_02000a18
	movs r1, #0
	movs r0, #14
	bl UiText_OpenMessageAtObject
	bl Func_02000a58
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_020085e2
	b .L_02008758
.L_020085e2:
	adds r0, r5, #1
	bl Func_02000a18
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02000a60
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	ldr r5, .L_02008778
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #244
	movs r2, #204
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPosition
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #252
	movs r2, #204
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r1, [r5]
	movs r0, #14
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #14
	bl Func_02000a80
	bl Func_02000a90
	cmp r0, #0
	beq .L_02008722
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #14
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #142
	movs r2, #204
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #142
	movs r2, #180
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #158
	movs r2, #173
	movs r0, #16
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	bl Object_SetModeById
	movs r1, #150
	movs r2, #180
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	bl Object_SetModeById
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl Object_GetById
	movs r3, #129
	adds r0, #89
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	adds r0, #35
	strb r3, [r0]
	movs r1, #1
	movs r0, #14
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r5, r3
	strb r5, [r0]
	movs r1, #0
	movs r0, #17
	movs r2, #0
	bl Func_020009f8
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	b .L_02008766
.L_02008722:
	movs r1, #236
	movs r2, #199
	adds r1, #255
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #14
	movs r1, #0
	bl Func_02000a60
	b .L_02008766
.L_02008758:
	adds r0, r5, #2
	bl Func_02000a18
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
.L_02008766:
	pop {r5, pc}
.L_02008768:
	.4byte MsgAhSoYou
.L_0200876c:
	.4byte MsgICanTWait
.L_02008770:
	.4byte MsgMyWifeHas
.L_02008774:
	.4byte MsgYouWantMe
.L_02008778:
	.4byte gPartyState
	.endif
	.section .text.x0200877c,"ax",%progbits
	.global Func_0200077c
	.thumb_func
Func_0200077c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #16
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008794
	ldr r0, .L_02008858
	b .L_02008806
.L_02008794:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020087a6
	ldr r0, .L_0200885c
	b .L_02008806
.L_020087a6:
	bl Func_02000a90
	cmp r0, #0
	beq .L_02008842
	bl Func_02000a90
	adds r6, r0, #0
	bl Func_02000988
	ldrh r0, [r0]
	movs r1, #2
	mov r8, r0
	adds r0, r6, #0
	bl Func_02000980
	movs r1, #5
	mov r0, r8
	bl Func_02000980
	ldr r7, .L_02008860
	adds r0, r7, #0
	bl Func_02000a18
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000a58
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_020087ee
	adds r0, r7, #1
	b .L_02008806
.L_020087ee:
	adds r0, r6, #0
	bl Func_020009a0
	cmp r0, #0
	bge .L_020087fc
	adds r0, r7, #2
	b .L_02008806
.L_020087fc:
	ldr r3, .L_02008864
	ldr r3, [r3, #16]
	cmp r3, r8
	bcs .L_02008814
	adds r0, r7, #3
.L_02008806:
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
	b .L_02008850
.L_02008814:
	adds r0, r7, #4
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
	adds r0, r6, #0
	movs r1, #3
	bl Func_02000a40
	movs r1, #0
	adds r0, r6, #0
	bl PartyInventory_GiveItem
	movs r0, #0
	bl Func_02000a88
	mov r3, r8
	negs r0, r3
	bl Party_AdjustCoins
	b .L_02008850
.L_02008842:
	ldr r0, .L_02008868
	bl Func_02000a18
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000a28
.L_02008850:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008858:
	.4byte MsgThisThisIs
.L_0200885c:
	.4byte MsgItTakesOur
.L_02008860:
	.4byte MsgHereSThe
.L_02008864:
	.4byte gPartyState
.L_02008868:
	.4byte MsgManSunshineWould
	.section .text.x0200886c,"ax",%progbits
	.global Func_0200086c
	.thumb_func
Func_0200086c:
	.ifdef TLA_EDITION_JA
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3, #0]
	ldr r3, .L_ja_02008940
	subs r2, #41
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_02008944
	cmp r2, r3
	bne .L_ja_020088de
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020088c2
	movs r1, #150
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #158
	movs r2, #173
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_ja_020088ca
.L_ja_020088c2:
	movs r0, #14
	movs r1, #0
	bl Func_02000a60
.L_ja_020088ca:
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
.L_ja_020088de:
	ldr r3, .L_ja_02008940
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_02008948
	cmp r2, r3
	bne .L_ja_0200893a
	movs r0, #0
	bl Func_02000a48
	movs r1, #2
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0, #0]
	movs r5, #4
	orrs r3, r5
	strb r3, [r0, #0]
	movs r1, #2
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0, #0]
	movs r1, #2
	orrs r3, r5
	strb r3, [r0, #0]
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0, #0]
	orrs r5, r3
	strb r5, [r0, #0]
.L_ja_0200893a:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_ja_02008940:
	.4byte gPartyState
.L_ja_02008944:
	.4byte 0x0000008d
.L_ja_02008948:
	.4byte 0x0000008e
	.else
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	ldr r3, .L_02008950
	subs r2, #41
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008954
	cmp r2, r3
	bne .L_020088f0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088d4
	movs r1, #150
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #158
	movs r2, #173
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_020009f8
	b .L_020088dc
.L_020088d4:
	movs r0, #14
	movs r1, #0
	bl Func_02000a60
.L_020088dc:
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
.L_020088f0:
	ldr r3, .L_02008950
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008958
	cmp r2, r3
	bne .L_0200894c
	movs r0, #0
	bl Func_02000a48
	movs r1, #2
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #4
	orrs r3, r5
	strb r3, [r0]
	movs r1, #2
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r1, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
.L_0200894c:
	movs r0, #0
	pop {r5, pc}
.L_02008950:
	.4byte gPartyState
.L_02008954:
	.4byte 0x0000008d
.L_02008958:
	.4byte 0x0000008e
	.endif
	.section .rodata.x02008ca4,"a",%progbits
	.global Data_02000ca4
Data_02000ca4:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.2byte 0x0000
	.2byte MsgFieldJenna
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte Data_02000a98
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01c60000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01260000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0xffff0068
	.4byte 0x00000001
	.4byte 0x01eb0000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00024000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00018000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x015a0000
	.4byte 0x0001c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01fa0000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
