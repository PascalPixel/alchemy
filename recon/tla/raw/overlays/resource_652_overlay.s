.syntax unified
	.thumb
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008058
	ldr r0, .L_0200805c
	b .L_0200805a
.L_02008058:
	ldr r0, .L_02008060
.L_0200805a:
	pop {pc}
.L_0200805c:
	.4byte Data_020011b8
.L_02008060:
	.4byte Data_0200116c
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {lr}
	ldr r3, .L_02008094
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008098
	cmp r2, r3
	bne .L_0200807c
	ldr r0, .L_0200809c
	b .L_02008090
.L_0200807c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200808e
	ldr r0, .L_020080a0
	b .L_02008090
.L_0200808e:
	ldr r0, .L_020080a4
.L_02008090:
	pop {pc}
	.2byte 0x0000
.L_02008094:
	.4byte gPartyState
.L_02008098:
	.4byte 0x0000000c
.L_0200809c:
	.4byte Data_020013fc
.L_020080a0:
	.4byte Data_020014a4
.L_020080a4:
	.4byte Data_02001204
	.4byte 0x00004770
	.section .text.x020080ac,"ax",%progbits
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	adds r0, r5, #0
	bl Func_02000d94
	adds r0, r5, #0
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	adds r0, r5, #0
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008112
	ldr r0, .L_02008148
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #180
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #72
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008140
.L_02008112:
	ldr r0, .L_0200814c
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #196
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #104
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02008140:
	movs r0, #20
	bl Battle_WaitMode0
	pop {r5, pc}
.L_02008148:
	.4byte MsgDeriHeyaGah
.L_0200814c:
	.4byte MsgDeriHeyaYarg
	.section .text.x02008150,"ax",%progbits
	.global Func_02000150
	.thumb_func
Func_02000150:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	adds r0, r5, #0
	adds r1, #255
	movs r2, #50
	bl Func_02000d94
	adds r0, r5, #0
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #15
	movs r1, #6
	adds r0, r5, #0
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_020081a0
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #10
	bl Battle_WaitMode0
	pop {r5, pc}
.L_020081a0:
	.4byte MsgDeriHeyaWhatAreYouDoing
	.section .text.x02008218,"ax",%progbits
	.global Func_02000218
	.thumb_func
Func_02000218:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	ldr r3, .L_02008258
	ldr r2, .L_0200825c
	cmp r0, #0
	beq .L_02008242
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, r2
	bne .L_0200823e
	ldr r0, .L_02008260
	b .L_02008256
.L_0200823e:
	ldr r0, .L_02008264
	b .L_02008256
.L_02008242:
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, r2
	bne .L_02008254
	ldr r0, .L_02008268
	b .L_02008256
.L_02008254:
	ldr r0, .L_0200826c
.L_02008256:
	pop {pc}
.L_02008258:
	.4byte gPartyState
.L_0200825c:
	.4byte 0x0000000c
.L_02008260:
	.4byte Data_02001c18
.L_02008264:
	.4byte Data_02001990
.L_02008268:
	.4byte Data_02001924
.L_0200826c:
	.4byte Data_020016e4
	.section .text.x02008270,"ax",%progbits
	.global Func_02000270
	.thumb_func
Func_02000270:
	push {r5, lr}
	ldr r3, .L_020082a8
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
	ldr r2, .L_020082a4
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020082ac
	movs r0, #1
	adds r1, r5, #0
	bl Func_02000ddc
	b .L_020082c8
	.2byte 0x0000
.L_020082a4:
	.4byte 0xffffc000
.L_020082a8:
	.4byte gPartyState
.L_020082ac:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_020082cc
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_020082c8:
	pop {r5, pc}
	.2byte 0x0000
.L_020082cc:
	.4byte MsgDeriHeyaSouthRoadDanger
	.section .text.x020082d0,"ax",%progbits
	.global Func_020002d0
	.thumb_func
