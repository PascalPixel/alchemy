.syntax unified
	.thumb
	.global Func_0815e288
	.thumb_func
Func_0815e288:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #48]
	mov r8, r1
	mov r9, r0
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	movs r1, #0
	adds r0, r5, #0
	bl GetMotionRecordFar
	mov r10, r0
	bl Func_08014de4
	adds r1, r6, #0
	adds r0, r6, #0
	adds r1, #12
	adds r5, #8
	bl Graphics_PrepareTransferInIwramWork
	mov r1, r8
	adds r0, r5, #0
	bl Render_ProjectPoint
	mov r2, r10
	ldr r1, [r2, #12]
	ldr r6, .L_0815e2f8
	mov lr, r6
	.2byte 0xf800
	adds r5, r0, #0
	mov r0, r9
	bl Battle_GetObjectTableValueFar
	adds r1, r0, #0
	asrs r1, r1, #17
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	ldr r3, [r2, #4]
	subs r3, r3, r0
	str r3, [r2, #4]
	movs r0, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0815e2f8:
	.4byte IwramMulQ16
