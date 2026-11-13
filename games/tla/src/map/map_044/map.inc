	.global Field_Map044
Field_Map044:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map044, .LField_Map044_CELL_PICTURES
	map_block Field_Map044, .LField_Map044_SHAPES
	map_block Field_Map044, .LField_Map044_GRID
	map_block Field_Map044, .LField_Map044_SHAPE_GRID
	map_block Field_Map044, .LField_Map044_ANIMATION
	map_block Field_Map044
	map_block Field_Map044, .LField_Map044_SCENE
.LField_Map044_CELL_PICTURES:
	.incbin "games/tla/src/map/map_044/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map044_SHAPES:
	.incbin "games/tla/src/map/map_044/shapes.table.lz"
	.balign 4, 0
.LField_Map044_GRID:
	.incbin "games/tla/src/map/map_044/map.grid.lz"
	.balign 4, 0
.LField_Map044_SHAPE_GRID:
	.incbin "games/tla/src/map/map_044/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map044_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map044_SCENE:
	.incbin "games/tla/src/map/map_044/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set27Palette
Tileset_Set27Palette:
	.incbin "games/tla/src/graphics/tileset/set_27/palette.gbapal.lz"
	.byte 0x60, 0x3f, 0xbe	@ PACKER: left in the buffer by Field_Map044+0x115
	.global Tileset_Set27TilesA
Tileset_Set27TilesA:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_a.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Field_Map041+0x22d3
	.global Tileset_Set27TilesB
Tileset_Set27TilesB:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_b.4bpp.mtf"
	.byte 0x11, 0xa7	@ PACKER: left in the buffer by Tileset_Set27TilesA+0x1c52
	.global Tileset_Set27TilesC
Tileset_Set27TilesC:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_c.4bpp.mtf"
	.byte 0x43	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x643
	.global Tileset_Set27TilesD
Tileset_Set27TilesD:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_d.4bpp.mtf"
	.byte 0x50	@ PACKER: left in the buffer by Tileset_Set27TilesC+0x13f
