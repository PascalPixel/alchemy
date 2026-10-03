.syntax unified
	.thumb
	.balign 4
	.global UiWork_InitializeWithResourceCounters
	.thumb_func
UiWork_InitializeWithResourceCounters:
	push {r5, r6, lr}
	movs r6, #152
	lsls r6, r6, #5
	adds r6, #144
	adds r1, r6, #0
	movs r0, #60
	bl Runtime_AllocateBlock
	adds r1, r6, #0
	ldr r3, .L_08038f98
	adds r5, r0, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #1
	strb r3, [r5, #3]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #70
	adds r2, r5, r3
	movs r3, #99
	strh r3, [r2]
	movs r3, #15
	strb r3, [r5, #7]
	movs r1, #160
	adds r5, #8
	ldr r3, .L_08038f9c
	ldr r2, .L_08038fa0
	lsls r1, r1, #3
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	bl UiWork_InitFreeList
	bl Func_0803d2d0
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08038fa4
	bl Scheduler_AddOrUpdateCallback
	bl Func_0803a448
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08038f98:
	.4byte IwramClearWords
.L_08038f9c:
	.4byte IwramFillWords
.L_08038fa0:
	.4byte 0xf000f000
.L_08038fa4:
	.4byte Func_080390b8
