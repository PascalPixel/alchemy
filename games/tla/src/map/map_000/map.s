	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map000
Field_Map000:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map000, .LField_Map000_CELL_PICTURES
	map_block Field_Map000, .LField_Map000_SHAPES
	map_block Field_Map000, .LField_Map000_GRID
	map_block Field_Map000, .LField_Map000_SHAPE_GRID
	map_block Field_Map000
	map_block Field_Map000
	map_block Field_Map000, .LField_Map000_SCENE
.LField_Map000_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map000_SHAPES:
	.incbin "games/tla/src/map/map_000/shapes.table.lz"
	.balign 4, 0
.LField_Map000_GRID:
	.incbin "games/tla/src/map/map_000/map.grid.lz"
	.balign 4, 0
.LField_Map000_SHAPE_GRID:
	.incbin "games/tla/src/map/map_000/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map000_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data26D
Resource_Data26D:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map000+0xb
	.global Resource_Data26E
Resource_Data26E:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x01, 0xff	@ PACKER: left in the buffer by Field_Map000+0x14e
	.global Resource_Data26F
Resource_Data26F:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data26E+0x13f
	.global Resource_Data270
Resource_Data270:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data26F+0x13f
	.global Resource_Data271
Resource_Data271:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data270+0x13f
