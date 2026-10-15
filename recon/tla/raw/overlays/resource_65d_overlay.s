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
