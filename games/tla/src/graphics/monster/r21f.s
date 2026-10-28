	.section .rodata
@ Twelve drawn pictures in a sixteen-picture table; four empty pictures use index 255.
	.global Resource_Data21F
Resource_Data21F:
	.incbin "games/tla/src/graphics/monster/r21f.pictures.lz"
	.byte 0x34	@ PACKER: left in the buffer by Monster_WerewolfSprites+0x343f
