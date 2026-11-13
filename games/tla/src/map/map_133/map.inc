	.global Field_Map133
Field_Map133:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_CELL_PICTURES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_SHAPE_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_SCENE
.LField_Map133_CELL_PICTURES:
	.incbin "games/tla/src/map/map_133/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "games/tla/src/map/map_133/shapes.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "games/tla/src/map/map_133/map.grid.lz"
	.balign 4, 0
.LField_Map133_SHAPE_GRID:
	.incbin "games/tla/src/map/map_133/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "games/tla/src/map/map_133/animation.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map133_SCENE:
	.incbin "games/tla/src/map/map_133/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set64Palette
Tileset_Set64Palette:
	.incbin "games/tla/src/graphics/tileset/set_64/palette.gbapal.lz"
	.byte 0x18, 0x8c	@ PACKER: left in the buffer by Field_Map133+0x116
	.global Tileset_Set64TilesA
Tileset_Set64TilesA:
	.incbin "games/tla/src/graphics/tileset/set_64/tiles_a.4bpp.mtf"
	.byte 0x00, 0x1b, 0x22	@ PACKER: left in the buffer by Field_YallamInteriors+0x2091
	.global Tileset_Set64TilesB
Tileset_Set64TilesB:
	.incbin "games/tla/src/graphics/tileset/set_64/tiles_b.4bpp.mtf"
	.global Tileset_Set64TilesC
Tileset_Set64TilesC:
	.incbin "games/tla/src/graphics/tileset/set_64/tiles_c.4bpp.mtf"
	.byte 0xf5	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x697
	.global Tileset_Set64TilesD
Tileset_Set64TilesD:
	.incbin "games/tla/src/graphics/tileset/set_64/tiles_d.4bpp.mtf"
