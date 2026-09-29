@ Monster_SmallLizardSprites: 12 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "GRAPHICS/MONSTER/SMALL_LIZARD.frames.lz"
