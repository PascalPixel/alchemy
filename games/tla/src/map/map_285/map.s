	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map285
Field_Map285:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map285, .LField_Map285_CELL_PICTURES
	map_block Field_Map285, .LField_Map285_SHAPES
	map_block Field_Map285, .LField_Map285_GRID
	map_block Field_Map285, .LField_Map285_SHAPE_GRID
	map_block Field_Map285, .LField_Map285_ANIMATION
	map_block Field_Map285, .LField_Map285_BLEND
	map_block Field_Map285, .LField_Map285_SCENE
.LField_Map285_CELL_PICTURES:
	.incbin "games/tla/src/map/map_285/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map285_SHAPES:
	.incbin "games/tla/src/map/map_285/shapes.table.lz"
	.balign 4, 0
.LField_Map285_GRID:
	.incbin "games/tla/src/map/map_285/map.grid.lz"
	.balign 4, 0
.LField_Map285_SHAPE_GRID:
	.incbin "games/tla/src/map/map_285/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map285_ANIMATION:
	.incbin "games/tla/src/map/map_285/animation.script.lz"
	.balign 4, 0
.LField_Map285_BLEND:
	.incbin "games/tla/src/map/map_285/blend.script.lz"
	.balign 4, 0
.LField_Map285_SCENE:
	.incbin "games/tla/src/map/map_285/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette69
Tileset_Palette69:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_69.gbapal.lz"
	.byte 0xbc, 0x70	@ PACKER: left in the buffer by Field_Map285+0x172
