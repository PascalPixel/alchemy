.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_0200807c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008080
	cmp r2, r3
	bne .L_02008064
	ldr r0, .L_02008084
	b .L_0200807a
.L_02008064:
	ldr r3, .L_02008088
	cmp r2, r3
	bne .L_0200806e
	ldr r0, .L_0200808c
	b .L_0200807a
.L_0200806e:
	ldr r3, .L_02008090
	cmp r2, r3
	bne .L_02008078
	ldr r0, .L_02008094
	b .L_0200807a
.L_02008078:
	ldr r0, .L_02008098
.L_0200807a:
	pop {pc}
.L_0200807c:
	.4byte gPartyState
.L_02008080:
	.4byte 0x00000040
.L_02008084:
	.4byte Data_02000964
.L_02008088:
	.4byte 0x00000041
.L_0200808c:
	.4byte Data_02000ab4
.L_02008090:
	.4byte 0x00000042
.L_02008094:
	.4byte Data_02000b2c
.L_02008098:
	.4byte Data_0200094c
	.section .text.x0200809c,"ax",%progbits
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {r5, lr}
	ldr r3, .L_020080e0
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
	ldr r2, .L_020080dc
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020080e4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080e4
	movs r0, #10
	adds r1, r5, #0
	bl Func_0200089c
	b .L_02008108
.L_020080dc:
	.4byte 0xffffc000
.L_020080e0:
	.4byte gPartyState
.L_020080e4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080fa
	ldr r0, .L_0200810c
	bl Func_02000844
	b .L_02008100
.L_020080fa:
	ldr r0, .L_02008110
	bl Func_02000844
.L_02008100:
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200085c
.L_02008108:
	pop {r5, pc}
	.2byte 0x0000
.L_0200810c:
	.4byte MsgScene660No52
.L_02008110:
	.4byte MsgScene660No17
	.section .text.x02008114,"ax",%progbits
	.global Func_02000114
	.thumb_func
Func_02000114:
	push {r5, lr}
	ldr r3, .L_02008158
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
	ldr r2, .L_02008154
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200815c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200815c
	movs r0, #11
	adds r1, r5, #0
	bl Func_0200089c
	b .L_02008180
.L_02008154:
	.4byte 0xffffc000
.L_02008158:
	.4byte gPartyState
.L_0200815c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008172
	ldr r0, .L_02008184
	bl Func_02000844
	b .L_02008178
.L_02008172:
	ldr r0, .L_02008188
	bl Func_02000844
.L_02008178:
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200085c
.L_02008180:
	pop {r5, pc}
	.2byte 0x0000
.L_02008184:
	.4byte MsgScene660No54
.L_02008188:
	.4byte MsgScene660No19
	.section .text.x0200818c,"ax",%progbits
	.global Func_0200018c
	.thumb_func
Func_0200018c:
	push {r5, r6, lr}
	ldr r3, .L_020081c4
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
	ldr r2, .L_020081c0
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020081c8
	movs r0, #12
	adds r1, r6, #0
	bl Func_0200089c
	b .L_02008226
	.2byte 0x0000
.L_020081c0:
	.4byte 0xffffc000
.L_020081c4:
	.4byte gPartyState
.L_020081c8:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081e6
	ldr r0, .L_02008228
	bl Func_02000844
	adds r0, r6, #0
	movs r1, #0
	bl Func_0200085c
	b .L_02008226
.L_020081e6:
	ldr r5, .L_0200822c
	adds r0, r5, #0
	bl Func_02000844
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_0200088c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008212
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000844
	b .L_0200821e
.L_02008212:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000844
.L_0200821e:
	adds r0, r6, #0
	movs r1, #0
	bl Func_0200085c
.L_02008226:
	pop {r5, r6, pc}
.L_02008228:
	.4byte MsgScene660No56
.L_0200822c:
	.4byte MsgScene660No21
	.section .text.x02008230,"ax",%progbits
	.global Func_02000230
	.thumb_func
Func_02000230:
	push {r5, lr}
	ldr r3, .L_0200827c
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
	ldr r2, .L_02008278
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008262
	movs r0, #3
	adds r1, r5, #0
	bl Func_020008ac
	b .L_02008292
.L_02008262:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008284
	ldr r0, .L_02008280
	bl Func_02000844
	b .L_0200828a
.L_02008278:
	.4byte 0xffffc000
.L_0200827c:
	.4byte gPartyState
.L_02008280:
	.4byte MsgScene660No58
