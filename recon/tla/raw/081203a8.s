.syntax unified
	.thumb
	.global BattleUnit_KeepsOneHp
	.thumb_func
BattleUnit_KeepsOneHp:
	push {lr}
	bl Owner_GetState
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrh r0, [r3]
	cmp r0, #110
	beq .L_081203be
	cmp r0, #113
	bne .L_081203c2
.L_081203be:
	movs r0, #1
	b .L_081203c4
.L_081203c2:
	movs r0, #0
.L_081203c4:
	pop {pc}
	.2byte 0x0000
