	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map294
Field_Map294:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map294, .LField_Map294_METATILES
	map_block Field_Map294, .LField_Map294_SHAPES
	map_block Field_Map294, .LField_Map294_GRID
	map_block Field_Map294, .LField_Map294_SHAPE_GRID
	map_block Field_Map294
	map_block Field_Map294
	map_block Field_Map294, .LField_Map294_SCENE
.LField_Map294_METATILES:
	.incbin "games/common/src/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map294_SHAPES:
	.incbin "games/tla/src/map/map_220/shapes.table.lz"
	.balign 4, 0
.LField_Map294_GRID:
	.incbin "games/tla/src/map/map_294/grid.parts.lz"
	.balign 4, 0
.LField_Map294_SHAPE_GRID:
	.incbin "games/tla/src/map/map_294/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map294_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
