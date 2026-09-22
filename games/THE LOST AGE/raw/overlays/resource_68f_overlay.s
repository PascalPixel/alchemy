.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000400, 0x02000400
	.set sub_020006c4, 0x020006c4
	.set sub_020006d0, 0x020006d0
	.set sub_02001016, 0x02001016
	.set sub_0200168e, 0x0200168e
	.set sub_0200169e, 0x0200169e
	.set sub_020016ac, 0x020016ac
	.set sub_020030a4, 0x020030a4
	.set sub_020030a6, 0x020030a6
	.set sub_020030c8, 0x020030c8
	.set sub_020030d4, 0x020030d4
	.set sub_020030e8, 0x020030e8
	.set sub_020030f6, 0x020030f6
	.set sub_02003126, 0x02003126
	.set sub_02003138, 0x02003138
	.set sub_0200313a, 0x0200313a
	.set sub_02003196, 0x02003196
	.set sub_020031a4, 0x020031a4
	.set sub_020031bc, 0x020031bc
	.set sub_020031fa, 0x020031fa
	.set sub_02003246, 0x02003246
	.set sub_020032f0, 0x020032f0
	.set sub_02003352, 0x02003352
	.set sub_02003382, 0x02003382
	.set sub_020033a0, 0x020033a0
	.set sub_020033c6, 0x020033c6
	.set sub_020033d8, 0x020033d8
	.set sub_0200341e, 0x0200341e
	.set sub_02003422, 0x02003422
	.set sub_02003474, 0x02003474
	.set sub_020034f0, 0x020034f0
	.set sub_02003534, 0x02003534
	.set sub_02003536, 0x02003536
	.set sub_0200354a, 0x0200354a
	.set sub_0200357a, 0x0200357a
	.set sub_0200358e, 0x0200358e
	.set sub_02003596, 0x02003596
	.set sub_020035d4, 0x020035d4
	.set sub_020035da, 0x020035da
	.set sub_020035ec, 0x020035ec
	.set sub_02003618, 0x02003618
	.set sub_02003628, 0x02003628
	.set sub_0200364e, 0x0200364e
	.set sub_0200365a, 0x0200365a
	.set sub_02003672, 0x02003672
	.set sub_02003674, 0x02003674
	.set sub_0200367e, 0x0200367e
	.set sub_020036a0, 0x020036a0
	.set sub_020036a6, 0x020036a6
	.set sub_020036a8, 0x020036a8
	.set sub_020036b2, 0x020036b2
	.set sub_020036c0, 0x020036c0
	.set sub_020036de, 0x020036de
	.set sub_020036e6, 0x020036e6
	.set sub_020036ea, 0x020036ea
	.set sub_020036f2, 0x020036f2
	.set sub_02003704, 0x02003704
	.set sub_02003714, 0x02003714
	.set sub_0200372e, 0x0200372e
	.set sub_02003738, 0x02003738
	.set sub_02003760, 0x02003760
	.set sub_02003768, 0x02003768
	.set sub_020037a6, 0x020037a6
	.set sub_020037a8, 0x020037a8
	.set sub_020037d6, 0x020037d6
	.set sub_020037dc, 0x020037dc
	.set sub_020037e2, 0x020037e2
	.set sub_02003828, 0x02003828
	.set sub_02003836, 0x02003836
	.set sub_020038a4, 0x020038a4
	.set sub_020038c2, 0x020038c2
	.set sub_020038fe, 0x020038fe
	.set sub_02003908, 0x02003908
	.set sub_02003964, 0x02003964
	.set sub_0200396a, 0x0200396a
	.set sub_0200396c, 0x0200396c
	.set sub_0200396e, 0x0200396e
	.set sub_02003980, 0x02003980
	.set sub_0200398e, 0x0200398e
	.set sub_02003998, 0x02003998
	.set sub_020039a4, 0x020039a4
	.set sub_020039a8, 0x020039a8
	.set sub_020039b8, 0x020039b8
	.set sub_020039ba, 0x020039ba
	.set sub_020039c2, 0x020039c2
	.set sub_02003a00, 0x02003a00
	.set sub_02003a04, 0x02003a04
	.set sub_02003a50, 0x02003a50
	.set sub_02003a62, 0x02003a62
	.set sub_02003a72, 0x02003a72
	.set sub_02003a76, 0x02003a76
	.set sub_02003a8c, 0x02003a8c
	.set sub_02003a90, 0x02003a90
	.set sub_02003a94, 0x02003a94
	.set sub_02003aaa, 0x02003aaa
	.set sub_02003abe, 0x02003abe
	.set sub_02003ac0, 0x02003ac0
	.set sub_02003ad2, 0x02003ad2
	.set sub_02003ad8, 0x02003ad8
	.set sub_02003ae8, 0x02003ae8
	.set sub_02003b40, 0x02003b40
	.set sub_02003b54, 0x02003b54
	.set sub_02003b68, 0x02003b68
	.set sub_02003b6a, 0x02003b6a
	.set sub_02003b9c, 0x02003b9c
	.set sub_02003ba6, 0x02003ba6
	.set sub_02003bc6, 0x02003bc6
	.set sub_02003bc8, 0x02003bc8
	.set sub_02003bca, 0x02003bca
	.set sub_02003bd6, 0x02003bd6
	.set sub_02003bee, 0x02003bee
	.set sub_02003c0a, 0x02003c0a
	.set sub_02003c0c, 0x02003c0c
	.set sub_02003c28, 0x02003c28
	.set sub_02003c3a, 0x02003c3a
	.set sub_02003c40, 0x02003c40
	.set sub_02003c4c, 0x02003c4c
	.set sub_02003c50, 0x02003c50
	.set sub_02003c54, 0x02003c54
	.set sub_02003c64, 0x02003c64
	.set sub_02003c68, 0x02003c68
	.set sub_02003c6e, 0x02003c6e
	.set sub_02003c88, 0x02003c88
	.set sub_02003ca2, 0x02003ca2
	.set sub_02003cca, 0x02003cca
	.set sub_02003cd4, 0x02003cd4
	.set sub_02003d1e, 0x02003d1e
	.set sub_02003d28, 0x02003d28
	.set sub_02003d2c, 0x02003d2c
	.set sub_02003d3e, 0x02003d3e
	.set sub_02003d5a, 0x02003d5a
	.set sub_02003d6c, 0x02003d6c
	.set sub_02003d70, 0x02003d70
	.set sub_02003d74, 0x02003d74
	.set sub_02003d8a, 0x02003d8a
	.set sub_02003d8c, 0x02003d8c
	.set sub_02003da2, 0x02003da2
	.set sub_02003db2, 0x02003db2
	.set sub_02003dc6, 0x02003dc6
	.set sub_02003dd0, 0x02003dd0
	.set sub_02003ddc, 0x02003ddc
	.set sub_02003df8, 0x02003df8
	.set sub_02003e18, 0x02003e18
	.set sub_02003e22, 0x02003e22
	.set sub_02003e4e, 0x02003e4e
	.set sub_02003e70, 0x02003e70
	.set sub_02003e92, 0x02003e92
	.set sub_02003e94, 0x02003e94
	.set sub_02003e98, 0x02003e98
	.set sub_02003ea2, 0x02003ea2
	.set sub_02003eac, 0x02003eac
	.set sub_02003ecc, 0x02003ecc
	.set sub_02003ed6, 0x02003ed6
	.set sub_02003ee8, 0x02003ee8
	.set sub_02003ef6, 0x02003ef6
	.set sub_02003f02, 0x02003f02
	.set sub_02003f08, 0x02003f08
	.set sub_02003f20, 0x02003f20
	.set sub_02003f28, 0x02003f28
	.set sub_02003f44, 0x02003f44
	.set sub_02003f70, 0x02003f70
	.set sub_02003fa2, 0x02003fa2
	.set sub_02003fde, 0x02003fde
	.set sub_0200404e, 0x0200404e
	.set sub_02004092, 0x02004092
	.set sub_020040ae, 0x020040ae
	.set sub_020040fa, 0x020040fa
	.set sub_0200410e, 0x0200410e
	.set sub_02004140, 0x02004140
	.set sub_020041a2, 0x020041a2
	.set sub_020041a6, 0x020041a6
	.set sub_020041a8, 0x020041a8
	.set sub_020041bc, 0x020041bc
	.set sub_020041c0, 0x020041c0
	.set sub_02004244, 0x02004244
	.set sub_0200424e, 0x0200424e
	.set sub_0200426e, 0x0200426e
	.set sub_0200429e, 0x0200429e
	.set sub_020042e2, 0x020042e2
	.set sub_020042f4, 0x020042f4
	.set sub_0200431a, 0x0200431a
	.set sub_0200432c, 0x0200432c
	.set sub_02004338, 0x02004338
	.set sub_0200434a, 0x0200434a
	.set sub_0200436a, 0x0200436a
	.set sub_02004372, 0x02004372
	.set sub_0200437e, 0x0200437e
	.set sub_02004384, 0x02004384
	.set sub_0200439a, 0x0200439a
	.set sub_020043a2, 0x020043a2
	.set sub_020043aa, 0x020043aa
	.set sub_020043ca, 0x020043ca
	.set sub_020043da, 0x020043da
	.set sub_020043e2, 0x020043e2
	.set sub_020043fc, 0x020043fc
	.set sub_02004402, 0x02004402
	.set sub_02004436, 0x02004436
	.set sub_0200443c, 0x0200443c
	.set sub_0200444c, 0x0200444c
	.set sub_02004488, 0x02004488
	.set sub_02004492, 0x02004492
	.set sub_02004496, 0x02004496
	.set sub_02004498, 0x02004498
	.set sub_0200449e, 0x0200449e
	.set sub_020044a8, 0x020044a8
	.set sub_020044d8, 0x020044d8
	.set sub_020044e4, 0x020044e4
	.set sub_02004502, 0x02004502
	.set sub_0200450c, 0x0200450c
	.set sub_02004536, 0x02004536
	.set sub_0200454a, 0x0200454a
	.set sub_0200458e, 0x0200458e
	.set sub_020045d0, 0x020045d0
	.set sub_020046b4, 0x020046b4
	.set sub_020046ea, 0x020046ea
	.set sub_020046ec, 0x020046ec
	.set sub_020046ee, 0x020046ee
	.set sub_0200475c, 0x0200475c
	.set sub_02004768, 0x02004768
	.set sub_02004798, 0x02004798
	.set sub_020047a8, 0x020047a8
	.set sub_020047d0, 0x020047d0
	.set sub_020047e0, 0x020047e0
	.set sub_020047f6, 0x020047f6
	.set sub_0200480e, 0x0200480e
	.set sub_02004892, 0x02004892
	.set sub_020048d2, 0x020048d2
	.set sub_020048f2, 0x020048f2
	.set sub_0200492a, 0x0200492a
	.set sub_0200498a, 0x0200498a
	.set sub_0200498c, 0x0200498c
	.set sub_02004998, 0x02004998
	.set sub_0200499c, 0x0200499c
	.set sub_020049a4, 0x020049a4
	.set sub_020049b4, 0x020049b4
	.set sub_020049ce, 0x020049ce
	.set sub_020049d2, 0x020049d2
	.set sub_020049f0, 0x020049f0
	.set sub_020049fe, 0x020049fe
	.set sub_02004a06, 0x02004a06
	.set sub_02004a18, 0x02004a18
	.set sub_02004a6e, 0x02004a6e
	.set sub_02004a7c, 0x02004a7c
	.set sub_02004ab0, 0x02004ab0
	.set sub_02004ab2, 0x02004ab2
	.set sub_02004ab4, 0x02004ab4
	.set sub_02004ac0, 0x02004ac0
	.set sub_02004ac4, 0x02004ac4
	.set sub_02004b4e, 0x02004b4e
	.set sub_02004b8e, 0x02004b8e
	.set sub_02004bb6, 0x02004bb6
	.set sub_02004bea, 0x02004bea
	.set sub_02004bec, 0x02004bec
	.set sub_02004bf2, 0x02004bf2
	.set sub_02004bf8, 0x02004bf8
	.set sub_02004c48, 0x02004c48
	.set sub_02004c54, 0x02004c54
	.set sub_02004c58, 0x02004c58
	.set sub_02004c60, 0x02004c60
	.set sub_02004c70, 0x02004c70
	.set sub_02004c9c, 0x02004c9c
	.set sub_02004c9e, 0x02004c9e
	.set sub_02004cf4, 0x02004cf4
	.set sub_02004d0c, 0x02004d0c
	.set sub_02004d14, 0x02004d14
	.set sub_02004d1e, 0x02004d1e
	.set sub_02004d56, 0x02004d56
	.set sub_02004d96, 0x02004d96
	.set sub_02004dc0, 0x02004dc0
	.set sub_02004dc2, 0x02004dc2
	.set sub_02004dce, 0x02004dce
	.set sub_02004dd2, 0x02004dd2
	.set sub_02004ddc, 0x02004ddc
	.set sub_02004e12, 0x02004e12
	.set sub_02004e32, 0x02004e32
	.set sub_02004e36, 0x02004e36
	.set sub_02004ed6, 0x02004ed6
	.set sub_02004edc, 0x02004edc
	.set sub_02004ee6, 0x02004ee6
	.set sub_02004ef6, 0x02004ef6
	.set sub_02004f00, 0x02004f00
	.set sub_02004f0e, 0x02004f0e
	.set sub_02004f2a, 0x02004f2a
	.set sub_02004f32, 0x02004f32
	.set sub_02004f4c, 0x02004f4c
	.set sub_02004f54, 0x02004f54
	.set sub_02004f66, 0x02004f66
	.set sub_02004f96, 0x02004f96
	.set sub_02004fbc, 0x02004fbc
	.set sub_02004fc4, 0x02004fc4
	.set sub_02004fca, 0x02004fca
	.set sub_02004ffe, 0x02004ffe
	.set sub_02005000, 0x02005000
	.set sub_02005002, 0x02005002
	.set sub_0200500e, 0x0200500e
	.set sub_02005012, 0x02005012
	.set sub_020050a0, 0x020050a0
	.set sub_020050be, 0x020050be
	.set sub_020050c0, 0x020050c0
	.set sub_02005108, 0x02005108
	.set sub_0200510a, 0x0200510a
	.set sub_0200511c, 0x0200511c
	.set sub_0200511e, 0x0200511e
	.set sub_02005160, 0x02005160
	.set sub_0200516a, 0x0200516a
	.set sub_02005174, 0x02005174
	.set sub_02005178, 0x02005178
	.set sub_02005182, 0x02005182
	.set sub_02005190, 0x02005190
	.set sub_0200519a, 0x0200519a
	.set sub_020051a4, 0x020051a4
	.set sub_020051ea, 0x020051ea
	.set sub_020051f6, 0x020051f6
	.set sub_02005216, 0x02005216
	.set sub_0200525a, 0x0200525a
	.set sub_02005272, 0x02005272
	.set sub_0200527e, 0x0200527e
	.set sub_02005298, 0x02005298
	.set sub_020052a0, 0x020052a0
	.set sub_020052b4, 0x020052b4
	.set sub_020052be, 0x020052be
	.set sub_020052ca, 0x020052ca
	.set sub_020052d8, 0x020052d8
	.set sub_020052ea, 0x020052ea
	.set sub_02005306, 0x02005306
	.set sub_0200530c, 0x0200530c
	.set sub_0200531a, 0x0200531a
	.set sub_02005324, 0x02005324
	.set sub_0200533c, 0x0200533c
	.set sub_02005356, 0x02005356
	.set sub_0200535c, 0x0200535c
	.set sub_0200536a, 0x0200536a
	.set sub_02005376, 0x02005376
	.set sub_02005382, 0x02005382
	.set sub_02005386, 0x02005386
	.set sub_0200538c, 0x0200538c
	.set sub_02005390, 0x02005390
	.set sub_0200539e, 0x0200539e
	.set sub_020053ae, 0x020053ae
	.set sub_020053ce, 0x020053ce
	.set sub_020053d8, 0x020053d8
	.set sub_020053f4, 0x020053f4
	.set sub_020053f6, 0x020053f6
	.set sub_02005404, 0x02005404
	.set sub_0200541a, 0x0200541a
	.set sub_02005420, 0x02005420
	.set sub_02005434, 0x02005434
	.set sub_0200543c, 0x0200543c
	.set sub_02005446, 0x02005446
	.set sub_02005448, 0x02005448
	.set sub_02005458, 0x02005458
	.set sub_0200545a, 0x0200545a
	.set sub_02005466, 0x02005466
	.set sub_02005470, 0x02005470
	.set sub_02005478, 0x02005478
	.set sub_02005482, 0x02005482
	.set sub_02005486, 0x02005486
	.set sub_0200548c, 0x0200548c
	.set sub_02005492, 0x02005492
	.set sub_020054a4, 0x020054a4
	.set sub_020054b6, 0x020054b6
	.set sub_020054cc, 0x020054cc
	.set sub_0200550e, 0x0200550e
	.set sub_02005514, 0x02005514
	.set sub_02005518, 0x02005518
	.set sub_0200551e, 0x0200551e
	.set sub_0200552c, 0x0200552c
	.set sub_0200552e, 0x0200552e
	.set sub_02005538, 0x02005538
	.set sub_0200554e, 0x0200554e
	.set sub_02005552, 0x02005552
	.set sub_0200555c, 0x0200555c
	.set sub_02005564, 0x02005564
	.set sub_0200556a, 0x0200556a
	.set sub_0200557a, 0x0200557a
	.set sub_0200557e, 0x0200557e
	.set sub_020055d4, 0x020055d4
	.set sub_02005602, 0x02005602
	.set sub_02005612, 0x02005612
	.set sub_0200561c, 0x0200561c
	.set sub_02005626, 0x02005626
	.set sub_0200565c, 0x0200565c
	.set sub_02005670, 0x02005670
	.set sub_02005698, 0x02005698
	.set sub_0200569a, 0x0200569a
	.set sub_020056a0, 0x020056a0
	.set sub_020056a8, 0x020056a8
	.set sub_020056ba, 0x020056ba
	.set sub_020056c8, 0x020056c8
	.set sub_020056d0, 0x020056d0
	.set sub_020056d8, 0x020056d8
	.set sub_020056da, 0x020056da
	.set sub_020056e4, 0x020056e4
	.set sub_020056e6, 0x020056e6
	.set sub_020056f0, 0x020056f0
	.set sub_020056f4, 0x020056f4
	.set sub_02005700, 0x02005700
	.set sub_02005712, 0x02005712
	.set sub_02005724, 0x02005724
	.set sub_02005736, 0x02005736
	.set sub_02005740, 0x02005740
	.set sub_02005744, 0x02005744
	.set sub_0200575a, 0x0200575a
	.set sub_02005774, 0x02005774
	.set sub_02005788, 0x02005788
	.set sub_0200579e, 0x0200579e
	.set sub_020057aa, 0x020057aa
	.set sub_020057be, 0x020057be
	.set sub_020057ca, 0x020057ca
	.set sub_020057cc, 0x020057cc
	.set sub_020057ce, 0x020057ce
	.set sub_020057e0, 0x020057e0
	.set sub_020057e4, 0x020057e4
	.set sub_020057e8, 0x020057e8
	.set sub_020057f0, 0x020057f0
	.set sub_020057f4, 0x020057f4
	.set sub_02005802, 0x02005802
	.set sub_02005810, 0x02005810
	.set sub_02005812, 0x02005812
	.set sub_02005820, 0x02005820
	.set sub_0200583a, 0x0200583a
	.set sub_0200584c, 0x0200584c
	.set sub_0200585a, 0x0200585a
	.set sub_0200586c, 0x0200586c
	.set sub_0200586e, 0x0200586e
	.set sub_0200587e, 0x0200587e
	.set sub_02005880, 0x02005880
	.set sub_02005892, 0x02005892
	.set sub_02005898, 0x02005898
	.set sub_0200589c, 0x0200589c
	.set sub_020058aa, 0x020058aa
	.set sub_020058ac, 0x020058ac
	.set sub_020058cc, 0x020058cc
	.set sub_020058d2, 0x020058d2
	.set sub_020058de, 0x020058de
	.set sub_020058e0, 0x020058e0
	.set sub_020058ee, 0x020058ee
	.set sub_020058f0, 0x020058f0
	.set sub_020058fc, 0x020058fc
	.set sub_0200592e, 0x0200592e
	.set sub_0200597c, 0x0200597c
	.set sub_02005996, 0x02005996
	.set sub_0200599c, 0x0200599c
	.set sub_020059a0, 0x020059a0
	.set sub_020059a4, 0x020059a4
	.set sub_020059b2, 0x020059b2
	.set sub_020059b4, 0x020059b4
	.set sub_020059c8, 0x020059c8
	.set sub_020059e0, 0x020059e0
	.set sub_02005a08, 0x02005a08
	.set sub_02005a30, 0x02005a30
	.set sub_02005a34, 0x02005a34
	.set sub_02005a40, 0x02005a40
	.set sub_02005a96, 0x02005a96
	.set sub_02005a98, 0x02005a98
	.set sub_02005a9a, 0x02005a9a
	.set sub_02005aae, 0x02005aae
	.set sub_02005ac0, 0x02005ac0
	.set sub_02005ac4, 0x02005ac4
	.set sub_02005acc, 0x02005acc
	.set sub_02005ae4, 0x02005ae4
	.set sub_02005afc, 0x02005afc
	.set sub_02005b4c, 0x02005b4c
	.set sub_02005b72, 0x02005b72
	.set sub_02005b7a, 0x02005b7a
	.set sub_02005c3e, 0x02005c3e
	.set sub_02005c6a, 0x02005c6a
	.set sub_02005c76, 0x02005c76
	.set sub_02005c84, 0x02005c84
	.set sub_02005c92, 0x02005c92
	.set sub_02005caa, 0x02005caa
	.set sub_02005ccc, 0x02005ccc
	.set sub_02005cda, 0x02005cda
	.set sub_02005d04, 0x02005d04
	.set sub_02005d44, 0x02005d44
	.set sub_02005d62, 0x02005d62
	.set sub_02005d7c, 0x02005d7c
	.set sub_02005d7e, 0x02005d7e
	.set sub_02005d82, 0x02005d82
	.set sub_02005da0, 0x02005da0
	.set sub_02005dca, 0x02005dca
	.set sub_02005e10, 0x02005e10
	.set sub_02005e18, 0x02005e18
	.set sub_02005e22, 0x02005e22
	.set sub_02005ea4, 0x02005ea4
	.set sub_02005eb6, 0x02005eb6
	.set sub_02005ff8, 0x02005ff8
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020093b9, 0x02008481, 0x020084c1, 0x020084c9, 0x02009045, 0x02008489, 0x020097d9
	overlay_veneer \EntryTarget
	.endr
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_0200007c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl	sub_0200313a
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000100
	cmp	r7, #0
	beq.n	.L_02000100
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000108
.L_02000100:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000108:
	mov	r3, sl
	bl	sub_020030a4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000116
	b.n	.L_02000262
