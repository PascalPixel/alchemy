	.global Field_TaopoSwamp9
Field_TaopoSwamp9:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_TaopoSwamp9, .LField_TaopoSwamp9_CELL_PICTURES
	map_block Field_TaopoSwamp9, .LField_TaopoSwamp9_SHAPES
	map_block Field_TaopoSwamp9, .LField_TaopoSwamp9_GRID
	map_block Field_TaopoSwamp9, .LField_TaopoSwamp9_SHAPE_GRID
	map_block Field_TaopoSwamp9, .LField_TaopoSwamp9_ANIMATION
	map_block Field_TaopoSwamp9, .LField_TaopoSwamp9_BLEND
	map_block Field_TaopoSwamp9, .LField_TaopoSwamp9_SCENE
.LField_TaopoSwamp9_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_9/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TaopoSwamp9_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_9/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp9_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_9/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_9/map.grid.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwamp9_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_9/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_9/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwamp9_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_9/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp9_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp9_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_9/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_9/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_PaletteTaopoSwamp9
Tileset_PaletteTaopoSwamp9:
	.incbin "games/tla/src/graphics/tileset/palettes/taopo_swamp_9.gbapal.lz"
	.byte 0x01, 0x2f	@ PACKER: left in the buffer by Field_TaopoSwamp9+0x1ae
