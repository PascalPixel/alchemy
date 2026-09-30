.syntax unified
	.thumb
	.global Func_08149ba0
	.thumb_func
Func_08149ba0:
	push {lr}
	movs r1, #1
	bl BattleFx_RunSparkGroups
	pop {pc}
	.2byte 0x0000
