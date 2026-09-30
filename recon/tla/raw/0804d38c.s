.syntax unified
	.thumb
	.global Menu_AppendResourceEntry
	.thumb_func
Menu_AppendResourceEntry:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #232
	ldr r3, [r3]
	mov r10, r0
	mov r8, r3
	mov r2, r8
	adds r2, #142
	movs r1, #0
	ldrsh r7, [r2, r1]
	ldrh r3, [r2]
	cmp r7, #5
	bgt .L_0804d3e0
	adds r3, #1
	strh r3, [r2]
	bl Resource_FindFreeEntry
	mov r1, r10
	adds r6, r0, #0
	lsls r5, r7, #2
	bl Func_0804d344
	lsls r3, r7, #1
	adds r5, r5, r7
	adds r3, r3, r7
	lsls r5, r5, #2
	lsls r3, r3, #3
	add r5, r8
	adds r3, #32
	strh r3, [r5, #12]
	movs r3, #136
	strh r3, [r5, #14]
	adds r3, r7, #0
	adds r3, #132
	mov r1, r10
	mov r2, r8
	strh r6, [r5, #18]
	strb r1, [r2, r3]
.L_0804d3e0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
