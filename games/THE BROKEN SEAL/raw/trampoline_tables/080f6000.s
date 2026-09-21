@ Unresolved fixed-width trampoline table.
.syntax unified
.text
	.thumb
	.global Func_080f6000
	.thumb_func
Func_080f6000:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080f6009
