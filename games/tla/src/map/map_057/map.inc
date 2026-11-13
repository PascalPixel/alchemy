	.global Field_Map057
Field_Map057:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map057, .LField_Map057_CELL_PICTURES
	map_block Field_Map057, .LField_Map057_SHAPES
	map_block Field_Map057, .LField_Map057_GRID
	map_block Field_Map057, .LField_Map057_SHAPE_GRID
	map_block Field_Map057, .LField_Map057_ANIMATION
	map_block Field_Map057, .LField_Map057_BLEND
	map_block Field_Map057, .LField_Map057_SCENE
.LField_Map057_CELL_PICTURES:
	.incbin "games/tla/src/map/map_057/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map057_SHAPES:
	.incbin "games/tla/src/map/map_057/shapes.table.lz"
	.balign 4, 0
.LField_Map057_GRID:
	.incbin "games/tla/src/map/map_057/map.grid.lz"
	.balign 4, 0
.LField_Map057_SHAPE_GRID:
	.incbin "games/tla/src/map/map_057/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map057_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map057_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map057_SCENE:
	.incbin "games/tla/src/map/map_057/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette05
Tileset_Palette05:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_05.gbapal.lz"
	.byte 0xf0	@ PACKER: left in the buffer by Field_Map057+0xf3
