	.global Field_Contigo
Field_Contigo:
	map_header 0, 0, 128, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 1, 1, 0x1f, 0x1f
	map_block Field_Contigo, .LField_Contigo_CELL_TILES
	map_block Field_Contigo, .LField_Contigo_SHAPES
	map_block Field_Contigo, .LField_Contigo_GRID
	map_block Field_Contigo, .LField_Contigo_SHAPE_GRID
	map_block Field_Contigo, .LField_Contigo_ANIMATION
	map_block Field_Contigo, .LField_Contigo_BLEND
	map_block Field_Contigo, .LField_Contigo_SCENE
.LField_Contigo_CELL_TILES:
	.incbin "games/tla/src/map/contigo/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_Contigo_SHAPES:
	.incbin "games/tla/src/map/contigo/shapes.table.lz"
	.balign 4, 0
.LField_Contigo_GRID:
	.incbin "games/tla/src/map/contigo/map.grid.lz"
	.balign 4, 0
.LField_Contigo_SHAPE_GRID:
	.incbin "games/tla/src/map/contigo/shape_grid.plane.lz"
	.balign 4, 0
.LField_Contigo_ANIMATION:
	.incbin "games/tla/src/map/contigo/animation.script.lz"
	.balign 4, 0
.LField_Contigo_BLEND:
	.incbin "games/tla/src/map/contigo/blend.script.lz"
	.balign 4, 0
.LField_Contigo_SCENE:
	.incbin "games/tla/src/map/contigo/scene.scene.lz"
	.global Tileset_ContigoPalette
Tileset_ContigoPalette:
	.incbin "games/tla/src/graphics/tileset/contigo/palette.gbapal.lz"
	.byte 0xed	@ PACKER: left in the buffer by Field_Contigo1+0x193
	.global Tileset_ContigoTilesA
Tileset_ContigoTilesA:
	.incbin "games/tla/src/graphics/tileset/contigo/tiles_a.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
