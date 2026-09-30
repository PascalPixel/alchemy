.syntax unified
	.thumb
	.global Func_080d1cc0
	.thumb_func
Func_080d1cc0:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r1, #28
	movs r0, #144
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r1, #128
	adds r6, r0, #0
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateHeapBlock
	movs r3, #0
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r4, #0
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #2
	adds r2, r4, #0
	movs r0, #94
	bl VramBlock_LoadCached
	movs r1, #144
	ldr r0, .L_080d1d2c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	cmp r5, #0
	bne .L_080d1d24
	ldr r3, .L_080d1d30
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl ObjectTable_Get
	adds r5, r0, #0
.L_080d1d24:
	str r5, [r6, #24]
	add sp, #4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080d1d2c:
	.4byte Func_080d1ae4
.L_080d1d30:
	.4byte gPartyState
