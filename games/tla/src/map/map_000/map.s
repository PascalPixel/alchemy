	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map000
Field_Map000:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map000, .LField_Map000_METATILES
	map_block Field_Map000, .LField_Map000_SHAPES
	map_block Field_Map000, .LField_Map000_GRID
	map_block Field_Map000, .LField_Map000_SHAPE_GRID
	map_block Field_Map000
	map_block Field_Map000
	map_block Field_Map000, .LField_Map000_SCENE
.LField_Map000_METATILES:
	.incbin "games/tla/src/map/map_000/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map000_SHAPES:
	.incbin "games/tla/src/map/map_000/shapes.table.lz"
	.balign 4, 0
.LField_Map000_GRID:
	.incbin "games/tla/src/map/map_000/grid.parts.lz"
	.balign 4, 0
.LField_Map000_SHAPE_GRID:
	.incbin "games/tla/src/map/map_000/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map000_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
