@ What look like two keysplit tables, unused: keys 0-35 take 0 and 36-71
@ take 1, twice over. No voice refers to them; The Broken Seal has the
@ same tables in the same place.
	.section .rodata
	.global Sound_UnusedKeySplitTables
Sound_UnusedKeySplitTables:
	.fill 36, 1, 0
	.fill 36, 1, 1
	.fill 36, 1, 0
	.fill 36, 1, 1
