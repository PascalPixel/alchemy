	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"

@ シーンの曲: シーンかグループと入口 (-1 はどれでも) から、
@ フラグ (-1 は常に) が立っている最初の行の曲
	.macro scene_cue scene:req, entrance:req, flag:req, cue:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \cue
	.endm
	.macro group_cue group:req, entrance:req, flag:req, cue:req
	.2byte \group, \entrance & 0x7fff, \flag, \cue
	.endm

@ 地名: シーンかグループと入口 (-1 はどれでも) から地名番号
	.macro scene_name scene:req, entrance:req, name:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro group_name group:req, entrance:req, name:req
	.2byte \group, \entrance & 0x7fff
	.4byte \name
	.endm

@ 戻り先: シーンと入口 (-1 はどれでも) から、記録するシーンと入口
	.macro return_to scene:req, entrance:req, saved:req, saved_entrance:req
	.2byte SceneId_\scene, \entrance, SceneId_\saved, \saved_entrance
	.endm

@ 戦闘の背景と、それを使うシーン
	.macro backdrop resource:req, scenes:vararg
	.2byte ResourceId_\resource, 0x8000
	.ifnb \scenes
	.irp scene, \scenes
	.4byte SceneId_\scene
	.endr
	.endif
	.endm

	.section .rodata
	.balign 4
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	group_cue 1, -1, -1, 74
	group_cue 2, 90, -1, 19
	group_cue 2, 91, -1, 19
	group_cue 2, 65, -1, 41
	group_cue 2, 66, -1, 41
	group_cue 2, 67, -1, 41
	group_cue 2, 68, -1, 41
	group_cue 2, 69, -1, 41
	group_cue 2, 70, -1, 41
	group_cue 2, 75, -1, 41
	group_cue 2, 73, -1, 22
	group_cue 2, -1, 0x16e, 9
	group_cue 2, -1, -1, 42
	scene_cue ShindenHeya, -1, 0x9f0, 74
	scene_cue ShindenHeya, -1, 0x814, 2
	scene_cue ShindenHeya, -1, -1, 74
	scene_cue HaidiaBabi, 19, -1, 3
	group_cue 3, -1, 0x9f0, 0
	group_cue 3, -1, 0x814, 2
	group_cue 3, -1, 0x901, 0
	group_cue 3, -1, 0x900, 23
	group_cue 3, -1, -1, 19
	scene_cue SoruStar, -1, -1, 22
	scene_cue SoruFunka, -1, -1, 63
	group_cue 4, -1, 0x9f0, 2
	group_cue 4, -1, 0x814, 63
	scene_cue SoruIriguchi2, -1, -1, 20
	group_cue 4, -1, -1, 20
	scene_cue KuupuappuRouya, 2, -1, 1
	group_cue 5, -1, -1, 0
	group_cue 6, -1, -1, 42
	group_cue 7, -1, -1, 24
	group_cue 8, -1, 0x845, 1
	group_cue 8, -1, -1, 2
	group_cue 9, -1, -1, 3
	group_cue 10, -1, 0x845, 1
	group_cue 10, -1, -1, 2
	scene_cue KorimaMura2, -1, -1, 2
	group_cue 11, -1, 0x845, 1
	group_cue 11, -1, -1, 2
	group_cue 12, -1, 0x845, 1
	group_cue 12, -1, -1, 2
	group_cue 13, -1, 0x845, 1
	group_cue 13, -1, -1, 21
	group_cue 14, -1, 0x845, 1
	group_cue 14, -1, 0x844, 21
	group_cue 14, -1, -1, 4
	scene_cue BiribinoDou3, -1, -1, 42
	group_cue 15, -1, -1, 24
	group_cue 17, -1, 0x881, 26
	group_cue 17, -1, -1, 2
	group_cue 18, -1, -1, 24
	scene_cue MakyuriChojo1, -1, -1, 22
	group_cue 19, -1, 0x881, 22
	group_cue 19, -1, -1, 27
	scene_cue ShianJiin2, 3, -1, 60
	group_cue 20, -1, -1, 5
	group_cue 21, -1, -1, 28
	group_cue 22, -1, -1, 21
	group_cue 23, -1, -1, 5
	group_cue 24, -1, -1, 42
	group_cue 25, -1, 0x909, 0
	group_cue 25, -1, -1, 4
	group_cue 26, -1, -1, 23
	group_cue 27, -1, -1, 5
	group_cue 28, -1, -1, 25
	group_cue 29, -1, -1, 0
	group_cue 30, -1, -1, 24
	group_cue 31, -1, -1, 24
	group_cue 32, -1, -1, 6
	group_cue 33, -1, -1, 3
	scene_cue RunpaSuhara, 10, -1, 24
	group_cue 34, -1, -1, 1
	group_cue 35, -1, -1, 24
	group_cue 36, -1, -1, 6
	group_cue 37, -1, 0x8f0, 29
	group_cue 37, -1, -1, 41
	group_cue 38, -1, -1, 6
	scene_cue TakaraShima1, -1, -1, 42
	scene_cue TakaraShima5, -1, -1, 61
	group_cue 39, -1, -1, 28
	group_cue 40, -1, 0x950, 1
	group_cue 40, -1, -1, 10
	group_cue 41, -1, -1, 3
	scene_cue KorosseoKawa, 1, -1, 7
	scene_cue KorosseoKabe, 1, -1, 7
	scene_cue KorosseoMaruta, 1, -1, 7
	scene_cue KorosseoKawa, -1, -1, 8
	scene_cue KorosseoKabe, -1, -1, 8
	scene_cue KorosseoMaruta, -1, -1, 8
	scene_cue KorashiamuIriguchi1, 2, -1, 3
	scene_cue KorashiamuIriguchi1, 4, -1, 3
	scene_cue KorashiamuIriguchi1, 8, -1, 3
	scene_cue KorashiamuIriguchi1, 12, -1, 3
	scene_cue KorashiamuIriguchi1, 21, -1, 3
	scene_cue KorashiamuIriguchi1, 31, -1, 3
	scene_cue KorashiamuIriguchi1, 64, -1, 3
	scene_cue KorashiamuIriguchi3, -1, 0x950, 1
	group_cue 42, -1, -1, 8
	group_cue 43, -1, -1, 29
	group_cue 44, -1, -1, 42
	group_cue 45, -1, -1, 42
	group_cue 46, -1, -1, 24
	group_cue 47, -1, -1, 24
	group_cue 48, -1, -1, 1
	group_cue 49, -1, -1, 23
	group_cue 50, -1, -1, 25
	group_cue 51, -1, -1, 26
	group_cue 52, -1, -1, 24
	scene_cue BabiChika2, 1, -1, 28
	scene_cue BabiChika2, 2, -1, 28
	scene_cue BabiChika2, 3, -1, 28
	group_cue 53, -1, -1, 29
	group_cue 54, -1, -1, 28
	group_cue 55, -1, 0x9a7, 1
	group_cue 55, -1, -1, 2
	group_cue 56, -1, -1, 27
	scene_cue VinasuChojo, -1, 0x351, 22
	scene_cue VinasuChojo, -1, 0x350, 30
	scene_cue VinasuChojo, -1, -1, 19
	group_cue 57, -1, -1, 43
	group_cue 58, -1, -1, 22
	group_cue 59, -1, -1, 9
	group_cue 60, -1, -1, 9
	.2byte -1, -1, -1, 0	@ 終わり

	.global Scene_AreaNameRules
