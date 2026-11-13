	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Slots_MachinePicture
Slots_MachinePicture:
	.incbin "games/tla/src/graphics/lucky_wheels/machine.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/machine.4bpp.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x93, 0x34, 0x90	@ PACKER: left in the buffer by Ending_TheEndPicture+0x781
	.else
	.byte 0x00, 0x00, 0x00	@ PACKER: left in the buffer by Ending_TheEndPicture+0x781
	.endif
	.global Slots_Icons
Slots_Icons:
	.incbin "games/tla/src/graphics/lucky_wheels/icons.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/icons.8bpp.lz"
	.global Slots_JackpotBanner
Slots_JackpotBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot_es.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/jackpot.4bpp32x32.lz"
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0xa2	@ PACKER: left in the buffer by Slots_Icons+0x66b
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x39	@ PACKER: left in the buffer by Slots_Icons+0x64b
	.else
	.byte 0x75	@ PACKER: left in the buffer by Slots_Icons+0x61b
	.endif
	.endif
	.section .rodata.a
	.global Slots_YouWinBanner
Slots_YouWinBanner:
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_win_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/you_win.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_win.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x21	@ PACKER: left in the buffer by Resource_Data087+0x613
	.else
	.ifdef TLA_EDITION_ES
	.byte 0xa4, 0x62	@ PACKER: left in the buffer by Resource_Data087+0x61e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x17	@ PACKER: left in the buffer by Resource_Data087+0x5e7
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xa1, 0x82	@ PACKER: left in the buffer by Resource_Data087+0x582
	.else
	.byte 0x78, 0x00, 0xd2	@ PACKER: left in the buffer by Resource_Data087+0x455
	.endif
	.endif
	.endif
	.endif
	.global Slots_YouLoseBanner
Slots_YouLoseBanner:
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/you_lose.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x10	@ PACKER: left in the buffer by Slots_YouWinBanner+0x41f
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x90	@ PACKER: left in the buffer by Slots_YouWinBanner+0x577
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xcd, 0xe4, 0x07	@ PACKER: left in the buffer by Slots_YouWinBanner+0x4d5
	.endif
	.endif
	.endif
	.global Slots_HigherBanner
Slots_HigherBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/higher_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/higher_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/higher_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/higher_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/higher_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/higher_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/higher_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/higher_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/higher_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/higher_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/higher.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/higher.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0xad, 0xcb, 0x71	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x2ad
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x34, 0x2f	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x27e
	.else
	.ifdef TLA_EDITION_FR
	.byte 0xd8, 0xdd, 0x51	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x299
	.endif
	.endif
	.endif
	.global Slots_LowerBanner
Slots_LowerBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/lower_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/lower_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/lower_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/lower_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/lower_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/lower_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/lower_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/lower_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/lower_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/lower_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/lower.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/lower.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x13	@ PACKER: left in the buffer by Slots_HigherBanner+0x23b
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x12, 0x40	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x90, 0xb8, 0x94	@ PACKER: left in the buffer by Slots_HigherBanner+0x2bd
	.else
	.ifdef TLA_EDITION_FR
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x3a	@ PACKER: left in the buffer by Slots_HigherBanner+0x257
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Slots_DoubleUpBanner
Slots_DoubleUpBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/double_up.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/double_up.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x04, 0x00, 0x00	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.else
	.ifdef TLA_EDITION_ES
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xbe	@ PACKER: left in the buffer by Slots_YouLoseBanner+0x48f
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Slots_CashOutBanner
Slots_CashOutBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_ja.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_de.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_es.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_fr.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out_it.4bpp32x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/cash_out.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x9f, 0x7a, 0xe0	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x305
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x0f	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x48f
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x80, 0x4e	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x3a2
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x01, 0x00, 0x00	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.else
	.ifdef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Slots_PinkDigits
Slots_PinkDigits:
	.incbin "games/tla/src/graphics/lucky_wheels/pink_digits.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/pink_digits.4bpp16x16.lz"
	.ifdef TLA_EDITION_EN
	.byte 0xd9, 0x20, 0xbe	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x97, 0x23, 0x59	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.ifdef TLA_EDITION_ES
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x19, 0x07, 0x38	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x00, 0x0c, 0xe0	@ PACKER: left in the buffer by Slots_CashOutBanner+0x36d
	.else
	.byte 0xae, 0xf0, 0x1c	@ PACKER: left in the buffer by Slots_DoubleUpBanner+0x36d
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Slots_OrangeDigits
Slots_OrangeDigits:
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits_es.4bpp16x32.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/orange_digits.4bpp16x32.lz"
	.endif
	.ifdef TLA_EDITION_ES
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.global Slots_GreenDigits
Slots_GreenDigits:
	.incbin "games/tla/src/graphics/lucky_wheels/green_digits.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/green_digits.4bpp32x32.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Resource_Data087+0x733
	.global Slots_TimesSign
