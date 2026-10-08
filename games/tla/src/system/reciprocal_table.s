.syntax unified
	.section .rodata,"a",%progbits
	.align 2
	.global Runtime_ReciprocalTable
	.type Runtime_ReciprocalTable, %object
Runtime_ReciprocalTable:
	.4byte 0
	.set .Ldivisor, 1
	.rept 0x3fff
	.4byte ((0x100000000 + .Ldivisor - 1) / .Ldivisor) & 0xffffffff
	.set .Ldivisor, .Ldivisor + 1
	.endr
	.size Runtime_ReciprocalTable, .-Runtime_ReciprocalTable
