@ Four unused self-pointer slots between the character animation scripts.
@ Only its own slots reference the table.
.syntax unified
	.section .rodata,"a",%progbits
	.set AlchemyLoadAddress, 0x08188acc
	.align 2
	.global Animation_SelfSlots0
	.type Animation_SelfSlots0, %object
Animation_SelfSlots0:
	.rept 4
	.4byte Animation_SelfSlots0
	.endr
	.size Animation_SelfSlots0, .-Animation_SelfSlots0
