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
	.section .text.x02008078,"ax",%progbits
	.global Func_02000078
	.thumb_func
Func_02000078:
	push {lr}
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r0, .L_0200809c
	bl Func_02000604
	movs r0, #8
	movs r1, #0
	bl Func_02000614
	bl Func_020005cc
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_0200809c:
	.4byte 0x00001949
	.section .text.x020080a0,"ax",%progbits
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {lr}
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r0, .L_020080c4
	bl Func_02000604
	movs r0, #9
	movs r1, #0
	bl Func_02000614
	bl Func_020005cc
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020080c4:
	.4byte 0x0000229a
	.section .text.x020080c8,"ax",%progbits
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
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, lr}
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r5, .L_02008148
	adds r0, r5, #0
	bl Func_02000604
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	bl Func_02000634
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200812c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000604
	b .L_02008138
.L_0200812c:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000604
.L_02008138:
	movs r0, #10
	movs r1, #0
	bl Func_02000614
	bl Func_020005cc
	pop {r5, pc}
	.2byte 0x0000
.L_02008148:
	.4byte 0x00001932
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {r5, lr}
	ldr r3, .L_02008184
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
	ldr r2, .L_02008180
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008188
	movs r0, #13
	adds r1, r5, #0
	bl Func_0200063c
	b .L_02008196
	.2byte 0x0000
.L_02008180:
	.4byte 0xffffc000
.L_02008184:
	.4byte gPartyState
.L_02008188:
	ldr r0, .L_02008198
	bl Func_02000604
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000614
.L_02008196:
	pop {r5, pc}
.L_02008198:
	.4byte 0x00001940
	.section .text.x0200819c,"ax",%progbits
	.global Func_0200019c
	.thumb_func
Func_0200019c:
	push {r5, lr}
	ldr r3, .L_020081d4
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
	ldr r2, .L_020081d0
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020081d8
	movs r0, #4
	adds r1, r5, #0
	bl Func_0200064c
	b .L_020081f4
	.2byte 0x0000
.L_020081d0:
	.4byte 0xffffc000
.L_020081d4:
	.4byte gPartyState
.L_020081d8:
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r0, .L_020081f8
	bl Func_02000604
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000614
	bl Func_020005cc
.L_020081f4:
	pop {r5, pc}
	.2byte 0x0000
.L_020081f8:
	.4byte 0x00001943
	.section .text.x020081fc,"ax",%progbits
	.global Func_020001fc
	.thumb_func
Func_020001fc:
	push {r5, r6, lr}
	ldr r3, .L_02008230
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
	ldr r2, .L_0200822c
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008234
	adds r0, r6, #0
	bl Func_02000644
	b .L_02008282
.L_0200822c:
	.4byte 0xffffc000
.L_02008230:
	.4byte gPartyState
.L_02008234:
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r5, .L_02008284
	adds r0, r5, #0
	bl Func_02000604
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000634
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200826a
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000604
	b .L_02008276
.L_0200826a:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000604
.L_02008276:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02000614
	bl Func_020005cc
.L_02008282:
	pop {r5, r6, pc}
.L_02008284:
	.4byte 0x00001946
	.section .text.x02008288,"ax",%progbits
	.global Func_02000288
	.thumb_func
Func_02000288:
	push {r5, lr}
	ldr r3, .L_020082bc
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
	ldr r2, .L_020082b8
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020082c0
	adds r0, r5, #0
	bl Func_02000644
	b .L_020082dc
.L_020082b8:
	.4byte 0xffffc000
.L_020082bc:
	.4byte gPartyState
.L_020082c0:
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r0, .L_020082e0
	bl Func_02000604
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000614
	bl Func_020005cc
.L_020082dc:
	pop {r5, pc}
	.2byte 0x0000
.L_020082e0:
	.4byte 0x00002297
	.section .text.x020082e4,"ax",%progbits
	.global Func_020002e4
	.thumb_func
Func_020002e4:
	push {r5, lr}
	ldr r3, .L_02008318
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
	ldr r2, .L_02008314
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200831c
	adds r0, r5, #0
	bl Func_02000644
	b .L_02008338
.L_02008314:
	.4byte 0xffffc000
.L_02008318:
	.4byte gPartyState
.L_0200831c:
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r0, .L_0200833c
	bl Func_02000604
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000614
	bl Func_020005cc
.L_02008338:
	pop {r5, pc}
	.2byte 0x0000
.L_0200833c:
	.4byte 0x00002221
	.section .text.x02008340,"ax",%progbits
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
	.4byte 0x0000194f
.L_020084ac:
	.4byte 0x00001950
	.section .text.x020084b0,"ax",%progbits
	.global Func_020004b0
	.thumb_func
Func_020004b0:
	push {lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #57
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c6
	bl Func_020004dc
	b .L_020084d4
.L_020084c6:
	ldr r0, .L_020084d8
	bl Func_02000604
	movs r0, #18
	movs r1, #0
	bl Func_02000614
.L_020084d4:
	pop {pc}
	.2byte 0x0000
.L_020084d8:
	.4byte 0x00001951
	.section .text.x020084dc,"ax",%progbits
	.global Func_020004dc
	.thumb_func
Func_020004dc:
	push {r5, lr}
	bl Func_020005c4
	movs r0, #0
	bl Func_0200062c
	ldr r0, .L_0200859c
	bl Func_02000604
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #18
	bl Func_02000624
	ldr r5, .L_020085a0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #0
	ldr r1, [r5]
	movs r0, #18
	bl ObjectMotion_SetAngleToward
	movs r0, #18
	movs r1, #0
	bl Func_02000614
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_02000624
	movs r1, #4
	movs r0, #18
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_02000614
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #18
	bl Func_02000624
	movs r1, #0
	movs r0, #18
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008568
	movs r0, #18
	movs r1, #0
	bl Func_02000614
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008582
.L_02008568:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #18
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02000614
.L_02008582:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #57
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_SetBit
	bl Func_020005cc
	pop {r5, pc}
.L_0200859c:
	.4byte 0x0000194a
.L_020085a0:
	.4byte gPartyState
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
	.4byte 0x00001930
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001931
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020000f4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000193c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000193d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_0200014c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001942
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_0200019c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002194
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002195
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000470
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001935
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001936
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001937
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000193e
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000193f
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001941
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001944
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001945
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002197
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002198
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte Func_020004b0
	.4byte 0x00000173
	.4byte 0xffff00ce
	.4byte 0x00403049
	.4byte 0x00000173
	.4byte 0xffff00cf
	.4byte 0x0040304a
	.4byte 0x00000173
	.4byte 0xffff00d0
	.4byte 0x0040304b
	.4byte 0x00000173
	.4byte 0xffff00d1
	.4byte 0x00403057
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
	.4byte 0x00002298
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002299
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020002e4
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_020001fc
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001949
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000229a
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000229b
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000229c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002222
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte Func_02000078
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte Func_020000a0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
