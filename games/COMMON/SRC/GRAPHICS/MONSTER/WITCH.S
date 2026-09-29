@ Monster_WitchSprites: 10 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_WitchSprites
Monster_WitchSprites:
	.incbin "GRAPHICS/MONSTER/WITCH.frames.lz"
