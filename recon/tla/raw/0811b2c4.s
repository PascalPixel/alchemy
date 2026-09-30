.syntax unified
	.thumb
	.global Summon_Refresh
	.thumb_func
Summon_Refresh:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #36]
	movs r3, #100
	adds r2, #2
	ldrsh r3, [r2, r3]
	movs r1, #1
	negs r1, r1
	sub sp, #76
	mov r11, r1
	movs r0, #0
	movs r7, #0
	mov r12, r2
	cmp r3, #255
	beq .L_0811b322
	add r1, sp, #48
	mov r10, r1
	mov r4, r10
	movs r1, #100
	adds r5, r2, #0
.L_0811b2fa:
	ldrsh r3, [r2, r1]
	cmp r3, #254
	bne .L_0811b306
	cmp r0, #0
	bne .L_0811b308
	b .L_0811b310
.L_0811b306:
	mov r11, r0
.L_0811b308:
	ldrh r3, [r5, r1]
	adds r0, #1
	strh r3, [r4]
	adds r4, #2
.L_0811b310:
	adds r7, #1
	adds r1, #2
	cmp r7, #5
	bgt .L_0811b326
	mov r2, r12
	ldrsh r3, [r2, r1]
	cmp r3, #255
	bne .L_0811b2fa
	b .L_0811b326
.L_0811b322:
	add r1, sp, #48
	mov r10, r1
.L_0811b326:
	mov r5, r11
	adds r5, #1
	movs r2, #24
	mov r8, sp
	add r2, sp
	mov r0, r10
	adds r1, r5, #0
	mov r3, r8
	mov r9, r2
	bl Func_0811b180
	cmp r5, #0
	ble .L_0811b36c
	mov r7, r11
	movs r6, #0
	mov r5, r10
	adds r7, #1
.L_0811b348:
	ldrh r0, [r5]
	adds r5, #2
	cmp r0, #254
	beq .L_0811b364
	bl GetBattleObjectSlot
	mov r1, r9
	ldr r3, [r6, r1]
	mov r2, r8
	lsls r3, r3, #16
	str r3, [r0, #12]
	ldr r3, [r6, r2]
	lsls r3, r3, #16
	str r3, [r0, #16]
.L_0811b364:
	subs r7, #1
	adds r6, #4
	cmp r7, #0
	bne .L_0811b348
.L_0811b36c:
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