.L_02000116:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl	sub_020030a6
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl	sub_020030c8
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_02003138
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl	.L_020001a0
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000262
	cmp	r7, #0
	beq.n	.L_02000262
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000198
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl	sub_02003246
.L_02000198:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
.L_020001a0:
	cmp	r3, #0
	beq.n	.L_020001b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl	.L_020001ee
.L_020001b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020001cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020001cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000212
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020001fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl	sub_020030d4
.L_020001ee:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200020c
.L_020001fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl	sub_020030e8
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl	sub_020030f6
	str	r0, [r6, #52]
.L_02000212:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200022e
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_020031a4
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl	sub_020031bc
.L_0200022e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000240
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000240:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000252
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000252:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000262
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000262:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200b334
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_020031fa
	movs	r0, #8
	movs	r1, #52
	bl	sub_02003382
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #60
.L_0200029e:
	cmp	r5, #0
	beq.n	.L_020002b0
	movs	r0, #1
	bl	sub_02003196
	ldr	r3, [r6, #40]
	subs	r5, #1
	cmp	r3, #0
	bne.n	.L_0200029e
.L_020002b0:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_020002c4
	movs	r0, #0
	b.n	.L_020002ea
.L_020002c4:
	cmp	r0, #2
	bhi.n	.L_020002d8
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_020002da
.L_020002d8:
	ldr	r4, [pc, #16]
.L_020002da:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
.L_020002ea:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000304
	movs	r0, #0
	b.n	.L_02000330
.L_02000304:
	cmp	r0, #2
	bhi.n	.L_02000318
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_0200031a
.L_02000318:
	ldr	r4, [pc, #24]
.L_0200031a:
	lsls	r3, r2, #7
	adds	r3, r5, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
	asrs	r3, r1, #8
	strb	r3, [r4, #2]
	strb	r1, [r4, #3]
.L_02000330:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #0
	asrs	r3, r3, #20
	mov	r9, r3
	ldr	r3, [r5, #16]
	mov	r1, r9
	asrs	r3, r3, #20
	mov	sl, r3
	mov	r2, sl
	bl	.L_02000610
	mov	r1, r9
	mov	r2, sl
	mov	r8, r0
	movs	r0, #2
	bl	.L_0200061c
	movs	r2, #34
	adds	r2, r2, r5
	adds	r6, r0, #0
	mov	fp, r2
	ldrb	r0, [r2, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl	sub_02003352
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r7, r0, #19
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_020032f0
	cmp	r0, #0
	beq.n	.L_020003b0
	ldr	r3, [pc, #112]
	mov	r2, r8
	ands	r6, r3
	movs	r3, #129
	negs	r3, r3
	ands	r2, r3
	ldr	r3, [r5, #20]
	mov	r8, r2
	asrs	r3, r3, #19
	cmp	r3, r7
	beq.n	.L_020003a6
	subs	r7, #4
.L_020003a6:
	adds	r0, r5, #0
	movs	r1, #3
	bl	.L_020003e4
	b.n	.L_020003ca
.L_020003b0:
	movs	r3, #255
	ands	r6, r3
	lsls	r3, r3, #8
	mov	r2, r8
	orrs	r6, r3
	movs	r3, #128
	orrs	r2, r3
	adds	r0, r5, #0
	movs	r1, #2
	mov	r8, r2
	adds	r7, #4
	bl	sub_02000400
.L_020003ca:
	mov	r1, r9
	mov	r2, sl
	mov	r3, r8
	movs	r0, #0
	bl	sub_020006c4
	mov	r1, r9
	mov	r2, sl
	adds	r3, r6, #0
	movs	r0, #2
	bl	sub_020006d0
	mov	r3, r9
.L_020003e4:
	mov	r2, sl
	lsls	r0, r3, #20
	mov	r3, fp
	lsls	r1, r2, #20
	ldrb	r2, [r3, #0]
	adds	r3, r7, #0
	bl	sub_02003422
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02000478
	adds	r7, r0, #0
.L_02000418:
	ldrh	r0, [r7, #0]
	bl	sub_02003474
	movs	r3, #4
	ldrsh	r2, [r7, r3]
	movs	r1, #0
	mov	r8, r2
	adds	r6, r0, #0
	movs	r3, #2
	ldrsh	r5, [r7, r3]
	bl	sub_0200341e
	mov	r2, r8
	lsls	r0, r2, #16
	lsrs	r0, r0, #16
	bl	sub_020033a0
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r5, r5, r0
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_020033c6
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r6, #0
	adds	r3, #100
	mov	r2, r8
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	adds	r7, #6
	bl	.L_020007a2
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02000418
.L_02000478:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb3a0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020004a0
	ldr	r0, [pc, #20]
	b.n	.L_020004aa
.L_020004a0:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_020004aa
	ldr	r0, [pc, #16]
.L_020004aa:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000d3
	.4byte 0x0200b3d0
	.4byte 0x000000d4
	.2byte 0xb3f0
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb410
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #72]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	beq.n	.L_0200050e
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020004e6
	ldr	r0, [pc, #60]
	b.n	.L_02000510
.L_020004e6:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020004f0
	ldr	r0, [pc, #56]
	b.n	.L_02000510
.L_020004f0:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020004fa
	ldr	r0, [pc, #56]
	b.n	.L_02000510
.L_020004fa:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000504
	ldr	r0, [pc, #52]
	b.n	.L_02000510
.L_02000504:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_0200050e
	ldr	r0, [pc, #52]
	b.n	.L_02000510
.L_0200050e:
	ldr	r0, [pc, #52]
.L_02000510:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000cf
	.4byte 0x000000d0
	.4byte 0x0200b648
	.4byte 0x000000d1
	.4byte 0x0200b660
	.4byte 0x000000d2
	.4byte 0x0200b690
	.4byte 0x000000d3
	.4byte 0x0200b708
	.4byte 0x000000d4
	.4byte 0x0200b7c8
	.2byte 0xb618
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	bl	sub_02003596
	movs	r3, #24
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #19
	movs	r1, #55
	movs	r2, #7
	movs	r3, #8
	bl	sub_0200354a
	movs	r3, #7
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #102
	movs	r2, #24
	movs	r3, #110
	movs	r0, #24
	bl	sub_02003536
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #66
	bl	sub_020034f0
	bl	sub_020035d4
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	bl	sub_020035da
	movs	r3, #32
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #37
	movs	r1, #55
	movs	r2, #7
	movs	r3, #8
	bl	sub_0200358e
	movs	r3, #7
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #102
	movs	r2, #32
	movs	r3, #110
	movs	r0, #32
	bl	sub_0200357a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #67
	bl	sub_02003534
	bl	sub_02003618
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	sub	sp, #8
	bl	sub_02003628
	movs	r0, #0
	bl	sub_0200372e
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r7, r5, r2
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r6, #37
	cmp	r3, #3
	bne.n	.L_020005fa
	movs	r6, #25
.L_020005fa:
	movs	r0, #158
	bl	sub_020037a6
	movs	r3, #1
	str	r3, [sp, #0]
	mov	r8, r3
	adds	r2, r6, #0
	movs	r5, #2
	movs	r1, #38
	movs	r3, #49
	movs	r0, #30
.L_02000610:
	str	r5, [sp, #4]
	bl	sub_020035d4
	movs	r0, #10
	bl	sub_0200365a
.L_0200061c:
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r1, #38
	adds	r2, r6, #0
	movs	r3, #49
	movs	r0, #32
	str	r5, [sp, #4]
	bl	sub_020035ec
	movs	r0, #10
	bl	sub_02003672
	ldr	r6, [pc, #152]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	bl	sub_020036a8
	ldr	r0, [r6, #0]
	bl	sub_020036a6
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r0, [r6, #0]
	bl	sub_020036b2
	ldr	r5, [r0, #8]
	mov	r2, r8
	ldr	r0, [r6, #0]
	asrs	r5, r5, #19
	orrs	r5, r2
	bl	sub_020036c0
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	lsls	r5, r5, #3
	mov	r3, r8
	orrs	r2, r3
	adds	r1, r5, #0
	ldr	r0, [r6, #0]
	bl	sub_020036f2
	movs	r0, #123
	bl	sub_02003828
	ldr	r0, [r6, #0]
	bl	sub_020036de
	ldr	r5, [r0, #8]
	ldr	r0, [r6, #0]
	bl	sub_020036e6
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	asrs	r5, r5, #19
	mov	r3, r8
	orrs	r2, r3
	lsls	r5, r5, #3
	subs	r2, #16
	adds	r1, r5, #0
	ldr	r0, [r6, #0]
	bl	sub_02003714
	movs	r0, #5
	bl	sub_020036ea
	bl	sub_020037d6
	bl	sub_020037e2
	ldr	r0, [r6, #0]
	bl	sub_02003738
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	bl	sub_020037a8
	bl	sub_02003714
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x6c426883
	.4byte 0x189b6c81
	.4byte 0x68c36083
	.4byte 0x185b6cc2
	.4byte 0x690360c3
	.4byte 0x6103189b
	.4byte 0x69836b02
	.4byte 0x6183189b
	.4byte 0x69c36b42
	.4byte 0x61c3189b
	.4byte 0x18c94b01
	.4byte 0x47706481
	.2byte 0xb334
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	add	r7, sp, #20
	movs	r3, #1
	str	r1, [sp, #16]
	str	r3, [r7, #0]
	movs	r3, #24
	adds	r3, #255
	strh	r3, [r7, #24]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r7, #20]
	str	r3, [r7, #16]
	ldr	r3, [pc, #88]
	str	r2, [r7, #12]
	str	r2, [r7, #8]
	str	r3, [r7, #36]
	ldr	r3, [sp, #16]
	movs	r2, #0
	mov	fp, r0
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_020007cc
.L_02000744:
	bl	sub_0200364e
	ldr	r3, [pc, #60]
	ands	r0, r3
	lsls	r0, r0, #12
	strh	r0, [r7, #32]
	bl	sub_0200365a
	mov	r8, r0
	movs	r2, #15
	mov	r3, r8
	ands	r3, r2
	mov	sl, r2
	mov	r8, r3
	subs	r2, #23
	add	r8, r2
	mov	r3, r8
	lsls	r3, r3, #14
	mov	r8, r3
	bl	sub_02003674
	mov	r2, sl
	adds	r5, r0, #0
	ands	r5, r2
	bl	sub_0200367e
	mov	r2, fp
	ldr	r6, [r2, #0]
	movs	r3, #31
	ands	r3, r0
	lsls	r3, r3, #16
	subs	r6, r6, r3
	b.n	.L_02000790
	.2byte 0x0000
	.4byte 0x0000000f
	.2byte 0x86d5
	.2byte 0x0200
.L_02000790:
	movs	r3, #240
	lsls	r3, r3, #12
	adds	r6, r6, r3
	bl	sub_020036a0
	mov	r3, fp
	mov	r2, sl
	ldr	r1, [r3, #4]
	ands	r0, r2
.L_020007a2:
	ldr	r2, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #175
	lsls	r0, r0, #16
	lsls	r3, r3, #17
	adds	r5, #8
	adds	r1, r1, r0
	str	r3, [sp, #8]
	lsls	r5, r5, #14
	mov	r3, r8
	adds	r0, r6, #0
	str	r5, [sp, #0]
	str	r7, [sp, #12]
	bl	.L_02000878
	ldr	r3, [sp, #16]
	movs	r2, #1
	add	r9, r2
	cmp	r9, r3
	bne.n	.L_02000744
.L_020007cc:
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #20
	cmp	r0, #2
	beq.n	.L_020007ee
	cmp	r0, #2
	ble.n	.L_02000838
	cmp	r0, #3
	beq.n	.L_0200080c
	b.n	.L_02000838
.L_020007ee:
	ldr	r3, [pc, #76]
	add	r0, sp, #8
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r2, #204
	movs	r3, #188
	lsls	r3, r3, #17
	lsls	r2, r2, #8
	str	r3, [r0, #8]
	adds	r2, #204
	movs	r1, #1
	bl	.L_02000f10
	b.n	.L_02000838
.L_0200080c:
	movs	r3, #4
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #31
	movs	r1, #85
	movs	r2, #20
	movs	r3, #85
	bl	sub_020037dc
	ldr	r3, [pc, #28]
	add	r0, sp, #8
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r3, #188
	lsls	r3, r3, #17
	movs	r2, #192
	str	r3, [r0, #8]
	lsls	r2, r2, #9
	movs	r1, #8
	bl	.L_02000f3e
.L_02000838:
	add	sp, #20
	pop	{pc}
	.2byte 0x0000
	.2byte 0x015b
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r6, #7
	ldr	r7, [r3, #0]
	sub	sp, #56
	ands	r7, r6
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_0200089a
	bl	sub_02003760
	movs	r5, #15
	ands	r5, r0
	bl	sub_02003768
	movs	r3, #209
	lsls	r3, r3, #1
	ands	r0, r6
	adds	r3, #255
	add	r6, sp, #16
	strh	r3, [r6, #24]
	mov	r3, r8
	ldr	r4, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r2, [r3, #16]
	movs	r3, #128
.L_02000878:
	lsls	r3, r3, #8
	subs	r5, #8
	lsls	r5, r5, #16
	subs	r0, #8
	str	r3, [sp, #0]
	movs	r3, #128
	lsls	r0, r0, #16
	lsls	r3, r3, #13
	adds	r4, r4, r5
	adds	r1, r1, r0
	str	r3, [sp, #8]
	adds	r0, r4, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl	.L_02000950
.L_0200089a:
	add	sp, #56
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	movs	r0, #8
	sub	sp, #8
	bl	sub_02003908
	adds	r5, r0, #0
	bl	sub_020038fe
	movs	r0, #0
	bl	sub_02003a04
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #68
	bl	sub_02003836
	movs	r3, #20
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #22
	movs	r0, #11
	movs	r2, #3
	movs	r3, #2
	bl	sub_020038c2
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	.L_02000b86
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl	sub_02003980
	movs	r1, #172
	movs	r2, #186
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #68
	bl	sub_0200398e
	movs	r0, #68
	bl	sub_02003964
	ldr	r3, [pc, #12]
	str	r3, [r0, #108]
	bl	sub_02003964
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x8841
	.2byte 0x0200
	push	{lr}
	bl	sub_0200396c
	movs	r0, #0
	bl	sub_02003a72
	ldr	r0, [pc, #40]
	movs	r1, #1
	bl	sub_0200396a
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #73
	bl	sub_020038a4
	cmp	r0, #0
	beq.n	.L_02000952
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
.L_02000950:
	strh	r3, [r2, #0]
.L_02000952:
	bl	sub_020039a4
	pop	{pc}
	.2byte 0x1a95
	.2byte 0x0000
	push	{lr}
	bl	.L_020021fc
	pop	{pc}
	push	{r5, lr}
	movs	r0, #8
	bl	sub_020039c2
	adds	r5, r0, #0
	bl	sub_020039b8
	movs	r0, #0
	bl	sub_02003abe
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_02003a90
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl	sub_02003a94
	movs	r0, #2
	bl	sub_02003aaa
	movs	r0, #132
	bl	sub_02003b40
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl	sub_020039a8
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r1, r1
	negs	r0, r0
	bl	sub_020039ba
	movs	r0, #2
	bl	sub_02003a00
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl	sub_02003ad2
	movs	r0, #2
	bl	sub_02003ae8
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #90
	strb	r3, [r2, #0]
	movs	r1, #212
	movs	r2, #134
	str	r3, [r5, #108]
	lsls	r2, r2, #2
	movs	r0, #8
	lsls	r1, r1, #1
	bl	sub_02003a50
	ldr	r1, [pc, #84]
	adds	r0, r5, #0
	bl	sub_02003980
	movs	r0, #8
	bl	sub_02003a76
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_0200396e
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r5, #200
	ldr	r1, [r3, #0]
	lsls	r5, r5, #6
	movs	r0, #8
	bl	sub_02003b6a
	adds	r3, r5, #0
	movs	r1, #21
	movs	r2, #33
	movs	r0, #0
	bl	.L_02000d0e
	adds	r3, r5, #0
	movs	r1, #22
	movs	r2, #33
	movs	r0, #0
	bl	.L_02000d1a
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r1, #26
	movs	r2, #33
	movs	r0, #0
	bl	.L_02000d28
	bl	sub_02003a8c
	pop	{r5, pc}
	.4byte 0x0200b370
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	adds	r6, r1, #0
	mov	sl, r0
	movs	r1, #26
	ldrsh	r0, [r3, r1]
	sub	sp, #56
	mov	r9, r3
	bl	sub_02003ac0
	adds	r7, r0, #0
	cmp	r6, #7
	bgt.n	.L_02000adc
	movs	r3, #24
	add	r5, sp, #16
	adds	r3, #255
	strh	r3, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #12]
	str	r3, [r5, #8]
	movs	r3, #128
	movs	r2, #0
	lsls	r3, r3, #8
	mov	r8, r2
	str	r2, [r5, #0]
	str	r3, [r5, #20]
	str	r3, [r5, #16]
	bl	sub_02003998
	ldr	r3, [pc, #60]
	ands	r0, r3
	lsls	r0, r0, #12
	strh	r0, [r5, #32]
	bl	sub_020039a4
	ldr	r1, [r7, #12]
	lsls	r2, r6, #1
	adds	r2, r2, r6
	lsls	r2, r2, #16
	movs	r4, #128
	subs	r1, r1, r2
	lsls	r4, r4, #13
	movs	r3, #15
	adds	r1, r1, r4
	ands	r3, r0
	mov	r4, r8
	ldr	r0, [r7, #8]
	ldr	r2, [r7, #16]
	subs	r3, #8
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	movs	r4, #180
	lsls	r3, r3, #13
	lsls	r4, r4, #15
	str	r4, [sp, #8]
	str	r5, [sp, #12]
	bl	.L_02000b82
	b.n	.L_02000ad4
	.2byte 0x0000
	.2byte 0x000f
	.2byte 0x0000
.L_02000ad4:
	ldr	r3, [r7, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r1
	str	r3, [r7, #28]
.L_02000adc:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_02000b22
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #28]
	adds	r3, r7, #0
	adds	r3, #100
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl	sub_02003a62
	mov	r2, r9
	movs	r1, #26
	ldrsh	r0, [r2, r1]
	bl	sub_02003b54
	bl	.L_02000e38
	ldr	r2, [r7, #8]
	ldr	r3, [r7, #16]
	asrs	r2, r2, #20
	mov	r4, sl
	asrs	r3, r3, #20
	adds	r2, #64
	movs	r0, #67
	movs	r1, #23
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl	sub_02003ad8
	movs	r3, #0
	str	r3, [r7, #16]
	str	r3, [r7, #12]
	str	r3, [r7, #8]
.L_02000b22:
	add	sp, #56
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb500
	bl	.L_02001580
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r5, [pc, #180]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl	sub_02003ba6
	adds	r6, r0, #0
	bl	sub_02003b9c
	movs	r0, #0
	bl	sub_02003ca2
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r1, r1
	negs	r0, r0
	bl	sub_02003c4c
	ldr	r0, [r5, #0]
	bl	sub_02003bca
	movs	r1, #0
	bl	sub_02003b68
	movs	r3, #0
	mov	r8, r3
	movs	r3, #128
	lsls	r3, r3, #7
.L_02000b82:
	strh	r3, [r6, #6]
	movs	r0, #10
.L_02000b86:
	bl	sub_02003bc8
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl	sub_02003c28
	movs	r0, #30
	bl	sub_02003bd6
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl	sub_02003c68
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl	sub_02003c50
	movs	r0, #20
	bl	sub_02003bee
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
	movs	r1, #5
	ldr	r0, [r5, #0]
	bl	sub_02003c54
	ldr	r0, [r5, #0]
	movs	r1, #24
	bl	sub_02003c64
	movs	r0, #40
	bl	sub_02003c0a
	movs	r0, #152
	movs	r1, #228
	movs	r3, #12
	lsls	r1, r1, #18
	movs	r2, #0
	negs	r3, r3
	lsls	r0, r0, #17
	bl	sub_02003c0c
	ldr	r0, [r5, #0]
	bl	sub_02003c3a
	mov	r3, r8
	str	r3, [r0, #68]
	movs	r0, #11
	bl	sub_02003cd4
	bl	sub_02003c40
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000c4c
	ldr	r0, [pc, #48]
	bl	sub_02003d8a
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #20]
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	lsls	r2, r2, #8
	ands	r3, r1
	orrs	r2, r3
	adds	r0, #100
	strh	r2, [r0, #0]
	b.n	.L_02000c58
	.4byte 0x000000ff
	.4byte 0x02000240
	.4byte 0x000000d3
	.2byte 0xb264
	.2byte 0x0200
.L_02000c4c:
	ldr	r3, [pc, #12]
	cmp	r2, r3
	bne.n	.L_02000c58
	ldr	r0, [pc, #12]
	bl	sub_02003dc6
.L_02000c58:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000000d4
	.2byte 0xb26e
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	lsls	r1, r1, #16
	adds	r3, r3, r2
	asrs	r1, r1, #16
	ldr	r5, [r3, #0]
	mov	r8, r1
	bl	sub_02003cca
	movs	r0, #0
	bl	sub_02003dd0
	adds	r3, r5, #0
	adds	r3, #100
	ldrh	r1, [r3, #0]
	ldr	r2, [r5, #8]
	lsls	r3, r1, #16
	asrs	r2, r2, #20
	asrs	r3, r3, #24
	subs	r7, r2, r3
	ldr	r2, [r5, #16]
	movs	r3, #255
	ands	r3, r1
	asrs	r2, r2, #20
	subs	r6, r2, r3
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #48]
	b.n	.L_02000cd8
.L_02000cac:
	ldr	r3, [r5, #8]
	ldr	r0, [r5, #16]
	lsls	r1, r7, #20
	adds	r1, r1, r3
	lsls	r3, r6, #20
	ldr	r2, [r5, #12]
	adds	r3, r3, r0
	adds	r0, r5, #0
	bl	sub_02003c6e
	mov	r3, r8
	lsls	r0, r3, #16
	lsrs	r0, r0, #16
	movs	r1, #1
	bl	sub_02003da2
	adds	r0, r5, #0
	bl	sub_02003c88
	movs	r0, #1
	bl	sub_02003bc6
.L_02000cd8:
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r1, r1, r7
	adds	r2, r2, r6
	movs	r0, #1
	bl	.L_02000f9c
	asrs	r0, r0, #8
	cmp	r0, #50
	bne.n	.L_02000d08
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r1, r1, r7
	adds	r2, r2, r6
	movs	r0, #2
	bl	.L_02000fb4
	asrs	r0, r0, #8
	cmp	r0, #255
	bne.n	.L_02000cac
.L_02000d08:
	ldr	r3, [r5, #8]
	movs	r2, #1
	asrs	r3, r3, #19
.L_02000d0e:
	orrs	r3, r2
	lsls	r3, r3, #19
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	asrs	r3, r3, #19
	orrs	r3, r2
.L_02000d1a:
	lsls	r3, r3, #19
	str	r3, [r5, #16]
	bl	sub_02003e98
	bl	sub_02003d74
	pop	{r3}
.L_02000d28:
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r3, r1
	ldr	r6, [r3, #0]
	bl	sub_02003d8c
	movs	r0, #0
	bl	sub_02003e92
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #2
	bl	sub_02003d2c
	ldr	r3, [r6, #12]
	cmp	r0, r3
	beq.n	.L_02000d88
	movs	r2, #34
	adds	r2, r2, r6
	movs	r3, #2
	adds	r7, r6, #0
	strb	r3, [r2, #0]
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
	adds	r0, r6, #0
	mov	r8, r2
	bl	.L_0200100a
	movs	r0, #188
	bl	sub_02003f20
	adds	r0, r6, #0
	bl	sub_02001016
	movs	r5, #0
	mov	r3, r8
	strb	r5, [r7, #0]
	strb	r5, [r3, #0]
.L_02000d88:
	bl	sub_02003f02
	ldr	r3, [pc, #76]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000dd0
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #27
	bne.n	.L_02000db0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #71
	bl	sub_02003d1e
.L_02000db0:
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #25
	bne.n	.L_02000dd0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #71
	bl	sub_02003d28
	cmp	r0, #0
	beq.n	.L_02000dd0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #70
	bl	sub_02003d3e
.L_02000dd0:
	bl	sub_02003e22
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x00d3
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	bl	sub_02003db2
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl	sub_02003d6c
	ldr	r5, [pc, #124]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #6
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02000e70
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #39
	movs	r1, #100
	movs	r2, #53
	movs	r3, #100
	bl	sub_02003ddc
	movs	r3, #53
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #43
	movs	r1, #41
	movs	r2, #3
	movs	r3, #3
	bl	sub_02003e18
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
.L_02000e38:
	ldr	r0, [r5, #0]
	bl	sub_02003e94
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #38
	bne.n	.L_02000e70
	ldr	r0, [r5, #0]
	bl	sub_02003ea2
	movs	r2, #6
	ldrsh	r3, [r0, r2]
	ldr	r0, [r5, #0]
	cmp	r3, #0
	bge.n	.L_02000e64
	movs	r1, #218
	movs	r2, #150
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl	sub_02003ee8
	b.n	.L_02000e70
.L_02000e64:
	movs	r1, #218
	movs	r2, #158
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl	sub_02003ef6
.L_02000e70:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	bl	sub_02003e4e
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl	sub_02003df8
	ldr	r5, [pc, #112]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #6
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02000ef8
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #100
	movs	r2, #53
	movs	r3, #100
	bl	sub_02003e70
	movs	r3, #53
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #43
	movs	r1, #37
	movs	r2, #3
	movs	r3, #3
	bl	sub_02003eac
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl	sub_02003f28
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #38
	ble.n	.L_02000eea
	movs	r3, #248
	lsls	r3, r3, #5
	movs	r0, #0
	movs	r1, #54
	movs	r2, #37
	bl	.L_020011d6
	b.n	.L_02000ef8
.L_02000eea:
	movs	r3, #248
	lsls	r3, r3, #5
	movs	r0, #0
	movs	r1, #54
	movs	r2, #39
	bl	.L_020011e6
.L_02000ef8:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl	sub_0200404e
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #20
.L_02000f10:
	cmp	r0, #2
	beq.n	.L_02000f6a
	cmp	r0, #2
	bgt.n	.L_02000f1e
	cmp	r0, #1
	beq.n	.L_02000f24
	b.n	.L_02000fa6
.L_02000f1e:
	cmp	r0, #3
	beq.n	.L_02000f8a
	b.n	.L_02000fa6
.L_02000f24:
	movs	r3, #4
	str	r3, [sp, #4]
	movs	r5, #3
	movs	r0, #18
	movs	r1, #97
	movs	r2, #6
	movs	r3, #97
	str	r5, [sp, #0]
	bl	sub_02003ef6
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #82
.L_02000f3e:
	movs	r1, #34
	movs	r2, #70
	movs	r3, #34
	str	r5, [sp, #0]
	bl	sub_02003f08
	movs	r3, #7
	movs	r2, #35
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #36
	movs	r2, #1
	movs	r3, #1
	bl	sub_02003f44
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #75
	bl	sub_02003ed6
	b.n	.L_02000fa6
.L_02000f6a:
	movs	r3, #240
	add	r0, sp, #8
	lsls	r3, r3, #15
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r2, #204
	movs	r3, #142
	lsls	r3, r3, #18
	lsls	r2, r2, #8
	str	r3, [r0, #8]
	adds	r2, #204
	movs	r1, #1
	bl	sub_0200168e
	b.n	.L_02000fa6
.L_02000f8a:
	movs	r3, #240
	add	r0, sp, #8
	lsls	r3, r3, #15
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r3, #142
	lsls	r3, r3, #18
	movs	r2, #192
.L_02000f9c:
	str	r3, [r0, #8]
	lsls	r2, r2, #9
	movs	r1, #8
	bl	sub_020016ac
.L_02000fa6:
	add	sp, #20
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl	sub_020040fa
.L_02000fb4:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r1, #0
	sub	sp, #8
	adds	r5, r0, #0
	cmp	r6, #0
	bne.n	.L_02000fce
	bl	sub_02003dc6
	ldr	r0, [pc, #116]
	bl	sub_02003ecc
.L_02000fce:
	cmp	r5, #1
	bne.n	.L_02001002
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #13
	movs	r3, #16
	bl	sub_02003fa2
	movs	r3, #13
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #3
	movs	r3, #3
	bl	sub_02003fde
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #72
	bl	sub_02003f70
.L_02001002:
	cmp	r5, #2
	bne.n	.L_02001018
	movs	r0, #232
	movs	r1, #128
.L_0200100a:
	movs	r2, #140
	lsls	r0, r0, #16
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	movs	r3, #2
	bl	sub_02003d5a
.L_02001018:
	cmp	r5, #3
	bne.n	.L_0200102e
	movs	r0, #232
	movs	r1, #128
	movs	r2, #140
	lsls	r0, r0, #16
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	movs	r3, #30
	bl	sub_02003d70
.L_0200102e:
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_0200103c
	bl	sub_02003ef6
.L_0200103c:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x937d
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #80]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_0200105c
	ldr	r0, [pc, #68]
	b.n	.L_02001096
.L_0200105c:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	beq.n	.L_02001094
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_0200106c
	ldr	r0, [pc, #64]
	b.n	.L_02001096
.L_0200106c:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02001076
	ldr	r0, [pc, #64]
	b.n	.L_02001096
.L_02001076:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02001080
	ldr	r0, [pc, #60]
	b.n	.L_02001096
.L_02001080:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_0200108a
	ldr	r0, [pc, #60]
	b.n	.L_02001096
.L_0200108a:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02001094
	ldr	r0, [pc, #56]
	b.n	.L_02001096
.L_02001094:
	ldr	r0, [pc, #56]
.L_02001096:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ce
	.4byte 0x0200b7f8
	.4byte 0x000000cf
	.4byte 0x000000d0
	.4byte 0x0200b93c
	.4byte 0x000000d1
	.4byte 0x0200b9e4
	.4byte 0x000000d2
	.4byte 0x0200ba5c
	.4byte 0x000000d3
	.4byte 0x0200bb28
	.4byte 0x000000d4
	.4byte 0x0200bc90
	.2byte 0xb81c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02004140
	adds	r6, r0, #0
	ldr	r3, [r6, #80]
	ldr	r2, [r6, #16]
	mov	r8, r3
	ldr	r3, [r5, #76]
	ldr	r7, [r5, #80]
	cmp	r2, r3
	bgt.n	.L_02001124
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #12
	movs	r3, #0
	ldrh	r2, [r1, #0]
	str	r3, [r5, #16]
	str	r3, [r5, #8]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #8]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_02001182
	.4byte 0x00000001
	.2byte 0x0240
	.2byte 0x0200
.L_02001124:
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #12
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #28]
	ldrh	r3, [r1, #0]
	movs	r0, #128
	orrs	r3, r2
	lsls	r0, r0, #2
	strh	r3, [r1, #0]
	adds	r0, #18
	bl	sub_020040ae
	cmp	r0, #0
	beq.n	.L_02001158
	movs	r3, #0
	str	r3, [r5, #16]
	str	r3, [r5, #8]
	b.n	.L_02001182
	.2byte 0x0002
	.2byte 0x0000
.L_02001158:
	ldr	r3, [r6, #8]
	ldr	r1, [r7, #40]
	str	r3, [r5, #8]
	ldr	r3, [r6, #12]
	str	r3, [r5, #12]
	ldr	r3, [r5, #76]
	ldr	r2, [r6, #16]
	subs	r2, r2, r3
	subs	r3, r3, r2
	str	r3, [r5, #16]
	ldrh	r3, [r6, #6]
	mvns	r3, r3
	strh	r3, [r5, #6]
	mov	r3, r8
	ldr	r2, [r3, #40]
	ldr	r3, [r2, #16]
	str	r3, [r1, #16]
	ldrh	r3, [r2, #2]
	strh	r3, [r1, #2]
	ldrb	r3, [r2, #20]
	strb	r3, [r1, #20]
.L_02001182:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl	sub_02004092
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020011b8
	adds	r3, #15
.L_020011b8:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
.L_020011d6:
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
.L_020011e6:
	push	{r6, r7}
	ldr	r3, [pc, #380]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl	sub_0200424e
	adds	r7, r0, #0
	bl	sub_02004244
	movs	r0, #0
	bl	sub_0200434a
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl	sub_020042f4
	bl	sub_020041c0
	movs	r0, #1
	bl	sub_0200410e
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl	sub_0200436a
	bl	sub_0200437e
	movs	r0, #204
	bl	sub_020043fc
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl	sub_0200429e
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #260]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_0200127a:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl	sub_020041a2
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl	sub_020041a8
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl	sub_020041a6
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #192]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl	sub_020041bc
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #180]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #160]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl	.L_02001398
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_0200127a
	movs	r0, #188
	bl	sub_02004498
	ldr	r5, [pc, #116]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl	sub_020043ca
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl	sub_020043a2
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl	sub_0200431a
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl	sub_0200432c
	bl	sub_02004338
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl	sub_02004402
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl	sub_020043aa
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02009189
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_020043e2
	ldr	r3, [r0, #12]
	asrs	r3, r3, #19
	cmp	r3, #0
	beq.n	.L_020013a2
	movs	r0, #0
	movs	r1, #14
.L_02001398:
	movs	r2, #17
	movs	r3, #0
	bl	sub_0200168e
	b.n	.L_020013b0
.L_020013a2:
	movs	r3, #200
	lsls	r3, r3, #6
	movs	r0, #0
	movs	r1, #14
	movs	r2, #17
	bl	sub_0200169e
.L_020013b0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	ldr	r1, [pc, #880]
	str	r2, [r3, #0]
	subs	r2, #36
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #872]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_020013e0
	b.n	.L_020017c4
.L_020013e0:
	ldr	r3, [pc, #864]
	cmp	r2, r3
	beq.n	.L_020013e8
	b.n	.L_0200150a
.L_020013e8:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bge.n	.L_020013f8
	b.n	.L_020017c4
.L_020013f8:
	cmp	r3, #4
	ble.n	.L_02001402
	cmp	r3, #11
	beq.n	.L_0200142a
	b.n	.L_020017c4
.L_02001402:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #66
	bl	sub_02004372
	cmp	r0, #0
	beq.n	.L_02001414
	bl	.L_0200195a
.L_02001414:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #67
	bl	sub_02004384
	cmp	r0, #0
	bne.n	.L_02001424
	b.n	.L_020017c4
.L_02001424:
	bl	.L_020019b2
	b.n	.L_020017c4
.L_0200142a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #69
	bl	sub_0200439a
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020014c6
	movs	r0, #8
	bl	sub_02004496
	adds	r5, r0, #0
	movs	r0, #8
	bl	sub_0200449e
	movs	r1, #0
	bl	sub_0200443c
	adds	r3, r5, #0
	adds	r3, #89
	strb	r6, [r3, #0]
	subs	r3, #4
	strb	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #186
	adds	r0, r5, #0
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	adds	r1, #255
	bl	sub_02004488
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #68
	bl	sub_020043da
	cmp	r0, #0
	bne.n	.L_0200147a
	b.n	.L_020017c4
.L_0200147a:
	movs	r3, #3
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #31
	movs	r1, #85
	movs	r2, #20
	movs	r3, #85
	bl	sub_0200444c
	movs	r3, #20
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #2
	movs	r0, #11
	movs	r1, #22
	movs	r2, #3
	bl	sub_02004488
	movs	r1, #172
	movs	r2, #186
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #68
	bl	sub_02004536
	movs	r0, #68
	bl	sub_0200450c
	ldr	r3, [pc, #656]
	movs	r1, #0
	str	r3, [r0, #108]
	movs	r2, #0
	movs	r0, #8
	bl	sub_0200454a
	b.n	.L_020017c4
.L_020014c6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #68
	bl	sub_02004436
	cmp	r0, #0
	bne.n	.L_020014d6
	b.n	.L_020017c4
.L_020014d6:
	movs	r3, #3
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #31
	movs	r1, #85
	movs	r2, #20
	movs	r3, #85
	bl	sub_020044a8
	movs	r3, #20
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #11
	movs	r1, #22
	movs	r2, #3
	movs	r3, #2
	bl	sub_020044e4
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl	sub_0200458e
	b.n	.L_020017c4
.L_0200150a:
	ldr	r3, [pc, #576]
	cmp	r2, r3
	bne.n	.L_020015ba
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #7
	ble.n	.L_02001520
	b.n	.L_020017c4
.L_02001520:
	cmp	r3, #6
	bge.n	.L_02001526
	b.n	.L_020017c4
.L_02001526:
	movs	r0, #100
	bl	sub_02004492
	cmp	r0, #0
	bne.n	.L_02001594
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_0200449e
	cmp	r0, #0
	bne.n	.L_02001568
	movs	r1, #172
	movs	r2, #134
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl	sub_020045d0
	movs	r3, #204
	lsls	r3, r3, #6
	movs	r1, #21
	movs	r2, #33
	movs	r0, #0
	bl	.L_02001846
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r0, #0
	movs	r1, #22
	movs	r2, #33
	bl	.L_02001854
	b.n	.L_02001584
.L_02001568:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_020044d8
	cmp	r0, #0
	bne.n	.L_02001584
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r0, #0
	movs	r1, #26
	movs	r2, #33
.L_02001580:
	bl	.L_02001872
.L_02001584:
	ldr	r3, [pc, #436]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #8
	bl	sub_020046ea
.L_02001594:
	movs	r0, #10
	adds	r0, #255
	bl	sub_02004502
	cmp	r0, #0
	beq.n	.L_020015a2
	b.n	.L_020017c4
.L_020015a2:
	ldr	r3, [pc, #408]
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #7
	beq.n	.L_020015b4
	b.n	.L_020017c4
.L_020015b4:
	bl	.L_02002796
	b.n	.L_020017c4
.L_020015ba:
	ldr	r3, [pc, #404]
	cmp	r2, r3
	bne.n	.L_020015c8
	ldr	r0, [pc, #400]
	bl	.L_020019c8
	b.n	.L_020017c4
.L_020015c8:
	ldr	r3, [pc, #396]
	cmp	r2, r3
	beq.n	.L_020015d0
	b.n	.L_02001770
.L_020015d0:
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #1
	cmp	r3, #9
	bls.n	.L_020015e2
	b.n	.L_020017c4
.L_020015e2:
	ldr	r2, [pc, #376]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x02009656
	.4byte 0x02009656
	.4byte 0x02009614
	.4byte 0x020096a4
	.4byte 0x020096a4
	.4byte 0x020096fe
	.4byte 0x020096fe
	.4byte 0x020097c4
	.4byte 0x020097c4
	.4byte 0x02009728
	.4byte 0x21ec208f
	.4byte 0x004023c8
	.4byte 0x22000449
	.4byte 0xf002041b
	.4byte 0x1c05ffcb
	.4byte 0xd1002d00
	.4byte 0x2101e0ca
	.4byte 0xf81bf000
	.4byte 0x32551c2a
	.4byte 0x70132300
	.4byte 0x21001c28
	.4byte 0xfff7f002
	.4byte 0x210e1c28
	.4byte 0xf857f003
	.4byte 0x21011c28
	.4byte 0xfffbf002
	.4byte 0x4842e0b6
	.4byte 0xfa01f000
	.4byte 0x01002090
	.4byte 0xf0023047
	.4byte 0x2800ffb3
	.4byte 0x21d8d014
	.4byte 0x200d2288
	.4byte 0x04120409
	.4byte 0xf843f003
	.4byte 0x01002090
	.4byte 0xf0023046
	.4byte 0x2800ffb3
	.4byte 0x21c8d006
	.4byte 0x200c2288
	.4byte 0x04120409
	.4byte 0xf843f003
	.4byte 0xf0034833
	.4byte 0x200cf8b3
	.4byte 0xf0032102
	.4byte 0xe08ff85b
	.4byte 0x01002090
	.4byte 0xf002304b
	.4byte 0x2800ffb3
	.4byte 0x2304d01c
	.4byte 0x25039301
	.4byte 0x21612012
	.4byte 0x23612206
	.4byte 0xf0029500
	.4byte 0x2302ffdf
	.4byte 0x20529301
	.4byte 0x22462122
	.4byte 0x95002322
	.4byte 0xffdff002
	.4byte 0x22232307
	.4byte 0x92019300
	.4byte 0x21242007
	.4byte 0x23012201
	.4byte 0xfff3f002
	.4byte 0x22002100
	.4byte 0x20042300
	.4byte 0xf0024d1c
	.4byte 0x2390ffcb
	.4byte 0x2100e007
	.4byte 0x23002200
	.4byte 0x4d182004
	.4byte 0xffcbf002
	.4byte 0x049b239c
	.4byte 0x4b1664c3
	.4byte 0x66c31c02
	.4byte 0x23003255
	.4byte 0x70136028
	.4byte 0xf0002103
	.4byte 0xe04df81b
	.4byte 0x30ff200a
	.4byte 0xffb3f002
	.4byte 0xd1472800
	.4byte 0xf8eff001
	.4byte 0x0000e044
	.4byte 0x02000240
	.4byte 0x000000ce
	.4byte 0x000000cf
	.4byte 0x02008841
	.4byte 0x000000d1
	.4byte 0x000000d2
	.4byte 0x0200b340
	.4byte 0x000000d3
	.4byte 0x020095ec
	.4byte 0x0200b35a
	.4byte 0x0200b264
	.4byte 0x0200bccc
	.2byte 0x90d5
	.2byte 0x0200
.L_02001770:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020017c4
	movs	r0, #162
	lsls	r0, r0, #1
	bl	sub_020046ec
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #72
	bl	sub_020046ee
	cmp	r0, #0
	beq.n	.L_020017b4
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #13
	movs	r3, #16
	bl	sub_0200475c
	movs	r3, #13
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #3
	movs	r3, #3
	bl	sub_02004798
	b.n	.L_020017be
.L_020017b4:
	movs	r1, #144
	ldr	r0, [pc, #24]
	lsls	r1, r1, #3
	bl	sub_020046b4
.L_020017be:
	ldr	r0, [pc, #20]
	bl	sub_0200492a
.L_020017c4:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x000000d4
	.4byte 0x0200937d
	.2byte 0xb26e
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldrh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_020017f4
	subs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200185a
.L_020017f4:
	adds	r3, r5, #0
	adds	r3, #90
	movs	r0, #131
	strb	r1, [r3, #0]
	lsls	r0, r0, #1
	bl	sub_02004768
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	bne.n	.L_0200181a
	ldr	r3, [pc, #80]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #76]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
.L_0200181a:
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_0200182c
	adds	r0, r5, #0
	movs	r1, #9
	bl	sub_020047a8
	b.n	.L_0200185a
.L_0200182c:
	ldrh	r1, [r5, #6]
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_0200183e
	adds	r3, r2, #0
.L_0200183e:
	ldr	r2, [pc, #36]
	cmp	r3, r2
	bge.n	.L_02001846
	adds	r3, r2, #0
.L_02001846:
	adds	r3, r1, r3
	adds	r0, r5, #0
	movs	r1, #2
	strh	r3, [r5, #6]
	bl	sub_020047d0
	adds	r0, r5, #0
.L_02001854:
	movs	r1, #48
	bl	sub_020047e0
.L_0200185a:
	pop	{r5, pc}
	.4byte 0x03001150
	.4byte 0x0200b272
	.2byte 0xf002
	.2byte 0xfc32
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
.L_02001872:
	lsls	r2, r2, #4
	adds	r2, #162
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02001898
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_020047f6
	bl	sub_0200498a
	bl	sub_020049ce
	movs	r0, #131
	lsls	r0, r0, #1
	bl	sub_0200480e
.L_02001898:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl	sub_020049f0
	adds	r7, r0, #0
.L_020018bc:
	bl	sub_02003126
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #132]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl	sub_020048f2
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #68]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02001974
	ldr	r3, [pc, #36]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #36]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #24]
	cmp	r3, r2
	bne.n	.L_020019a0
.L_0200195a:
	b.n	.L_02001b36
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200b2b2
	.2byte 0x0000
	.2byte 0xffff
.L_02001974:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl	sub_02004892
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_020019a0
	.2byte 0xc000
	.2byte 0xffff
.L_020019a0:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl	sub_020048d2
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
.L_020019b2:
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl	sub_02004998
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02001a22
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
.L_020019c8:
	ldr	r2, [r2, #8]
	bl	sub_020049a4
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001a22
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r2, [r7, #12]
	bl	sub_020049b4
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_0200498c
	adds	r0, r7, #0
	movs	r1, #48
	bl	sub_0200499c
	adds	r0, r7, #0
	bl	sub_020049d2
	ldr	r3, [pc, #292]
	str	r3, [r7, #108]
	b.n	.L_02001acc
.L_02001a22:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02001b18
.L_02001a36:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl	sub_02004a18
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001aec
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02001a64:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001a8e
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001a8e
	cmp	r5, r7
	beq.n	.L_02001a8e
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl	sub_02004ab0
	cmp	r0, #0
	bge.n	.L_02001aec
.L_02001a8e:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02001a64
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	bl	sub_02004a6e
	adds	r0, r7, #0
	bl	sub_02004a7c
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02001b12
.L_02001acc:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl	sub_020049fe
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl	sub_02004ac4
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02001a36
.L_02001aec:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl	sub_02004ab2
	adds	r0, r7, #0
	bl	sub_02004ac0
	movs	r0, #2
	bl	sub_020049fe
	b.n	.L_020018bc
.L_02001b12:
	movs	r0, #10
	bl	sub_02004a06
.L_02001b18:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_02004ab4
.L_02001b36:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x97dd
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl	sub_02004c9c
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	strh	r3, [r2, #0]
.L_02001b6e:
	bl	sub_020033d8
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #32]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	b.n	.L_02001bb4
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0200bcd0
	.2byte 0x0000
	.2byte 0xfff0
.L_02001bb4:
	.2byte 0x68fb
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl	sub_02004bb6
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #60]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02001c30
	ldr	r3, [pc, #28]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #28]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #16]
	cmp	r3, r2
	bne.n	.L_02001c5c
	b.n	.L_02001e26
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200b2b2
	.2byte 0x0000
	.2byte 0xffff
.L_02001c30:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl	sub_02004b4e
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_02001c5c
	.2byte 0xc000
	.2byte 0xffff
.L_02001c5c:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl	sub_02004b8e
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl	sub_02004c54
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02001cd6
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl	sub_02004c60
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001cd6
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [r7, #12]
	ldr	r3, [sp, #8]
	bl	sub_02004c70
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_02004c48
	adds	r0, r7, #0
	movs	r1, #48
	bl	sub_02004c58
	movs	r5, #0
	b.n	.L_02001cfe
.L_02001cd6:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02001e26
.L_02001cea:
	ldr	r3, [pc, #360]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02001cf6
	b.n	.L_02001e26
.L_02001cf6:
	movs	r0, #1
	bl	sub_02004bea
	adds	r5, #1
.L_02001cfe:
	cmp	r5, #179
	bgt.n	.L_02001d0c
	adds	r0, r7, #0
	bl	sub_02004d1e
	cmp	r0, #0
	beq.n	.L_02001cea
.L_02001d0c:
	ldr	r3, [pc, #328]
	str	r3, [r7, #108]
	b.n	.L_02001dda
.L_02001d12:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl	sub_02004cf4
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001dfa
	ldr	r3, [pc, #296]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001e26
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02001d4a:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001d74
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001d74
	cmp	r5, r7
	beq.n	.L_02001d74
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl	sub_02004d96
	cmp	r0, #0
	bge.n	.L_02001dfa
.L_02001d74:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02001d4a
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	movs	r5, #0
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	sub_02004d56
	b.n	.L_02001db2
.L_02001daa:
	movs	r0, #1
	bl	sub_02004c9e
	adds	r5, #1
.L_02001db2:
	cmp	r5, #179
	bgt.n	.L_02001dca
	adds	r0, r7, #0
	bl	sub_02004dd2
	cmp	r0, #0
	bne.n	.L_02001dca
	ldr	r3, [pc, #144]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02001daa
.L_02001dca:
	ldr	r3, [pc, #136]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001e26
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02001e20
.L_02001dda:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl	sub_02004d0c
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl	sub_02004dd2
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02001d12
.L_02001dfa:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl	sub_02004dc0
	adds	r0, r7, #0
	bl	sub_02004dce
	movs	r0, #2
	bl	sub_02004d0c
	b.n	.L_02001b6e
.L_02001e20:
	movs	r0, #10
	bl	sub_02004d14
.L_02001e26:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_02004dc2
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200bcd0
	.4byte 0x020097dd
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xbcd0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #32
	bl	sub_02004fbc
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
.L_02001e9a:
	bl	sub_02003704
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r2, [pc, #28]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r6, sp, #20
	add	r3, r9
	str	r3, [r6, #0]
	mov	r8, r3
	b.n	.L_02001edc
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
.L_02001edc:
	.2byte 0x68eb
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r7, r3, r1
	mov	r2, r8
	str	r7, [r6, #8]
	str	r2, [sp, #8]
	str	r7, [sp, #4]
	movs	r3, #34
	adds	r3, r3, r5
	ldrb	r0, [r3, #0]
	adds	r1, r2, #0
	adds	r2, r7, #0
	mov	fp, r3
	bl	sub_02004edc
	str	r0, [sp, #12]
	movs	r0, #128
	ldr	r1, [sp, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl	sub_02004e32
	mov	r1, fp
	ldrb	r0, [r1, #0]
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #0]
	bl	sub_02004ef6
	mov	sl, r0
	cmp	r0, #255
	beq.n	.L_02001f70
	mov	r2, fp
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl	sub_02004f00
	ldr	r3, [r5, #12]
	subs	r0, r0, r3
	cmp	r0, r9
	bgt.n	.L_02001f70
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #48]
	str	r3, [r6, #0]
	ldr	r1, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r1, [r6, #8]
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_02004ed6
	adds	r0, r5, #0
	movs	r1, #48
	bl	sub_02004ee6
	ldr	r3, [pc, #8]
	str	r3, [r5, #108]
	b.n	.L_0200201a
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x97dd
	.2byte 0x0200
.L_02001f70:
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r3, #0
	mov	r2, r8
	strh	r1, [r5, #6]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	str	r2, [r5, #8]
	str	r7, [r5, #16]
	b.n	.L_02002066
.L_02001f84:
	mov	r3, fp
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl	sub_02004f66
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #12
	cmp	r0, r1
	bgt.n	.L_0200203a
	ldrh	r3, [r5, #32]
	movs	r2, #0
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #20]
	movs	r3, #89
	adds	r3, r3, r6
	mov	r9, r2
	mov	r8, r3
.L_02001fb2:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001fdc
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001fdc
	cmp	r6, r5
	beq.n	.L_02001fdc
	ldrh	r3, [r6, #32]
	adds	r0, r6, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #20
	bl	sub_02004ffe
	cmp	r0, #0
	bge.n	.L_0200203a
.L_02001fdc:
	movs	r2, #1
	add	r9, r2
	movs	r3, #128
	mov	r1, r9
	add	r8, r3
	adds	r6, #128
	cmp	r1, #63
	ble.n	.L_02001fb2
	ldr	r2, [r7, #0]
	adds	r0, r5, #0
	str	r2, [sp, #8]
	ldr	r3, [r7, #8]
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl	sub_02004fbc
	adds	r0, r5, #0
	bl	sub_02004fca
	ldr	r1, [sp, #12]
	cmp	sl, r1
	bne.n	.L_02002060
.L_0200201a:
	movs	r0, #128
	ldr	r1, [sp, #16]
	add	r2, sp, #20
	lsls	r0, r0, #13
	bl	sub_02004f4c
	mov	r2, fp
	add	r7, sp, #20
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl	sub_02005012
	mov	sl, r0
	cmp	r0, #255
	bne.n	.L_02001f84
.L_0200203a:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #4]
	bl	sub_02005000
	adds	r0, r5, #0
	bl	sub_0200500e
	movs	r0, #2
	bl	sub_02004f4c
	b.n	.L_02001e9a
.L_02002060:
	movs	r0, #10
	bl	sub_02004f54
.L_02002066:
	movs	r3, #0
	str	r3, [r5, #108]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_02005002
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_020020f8
	adds	r7, r0, #0
.L_020020aa:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl	sub_0200510a
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #89
	movs	r2, #2
	ldrsh	r6, [r7, r2]
	ldrb	r2, [r1, #0]
	movs	r3, #4
	ldrsh	r4, [r7, r3]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	str	r4, [sp, #0]
	bl	sub_020050c0
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl	sub_0200426e
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020020aa
.L_020020f8:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r5, r0, #0
	mov	r9, r1
	mov	sl, r2
	movs	r1, #255
	ldr	r2, [r3, #0]
	b.n	.L_0200216c
.L_0200211c:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_02002168
	adds	r0, r7, #0
	bl	sub_02005182
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_02002144
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_02002144:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_02002176
	adds	r0, r5, #0
	bl	sub_020050be
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl	sub_020042e2
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_02002176
.L_02002168:
	adds	r5, #6
	movs	r1, #255
.L_0200216c:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_0200211c
.L_02002176:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	mov	r8, r2
	adds	r6, r1, #0
	bl	sub_020051ea
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r5, [r2, r3]
	adds	r7, r0, #0
	bl	sub_0200530c
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl	sub_0200511e
	cmp	r0, #0
	beq.n	.L_020021f8
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_020021e4
	cmp	r6, #1
	bcc.n	.L_020021da
	cmp	r6, #2
	beq.n	.L_020021ee
	b.n	.L_02002226
.L_020021da:
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_02005160
	b.n	.L_02002226
.L_020021e4:
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_0200516a
	b.n	.L_02002226
.L_020021ee:
	adds	r0, r7, #0
	movs	r1, #6
	bl	sub_02005174
	b.n	.L_02002226
.L_020021f8:
	movs	r3, #255
	strb	r3, [r5, #2]
.L_020021fc:
	cmp	r6, #1
	beq.n	.L_02002214
	cmp	r6, #1
	bcc.n	.L_0200220a
	cmp	r6, #2
	beq.n	.L_0200221e
	b.n	.L_02002226
.L_0200220a:
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_02005190
	b.n	.L_02002226
.L_02002214:
	adds	r0, r7, #0
	movs	r1, #3
	bl	sub_0200519a
	b.n	.L_02002226
.L_0200221e:
	adds	r0, r7, #0
	movs	r1, #5
	bl	sub_020051a4
.L_02002226:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #20
	str	r3, [sp, #16]
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	fp, r0
	cmp	r3, r2
	beq.n	.L_02002312
.L_02002252:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	str	r3, [sp, #12]
	bl	sub_020052b4
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r7, r0, #0
	str	r2, [sp, #8]
	movs	r3, #34
	adds	r3, r3, r7
	adds	r0, r2, #0
	ldrb	r2, [r3, #0]
	mov	r9, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #16]
	adds	r0, #1
	ldr	r5, [r2, r3]
	ldr	r2, [pc, #196]
	adds	r3, r5, r2
	ldr	r2, [pc, #196]
	asrs	r3, r3, #2
	adds	r6, r3, r2
	bl	sub_020051f6
	cmp	r0, #0
	beq.n	.L_020022a0
	ldr	r0, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	bl	sub_02005324
	b.n	.L_02002300
.L_020022a0:
	adds	r0, r7, #0
	bl	sub_02005404
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r5, r5, r3
	str	r5, [sp, #4]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl	sub_020052ca
	mov	r3, r9
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	sl, r0
	ldrb	r0, [r3, #0]
	bl	sub_020052a0
	adds	r5, r0, #0
	ldr	r0, [sp, #12]
	bl	sub_02005390
	ldr	r2, [sp, #4]
	movs	r3, #128
	asrs	r5, r5, #19
	strb	r3, [r2, #3]
	adds	r5, #4
	mov	r3, r9
	adds	r2, r5, #0
	ldrb	r0, [r3, #0]
	mov	r1, sl
	bl	sub_02005466
	add	r8, r6
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl	sub_0200525a
	cmp	r0, #0
	beq.n	.L_02002300
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_0200527e
.L_02002300:
	movs	r3, #4
	add	fp, r3
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002252
.L_02002312:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_02005376
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl	sub_02005306
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_0200233a
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_0200233a:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r5, r0, #0
	bl	sub_0200545a
	cmp	r0, #0
	beq.n	.L_0200236a
	b.n	.L_020024da
.L_0200236a:
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_020053ce
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl	sub_02005514
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02002396
	b.n	.L_020024da
.L_02002396:
	b.n	.L_020024cc
.L_02002398:
	ldrh	r7, [r5, #0]
	adds	r0, r7, #0
	bl	sub_020053f6
	cmp	r0, r8
	beq.n	.L_020023a8
	adds	r5, #4
	b.n	.L_020024cc
.L_020023a8:
	ldrh	r5, [r5, #2]
	bl	sub_020053f4
	adds	r0, r5, #0
	bl	sub_0200531a
	cmp	r0, #0
	bne.n	.L_02002416
	movs	r0, #125
	bl	sub_02005564
	adds	r0, r7, #0
	bl	sub_0200541a
	movs	r1, #7
	bl	sub_02005478
	movs	r0, #2
	bl	sub_020052be
	movs	r1, #0
	mov	r0, r8
	bl	sub_02005356
	adds	r0, r7, #0
	bl	sub_02005434
	movs	r1, #0
	bl	sub_02005492
	movs	r0, #2
	bl	sub_020052d8
	adds	r0, r7, #0
	bl	sub_02005446
	movs	r1, #7
	bl	sub_020054a4
	movs	r0, #4
	bl	sub_020052ea
	adds	r0, r7, #0
	bl	sub_02005458
	movs	r1, #0
	bl	sub_020054b6
	movs	r0, #0
	bl	sub_02004ddc
	adds	r0, r5, #0
	bl	sub_02005382
	b.n	.L_020024c6
.L_02002416:
	adds	r5, #1
	mov	sl, r5
	mov	r0, sl
	bl	sub_02005386
	cmp	r0, #0
	bne.n	.L_020024c6
	adds	r6, #85
	strb	r0, [r6, #0]
	movs	r0, #185
	bl	sub_020055d4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl	sub_0200543c
	movs	r0, #0
	bl	sub_02004e12
	movs	r5, #2
	movs	r0, #8
	mov	r7, r8
	bl	sub_0200533c
	negs	r5, r5
	mov	r0, r8
	movs	r1, #2
	adds	r7, #34
	bl	sub_020053d8
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl	sub_02005108
	movs	r0, #1
	bl	sub_02004e36
	movs	r0, #16
	bl	sub_0200535c
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0200511c
	movs	r0, #4
	bl	sub_0200536a
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl	sub_0200548c
	movs	r0, #8
	bl	sub_02005382
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #5
	bl	sub_0200538c
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r8
	bl	sub_02005448
	movs	r0, #2
	bl	sub_0200539e
	movs	r0, #188
	bl	sub_0200565c
	bl	sub_02004fbc
	movs	r0, #20
	bl	sub_020053ae
	mov	r0, sl
	bl	sub_02005434
.L_020024c6:
	bl	sub_02005518
	b.n	.L_020024da
.L_020024cc:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_020024da
	b.n	.L_02002398
.L_020024da:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_0200554e
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r7, r0, #0
	cmp	r3, r2
	beq.n	.L_020025c2
.L_02002506:
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L_02002510
	adds	r5, #4
	b.n	.L_020025b6
.L_02002510:
	ldrh	r5, [r5, #2]
	bl	sub_0200555c
	adds	r3, r5, #1
	mov	r8, r3
	mov	r0, r8
	bl	sub_02005486
	cmp	r0, #0
	bne.n	.L_020025b0
	movs	r0, #185
	bl	sub_020056d0
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl	sub_02005538
	movs	r0, #0
	bl	sub_02004f0e
	movs	r0, #8
	bl	sub_02005434
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_020054cc
	adds	r3, r7, #0
	adds	r3, #34
	movs	r0, #4
	ldrb	r1, [r3, #0]
	adds	r2, r6, #0
	negs	r0, r0
	bl	sub_02005178
	movs	r0, #1
	bl	sub_02004f32
	movs	r0, #16
	bl	sub_02005458
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl	sub_0200557a
	movs	r0, #8
	bl	sub_02005470
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl	sub_0200552c
	movs	r0, #2
	bl	sub_02005482
	movs	r0, #188
	bl	sub_02005740
	bl	sub_020050a0
	movs	r0, #20
	bl	sub_02005492
	adds	r0, r5, #0
	bl	sub_02005518
	mov	r0, r8
	bl	sub_0200551e
.L_020025b0:
	bl	sub_02005602
	b.n	.L_020025c2
.L_020025b6:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002506
.L_020025c2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_02005626
	movs	r3, #3
	adds	r0, #92
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl	sub_0200569a
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #16
	ldr	r5, [pc, #324]
	str	r3, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	fp, r0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl	sub_02005698
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl	sub_020056a0
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl	sub_020056a8
	movs	r0, #1
	bl	sub_0200550e
	movs	r0, #8
	bl	sub_02004bec
	movs	r0, #9
	bl	sub_02004bf2
	movs	r0, #10
	bl	sub_02004bf8
	movs	r1, #0
	movs	r0, #9
	bl	sub_020056d0
	movs	r0, #1
	bl	sub_0200552e
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl	sub_020056d0
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl	sub_020056da
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl	sub_020056e4
	movs	r0, #1
	bl	sub_02005552
	b.n	.L_0200271a
.L_02002666:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	mov	r9, r3
	mov	r0, r9
	bl	sub_020056c8
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r5, r0, #0
	str	r2, [sp, #8]
	adds	r7, r5, #0
	adds	r7, #34
	adds	r0, r2, #0
	ldrb	r2, [r7, #0]
	adds	r0, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldr	r6, [r2, r3]
	ldr	r2, [pc, #168]
	adds	r3, r6, r2
	ldr	r2, [pc, #168]
	asrs	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #3
	strb	r3, [r2, #0]
	bl	sub_02005612
	cmp	r0, #0
	beq.n	.L_020026bc
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl	sub_02005740
	b.n	.L_02002716
.L_020026bc:
	adds	r0, r5, #0
	bl	sub_02005820
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r6, r6, r3
	str	r6, [sp, #4]
	add	r8, sl
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r7, #0]
	bl	sub_020056e6
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	mov	sl, r0
	ldrb	r0, [r7, #0]
	bl	sub_020056ba
	adds	r5, r0, #0
	mov	r0, r9
	bl	sub_020057aa
	ldr	r6, [sp, #4]
	asrs	r5, r5, #19
	movs	r3, #128
	adds	r5, #4
	adds	r2, r5, #0
	strb	r3, [r6, #3]
	ldrb	r0, [r7, #0]
	mov	r1, sl
	bl	sub_0200587e
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl	sub_02005670
	cmp	r0, #0
	beq.n	.L_02002716
	mov	r0, r9
	movs	r1, #9
	bl	sub_020057e4
.L_02002716:
	movs	r3, #4
	add	fp, r3
.L_0200271a:
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002666
	movs	r0, #10
	bl	sub_0200561c
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, lr}
	adds	r5, r1, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	bl	sub_020056f4
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_02005700
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl	sub_0200586c
	cmp	r0, #0
	beq.n	.L_0200277c
	b.n	.L_0200293e
.L_0200277c:
	ldr	r3, [pc, #460]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_020057e0
	adds	r5, r0, #0
	movs	r0, #8
	bl	sub_020057e8
	ldr	r3, [r5, #8]
	adds	r7, r0, #0
.L_02002796:
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl	sub_0200592e
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_020027b0
	b.n	.L_0200293e
.L_020027b0:
	b.n	.L_02002930
.L_020027b2:
	ldrh	r3, [r6, #0]
	mov	r8, r3
	mov	r0, r8
	bl	sub_02005812
	cmp	r0, sl
	beq.n	.L_020027c4
	adds	r6, #4
	b.n	.L_02002930
.L_020027c4:
	ldrh	r6, [r6, #2]
	bl	sub_02005810
	adds	r0, r6, #0
	bl	sub_02005736
	cmp	r0, #0
	bne.n	.L_0200285e
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_0200575a
	mov	r1, sl
	adds	r0, r7, #0
	bl	sub_02004f2a
	movs	r0, #1
	bl	sub_020056d8
	movs	r0, #125
	bl	sub_02005996
	movs	r0, #8
	bl	sub_0200584c
	movs	r1, #7
	bl	sub_020058aa
	movs	r0, #2
	bl	sub_020056f0
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_02005788
	movs	r1, #9
	mov	r0, r8
	bl	sub_020058e0
	movs	r0, #8
	bl	sub_0200586e
	movs	r1, #0
	bl	sub_020058cc
	movs	r0, #2
	bl	sub_02005712
	movs	r0, #8
	bl	sub_02005880
	movs	r1, #7
	bl	sub_020058de
	movs	r0, #4
	bl	sub_02005724
	movs	r0, #8
	bl	sub_02005892
	movs	r1, #0
	bl	sub_020058f0
	movs	r0, #0
	bl	sub_02005216
	mov	r0, sl
	adds	r1, r7, #0
	bl	sub_02004f96
	movs	r0, #1
	bl	sub_02005744
	adds	r0, r6, #0
	bl	sub_020057ca
	b.n	.L_0200292a
.L_0200285e:
	adds	r6, #1
	mov	r9, r6
	mov	r0, r9
	bl	sub_020057ce
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200292a
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_020057f4
	mov	r1, sl
	adds	r0, r7, #0
	bl	sub_02004fc4
	adds	r5, #85
	movs	r0, #1
	bl	sub_02005774
	strb	r6, [r5, #0]
	movs	r0, #185
	bl	sub_02005a34
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl	sub_0200589c
	movs	r0, #0
	bl	sub_02005272
	mov	r8, r5
	movs	r0, #8
	movs	r6, #2
	mov	r5, sl
	bl	sub_0200579e
	negs	r6, r6
	adds	r0, r7, #0
	movs	r1, #2
	adds	r5, #34
	bl	sub_0200583a
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl	sub_0200556a
	movs	r0, #1
	bl	sub_02005298
	movs	r0, #16
	bl	sub_020057be
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl	sub_0200557e
	movs	r0, #4
	bl	sub_020057cc
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl	sub_020058ee
	movs	r0, #8
	bl	sub_020057e4
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r0, #5
	bl	sub_020057f0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl	sub_020058ac
	movs	r0, #2
	bl	sub_02005802
	movs	r0, #188
	bl	sub_02005ac0
	bl	sub_02005420
	movs	r0, #20
	bl	sub_02005812
	mov	r0, r9
	bl	sub_02005898
.L_0200292a:
	bl	sub_0200597c
	b.n	.L_0200293e
.L_02002930:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_0200293e
	b.n	.L_020027b2
.L_0200293e:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl	sub_0200585a
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002980
	adds	r3, #15
.L_02002980:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_02005a08
	ldr	r3, [r0, #80]
	ldr	r4, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r4, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #256]
	mov	r8, r0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_02005a40
	movs	r2, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r2
.L_020029f2:
	bl	sub_020058fc
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r2, [r7, #12]
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	mov	r3, sl
	lsls	r1, r3, #17
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #212]
	adds	r1, r1, r3
	ldr	r3, [pc, #212]
	adds	r1, r1, r0
	movs	r0, #30
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r7, #16]
	bl	sub_020059b4
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02002ac6
	mov	r1, r9
	ldr	r0, [r6, #80]
	bl	sub_02005b7a
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r6, #0
	bl	sub_02005a30
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_020059c8
	adds	r0, r6, #0
	ldr	r1, [pc, #152]
	bl	sub_020059e0
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	ldr	r1, [r6, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r1, #9]
	cmp	r2, #0
	beq.n	.L_02002a90
	mov	r3, sl
	lsls	r5, r3, #13
	adds	r0, r5, #0
	bl	sub_0200599c
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6470
	adds	r0, r5, #0
	bl	sub_020059a4
	b.n	.L_02002a94
.L_02002a90:
	mov	r0, r8
	str	r0, [r6, #68]
.L_02002a94:
	str	r0, [r6, #76]
	bl	sub_020059a0
	movs	r2, #192
	lsls	r0, r0, #14
	lsls	r2, r2, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r6, #72]
	bl	sub_020059b2
	ldr	r3, [pc, #68]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #60]
	str	r3, [r6, #48]
	ldr	r3, [pc, #60]
	str	r3, [r6, #52]
	ldr	r3, [pc, #60]
	str	r3, [r6, #108]
.L_02002ac6:
	movs	r0, #1
	add	sl, r0
	mov	r2, sl
	cmp	r2, #7
	bls.n	.L_020029f2
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0xfffe0000
	.4byte 0x0200b2d4
	.4byte 0x0300021c
	.4byte 0x00013333
	.4byte 0xffffff00
	.4byte 0xfffff800
	.4byte 0xfffffa00
	.2byte 0xa951
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #244]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_02005b72
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02002bf8
	movs	r3, #0
	mov	r9, r3
	mov	sl, r3
.L_02002b28:
	movs	r0, #30
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, #255
	bl	sub_02005acc
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002bee
	mov	r1, r9
	ldr	r0, [r7, #80]
	bl	sub_02005c92
	movs	r4, #0
	mov	r8, r4
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, #4
	strb	r2, [r3, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r7, #0
	bl	sub_02005b4c
	adds	r0, r7, #0
	movs	r1, #2
	bl	sub_02005ae4
	ldr	r1, [pc, #160]
	adds	r0, r7, #0
	bl	sub_02005afc
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl	sub_02005a96
	mov	r4, r8
	str	r4, [r7, #72]
	str	r0, [r7, #68]
	adds	r0, r5, #0
	bl	sub_02005a9a
	ldr	r3, [r7, #68]
	str	r0, [r7, #76]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r7, #68]
	bl	sub_02005a98
	lsls	r3, r0, #1
	ldr	r2, [r7, #68]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #108]
	adds	r2, r2, r3
	str	r2, [r7, #68]
	bl	sub_02005aae
	lsls	r3, r0, #1
	ldr	r2, [r7, #76]
	adds	r3, r3, r0
	ldr	r4, [pc, #96]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r7, #76]
	bl	sub_02005ac4
	ldr	r2, [pc, #84]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	adds	r3, r7, #0
	adds	r0, r0, r2
	adds	r3, #100
	strh	r0, [r3, #0]
	mov	r3, r8
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r3, [pc, #68]
	ldr	r0, [r7, #80]
	str	r3, [r7, #108]
	ldr	r3, [r6, #80]
	movs	r1, #12
	ldrb	r3, [r3, #9]
	movs	r4, #13
	ands	r1, r3
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #9]
.L_02002bee:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02002b28
.L_02002bf8:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200b304
	.4byte 0xffffa000
	.4byte 0xffffd000
	.4byte 0xfffff800
	.2byte 0xa951
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	adds	r0, r2, #0
	adds	r5, r1, #0
	bl	sub_02005c84
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r2, [pc, #88]
	adds	r7, r0, #0
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	adds	r5, r7, #0
	asrs	r3, r3, #2
	adds	r5, #34
	adds	r6, r3, r1
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl	sub_02005c6a
	ldr	r2, [r7, #16]
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl	sub_02005c3e
	ldr	r3, [r7, #8]
	asrs	r2, r0, #19
	add	r2, sl
	cmp	r3, #0
	bge.n	.L_02002c76
	ldr	r1, [pc, #48]
	adds	r3, r3, r1
.L_02002c76:
	ldr	r0, [r7, #16]
	asrs	r1, r3, #20
	cmp	r0, #0
	bge.n	.L_02002c82
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_02002c82:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r0, [r5, #0]
	mov	r1, r8
	adds	r6, r6, r3
	bl	sub_02005e10
	strb	r0, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0xffff
	.2byte 0x000f
	push	{lr}
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	bl	sub_020058d2
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r3, #192
	adds	r5, r7, r2
	lsls	r3, r3, #4
	movs	r2, #63
	adds	r6, r7, r3
	mov	r8, r2
.L_02002cde:
	ldr	r3, [r5, #24]
	cmp	r3, #19
	bhi.n	.L_02002d2c
	movs	r2, #176
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	ldrh	r1, [r1, #0]
	movs	r2, #7
	asrs	r3, r3, #2
	ands	r3, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r6, #8]
	adds	r0, r6, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	adds	r1, r5, #0
	bl	sub_02005ea4
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl	sub_02005eb6
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_02002d2c
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_02002d2c:
	.2byte 0x2301
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_02002cde
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r2, [sp, #0]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	adds	r2, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r2, #0
	ble.n	.L_02002df0
	adds	r7, r2, #0
.L_02002d6c:
	bl	sub_02005c76
	movs	r1, #176
	lsls	r1, r1, #5
	add	r1, fp
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	sl, r1
	lsls	r6, r3, #3
	subs	r6, r6, r3
	lsls	r6, r6, #2
	movs	r3, #160
	add	r6, fp
	lsls	r3, r3, #3
	adds	r5, r6, r3
	movs	r1, #0
	str	r1, [r5, #24]
	ldr	r2, [sp, #8]
	mov	r8, r1
	str	r2, [r5, #0]
	ldr	r3, [sp, #4]
	mov	r9, r0
	str	r3, [r5, #4]
	ldr	r1, [sp, #0]
	subs	r7, #1
	str	r1, [r5, #8]
	bl	sub_02005caa
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r0, r0, #3
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r5, #0
	bl	sub_02005cda
	mov	r3, r8
	str	r3, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl	sub_02005ccc
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #12
	movs	r2, #128
	adds	r6, r6, r3
	lsls	r2, r2, #10
	lsls	r0, r0, #1
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r6, #0
	bl	sub_02005d04
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #63
	adds	r3, #1
	ands	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	cmp	r7, #0
	bne.n	.L_02002d6c
.L_02002df0:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #8
	movs	r0, #220
	sub	sp, #4
	bl	sub_02005d44
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl	sub_02005d7c
	adds	r1, r6, #0
	bl	sub_02005d62
	bl	sub_02005d7e
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl	sub_02005d82
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #2
	mov	sl, r0
	adds	r3, r6, r1
	mov	r2, sl
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r2, #160
	movs	r3, #192
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	movs	r1, #63
	adds	r7, r6, r2
	adds	r5, r6, r3
	mov	r8, r1
.L_02002e58:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl	sub_02005ff8
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r8, r3
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_02002e58
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r2, r6, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl	sub_02005da0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001f0
	.2byte 0xacc1
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl	sub_02005dca
	movs	r3, #176
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_02005e22
	movs	r0, #220
	bl	sub_02005e18
	pop	{r5, pc}
	.4byte 0x0200acc1
	.irp EntryTarget, 0x03000528, 0x080000c1, 0x080000d1, 0x080000d9, 0x080000f9, 0x08000101, 0x08000119, 0x08000121, 0x08000129, 0x08000141, 0x08000151, 0x080001a9, 0x080001b9, 0x080001c9, 0x080001d1, 0x08000291, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x08020179, 0x080201a9, 0x080201b1, 0x080201c1, 0x080201c9, 0x080201e9, 0x08020219, 0x08020221, 0x08020229, 0x08020231, 0x080202f1, 0x080202f9, 0x08020301, 0x08020349, 0x08020361, 0x08038041, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d1, 0x080c80d9, 0x080c80f1, 0x080c80f9, 0x080c8111, 0x080c8119, 0x080c8121, 0x080c8149, 0x080c8171, 0x080c8201, 0x080c8209, 0x080c8219, 0x080c8221, 0x080c8229, 0x080c8239, 0x080c8279, 0x080c8289, 0x080c82e1, 0x080c82f9, 0x080c8379, 0x080c8381, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c8459, 0x080c84d9, 0x080c84e1, 0x080c8519, 0x080c8601, 0x080c8711, 0x080c8719, 0x080c8721, 0x080c8729, 0x080c8731, 0x080c8739, 0x080c87c1, 0x080c87d1, 0x080c87e1, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x000c000b
	.4byte 0x000e000d
	.4byte 0x0008ffff
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xa000e000
	.4byte 0x4000c000
	.4byte 0x60002000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xffffffff
	.4byte 0x4000c000
	.4byte 0xffffffff
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0x80000000
	.4byte 0x4000c000
	.4byte 0x80000000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0200b1b0
	.4byte 0x0200b1ec
	.4byte 0x0200b228
	.4byte 0x00070008
	.4byte 0x00090200
	.4byte 0x02010007
	.4byte 0x0007000a
	.4byte 0x000b0202
	.4byte 0x02030007
	.4byte 0x0008ffff
	.4byte 0x02000007
	.4byte 0x00070009
	.4byte 0x000a0201
	.4byte 0x02020007
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x80010000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00500330
	.4byte 0x03400210
	.4byte 0x02200060
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000f00b0
	.4byte 0x00c000c0
	.4byte 0x00d00021
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ce
	.4byte 0x1012e002
	.4byte 0xffffffff
	.4byte 0x102010cf
	.4byte 0xffffffff
	.4byte 0x000000cf
	.4byte 0x101020ce
	.4byte 0xffffffff
	.4byte 0x1020b0cf
	.4byte 0xffffffff
	.4byte 0x103070cf
	.4byte 0xffffffff
	.4byte 0x104080cf
	.4byte 0xffffffff
	.4byte 0x105050d3
	.4byte 0xffffffff
	.4byte 0x106100cf
	.4byte 0xffffffff
	.4byte 0x107030cf
	.4byte 0xffffffff
	.4byte 0x108040cf
	.4byte 0xffffffff
	.4byte 0x109120cf
	.4byte 0xffffffff
	.4byte 0x10a090d0
	.4byte 0xffffffff
	.4byte 0x10b020cf
	.4byte 0xffffffff
	.4byte 0x10c040d3
	.4byte 0xffffffff
	.4byte 0x10d0f0cf
	.4byte 0xffffffff
	.4byte 0x10e110cf
	.4byte 0xffffffff
	.4byte 0x10f0d0cf
	.4byte 0xffffffff
	.4byte 0x110060cf
	.4byte 0xffffffff
	.4byte 0x1110e0cf
	.4byte 0xffffffff
	.4byte 0x112090cf
	.4byte 0xffffffff
	.4byte 0x000000d0
	.4byte 0x101030d0
	.4byte 0xffffffff
	.4byte 0x102070d0
	.4byte 0xffffffff
	.4byte 0x103010d0
	.4byte 0xffffffff
	.4byte 0x1040a0d0
	.4byte 0xffffffff
	.4byte 0x105020d1
	.4byte 0xffffffff
	.4byte 0x1060b0d0
	.4byte 0xffffffff
	.4byte 0x107020d0
	.4byte 0xffffffff
	.4byte 0x1080c0d0
	.4byte 0xffffffff
	.4byte 0x1090a0cf
	.4byte 0xffffffff
	.4byte 0x10a040d0
	.4byte 0xffffffff
	.4byte 0x10b060d0
	.4byte 0xffffffff
	.4byte 0x10c080d0
	.4byte 0xffffffff
	.4byte 0x000000d1
	.4byte 0x101020d2
	.4byte 0xffffffff
	.4byte 0x102050d0
	.4byte 0xffffffff
	.4byte 0x103060d2
	.4byte 0xffffffff
	.4byte 0x104050d2
	.4byte 0xffffffff
	.4byte 0x105060d1
	.4byte 0xffffffff
	.4byte 0x106050d1
	.4byte 0xffffffff
	.4byte 0x000000d2
	.4byte 0x101030d2
	.4byte 0xffffffff
	.4byte 0x102010d1
	.4byte 0xffffffff
	.4byte 0x103010d2
	.4byte 0xffffffff
	.4byte 0x104080d2
	.4byte 0xffffffff
	.4byte 0x105040d1
	.4byte 0xffffffff
	.4byte 0x106030d1
	.4byte 0xffffffff
	.4byte 0x107010d3
	.4byte 0xffffffff
	.4byte 0x108040d2
	.4byte 0xffffffff
	.4byte 0x109020d3
	.4byte 0xffffffff
	.4byte 0x10a090d3
	.4byte 0xffffffff
	.4byte 0x10b070d1
	.4byte 0xffffffff
	.4byte 0x000000d3
	.4byte 0x101070d2
	.4byte 0xffffffff
	.4byte 0x102090d2
	.4byte 0xffffffff
	.4byte 0x103020d4
	.4byte 0xffffffff
	.4byte 0x1040c0cf
	.4byte 0xffffffff
	.4byte 0x105050cf
	.4byte 0xffffffff
	.4byte 0x106010d4
	.4byte 0xffffffff
	.4byte 0x107080d3
	.4byte 0xffffffff
	.4byte 0x108070d3
	.4byte 0xffffffff
	.4byte 0x1090a0d2
	.4byte 0xffffffff
	.4byte 0x10a0a0d3
	.4byte 0xffffffff
	.4byte 0x000000d4
	.4byte 0x101060d3
	.4byte 0xffffffff
	.4byte 0x102030d3
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x006400f5
	.4byte 0x00000007
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020085d1
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x020085d1
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000003
	.4byte 0xffff0034
	.4byte 0x02008921
	.4byte 0x50008a05
	.4byte 0x09420032
	.4byte 0x02008549
	.4byte 0x50008a05
	.4byte 0x09430033
	.4byte 0x0200858d
	.4byte 0x50009705
	.4byte 0x09440034
	.4byte 0x020087dd
	.4byte 0x00009705
	.4byte 0x09440034
	.4byte 0x020088a9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x0200895d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x0200895d
	.4byte 0x00000002
	.4byte 0x02000033
	.4byte 0x02008965
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008281
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008b3d
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x0200895d
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte 0x02008b35
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02008f01
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008b35
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008bfd
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008d2d
	.4byte 0x80008c15
	.4byte 0xffff000b
	.4byte 0x02008fad
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte 0x02008bfd
	.4byte 0x50008c15
	.4byte 0xffff000b
	.4byte 0x02008c65
	.4byte 0x10008c15
	.4byte 0x0946000c
	.4byte 0x02008bfd
	.4byte 0x00008c15
	.4byte 0x0946000c
	.4byte 0x02008d2d
	.4byte 0x10008c15
	.4byte 0x0947000d
	.4byte 0x02008bfd
	.4byte 0x00008c15
	.4byte 0x0947000d
	.4byte 0x02008d2d
	.4byte 0x80008c15
	.4byte 0xffff000e
	.4byte 0x02008fad
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x02008bfd
	.4byte 0x50008c15
	.4byte 0xffff000e
	.4byte 0x02008c65
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008e79
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008de5
	.4byte 0x80009705
	.4byte 0xffff0034
	.4byte 0x02008fad
	.4byte 0x50009705
	.4byte 0x094b0034
	.4byte 0x02008f0d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008bfd
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008d2d
	.4byte 0x50009705
	.4byte 0x09480032
	.4byte 0x02008fb9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
