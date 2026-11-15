	.global Field_Garoh4
Field_Garoh4:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Garoh4, .LField_Garoh4_CELL_PICTURES
	map_block Field_Garoh4, .LField_Garoh4_SHAPES
	map_block Field_Garoh4, .LField_Garoh4_GRID
	map_block Field_Garoh4, .LField_Garoh4_SHAPE_GRID
	map_block Field_Garoh4, .LField_Garoh4_ANIMATION
	map_block Field_Garoh4, .LField_Garoh4_BLEND
	map_block Field_Garoh4, .LField_Garoh4_SCENE
.LField_Garoh4_CELL_PICTURES:
	.incbin "games/tla/src/map/garoh_4/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Garoh4_SHAPES:
	.incbin "games/tla/src/map/garoh_4/shapes.table.lz"
	.balign 4, 0
.LField_Garoh4_GRID:
	.incbin "games/tla/src/map/garoh_4/map.grid.lz"
	.balign 4, 0
.LField_Garoh4_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_Garoh4_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_Garoh4_BLEND:
	.incbin "games/tla/src/map/garoh_4/blend.script.lz"
	.balign 4, 0
.LField_Garoh4_SCENE:
	.incbin "games/tla/src/map/garoh_4/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGaroh4
Tileset_PaletteGaroh4:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_4.gbapal.lz"
	.byte 0xf0	@ PACKER: left in the buffer by Field_Garoh4+0xf3
