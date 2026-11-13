	.global Field_AncientLemuria
Field_AncientLemuria:
	map_header 0, 0, 76, 128, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AncientLemuria, .LField_AncientLemuria_CELL_PICTURES
	map_block Field_AncientLemuria, .LField_AncientLemuria_SHAPES
	map_block Field_AncientLemuria, .LField_AncientLemuria_GRID
	map_block Field_AncientLemuria, .LField_AncientLemuria_SHAPE_GRID
	map_block Field_AncientLemuria, .LField_AncientLemuria_ANIMATION
	map_block Field_AncientLemuria, .LField_AncientLemuria_BLEND
	map_block Field_AncientLemuria, .LField_AncientLemuria_SCENE
.LField_AncientLemuria_CELL_PICTURES:
	.incbin "games/tla/src/map/ancient_lemuria/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AncientLemuria_SHAPES:
	.incbin "games/tla/src/map/ancient_lemuria/shapes.table.lz"
	.balign 4, 0
.LField_AncientLemuria_GRID:
	.incbin "games/tla/src/map/ancient_lemuria/map.grid.lz"
	.balign 4, 0
.LField_AncientLemuria_SHAPE_GRID:
	.incbin "games/tla/src/map/ancient_lemuria/shape_grid.plane.lz"
	.balign 4, 0
.LField_AncientLemuria_ANIMATION:
	.incbin "games/tla/src/map/ancient_lemuria/animation.script.lz"
	.balign 4, 0
.LField_AncientLemuria_BLEND:
	.incbin "games/tla/src/map/ancient_lemuria/blend.script.lz"
	.balign 4, 0
.LField_AncientLemuria_SCENE:
	.incbin "games/tla/src/map/ancient_lemuria/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette38
Tileset_Palette38:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_38.gbapal.lz"
	.byte 0x2d, 0x03, 0x3c	@ PACKER: left in the buffer by Field_AncientLemuria+0x155
