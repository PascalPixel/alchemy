.syntax unified
	.thumb
	.global Func_08157530
	.thumb_func
Func_08157530:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	mov r9, r3
	adds r6, r1, #0
	mov r11, r2
	bl GetBattleObjectSlotFar
	adds r5, r0, #0
	adds r0, r6, #0
	bl GetBattleObjectSlotFar
	ldr r6, [r5]
	ldr r0, [r0]
	ldr r2, [r6, #8]
	ldr r3, [r0, #8]
	mov r8, r0
	subs r3, r3, r2
	mov r0, r9
	muls r0, r3
	movs r1, #100
	mov r10, r2
	bl __divsi3
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r2, [r6, #16]
	adds r5, r0, #0
	subs r3, r3, r2
	mov r0, r9
	muls r0, r3
	movs r1, #100
	mov r8, r2
	bl __divsi3
	add r10, r5
	add r8, r0
	asrs r5, r5, #8
	asrs r0, r0, #8
	adds r2, r0, #0
	muls r2, r0
	adds r3, r5, #0
	muls r3, r5
	adds r3, r3, r2
	adds r0, r3, #0
	ldr r2, .L_081575e8
	mov lr, r2
	.2byte 0xf800
	mov r1, r11
	lsls r0, r0, #8
	bl __divsi3
	adds r3, r6, #0
	movs r1, #1
	adds r3, #88
	str r0, [r6, #52]
	str r0, [r6, #48]
	strb r1, [r3]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	str r3, [r6, #72]
	adds r3, r6, #0
	movs r2, #0
	adds r3, #90
	str r2, [r6, #40]
	str r2, [r6, #68]
	adds r0, r6, #0
	strb r1, [r3]
	bl Object_ResetMotion
	adds r0, r6, #0
	mov r1, r10
	movs r2, #0
	mov r3, r8
	bl Object_SetPosition
	adds r0, r6, #0
	movs r1, #2
	bl Object_SetMode
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
.L_081575e8:
	.4byte IwramFillWords + 0x74