.L_02008284:
	ldr r0, .L_02008294
	bl Func_02000844
.L_0200828a:
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200085c
.L_02008292:
	pop {r5, pc}
.L_02008294:
	.4byte MsgScene660No23
	.section .text.x02008298,"ax",%progbits
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
	.section .text.x0200839c,"ax",%progbits
	.global Func_0200039c
	.thumb_func
Func_0200039c:
	push {lr}
	bl Func_020007f4
	movs r0, #0
	bl Func_02000884
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083be
	ldr r0, .L_020083d4
	bl Func_02000844
	b .L_020083c4
.L_020083be:
	ldr r0, .L_020083d8
	bl Func_02000844
.L_020083c4:
	movs r0, #17
	movs r1, #0
	bl Func_0200085c
	bl Func_020007fc
	pop {pc}
	.2byte 0x0000
.L_020083d4:
	.4byte MsgScene660No60
.L_020083d8:
	.4byte MsgScene660No25
	.section .text.x020083dc,"ax",%progbits
	.global Func_020003dc
	.thumb_func
Func_020003dc:
	push {lr}
	bl Func_020007f4
	movs r0, #0
	bl Func_02000884
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083fe
	ldr r0, .L_02008414
	bl Func_02000844
	b .L_02008404
.L_020083fe:
	ldr r0, .L_02008418
	bl Func_02000844
.L_02008404:
	movs r0, #18
	movs r1, #0
	bl Func_0200085c
	bl Func_020007fc
	pop {pc}
	.2byte 0x0000
.L_02008414:
	.4byte MsgScene660No61
.L_02008418:
	.4byte MsgScene660No26
	.section .text.x0200841c,"ax",%progbits
	.global Func_0200041c
	.thumb_func
Func_0200041c:
	push {lr}
	bl Func_020007f4
	movs r0, #0
	bl Func_02000884
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200843e
	ldr r0, .L_02008454
	bl Func_02000844
	b .L_02008444
.L_0200843e:
	ldr r0, .L_02008458
	bl Func_02000844
.L_02008444:
	movs r0, #17
	movs r1, #0
	bl Func_0200085c
	bl Func_020007fc
	pop {pc}
	.2byte 0x0000
.L_02008454:
	.4byte MsgScene660No64
.L_02008458:
	.4byte MsgScene660No29
	.section .text.x0200845c,"ax",%progbits
	.global Func_0200045c
	.thumb_func
Func_0200045c:
	push {lr}
	bl Func_020007f4
	movs r0, #0
	bl Func_02000884
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200847e
	ldr r0, .L_02008494
	bl Func_02000844
	b .L_02008484
.L_0200847e:
	ldr r0, .L_02008498
	bl Func_02000844
.L_02008484:
	movs r0, #18
	movs r1, #0
	bl Func_0200085c
	bl Func_020007fc
	pop {pc}
	.2byte 0x0000
.L_02008494:
	.4byte MsgScene660No65
.L_02008498:
	.4byte MsgScene660No30
	.section .text.x0200849c,"ax",%progbits
	.global Func_0200049c
	.thumb_func
Func_0200049c:
	push {r5, lr}
	ldr r5, .L_020084ec
	adds r0, r5, #0
	bl Func_02000844
	movs r1, #0
	movs r0, #11
	bl UiText_OpenMessageAtObject
	bl Func_0200088c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020084ca
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000844
	b .L_020084d6
.L_020084ca:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000844
.L_020084d6:
	movs r0, #11
	movs r1, #0
	bl Func_0200085c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #26
	bl GameFlag_SetBit
	pop {r5, pc}
	.2byte 0x0000
.L_020084ec:
	.4byte MsgScene660No47
	.section .text.x020084f0,"ax",%progbits
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
	.section .text.x02008710,"ax",%progbits
	.global Func_02000710
	.thumb_func
Func_02000710:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_SetBit
	bl Func_020007f4
	movs r0, #0
	bl Func_02000884
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_020007cc
	movs r0, #86
	bl Func_020008b4
	movs r1, #0
	movs r2, #0
	ldr r0, .L_0200879c
	bl Func_020007e4
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_020007d4
	bl Func_020007bc
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_020087a0
	bl Func_02000844
	movs r2, #10
	movs r0, #16
	movs r1, #0
	bl Func_02000854
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_02000854
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_02000854
	bl Func_020007fc
	pop {pc}
.L_0200879c:
	.4byte MsgScene660No33
.L_020087a0:
	.4byte MsgScene660No34
