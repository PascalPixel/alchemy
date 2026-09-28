@ The dispatcher's jump table in the SENTOU_KOUKA_TENKAI.S block: 32 zero
@ bytes (0x080f0104) that the decoder fills at run time
@ (add r4, pc, #196; str r6, [r4], #4) with the eight case addresses that
@ follow (0x080f0124), each moved by the distance the block was copied. A
@ table rewritten at run time and the addresses it is built from are data
@ and carry no credit; TLA leaves the same 64 bytes outside its credited
@ modules.
.syntax unified
	.section .data,"aw",%progbits
	.arm
.L_dispatch_runtime:
	.space 32, 0
	.global SentouKouka_TenkaiCaseTable
SentouKouka_TenkaiCaseTable:
	.4byte SentouKouka_TenkaiCase2
	.4byte SentouKouka_TenkaiCase4Plus
	.4byte SentouKouka_TenkaiCase8
	.4byte SentouKouka_TenkaiCase6Plus
	.4byte SentouKouka_TenkaiCase2
	.4byte SentouKouka_TenkaiCase4Minus
	.4byte SentouKouka_TenkaiCase10
	.4byte SentouKouka_TenkaiCase6Minus
