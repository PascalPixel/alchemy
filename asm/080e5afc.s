@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e5e28, 0x080e5e28
	.set sub_080e640e, 0x080e640e
	.global Region_080e5afc
Region_080e5afc:
	ldr r1, [sp, #96]
	cmp r1, #30
	bne .L_080e5bac
	ldr r2, [sp, #84]
	cmp r2, #15
	ble .L_080e5b16
	ldr r5, [sp, #84]
	ldr r2, [pc, #60]
	ldr r1, [pc, #60]
	ldr r3, [pc, #64]
	subs r2, r2, r5
	orrs r2, r1
	strh r2, [r3, #0]
.L_080e5b16:
	ldr r0, [sp, #84]
	cmp r0, #5
	bgt .L_080e5b20
	bl sub_080e640e
.L_080e5b20:
	ldr r1, [sp, #60]
	ldr r2, [sp, #84]
	ldr r6, [r1, #0]
	lsrs r0, r0, #31
	adds r0, r2, r0
	lsrs r3, r6, #31
	movs r1, #3
	asrs r0, r0, #1
	adds r6, r6, r3
	bl sub_080022fc
	lsls r5, r0, #2
	adds r5, r5, r0
	lsls r3, r5, #9
	asrs r6, r6, #1
	ldr r1, [pc, #20]
	subs r6, #20
	mov r9, r3
	b .L_080e5b58
	.2byte 0x0000
	.4byte 0x00000020
	.4byte 0x00001000
	.4byte 0x04000052
	.4byte 0x02010c56
.L_080e5b58:
	movs r0, #40
	movs r2, #32
	add r1, r9
	mov r10, r0
	str r0, [sp, #0]
	mov r8, r2
	str r2, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	adds r2, r6, #0
	movs r3, #16
	bl sub_080072f4
	ldr r3, [pc, #636]
	lsls r5, r5, #8
	adds r5, r5, r3
	mov r0, r10
	mov r1, r8
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #104]
	adds r1, r5, #0
	adds r2, r6, #0
	movs r3, #48
	ldr r0, [sp, #88]
	bl sub_080072f4
	ldr r2, [pc, #612]
	mov r3, r10
	add r9, r2
	mov r5, r8
	str r3, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	mov r1, r9
	adds r2, r6, #0
	movs r3, #80
	bl sub_080072f4
	bl sub_080e640e
.L_080e5bac:
	ldr r0, [sp, #96]
	cmp r0, #5
	beq .L_080e5bb6
	cmp r0, #23
	bne .L_080e5c32
.L_080e5bb6:
	movs r2, #112
	movs r1, #0
	add r2, sp
	ldr r7, [pc, #568]
	mov r10, r1
	mov r8, r2
.L_080e5bc2:
	mov r5, r10
	lsrs r3, r5, #31
	add r3, r10
	asrs r3, r3, #1
	ldr r0, [sp, #84]
	adds r3, #4
	cmp r0, r3
	blt .L_080e5c22
	ldr r3, [r7, #24]
	cmp r3, #11
	bgt .L_080e5c22
	mov r6, r8
	lsrs r5, r3, #31
	adds r1, r6, #0
	adds r0, r7, #0
	adds r5, r3, r5
	bl sub_080e3944
	ldr r2, [r6, #0]
	ldr r1, [pc, #528]
	lsrs r3, r2, #31
	asrs r5, r5, #1
	adds r2, r2, r3
	lsls r5, r5, #11
	ldr r3, [r6, #4]
	asrs r2, r2, #1
	adds r5, r5, r1
	movs r1, #32
	str r2, [r6, #0]
	str r1, [sp, #0]
	movs r1, #64
	subs r3, #32
	str r1, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	adds r1, r5, #0
	bl sub_080072f4
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #5
	bl sub_080e38b8
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_080e5c22:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080e5bc2
	bl sub_080e640e
.L_080e5c32:
	ldr r5, [sp, #96]
	cmp r5, #4
	bne .L_080e5c3a
	b sub_080e640e
.L_080e5c3a:
	cmp r5, #11
	beq .L_080e5c40
	b sub_080e5e28
.L_080e5c40:
	ldr r0, [sp, #84]
	lsls r5, r0, #9
	adds r0, r5, #0
	bl sub_08002322
	adds r0, r5, #0
	bl sub_0800231c
	ldr r2, [sp, #60]
	lsls r0, r0, #2
	movs r1, #6
	ldrsh r3, [r2, r1]
	asrs r0, r0, #16
	adds r3, r3, r0
	adds r5, r3, #0
	ldr r3, [sp, #84]
	adds r5, #16
	cmp r3, #3
	bgt .L_080e5c92
	ldr r0, [sp, #92]
	ldr r2, [pc, #404]
	adds r3, r0, r2
	ldr r3, [r3, #0]
	ldr r2, [r3, #4]
	ldr r1, [pc, #400]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r1, r3]
	ldr r3, [pc, #396]
	movs r1, #57
	ldrb r3, [r3, #0]
	str r1, [sp, #0]
	movs r1, #98
	str r1, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	bl sub_080072f4
	b sub_080e640e
.L_080e5c92:
	ldr r3, [sp, #84]
	cmp r3, #7
	bgt .L_080e5cc2
	ldr r0, [sp, #92]
	ldr r2, [pc, #356]
	adds r3, r0, r2
	ldr r3, [r3, #0]
	ldr r2, [r3, #4]
	ldr r1, [pc, #352]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r1, r3]
	ldr r3, [pc, #348]
	movs r1, #57
	ldrb r3, [r3, #0]
	str r1, [sp, #0]
	movs r1, #98
	str r1, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	bl sub_080072f4
.L_080e5cc2:
	ldr r3, [sp, #92]
	ldr r0, [pc, #324]
	adds r1, r3, r0
	ldr r0, [pc, #308]
	adds r6, r3, r0
	ldr r3, [r6, #0]
	ldr r2, [pc, #308]
	mov r8, r2
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r7, [pc, #300]
	adds r3, #1
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #1]
	movs r0, #99
	str r0, [sp, #0]
	movs r0, #69
	adds r3, r5, r3
	str r0, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl sub_080072f4
	ldr r3, [sp, #84]
	subs r3, #4
	cmp r3, #1
	bhi .L_080e5d0a
	movs r1, #128
	ldr r3, [pc, #272]
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	ldr r2, [pc, #268]
	bl sub_080072f0
.L_080e5d0a:
	ldr r3, [sp, #84]
	subs r3, #6
	cmp r3, #1
	bhi .L_080e5d3a
	ldr r2, [sp, #92]
	ldr r3, [pc, #256]
	adds r1, r2, r3
	ldr r3, [r6, #0]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #2
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #2]
	movs r0, #128
	str r0, [sp, #0]
	movs r0, #91
	str r0, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl sub_080072f4
.L_080e5d3a:
	ldr r3, [sp, #84]
	subs r3, #8
	cmp r3, #1
	bhi .L_080e5d66
	ldr r3, [r6, #0]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #3
	mov r1, r8
	ldrb r2, [r1, r3]
	ldrb r3, [r7, #3]
	movs r1, #128
	str r1, [sp, #0]
	movs r1, #91
	str r1, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [pc, #152]
	bl sub_080072f4
.L_080e5d66:
	ldr r3, [sp, #84]
	subs r3, #10
	cmp r3, #1
	bhi .L_080e5d92
	ldr r3, [r6, #0]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #4
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #4]
	movs r0, #128
	str r0, [sp, #0]
	movs r0, #59
	str r0, [sp, #4]
	ldr r1, [pc, #148]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl sub_080072f4
.L_080e5d92:
	ldr r3, [sp, #84]
	subs r3, #12
	cmp r3, #1
	bhi .L_080e5dbe
	ldr r3, [r6, #0]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #5
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #5]
	movs r0, #122
	str r0, [sp, #0]
	movs r0, #29
	str r0, [sp, #4]
	ldr r1, [pc, #108]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl sub_080072f4
.L_080e5dbe:
	ldr r3, [sp, #84]
	subs r3, #14
	cmp r3, #1
	bls .L_080e5dc8
	b sub_080e640e
.L_080e5dc8:
	ldr r3, [r6, #0]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #6
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #6]
	movs r0, #76
	str r0, [sp, #0]
	movs r0, #25
	str r0, [sp, #4]
	ldr r1, [pc, #64]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl sub_080072f4
	b sub_080e640e
