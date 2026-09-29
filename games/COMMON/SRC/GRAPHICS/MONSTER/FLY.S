@ Monster_FlySprites: 10 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_FlySprites
Monster_FlySprites:
	.incbin "GRAPHICS/MONSTER/FLY.frames.lz"
