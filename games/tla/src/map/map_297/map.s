	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map297
Field_Map297:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map297, .LField_Map297_CELL_PICTURES
	map_block Field_Map297, .LField_Map297_SHAPES
	map_block Field_Map297, .LField_Map297_GRID
	map_block Field_Map297, .LField_Map297_SHAPE_GRID
	map_block Field_Map297
	map_block Field_Map297
	map_block Field_Map297, .LField_Map297_SCENE
.LField_Map297_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map297_SHAPES:
	.incbin "games/tla/src/map/map_297/shapes.table.lz"
	.balign 4, 0
.LField_Map297_GRID:
	.incbin "games/tla/src/map/map_297/map.grid.lz"
	.balign 4, 0
.LField_Map297_SHAPE_GRID:
	.incbin "games/tla/src/map/map_297/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map297_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data638
Resource_Data638:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map297+0xb
	.global Resource_Data639
Resource_Data639:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x01, 0xff	@ PACKER: left in the buffer by Field_Map297+0x14e
	.global Resource_Data63A
Resource_Data63A:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data639+0x13f
	.global Resource_Data63B
Resource_Data63B:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data63A+0x13f
	.global Resource_Data63C
Resource_Data63C:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data63B+0x13f
