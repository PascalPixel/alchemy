.syntax unified
	.thumb
	.global Func_0811f444
	.thumb_func
Func_0811f444:
	push {r5, r6, r7, lr}
	sub sp, #16
	adds r6, r0, #0
	bl Owner_GetState
	movs r5, #0
	adds r7, r0, #0
	b .L_0811f470
.L_0811f454:
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_0811f468
	movs r1, #4
	bl Animation_ApplyChildArgumentFar
	b .L_0811f46e
.L_0811f468:
	movs r1, #5
	bl Animation_ApplyChildArgumentFar
.L_0811f46e:
	adds r5, #1
.L_0811f470:
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r5, #0
	ldr r0, [r0]
	bl GetMotionRecord
	cmp r0, #0
	bne .L_0811f454
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0811f4d0
	movs r5, #0
	mov r7, sp
	b .L_0811f4a4
.L_0811f494:
	ldr r2, [r0, #40]
	lsls r3, r5, #2
	str r0, [r7, r3]
	movs r3, #6
	strb r3, [r2, #5]
	movs r3, #255
	strb r3, [r2, #22]
	adds r5, #1
.L_0811f4a4:
	adds r0, r6, #0
	bl GetBattleObjectSlot
	adds r1, r5, #0
	ldr r0, [r0]
	bl GetMotionRecord
	cmp r0, #0
	bne .L_0811f494
	movs r0, #4
	bl WaitFrames
	adds r0, r6, #0
	bl Func_0811f3b8
	adds r0, r7, #0
	adds r1, r5, #0
	bl Object_SetPositionAndResetMotionFar + 0x18
	adds r0, r6, #0
	bl Func_0811bc64
.L_0811f4d0:
	add sp, #16
	pop {r5, r6, r7, pc}
