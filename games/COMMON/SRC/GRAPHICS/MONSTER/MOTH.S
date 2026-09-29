@ Monster_MothSprites: 12 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_MothSprites
Monster_MothSprites:
	.incbin "GRAPHICS/MONSTER/MOTH.frames.lz"
