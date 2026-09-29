@ Monster_ManEaterSprites: 12 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_ManEaterSprites
Monster_ManEaterSprites:
	.incbin "GRAPHICS/MONSTER/MAN_EATER.frames.lz"