Scene_AreaNameRules:
	group_name 0, -1, 0
	group_name 1, -1, 1
	group_name 2, -1, 2
	scene_name ShindenHeya, 1, 3
	scene_name ShindenHeya, 2, 3
	scene_name ShindenHeya, 20, 3
	scene_name ShindenHeya, 21, 3
	scene_name ShindenHeya, 12, 4
	scene_name ShindenHeya, 11, 4
	scene_name ShindenHeya, 10, 4
	scene_name ShindenHeya, 35, 5
	scene_name ShindenHeya, 32, 6
	scene_name ShindenHeya, 29, 7
	scene_name ShindenHeya, -1, 8
	scene_name HaidiaBabi, 19, 9
	group_name 3, -1, 10
	scene_name SoruIriguchi2, -1, 11
	scene_name SoruStar, -1, 12
	scene_name SoruFunka, -1, 12
	group_name 4, -1, 13
	scene_name RunpaSuhara, 7, 14
	scene_name RunpaSuhara, 11, 14
	scene_name KuupuappuRouya, 2, 15
	group_name 5, -1, 16
	group_name 6, -1, 17
	group_name 7, -1, 18
	scene_name BiribinoMura2, -1, 19
	group_name 8, -1, 20
	scene_name BiribinoNiwa, -1, 21
	group_name 9, -1, 21
	group_name 10, -1, 22
	scene_name KorimaMura2, -1, 23
	group_name 11, -1, 24
	group_name 12, -1, 25
	group_name 13, -1, 26
	group_name 14, -1, 27
	scene_name BiribinoDou3, -1, 28
	group_name 15, -1, 29
	scene_name ImiruMura2, 7, 30
	group_name 17, -1, 31
	group_name 18, -1, 32
	scene_name MakyuriChojo1, -1, 33
	group_name 19, -1, 34
	scene_name ShianJiin2, 2, 35
	scene_name ShianJiin2, 3, 36
	group_name 20, -1, 37
	group_name 21, -1, 38
	group_name 22, -1, 39
	group_name 23, -1, 40
	group_name 24, -1, 41
	.if EDITION_INTERNATIONAL
	scene_name ArutinYama4, 6, 41
	scene_name ArutinYama4, 7, 41
	.endif
	group_name 25, -1, 42
	group_name 26, -1, 43
	group_name 27, -1, 44
	group_name 28, -1, 45
	group_name 29, -1, 46
	group_name 30, -1, 47
	group_name 31, -1, 48
	scene_name KareiHeya2, -1, 49
	group_name 32, -1, 50
	scene_name KareiKyuden, -1, 51
	group_name 33, -1, 51
	scene_name RunpaSuhara, 10, 52
	group_name 34, -1, 53
	group_name 35, -1, 54
	scene_name KareiTorebi2, 2, 55
	group_name 36, -1, 56
	group_name 37, -1, 57
	group_name 38, -1, 58
	scene_name TakaraShima1, -1, 59
	scene_name TakaraShima5, -1, 60
	group_name 39, -1, 61
	scene_name TorebiHeya, 8, 62
	scene_name TorebiHeya, 30, 62
	group_name 40, -1, 63
	scene_name TorebiKyuden2, -1, 64
	scene_name TorebiKyuden1, -1, 64
	scene_name KorashiamuIriguchi2, -1, 65
	scene_name KorashiamuIriguchi3, -1, 66
	scene_name KorashiamuIriguchi1, 2, 67
	scene_name KorashiamuIriguchi1, 4, 67
	scene_name KorashiamuIriguchi1, -1, 68
	scene_name KorosseoKawa, -1, 69
	scene_name KorosseoKabe, -1, 70
	scene_name KorosseoMaruta, -1, 71
	group_name 43, -1, 72
	group_name 44, -1, 73
	group_name 45, -1, 74
	group_name 46, -1, 75
	group_name 47, -1, 76
	group_name 48, -1, 77
	group_name 49, -1, 78
	group_name 50, -1, 79
	scene_name SuharaHeya, 2, 80
	scene_name SuharaHeya, 6, 80
	group_name 51, -1, 81
	scene_name SuharaGate3, -1, 82
	group_name 52, -1, 83
	scene_name BabiChika2, 1, 84
	scene_name BabiChika2, 2, 84
	scene_name BabiChika2, 3, 84
	group_name 53, -1, 85
	scene_name BabiIriguchi3, -1, 86
	scene_name BabiIriguchi4, -1, 87
	group_name 54, -1, 88
	scene_name RariberoHeya1, 11, 89
	scene_name RariberoHeya1, 12, 89
	group_name 55, -1, 90
	scene_name VinasuHeya2, -1, 91
	group_name 56, -1, 92
	scene_name VinasuChojo, -1, 93
	group_name 57, -1, 94
	group_name 58, -1, 95
	group_name 59, -1, 96
	scene_name LinkLobby, 1, 97
	group_name 61, -1, 98
	group_name 62, -1, 99
	group_name 63, -1, 100
	group_name 64, -1, 101
	group_name 65, -1, 102
	group_name 66, -1, 103
	group_name 67, -1, 104
	group_name 68, -1, 105
	group_name 69, -1, 106
	group_name 70, -1, 107
	.2byte -1, -1	@ 終わり
	.4byte -1
	.2byte 0, 0	@ 終わりの後の 0 の行
	.4byte 0

	.global Party_PairResolveRules
