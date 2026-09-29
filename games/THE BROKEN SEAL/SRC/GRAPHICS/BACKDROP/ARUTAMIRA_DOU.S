@ Battle_ArutamiraDouBackdrop: a 256x120 backdrop: its 128-colour palette, then its pixels, each a
@ delta from the one before.
	.section .rodata
	.global Battle_ArutamiraDouBackdrop
Battle_ArutamiraDouBackdrop:
	.incbin "GRAPHICS/BACKDROP/ARUTAMIRA_DOU.gbapal"
	.incbin "GRAPHICS/BACKDROP/ARUTAMIRA_DOU.bitmap.d7"
