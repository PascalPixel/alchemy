	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map294
Field_Map294:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map294, .LField_Map294_CELL_PICTURES
	map_block Field_Map294, .LField_Map294_SHAPES
	map_block Field_Map294, .LField_Map294_GRID
	map_block Field_Map294, .LField_Map294_SHAPE_GRID
	map_block Field_Map294
	map_block Field_Map294
	map_block Field_Map294, .LField_Map294_SCENE
.LField_Map294_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map294_SHAPES:
	.incbin "games/tla/src/map/map_220/shapes.table.lz"
	.balign 4, 0
.LField_Map294_GRID:
	.incbin "games/tla/src/map/map_294/map.grid.lz"
	.balign 4, 0
.LField_Map294_SHAPE_GRID:
	.incbin "games/tla/src/map/map_294/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map294_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
	.global Resource_Data626
Resource_Data626:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map294+0xb
	.global Resource_Data627
Resource_Data627:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x01, 0x63	@ PACKER: left in the buffer by Field_Map294+0x14e
	.global Resource_Data628
Resource_Data628:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data627+0x13f
	.global Resource_Data629
Resource_Data629:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data628+0x13f
	.global Resource_Data62A
Resource_Data62A:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data629+0x13f
