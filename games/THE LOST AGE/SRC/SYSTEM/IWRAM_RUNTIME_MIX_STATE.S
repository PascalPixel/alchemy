@ The template that IWRAM_RUNTIME_MIX_LANES.S last copied into its sample
@ loop: the mixer compares it (ldr r2, [pc, #404]) and rewrites it
@ (str r0, [pc, #392]), patching the loop only when the choice changes. A
@ run-time state word carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x080010b0
	.set AlchemyRunAddress, 0x03000af8
	.4byte	0x00000000
