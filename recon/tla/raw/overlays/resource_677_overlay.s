.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000c18
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
	.4byte Data_02000c48
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
	.section .text.x0200808c,"ax",%progbits
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000960
	cmp r0, #0
	beq .L_020080ae
	ldr r0, .L_020080c4
	bl Func_02000a18
	b .L_020080b4
.L_020080ae:
	ldr r0, .L_020080c8
	bl Func_02000a18
.L_020080b4:
	movs r0, #14
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	movs r0, #0
	pop {pc}
.L_020080c4:
	.4byte 0x00002233
.L_020080c8:
	.4byte 0x00002228
	.section .text.x020080cc,"ax",%progbits
	.global Func_020000cc
	.thumb_func
Func_020000cc:
	push {lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000960
	cmp r0, #0
	beq .L_020080ee
	ldr r0, .L_02008104
	bl Func_02000a18
	b .L_020080f4
.L_020080ee:
	ldr r0, .L_02008108
	bl Func_02000a18
.L_020080f4:
	movs r0, #15
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	movs r0, #0
	pop {pc}
.L_02008104:
	.4byte 0x00002234
.L_02008108:
	.4byte 0x00002229
	.section .text.x0200810c,"ax",%progbits
	.global Func_0200010c
	.thumb_func
Func_0200010c:
	push {lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000960
	cmp r0, #0
	beq .L_0200812e
	ldr r0, .L_02008144
	bl Func_02000a18
	b .L_02008134
.L_0200812e:
	ldr r0, .L_02008148
	bl Func_02000a18
.L_02008134:
	movs r0, #16
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	movs r0, #0
	pop {pc}
.L_02008144:
	.4byte 0x00002235
.L_02008148:
	.4byte 0x0000222a
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000960
	cmp r0, #0
	beq .L_0200816e
	ldr r0, .L_02008184
	bl Func_02000a18
	b .L_02008174
.L_0200816e:
	ldr r0, .L_02008188
	bl Func_02000a18
.L_02008174:
	movs r0, #16
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	movs r0, #0
	pop {pc}
.L_02008184:
	.4byte 0x00002232
.L_02008188:
	.4byte 0x00002227
	.section .text.x0200818c,"ax",%progbits
	.global Func_0200018c
	.thumb_func
Func_0200018c:
	push {lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_020081b0
	bl Func_02000a18
	movs r0, #10
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020081b0:
	.4byte 0x00002218
	.section .text.x020081b4,"ax",%progbits
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	push {lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_020081d8
	bl Func_02000a18
	movs r0, #11
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020081d8:
	.4byte 0x0000221a
	.section .text.x020081dc,"ax",%progbits
	.global Func_020001dc
	.thumb_func
Func_020001dc:
	push {lr}
	bl Func_020009b0
	movs r0, #0
	bl Func_02000a50
	ldr r0, .L_02008200
	bl Func_02000a18
	movs r0, #12
	movs r1, #0
	bl Func_02000a28
	bl Func_020009b8
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008200:
	.4byte 0x0000221c
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
	bl Func_02000a20
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
	.4byte 0x0000220a
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
	.4byte 0x00002217
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
	.4byte 0x00002219
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
	.4byte 0x0000221b
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
	bl Func_02000970
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
	.4byte 0x0000221d
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
	.4byte 0x00002221
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
	bl Func_02000a20
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
	.4byte 0x00002223
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
	push {r5, lr}
	bl Func_020004e0
	cmp r0, #0
	beq .L_02008538
	movs r0, #16
	adds r0, #255
	bl Func_02000968
	b .L_02008556
.L_02008538:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000960
	cmp r0, #0
	bne .L_02008556
	bl Func_02000a90
	cmp r0, #0
	bne .L_02008556
	movs r0, #16
	adds r0, #255
	bl Func_02000970
.L_02008556:
	movs r0, #16
	adds r0, #255
	bl Func_02000960
	cmp r0, #0
	bne .L_02008598
	ldr r5, .L_02008768
	adds r0, r5, #0
	bl Func_02000a18
	movs r1, #0
	movs r0, #14
	bl Func_02000a20
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
	bl Func_02000960
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
	bl Func_02000a20
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Func_02000a30
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
	bl Func_02000a10
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
	bl Func_02000a30
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
	bl Func_02000a30
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
	bl Func_02000a38
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
	bl Func_02000968
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
	bl Func_02000a30
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte 0x00001307
.L_0200876c:
	.4byte 0x0000130d
.L_02008770:
	.4byte 0x0000130e
.L_02008774:
	.4byte 0x0000130a
.L_02008778:
	.4byte gPartyState
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
	bl Func_02000960
	cmp r0, #0
	bne .L_02008794
	ldr r0, .L_02008858
	b .L_02008806
.L_02008794:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000960
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
	bl Func_02000a20
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
	bl Party_AdjustSixDigitCounterA
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
	.4byte 0x0000130f
.L_0200885c:
	.4byte 0x00001310
.L_02008860:
	.4byte 0x00001311
.L_02008864:
	.4byte gPartyState
.L_02008868:
	.4byte 0x00001316
	.section .text.x0200886c,"ax",%progbits
	.global Func_0200086c
	.thumb_func
Func_0200086c:
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
	bl Func_02000960
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
	bl Func_02000a30
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
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	movs r1, #2
	bl Func_02000a38
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
	bl Func_02000a38
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #4
	orrs r3, r5
	strb r3, [r0]
	movs r1, #2
	movs r0, #10
	bl Func_02000a38
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r1, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Func_02000a38
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
	.section .text.x0200895c,"ax",%progbits
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	movs r0, #0
	bx lr
	.section .rodata.x02008a98,"a",%progbits
.L_02008a98:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b50000
	.4byte 0x00000000
	.4byte 0x007f0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01c50000
	.4byte 0x00000000
	.4byte 0x00720000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.global Data_02000c18
Data_02000c18:
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
	.global Data_02000c48
Data_02000c48:
	.4byte 0x0000008d
	.4byte 0x1010108b
	.4byte 0xffffffff
	.4byte 0x1020208b
	.4byte 0xffffffff
	.4byte 0x1030708b
	.4byte 0xffffffff
	.4byte 0x1040408b
	.4byte 0xffffffff
	.4byte 0x0000008e
	.4byte 0x1010308b
	.4byte 0xffffffff
	.4byte 0x1020608b
	.4byte 0xffffffff
	.4byte 0x1030508b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000c8c
Data_02000c8c:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000ca4
Data_02000ca4:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte .L_02008a98
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
	.global Data_02000dac
Data_02000dac:
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00010000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000e84
Data_02000e84:
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000ea8
Data_02000ea8:
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002209
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000244
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000220f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002210
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020003bc
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000221e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000524
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_0200077c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_0200014c
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000524
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000220d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000220e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002211
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002212
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000221f
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002220
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte Func_0200008c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte Func_020000cc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte Func_0200010c
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte Func_0200008c
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403056
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000fe0
Data_02000fe0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002213
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002214
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200029c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020002fc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_0200035c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_0200029c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_020002fc
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_0200035c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002215
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002216
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002218
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000221a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000221c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte Func_0200018c
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte Func_020001b4
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte Func_020001dc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
