.syntax unified
	.thumb
	.global Resource_FarCall003
Resource_FarCall003:
	.global Func_08020000
	.thumb_func
Func_08020000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x1919
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08022bd9
