@ Monster_TreantSprites: 12 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_TreantSprites
Monster_TreantSprites:
	.incbin "GRAPHICS/MONSTER/TREANT.frames.lz"
