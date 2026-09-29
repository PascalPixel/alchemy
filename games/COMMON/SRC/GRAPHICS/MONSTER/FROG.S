@ Monster_FrogSprites: 12 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_FrogSprites
Monster_FrogSprites:
	.incbin "GRAPHICS/MONSTER/FROG.frames.lz"
