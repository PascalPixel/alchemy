.syntax unified
	.thumb
	.global Func_0802cb08
	.thumb_func
Func_0802cb08:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r2, .L_0802cb50
	ldr r3, .L_0802cb54
	movs r0, #1
	str r3, [r2]
	movs r3, #130
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #0
	strh r3, [r2]
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #159
	strh r3, [r2]
	bl WaitFrames
	ldr r0, .L_0802cb58
	bl Resource_GetTableEntry
	ldr r1, .L_0802cb5c
	bl Func_0801587c
	bl Func_0802c4d8
	ldr r0, .L_0802cb60
	bl Func_0801475c
	movs r0, #1
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
.L_0802cb50:
	.4byte Data_030011f8
.L_0802cb54:
	.4byte Func_0802c98c
.L_0802cb58:
	.4byte 0x00000198
.L_0802cb5c:
	.4byte gMapCellBuffer
.L_0802cb60:
	.4byte Func_0802cb64
