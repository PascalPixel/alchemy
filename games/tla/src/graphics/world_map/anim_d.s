	.section .rodata
@ World map D animated tiles.
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "games/tla/src/graphics/world_map/anim_d.script.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
