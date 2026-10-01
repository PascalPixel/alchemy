.syntax unified
	.thumb
	.global BattleAction_GetDirect
	.thumb_func
BattleAction_GetDirect:
	push {lr}
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	adds r2, r0, #0
	ands r2, r3
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #222
	cmp r2, r3
	bls .L_080af454
	movs r2, #0
.L_080af454:
	lsls r0, r2, #1
	ldr r3, .L_080af460
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r3
	pop {pc}
.L_080af460:
	.4byte BattleAction_DefinitionTable
