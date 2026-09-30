.syntax unified
	.thumb
	.global UiWork_Initialize
	.thumb_func
UiWork_Initialize:
	push {r5, r6, lr}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	movs r6, #152
	lsls r6, r6, #5
	adds r6, #144
	mov r8, r0
	adds r1, r6, #0
	movs r0, #60
	bl Runtime_AllocateBlock
	ldr r3, .L_080390a8
	adds r1, r6, #0
	adds r5, r0, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #70
	adds r2, r5, r3
	movs r1, #1
	movs r3, #99
	strb r1, [r5, #3]
	adds r0, r5, #0
	strh r3, [r2]
	strb r1, [r5, #5]
	movs r3, #15
	movs r1, #160
	ldr r2, .L_080390ac
	strb r3, [r5, #7]
	lsls r1, r1, #3
	ldr r3, .L_080390b0
	adds r0, #8
	mov lr, r3
	.2byte 0xf800
	bl Func_08038f08
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_080390b4
	bl Scheduler_AddOrUpdateCallback
	mov r0, r8
	bl Func_0803a4b0
	movs r0, #240
	lsls r0, r0, #8
	mov r9, sp
	movs r1, #128
	adds r0, #19
	bl Func_08038fa8
	movs r0, #240
	lsls r0, r0, #8
	mov r9, sp
	movs r1, #129
	adds r0, #20
	bl Func_08038fa8
	movs r0, #240
	lsls r0, r0, #8
	movs r1, #130
	adds r0, #21
	mov r9, sp
	bl Func_08038fa8
	movs r1, #224
	lsls r1, r1, #4
	adds r1, #58
	movs r2, #4
	movs r3, #2
	adds r5, r5, r1
.L_08039096:
	subs r3, #1
	strb r2, [r5]
	subs r5, #1
	cmp r3, #0
	bge .L_08039096
	pop {r3, r5}
	mov r8, r3
	mov r9, r5
	pop {r5, r6, pc}
.L_080390a8:
	.4byte IwramClearWords
.L_080390ac:
	.4byte 0xf000f000
.L_080390b0:
	.4byte IwramFillWords
.L_080390b4:
	.4byte Func_080390b8
