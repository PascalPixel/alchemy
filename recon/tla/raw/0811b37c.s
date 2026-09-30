.syntax unified
	.thumb
	.global Func_0811b37c
	.thumb_func
Func_0811b37c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	sub sp, #80
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	mov r2, sp
	adds r2, #52
	movs r1, #0
	adds r0, r2, #0
	mov r10, r3
	str r2, [sp, #0]
	mov r9, r1
	bl BattleParty_PrepareActiveOwners
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r0, r5, #1
	adds r0, r3, r0
	bl Func_0811a0b0
	mov r3, r10
	adds r5, r5, r0
	movs r2, #255
	movs r7, #13
	adds r3, #129
.L_0811b3ba:
	subs r7, #1
	strb r2, [r3]
	subs r3, #1
	cmp r7, #0
	bge .L_0811b3ba
	mov r3, r10
	movs r1, #5
	adds r3, #129
	movs r2, #13
.L_0811b3cc:
	subs r1, #1
	strb r2, [r3]
	subs r3, #1
	subs r2, #1
	cmp r1, #0
	bge .L_0811b3cc
	cmp r5, #0
	ble .L_0811b41e
	ldr r1, .L_0811b4a8
	ldr r2, [sp, #0]
	mov r3, r9
	mov r11, r1
	mov r8, r2
	lsls r6, r3, #1
	adds r7, r5, #0
.L_0811b3ea:
	mov r1, r8
	ldrh r5, [r1]
	movs r2, #2
	adds r3, r5, #0
	adds r3, #116
	mov r1, r10
	add r8, r2
	mov r2, r9
	strb r2, [r1, r3]
	adds r0, r5, #0
	bl GetBattleObjectSlot
	mov r3, r11
	ldrsb r2, [r6, r3]
	mov r1, r11
	adds r3, r6, #1
	ldrsb r3, [r3, r1]
	adds r1, r5, #0
	bl BattlePresentation_SpawnActorObject
	subs r7, #1
	movs r2, #1
	adds r6, #2
	add r9, r2
	cmp r7, #0
	bne .L_0811b3ea
.L_0811b41e:
	movs r5, #2
	add r5, r10
	movs r3, #100
	ldrsh r3, [r5, r3]
	movs r7, #0
	mov r11, r5
	cmp r3, #255
	beq .L_0811b44a
	ldr r4, [sp, #0]
	movs r0, #0
	movs r2, #100
	mov r1, r11
.L_0811b436:
	ldrh r3, [r1, r2]
	adds r7, #1
	strh r3, [r0, r4]
	adds r2, #2
	adds r0, #2
	cmp r7, #5
	bgt .L_0811b44a
	ldrsh r3, [r1, r2]
	cmp r3, #255
	bne .L_0811b436
.L_0811b44a:
	movs r1, #28
	movs r2, #4
	add r1, sp
	add r2, sp
	adds r5, r7, #0
	mov r9, r1
	mov r10, r2
	ldr r0, [sp, #0]
	adds r1, r5, #0
	mov r2, r9
	mov r3, r10
	bl Func_0811b180
	cmp r5, #0
	ble .L_0811b49a
	movs r3, #0
	lsls r6, r3, #2
	movs r3, #100
	mov r8, r3
.L_0811b470:
	mov r1, r11
	mov r3, r8
	ldrsh r5, [r1, r3]
	cmp r5, #254
	beq .L_0811b48e
	adds r0, r5, #0
	bl GetBattleObjectSlot
	mov r1, r9
	ldr r2, [r6, r1]
	mov r1, r10
	ldr r3, [r6, r1]
	adds r1, r5, #0
	bl BattlePresentation_SpawnActorObject
.L_0811b48e:
	movs r2, #2
	subs r7, #1
	add r8, r2
	adds r6, #4
	cmp r7, #0
	bne .L_0811b470
.L_0811b49a:
	add sp, #80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0811b4a8:
	.4byte Data_08128844
