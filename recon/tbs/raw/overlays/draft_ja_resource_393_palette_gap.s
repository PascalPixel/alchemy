/* NONMATCHING: Korima river's old fixed palette-bank gap, 2026-10-01.
 * After the Japanese 56-byte leader callback, the loaded image is 4288
 * bytes. This fixed gap moves its palette banks twelve bytes past their
 * aligned position and changes four whole-resource literal bytes.
 * The international image needs the same twelve bytes as align16 padding.
 */
	.section .bss,"aw",%nobits
	.space 12
	.global KorimaPalette_First
KorimaPalette_First:
	.space 1792
	.global KorimaPalette_Second
KorimaPalette_Second:
	.space 896
