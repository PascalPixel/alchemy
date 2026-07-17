@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_080152d0
	.thumb_func
Func_080152d0:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0801eb91
