	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map108
Field_Map108:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map108, .LField_Map108_CELL_PICTURES
	map_block Field_Map108, .LField_Map108_SHAPES
	map_block Field_Map108, .LField_Map108_GRID
	map_block Field_Map108, .LField_Map108_SHAPE_GRID
	map_block Field_Map108, .LField_Map108_ANIMATION
	map_block Field_Map108, .LField_Map108_BLEND
	map_block Field_Map108, .LField_Map108_SCENE
.LField_Map108_CELL_PICTURES:
	.incbin "games/tla/src/map/map_108/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map108_SHAPES:
	.incbin "games/tla/src/map/map_108/shapes.table.lz"
	.balign 4, 0
.LField_Map108_GRID:
	.incbin "games/tla/src/map/map_108/map.grid.lz"
	.balign 4, 0
.LField_Map108_SHAPE_GRID:
	.incbin "games/tla/src/map/map_108/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map108_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map108_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map108_SCENE:
	.incbin "games/tla/src/map/map_108/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette14
Tileset_Palette14:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_14.gbapal.lz"
	.byte 0xb5	@ PACKER: left in the buffer by Field_Map108+0xe3
