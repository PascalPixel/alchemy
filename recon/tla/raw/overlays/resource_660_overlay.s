.syntax unified
	.thumb
	.section .text.x02008298,"ax",%progbits
	.balign 4
	.global Func_02000298
	.thumb_func
Func_02000298:
	push {r5, r6, lr}
	ldr r6, .L_020082d8
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r0, [r6]
	bl Object_GetById
	ldrh r5, [r0, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r5, r5, r3
	ldr r3, .L_020082d4
	ldr r1, [r6]
	ands r5, r3
	lsls r5, r5, #16
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	asrs r5, r5, #16
	movs r3, #192
	lsls r5, r5, #16
	lsls r3, r3, #24
	cmp r5, r3
	bne .L_020082dc
	movs r0, #8
	bl Func_020008a4
	b .L_02008300
.L_020082d4:
	.4byte 0xffffc000
.L_020082d8:
	.4byte gPartyState
.L_020082dc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082f2
	ldr r0, .L_02008310
	bl Func_02000844
	b .L_020082f8
.L_020082f2:
	ldr r0, .L_02008314
	bl Func_02000844
.L_020082f8:
	movs r0, #8
	movs r1, #0
	bl Func_0200085c
.L_02008300:
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008310:
	.4byte MsgScene660No66
.L_02008314:
	.4byte MsgScene660No31
	.section .text.x02008318,"ax",%progbits
	.balign 4
	.global Func_02000318
	.thumb_func
Func_02000318:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	bl Func_020007f4
	movs r0, #0
	bl Func_02000884
	ldr r7, .L_02008394
	adds r0, r7, #0
	bl Func_02000844
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	ldr r3, .L_02008398
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008384
	adds r0, r7, #2
	bl Func_02000844
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008368
	adds r0, r5, #0
	b .L_0200837c
.L_02008368:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r0, r5, #0
	adds r3, #1
	strh r3, [r2]
.L_0200837c:
	movs r1, #0
	bl Func_0200085c
	b .L_0200838c
.L_02008384:
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200085c
.L_0200838c:
	bl Func_020007fc
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008394:
	.4byte MsgScene660No01
.L_02008398:
	.4byte gPartyState
	.section .text.x020084f0,"ax",%progbits
	.balign 4
	.global Func_020004f0
	.thumb_func
Func_020004f0:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	ldr r1, .L_02008558
	ldr r4, .L_0200855c
	cmp r0, #0
	beq .L_0200851c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, r4
	beq .L_02008538
	ldr r3, .L_02008560
	cmp r2, r3
	bne .L_0200854a
	ldr r0, .L_02008564
	b .L_02008556
.L_0200851c:
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, r4
	bne .L_02008540
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #99
	bne .L_0200853c
.L_02008538:
	ldr r0, .L_02008568
	b .L_02008556
.L_0200853c:
	ldr r0, .L_0200856c
	b .L_02008556
.L_02008540:
	ldr r3, .L_02008560
	cmp r2, r3
	bne .L_0200854a
	ldr r0, .L_02008570
	b .L_02008556
.L_0200854a:
	ldr r3, .L_02008574
	cmp r2, r3
	bne .L_02008554
	ldr r0, .L_02008578
	b .L_02008556
.L_02008554:
	ldr r0, .L_0200857c
.L_02008556:
	pop {pc}
.L_02008558:
	.4byte gPartyState
.L_0200855c:
	.4byte 0x00000040
.L_02008560:
	.4byte 0x00000041
.L_02008564:
	.4byte Data_02000fe8
.L_02008568:
	.4byte Data_02000e44
.L_0200856c:
	.4byte Data_02000b80
.L_02008570:
	.4byte Data_02000d18
.L_02008574:
	.4byte 0x00000042
.L_02008578:
	.4byte Data_02000de4
.L_0200857c:
	.4byte Data_02000b5c
	.section .text.x02008580,"ax",%progbits
	.balign 4
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	ldr r5, .L_020086fc
	adds r2, #255
	str r2, [r3]
	adds r2, #11
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	bl Func_0200087c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008700
	cmp r2, r3
	beq .L_020085c4
	b .L_020086ce
.L_020085c4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020085e2
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #99
	bne .L_02008600
.L_020085e2:
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_0200081c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_0200081c
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_EnableActionAndSetCallback
	b .L_020086ac
.L_02008600:
	movs r5, #128
	lsls r5, r5, #6
	movs r1, #153
	movs r0, #17
	lsls r1, r1, #17
	ldr r2, .L_02008704
	adds r3, r5, #0
	bl Func_02000824
	movs r3, #192
	movs r2, #168
	lsls r3, r3, #7
	lsls r2, r2, #17
	ldr r1, .L_02008708
	movs r0, #18
	bl Func_02000824
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #20
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #17
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #17
	bl Object_GetById
	str r6, [r0, #12]
	movs r0, #18
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #18
	bl Object_GetById
	str r6, [r0, #12]
	movs r0, #19
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #19
	bl Object_GetById
	str r6, [r0, #12]
	movs r0, #20
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #20
	bl Object_GetById
	movs r1, #224
	lsls r1, r1, #8
	str r6, [r0, #12]
	movs r0, #17
	bl Func_02000894
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r1, r5, #0
	movs r0, #18
	bl Func_02000894
	movs r0, #18
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
.L_020086ac:
	ldr r3, .L_020086fc
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #99
	bne .L_020086ce
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086ce
	bl Func_02000710
.L_020086ce:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086e4
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_020086e4:
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086f8
	movs r0, #160
	lsls r0, r0, #4
	bl Func_020007dc
.L_020086f8:
	movs r0, #0
	pop {r5, r6, pc}
.L_020086fc:
	.4byte gPartyState
.L_02008700:
	.4byte 0x00000040
.L_02008704:
	.4byte 0x014f0000
.L_02008708:
	.4byte 0x01590000
