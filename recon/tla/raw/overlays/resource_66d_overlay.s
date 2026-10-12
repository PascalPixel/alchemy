.syntax unified
	.thumb
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #56
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_020080b4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #10
	beq .L_020080b0
	movs r3, #0
	strb r3, [r2]
.L_020080b0:
	pop {pc}
	.2byte 0x0000
.L_020080b4:
	.4byte gPartyState
	.section .text.x02008368,"ax",%progbits
	.global Func_02000368
	.thumb_func
Func_02000368:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	movs r5, #8
.L_0200837c:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200838e
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200838e:
	adds r5, #1
	cmp r5, #63
	bls .L_0200837c
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	bl Func_020005f4
	subs r5, #4
	ldr r0, .L_020083f8
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_02000534
	ldr r5, .L_020083fc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020005a4
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02000554
	pop {r5, r6, pc}
.L_020083f8:
	.4byte Data_0200064c
.L_020083fc:
	.4byte gPartyState
