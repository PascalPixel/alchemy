@ Two data words of the SENTOU_KOUKA_TENKAI.S block: 0x80808080, which
@ nothing in the block reads (this decoder takes its bias in r2), and the
@ address of the case-address table (0x080f0124), which the decoder loads
@ with ldr r2, [pc, #192] to find how far the block was moved. They sit
@ between two routines, and TLA and the main-bank copy around
@ GRAPHICS/TILE/DECODE.S leave the same words uncredited, so they carry no
@ credit.
.syntax unified
	.set	.L_dispatch_source, 0x080f0124
	.arm
	.set AlchemyLoadAddress, 0x080f00f0
.L_bias:
	.4byte 0x80808080
.L_dispatch_base:
	.4byte .L_dispatch_source
