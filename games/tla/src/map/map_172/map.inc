	.global Field_Map172
Field_Map172:
	map_header 0, 0, 128, 96, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -3, 3, 0x1f, 0x1f
	map_block Field_Map172, .LField_Map172_CELL_PICTURES
	map_block Field_Map172, .LField_Map172_SHAPES
	map_block Field_Map172, .LField_Map172_GRID
	map_block Field_Map172, .LField_Map172_SHAPE_GRID
	map_block Field_Map172, .LField_Map172_ANIMATION
	map_block Field_Map172, .LField_Map172_BLEND
	map_block Field_Map172, .LField_Map172_SCENE
.LField_Map172_CELL_PICTURES:
	.incbin "games/tla/src/map/map_172/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map172_SHAPES:
	.incbin "games/tla/src/map/map_172/shapes.table.lz"
	.balign 4, 0
.LField_Map172_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_172/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/map_172/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Map172_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_172/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/map_172/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_Map172_ANIMATION:
	.incbin "games/tla/src/map/map_172/animation.script.lz"
	.balign 4, 0
.LField_Map172_BLEND:
	.incbin "games/tla/src/map/map_172/blend.script.lz"
	.balign 4, 0
.LField_Map172_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_172/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/map_172/scene.scene.lz"
	.endif
	.global Tileset_Palette30
Tileset_Palette30:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_30.gbapal.lz"
	.byte 0x22, 0xb6, 0x05	@ PACKER: left in the buffer by Field_Map172+0x175
