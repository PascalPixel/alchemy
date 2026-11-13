	.global Field_Alhafra
Field_Alhafra:
	map_header 0, 0, 100, 128, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Alhafra, .LField_Alhafra_CELL_PICTURES
	map_block Field_Alhafra, .LField_Alhafra_SHAPES
	map_block Field_Alhafra, .LField_Alhafra_GRID
	map_block Field_Alhafra, .LField_Alhafra_SHAPE_GRID
	map_block Field_Alhafra, .LField_Alhafra_ANIMATION
	map_block Field_Alhafra, .LField_Alhafra_BLEND
	map_block Field_Alhafra, .LField_Alhafra_SCENE
.LField_Alhafra_CELL_PICTURES:
	.incbin "games/tla/src/map/alhafra/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Alhafra_SHAPES:
	.incbin "games/tla/src/map/alhafra/shapes.table.lz"
	.balign 4, 0
.LField_Alhafra_GRID:
	.incbin "games/tla/src/map/alhafra/map.grid.lz"
	.balign 4, 0
.LField_Alhafra_SHAPE_GRID:
	.incbin "games/tla/src/map/alhafra/shape_grid.plane.lz"
	.balign 4, 0
.LField_Alhafra_ANIMATION:
	.incbin "games/tla/src/map/madra/animation.script.lz"
	.balign 4, 0
.LField_Alhafra_BLEND:
	.incbin "games/tla/src/map/alhafra/blend.script.lz"
	.balign 4, 0
.LField_Alhafra_SCENE:
	.incbin "games/tla/src/map/alhafra/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteAlhafra
Tileset_PaletteAlhafra:
	.incbin "games/tla/src/graphics/tileset/palettes/alhafra.gbapal.lz"
	.byte 0x10, 0x08	@ PACKER: left in the buffer by Field_Alhafra+0x11a
