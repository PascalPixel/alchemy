.syntax unified
	.thumb
	.section .text.x0200806c,"ax",%progbits
	.balign 4
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	movs r5, #8
.L_02008080:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008092
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008092:
	adds r5, #1
	cmp r5, #63
	bls .L_02008080
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	bl Func_02002c90
	subs r5, #1
	ldr r0, .L_02008100
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_02002ae8
	ldr r5, .L_02008104
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r2, #8
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_CenterOnColumn
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_02002c30
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02002b30
	pop {r5, r6, pc}
.L_02008100:
	.4byte gData_02003308
.L_02008104:
	.4byte gPartyState
