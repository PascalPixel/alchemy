.syntax unified
	.thumb
	.global UiText_DrawNumberAtOffsetFar
	.thumb_func
UiText_DrawNumberAtOffsetFar:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2215
	.2byte 0x0804
	.global Func_080380b0
	.thumb_func
Func_080380b0:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2245
	.2byte 0x0804
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x1f71
	.2byte 0x0804
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08041f91
