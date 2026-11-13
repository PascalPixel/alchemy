	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_CrossboneIsleIcePillars
Field_CrossboneIsleIcePillars:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleIcePillars, .LField_CrossboneIsleIcePillars_CELL_PICTURES
	map_offset Field_CrossboneIsleIcePillars, .LField_CrossboneIsleIcePillars_SHAPES
	map_offset Field_CrossboneIsleIcePillars, .LField_CrossboneIsleIcePillars_GRID
	map_offset Field_CrossboneIsleIcePillars, .LField_CrossboneIsleIcePillars_ANIMATION
	map_offset Field_CrossboneIsleIcePillars, .LField_CrossboneIsleIcePillars_BLEND
	map_offset Field_CrossboneIsleIcePillars, .LField_CrossboneIsleIcePillars_MARKERS
.LField_CrossboneIsleIcePillars_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_ice_pillars/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_CrossboneIsleIcePillars_SHAPES:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleIcePillars_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_ice_pillars/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleIcePillars_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleIcePillars_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleIcePillars_MARKERS:
	treasure_spot 19, 8, 100
	treasure_spot 7, 13, 101
	treasure_spot 18, 13, 102
	treasure_spot 12, 17, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteCrossboneIsleIcePillars
Tileset_PaletteCrossboneIsleIcePillars:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle_ice_pillars.gbapal.lz"
	.byte 0xf8, 0xe4	@ PACKER: left in the buffer by Field_CrossboneIsleIcePillars+0x17e
