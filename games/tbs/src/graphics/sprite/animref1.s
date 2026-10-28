.syntax unified
	.section .rodata,"a",%progbits
	.align 2
	.global Anim_SelfSlots1
	.type Anim_SelfSlots1, %object
Anim_SelfSlots1:
	.rept 5
	.4byte Anim_SelfSlots1
	.endr
	.size Anim_SelfSlots1, .-Anim_SelfSlots1
