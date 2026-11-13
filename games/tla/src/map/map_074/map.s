	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map074
Field_Map074:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map074, .LField_Map074_CELL_PICTURES
	map_block Field_Map074, .LField_Map074_SHAPES
	map_block Field_Map074, .LField_Map074_GRID
	map_block Field_Map074, .LField_Map074_SHAPE_GRID
	map_block Field_Map074, .LField_Map074_ANIMATION
	map_block Field_Map074
	map_block Field_Map074, .LField_Map074_SCENE
.LField_Map074_CELL_PICTURES:
	.incbin "games/tla/src/map/map_074/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map074_SHAPES:
	.incbin "games/tla/src/map/map_074/shapes.table.lz"
	.balign 4, 0
.LField_Map074_GRID:
	.incbin "games/tla/src/map/map_074/map.grid.lz"
	.balign 4, 0
.LField_Map074_SHAPE_GRID:
	.incbin "games/tla/src/map/map_074/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map074_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map074_SCENE:
	.incbin "games/tla/src/map/map_074/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set38Palette
Tileset_Set38Palette:
	.incbin "games/tla/src/graphics/tileset/set_38/palette.gbapal.lz"
	.byte 0x46	@ PACKER: left in the buffer by Field_Map074+0x14f
	.global Tileset_Set38TilesA
Tileset_Set38TilesA:
	.incbin "games/tla/src/graphics/tileset/set_38/tiles_a.4bpp.mtf"
	.byte 0x21, 0x0d	@ PACKER: left in the buffer by Field_Map074+0x1a06
