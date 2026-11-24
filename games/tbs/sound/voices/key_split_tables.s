@ Apparent unused key-split tables: two 72-key rows, 0 for keys 0-35 and 1 for 36-71.
@ No voice references these; this edition has no key-split (0x40) voices.
@ TLA has the same tables in the corresponding position.
	.section .rodata
	.global Sound_UnusedKeySplitTables
Sound_UnusedKeySplitTables:
	.fill 36, 1, 0
	.fill 36, 1, 1
	.fill 36, 1, 0
	.fill 36, 1, 1
