.syntax unified
	.thumb
	.global Func_0811c120
	.thumb_func
Func_0811c120:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	mov r11, r2
	mov r9, r3
	bl GetBattleObjectSlot
	adds r5, r0, #0
	adds r0, r6, #0
	bl GetBattleObjectSlot
	ldr r7, [r5]
	ldr r6, [r0]
	movs r2, #75
	mov r8, r2
	ldr r3, [r6, #8]
	ldr r2, [r7, #8]
	movs r1, #100
	subs r3, r3, r2
	mov r0, r8
	muls r0, r3
	mov r10, r2
	bl Math_Div
	ldr r3, [r6, #16]
	ldr r6, [r7, #16]
	adds r5, r0, #0
	subs r3, r3, r6
	mov r0, r8
	muls r0, r3
	movs r1, #100
	bl Math_Div
	mov r3, r10
	adds r3, r3, r5
	adds r6, r6, r0
	asrs r5, r5, #8
	asrs r0, r0, #8
	mov r8, r3
	adds r2, r5, #0
	muls r2, r5
	adds r3, r0, #0
	muls r3, r0
	adds r0, r2, r3
	cmp r0, #0
	beq .L_0811c190
	ldr r3, .L_0811c1f4
	mov lr, r3
	.2byte 0xf800
	lsls r5, r0, #8
	b .L_0811c192
.L_0811c190:
	movs r5, #0
.L_0811c192:
	adds r0, r5, #0
	mov r1, r11
	bl Math_Div
	adds r3, r7, #0
	adds r5, r0, #0
	adds r3, #88
	movs r1, #1
	str r5, [r7, #52]
	str r5, [r7, #48]
	strb r1, [r3]
	subs r3, #3
	ldrb r2, [r3]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_0811c1b8
	mov r2, r9
	str r2, [r7, #40]
.L_0811c1b8:
	mov r3, r9
	str r3, [r7, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r7, #72]
	adds r3, r7, #0
	adds r3, #90
	strb r1, [r3]
	adds r0, r7, #0
	bl Object_ResetMotion
	cmp r5, #0
	beq .L_0811c1e0
	adds r0, r7, #0
	mov r1, r8
	movs r2, #0
	adds r3, r6, #0
	bl Object_SetPosition
.L_0811c1e0:
	adds r0, r7, #0
	movs r1, #2
	bl Object_SetMode
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0811c1f4:
	.4byte IwramFillWords + 0x74
