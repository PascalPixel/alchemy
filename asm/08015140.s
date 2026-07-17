@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_08015140
	.thumb_func
Func_08015140:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08019a55
