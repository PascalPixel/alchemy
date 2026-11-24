	.global Field_KibomboHouses
Field_KibomboHouses:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboHouses, .LField_KibomboHouses_CELL_TILES
	map_block Field_KibomboHouses, .LField_KibomboHouses_SHAPES
	map_block Field_KibomboHouses, .LField_KibomboHouses_GRID
	map_block Field_KibomboHouses, .LField_KibomboHouses_SHAPE_GRID
	map_block Field_KibomboHouses, .LField_KibomboHouses_ANIMATION
	map_block Field_KibomboHouses, .LField_KibomboHouses_BLEND
	map_block Field_KibomboHouses, .LField_KibomboHouses_SCENE
.LField_KibomboHouses_CELL_TILES:
	.incbin "games/tla/src/map/kibombo_houses/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_KibomboHouses_SHAPES:
	.incbin "games/tla/src/map/kibombo_houses/shapes.table.lz"
	.balign 4, 0
.LField_KibomboHouses_GRID:
	.incbin "games/tla/src/map/kibombo_houses/map.grid.lz"
	.balign 4, 0
.LField_KibomboHouses_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_houses/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboHouses_ANIMATION:
	.incbin "games/tla/src/map/naribwe_rooms/animation.script.lz"
	.balign 4, 0
.LField_KibomboHouses_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_KibomboHouses_SCENE:
	.incbin "games/tla/src/map/kibombo_houses/scene.scene.lz"
	.global Tileset_PaletteKibombo3
Tileset_PaletteKibombo3:
	.incbin "games/tla/src/graphics/tileset/palettes/kibombo_houses.gbapal.lz"
	.byte 0xa0, 0x05	@ PACKER: left in the buffer by Field_KibomboHouses+0x1e2
