.syntax unified
	.thumb
	.global Func_080d1a18
	.thumb_func
Func_080d1a18:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	movs r1, #28
	movs r0, #144
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r1, #128
	adds r7, r0, #0
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateHeapBlock
	ldr r3, .L_080d1ab8
	mov r5, sp
	str r3, [r5]
	movs r2, #133
	movs r3, #128
	adds r4, r0, #0
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	adds r1, r4, #0
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	adds r2, r4, #0
	lsls r1, r1, #2
	movs r0, #94
	bl VramBlock_LoadCached
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d1abc
	bl Scheduler_AddOrUpdateCallback
	movs r2, #252
	movs r3, #128
	lsls r2, r2, #6
	lsls r3, r3, #19
	adds r2, #158
	adds r3, #80
	strh r2, [r3]
	movs r2, #16
	adds r3, #2
	strh r2, [r3]
	movs r2, #31
	adds r3, #2
	strh r2, [r3]
	movs r3, #0
	str r3, [r5]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	adds r1, r7, #0
	adds r2, #7
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	cmp r6, #0
	bne .L_080d1ab0
	ldr r3, .L_080d1ac0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl ObjectTable_Get
	adds r6, r0, #0
.L_080d1ab0:
	str r6, [r7, #24]
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d1ab8:
	.4byte 0x11111111
.L_080d1abc:
	.4byte Func_080d1840
.L_080d1ac0:
	.4byte gPartyState
