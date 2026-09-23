@ The dispatcher's jump table in the SENTOU_KOUKA_TENKAI.S block: 32 zero
@ bytes (0x080f0104) that the decoder fills at run time
@ (add r4, pc, #196; str r6, [r4], #4) with the eight case addresses that
@ follow (0x080f0124), each moved by the distance the block was copied. A
@ table rewritten at run time and the addresses it is built from are data
@ and carry no credit; TLA leaves the same 64 bytes outside its credited
@ modules.
.syntax unified
	.set	.L_case_2, 0x080f0144
	.set	.L_case_4_plus, 0x080f0160
	.set	.L_case_8, 0x080f0200
	.set	.L_case_6_plus, 0x080f01b0
	.set	.L_case_4_minus, 0x080f0188
	.set	.L_case_10, 0x080f0230
	.set	.L_case_6_minus, 0x080f01d8
	.arm
	.set AlchemyLoadAddress, 0x080f0104
.L_dispatch_runtime:
	.space 32, 0
.L_dispatch_source:
	.4byte .L_case_2
	.4byte .L_case_4_plus
	.4byte .L_case_8
	.4byte .L_case_6_plus
	.4byte .L_case_2
	.4byte .L_case_4_minus
	.4byte .L_case_10
	.4byte .L_case_6_minus
