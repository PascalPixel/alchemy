	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map298
Field_Map298:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map298, .LField_Map298_CELL_PICTURES
	map_block Field_Map298, .LField_Map298_SHAPES
	map_block Field_Map298, .LField_Map298_GRID
	map_block Field_Map298, .LField_Map298_SHAPE_GRID
	map_block Field_Map298
	map_block Field_Map298
	map_block Field_Map298, .LField_Map298_SCENE
.LField_Map298_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map298_SHAPES:
	.incbin "games/tla/src/map/map_231/shapes.table.lz"
	.balign 4, 0
.LField_Map298_GRID:
	.incbin "games/tla/src/map/map_298/map.grid.lz"
	.balign 4, 0
.LField_Map298_SHAPE_GRID:
	.incbin "games/tla/src/map/map_231/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map298_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data63E
Resource_Data63E:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map298+0xb
	.global Resource_Data63F
Resource_Data63F:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by Field_Map298+0x14e
	.global Resource_Data640
Resource_Data640:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data63F+0x13f
	.global Resource_Data641
Resource_Data641:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data640+0x13f
	.global Resource_Data642
Resource_Data642:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data641+0x13f
