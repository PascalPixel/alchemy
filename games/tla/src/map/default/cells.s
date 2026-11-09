	.section .rodata
	.global Field_DefaultMapCells
Field_DefaultMapCells:
	.incbin "games/tla/src/map/default/cells.table.lz"
	.byte 0x86, 0xb2, 0xb7	@ PACKER: left in the buffer by Ending_VillagersPictureA+0x93c1