Func_020002d0:
	push {r5, lr}
	ldr r3, .L_02008308
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
	ldr r2, .L_02008304
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200830c
	movs r0, #2
	adds r1, r5, #0
	bl Func_02000ddc
	b .L_02008328
	.2byte 0x0000
.L_02008304:
	.4byte 0xffffc000
.L_02008308:
	.4byte gPartyState
.L_0200830c:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_0200832c
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_02008328:
	pop {r5, pc}
	.2byte 0x0000
.L_0200832c:
	.4byte MsgDeriHeyaNewTravelers
	.section .text.x02008330,"ax",%progbits
	.global Func_02000330
	.thumb_func
Func_02000330:
	push {r5, lr}
	ldr r3, .L_02008368
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
	ldr r2, .L_02008364
	ands r3, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200836c
	movs r0, #0
	adds r1, r5, #0
	bl Func_02000dec
	b .L_02008388
	.2byte 0x0000
.L_02008364:
	.4byte 0xffffc000
.L_02008368:
	.4byte gPartyState
.L_0200836c:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_0200838c
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_02008388:
	pop {r5, pc}
	.2byte 0x0000
.L_0200838c:
	.4byte MsgDeriHeyaDinnerChoice
	.section .text.x02008390,"ax",%progbits
	.global Func_02000390
	.thumb_func
Func_02000390:
	push {r5, r6, lr}
	ldr r3, .L_020083e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_020083e0
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008438
	movs r0, #233
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008430
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r5, .L_020083e8
	adds r0, r5, #0
	bl Engine_EventSetMessage
	movs r1, #0
	adds r0, r6, #0
	b .L_020083ec
	.2byte 0x0000
.L_020083e0:
	.4byte 0xffffc000
.L_020083e4:
	.4byte gPartyState
.L_020083e8:
	.4byte MsgDeriHeyaTravelersFreeTime
.L_020083ec:
	bl UiText_OpenMessageAtObject
	bl Func_02000dd4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200840c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Engine_EventSetMessage
	b .L_02008418
.L_0200840c:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Engine_EventSetMessage
.L_02008418:
	adds r0, r6, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
	movs r0, #233
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008490
.L_02008430:
	adds r0, r6, #0
	bl Func_02000de4
	b .L_02008490
.L_02008438:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r5, .L_02008494
	adds r0, r5, #0
	bl Engine_EventSetMessage
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000dd4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200846e
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Engine_EventSetMessage
	b .L_0200847a
.L_0200846e:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Engine_EventSetMessage
.L_0200847a:
	adds r0, r6, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
	movs r0, #233
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
.L_02008490:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008494:
	.4byte MsgDeriHeyaTravelersFreeTime
	.section .text.x02008498,"ax",%progbits
	.global Func_02000498
	.thumb_func
Func_02000498:
	push {r5, lr}
	ldr r3, .L_020084cc
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
	ldr r2, .L_020084c8
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020084d0
	adds r0, r5, #0
	bl Func_02000de4
	b .L_020084ec
.L_020084c8:
	.4byte 0xffffc000
.L_020084cc:
	.4byte gPartyState
.L_020084d0:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_020084f0
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_020084ec:
	pop {r5, pc}
	.2byte 0x0000
.L_020084f0:
	.4byte MsgDeriHeyaLeaveForMadora
	.section .text.x020084f4,"ax",%progbits
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {r5, lr}
	ldr r3, .L_0200852c
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
	ldr r2, .L_02008528
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008530
	movs r0, #1
	adds r1, r5, #0
	bl Func_02000ddc
	b .L_0200854c
	.2byte 0x0000
.L_02008528:
	.4byte 0xffffc000
.L_0200852c:
	.4byte gPartyState
.L_02008530:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_02008550
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_0200854c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008550:
	.4byte MsgDeriHeyaPirateTreasure
	.section .text.x02008554,"ax",%progbits
	.global Func_02000554
	.thumb_func
