.syntax unified
	.thumb
	.global BattleMotion_GetSlotField14
	.thumb_func
BattleMotion_GetSlotField14:
	push {lr}
	bl GetBattleObjectSlot
	ldr r0, [r0, #20]
	pop {pc}
	.2byte 0x0000
