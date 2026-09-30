.syntax unified
	.thumb
	.global Func_080d9b08
	.thumb_func
Func_080d9b08:
	push {r5, r6, lr}
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #236
	movs r0, #160
	sub sp, #4
	bl Runtime_AllocateBlock
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	movs r3, #0
	adds r6, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_080d9b70
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Resource_FindFreeEntry
	adds r1, r6, #0
	str r0, [r5, #8]
	ldr r0, .L_080d9b74
	bl Func_0801587c
	adds r2, r6, #0
	ldr r0, [r5, #8]
	movs r1, #64
	bl VramBlock_LoadCached
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d9b78
	bl Func_080145a8
	movs r1, #228
	lsls r1, r1, #2
	adds r1, #255
	ldr r0, .L_080d9b7c
	bl Func_080145a8
	adds r0, r6, #0
	bl Sys_Free
	add sp, #4
	pop {r5, r6, pc}
.L_080d9b70:
	.4byte 0x8500043b
.L_080d9b74:
	.4byte Data_080f0c04
.L_080d9b78:
	.4byte Func_080d9b80
.L_080d9b7c:
	.4byte Func_080d9d40
