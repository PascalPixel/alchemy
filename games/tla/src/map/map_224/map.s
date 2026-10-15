	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map224
Field_Map224:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map224, .LField_Map224_METATILES
	map_block Field_Map224, .LField_Map224_SHAPES
	map_block Field_Map224, .LField_Map224_GRID
	map_block Field_Map224, .LField_Map224_SHAPE_GRID
	map_block Field_Map224, .LField_Map224_ANIMATION
	map_block Field_Map224, .LField_Map224_BLEND
	map_block Field_Map224, .LField_Map224_SCENE
.LField_Map224_METATILES:
	.incbin "games/tla/src/map/map_224/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map224_SHAPES:
	.incbin "games/tla/src/map/map_224/shapes.table.lz"
	.balign 4, 0
.LField_Map224_GRID:
	.incbin "games/tla/src/map/map_224/grid.parts.lz"
	.balign 4, 0
.LField_Map224_SHAPE_GRID:
	.incbin "games/tla/src/map/map_224/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map224_ANIMATION:
	.incbin "games/tla/src/map/map_224/animation.script.lz"
	.balign 4, 0
.LField_Map224_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map224_SCENE:
	.incbin "games/tla/src/map/map_224/scene.parts.lz"
	.balign 4, 0
