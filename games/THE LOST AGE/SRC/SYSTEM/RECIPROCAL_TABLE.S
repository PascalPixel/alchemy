@ Reciprocal table read by IWRAM_RUNTIME_DIVIDE.S for divisors below 0x4000.
@ Each nonzero divisor uses ceil(2^32 / divisor), stored modulo 2^32. The
@ divide helper handles divisor 1's wrapped zero product separately.
.syntax unified
	.section .rodata,"a",%progbits
	.set AlchemyLoadAddress, 0x080028a8
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
