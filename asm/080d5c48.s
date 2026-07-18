@ 呼出しグラフから到達した領域の再構築サム逆アセンブル。
@ （コードとデータが混在）。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_08007300, 0x08007300
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080e0524, 0x080e0524
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Region_080d5c48
Region_080d5c48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #72]
	ldr r1, [r7, #0]
	sub sp, #36
	str r1, [sp, #20]
	subs r3, r7, #4
	ldr r3, [r3, #0]
	ldr r6, [pc, #64]
	mov r8, r3
	add r6, r8
	str r0, [r6, #0]
	movs r0, #1
	bl sub_080cd594
	ldr r2, [pc, #52]
	ldr r3, [pc, #36]
	strh r3, [r2, #0]
	ldr r3, [pc, #36]
	adds r2, #48
	strh r3, [r2, #0]
	ldr r0, [pc, #44]
	movs r3, #1
	mov r1, r8
	movs r2, #1
	bl sub_080e0524
	ldr r3, [r6, #0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080d5cb8
	ldr r2, [pc, #28]
	ldr r3, [pc, #32]
	str r3, [r2, #0]
	b .L_080d5cb8
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x0000007a
	.4byte 0x04000028
	.4byte 0xffff9000
.L_080d5cb8:
	movs r5, #1
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl sub_080ed408
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, [pc, #368]
	add r3, r8
	str r5, [r3, #0]
	add r2, r8
	movs r3, #0
	ldr r7, [r7, #24]
	movs r1, #144
	str r3, [r2, #0]
	ldr r0, [pc, #356]
	lsls r1, r1, #3
	str r7, [sp, #12]
	bl sub_080041d8
	ldr r3, [r6, #0]
	ldr r3, [r3, #20]
	movs r2, #0
	mov r10, r2
	cmp r3, #0
	beq .L_080d5d12
	ldr r5, [pc, #340]
	movs r6, #63
	add r5, r8
.L_080d5cf8:
	bl sub_08004458
	movs r3, #1
	add r10, r3
	ldr r3, [pc, #328]
	ands r0, r6
	add r3, r8
	str r0, [r5, #0]
	ldr r3, [r3, #0]
	ldr r3, [r3, #20]
	adds r5, #28
	cmp r10, r3
	bne .L_080d5cf8
.L_080d5d12:
	ldr r3, [pc, #312]
	add r3, r8
	ldr r3, [r3, #0]
	ldr r3, [r3, #20]
	movs r1, #32
	movs r0, #0
	lsls r3, r3, #5
	negs r1, r1
	mov r9, r0
	cmp r3, r1
	beq .L_080d5e1e
.L_080d5d28:
	mov r2, r9
	cmp r2, #32
	bne .L_080d5d34
	movs r0, #0
	bl sub_080b50e8
.L_080d5d34:
	ldr r2, [pc, #276]
	movs r3, #0
	add r2, r8
	mov r10, r3
	ldr r3, [r2, #0]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080d5dfc
	movs r0, #225
	lsls r0, r0, #7
	add r0, r8
	str r0, [sp, #8]
	mov r11, r2
.L_080d5d4e:
	mov r1, r10
	lsls r6, r1, #4
	cmp r9, r6
	bne .L_080d5d5c
	movs r0, #143
	bl sub_080f9010
.L_080d5d5c:
	cmp r9, r6
	blt .L_080d5de8
	adds r3, r6, #0
	adds r3, #72
	cmp r9, r3
	bge .L_080d5de8
	mov r3, r11
	mov r0, r10
	ldr r2, [r3, #0]
	lsls r3, r0, #1
	adds r3, #36
	add r5, sp, #24
	ldrsh r0, [r2, r3]
	adds r1, r5, #0
	bl sub_080e396c
	mov r2, r11
	ldr r3, [r2, #0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080d5d8c
	ldr r3, [r5, #0]
	subs r3, #112
	str r3, [r5, #0]
.L_080d5d8c:
	ldr r3, [r5, #4]
	adds r2, r3, #0
	subs r2, #16
	movs r1, #20
	str r2, [r5, #4]
	ldr r2, [r5, #0]
	str r1, [sp, #4]
	movs r1, #216
	movs r0, #16
	lsls r1, r1, #3
	str r0, [sp, #0]
	subs r2, #8
	subs r3, #20
	ldr r7, [sp, #12]
	ldr r0, [sp, #20]
	add r1, r8
	bl sub_08007300
	cmp r9, r6
	blt .L_080d5de8
	ldr r2, [sp, #8]
	mov r1, r9
	ldr r3, [r2, #24]
	subs r0, r1, r6
	adds r0, r0, r3
	movs r1, #6
	bl sub_080022ec
	movs r1, #9
	bl sub_080022fc
	lsls r1, r0, #1
	ldr r2, [r5, #0]
	ldr r3, [r5, #4]
	adds r1, r1, r0
	movs r0, #16
	lsls r1, r1, #6
	str r0, [sp, #0]
	movs r0, #12
	str r0, [sp, #4]
	add r1, r8
	subs r2, #8
	subs r3, #16
	ldr r0, [sp, #20]
	bl sub_08007300
.L_080d5de8:
	ldr r1, [sp, #8]
	adds r1, #28
	str r1, [sp, #8]
	mov r0, r11
	ldr r3, [r0, #0]
	movs r2, #1
	ldr r3, [r3, #20]
	add r10, r2
	cmp r10, r3
	bne .L_080d5d4e
.L_080d5dfc:
	ldr r2, [pc, #80]
	movs r3, #1
	add r2, r8
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	ldr r3, [pc, #64]
	add r3, r8
	ldr r3, [r3, #0]
	ldr r3, [r3, #20]
	movs r1, #1
	lsls r3, r3, #5
	add r9, r1
	adds r3, #32
	cmp r9, r3
	bne .L_080d5d28
.L_080d5e1e:
	ldr r0, [pc, #36]
	bl sub_08004278
	movs r0, #46
	bl sub_08002dd8
	bl sub_080cdbc0
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
