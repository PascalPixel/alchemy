.syntax unified
	.thumb
	.global BattleFx_RunNoEffect
	.thumb_func
BattleFx_RunNoEffect:
	bx	lr
	.2byte 0x0000
