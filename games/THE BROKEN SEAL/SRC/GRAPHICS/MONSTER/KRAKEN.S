@ Monster_KrakenSprites: 16 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_KrakenSprites
Monster_KrakenSprites:
	.incbin "GRAPHICS/MONSTER/KRAKEN.frames.lz"
