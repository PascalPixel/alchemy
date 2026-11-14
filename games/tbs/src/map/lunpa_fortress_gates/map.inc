	.global Field_LunpaFortressGates
Field_LunpaFortressGates:
	map_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LunpaFortressGates, .LField_LunpaFortressGates_CELL_PICTURES
	map_offset Field_LunpaFortressGates, .LField_LunpaFortressGates_SHAPES
	map_offset Field_LunpaFortressGates, .LField_LunpaFortressGates_GRID
	map_offset Field_LunpaFortressGates, .LField_LunpaFortressGates_ANIMATION
	map_offset Field_LunpaFortressGates, .LField_LunpaFortressGates_BLEND
	map_offset Field_LunpaFortressGates, .LField_LunpaFortressGates_MARKERS
.LField_LunpaFortressGates_CELL_PICTURES:
	.incbin "games/tbs/src/map/lunpa_fortress_gates/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_LunpaFortressGates_SHAPES:
	.incbin "games/tbs/src/map/lunpa_fortress_gates/shapes.table.lz"
	.balign 4, 0
.LField_LunpaFortressGates_GRID:
	.incbin "games/tbs/src/map/lunpa_fortress_gates/map.grid.lz"
	.balign 4, 0
.LField_LunpaFortressGates_ANIMATION:
	.incbin "games/tbs/src/map/bilibin/tileset_anims.script.lz"
	.balign 4, 0
.LField_LunpaFortressGates_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_LunpaFortressGates_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteLunpaFortressGates
Tileset_PaletteLunpaFortressGates:
	.incbin "games/tbs/src/graphics/tileset/palettes/lunpa_fortress_gates.gbapal.lz"
	.balign 4, 0
