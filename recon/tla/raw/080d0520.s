.syntax unified
	.thumb
	.global Func_080d0520
	.thumb_func
Func_080d0520:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #255
	asrs r2, r0, #8
	adds r6, r3, #0
	ands r2, r3
	mov r10, r1
	ands r6, r0
	cmp r2, #4
	bls .L_080d053a
	b .L_080d0718
.L_080d053a:
	lsls r3, r2, #2
	ldr r2, .L_080d0578
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080d0544:
	.4byte .L_080d0558
	.4byte .L_080d0566
	.4byte .L_080d057c
	.4byte .L_080d0628
	.4byte .L_080d069a
.L_080d0558:
	movs r0, #0
	bl Func_08013eb4
	mov r0, r10
	bl Blend_SetDarkenTarget16
	b .L_080d0718
.L_080d0566:
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	bl Func_080d170c
	mov r0, r10
	bl Func_080d17ac
	b .L_080d0718
.L_080d0578:
	.4byte .L_080d0544
.L_080d057c:
	bl Func_080d019c
	adds r5, r0, #0
	movs r0, #165
	lsls r0, r0, #3
	adds r3, r5, r0
	strh r6, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #42
	adds r2, r5, r3
	movs r3, #32
	strh r3, [r2]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #52
	adds r2, r5, r3
	movs r3, #63
	strh r3, [r2]
	movs r3, #128
	ldr r0, .L_080d05dc
	lsls r3, r3, #19
	adds r3, #74
	ldrh r2, [r3]
	mov r8, r0
	ldr r1, .L_080d05e0
	movs r3, #255
	movs r0, #160
	lsls r3, r3, #8
	lsls r0, r0, #3
	ands r3, r2
	adds r0, #54
	adds r2, r5, r0
	orrs r3, r1
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d05e4
	bl Scheduler_AddOrUpdateCallback
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #118
	ldr r0, .L_080d05e8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	b .L_080d05ec
.L_080d05dc:
	.4byte 0x00000020
.L_080d05e0:
	.4byte 0x00000001
.L_080d05e4:
	.4byte Func_080cf78c
.L_080d05e8:
	.4byte Func_080cf6fc
.L_080d05ec:
	bl WaitFrames
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #58
	adds r3, r5, r2
	mov r0, r8
	strb r0, [r3]
	movs r3, #160
	lsls r3, r3, #3
	movs r0, #160
	adds r3, #59
	lsls r0, r0, #3
	adds r2, r5, r3
	adds r0, #60
	movs r3, #64
	strb r3, [r2]
	adds r3, r5, r0
	mov r2, r10
	strb r2, [r3]
	movs r3, #160
	ldr r6, .L_080d0624
	lsls r3, r3, #3
	adds r3, #61
	adds r5, r5, r3
	strb r6, [r5]
	b .L_080d0718
	.2byte 0x0000
.L_080d0624:
	.4byte 0x00000000
.L_080d0628:
	bl Func_080d019c
	adds r5, r0, #0
	movs r0, #165
	lsls r0, r0, #3
	adds r3, r5, r0
	strh r6, [r3]
	ldr r2, .L_080d0674
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #42
	mov r8, r2
	adds r2, r5, r3
	movs r3, #32
	strh r3, [r2]
	movs r0, #0
	bl DisplayTransition_FillTilemapAndSolidTile
	movs r0, #1
	bl WaitFrames
	movs r1, #144
	ldr r0, .L_080d067c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #160
	ldr r6, .L_080d0678
	lsls r0, r0, #3
	adds r0, #58
	adds r3, r5, r0
	strb r6, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #59
	adds r2, r5, r3
	b .L_080d0680
	.2byte 0x0000
.L_080d0674:
	.4byte 0x00000000
.L_080d0678:
	.4byte 0x00000020
.L_080d067c:
	.4byte DisplayTransition_UpdateFrame
.L_080d0680:
	adds r0, #2
	movs r3, #64
	strb r3, [r2]
	adds r3, r5, r0
	mov r2, r10
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #61
	adds r5, r5, r3
	mov r0, r8
	strb r0, [r5]
	b .L_080d0718
.L_080d069a:
	bl Func_080d019c
	movs r3, #136
	lsls r3, r3, #3
	adds r3, #255
	adds r5, r0, #0
	adds r2, r5, r3
	movs r3, #1
	movs r7, #0
	strb r3, [r2]
	cmp r6, #0
	bne .L_080d06dc
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d0720
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080d0724
	movs r0, #1
	movs r1, #0
	bl Func_08013438
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #58
	adds r3, r5, r0
	strb r7, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #59
	adds r2, r5, r3
	adds r0, #2
	b .L_080d0708
.L_080d06dc:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d0728
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080d0724
	movs r0, #1
	movs r1, #0
	bl Func_08013438
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #58
	adds r3, r5, r2
	strb r7, [r3]
	movs r3, #160
	lsls r3, r3, #3
	movs r0, #160
	adds r3, #59
	lsls r0, r0, #3
	adds r2, r5, r3
	adds r0, #60
.L_080d0708:
	movs r3, #80
	strb r3, [r2]
	adds r3, r5, r0
	mov r2, r10
	adds r0, #1
	strb r2, [r3]
	adds r3, r5, r0
	strb r7, [r3]
.L_080d0718:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080d0720:
	.4byte Func_080d0788
.L_080d0724:
	.4byte Func_080d0954
.L_080d0728:
	.4byte Func_080d085c
