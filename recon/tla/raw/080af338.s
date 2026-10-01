.syntax unified
	.thumb
	.global Item_GetTargetMode
	.thumb_func
Item_GetTargetMode:
	push {lr}
	bl Item_GetDirect
	ldrh r0, [r0, #40]
	bl BattleAction_GetDirect
	ldrb r0, [r0]
	pop {pc}
