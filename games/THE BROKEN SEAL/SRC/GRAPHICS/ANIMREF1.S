@ Five unused self-pointer slots between the character animation scripts.
@ Only its own slots reference the table.
.syntax unified
	.section .rodata,"a",%progbits
	.set AlchemyLoadAddress, 0x081897ec
	.align 2
	.global Animation_SelfSlots1
	.type Animation_SelfSlots1, %object
Animation_SelfSlots1:
	.rept 5
	.4byte Animation_SelfSlots1
	.endr
	.size Animation_SelfSlots1, .-Animation_SelfSlots1
