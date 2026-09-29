@ Monster_MenardiSprites: 18 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "GRAPHICS/MONSTER/MENARDI.frames.lz"
