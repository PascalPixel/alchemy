	.section .rodata
	.global Field_PerspectiveDataA
Field_PerspectiveDataA:
	.incbin "games/tbs/src/map/world/blocks.blocks"
	.global Field_DefaultMapCells
Field_DefaultMapCells:
	.incbin "games/tbs/src/map/default/cells.table.lz"
	.byte 0x01	@ PACKER: left in the buffer by Field_PerspectiveDataA+0x39eb
	.global Field_DefaultMapAnimation
Field_DefaultMapAnimation:
	.incbin "games/tbs/src/map/default/animation.table.lz"
	.byte 0x6b, 0x4e, 0xad	@ PACKER: left in the buffer by Field_DefaultMapCells+0x151
	.global Field_DefaultMetatileAttributes
Field_DefaultMetatileAttributes:
	.incbin "games/tbs/src/map/default/attributes.table.lz"
	.byte 0xfa, 0x60	@ PACKER: left in the buffer by Field_DefaultMapCells+0x48e
