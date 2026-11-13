	.global Field_NorthernReaches
Field_NorthernReaches:
	map_header 0, 0, 92, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_NorthernReaches, .LField_NorthernReaches_CELL_PICTURES
	map_block Field_NorthernReaches, .LField_NorthernReaches_SHAPES
	map_block Field_NorthernReaches, .LField_NorthernReaches_GRID
	map_block Field_NorthernReaches, .LField_NorthernReaches_SHAPE_GRID
	map_block Field_NorthernReaches, .LField_NorthernReaches_ANIMATION
	map_block Field_NorthernReaches, .LField_NorthernReaches_BLEND
	map_block Field_NorthernReaches, .LField_NorthernReaches_SCENE
.LField_NorthernReaches_CELL_PICTURES:
	.incbin "games/tla/src/map/northern_reaches/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_NorthernReaches_SHAPES:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/northern_reaches/shapes_ja.table.lz"
	.else
	.incbin "games/tla/src/map/northern_reaches/shapes.table.lz"
	.endif
	.balign 4, 0
.LField_NorthernReaches_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/northern_reaches/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/northern_reaches/map.grid.lz"
	.endif
	.balign 4, 0
.LField_NorthernReaches_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/northern_reaches/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/northern_reaches/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_NorthernReaches_ANIMATION:
	.incbin "games/tla/src/map/northern_reaches/animation.script.lz"
	.balign 4, 0
.LField_NorthernReaches_BLEND:
	.incbin "games/tla/src/map/northern_reaches/blend.script.lz"
	.balign 4, 0
.LField_NorthernReaches_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/northern_reaches/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/northern_reaches/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_Palette57
Tileset_Palette57:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_57.gbapal.lz"
	.byte 0x2e	@ PACKER: left in the buffer by Field_NorthernReaches+0x117
