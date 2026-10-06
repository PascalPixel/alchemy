.syntax unified
	.section .rodata,"a",%progbits
	.arm
.L_bias:
	.4byte 0x80808080
.L_dispatch_base:
	.4byte SentouKouka_TenkaiCaseTable
