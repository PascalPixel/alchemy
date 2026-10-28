	.section .rodata
	.global gBattleObject_EmptyScript
gBattleObject_EmptyScript:
	.4byte 16
@ A weapon's item id | its BattleWeaponKind << 9 (sprite_slots.c), terminated by 0xffff.
	.global gBattleUnit_WeaponKinds
gBattleUnit_WeaponKinds:
	.2byte 1, 2, 3, 4, 5, 6, 7, 8
	.2byte 9, 10, 11, 15 | 1 << 9, 16 | 1 << 9, 17 | 1 << 9, 18 | 1 << 9, 19 | 1 << 9
	.2byte 20 | 1 << 9, 21 | 1 << 9, 22 | 1 << 9, 23 | 1 << 9, 24 | 1 << 9, 25 | 1 << 9, 26 | 1 << 9, 27 | 1 << 9
	.2byte 31 | 2 << 9, 32 | 2 << 9, 33 | 2 << 9, 34 | 2 << 9, 35 | 2 << 9, 36 | 2 << 9, 37 | 2 << 9, 38 | 2 << 9
	.2byte 43 | 3 << 9, 44 | 3 << 9, 45 | 3 << 9, 46 | 3 << 9, 47 | 3 << 9, 48 | 3 << 9, 49 | 3 << 9, 50 | 3 << 9
	.2byte 55 | 4 << 9, 56 | 4 << 9, 57 | 4 << 9, 58 | 5 << 9, 59 | 4 << 9, 60 | 4 << 9, 61 | 5 << 9, 62 | 4 << 9
	.2byte 63 | 4 << 9, 64 | 4 << 9, 65 | 4 << 9, 0xffff
	.global gBattleKnockback_Acceleration
gBattleKnockback_Acceleration:
	.4byte 1 << 16, 1 << 16, 1 << 16, 1 << 16, 8 << 16, 1 << 16, 1 << 16, 1 << 16
	.global gBattleKnockback_SpeedLimit
gBattleKnockback_SpeedLimit:
	.4byte 4 << 16, 4 << 16, 4 << 16, 4 << 16, 20 << 16, 4 << 16, 4 << 16, 4 << 16
	.global gBattleKnockback_VelocityY
gBattleKnockback_VelocityY:
	.4byte 3 << 16, 6 << 16, 7 << 16, 10 << 16, 5 << 16, 2 << 16, 3 << 16, 0x58000
	.global gBattleKnockback_DistancePercent
gBattleKnockback_DistancePercent:
	.4byte 140, 140, 150, 200, 200, 150, 100, 100
	.4byte 0, 20, 27
	.global gBattleTransition_CurtainVariants
gBattleTransition_CurtainVariants:
	.incbin "games/tbs/src/battle/transition/curtain_variants.4bpp"
	.global gBattleTransition_CurtainTiles
gBattleTransition_CurtainTiles:
	.incbin "games/tbs/src/battle/transition/curtain.4bpp"
@ BLDY values, the same in both games.
	.global gBattleHighlight_BlendRamp
gBattleHighlight_BlendRamp:
	.byte 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
	.byte 4, 6, 7, 8, 8, 8, 8, 8, 4, 2, 1, 0, 0, 0, 0, 0
	.4byte 1, 27
