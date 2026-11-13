	.global Field_Map255
Field_Map255:
	map_header 0, 0, 90, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map255, .LField_Map255_CELL_PICTURES
	map_block Field_Map255, .LField_Map255_SHAPES
	map_block Field_Map255, .LField_Map255_GRID
	map_block Field_Map255, .LField_Map255_SHAPE_GRID
	map_block Field_Map255, .LField_Map255_ANIMATION
	map_block Field_Map255, .LField_Map255_BLEND
	map_block Field_Map255, .LField_Map255_SCENE
.LField_Map255_CELL_PICTURES:
	.incbin "games/tla/src/map/map_255/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map255_SHAPES:
	.incbin "games/tla/src/map/map_255/shapes.table.lz"
	.balign 4, 0
.LField_Map255_GRID:
	.incbin "games/tla/src/map/map_255/map.grid.lz"
	.balign 4, 0
.LField_Map255_SHAPE_GRID:
	.incbin "games/tla/src/map/map_255/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map255_ANIMATION:
	.incbin "games/tla/src/map/map_254/animation.script.lz"
	.balign 4, 0
.LField_Map255_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map255_SCENE:
	.incbin "games/tla/src/map/map_255/scene.scene.lz"
	.global Tileset_Palette58
Tileset_Palette58:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_58.gbapal.lz"
	.byte 0xbb, 0x1e	@ PACKER: left in the buffer by Field_Map255+0x162
