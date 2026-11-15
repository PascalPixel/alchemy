	.global Field_AnemosInnerSanctum1
Field_AnemosInnerSanctum1:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctum1, .LField_AnemosInnerSanctum1_CELL_PICTURES
	map_block Field_AnemosInnerSanctum1, .LField_AnemosInnerSanctum1_SHAPES
	map_block Field_AnemosInnerSanctum1, .LField_AnemosInnerSanctum1_GRID
	map_block Field_AnemosInnerSanctum1, .LField_AnemosInnerSanctum1_SHAPE_GRID
	map_block Field_AnemosInnerSanctum1, .LField_AnemosInnerSanctum1_ANIMATION
	map_block Field_AnemosInnerSanctum1, .LField_AnemosInnerSanctum1_BLEND
	map_block Field_AnemosInnerSanctum1, .LField_AnemosInnerSanctum1_SCENE
.LField_AnemosInnerSanctum1_CELL_PICTURES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum1_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_1/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum1_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_1/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum1_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum1_BLEND:
	.incbin "games/tla/src/map/anemos_inner_sanctum_1/blend.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum1_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AnemosInnerSanctum1Palette
Tileset_AnemosInnerSanctum1Palette:
	.incbin "games/tla/src/graphics/tileset/anemos_inner_sanctum_1/palette.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_AnemosInnerSanctum1+0x193
	.global Tileset_AnemosInnerSanctum1TilesA
Tileset_AnemosInnerSanctum1TilesA:
	.incbin "games/tla/src/graphics/tileset/anemos_inner_sanctum_1/tiles_a.4bpp.mtf"
	.byte 0x72	@ PACKER: left in the buffer by Field_ShamanVillage4+0x20b3
