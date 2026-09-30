.syntax unified
	.thumb
	.global Func_08104a58
	.thumb_func
Func_08104a58:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	bl Resource_FindFreeEntry
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #238
	movs r6, #1
	adds r3, r5, r2
	negs r6, r6
	strh r0, [r3]
	cmp r0, r6
	beq .L_08104a80
	ldr r2, .L_08104a9c
	movs r1, #128
	bl VramBlock_LoadResourceFar
.L_08104a80:
	bl Resource_FindFreeEntry
	movs r2, #158
	lsls r2, r2, #3
	adds r3, r5, r2
	strh r0, [r3]
	cmp r0, r6
	beq .L_08104a98
	ldr r2, .L_08104aa0
	movs r1, #128
	bl VramBlock_LoadResourceFar
.L_08104a98:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08104a9c:
	.4byte 0x00000200
.L_08104aa0:
	.4byte 0x00000201
