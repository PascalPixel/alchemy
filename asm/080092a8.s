@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_080092a8
	.thumb_func
Func_080092a8:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08011fd9
