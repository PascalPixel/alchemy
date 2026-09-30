.syntax unified
	.thumb
	.global Func_080455dc
	.thumb_func
Func_080455dc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r1, #0
	movs r1, #193
	mov r8, r0
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlock
	adds r6, r0, #0
	ldr r0, .L_08045628
	bl Resource_GetTableEntry
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	lsls r5, r5, #1
	adds r2, r6, r3
	ldrh r3, [r5, r0]
	adds r1, r6, #0
	adds r0, r0, r3
	str r0, [r2]
	bl Func_0801591c
	adds r1, r6, #0
	mov r0, r8
	bl Resource_GetBuffer
	adds r5, r0, #0
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	adds r0, r5, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08045628:
	.4byte 0x000001d7
