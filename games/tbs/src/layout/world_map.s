	.section .rodata
	.global Field_PerspectiveDataA
Field_PerspectiveDataA:
	.incbin "games/tbs/src/layout/world/blocks.blocks"
	.global Field_DefaultGridCells
Field_DefaultGridCells:
	.incbin "games/tbs/src/layout/default/cells.table.lz"
	.byte 0x01	@ PACKER: left in the buffer by Field_PerspectiveDataA+0x39eb
	.global Field_DefaultTileAnimation
Field_DefaultTileAnimation:
	.incbin "games/tbs/src/layout/default/tileset_anims.table.lz"
	.byte 0x6b, 0x4e, 0xad	@ PACKER: left in the buffer by Field_DefaultGridCells+0x151
	.global Field_DefaultMetatileAttributes
Field_DefaultMetatileAttributes:
	.incbin "games/tbs/src/layout/default/attributes.table.lz"
	.byte 0xfa, 0x60	@ PACKER: left in the buffer by Field_DefaultGridCells+0x48e
