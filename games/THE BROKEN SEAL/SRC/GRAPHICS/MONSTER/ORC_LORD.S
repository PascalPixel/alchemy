@ Monster_OrcLordSprites: 12 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_OrcLordSprites
Monster_OrcLordSprites:
	.incbin "GRAPHICS/MONSTER/ORC_LORD.frames.lz"
