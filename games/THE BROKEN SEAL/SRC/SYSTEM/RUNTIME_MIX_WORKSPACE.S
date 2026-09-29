@ Mixing workspace of the resident runtime, 0x590 zero bytes in this
@ image: RUNTIME_MIX.S accumulates channel samples here (a literal it
@ loads) and RUNTIME_MIX_RENDER.S reads them back
@ (add r0, pc, #656). A run-time buffer carries no credit.
.syntax unified
	.section .data,"aw",%progbits
	.arm

@ 合成中に命令雛形と一時標本を置く常駐作業域。
	.global IwramSoundMixWorkspace
IwramSoundMixWorkspace:
	.space 0x590, 0
