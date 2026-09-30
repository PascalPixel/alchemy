.syntax unified
	.thumb
	.global Menu_LoadResourceSlot
	.thumb_func
Menu_LoadResourceSlot:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r3, #128
	lsls r3, r3, #3
	mov r8, r3
	mov r10, r0
	mov r0, r8
	adds r5, r1, #0
	bl Runtime_BumpAllocate
	adds r6, r0, #0
	ldr r0, .L_0804d388
	bl Resource_GetTableEntry
	lsls r5, r5, #1
	ldrh r3, [r5, r0]
	adds r1, r6, #0
	adds r0, r0, r3
	bl Func_0801591c
	mov r0, r10
	mov r1, r8
	adds r2, r6, #0
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0804d388:
	.4byte 0x000001d7
