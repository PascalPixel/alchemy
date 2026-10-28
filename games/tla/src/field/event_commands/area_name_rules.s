	.section .rodata
@ Area names: map scene/group and entrance (-1 matches any) to an area name ID.
	.macro scene_name scene:req, entrance:req, name:req
	.2byte \scene, (\entrance & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro group_name group:req, entrance:req, name:req
	.2byte \group, \entrance & 0x7fff
	.4byte \name
	.endm
	.global gScene_AreaNameRules
gScene_AreaNameRules:
	group_name 0, -1, 0
	group_name 1, -1, 1
	group_name 2, -1, 2
	group_name 3, -1, 3
	group_name 4, -1, 4
	group_name 5, -1, 5
	group_name 6, -1, 6
	scene_name 11, 7, 7
	group_name 7, -1, 8
	group_name 8, -1, 9
	group_name 9, -1, 9
	group_name 10, -1, 10
	group_name 11, -1, 11
	group_name 14, -1, 12
	group_name 15, -1, 13
	group_name 16, -1, 14
	group_name 17, -1, 15
	scene_name 57, 5, 16
	scene_name 58, 4, 17
	scene_name 58, 5, 18
	group_name 18, -1, 19
	group_name 19, -1, 20
	group_name 20, -1, 21
	group_name 21, -1, 22
	group_name 22, -1, 23
	group_name 23, -1, 23
	group_name 24, -1, 24
	group_name 25, -1, 25
	group_name 26, -1, 26
	group_name 27, -1, 27
	group_name 28, -1, 28
	group_name 29, -1, 29
	group_name 30, -1, 30
	group_name 31, -1, 30
	group_name 32, -1, 31
	group_name 33, -1, 32
	group_name 12, -1, 33
	group_name 13, -1, 34
	group_name 34, -1, 35
	group_name 35, -1, 36
	group_name 36, -1, 37
	group_name 37, -1, 38
	group_name 38, -1, 39
	group_name 40, -1, 40
	group_name 41, -1, 41
	group_name 42, -1, 41
	scene_name 156, 6, 42
	group_name 43, -1, 43
	group_name 44, -1, 44
	group_name 49, -1, 45
	group_name 50, -1, 45
	group_name 45, -1, 46
	group_name 46, -1, 46
	group_name 47, -1, 47
	group_name 48, -1, 48
	scene_name 183, 8, 49
	scene_name 183, 9, 50
	group_name 51, -1, 51
	group_name 53, -1, 52
	group_name 52, -1, 53
	scene_name 201, 1, 54
	group_name 54, -1, 55
	group_name 55, -1, 55
	group_name 56, -1, 56
	group_name 60, -1, 57
	group_name 57, -1, 58
	group_name 58, -1, 59
	group_name 59, -1, 60
	group_name 61, -1, 61
	group_name 62, -1, 61
	group_name 63, -1, 62
	group_name 66, -1, 63
	group_name 67, -1, 64
	group_name 68, -1, 65
	group_name 69, -1, 65
	scene_name 237, 21, 66
	group_name 64, -1, 67
	group_name 65, -1, 68
	group_name 70, -1, 69
	group_name 71, -1, 70
	group_name 72, -1, 71
	group_name 73, -1, 72
	group_name 74, -1, 73
	group_name 75, -1, 74
	group_name 77, -1, 75
	group_name 76, -1, 76
	group_name 39, -1, 77
	group_name 78, -1, 78
	group_name 79, -1, 79
	group_name 80, -1, 80
	group_name 81, -1, 81
	group_name 82, -1, 82
	group_name 83, -1, 83
	group_name 84, -1, 84
	group_name 85, -1, 85
	group_name 86, -1, 86
	group_name 87, -1, 87
	group_name 88, -1, 88
	group_name 89, -1, 89
	group_name 90, -1, 90
	group_name 91, -1, 91
	group_name 92, -1, 92
	scene_name 321, 96, 93
	scene_name 321, 97, 93
	scene_name 321, 98, 93
	scene_name 321, 99, 93
	group_name 93, -1, 94
	group_name 94, -1, 95
	group_name 95, -1, 96
	group_name 96, -1, 97
	.2byte 0xffff, 0xffff, 0xffff, 0xffff	@ End.
