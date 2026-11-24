	.global Field_IsletsCow
Field_IsletsCow:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IsletsCow, .LField_IsletsCow_CELL_TILES
	map_block Field_IsletsCow, .LField_IsletsCow_SHAPES
	map_block Field_IsletsCow, .LField_IsletsCow_GRID
	map_block Field_IsletsCow, .LField_IsletsCow_SHAPE_GRID
	map_block Field_IsletsCow, .LField_IsletsCow_ANIMATION
	map_block Field_IsletsCow
	map_block Field_IsletsCow, .LField_IsletsCow_SCENE
.LField_IsletsCow_CELL_TILES:
	.incbin "games/tla/src/map/islets_cow/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_IsletsCow_SHAPES:
	.incbin "games/tla/src/map/islets_cow/shapes.table.lz"
	.balign 4, 0
.LField_IsletsCow_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/islets_cow/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/islets_cow/map.grid.lz"
	.endif
	.balign 4, 0
.LField_IsletsCow_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_cow/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletsCow_ANIMATION:
	.incbin "games/tla/src/map/daila_flooded/animation.script.lz"
	.balign 4, 0
.LField_IsletsCow_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/islets_cow/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/islets_cow/scene.scene.lz"
	.endif
	.global Tileset_PaletteIsletsCow
Tileset_PaletteIsletsCow:
	.incbin "games/tla/src/graphics/tileset/palettes/islets_cow.gbapal.lz"
	.byte 0xe0, 0xe4	@ PACKER: left in the buffer by Field_Islets3+0x18e
