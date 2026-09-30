.syntax unified
	.thumb
	.global Func_08104acc
	.thumb_func
Func_08104acc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r1, #0
	movs r1, #193
	mov r8, r0
	lsls r1, r1, #3
	movs r0, #68
	sub sp, #12
	bl Runtime_AllocateBlock
	movs r1, #1
	add r2, sp, #8
	add r3, sp, #4
	str r1, [sp, #0]
	adds r5, r0, #0
	movs r1, #0
	mov r0, r8
	str r6, [sp, #8]
	bl Ability_LoadGlyphFar
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r1, r5, #0
	adds r0, r6, #0
	bl Resource_GetBuffer
	adds r5, r0, #0
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	adds r0, r5, #0
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
