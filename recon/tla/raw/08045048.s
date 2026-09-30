.syntax unified
	.thumb
	.global RenderResource_LoadFrame
	.thumb_func
RenderResource_LoadFrame:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #128
	lsls r3, r3, #3
	mov r10, r3
	adds r6, r0, #0
	adds r7, r1, #0
	movs r0, #56
	mov r1, r10
	mov r8, r2
	bl Runtime_AllocateBlock
	adds r5, r0, #0
	ldr r0, .L_080450a8
	bl Resource_GetTableEntry
	adds r2, r0, #0
	cmp r7, #95
	bgt .L_0804509e
	lsls r3, r6, #1
	ldrh r0, [r3, r2]
	adds r1, r5, #0
	adds r0, r2, r0
	bl Func_0801591c
	mov r3, r8
	cmp r3, #0
	beq .L_0804508e
	movs r1, #192
	lsls r1, r1, #2
	adds r0, r5, #0
	bl Func_080202d0
.L_0804508e:
	adds r0, r7, #0
	mov r1, r10
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
.L_0804509e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080450a8:
	.4byte 0x000001d7
