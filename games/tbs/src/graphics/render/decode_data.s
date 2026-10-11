.syntax unified
@ A table of the handwritten routine beside it: it lives in the routine's text.
	.text
	.arm
	.4byte 0x00000000
	.4byte 0
	.reloc .-4, R_ARM_ABS32_NOI, Map_CheckObjectStepFar
	.4byte 0
	.reloc .-4, R_ARM_ABS32_NOI, Map_CopyMetatileCellsRectFar
	.global Render_DecodeFrameBuffer
Render_DecodeFrameBuffer:
	.space 224, 0
