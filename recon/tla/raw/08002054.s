.syntax unified
	.thumb
	.global Func_08002054
	.thumb_func
Func_08002054:
	.global Math_Div
	.thumb_func
Math_Div:
	ldr	r3, [pc, #0]
	bx	r3
	.2byte 0x0528
	.2byte 0x0300
	.global Math_DivU
	.thumb_func
Math_DivU:
	ldr	r3, [pc, #0]
	bx	r3
	.2byte 0x0534
	.2byte 0x0300
	.global Math_Mod
	.thumb_func
Math_Mod:
	ldr	r3, [pc, #0]
	bx	r3
	.2byte 0x0508
	.2byte 0x0300
	ldr	r3, [pc, #0]
	bx	r3
	.4byte 0x03000514
