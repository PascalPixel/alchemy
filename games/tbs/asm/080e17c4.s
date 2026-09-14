@ 複数領域関数の継続部。
@ 間接演算呼出し後のループ末尾までを同一ファイルに保持する。
.syntax unified
	.thumb
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_08004cf0, 0x08004cf0
	.set sub_08005258, 0x08005258
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080d40ec, 0x080d40ec
	.set sub_080e1a48, 0x080e1a48
	.set sub_080e1a64, 0x080e1a64
	.set sub_080e1cc2, 0x080e1cc2
	.set sub_080e3944, 0x080e3944
	.global Continuation_080e17c4
Continuation_080e17c4:
	ldr r5, [sp, #128]
	ldr r0, [pc, #108]
	movs r3, #75
	adds r2, r5, r0
	str r3, [r2, #0]
	movs r2, #152
	lsls r2, r2, #1
	add r2, sp
	adds r5, r2, #0
	str r2, [sp, #48]
	ldr r2, [pc, #92]
	movs r1, #0
	add r2, sp
	mov r9, r1
	movs r6, #63
	adds r0, r5, #0
	movs r1, #3
	adds r4, r2, #0
.L_080e17e8:
	strb r1, [r0, #0]
	strb r1, [r4, #0]
	ldrb r3, [r0, #0]
	subs r4, #1
	cmp r3, #63
	bls .L_080e17f6
	strb r6, [r5, #0]
.L_080e17f6:
	ldrb r3, [r2, #0]
	cmp r3, #63
	bls .L_080e17fe
	strb r6, [r2, #0]
.L_080e17fe:
	movs r3, #1
	add r9, r3
	mov r3, r9
	adds r5, #1
	adds r0, #1
	subs r2, #1
	adds r1, #8
	cmp r3, #16
	bne .L_080e17e8
	ldr r2, [pc, #40]
	ldr r3, [pc, #28]
	mov r5, sp
	ldr r0, [pc, #40]
	ldr r1, [pc, #40]
	movs r4, #0
	adds r5, #136
	strh r3, [r2, #0]
	str r4, [sp, #116]
	str r5, [sp, #96]
	str r0, [sp, #28]
	str r4, [sp, #24]
	str r4, [sp, #20]
	str r1, [sp, #16]
	b sub_080e1cc2
	.2byte 0x0000
	.4byte 0x00000784
	.4byte 0x00007784
	.4byte 0x0000014f
	.4byte 0x0400000c
	.4byte 0xfffff460
	.4byte 0xfff26c00
	ldr r2, [pc, #588]
	ldr r0, [sp, #20]
	ldr r1, [pc, #588]
	movs r3, #0
	bl sub_080d40ec
	ldr r2, [sp, #116]
	cmp r2, #150
	bne .L_080e1870
	ldr r3, [sp, #128]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2, #0]
	ldr r5, [sp, #128]
	ldr r0, [pc, #564]
	ldr r3, [pc, #568]
	adds r2, r5, r0
	b .L_080e1882
.L_080e1870:
	ldr r1, [sp, #128]
	movs r2, #239
	lsls r2, r2, #7
	ldr r4, [sp, #104]
	ldr r5, [pc, #548]
	adds r3, r1, r2
	str r4, [r3, #0]
	adds r2, r1, r5
	movs r3, #75
.L_080e1882:
	str r3, [r2, #0]
	movs r0, #255
	movs r1, #192
	ldr r3, [pc, #540]
	lsls r1, r1, #8
	lsls r0, r0, #17
	bl sub_080072f0
	adds r1, r0, #0
	movs r0, #255
	lsls r0, r0, #17
	ldr r2, [pc, #528]
	bl sub_08005258
	bl sub_080049ac
	ldr r0, [sp, #116]
	cmp r0, #128
	ble .L_080e1922
	adds r6, r0, #0
	subs r6, #128
	cmp r6, #22
	ble .L_080e18b2
	movs r6, #20
.L_080e18b2:
	add r0, sp, #148
	movs r3, #0
	str r3, [r0, #0]
	negs r3, r6
	lsls r3, r3, #17
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r0, #8]
	asrs r3, r6, #2
	adds r3, #2
	str r3, [sp, #104]
	cmp r3, #8
	ble .L_080e18d2
	movs r1, #8
	str r1, [sp, #104]
.L_080e18d2:
	movs r5, #128
	bl sub_08004cb4
	lsls r5, r5, #8
	ldr r0, [pc, #468]
	bl sub_08004bd4
	adds r0, r5, #0
	bl sub_08004c6c
	adds r0, r5, #0
	bl sub_08004bd4
	lsls r0, r6, #12
	bl sub_08004c1c
	ldr r2, [sp, #116]
	cmp r2, #150
	ble .L_080e1908
	ldr r0, [sp, #96]
	ldr r3, [sp, #16]
	movs r4, #5
	str r3, [r0, #0]
	str r3, [r0, #4]
	str r3, [r0, #8]
	str r4, [sp, #104]
	b .L_080e191c
.L_080e1908:
	lsls r3, r6, #1
	adds r3, r3, r6
	movs r2, #128
	ldr r0, [sp, #96]
	lsls r3, r3, #10
	lsls r2, r2, #9
	subs r2, r2, r3
	str r2, [r0, #0]
	str r2, [r0, #4]
	str r2, [r0, #8]
.L_080e191c:
	bl sub_08004cf0
	b .L_080e194a
.L_080e1922:
	add r0, sp, #148
	movs r3, #0
	str r3, [r0, #0]
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #17
	str r3, [r0, #8]
	bl sub_08004cb4
	ldr r0, [pc, #376]
	bl sub_08004bd4
	ldr r0, [sp, #116]
	lsls r5, r0, #8
	adds r0, r5, #0
	bl sub_08004c6c
	adds r0, r5, #0
	bl sub_08004bd4
.L_080e194a:
	ldr r1, [sp, #116]
	cmp r1, #149
	ble .L_080e1952
	b sub_080e1a64
.L_080e1952:
	mov r3, sp
	movs r2, #0
	adds r3, #160
	mov r9, r2
	str r3, [sp, #52]
	ldr r2, [pc, #340]
	add r6, sp, #292
	adds r5, r3, #0
	movs r7, #0
.L_080e1964:
	ldrsh r3, [r7, r2]
	subs r3, #96
	lsls r3, r3, #16
	str r3, [r6, #0]
	movs r3, #0
	str r3, [r6, #4]
	adds r3, r7, #2
	ldrsh r3, [r3, r2]
	subs r3, #96
	lsls r3, r3, #16
	adds r1, r5, #0
	str r3, [r6, #8]
	adds r0, r6, #0
	str r2, [sp, #12]
	bl sub_080e3944
	ldr r3, [r5, #0]
	asrs r3, r3, #17
	adds r3, #64
	str r3, [r5, #0]
	movs r1, #6
	ldrsh r3, [r5, r1]
	adds r3, #60
	str r3, [r5, #4]
	movs r3, #1
	add r9, r3
	mov r4, r9
	adds r5, #12
	adds r7, #4
	ldr r2, [sp, #12]
	cmp r4, #6
	bne .L_080e1964
	ldr r1, [sp, #24]
	movs r5, #0
	movs r0, #4
	str r0, [sp, #40]
	str r5, [sp, #36]
	str r1, [sp, #32]
	mov r9, r5
	ldr r2, [sp, #32]
	ldr r3, [pc, #256]
	adds r2, r2, r3
	mov r10, r2
	cmp r2, #48
	ble .L_080e19c2
	movs r4, #48
	mov r10, r4
.L_080e19c2:
	mov r5, r10
	cmp r5, #0
	blt sub_080e1a48
	movs r0, #0
	mov r8, r0
	cmp r5, #0
	beq sub_080e1a48
	ldr r1, [sp, #104]
	ldr r2, [sp, #40]
	ldr r3, [sp, #104]
	asrs r1, r1, #31
	lsls r3, r3, #1
	str r1, [sp, #44]
	ldr r5, [sp, #52]
	ldr r6, [sp, #36]
	ldr r7, [pc, #216]
	str r2, [sp, #92]
	mov r11, r3
.L_080e19e6:
	adds r3, r6, #0
	adds r3, #12
	ldr r3, [r5, r3]
	ldr r4, [r5, r6]
	subs r3, r3, r4
	mov r0, r8
	muls r0, r3
	ldr r1, [pc, #200]
	lsls r0, r0, #0
	mov r12, pc
	bx r7
	adds r4, r4, r0
	adds r3, r6, #0
	ldr r0, [sp, #92]
	adds r3, #16
	ldr r2, [r5, r3]
	ldr r3, [r5, r0]
	subs r2, r2, r3
	mov r0, r8
	muls r0, r2
	ldr r1, [pc, #176]
	mov r12, pc
	bx r7
	adds r3, r3, r0
	mov r2, r11
	ldr r0, [pc, #168]
	subs r2, #2
	ldrh r1, [r0, r2]
	ldr r2, [sp, #120]
	ldr r0, [sp, #44]
	adds r1, r2, r1
	lsrs r2, r0, #31
	ldr r0, [sp, #104]
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r4, r4, r2
	mov r2, r11
	subs r3, r3, r0
	str r0, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #124]
	adds r2, r4, #0
	ldr r4, [sp, #108]
	bl sub_080072f4
	movs r0, #1
	add r8, r0
	cmp r8, r10
	bne .L_080e19e6
