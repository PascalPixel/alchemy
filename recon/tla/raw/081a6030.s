.syntax unified
	.thumb
	.global Func_081a6030
	.thumb_func
Func_081a6030:
	push {r5, lr}
	ldr r5, .L_081a606c
	movs r3, #128
	lsls r3, r3, #19
	strh r5, [r3]
	ldr r0, .L_081a6070
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r4, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #160
	lsls r3, r3, #19
	strh r5, [r3]
	ldr r5, .L_081a6074
	movs r3, #128
	lsls r3, r3, #2
	adds r4, r4, r3
	adds r1, r5, #0
	adds r0, r4, #0
	b .L_081a6078
	.2byte 0x0000
.L_081a606c:
	.4byte 0x00000000
.L_081a6070:
	.4byte 0x00000018
.L_081a6074:
	.4byte gMapCellBuffer
.L_081a6078:
	bl Func_0801591c
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	ldr r2, .L_081a6090
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r5, pc}
.L_081a6090:
	.4byte 0x84002700
