.syntax unified
	.section .rodata,"a",%progbits
	.align 2
	.global Animation_SelfSlots2
	.type Animation_SelfSlots2, %object
Animation_SelfSlots2:
	.rept 5
	.4byte Animation_SelfSlots2
	.endr
	.size Animation_SelfSlots2, .-Animation_SelfSlots2
