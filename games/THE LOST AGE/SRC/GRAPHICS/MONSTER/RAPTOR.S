@ Monster_RaptorSprites: 12 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_RaptorSprites
Monster_RaptorSprites:
	.incbin "GRAPHICS/MONSTER/RAPTOR.frames.lz"
