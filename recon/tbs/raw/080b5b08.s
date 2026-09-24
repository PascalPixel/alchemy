.syntax unified
	.thumb
	.global BattlePresentation_ClearBg0VerticalOffset
	.global Func_080b5b08
	.thumb_func
BattlePresentation_ClearBg0VerticalOffset:
Func_080b5b08:
	ldr	r3, [pc, #4]
	movs	r2, #0
	strh	r2, [r3, #0]
	bx	lr
	.4byte 0x04000012
