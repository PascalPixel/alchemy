	.global Field_Kibombo
Field_Kibombo:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Kibombo, .LField_Kibombo_CELL_TILES
	map_block Field_Kibombo, .LField_Kibombo_SHAPES
	map_block Field_Kibombo, .LField_Kibombo_GRID
	map_block Field_Kibombo, .LField_Kibombo_SHAPE_GRID
	map_block Field_Kibombo, .LField_Kibombo_ANIMATION
	map_block Field_Kibombo, .LField_Kibombo_BLEND
	map_block Field_Kibombo, .LField_Kibombo_SCENE
.LField_Kibombo_CELL_TILES:
	.incbin "games/tla/src/map/kibombo/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Kibombo_SHAPES:
	.incbin "games/tla/src/map/kibombo/shapes.table.lz"
	.balign 4, 0
.LField_Kibombo_GRID:
	.incbin "games/tla/src/map/kibombo/map.grid.lz"
	.balign 4, 0
.LField_Kibombo_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo/shape_grid.plane.lz"
	.balign 4, 0
.LField_Kibombo_ANIMATION:
	.incbin "games/tla/src/map/kibombo/animation.script.lz"
	.balign 4, 0
.LField_Kibombo_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Kibombo_SCENE:
	.incbin "games/tla/src/map/kibombo/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KibomboPalette
Tileset_KibomboPalette:
	.incbin "games/tla/src/graphics/tileset/kibombo/palette.gbapal.lz"
	.byte 0x5f, 0x52	@ PACKER: left in the buffer by Field_Kibombo+0x156
	.global Tileset_KibomboTilesA
Tileset_KibomboTilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0xd4, 0x0e, 0x40	@ PACKER: unused post-stream residue; Decompress_MoveToFrontLz uses decoded RAM and output length; writer unknown
	.else
	.byte 0xef, 0x12, 0x40	@ PACKER: unused post-stream residue; Decompress_MoveToFrontLz uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_KibomboTilesB
Tileset_KibomboTilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo/tiles_b.4bpp.mtf"
	.byte 0x8e, 0x16, 0x18	@ PACKER: left in the buffer by Tileset_KibomboTilesA+0x2221
	.global Tileset_KibomboTilesC
Tileset_KibomboTilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo/tiles_c.4bpp.mtf"
	.global Tileset_KibomboTilesD
Tileset_KibomboTilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x75	@ PACKER: left in the buffer by Tileset_KibomboTilesC+0x30a
