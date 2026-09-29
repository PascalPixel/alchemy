@ Monster_MiniKnightSprites: 12 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_MiniKnightSprites
Monster_MiniKnightSprites:
	.incbin "GRAPHICS/MONSTER/MINI_KNIGHT.frames.lz"
