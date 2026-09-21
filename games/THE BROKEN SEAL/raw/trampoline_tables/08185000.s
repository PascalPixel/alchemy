@ Unresolved fixed-width trampoline table.
.syntax unified
.text
	.thumb
	.global Func_08185000
	.thumb_func
Func_08185000:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08185009
