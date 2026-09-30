.syntax unified
	.thumb
	.global Func_0817b970
	.thumb_func
Func_0817b970:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	bl GetBattleObjectSlotFar
	movs r3, #3
	movs r2, #13
	ands r5, r3
	negs r2, r2
	adds r6, r0, #0
	movs r7, #0
	lsls r5, r5, #2
	mov r8, r2
	b .L_0817b9a4
.L_0817b98e:
	adds r3, r6, #0
	adds r3, #42
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0817b9a2
	ldrb r3, [r0, #9]
	mov r2, r8
	ands r3, r2
	orrs r3, r5
	strb r3, [r0, #9]
.L_0817b9a2:
	adds r7, #1
.L_0817b9a4:
	ldr r0, [r6]
	adds r1, r7, #0
	bl GetMotionRecordFar
	cmp r0, #0
	bne .L_0817b98e
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
