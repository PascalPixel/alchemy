.syntax unified
	.thumb
	.global Func_080e15fc
	.thumb_func
Func_080e15fc:
	push {r5, r6, lr}
	movs r1, #172
	movs r0, #248
	bl Runtime_AllocateHeapBlock
	adds r6, r0, #0
	bl Resource_FindFreeEntry
	adds r3, r6, #0
	adds r3, #164
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	lsls r1, r1, #1
	ldr r2, .L_080e1648
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r3, r6, #0
	movs r2, #186
	movs r5, #0
	adds r3, #166
	lsls r2, r2, #2
	strh r5, [r3]
	adds r2, #255
	subs r3, #6
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r1, #144
	adds r3, #6
	str r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_080e164c
	bl Func_080145a8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080e1648:
	.4byte Data_080ed80c
.L_080e164c:
	.4byte Func_080e150c
