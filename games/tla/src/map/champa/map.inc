	.global Field_Champa
Field_Champa:
	map_header 0, 0, 128, 96, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -3, 3, 0x1f, 0x1f
	map_block Field_Champa, .LField_Champa_CELL_PICTURES
	map_block Field_Champa, .LField_Champa_SHAPES
	map_block Field_Champa, .LField_Champa_GRID
	map_block Field_Champa, .LField_Champa_SHAPE_GRID
	map_block Field_Champa, .LField_Champa_ANIMATION
	map_block Field_Champa, .LField_Champa_BLEND
	map_block Field_Champa, .LField_Champa_SCENE
.LField_Champa_CELL_PICTURES:
	.incbin "games/tla/src/map/champa/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Champa_SHAPES:
	.incbin "games/tla/src/map/champa/shapes.table.lz"
	.balign 4, 0
.LField_Champa_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/champa/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Champa_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/champa/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_Champa_ANIMATION:
	.incbin "games/tla/src/map/champa/animation.script.lz"
	.balign 4, 0
.LField_Champa_BLEND:
	.incbin "games/tla/src/map/champa/blend.script.lz"
	.balign 4, 0
.LField_Champa_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/champa/scene.scene.lz"
	.endif
	.global Tileset_PaletteChampa
Tileset_PaletteChampa:
	.incbin "games/tla/src/graphics/tileset/palettes/champa.gbapal.lz"
	.byte 0x22, 0xb6, 0x05	@ PACKER: left in the buffer by Field_Champa+0x175
