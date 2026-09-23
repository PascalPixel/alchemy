@ The template that RUNTIME_MIX.S last copied into its sample loop: the
@ mixer compares it (ldr r2, [pc, #228]) and rewrites it
@ (str r0, [pc, #216]), patching the loop only when the choice changes. A
@ run-time state word carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08001040
	.set AlchemyRunAddress, 0x030008d0
.L_audio_template_cache:
	.4byte 0x00000000
