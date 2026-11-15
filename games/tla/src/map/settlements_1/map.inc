	.global Field_Settlements1
Field_Settlements1:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Settlements1, .LField_Settlements1_CELL_PICTURES
	map_block Field_Settlements1, .LField_Settlements1_SHAPES
	map_block Field_Settlements1, .LField_Settlements1_GRID
	map_block Field_Settlements1, .LField_Settlements1_SHAPE_GRID
	map_block Field_Settlements1, .LField_Settlements1_ANIMATION
	map_block Field_Settlements1, .LField_Settlements1_BLEND
	map_block Field_Settlements1, .LField_Settlements1_SCENE
.LField_Settlements1_CELL_PICTURES:
	.incbin "games/tla/src/map/settlements_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Settlements1_SHAPES:
	.incbin "games/tla/src/map/settlements_1/shapes.table.lz"
	.balign 4, 0
.LField_Settlements1_GRID:
	.incbin "games/tla/src/map/settlements_1/map.grid.lz"
	.balign 4, 0
.LField_Settlements1_SHAPE_GRID:
	.incbin "games/tla/src/map/settlements_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Settlements1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_Settlements1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Settlements1_SCENE:
	.incbin "games/tla/src/map/settlements_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteSettlements1
Tileset_PaletteSettlements1:
	.incbin "games/tla/src/graphics/tileset/palettes/settlements_1.gbapal.lz"
	.byte 0x38, 0x10	@ PACKER: left in the buffer by Field_Settlements1+0x14e
