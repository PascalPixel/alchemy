.syntax unified
	.section .rodata,"a",%progbits
	.align 2
	.global System_ReciprocalTable
	.type System_ReciprocalTable, %object
System_ReciprocalTable:
	.4byte 0
	.set .Ldivisor, 1
	.rept 0x3fff
	.4byte ((0x100000000 + .Ldivisor - 1) / .Ldivisor) & 0xffffffff
	.set .Ldivisor, .Ldivisor + 1
	.endr
	.size System_ReciprocalTable, .-System_ReciprocalTable
