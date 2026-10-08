	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map276
Field_Map276:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map276, .LField_Map276_METATILES
	map_block Field_Map276, .LField_Map276_SHAPES
	map_block Field_Map276, .LField_Map276_GRID
	map_block Field_Map276, .LField_Map276_SHAPE_GRID
	map_block Field_Map276, .LField_Map276_ANIMATION
	map_block Field_Map276
	map_block Field_Map276, .LField_Map276_SCENE
.LField_Map276_METATILES:
	.incbin "map/map_276/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map276_SHAPES:
	.incbin "map/map_276/shapes.table.lz"
	.balign 4, 0
.LField_Map276_GRID:
	.incbin "map/map_276/grid.parts.lz"
	.balign 4, 0
.LField_Map276_SHAPE_GRID:
	.incbin "map/map_276/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map276_ANIMATION:
	.incbin "map/map_263/animation.script.lz"
	.balign 4, 0
.LField_Map276_SCENE:
	.incbin "map/map_276/scene.parts.lz"
