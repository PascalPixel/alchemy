	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map293
Field_Map293:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map293, .LField_Map293_CELL_PICTURES
	map_block Field_Map293, .LField_Map293_SHAPES
	map_block Field_Map293, .LField_Map293_GRID
	map_block Field_Map293, .LField_Map293_SHAPE_GRID
	map_block Field_Map293
	map_block Field_Map293
	map_block Field_Map293, .LField_Map293_SCENE
.LField_Map293_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map293_SHAPES:
	.incbin "games/tla/src/map/map_041/shapes.table.lz"
	.balign 4, 0
.LField_Map293_GRID:
	.incbin "games/tla/src/map/map_293/map.grid.lz"
	.balign 4, 0
.LField_Map293_SHAPE_GRID:
	.incbin "games/tla/src/map/map_293/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map293_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data620
Resource_Data620:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map293+0xb
	.global Resource_Data621
Resource_Data621:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x80, 0x06	@ PACKER: left in the buffer by Field_Map293+0x14e
	.global Resource_Data622
Resource_Data622:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data621+0x13f
	.global Resource_Data623
Resource_Data623:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data622+0x13f
	.global Resource_Data624
Resource_Data624:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data623+0x13f
