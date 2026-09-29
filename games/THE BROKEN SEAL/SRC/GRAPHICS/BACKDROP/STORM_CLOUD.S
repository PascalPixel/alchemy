@ Battle_StormCloudBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_StormCloudBackdrop
Battle_StormCloudBackdrop:
	.incbin "GRAPHICS/BACKDROP/STORM_CLOUD.gbapal"
	.incbin "GRAPHICS/BACKDROP/STORM_CLOUD.bitmap.d7"
