	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map186
Field_Map186:
	map_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map186, .LField_Map186_METATILES
	map_block Field_Map186, .LField_Map186_SHAPES
	map_block Field_Map186, .LField_Map186_GRID
	map_block Field_Map186, .LField_Map186_SHAPE_GRID
	map_block Field_Map186, .LField_Map186_ANIMATION
	map_block Field_Map186, .LField_Map186_BLEND
	map_block Field_Map186, .LField_Map186_SCENE
.LField_Map186_METATILES:
	.incbin "games/tla/src/map/map_186/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map186_SHAPES:
	.incbin "games/tla/src/map/map_186/shapes.table.lz"
	.balign 4, 0
.LField_Map186_GRID:
	.incbin "games/tla/src/map/map_186/grid.parts.lz"
	.balign 4, 0
.LField_Map186_SHAPE_GRID:
	.incbin "games/tla/src/map/map_186/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map186_ANIMATION:
	.incbin "games/tla/src/map/map_183/animation.script.lz"
	.balign 4, 0
.LField_Map186_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map186_SCENE:
	.incbin "games/tla/src/map/map_186/scene.parts.lz"
	.balign 4, 0
