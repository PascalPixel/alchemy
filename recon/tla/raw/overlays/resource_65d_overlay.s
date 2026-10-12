.syntax unified
	.thumb
	.section .text.x0200809c,"ax",%progbits
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02000648
	movs r0, #0
	bl Func_020006d0
	movs r5, #8
.L_020080b0:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_020080c2
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_020080c2:
	adds r5, #1
	cmp r5, #63
	bls .L_020080b0
	movs r2, #170
	lsls r2, r2, #1
	adds r7, r6, r2
	movs r3, #0
	ldrsh r5, [r7, r3]
	movs r0, #158
	bl Func_020006f0
	subs r5, #1
	ldr r0, .L_02008144
	lsls r4, r5, #3
	adds r3, r4, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r4]
	bl Func_02000610
	ldr r3, .L_02008148
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	ldr r0, [r6]
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r6]
	bl Object_SetModeById
	cmp r5, #4
	beq .L_0200812c
	movs r2, #8
	ldr r0, [r6]
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_CenterOnColumn
	movs r0, #10
	bl Battle_WaitMode0
.L_0200812c:
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl Func_020006b8
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02000650
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008144:
	.4byte Data_0200073c
.L_02008148:
	.4byte gPartyState
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #16
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	ldr r5, .L_02008430
	strb r3, [r0]
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #240
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008434
	cmp r2, r3
	bne .L_02008414
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008426
	movs r0, #0
	bl Func_02000620
	movs r0, #1
	bl Func_02000620
	movs r0, #2
	bl Func_02000620
	movs r5, #16
	movs r0, #56
	movs r1, #46
	movs r2, #30
	movs r3, #46
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000618
	movs r0, #56
	movs r1, #174
	movs r2, #30
	movs r3, #174
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000618
	movs r2, #15
	movs r3, #23
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r2
	movs r0, #28
	movs r1, #23
	movs r2, #16
	movs r3, #16
	bl Func_02000628
	movs r3, #21
	movs r2, #88
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #2
	bl Func_02000628
	movs r3, #89
	str r3, [sp, #4]
	movs r6, #17
	movs r0, #28
	movs r1, #90
	movs r2, #4
	movs r3, #4
	str r6, [sp, #0]
	bl Func_02000628
	movs r5, #91
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #1
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl Func_02000628
	movs r3, #19
	str r3, [sp, #0]
	movs r0, #28
	movs r1, #90
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02000628
	mov r3, r8
	movs r5, #92
	str r3, [sp, #0]
	movs r0, #28
	movs r1, #92
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02000628
	movs r0, #30
	movs r1, #92
	movs r2, #2
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02000628
	b .L_02008426
.L_02008414:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #91
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008426
	bl Func_020001e0
.L_02008426:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008430:
	.4byte gPartyState
.L_02008434:
	.4byte 0x00000038