Party_PairResolveRules:
	return_to HaidiaArashi, 15, HaidiaBabi, 16
	return_to HaidiaMura, 16, ShindenHeya, 2
	return_to KuupuappuMura, 1, ShindenHeya, 12
	return_to KuupuappuMura, 3, ShindenHeya, 12
	return_to BiribinoMura1, -1, ShindenHeya, 35
	return_to KorimaMura1, -1, KorimaMura1, 12
	return_to ImiruMura1, -1, ImiruMura2, 7
	return_to ShianJiin1, -1, ShianJiin1, 5
	return_to ShianMura, -1, ShindenHeya, 32
	return_to ArutinMura1, -1, ShindenHeya, 29
	return_to YamaRama2, -1, YamaRama2, 5
	return_to KuupuappuMuraSai, -1, ShindenHeya, 11
	return_to KareiMachi1, -1, KareiHeya2, 2
	return_to RunpaMura1, 3, RunpaSuhara, 11
	return_to TorebiMachi, -1, TorebiHeya, 30
	return_to SuharaMura, -1, SuharaHeya, 6
	return_to RariberoMachi, -1, RariberoHeya1, 12
	return_to LinkLobby, -1, LinkLobby, 1
	.2byte -1, -1, -1, -1	@ 終わり

@ 合うシーンがなければ最後の背景
	.global Battle_BackdropScenes
