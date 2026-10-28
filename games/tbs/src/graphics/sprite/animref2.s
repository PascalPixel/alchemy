.syntax unified
	.section .rodata,"a",%progbits
	.align 2
	.global Anim_SelfSlots2
	.type Anim_SelfSlots2, %object
Anim_SelfSlots2:
	.rept 5
	.4byte Anim_SelfSlots2
	.endr
	.size Anim_SelfSlots2, .-Anim_SelfSlots2