Func_02000554:
	push {r5, lr}
	ldr r3, .L_0200858c
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
	ldr r2, .L_02008588
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008590
	movs r0, #2
	adds r1, r5, #0
	bl Func_02000ddc
	b .L_020085ac
	.2byte 0x0000
.L_02008588:
	.4byte 0xffffc000
.L_0200858c:
	.4byte gPartyState
.L_02008590:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_020085b0
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_020085ac:
	pop {r5, pc}
	.2byte 0x0000
.L_020085b0:
	.4byte MsgDeriHeyaSpaciousVillage
	.section .text.x020085b4,"ax",%progbits
	.global Func_020005b4
	.thumb_func
Func_020005b4:
	push {r5, lr}
	ldr r3, .L_020085ec
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
	ldr r2, .L_020085e8
	ands r3, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020085f0
	movs r0, #0
	adds r1, r5, #0
	bl Func_02000dec
	b .L_0200860c
	.2byte 0x0000
.L_020085e8:
	.4byte 0xffffc000
.L_020085ec:
	.4byte gPartyState
.L_020085f0:
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_02008610
	bl Engine_EventSetMessage
	adds r0, r5, #0
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
.L_0200860c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008610:
	.4byte MsgDeriHeyaFishermenWaiting
	.section .text.x02008614,"ax",%progbits
	.global Func_02000614
	.thumb_func
Func_02000614:
	push {r5, lr}
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r5, .L_02008668
	adds r0, r5, #0
	bl Engine_EventSetMessage
	movs r1, #0
	movs r0, #12
	bl UiText_OpenMessageAtObject
	bl Func_02000dd4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200864c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Engine_EventSetMessage
	b .L_02008658
.L_0200864c:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Engine_EventSetMessage
.L_02008658:
	movs r0, #12
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
	pop {r5, pc}
	.2byte 0x0000
.L_02008668:
	.4byte MsgDeriHeyaGoingToMadora
	.section .text.x0200866c,"ax",%progbits
	.global Func_0200066c
	.thumb_func
Func_0200066c:
	push {r5, lr}
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r5, .L_020086c0
	adds r0, r5, #0
	bl Engine_EventSetMessage
	movs r1, #0
	movs r0, #14
	bl UiText_OpenMessageAtObject
	bl Func_02000dd4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020086a4
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Engine_EventSetMessage
	b .L_020086b0
.L_020086a4:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Engine_EventSetMessage
.L_020086b0:
	movs r0, #14
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
	pop {r5, pc}
	.2byte 0x0000
.L_020086c0:
	.4byte MsgDeriHeyaLookingForBoat
	.section .text.x020086c4,"ax",%progbits
	.global Func_020006c4
	.thumb_func
Func_020006c4:
	push {lr}
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r0, .L_02008700
	bl Engine_EventSetMessage
	movs r1, #0
	movs r0, #26
	bl Engine_EventShowMessage
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #26
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #0
	bl Engine_EventShowMessage
	bl Engine_EventEnd
	pop {pc}
.L_02008700:
	.4byte MsgDeriHeyaWatchingPractice
	.section .text.x02008704,"ax",%progbits
	.global Func_02000704
	.thumb_func
