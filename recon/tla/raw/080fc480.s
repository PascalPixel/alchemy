.syntax unified
	.thumb
	.global Func_080fc480
	.thumb_func
Func_080fc480:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	bl Resource_FindFreeEntry
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #238
	adds r3, r5, r2
	strh r0, [r3]
	ldr r2, .L_080fc4b8
	movs r1, #128
	bl VramBlock_LoadResourceFar
	bl Resource_FindFreeEntry
	movs r3, #158
	lsls r3, r3, #3
	adds r5, r5, r3
	strh r0, [r5]
	ldr r2, .L_080fc4bc
	movs r1, #128
	bl VramBlock_LoadResourceFar
	pop {r5, pc}
	.2byte 0x0000
.L_080fc4b8:
	.4byte 0x000001fd
.L_080fc4bc:
	.4byte 0x000001fc