Slots_TimesSign:
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign_it.4bpp16x16.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/times_sign.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x26	@ PACKER: left in the buffer by Slots_GreenDigits+0x10b
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x00, 0x81, 0xc2	@ PACKER: left in the buffer by Slots_GreenDigits+0xc9
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x45, 0x8a	@ PACKER: left in the buffer by Slots_GreenDigits+0xf2
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xd3	@ PACKER: left in the buffer by Slots_GreenDigits+0xbf
	.else
	.byte 0x91, 0x4c, 0x7b	@ PACKER: left in the buffer by Slots_GreenDigits+0xdd
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Slots_SmallDoubleUpBanner
Slots_SmallDoubleUpBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_ja.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up_it.4bpp16x16.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/small_double_up.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x5e, 0xe3, 0x09	@ PACKER: left in the buffer by Slots_GreenDigits+0x19d
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x1d, 0x98	@ PACKER: left in the buffer by Slots_GreenDigits+0x162
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x40	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.endif
	.endif
	.endif
	.endif
	.global Slots_OnePairBanner
Slots_OnePairBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/one_pair.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x34, 0x5e, 0x57	@ PACKER: left in the buffer by Slots_GreenDigits+0x2f9
	.else
	.ifdef TLA_EDITION_EN
	.byte 0xb2, 0x32, 0xe8	@ PACKER: left in the buffer by Slots_GreenDigits+0x3c9
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x63, 0x27	@ PACKER: left in the buffer by Slots_GreenDigits+0x46a
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x67, 0xb6	@ PACKER: left in the buffer by Slots_GreenDigits+0x4de
	.endif
	.endif
	.endif
	.endif
	.global Slots_TwoPairBanner
Slots_TwoPairBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/two_pair.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x04, 0xa6, 0xc6	@ PACKER: left in the buffer by Slots_GreenDigits+0x579
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x4c, 0x01, 0x8f	@ PACKER: left in the buffer by Slots_GreenDigits+0x6e5
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x04	@ PACKER: left in the buffer by Slots_GreenDigits+0x5e3
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x03, 0x00, 0x00	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Slots_TripleBanner
Slots_TripleBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/triple_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/triple_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/triple_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/triple_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/triple_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/triple.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/triple.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0x7c, 0x2e, 0xbc	@ PACKER: left in the buffer by Slots_GreenDigits+0x3e1
	.else
	.ifdef TLA_EDITION_EN
	.byte 0x1d, 0xe9	@ PACKER: left in the buffer by Slots_TwoPairBanner+0x396
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x44	@ PACKER: left in the buffer by Slots_TwoPairBanner+0x393
	.endif
	.endif
	.endif
	.global Slots_PerfectBanner
Slots_PerfectBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_ja.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_de.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_es.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_fr.4bpp64x64.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/perfect_it.4bpp64x64.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/perfect.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/perfect.4bpp64x64.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_EN
	.byte 0xc3	@ PACKER: left in the buffer by Slots_GreenDigits+0x663
	.else
	.ifdef TLA_EDITION_DE
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x02	@ PACKER: left in the buffer by Resource_Data087+0x7ab
	.else
	.ifdef TLA_EDITION_IT
	.byte 0xe2, 0x3c, 0x30	@ PACKER: left in the buffer by Slots_GreenDigits+0x64d
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Slots_FinalGameBanner
Slots_FinalGameBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_ja.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_ja.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_de.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_de.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_es.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_es.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_fr.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_fr.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_it.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game_it.4bpp16x16.lz"
	.else
	.incbin "games/tla/src/graphics/lucky_wheels/final_game.gbapal"
	.incbin "games/tla/src/graphics/lucky_wheels/final_game.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_JA
	.byte 0xf0, 0x71	@ PACKER: left in the buffer by Slots_PerfectBanner+0x272
	.else
	.ifdef TLA_EDITION_DE
	.byte 0x27, 0x70, 0x24	@ PACKER: left in the buffer by Slots_PerfectBanner+0x201
	.else
	.ifdef TLA_EDITION_ES
	.byte 0x1e	@ PACKER: left in the buffer by Slots_PerfectBanner+0x237
	.else
	.ifdef TLA_EDITION_IT
	.byte 0x74	@ PACKER: left in the buffer by Slots_PerfectBanner+0x23f
	.endif
	.endif
	.endif
	.endif
