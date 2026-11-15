	.global Field_Caverns4
Field_Caverns4:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Caverns4, .LField_Caverns4_CELL_PICTURES
	map_block Field_Caverns4, .LField_Caverns4_SHAPES
	map_block Field_Caverns4, .LField_Caverns4_GRID
	map_block Field_Caverns4, .LField_Caverns4_SHAPE_GRID
	map_block Field_Caverns4, .LField_Caverns4_ANIMATION
	map_block Field_Caverns4, .LField_Caverns4_BLEND
	map_block Field_Caverns4, .LField_Caverns4_SCENE
.LField_Caverns4_CELL_PICTURES:
	.incbin "games/tla/src/map/caverns_4/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Caverns4_SHAPES:
	.incbin "games/tla/src/map/caverns_4/shapes.table.lz"
	.balign 4, 0
.LField_Caverns4_GRID:
	.incbin "games/tla/src/map/caverns_4/map.grid.lz"
	.balign 4, 0
.LField_Caverns4_SHAPE_GRID:
	.incbin "games/tla/src/map/caverns_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_Caverns4_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_Caverns4_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Caverns4_SCENE:
	.incbin "games/tla/src/map/caverns_4/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteCaverns4
Tileset_PaletteCaverns4:
	.incbin "games/tla/src/graphics/tileset/palettes/caverns_4.gbapal.lz"
	.byte 0x68, 0xe9, 0x10	@ PACKER: left in the buffer by Field_Caverns4+0x151
