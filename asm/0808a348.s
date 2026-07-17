@ Long-call interworking veneer: a fixed ldr r4,[pc]; bx r4 stub the
@ linker emits to reach a distant routine. Not expressible in C.
.syntax unified
	.thumb
	.global Func_0808a348
	.thumb_func
Func_0808a348:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08091255
