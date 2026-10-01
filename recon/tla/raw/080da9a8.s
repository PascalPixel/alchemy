.syntax unified
	.thumb
	.global Func_080da9a8
	.thumb_func
Func_080da9a8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #228
	adds r5, r0, #0
	movs r0, #164
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #128
	lsls r3, r3, #3
	mov r8, r3
	adds r7, r0, #0
	mov r0, r8
	bl Runtime_BumpAllocate
	movs r3, #0
	adds r6, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r7, #0
	ldr r2, .L_080daa88
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Resource_FindFreeEntry
	movs r3, #186
	lsls r3, r3, #1
	str r0, [r7]
	cmp r5, r3
	bne .L_080daa28
	adds r1, r6, #0
	ldr r0, .L_080daa8c
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #1
	adds r1, r6, r3
	ldr r0, .L_080daa90
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #2
	adds r1, r6, r3
	ldr r0, .L_080daa94
	bl Resource_DecodeType01
	movs r3, #192
	lsls r3, r3, #2
	adds r1, r6, r3
	ldr r0, .L_080daa98
	bl Resource_DecodeType01
	ldr r0, [r7]
	mov r1, r8
	adds r2, r6, #0
	bl VramBlock_LoadCached
	b .L_080daa60
.L_080daa28:
	adds r1, r6, #0
	ldr r0, .L_080daa9c
	bl Resource_DecodeType01
	ldr r5, .L_080daaa0
	movs r3, #128
	lsls r3, r3, #1
	adds r1, r6, r3
	adds r0, r5, #0
	bl Resource_DecodeType01
	movs r3, #128
	lsls r3, r3, #2
	adds r1, r6, r3
	adds r0, r5, #0
	bl Resource_DecodeType01
	movs r3, #192
	lsls r3, r3, #2
	adds r1, r6, r3
	ldr r0, .L_080daaa4
	bl Resource_DecodeType01
	ldr r0, [r7]
	mov r1, r8
	adds r2, r6, #0
	bl VramBlock_LoadCached
.L_080daa60:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080daaa8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #228
	lsls r1, r1, #2
	adds r1, #255
	ldr r0, .L_080daaac
	bl Scheduler_AddOrUpdateCallback
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r7, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080daa88:
	.4byte 0x85000439
.L_080daa8c:
	.4byte Data_080f0c1c
.L_080daa90:
	.4byte Data_080f0c63
.L_080daa94:
	.4byte Data_080f0cb8
.L_080daa98:
	.4byte Data_080f0d07
.L_080daa9c:
	.4byte Data_080f0d5e
.L_080daaa0:
	.4byte Data_080f0dbb
.L_080daaa4:
	.4byte Data_080f0e00
.L_080daaa8:
	.4byte Func_080daab0
.L_080daaac:
	.4byte Func_080dacb8
