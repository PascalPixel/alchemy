@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_080f9018
	.thumb_func
Func_080f9018:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080f91e9
