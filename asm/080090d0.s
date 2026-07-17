@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_080090d0
	.thumb_func
Func_080090d0:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800c0f5
