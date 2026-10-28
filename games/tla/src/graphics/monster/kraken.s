	.section .rodata
	.global Monster_KrakenSprites
Monster_KrakenSprites:
	.incbin "games/tla/src/graphics/monster/kraken.pictures.lz"
	.byte 0xbd, 0xca, 0x95	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x40c5
