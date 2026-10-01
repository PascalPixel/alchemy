/* NONMATCHING: localized scene data, 2026-10-01.
 * The original work bank reserves a fixed four-byte prefix.
 * Its named records require sixteen-byte bank alignment across the editions:
 * EN/DE keep four bytes, Japanese needs twelve, ES/FR/IT need none.
 */
	.section .bss,"aw",%nobits
	.space 4
	.global TakaraHashira_PillarSlots
TakaraHashira_PillarSlots:
	.space 80
	.space 12
	.global TakaraHashira_ShakenScroll
TakaraHashira_ShakenScroll:
	.space 12
	.global TakaraHashira_ShakeChance
TakaraHashira_ShakeChance:
	.space 4
