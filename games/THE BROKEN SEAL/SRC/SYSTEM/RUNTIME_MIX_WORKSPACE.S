@ Mixing workspace of the resident runtime, 0x590 zero bytes in this
@ image: RUNTIME_MIX.S accumulates channel samples here (its literal
@ 0x03000df8) and RUNTIME_MIX_RENDER.S reads them back
@ (add r0, pc, #656). A run-time buffer carries no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08001568
	.set AlchemyRunAddress, 0x03000df8

@ 合成中に命令雛形と一時標本を置く常駐作業域。
.L_audio_workspace:
	.space 0x590, 0
