@ Monster_ScorpionSprites: 16 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_ScorpionSprites
Monster_ScorpionSprites:
	.incbin "GRAPHICS/MONSTER/SCORPION.frames.lz"
