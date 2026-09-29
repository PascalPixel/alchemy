@ Monster_BatSprites: 12 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_BatSprites
Monster_BatSprites:
	.incbin "GRAPHICS/MONSTER/BAT.frames.lz"
