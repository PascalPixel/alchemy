.syntax unified
	.thumb
	.global RenderResource_LoadPair
	.thumb_func
RenderResource_LoadPair:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	movs r1, #128
	adds r5, r0, #0
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	ldr r3, .L_08044f84
	lsls r5, r5, #2
	adds r6, r0, #0
	ldr r0, [r3, r5]
	cmp r7, #95
	bgt .L_08044f80
	adds r1, r6, #0
	bl Func_0801591c
	movs r1, #128
	lsls r1, r1, #2
	adds r0, r7, #0
	adds r2, r6, #0
	bl VramBlock_LoadCached
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
.L_08044f80:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08044f84:
	.4byte RenderResource_PairSourceTable
