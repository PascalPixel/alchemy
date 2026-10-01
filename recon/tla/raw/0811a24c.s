.syntax unified
	.thumb
	.global Func_0811a24c
	.thumb_func
Func_0811a24c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r0
	movs r0, #182
	adds r7, r1, #0
	movs r2, #6
	movs r1, #0
	lsls r0, r0, #1
	sub sp, #24
	mov r8, r1
	mov r9, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0811a274
	movs r3, #3
	mov r9, r3
.L_0811a274:
	movs r3, #1
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0811a2c0
	add r6, sp, #4
	adds r0, r6, #0
	bl BattleParty_PrepareActiveOwners
	adds r5, r0, #0
	lsls r0, r5, #1
	adds r0, r6, r0
	bl BattleParty_PrepareReserveOwners
	adds r5, r5, r0
	cmp r8, r5
	bge .L_0811a2c0
	adds r2, r6, #0
.L_0811a298:
	ldrh r6, [r2]
	adds r2, #2
	adds r0, r6, #0
	str r2, [sp, #0]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	ldr r2, [sp, #0]
	cmp r3, #0
	ble .L_0811a2ba
	cmp r7, #0
	beq .L_0811a2b6
	strh r6, [r7]
	adds r7, #2
.L_0811a2b6:
	movs r3, #1
	add r8, r3
.L_0811a2ba:
	subs r5, #1
	cmp r5, #0
	bne .L_0811a298
.L_0811a2c0:
	movs r3, #2
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0811a300
	mov r6, r9
	movs r5, #128
	adds r6, #128
	cmp r5, r6
	bge .L_0811a300
.L_0811a2d4:
	adds r0, r5, #0
	bl Owner_GetState
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811a2fa
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	ble .L_0811a2fa
	cmp r7, #0
	beq .L_0811a2f6
	strh r5, [r7]
	adds r7, #2
.L_0811a2f6:
	movs r2, #1
	add r8, r2
.L_0811a2fa:
	adds r5, #1
	cmp r5, r6
	blt .L_0811a2d4
.L_0811a300:
	cmp r7, #0
	beq .L_0811a308
	ldr r3, .L_0811a318
	strh r3, [r7]
.L_0811a308:
	mov r0, r8
	add sp, #24
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811a318:
	.4byte 0x000000ff