Func_02000704:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, .L_0200872c
	ldr r5, [r3, #108]
	bl Func_02000dcc
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #188
	adds r5, r5, r3
	ldr r1, [r5]
	movs r3, #1
	adds r1, #35
	ldrb r2, [r1]
	orrs r3, r2
	movs r2, #253
	ands r3, r2
	strb r3, [r1]
	pop {r5, pc}
.L_0200872c:
	.4byte Data_02000e04
	.section .text.x02008730,"ax",%progbits
	.global Func_02000730
	.thumb_func
Func_02000730:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	ldr r0, [r0, #72]
	movs r5, #229
	mov r8, r0
	movs r0, #9
	bl Object_GetById
	ldr r6, [r0, #40]
	movs r0, #9
	bl Object_GetById
	lsls r5, r5, #1
	ldr r7, [r0, #12]
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	adds r0, r5, #0
	bl Func_02000d14
	cmp r0, #0
	blt .L_020087a0
	movs r0, #9
	bl Object_GetById
	movs r1, #3
	bl Func_02000da4
	movs r0, #83
	bl Func_02000dfc
	adds r0, r5, #0
	movs r1, #2
	bl Func_02000ccc
	movs r1, #1
	ldr r0, .L_02008870
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	bl PartyInventory_Add
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02000d4c
	movs r1, #232
	movs r2, #194
	b .L_020087d0
.L_020087a0:
	movs r0, #9
	bl Object_GetById
	movs r1, #3
	bl Func_02000da4
	movs r0, #83
	bl Func_02000dfc
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000878
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_020087e4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02000d4c
	movs r1, #232
	movs r2, #196
.L_020087d0:
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02000d4c
	movs r0, #162
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02008866
.L_020087e4:
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl Func_02000d4c
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	str r6, [r0, #40]
	movs r0, #9
	bl Object_GetById
	str r7, [r0, #12]
	movs r0, #9
	bl Object_GetById
	mov r3, r8
	str r3, [r0, #72]
	movs r0, #9
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r0, #9
	bl Object_GetById
	bl Func_02000c9c
	movs r1, #1
	movs r0, #9
	bl Engine_ActorSetSpritePriority
	movs r0, #9
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	ldr r5, .L_02008874
	str r5, [r0, #28]
	movs r0, #9
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #9
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	movs r1, #232
	movs r2, #196
	strb r3, [r0]
	lsls r1, r1, #16
	movs r0, #9
	lsls r2, r2, #16
	bl Func_02000d4c
.L_02008866:
	bl Engine_EventEnd
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008870:
	.4byte MsgItemReceived
.L_02008874:
	.4byte 0x00013333
	.section .text.x02008878,"ax",%progbits
	.global Func_02000878
	.thumb_func
Func_02000878:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	mov r10, r3
	lsls r2, r2, #1
	add r2, r10
	mov r8, r2
	movs r3, #0
	ldrsh r2, [r2, r3]
	sub sp, #8
	adds r6, r0, #0
	mov r9, r2
	bl PartyInventory_Add
	movs r3, #1
	adds r7, r0, #0
	negs r3, r3
	cmp r7, r3
	beq .L_020088ac
	b .L_020089e8
.L_020088ac:
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	ldr r0, .L_02008a3c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r0, .L_02008a40
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_020088c4:
	ldr r2, .L_02008a44
	movs r1, #1
	mov r8, r2
	mov r0, r8
	bl UiText_ShowPositionedMessageAndWait
	add r0, sp, #4
	mov r1, sp
	bl Func_02000df4
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_02008940
	adds r0, r6, #0
	bl Func_02000ce4
	ldrb r2, [r0, #3]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_02008900
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #4
	b .L_02008970
.L_02008900:
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #1
	movs r1, #5
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_02000dbc
	adds r5, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r5, #0
	bne .L_020088c4
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #226
	lsls r3, r3, #1
	add r3, r10
	mov r2, r9
	strh r2, [r3]
	b .L_02008a2c
.L_02008940:
	ldr r0, [sp, #4]
	bl Owner_GetState
	ldr r1, [sp, #0]
	ldr r0, [sp, #4]
	bl Inventory_GetQuantity
	adds r1, r6, #0
	adds r5, r0, #0
	ldr r0, [sp, #4]
	bl Inventory_CountItem
	cmp r0, #29
	ble .L_02008978
	ldr r0, [sp, #4]
	movs r1, #1
	bl Func_02000ccc
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	mov r0, r8
	adds r0, #7
.L_02008970:
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_020088c4
.L_02008978:
	cmp r5, #0
	ble .L_0200898a
.L_0200897c:
	ldr r0, [sp, #4]
	ldr r1, [sp, #0]
	subs r5, #1
	bl Inventory_Discard
	cmp r5, #0
	bne .L_0200897c
.L_0200898a:
	ldr r0, [sp, #4]
	bl Owner_RefreshClassActions
	ldr r0, [sp, #4]
	bl Owner_RecalculateStats
	adds r0, r6, #0
	bl PartyInventory_Add
	adds r7, r0, #0
	movs r0, #83
	bl Func_02000dfc
	ldr r3, .L_02008a48
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r7, r3
	bne .L_020089c4
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	ldr r0, .L_02008a3c
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWait
	b .L_020089dc
.L_020089c4:
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	adds r0, r7, #0
	movs r1, #1
	bl Func_02000ccc
	ldr r0, .L_02008a4c
	movs r1, #3
	bl UiText_ShowPositionedMessageAndWait
.L_020089dc:
	movs r3, #226
	lsls r3, r3, #1
	add r3, r10
	mov r2, r9
	strh r2, [r3]
	b .L_02008a2c
.L_020089e8:
	movs r0, #83
	bl Func_02000dfc
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	ldr r5, .L_02008a3c
	movs r1, #3
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_02008a48
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r7, r3
	beq .L_02008a26
	adds r0, r6, #0
	movs r1, #2
	bl Func_02000ccc
	adds r0, r7, #0
	movs r1, #1
	bl Func_02000ccc
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_02008a26:
	mov r3, r9
	mov r2, r8
	strh r3, [r2]
.L_02008a2c:
	adds r0, r7, #0
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a3c:
	.4byte MsgItemReceived
.L_02008a40:
	.4byte MsgItemCannotCarry
.L_02008a44:
	.4byte MsgItemDropPrompt
.L_02008a48:
	.4byte gPartyState
.L_02008a4c:
	.4byte MsgItemGiven
	.section .text.x02008a50,"ax",%progbits
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	push {r5, r6, lr}
	movs r1, #192
	lsls r1, r1, #18
	ldr r3, [r1, #108]
	movs r0, #214
	movs r2, #133
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	ldr r5, .L_02008bf8
	str r2, [r3]
	subs r2, #41
	adds r3, r5, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008bfc
	cmp r2, r3
	bne .L_02008b0c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #7
	bne .L_02008a8a
	bl Func_02000c58
	b .L_02008bf2
.L_02008a8a:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ac0
	movs r1, #5
	movs r0, #24
	bl Object_SetModeById
	movs r0, #24
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #5
	movs r0, #25
	bl Object_SetModeById
	movs r0, #25
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_02008ac0:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #19
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #2
	orrs r5, r3
	strb r5, [r0]
	movs r0, #19
	bl Engine_ActorSetSpritePriority
	movs r0, #19
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	b .L_02008bf2
.L_02008b0c:
	ldr r3, .L_02008c00
	cmp r2, r3
	bne .L_02008bf2
	ldr r6, [r1, #32]
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	bl Func_02000dac
	movs r0, #162
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008b90
	movs r0, #9
	bl Object_GetById
	movs r1, #229
	lsls r1, r1, #1
	bl Func_02000cbc
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #1
	movs r0, #9
	bl Engine_ActorSetSpritePriority
	movs r0, #9
	bl Object_GetById
	ldr r5, .L_02008c04
	str r5, [r0, #28]
	movs r0, #9
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #9
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #4
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	b .L_02008bac
.L_02008b90:
	movs r0, #9
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_02008bac:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008be8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02000d4c
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02000d4c
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02000d4c
	ldrb r3, [r6, #23]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r6, #23]
	b .L_02008bee
.L_02008be8:
	ldr r0, .L_02008c08
	bl Func_02000dc4
.L_02008bee:
	bl Engine_EventEnd
.L_02008bf2:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008bf8:
	.4byte gPartyState
.L_02008bfc:
	.4byte 0x0000000b
.L_02008c00:
	.4byte 0x0000000c
.L_02008c04:
	.4byte 0x00013333
.L_02008c08:
	.4byte Data_02000e04
	.section .text.x02008c0c,"ax",%progbits
	.global Func_02000c0c
	.thumb_func
Func_02000c0c:
	push {lr}
	ldr r3, .L_02008c50
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c54
	sub sp, #8
	cmp r2, r3
	bne .L_02008c4a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c4a
	movs r0, #0
	bl Func_02000ca4
	movs r3, #10
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #29
	movs r1, #33
	movs r2, #10
	movs r3, #6
	bl Func_02000cac
.L_02008c4a:
	movs r0, #0
	add sp, #8
	pop {pc}
.L_02008c50:
	.4byte gPartyState
.L_02008c54:
	.4byte 0x0000000c
	.section .text.x02008c58,"ax",%progbits
	.global Func_02000c58
	.thumb_func
Func_02000c58:
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
	.section .text.x02008c74,"ax",%progbits
	.global Func_02000c74
	.thumb_func
Func_02000c74:
	push {lr}
	bl Func_02000d9c
	pop {pc}
	.section .rodata.x02008e04,"a",%progbits
	.global Data_02000e04
Data_02000e04:
	.4byte 0x0220000a
	.4byte 0x0221000b
	.4byte 0x0222000c
	.4byte 0x0000ffff
.L_02008e14:
	.4byte 0x00000016
	.4byte 0x0000001e
	@ Property 0x1e differs in the first and third Japanese scripts.
	.ifdef TLA_EDITION_JA
	.4byte 0x00000083
	.else
	.4byte 0x00000081
	.endif
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
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
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
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
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
.L_02008ecc:
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
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
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
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
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
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
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
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	@ Property 0x1e differs in the first and third Japanese scripts.
	.ifdef TLA_EDITION_JA
	.4byte 0x00000083
	.else
	.4byte 0x00000081
	.endif
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00020000
	.4byte 0x00060000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000004
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
	.global Data_0200116c
Data_0200116c:
	.4byte 0x0000000b
	.4byte 0x1010100a
	.4byte 0xffffffff
	.4byte 0x1020200a
	.4byte 0xffffffff
	.4byte 0x1030300a
	.4byte 0xffffffff
	.4byte 0x1040400a
	.4byte 0xffffffff
	.4byte 0x1050500a
	.4byte 0xffffffff
	.4byte 0x1060600a
	.4byte 0xffffffff
	.4byte 0x10703085
	.4byte 0xffffffff
	.4byte 0x0000000c
	.4byte 0x1010700a
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020011b8
Data_020011b8:
	.4byte 0x0000000b
	.4byte 0x1010100d
	.4byte 0xffffffff
	.4byte 0x1020200d
	.4byte 0xffffffff
	.4byte 0x1030300d
	.4byte 0xffffffff
	.4byte 0x1040400d
	.4byte 0xffffffff
	.4byte 0x1050500d
	.4byte 0xffffffff
	.4byte 0x1060600d
	.4byte 0xffffffff
	.4byte 0x10703085
	.4byte 0xffffffff
	.4byte 0x0000000c
	.4byte 0x1010700d
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001204
Data_02001204:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte .L_02008e14
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00012000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0001e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d40000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013fc
Data_020013fc:
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x014d0000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00024000
	.4byte 0x02230122
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x02240122
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x02250122
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020014a4
Data_020014a4:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte .L_02008e14
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00012000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d40000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte .L_02008ecc
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff0012
	.4byte 0x00000001
	.4byte 0x00a10000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00014000
	.4byte 0xffff0011
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte .L_02008ecc
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020016e4
Data_020016e4:
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
	.4byte MsgFieldRikisReallyInForItWhen
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldThatNoGoodKidOfMine
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldIWonderWhereOurLittleTavi
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldTaviNeverMissesThreeOclockSnacksies
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldIfYoureLookingForABoat
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldIfYouHeadSouthYoullFind
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldInAllMyYearsAsMayor
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldTheWaveWasAwfulButThe
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldALotOfFolkSaidThey
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldBetweenThePiratesAndTheTidal
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000270
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020002d0
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_02000330
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldWeRunAVerySmallInn
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFieldIDontKnowWhenIllBe
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFieldIHearTheSeafoodInMadra
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgSeaOfTimeCurrents
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgSeaOfTimeRedRocks
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgSeaOfTimeThreeGenerations
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_020002d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldRikiTaviWhereAreYouWhat
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldThatTavisABadInfluenceOn
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIllBetThatWhereverTaviIs
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldIfTaviIsntHomeSoonWere
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldIWonderHowTheRoadTo
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldIWonderIfTheHolyMan
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldAtLeastNobodySeemsToHave
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldIStillDontKnowWhatThat
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldWeShouldFindADifferentWay
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldNowThatBriggsIsLockedAway
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldImGladImNotGoingTo
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgDeriHeyaScamps
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldWeCantSailAndWeCant
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldLookAtAllTheseGuestsWe
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldAfterAllThatShakingIWouldnt
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgFieldImSoTornDoILike
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgSeaOfTimeHusbandDizzy
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgSeaOfTimeYeppToldMe
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgSeaOfTimeGiveUp
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte DeriHeya_TalkScamps
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001924
Data_02001924:
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
	.4byte Func_02000390
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldTravelersWithNowhereToGoThis
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001990
Data_02001990:
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
	.4byte MsgFieldLetMeTellYouKidsAre
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldWhoStaysOutsideUntilHesAbsolutely
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldOurLittleOneIsFinallyHome
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldTaviWolfedDownHisSnackAnd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_02000614
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldIThinkMadraIsEastOf
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_0200066c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldItsTooDangerousToGoTo
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldWhoWasThatGuyIWonder
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldWhyDidThePirateBriggsPlague
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_02000554
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgDeriHeyaDinnerShowPractice
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgDeriHeyaSeafoodWhen
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgDeriHeyaVegetablesAgain
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgFieldMmmMmnphDadWasSoMad
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgFieldMmmphMmmmDontTellAnyoneWe
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_020006c4
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgSeaOfTimeCurrents
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgSeaOfTimeRedRocks
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgSeaOfTimeThreeGenerations
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Func_020002d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldDarnThatRikiAhImGlad
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldHeeHeeMyRikiIsMommys
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIGuessRikiNextDoorMade
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldOnceTaviFinishesEatingImPutting
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldDailaGotHitPrettyHardI
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldSomeoneShouldSeeHowTheRoad
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldHeSaidHedPayMeA
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldTheRoadToMadraGoesThrough
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldMaybeThatGuyWashedUpOn
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldBriggsProbablyHasLotsOfMen
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldThisTownMayBeDirtPoor
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgDeriHeyaMazeTreasure
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgDeriHeyaTiredPerformer
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgDeriHeyaDragonKingLine
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgDeriHeyaBadShows
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgDeriHeyaGuestDance
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgFieldIStillWantToCatchThat
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgFieldImSureGladIMadeIt
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgDeriHeyaSeafoodStory
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgSeaOfTimeHusbandDizzy
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgSeaOfTimeYeppToldMe
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgSeaOfTimeGiveUp
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte DeriHeya_TalkMazeTreasure
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001c18
Data_02001c18:
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
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte Func_02000c74
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte Func_02000c74
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte Func_02000c74
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte Func_02000c74
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000498
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgDeriHeyaBoringVillage
	.4byte 0x00001815
	.4byte 0x0220000a
	.4byte Func_02000704
	.4byte 0x00001815
	.4byte 0x0221000b
	.4byte Func_02000704
	.4byte 0x00001815
	.4byte 0x0222000c
	.4byte Func_02000704
	.4byte 0x00000003
	.4byte 0x0a200028
	.4byte Func_02000730
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
