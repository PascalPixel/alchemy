.syntax unified
	.thumb
	.global BattleFx_AdvanceSpinAngle
	.thumb_func
BattleFx_AdvanceSpinAngle:
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r0, #6]
	bx lr
