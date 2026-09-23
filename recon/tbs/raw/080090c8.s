.syntax unified
	.thumb
	.global Object_CreateFar
	.global Func_080090c8
	.thumb_func
Object_CreateFar:
Func_080090c8:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc151
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc0f5
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800c3ed
