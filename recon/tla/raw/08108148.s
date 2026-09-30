.syntax unified
	.thumb
	.global Func_08108148
	.thumb_func
Func_08108148:
	push {r5, lr}
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #236
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlock
	adds r5, r0, #0
	bl Func_080c84d8
	movs r3, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_0810822c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #5
	adds r2, r5, r3
	movs r3, #12
	strb r3, [r2]
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #202
	adds r0, r5, r2
	bl Party_ListActiveOwnersFar
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #4
	adds r3, r5, r2
	strb r0, [r3]
	bl Func_080f8048
	bl Resource_FindFreeEntry
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #236
	adds r3, r5, r2
	strh r0, [r3]
	ldr r2, .L_08108230
	movs r1, #128
	bl VramBlock_LoadResource
	bl Resource_FindFreeEntry
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #238
	adds r3, r5, r2
	strh r0, [r3]
	ldr r2, .L_08108234
	movs r1, #128
	bl VramBlock_LoadResource
	bl Resource_FindFreeEntry
	movs r2, #158
	lsls r2, r2, #3
	adds r3, r5, r2
	strh r0, [r3]
	ldr r2, .L_08108238
	movs r1, #128
	bl VramBlock_LoadResource
	bl Resource_FindFreeEntry
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #242
	adds r3, r5, r2
	strh r0, [r3]
	ldr r2, .L_0810823c
	movs r1, #128
	bl VramBlock_LoadResource
	bl Resource_FindFreeEntry
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #246
	adds r3, r5, r2
	strh r0, [r3]
	ldr r2, .L_08108240
	movs r1, #128
	bl VramBlock_LoadResource
	bl Resource_FindFreeEntry
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #244
	adds r5, r5, r3
	ldr r2, .L_08108244
	strh r0, [r5]
	movs r1, #128
	bl VramBlock_LoadResource
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_08108248
	bl Func_080145a8
	bl Func_0810bdf4
	add sp, #4
	pop {r5, pc}
.L_0810822c:
	.4byte 0x8500033b
.L_08108230:
	.4byte 0x000001fa
.L_08108234:
	.4byte 0x000001fe
.L_08108238:
	.4byte 0x000001ff
.L_0810823c:
	.4byte 0x000001fb
.L_08108240:
	.4byte 0x000001fc
.L_08108244:
	.4byte 0x000001fd
.L_08108248:
	.4byte Func_08108130
