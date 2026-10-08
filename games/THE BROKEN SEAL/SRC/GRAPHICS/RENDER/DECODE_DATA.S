.syntax unified
	.section .data,"aw",%progbits
	.arm
	.4byte 0x00000000
	.4byte 0
	.reloc .-4, R_ARM_ABS32_NOI, Map_CheckObjectStepFar
	.4byte 0
	.reloc .-4, R_ARM_ABS32_NOI, Map_CopyMetatileCellsRectFar
	.global Render_DecodeFrameBuffer
Render_DecodeFrameBuffer:
	.space 224, 0
	.global Render_DecodeFrameEnd
Render_DecodeFrameEnd:
