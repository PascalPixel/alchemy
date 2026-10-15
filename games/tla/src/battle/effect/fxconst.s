	.section .rodata
@ The constants the effect code copies into its locals, placed after its code
@ as The Broken Seal's effect/effect.c places them: {x, y} scales and
@ speeds in 16.16 fixed point, points and offsets.
@ read by Func_0813fd84
	.global Data_08196dd8
Data_08196dd8:
	.4byte 0x10000, 0x10000
@ read by Func_0814153c
	.global Data_08196de0
Data_08196de0:
	.4byte 0x10000, 0x10000, 0x55555556, 0x55555556
	.4byte 0x55555556, 0x55555556
@ read by Func_08146e00
	.global Data_08196df8
Data_08196df8:
	.4byte 0x10000, 0x10000, 0x71c71c8, 0x2d82d83
	.4byte 0x2d82d83
@ read by Func_0814bee0
	.global Data_08196e0c
Data_08196e0c:
	.4byte 0x10000, 0x10000
@ read by Func_0814db04
	.global Data_08196e14
Data_08196e14:
	.4byte 0x10000, 0x10000
@ The Broken Seal's RockWall_Heights; read by Func_08150c74
	.global Data_08196e1c
Data_08196e1c:
	.4byte -32, -32, -32, -32
@ The Broken Seal's Mode12_Points; read by Func_08158dc8
	.global Data_08196e2c
Data_08196e2c:
	.2byte 8, 135, 184, 135, 128, 135, 96, 32
	.2byte 96, 32, 64, 135
@ read by Func_0815a9e4
	.global Data_08196e44
Data_08196e44:
	.4byte 0x10000, 0x10000
@ read by Func_0815b68c
	.global Data_08196e4c
Data_08196e4c:
	.4byte 0x10000, 0x10000
@ read by Func_0815b764
	.global Data_08196e54
Data_08196e54:
	.4byte 0x10000, 0x10000
@ read by Func_0816442c
	.global Data_08196e5c
Data_08196e5c:
	.4byte 0x10000, 0x10000
@ The Broken Seal's Formation_SmallScale; read by Func_0816442c
	.global Data_08196e64
Data_08196e64:
	.4byte 0x14000, 0x14000
@ read by Func_08165ab4
	.global Data_08196e6c
Data_08196e6c:
	.4byte 0x10000, 0x10000
@ read by Func_0816729c
	.global Data_08196e74
Data_08196e74:
	.4byte 0x10000, 0x10000
@ read by Func_08168a40
	.global Data_08196e7c
Data_08196e7c:
	.4byte 0x10000, 0x10000
@ read by Func_08168a40
	.global Data_08196e84
Data_08196e84:
	.4byte 0x10000, 0x10000
@ The Broken Seal's BattleFx6_UnitScale; read by Func_081693d0
	.global Data_08196e8c
Data_08196e8c:
	.4byte 0x18000, 0x18000
@ The Broken Seal's BattleFx6_UnitScale; read by Func_08170364
	.global Data_08196e94
Data_08196e94:
	.4byte 0x18000, 0x18000
@ read by Func_08170dac
	.global Data_08196e9c
Data_08196e9c:
	.byte 40, -40, 0, 40, -40, -40, 40, 40, 0, -40, -40, 40
@ read by Func_08175f74
	.global Data_08196ea8
Data_08196ea8:
	.4byte 0x10000, 0x10000
@ read by Func_08175f74
	.global Data_08196eb0
Data_08196eb0:
	.4byte 0x10000, 0x10000
@ read by Func_08175f74
	.global Data_08196eb8
Data_08196eb8:
	.4byte 0x10000, 0x10000
@ read by Func_08175f74
	.global Data_08196ec0
Data_08196ec0:
	.4byte 0x10000, 0x10000
@ read by Func_08179f18
	.global Data_08196ec8
Data_08196ec8:
	.4byte 0x10000, 0x10000
@ read by Func_0817ea58
	.global Data_08196ed0
Data_08196ed0:
	.4byte 0x10000, 0x10000
@ read by Func_0817ea58
	.global Data_08196ed8
Data_08196ed8:
	.4byte 0x10000, 0x10000
@ read by Func_0817ea58
	.global Data_08196ee0
Data_08196ee0:
	.4byte 0x11999, 0x11999
@ read by Func_08180c94
	.global Data_08196ee8
Data_08196ee8:
	.4byte 0x20000, 0x20000
@ read by Func_08182aa8
	.global Data_08196ef0
Data_08196ef0:
	.4byte 0x10000, 0x10000
@ read by Func_081848ec
	.global Data_08196ef8
Data_08196ef8:
	.4byte 0x10000, 0x10000
@ read by Func_08186dfc
	.global Data_08196f00
Data_08196f00:
	.4byte 0x10000, 0x10000
@ read by Func_08186dfc
	.global Data_08196f08
Data_08196f08:
	.4byte 0x10000, 0x10000
@ read by Func_08186dfc
	.global Data_08196f10
Data_08196f10:
	.4byte 0x10000, 0x10000
@ read by Func_0818cb74
	.global Data_08196f18
Data_08196f18:
	.4byte 0x10000, 0x10000
@ read by Func_0818df5c
	.global Data_08196f20
Data_08196f20:
	.4byte 0x20000, 0x20000
@ read by Func_08191d60
	.global Data_08196f28
Data_08196f28:
	.byte 2, 3, 5, 8, 7, 6, 4, 0
@ read by Func_0819273c
	.global Data_08196f30
Data_08196f30:
	.4byte 0xf0f0, 0xf010, 3312, 3088
	.4byte 240, 16
@ read by Func_08192c9c
	.global Data_08196f48
Data_08196f48:
	.4byte 0x10000, 0x10000
@ read by Func_08192c9c
	.global Data_08196f50
Data_08196f50:
	.4byte 2, 3, 4, 5
	.4byte 6
@ read by Func_08192c9c
	.global Data_08196f64
Data_08196f64:
	.4byte 2, 3, 4, 5
	.4byte 6, 7, 8
@ read by Func_08192c9c
	.global Data_08196f80
Data_08196f80:
	.4byte 8, 7, 6, 5
	.4byte 4, 3, 2
@ read by Func_08192c9c
	.global Data_08196f9c
Data_08196f9c:
	.4byte 112, 27, 104, 28
	.4byte 92, 27, 74, 27
@ read by Func_08192c9c
	.global Data_08196fbc
Data_08196fbc:
	.4byte 196, 44, 176, 48
	.4byte 152, 45, 116, 45
