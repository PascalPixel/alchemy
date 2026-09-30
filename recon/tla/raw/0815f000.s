.syntax unified
	.thumb
	.global Func_0815f000
	.thumb_func
Func_0815f000:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r2
	mov r10, r3
	adds r5, r0, #0
	mov r9, r1
	bl GetBattleObjectSlotFar
	adds r7, r0, #0
	adds r0, r5, #0
	ldr r6, [r7]
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	movs r3, #102
	adds r3, #255
	cmp r0, r3
	beq .L_0815f07a
	adds r2, #39
	cmp r0, r2
	beq .L_0815f07a
	adds r3, #16
	cmp r0, r3
	beq .L_0815f07a
	adds r2, #7
	cmp r0, r2
	beq .L_0815f07a
	mov r3, r8
	str r3, [r6, #52]
	mov r2, r10
	mov r3, r9
	str r2, [r6, #48]
	cmp r3, #0
	bne .L_0815f056
	ldr r3, [r6, #40]
	cmp r3, #0
	beq .L_0815f05a
.L_0815f056:
	ldr r3, [sp, #28]
	str r3, [r6, #40]
.L_0815f05a:
	adds r0, r6, #0
	bl Object_ResetMotion
	ldr r2, [r7, #12]
	ldr r3, [sp, #32]
	movs r1, #100
	adds r0, r3, #0
	muls r0, r2
	bl Math_Div
	ldr r3, [r7, #16]
	adds r1, r0, #0
	movs r2, #0
	adds r0, r6, #0
	bl Object_SetPosition
.L_0815f07a:
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	adds r2, r6, #0
	str r3, [r6, #72]
	adds r2, #90
	movs r3, #0
	str r3, [r6, #68]
	adds r0, r6, #0
	strb r3, [r2]
	movs r1, #5
	bl Object_SetMode
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
