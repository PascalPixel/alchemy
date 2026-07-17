@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_08009098
	.thumb_func
Func_08009098:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800c2d9
