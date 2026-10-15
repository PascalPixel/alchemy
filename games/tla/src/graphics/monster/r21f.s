	.section .rodata
@ 十二こま描かれ、表は十六こま分: 残り四こまは絵なし (索引255)
	.global Resource_Data21F
Resource_Data21F:
	.incbin "games/tla/src/graphics/monster/r21f.frames.lz"
	.byte 0x34	@ PACKER: left in the buffer by Monster_WerewolfSprites+0x343f
