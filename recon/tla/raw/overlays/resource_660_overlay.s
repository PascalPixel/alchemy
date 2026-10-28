.syntax unified
	.thumb
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
	.4byte gAdventure
.L_02008700:
	.4byte 0x00000040
.L_02008704:
	.4byte 0x014f0000
.L_02008708:
	.4byte 0x01590000
