	.global Field_Islets2
Field_Islets2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Islets2, .LField_Islets2_CELL_PICTURES
	map_block Field_Islets2, .LField_Islets2_SHAPES
	map_block Field_Islets2, .LField_Islets2_GRID
	map_block Field_Islets2, .LField_Islets2_SHAPE_GRID
	map_block Field_Islets2, .LField_Islets2_ANIMATION
	map_block Field_Islets2
	map_block Field_Islets2, .LField_Islets2_SCENE
.LField_Islets2_CELL_PICTURES:
	.incbin "games/tla/src/map/islets_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Islets2_SHAPES:
	.incbin "games/tla/src/map/islets_2/shapes.table.lz"
	.balign 4, 0
.LField_Islets2_GRID:
	.incbin "games/tla/src/map/islets_2/map.grid.lz"
	.balign 4, 0
.LField_Islets2_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Islets2_ANIMATION:
	.incbin "games/tla/src/map/islets_2/animation.script.lz"
	.balign 4, 0
.LField_Islets2_SCENE:
	.incbin "games/tla/src/map/islets_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteIslets2
Tileset_PaletteIslets2:
	.incbin "games/tla/src/graphics/tileset/palettes/islets_2.gbapal.lz"
	.byte 0xac, 0x2f	@ PACKER: left in the buffer by Field_Islets2+0x116
