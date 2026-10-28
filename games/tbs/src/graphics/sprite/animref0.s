.syntax unified
	.section .rodata,"a",%progbits
	.align 2
	.global Anim_SelfSlots0
	.type Anim_SelfSlots0, %object
Anim_SelfSlots0:
	.rept 4
	.4byte Anim_SelfSlots0
	.endr
	.size Anim_SelfSlots0, .-Anim_SelfSlots0
