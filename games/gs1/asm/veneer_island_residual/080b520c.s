.syntax unified
.text
	.arm
	.global Func_080b520c
Func_080b520c:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
	nop
