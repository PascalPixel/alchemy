	.global Field_LemuriaInteriors
Field_LemuriaInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemuriaInteriors, .LField_LemuriaInteriors_CELL_PICTURES
	map_block Field_LemuriaInteriors, .LField_LemuriaInteriors_SHAPES
	map_block Field_LemuriaInteriors, .LField_LemuriaInteriors_GRID
	map_block Field_LemuriaInteriors, .LField_LemuriaInteriors_SHAPE_GRID
	map_block Field_LemuriaInteriors, .LField_LemuriaInteriors_ANIMATION
	map_block Field_LemuriaInteriors
	map_block Field_LemuriaInteriors, .LField_LemuriaInteriors_SCENE
.LField_LemuriaInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/lemuria_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LemuriaInteriors_SHAPES:
	.incbin "games/tla/src/map/lemuria_interiors/shapes.table.lz"
	.balign 4, 0
.LField_LemuriaInteriors_GRID:
	.incbin "games/tla/src/map/lemuria_interiors/map.grid.lz"
	.balign 4, 0
.LField_LemuriaInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/lemuria_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemuriaInteriors_ANIMATION:
	.incbin "games/tla/src/map/lemuria_interiors/animation.script.lz"
	.balign 4, 0
.LField_LemuriaInteriors_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/lemuria_interiors/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/lemuria_interiors/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_LemuriaInteriorsPalette
Tileset_LemuriaInteriorsPalette:
	.incbin "games/tla/src/graphics/tileset/lemuria_interiors/palette.gbapal.lz"
	.byte 0x9d, 0xf8	@ PACKER: left in the buffer by Field_LemuriaInteriors+0x1d2
	.global Tileset_LemuriaInteriorsTilesA
Tileset_LemuriaInteriorsTilesA:
	.incbin "games/tla/src/graphics/tileset/lemuria_interiors/tiles_a.4bpp.mtf"
	.byte 0x0a, 0x16	@ PACKER: left in the buffer by Field_LemuriaInteriors+0x203e
	.global Tileset_LemuriaInteriorsTilesB
Tileset_LemuriaInteriorsTilesB:
	.incbin "games/tla/src/graphics/tileset/lemuria_interiors/tiles_b.4bpp.mtf"
	.byte 0x40, 0x60, 0xb9	@ PACKER: left in the buffer by Tileset_LemuriaInteriorsTilesA+0x13f5
	.global Tileset_LemuriaInteriorsTilesC
Tileset_LemuriaInteriorsTilesC:
	.incbin "games/tla/src/graphics/tileset/lemuria_interiors/tiles_c.4bpp.mtf"
	.byte 0x44, 0xd3	@ PACKER: left in the buffer by Tileset_LemuriaInteriorsTilesB+0xb3a
	.global Tileset_LemuriaInteriorsTilesD
Tileset_LemuriaInteriorsTilesD:
	.incbin "games/tla/src/graphics/tileset/lemuria_interiors/tiles_d.4bpp.mtf"
	.byte 0xbf, 0x45, 0xca	@ PACKER: left in the buffer by Tileset_LemuriaInteriorsTilesB+0x1169
