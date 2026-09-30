.syntax unified
	.thumb
	.global Func_08118d70
	.thumb_func
Func_08118d70:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #20
	mov r8, sp
	mov r0, r8
	bl BattleParty_PrepareActiveOwners
	adds r7, r0, #0
	lsls r0, r7, #1
	add r0, r8
	bl Func_0811a0b0
	adds r7, r7, r0
	movs r0, #0
	mov r10, r0
	cmp r10, r7
	bge .L_08118e58
	movs r1, #0
	movs r5, #0
	movs r6, #0
	mov r9, r1
.L_08118da0:
	mov r3, r8
	ldrh r0, [r6, r3]
	bl Owner_GetState
	adds r2, r0, #0
	movs r0, #48
	adds r0, #255
	movs r1, #3
	adds r3, r2, r0
.L_08118db2:
	mov r0, r9
	subs r1, #1
	strb r0, [r3]
	subs r3, #1
	cmp r1, #0
	bge .L_08118db2
	movs r1, #153
	lsls r1, r1, #1
	movs r0, #52
	adds r3, r2, r1
	adds r0, #255
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #3
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #3
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	adds r1, #2
	adds r3, r2, r0
	strb r5, [r3]
	adds r0, #2
	adds r3, r2, r1
	strb r5, [r3]
	mov r1, r8
	adds r3, r2, r0
	strb r5, [r3]
	ldrh r0, [r6, r1]
	bl BattleUnit_Recalculate
	movs r3, #1
	add r10, r3
	adds r6, #2
	cmp r10, r7
	blt .L_08118da0
.L_08118e58:
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
