.syntax unified
	.thumb
	push	{lr}
	movs	r1, #9
	bl	BattlePres_RunBurstScene
	pop	{pc}
	.2byte 0x0000
