.syntax unified
	.thumb
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, r6, r7, lr}
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_020081a0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_020081a4
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02008194
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #162
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_02000550
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008194
	ldr r5, [r6, #80]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r2, #203
	lsls r2, r2, #18
	lsls r3, r3, #3
	adds r1, r6, #0
	adds r3, r3, r2
	adds r1, #85
	strb r7, [r1]
	movs r2, #174
	str r3, [r6, #8]
	movs r3, #160
	lsls r2, r2, #18
	lsls r3, r3, #16
	str r3, [r6, #12]
	str r2, [r6, #16]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r1]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ldr r1, .L_020081a8
	adds r0, r6, #0
	strb r3, [r5, #9]
	bl Func_02000548
	adds r0, r6, #0
	movs r1, #1
	bl Func_02000540
.L_02008194:
	ldr r2, .L_020081ac
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081a0:
	.4byte gPartyState
.L_020081a4:
	.4byte gFrameCount
.L_020081a8:
	.4byte gData_02000618
.L_020081ac:
	.4byte gData_020007e4
	.section .bss,"aw",%nobits
	.global gData_020007e4
gData_020007e4:
