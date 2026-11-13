	.global Field_GondowanCliffs
Field_GondowanCliffs:
	map_header 0, 0, 0, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 192, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanCliffs, .LField_GondowanCliffs_CELL_PICTURES
	map_block Field_GondowanCliffs, .LField_GondowanCliffs_SHAPES
	map_block Field_GondowanCliffs, .LField_GondowanCliffs_GRID
	map_block Field_GondowanCliffs, .LField_GondowanCliffs_SHAPE_GRID
	map_block Field_GondowanCliffs, .LField_GondowanCliffs_ANIMATION
	map_block Field_GondowanCliffs, .LField_GondowanCliffs_BLEND
	map_block Field_GondowanCliffs, .LField_GondowanCliffs_SCENE
.LField_GondowanCliffs_CELL_PICTURES:
	.incbin "games/tla/src/map/gondowan_cliffs/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_GondowanCliffs_SHAPES:
	.incbin "games/tla/src/map/gondowan_cliffs/shapes.table.lz"
	.balign 4, 0
.LField_GondowanCliffs_GRID:
	.incbin "games/tla/src/map/gondowan_cliffs/map.grid.lz"
	.balign 4, 0
.LField_GondowanCliffs_SHAPE_GRID:
	.incbin "games/tla/src/map/gondowan_cliffs/shape_grid.plane.lz"
	.balign 4, 0
.LField_GondowanCliffs_ANIMATION:
	.incbin "games/tla/src/map/gondowan_cliffs/animation.script.lz"
	.balign 4, 0
.LField_GondowanCliffs_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GondowanCliffs_SCENE:
	.incbin "games/tla/src/map/gondowan_cliffs/scene.scene.lz"
	.global Tileset_GondowanCliffsPalette
Tileset_GondowanCliffsPalette:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs/palette.gbapal.lz"
	.byte 0xe3, 0xe0, 0x4b	@ PACKER: left in the buffer by Field_GondowanCliffs+0x121
	.global Tileset_GondowanCliffsTilesA
Tileset_GondowanCliffsTilesA:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs/tiles_a.4bpp.mtf"
	.global Tileset_GondowanCliffsTilesB
Tileset_GondowanCliffsTilesB:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs/tiles_b.4bpp.mtf"
	.byte 0xd6	@ PACKER: left in the buffer by Tileset_GondowanCliffsTilesA+0x13cf
	.global Tileset_GondowanCliffsTilesC
Tileset_GondowanCliffsTilesC:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs/tiles_c.4bpp.mtf"
	.byte 0x8f, 0xa4	@ PACKER: left in the buffer by Tileset_GondowanCliffsTilesB+0xdf2
	.global Tileset_GondowanCliffsTilesD
Tileset_GondowanCliffsTilesD:
	.incbin "games/tla/src/graphics/tileset/gondowan_cliffs/tiles_d.4bpp.mtf"
	.byte 0x50, 0x4e	@ PACKER: left in the buffer by Tileset_GondowanCliffsTilesC+0xd76
