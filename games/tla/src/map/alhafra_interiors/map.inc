	.global Field_AlhafraInteriors
Field_AlhafraInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AlhafraInteriors, .LField_AlhafraInteriors_CELL_PICTURES
	map_block Field_AlhafraInteriors, .LField_AlhafraInteriors_SHAPES
	map_block Field_AlhafraInteriors, .LField_AlhafraInteriors_GRID
	map_block Field_AlhafraInteriors, .LField_AlhafraInteriors_SHAPE_GRID
	map_block Field_AlhafraInteriors, .LField_AlhafraInteriors_ANIMATION
	map_block Field_AlhafraInteriors, .LField_AlhafraInteriors_BLEND
	map_block Field_AlhafraInteriors, .LField_AlhafraInteriors_SCENE
.LField_AlhafraInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/alhafra_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AlhafraInteriors_SHAPES:
	.incbin "games/tla/src/map/alhafra_interiors/shapes.table.lz"
	.balign 4, 0
.LField_AlhafraInteriors_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/alhafra_interiors/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/alhafra_interiors/map.grid.lz"
	.endif
	.balign 4, 0
.LField_AlhafraInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/alhafra_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_AlhafraInteriors_ANIMATION:
	.incbin "games/tla/src/map/mikasalla_interiors/animation.script.lz"
	.balign 4, 0
.LField_AlhafraInteriors_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_AlhafraInteriors_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/alhafra_interiors/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/alhafra_interiors/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_Set42Palette
Tileset_Set42Palette:
	.incbin "games/tla/src/graphics/tileset/set_42/palette.gbapal.lz"
	.global Tileset_Set42TilesA
Tileset_Set42TilesA:
	.incbin "games/tla/src/graphics/tileset/set_42/tiles_a.4bpp.mtf"
	.byte 0x70, 0x0e	@ PACKER: left in the buffer by Field_AlhafraInteriors+0x15e6
