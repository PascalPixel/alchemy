@ Monster_TailedImpSprites: 8 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_TailedImpSprites
Monster_TailedImpSprites:
	.incbin "GRAPHICS/MONSTER/TAILED_IMP.frames.lz"
