	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_CrossboneIsleStackedPillars
Field_CrossboneIsleStackedPillars:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleStackedPillars, .LField_CrossboneIsleStackedPillars_CELL_PICTURES
	map_offset Field_CrossboneIsleStackedPillars, .LField_CrossboneIsleStackedPillars_SHAPES
	map_offset Field_CrossboneIsleStackedPillars, .LField_CrossboneIsleStackedPillars_GRID
	map_offset Field_CrossboneIsleStackedPillars, .LField_CrossboneIsleStackedPillars_ANIMATION
	map_offset Field_CrossboneIsleStackedPillars, .LField_CrossboneIsleStackedPillars_BLEND
	map_offset Field_CrossboneIsleStackedPillars, .LField_CrossboneIsleStackedPillars_MARKERS
.LField_CrossboneIsleStackedPillars_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_stacked_pillars/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_CrossboneIsleStackedPillars_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_stacked_pillars/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleStackedPillars_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_stacked_pillars/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleStackedPillars_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleStackedPillars_BLEND:
	.incbin "games/tbs/src/map/crossbone_isle_stacked_pillars/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleStackedPillars_MARKERS:
	treasure_spot 38, 9, 102
	treasure_spot 50, 11, 101
	treasure_spot 40, 16, 103
	treasure_spot 42, 31, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette19
Tileset_Palette19:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_19.gbapal.lz"
	.byte 0xb3, 0x90, 0x80	@ PACKER: left in the buffer by Field_CrossboneIsleStackedPillars+0x185
