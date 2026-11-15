	.global Field_Islets3
Field_Islets3:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Islets3, .LField_Islets3_CELL_PICTURES
	map_block Field_Islets3, .LField_Islets3_SHAPES
	map_block Field_Islets3, .LField_Islets3_GRID
	map_block Field_Islets3, .LField_Islets3_SHAPE_GRID
	map_block Field_Islets3, .LField_Islets3_ANIMATION
	map_block Field_Islets3
	map_block Field_Islets3, .LField_Islets3_SCENE
.LField_Islets3_CELL_PICTURES:
	.incbin "games/tla/src/map/islets_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Islets3_SHAPES:
	.incbin "games/tla/src/map/islets_3/shapes.table.lz"
	.balign 4, 0
.LField_Islets3_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/islets_3/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/islets_3/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Islets3_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_Islets3_ANIMATION:
	.incbin "games/tla/src/map/daila_1/animation.script.lz"
	.balign 4, 0
.LField_Islets3_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/islets_3/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/islets_3/scene.scene.lz"
	.endif
	.global Tileset_PaletteIslets3
Tileset_PaletteIslets3:
	.incbin "games/tla/src/graphics/tileset/palettes/islets_3.gbapal.lz"
	.byte 0xe0, 0xe4	@ PACKER: left in the buffer by Field_Islets3+0x18e