Battle_BackdropScenes:
	backdrop HaidiaArashiBackdrop, HaidiaArashi, HaidiaMura, HaidiaIe, HaidiaSukureta
	backdrop SoruShindenBackdrop, SoruRoka, SoruIriguchi1, SoruMeiro, SoruNichigetsu, ArutinYama10, TakaraHashira1
	backdrop InteriorBackdrop, KuupuappuHeya
	backdrop GomaDouBackdrop, GomaIriguchi
	backdrop GomaDouBackdrop, GomaIke, GomaHashira, GomaSuiro1, KorimaMura2
	backdrop KorimaMoriBackdrop, KorimaHashi, KorimaHiroba, KorimaKawa, KorimaMagari, KorimaKi
	backdrop ToretoEdaBackdrop, ToretoEda
	backdrop ToretoHeyaBackdrop, ToretoHeya
	backdrop GomaDouBackdrop, BiribinoDou3, BiribinoDou2, BiribinoDou1
	backdrop MakyuriHeyaBackdrop, MakyuriIriguchi, MakyuriHeya1, MakyuriHeya2, MakyuriHeya3, MakyuriHeya4
	backdrop MakyuriChojoBackdrop, MakyuriChojo1
	backdrop ImiruFuchinBackdrop, ImiruFuchin2, ImiruFuchin3, ImiruFuchin4, ImiruFuchin5, ImiruFuchin6, ImiruFuchin7, TakaraShima3
	backdrop MogoruMoriBackdrop, MogoruMori1, MogoruMori2, MogoruMori3
	backdrop ArutinYamaBackdrop, ArutinYama1, ArutinYama2, ArutinYama3, ArutinYama4, ArutinYama5, ArutinYama6, ArutinYama7, ArutinYama8, ArutinYama9, ArutinYama11
	backdrop RamakanSabakuBackdrop, RamakanSabaku1, RamakanSabaku2, RamakanSabaku3
	backdrop FuneKanpanBackdrop, FuneKanpan, FuneHobashira
	backdrop FuneHeyaBackdrop, FuneHeya
	backdrop KorosseoBackdrop, KorosseoKawa, KorosseoKabe, KorosseoMaruta, LinkLobby
	backdrop ArutamiraDouBackdrop, ArutamiraDou1, ArutamiraDou2, ArutamiraDou3, ArutamiraDou4, ArutamiraDou5, ArutamiraDou6
	backdrop ArutamiraDouBackdrop, RunpaJo1, RunpaJo2, RunpaJo3, RunpaJo4
	backdrop SuharaSabakuBackdrop, SuharaSabaku1, SuharaSabaku2
	backdrop TakaraShimaCaveBackdrop, TakaraShima6, TakaraShima7, TakaraShima8, TakaraShima9, TakaraShima10, TakaraShima11, TakaraShima12, TakaraShima13, TakaraShima14
	backdrop TakaraShimaBackdrop, TakaraShima5
	backdrop SuharaGateBackdrop, SuharaGate1, SuharaGate2
	backdrop GomaDouBackdrop, SuharaGate3
	backdrop BabiIriguchiBackdrop, BabiIriguchi3, BabiIriguchi2, BabiIriguchi1, TakaraAshiba3
	backdrop BabiChikaBackdrop, BabiChika1, BabiChika2
	backdrop InteriorBackdrop, RariberoMachi, RariberoHeya1, RariberoHeya2
	backdrop VinasuHeyaBackdrop, VinasuHeya2, VinasuHeya3, VinasuHeya4, VinasuHeya5, VinasuHeya6
	backdrop VinasuChojoBackdrop, VinasuChojo
	backdrop GomaDouBackdrop
	.4byte 0	@ 終わり
