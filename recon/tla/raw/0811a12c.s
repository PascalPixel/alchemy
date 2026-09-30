.syntax unified
	.thumb
	.global BattleParty_ListPresentEnemies
	.thumb_func
BattleParty_ListPresentEnemies:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r2, #0
	mov r8, r2
	movs r7, #6
	movs r0, #0
	cmp r6, #0
	beq .L_0811a17c
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0811a14e
	movs r7, #3
.L_0811a14e:
	movs r5, #128
	adds r7, #128
	cmp r5, r7
	bge .L_0811a176
.L_0811a156:
	adds r0, r5, #0
	bl Owner_GetState
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811a170
	movs r3, #1
	strh r5, [r6]
	add r8, r3
	adds r6, #2
.L_0811a170:
	adds r5, #1
	cmp r5, r7
	blt .L_0811a156
.L_0811a176:
	ldr r3, .L_0811a184
	mov r0, r8
	strh r3, [r6]
.L_0811a17c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811a184:
	.4byte 0x000000ff
