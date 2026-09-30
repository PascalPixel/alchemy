.syntax unified
	.thumb
	.global Func_0802a650
	.thumb_func
Func_0802a650:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r3, .L_0802a6b0
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r0, #128
	lsls r5, r5, #2
	lsls r0, r0, #2
	mov r10, r1
	adds r5, r5, r3
	bl Runtime_BumpAllocate
	movs r3, #160
	lsls r3, r3, #19
	adds r6, r0, #0
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldrh r0, [r5, #2]
	ldr r3, .L_0802a6b4
	mov r8, r1
	adds r0, r0, r3
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_0801587c
	mov r3, r8
	strh r3, [r6]
	movs r2, #132
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r6, #0
	mov r1, r10
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	bl Sys_Free
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0802a6b0:
	.4byte Data_0802f380
.L_0802a6b4:
	.4byte 0x0000026c
