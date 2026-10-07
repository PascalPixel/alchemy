.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_02008068
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200806c
	cmp r2, r3
	bne .L_02008064
	ldr r0, .L_02008070
	b .L_02008066
.L_02008064:
	ldr r0, .L_02008074
.L_02008066:
	pop {pc}
.L_02008068:
	.4byte gPartyState
.L_0200806c:
	.4byte 0x0000003a
.L_02008070:
	.4byte Data_02000960
.L_02008074:
	.4byte Data_02000840
	.section .text.x020080c8,"ax",%progbits
	.balign 4
	.global Func_020000c8
	.thumb_func
Func_020000c8:
	push {lr}
	ldr r3, .L_020080e4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080e8
	cmp r2, r3
	bne .L_020080e0
	ldr r0, .L_020080ec
	b .L_020080e2
.L_020080e0:
	ldr r0, .L_020080f0
.L_020080e2:
	pop {pc}
.L_020080e4:
	.4byte gPartyState
.L_020080e8:
	.4byte 0x0000003a
.L_020080ec:
	.4byte Data_02000bac
.L_020080f0:
	.4byte Data_02000a20
	.section .text.x02008340,"ax",%progbits
	.balign 4
	.global Func_02000340
	.thumb_func
Func_02000340:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	ldr r5, .L_02008444
	adds r2, #255
	str r2, [r3]
	adds r2, #11
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	movs r1, #240
	orrs r3, r2
	lsls r1, r1, #1
	strb r3, [r0]
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008448
	cmp r2, r3
	bne .L_020083de
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	bne .L_02008390
	bl Func_02000454
	b .L_0200843e
.L_02008390:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083c4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #57
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083ba
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083c4
.L_020083ba:
	movs r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083d0
.L_020083c4:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_020005e4
	b .L_0200843e
.L_020083d0:
	movs r0, #18
	bl Object_GetById
	movs r1, #4
	bl Object_SetPartAttribute
	b .L_0200843e
.L_020083de:
	ldr r3, .L_0200844c
	cmp r2, r3
	bne .L_0200843e
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_0200840c
	movs r0, #8
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r2, #4
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	b .L_0200843e
.L_0200840c:
	cmp r3, #5
	bne .L_02008432
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #9
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r2, #4
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	b .L_0200843e
.L_02008432:
	cmp r3, #4
	bne .L_0200843e
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200843e:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008444:
	.4byte gPartyState
.L_02008448:
	.4byte 0x00000039
.L_0200844c:
	.4byte 0x0000003a
	.section .text.x02008454,"ax",%progbits
	.global Func_02000454
	.thumb_func
Func_02000454:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #48
	adds r2, #93
	str r2, [r3]
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
	.section .text.x02008470,"ax",%progbits
	.global Func_02000470
	.thumb_func
Func_02000470:
	push {lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #57
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084a0
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008490
	ldr r0, .L_020084a8
	b .L_02008492
.L_02008490:
	ldr r0, .L_020084ac
.L_02008492:
	bl Func_02000604
	movs r0, #18
	movs r1, #0
	bl Func_02000614
	b .L_020084a4
.L_020084a0:
	bl Func_020004dc
.L_020084a4:
	pop {pc}
	.2byte 0x0000
.L_020084a8:
	.4byte MsgIThinkMust
.L_020084ac:
	.4byte MsgSEnemiesAre
	.section .rodata.x02008654,"a",%progbits
.L_02008654:
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
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_0200870c:
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
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
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
	.global gSceneExits
gSceneExits:
	.4byte 0x00000039
	.4byte 0x10101037
	.4byte 0xffffffff
	.4byte 0x10202037
	.4byte 0xffffffff
	.4byte 0x10303037
	.4byte 0xffffffff
	.4byte 0x10404037
	.4byte 0xffffffff
	.4byte 0x10502084
	.4byte 0xffffffff
	.4byte 0x0000003a
	.4byte 0x10105037
	.4byte 0xffffffff
	.4byte 0x1050909a
	.4byte 0xffffffff
	.4byte 0x1040808b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000840
Data_02000840:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00010000
	.4byte 0xffff0048
	.4byte .L_02008654
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00010000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0003c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00010000
	.4byte 0xffff004c
	.4byte .L_0200870c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0001c000
	.4byte 0xffff0082
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00010000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00010000
	.4byte 0xffff004c
	.4byte 0x00000002
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000960
Data_02000960:
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00930000
	.4byte 0x00014000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01d20000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0001c000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0001c000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000a20
Data_02000a20:
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgIHeardSomeone
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgMyFatherIn
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020000f4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgThoseWhoHave
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgMyHusbandHas
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_0200014c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgThereArenT
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_0200019c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgUsedToBe
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgYouMustBe
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000470
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgItMustBeNice
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgThatBoySure
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgILoveExploring
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgWhatDoesThe
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgICanTLet
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgIDonTKnowMuch
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgThereSNoReason
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgWhyAreTravelers
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgTundariaIsThe
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgAroundHereIt
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte Func_020004b0
	.4byte 0x00000173
	.4byte 0xffff00ce
	.2byte MsgItSARoast, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00cf
	.2byte MsgFreshEggsOver, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d0
	.2byte MsgThatCheeseLooks, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d1
	.2byte MsgThereSFish, 0x0040
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000bac
Data_02000bac:
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_020001fc
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000288
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgIMSick
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgIDLoveToGo
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020002e4
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_020001fc
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgIWonderIfThere
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgWheneverVisitorsCome
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgMaybeTheyLl
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgIWonderWhatIt
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgOnceThisWas
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte Func_02000078
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte Func_020000a0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
