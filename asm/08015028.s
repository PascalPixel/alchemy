@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_08015028
	.thumb_func
Func_08015028:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080196c5
