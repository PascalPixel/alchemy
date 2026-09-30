.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r0, #10
	bl Object_GetById
	ldr r2, .L_02008068
	ldr r3, [r0, #8]
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	bhi .L_02008064
	ldr r0, [r0, #16]
	movs r3, #155
	lsls r3, r3, #18
	cmp r0, r3
	blt .L_02008064
	movs r2, #157
	lsls r2, r2, #18
	cmp r0, r2
	bgt .L_02008064
	movs r0, #0
	b .L_02008066
.L_02008064:
	movs r0, #1
.L_02008066:
	pop {pc}
.L_02008068:
	.4byte 0xff6c0000
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {lr}
	movs r0, #9
	bl Object_GetById
	ldr r2, .L_0200809c
	ldr r3, [r0, #8]
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	bhi .L_02008098
	ldr r0, [r0, #16]
	movs r3, #155
	lsls r3, r3, #18
	cmp r0, r3
	blt .L_02008098
	movs r2, #157
	lsls r2, r2, #18
	cmp r0, r2
	bgt .L_02008098
	movs r0, #0
	b .L_0200809a
.L_02008098:
	movs r0, #1
.L_0200809a:
	pop {pc}
.L_0200809c:
	.4byte 0xff7c0000
	.section .text.x020080a0,"ax",%progbits
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	ldr r0, .L_020080a4
	bx lr
.L_020080a4:
	.4byte Data_02002ad0
	.section .text.x020080a8,"ax",%progbits
	.global Func_020000a8
	.thumb_func
Func_020000a8:
	movs r0, #0
	bx lr
	.section .text.x020080ac,"ax",%progbits
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	ldr r0, .L_020080b0
	bx lr
.L_020080b0:
	.4byte Data_02002b00
	.section .text.x020080b4,"ax",%progbits
	.global Func_020000b4
	.thumb_func
Func_020000b4:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080c8
	ldr r0, .L_020080e0
	b .L_020080de
.L_020080c8:
	ldr r3, .L_020080e4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_020080dc
	ldr r0, .L_020080e8
	b .L_020080de
.L_020080dc:
	ldr r0, .L_020080ec
.L_020080de:
	pop {pc}
.L_020080e0:
	.4byte Data_02002d80
.L_020080e4:
	.4byte gPartyState
.L_020080e8:
	.4byte Data_02002cf0
.L_020080ec:
	.4byte Data_02002b40
	.section .text.x020080f0,"ax",%progbits
	.global Func_020000f0
	.thumb_func
Func_020000f0:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008104
	ldr r0, .L_0200811c
	b .L_0200811a
.L_02008104:
	ldr r3, .L_02008120
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_02008118
	ldr r0, .L_02008124
	b .L_0200811a
.L_02008118:
	ldr r0, .L_02008128
.L_0200811a:
	pop {pc}
.L_0200811c:
	.4byte Data_0200323c
.L_02008120:
	.4byte gPartyState
.L_02008124:
	.4byte Data_02003164
.L_02008128:
	.4byte Data_02002f78
	.section .text.x0200812c,"ax",%progbits
	.global Func_0200012c
	.thumb_func
Func_0200012c:
	push {r5, lr}
	ldr r3, .L_02008160
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
	ldr r2, .L_0200815c
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008164
	adds r0, r5, #0
	bl Func_0200230c
	b .L_0200819e
.L_0200815c:
	.4byte 0xffffc000
.L_02008160:
	.4byte gPartyState
.L_02008164:
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200818c
	ldr r0, .L_020081a0
	bl Func_02002244
	movs r0, #12
	movs r1, #0
	bl Func_02002254
	b .L_0200819a
.L_0200818c:
	ldr r0, .L_020081a4
	bl Func_02002244
	movs r0, #22
	movs r1, #0
	bl Func_02002254
.L_0200819a:
	bl Func_020021cc
.L_0200819e:
	pop {r5, pc}
.L_020081a0:
	.4byte 0x000023df
.L_020081a4:
	.4byte 0x000022ef
	.section .text.x020081a8,"ax",%progbits
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	push {r5, lr}
	ldr r3, .L_020081e0
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
	ldr r2, .L_020081dc
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020081e4
	movs r0, #9
	adds r1, r5, #0
	bl Func_02002314
	b .L_02008216
	.2byte 0x0000
.L_020081dc:
	.4byte 0xffffc000
.L_020081e0:
	.4byte gPartyState
.L_020081e4:
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008204
	ldr r0, .L_02008218
	bl Func_02002244
	b .L_0200820a
.L_02008204:
	ldr r0, .L_0200821c
	bl Func_02002244
.L_0200820a:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02002254
	bl Func_020021cc
.L_02008216:
	pop {r5, pc}
.L_02008218:
	.4byte 0x000023db
.L_0200821c:
	.4byte 0x000022eb
	.section .text.x02008220,"ax",%progbits
	.global Func_02000220
	.thumb_func
Func_02000220:
	push {r5, lr}
	ldr r3, .L_02008258
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
	ldr r2, .L_02008254
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200825c
	movs r0, #24
	adds r1, r5, #0
	bl Func_02002304
	b .L_0200828e
	.2byte 0x0000
.L_02008254:
	.4byte 0xffffc000
.L_02008258:
	.4byte gPartyState
.L_0200825c:
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200827c
	ldr r0, .L_02008290
	bl Func_02002244
	b .L_02008282
.L_0200827c:
	ldr r0, .L_02008294
	bl Func_02002244
.L_02008282:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02002254
	bl Func_020021cc
.L_0200828e:
	pop {r5, pc}
.L_02008290:
	.4byte 0x000023d7
.L_02008294:
	.4byte 0x000022e7
	.section .text.x02008298,"ax",%progbits
	.global Func_02000298
	.thumb_func
Func_02000298:
	push {lr}
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200832c
	ldr r0, .L_02008350
	bl Func_02002244
	movs r1, #0
	movs r0, #8
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #8
	bl Func_0200226c
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #8
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #0
	adds r1, #255
	movs r0, #8
	bl Func_0200226c
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #8
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #8
	bl ObjectMotion_SetVariantCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #34
	bl GameFlag_SetBit
.L_0200832c:
	movs r1, #4
	movs r0, #8
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	ldr r0, .L_02008354
	bl Func_02002244
	movs r0, #8
	movs r1, #0
	bl Func_02002254
	bl Func_020021cc
	pop {pc}
	.2byte 0x0000
.L_02008350:
	.4byte 0x000022d9
.L_02008354:
	.4byte 0x000022dd
	.section .text.x02008358,"ax",%progbits
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #35
	bl GameFlag_SetBit
	ldr r0, .L_02008374
	bl Func_02002244
	movs r0, #8
	movs r1, #0
	bl Func_02002254
	pop {pc}
.L_02008374:
	.4byte 0x000022e2
	.section .text.x02008378,"ax",%progbits
	.global Func_02000378
	.thumb_func
Func_02000378:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	ldr r3, .L_020083f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #15
	ldr r1, [r3]
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #44
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083ba
	ldr r0, .L_020083f8
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	b .L_020083e2
.L_020083ba:
	ldr r0, .L_020083fc
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r0, #196
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_020083e2
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
.L_020083e2:
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_020021cc
	pop {r5, pc}
.L_020083f4:
	.4byte gPartyState
.L_020083f8:
	.4byte 0x000023d5
.L_020083fc:
	.4byte 0x000023a8
	.section .text.x02008400,"ax",%progbits
	.global Func_02000400
	.thumb_func
Func_02000400:
	push {r5, lr}
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	ldr r5, .L_02008454
	adds r0, r5, #0
	bl Func_02002244
	movs r1, #0
	movs r0, #16
	bl UiText_OpenMessageAtObject
	bl Func_020022dc
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008438
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02002244
	b .L_02008444
.L_02008438:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02002244
.L_02008444:
	movs r0, #16
	movs r1, #0
	bl Func_02002254
	bl Func_020021cc
	pop {r5, pc}
	.2byte 0x0000
.L_02008454:
	.4byte 0x000023c1
	.section .text.x02008458,"ax",%progbits
	.global Func_02000458
	.thumb_func
Func_02000458:
	push {r5, lr}
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	ldr r3, .L_02008510
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r0, #15
	ldr r1, [r5]
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008494
	ldr r0, .L_02008514
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	b .L_020084fc
.L_02008494:
	ldr r0, .L_02008518
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #35
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084f2
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	ldr r0, [r5]
	bl Func_0200226c
	movs r1, #4
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_0200226c
	movs r1, #4
	movs r0, #15
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_02002254
.L_020084f2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_SetBit
.L_020084fc:
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_020021cc
	pop {r5, pc}
	.2byte 0x0000
.L_02008510:
	.4byte gPartyState
.L_02008514:
	.4byte 0x000023b0
.L_02008518:
	.4byte 0x000023ad
	.section .text.x0200851c,"ax",%progbits
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {r5, lr}
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085a8
	ldr r0, .L_020085f8
	bl Func_02002244
	movs r1, #0
	movs r0, #17
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #17
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #17
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #17
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #17
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_SetBit
	b .L_020085b6
.L_020085a8:
	ldr r0, .L_020085fc
	bl Func_02002244
	movs r0, #17
	movs r1, #0
	bl Func_02002254
.L_020085b6:
	ldr r5, .L_02008600
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #16
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndReset
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_020021cc
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_020085f8:
	.4byte 0x000022d3
.L_020085fc:
	.4byte 0x000022d8
.L_02008600:
	.4byte gPartyState
	.section .text.x02008604,"ax",%progbits
	.global Func_02000604
	.thumb_func
Func_02000604:
	push {lr}
	movs r0, #0
	bl Func_0200061c
	pop {pc}
	.2byte 0x0000
	.section .text.x02008610,"ax",%progbits
	.global Func_02000610
	.thumb_func
Func_02000610:
	push {lr}
	movs r0, #1
	bl Func_0200061c
	pop {pc}
	.2byte 0x0000
	.section .text.x0200861c,"ax",%progbits
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r0, #143
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl Motion_CamBounds
	ldr r3, .L_02008930
	cmp r7, #0
	bne .L_02008654
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #184
	adds r2, #36
	bl ObjectMotion_SetPositionAndReset
	b .L_02008664
.L_02008654:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #104
	adds r2, #36
	bl ObjectMotion_SetPositionAndReset
.L_02008664:
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, .L_02008934
	bl Func_02002244
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #15
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r1, #4
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #22
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02002274
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r2, #30
	movs r0, #20
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #30
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #3
	movs r0, #15
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #22
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r3, .L_02008930
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #16
	movs r0, #15
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #1
	movs r2, #30
	bl Func_0200226c
	cmp r7, #0
	bne .L_020087c2
	movs r2, #134
	movs r0, #15
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	b .L_020087ce
.L_020087c2:
	movs r2, #134
	movs r0, #15
	movs r1, #120
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
.L_020087ce:
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_02008930
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #8
	movs r2, #0
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r0, #22
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #2
	adds r1, #255
	movs r0, #22
	bl Func_02002274
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #30
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #20
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r0, #22
	movs r1, #20
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	bl Func_02002274
	movs r1, #0
	movs r0, #22
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #130
	movs r1, #144
	lsls r2, r2, #2
	movs r0, #15
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	cmp r7, #0
	bne .L_0200890e
	movs r2, #142
	movs r0, #22
	movs r1, #104
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	b .L_0200891a
.L_0200890e:
	movs r2, #142
	movs r0, #22
	movs r1, #184
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
.L_0200891a:
	movs r0, #22
	bl ObjectMotion_CommitCurrentPositionAndActivate
	cmp r7, #0
	bne .L_02008938
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_02008944
.L_02008930:
	.4byte gPartyState
.L_02008934:
	.4byte 0x00002346
.L_02008938:
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02008944:
	ldr r3, .L_02008d3c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #144
	adds r2, #28
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r3, #192
	movs r0, #5
	movs r1, #8
	movs r2, #24
	lsls r3, r3, #8
	bl Func_020022cc
	movs r3, #192
	movs r0, #6
	movs r1, #24
	movs r2, #24
	lsls r3, r3, #8
	bl Func_020022cc
	movs r1, #8
	movs r3, #192
	movs r0, #7
	negs r1, r1
	movs r2, #24
	lsls r3, r3, #8
	bl Func_020022cc
	movs r1, #24
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #24
	negs r1, r1
	movs r0, #23
	bl Func_020022cc
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #7
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #5
	movs r1, #23
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #20
	movs r2, #0
	movs r0, #15
	bl Object_LinkPair
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #23
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	ldr r1, [r5]
	movs r2, #0
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #15
	bl Func_0200226c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #15
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
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
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #15
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02001d8c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_0200226c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200226c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #5
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #20
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	ldr r1, [r5]
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_0200226c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_02008d5e
	ldr r0, .L_02008d40
	bl Func_02002244
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	ldr r1, [r5]
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	b .L_02008d44
.L_02008d3c:
	.4byte gPartyState
.L_02008d40:
	.4byte 0x00002367
.L_02008d44:
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02002254
.L_02008d5e:
	ldr r0, .L_020090c8
	bl Func_02002244
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
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
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_0200226c
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl Func_02002254
	movs r0, #90
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #144
	movs r2, #170
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002214
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #21
	movs r0, #22
	bl ObjectMotion_SetAngleToward
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #15
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #21
	bl Object_AttachWorkTargetToObject
	movs r0, #120
	bl Battle_WaitMode0
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #21
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #21
	bl ObjectMotion_SetSpeedParameters
	movs r0, #21
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	movs r2, #162
	strb r3, [r0]
	movs r1, #144
	movs r0, #21
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	cmp r7, #0
	beq .L_02008eb0
	b .L_020090cc
.L_02008eb0:
	movs r2, #162
	movs r0, #21
	movs r1, #88
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #154
	movs r0, #21
	movs r1, #56
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #146
	movs r1, #56
	lsls r2, r2, #2
	movs r0, #21
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r2, #142
	strb r3, [r0]
	movs r1, #88
	lsls r2, r2, #2
	movs r0, #22
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r2, #0
	movs r0, #22
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #21
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	movs r0, #21
	movs r1, #104
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	lsls r2, r2, #2
	movs r0, #21
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #23
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #134
	movs r0, #21
	movs r1, #120
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r2, #146
	movs r0, #22
	movs r1, #56
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #162
	movs r0, #22
	movs r1, #88
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	b .L_020092e4
.L_020090c8:
	.4byte 0x00002369
.L_020090cc:
	movs r2, #162
	movs r0, #21
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #154
	movs r0, #21
	movs r1, #232
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #146
	movs r1, #232
	lsls r2, r2, #2
	movs r0, #21
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r2, #142
	strb r3, [r0]
	movs r1, #200
	lsls r2, r2, #2
	movs r0, #22
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	movs r1, #128
	strb r3, [r0]
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #21
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	movs r0, #21
	movs r1, #184
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #21
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #6
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	lsls r2, r2, #2
	movs r0, #21
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #134
	movs r0, #21
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r2, #146
	movs r0, #22
	movs r1, #232
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #162
	movs r0, #22
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_020092e4:
	movs r0, #21
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	ldr r5, .L_02009678
	strb r3, [r0]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
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
	movs r1, #192
	movs r0, #6
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
	movs r0, #23
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #20
	bl Func_02002254
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #20
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	ldr r1, [r5]
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #20
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #20
	movs r2, #0
	movs r0, #15
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
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
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	ldr r1, [r5]
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_0200226c
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #15
	bl Func_0200226c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #20
	bl Func_0200226c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #21
	bl Func_0200226c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #22
	bl Func_0200226c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200226c
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #21
	bl Func_0200226c
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #21
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #131
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200226c
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #23
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r0, #23
	movs r1, #7
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200226c
	movs r0, #23
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #15
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #6
	bl Func_0200226c
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200226c
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	bl Func_02002254
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
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #90
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #0
	movs r0, #15
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009680
	ldr r0, .L_0200967c
	bl Func_02002244
	movs r1, #132
	lsls r1, r1, #1
	movs r0, #15
	movs r2, #30
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	b .L_0200968e
.L_02009678:
	.4byte gPartyState
.L_0200967c:
	.4byte 0x00002384
.L_02009680:
	ldr r0, .L_02009948
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
.L_0200968e:
	ldr r6, .L_0200994c
	adds r0, r6, #0
	bl Func_02002244
	movs r1, #1
	movs r0, #23
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #21
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #20
	movs r0, #15
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200226c
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #15
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200226c
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_0200226c
	ldr r5, .L_02009950
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r1, #0
	movs r0, #6
	bl UiText_OpenMessageAtObject
	ldr r1, [r5]
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #6
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #20
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200983c
	adds r0, r6, #0
	adds r0, #10
	bl Func_02002244
	movs r1, #4
	movs r0, #21
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	b .L_0200985a
.L_0200983c:
	adds r0, r6, #0
	adds r0, #11
	bl Func_02002244
	movs r1, #3
	movs r0, #21
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	bl Func_02002254
.L_0200985a:
	ldr r6, .L_02009954
	adds r0, r6, #0
	bl Func_02002244
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #23
	bl Func_0200226c
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	ldr r5, .L_02009950
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r2, #0
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #23
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009958
	adds r0, r6, #4
	bl Func_02002244
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	b .L_02009994
.L_02009948:
	.4byte 0x00002385
.L_0200994c:
	.4byte 0x00002386
.L_02009950:
	.4byte gPartyState
.L_02009954:
	.4byte 0x00002392
.L_02009958:
	adds r0, r6, #7
	bl Func_02002244
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r0, #7
	movs r1, #0
	bl Func_02002254
.L_02009994:
	ldr r0, .L_02009d54
	bl Func_02002244
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #20
	movs r0, #15
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_02009d58
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r1, #1
	movs r0, #21
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_0200226c
	movs r2, #0
	ldr r1, [r5]
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #23
	ldr r0, [r5]
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r0, #5
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #7
	movs r1, #23
	bl ObjectMotion_SetAngleToward
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
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #15
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_0200226c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200226c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #6
	movs r2, #0
	adds r1, #255
	movs r0, #6
	bl Func_0200226c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #5
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #1
	movs r0, #7
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	ldr r0, [r5]
	movs r1, #7
	bl Object_LinkPair
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r2, #0
	movs r0, #5
	movs r1, #6
	bl Object_LinkPair
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #5
	movs r2, #0
	bl Object_LinkPair
	movs r2, #0
	movs r0, #7
	movs r1, #6
	bl Object_LinkPair
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #23
	movs r1, #15
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #15
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #21
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009cc0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02009cc0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009cf0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009cf0:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009d20
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009d20:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #23
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009d5c
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #23
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009d5c
	.2byte 0x0000
.L_02009d54:
	.4byte 0x0000239c
.L_02009d58:
	.4byte gPartyState
.L_02009d5c:
	movs r0, #23
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl Func_02002214
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_SetBit
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_020021cc
	pop {r5, r6, r7, pc}
	.section .text.x02009d84,"ax",%progbits
	.global Func_02001d84
	.thumb_func
Func_02001d84:
	movs r0, #1
	negs r0, r0
	bx lr
	.2byte 0x0000
	.section .text.x02009d8c,"ax",%progbits
	.global Func_02001d8c
	.thumb_func
Func_02001d8c:
	push {lr}
	movs r0, #140
	movs r1, #1
	bl Func_02002294
	movs r1, #1
	negs r1, r1
	movs r0, #15
	bl Func_0200229c
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	ldr r2, .L_02009dcc
	str r2, [r3, #36]
	adds r3, #32
	movs r2, #1
	strb r2, [r3]
	bl Func_020022b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	bl Field_DispatchTypeHandler
	bl Func_020022a4
	bl Func_020022ac
	pop {pc}
.L_02009dcc:
	.4byte Func_02001d84
	.section .text.x02009dd0,"ax",%progbits
	.global Func_02001dd0
	.thumb_func
Func_02001dd0:
	push {r5, r6, lr}
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	ldr r6, .L_0200a000
	adds r0, r6, #0
	bl Func_02002244
	ldr r5, .L_0200a004
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #134
	ldr r0, [r5]
	movs r1, #144
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #15
	bl Func_0200226c
	movs r1, #0
	movs r0, #15
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009e3a
	adds r0, r6, #1
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	b .L_02009e48
.L_02009e3a:
	adds r0, r6, #2
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
.L_02009e48:
	movs r1, #10
	movs r2, #60
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	ldr r0, .L_0200a008
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #6
	adds r1, #255
	movs r2, #120
	movs r0, #15
	bl Func_0200226c
	movs r1, #131
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200226c
	movs r1, #0
	movs r0, #15
	bl Func_02002254
	bl Func_020022e4
	movs r0, #15
	bl Object_GetById
	movs r1, #2
	bl Func_020022fc
	movs r0, #201
	bl Func_0200231c
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #144
	movs r2, #130
	movs r0, #24
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002214
	movs r0, #24
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #130
	movs r1, #160
	lsls r2, r2, #2
	movs r0, #24
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #24
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #15
	bl Object_GetById
	movs r1, #0
	bl Func_020022fc
	bl Func_020022f4
	bl Func_020022ec
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #24
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	ldr r5, .L_0200a004
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #0
	ldr r1, [r5]
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #15
	bl Func_02002254
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #15
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r0, #15
	movs r1, #24
	bl Object_LinkPair
	movs r1, #0
	movs r0, #15
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #4
	movs r2, #20
	movs r0, #24
	bl ObjectMotion_Launch
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r0, #24
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #11
	movs r0, #24
	bl Func_020022c4
	bl Func_020021cc
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r2, #0
	ldr r0, [r5]
	movs r1, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #15
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #15
	bl Func_02002254
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #44
	bl GameFlag_SetBit
	movs r0, #196
	adds r0, #255
	bl PartyInventory_Remove
	bl Func_020021cc
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a000:
	.4byte 0x000023cb
.L_0200a004:
	.4byte gPartyState
.L_0200a008:
	.4byte 0x000023ce
	.section .text.x0200a00c,"ax",%progbits
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	ldr r3, .L_0200a18c
	subs r2, #31
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #7
	beq .L_0200a034
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_0200a034:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a09e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a09e
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #7
	bne .L_0200a09e
	movs r0, #7
	bl Func_0200231c
	movs r1, #144
	movs r2, #142
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002214
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_0200a09e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200a104
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	movs r0, #7
	bl Func_0200231c
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_0200a104:
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_0200a134
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a134
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	movs r1, #2
	movs r0, #17
	adds r1, #255
	bl Func_02002274
.L_0200a134:
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200a186
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a186
	movs r0, #9
	movs r1, #5
	bl Object_SetModeById
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r0, #11
	movs r1, #5
	bl Object_SetModeById
	movs r1, #5
	movs r0, #12
	bl Object_SetModeById
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #24]
	negs r3, r3
	str r3, [r5, #24]
.L_0200a186:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0200a18c:
	.4byte gPartyState
	.section .text.x0200a190,"ax",%progbits
	.global Func_02002190
	.thumb_func
Func_02002190:
	movs r0, #0
	bx lr
	.section .rodata.x0200a324,"a",%progbits
.L_0200a324:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffffffff
.L_0200a4f0:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00006666
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x80010000
	.4byte 0x0000002e
	.4byte Func_0200006c
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffffffff
.L_0200a6bc:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000027
	.4byte 0x00000000
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
	.4byte 0x00000028
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
	.4byte 0x00000005
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
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffffffff
.L_0200a8dc:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
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
	.4byte 0x0000003c
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
	.4byte 0x0000000a
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
	.4byte 0x0000005a
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x00000015
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000011
	.global Data_02002ad0
Data_02002ad0:
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
	.global Data_02002b00
Data_02002b00:
	.4byte 0x000000aa
	.4byte 0x101020ab
	.4byte 0xffffffff
	.4byte 0x102030ab
	.4byte 0xffffffff
	.4byte 0x103040ab
	.4byte 0xffffffff
	.4byte 0x104050ab
	.4byte 0xffffffff
	.4byte 0x105060ab
	.4byte 0xffffffff
	.4byte 0x106070ab
	.4byte 0xffffffff
	.4byte 0x107080ab
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02002b40
Data_02002b40:
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001a000
	.4byte 0xffff0078
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00010000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00012000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001a000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0020
	.4byte .L_0200a8dc
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x016a0000
	.4byte 0x00014000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x006c0000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00012000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff007b
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00010000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff007f
	.4byte .L_0200a6bc
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00026000
	.4byte 0xffff007f
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002cf0
Data_02002cf0:
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte .L_0200a324
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00010000
	.4byte 0xffff0078
	.4byte .L_0200a4f0
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00018000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x005a0000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00018000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002d80
Data_02002d80:
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x016a0000
	.4byte 0x00014000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x006c0000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00012000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff007b
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00010000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff007f
	.4byte .L_0200a6bc
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00028000
	.4byte 0xffff007f
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001e000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00010000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00018000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001a000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00014000
	.4byte 0xffff001d
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0001c000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x006300f5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002f78
Data_02002f78:
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
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_0200051c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000022bc
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000022bd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022be
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000022c2
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022c3
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000022c4
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000022c5
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000022c6
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000022c7
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000220
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000022e8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_020001a8
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000022ec
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_0200012c
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000022f0
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x000022f1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000022bf
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022c0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022c1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022c8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022c9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000022ca
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000022cb
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000022cc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000022cd
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000022e9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000022ea
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000022ed
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000022ee
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000022f2
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000022f3
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000022f4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003164
Data_02003164:
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000298
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000022de
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022df
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000022e0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022e1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte Func_02000358
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022e3
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022e4
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022e5
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022e6
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200323c
Data_0200323c:
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
	.4byte 0x00000002
	.4byte 0x09280016
	.4byte Func_02000604
	.4byte 0x00000002
	.4byte 0x09280017
	.4byte Func_02000610
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000220
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000023d8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020001a8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000023dc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_0200012c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000023e0
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000023e1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000378
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000400
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000023c4
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000023c5
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000023c6
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000023a9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000023aa
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000023d9
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000023da
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000023dd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000023de
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000023e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000023e3
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000023e4
	.4byte 0x00008d15
	.4byte 0xffff040f
	.4byte Func_02000458
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000023c7
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000023c8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000023c9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000023ca
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000023ab
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000023ac
	.4byte 0x0001c314
	.4byte 0x092c000f
	.4byte Func_02001dd0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
