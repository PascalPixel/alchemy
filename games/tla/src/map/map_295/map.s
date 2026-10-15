	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map295
Field_Map295:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map295, .LField_Map295_METATILES
	map_block Field_Map295, .LField_Map295_SHAPES
	map_block Field_Map295, .LField_Map295_GRID
	map_block Field_Map295, .LField_Map295_SHAPE_GRID
	map_block Field_Map295
	map_block Field_Map295
	map_block Field_Map295, .LField_Map295_SCENE
.LField_Map295_METATILES:
	.incbin "games/common/src/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map295_SHAPES:
	.incbin "games/tla/src/map/map_018/shapes.table.lz"
	.balign 4, 0
.LField_Map295_GRID:
	.incbin "games/tla/src/map/map_295/grid.parts.lz"
	.balign 4, 0
.LField_Map295_SHAPE_GRID:
	.incbin "games/tla/src/map/map_018/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map295_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
