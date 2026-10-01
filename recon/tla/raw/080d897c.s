.syntax unified
	.thumb
	.global Func_080d897c
	.thumb_func
Func_080d897c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r1, #136
	lsls r1, r1, #5
	adds r1, #12
	movs r0, #156
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #128
	lsls r3, r3, #1
	mov r8, r3
	adds r5, r0, #0
	mov r0, r8
	bl Runtime_BumpAllocate
	movs r3, #0
	adds r6, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_080d89f0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Resource_FindFreeEntry
	adds r1, r6, #0
	str r0, [r5, #8]
	ldr r0, .L_080d89f4
	bl Resource_DecodeType01
	adds r2, r6, #0
	mov r1, r8
	ldr r0, [r5, #8]
	bl VramBlock_LoadCached
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d89f8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #228
	lsls r1, r1, #2
	adds r1, #255
	ldr r0, .L_080d89fc
	bl Scheduler_AddOrUpdateCallback
	adds r0, r6, #0
	bl Sys_Free
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_080d89f0:
	.4byte 0x85000443
.L_080d89f4:
	.4byte Data_080f0c04
.L_080d89f8:
	.4byte Func_080d8a00
.L_080d89fc:
	.4byte Func_080d8c10
