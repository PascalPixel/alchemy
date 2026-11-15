	.global Field_Kibombo3
Field_Kibombo3:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Kibombo3, .LField_Kibombo3_CELL_PICTURES
	map_block Field_Kibombo3, .LField_Kibombo3_SHAPES
	map_block Field_Kibombo3, .LField_Kibombo3_GRID
	map_block Field_Kibombo3, .LField_Kibombo3_SHAPE_GRID
	map_block Field_Kibombo3, .LField_Kibombo3_ANIMATION
	map_block Field_Kibombo3, .LField_Kibombo3_BLEND
	map_block Field_Kibombo3, .LField_Kibombo3_SCENE
.LField_Kibombo3_CELL_PICTURES:
	.incbin "games/tla/src/map/kibombo_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Kibombo3_SHAPES:
	.incbin "games/tla/src/map/kibombo_3/shapes.table.lz"
	.balign 4, 0
.LField_Kibombo3_GRID:
	.incbin "games/tla/src/map/kibombo_3/map.grid.lz"
	.balign 4, 0
.LField_Kibombo3_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_Kibombo3_ANIMATION:
	.incbin "games/tla/src/map/naribwe_interiors/animation.script.lz"
	.balign 4, 0
.LField_Kibombo3_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_Kibombo3_SCENE:
	.incbin "games/tla/src/map/kibombo_3/scene.scene.lz"
	.global Tileset_PaletteKibombo3
Tileset_PaletteKibombo3:
	.incbin "games/tla/src/graphics/tileset/palettes/kibombo_3.gbapal.lz"
	.byte 0xa0, 0x05	@ PACKER: left in the buffer by Field_Kibombo3+0x1e2
