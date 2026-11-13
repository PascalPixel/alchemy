	.global Field_GondowanCliffsHideout
Field_GondowanCliffsHideout:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanCliffsHideout, .LField_GondowanCliffsHideout_CELL_PICTURES
	map_block Field_GondowanCliffsHideout, .LField_GondowanCliffsHideout_SHAPES
	map_block Field_GondowanCliffsHideout, .LField_GondowanCliffsHideout_GRID
	map_block Field_GondowanCliffsHideout, .LField_GondowanCliffsHideout_SHAPE_GRID
	map_block Field_GondowanCliffsHideout, .LField_GondowanCliffsHideout_ANIMATION
	map_block Field_GondowanCliffsHideout, .LField_GondowanCliffsHideout_BLEND
	map_block Field_GondowanCliffsHideout, .LField_GondowanCliffsHideout_SCENE
.LField_GondowanCliffsHideout_CELL_PICTURES:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_GondowanCliffsHideout_SHAPES:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/shapes.table.lz"
	.balign 4, 0
.LField_GondowanCliffsHideout_GRID:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/map.grid.lz"
	.balign 4, 0
.LField_GondowanCliffsHideout_SHAPE_GRID:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/shape_grid.plane.lz"
	.balign 4, 0
.LField_GondowanCliffsHideout_ANIMATION:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/animation.script.lz"
	.balign 4, 0
.LField_GondowanCliffsHideout_BLEND:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/blend.script.lz"
	.balign 4, 0
.LField_GondowanCliffsHideout_SCENE:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GondowanCliffsHideoutPalette
Tileset_GondowanCliffsHideoutPalette:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs_hideout/palette.gbapal.lz"
	.byte 0x3a, 0x16, 0xda	@ PACKER: left in the buffer by Field_GondowanCliffsHideout+0x105
	.global Tileset_GondowanCliffsHideoutTilesA
Tileset_GondowanCliffsHideoutTilesA:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs_hideout/tiles_a.4bpp.mtf"
	.byte 0x7e, 0xa7	@ PACKER: left in the buffer by Tileset_Set17TilesC+0x1cbe
	.global Tileset_GondowanCliffsHideoutTilesB
Tileset_GondowanCliffsHideoutTilesB:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs_hideout/tiles_b.4bpp.mtf"
	.global Tileset_GondowanCliffsHideoutTilesC
Tileset_GondowanCliffsHideoutTilesC:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs_hideout/tiles_c.4bpp.mtf"
	.byte 0xd4	@ PACKER: left in the buffer by Tileset_GondowanCliffsHideoutTilesB+0x2df
	.global Tileset_GondowanCliffsHideoutTilesD
Tileset_GondowanCliffsHideoutTilesD:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs_hideout/tiles_d.4bpp.mtf"
	.byte 0x0f, 0x7c, 0xfe	@ PACKER: left in the buffer by Tileset_GondowanCliffsHideoutTilesC+0x2b5
