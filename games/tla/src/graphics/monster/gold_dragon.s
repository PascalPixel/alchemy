	.section .rodata
	.global Monster_GoldDragonSprites
Monster_GoldDragonSprites:
	.incbin "games/tla/src/graphics/monster/gold_dragon.frames.lz"
	.byte 0x22, 0x40, 0xbf	@ PACKER: left in the buffer by Ui_Icons+0x5599
