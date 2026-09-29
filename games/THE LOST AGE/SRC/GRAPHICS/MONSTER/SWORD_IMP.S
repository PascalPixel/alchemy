@ Monster_SwordImpSprites: 10 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_SwordImpSprites
Monster_SwordImpSprites:
	.incbin "GRAPHICS/MONSTER/SWORD_IMP.frames.lz"
