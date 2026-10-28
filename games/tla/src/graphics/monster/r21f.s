	.section .rodata
@ Twelve drawn frames in a sixteen-frame table; four empty frames use index 255.
	.global Resource_Data21F
Resource_Data21F:
	.incbin "games/tla/src/graphics/monster/r21f.pictures.lz"
	.byte 0x34	@ PACKER: left in the buffer by Monster_WerewolfSprites+0x343f
