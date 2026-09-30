.syntax unified
	.thumb
	.global Func_08027e20
	.thumb_func
Func_08027e20:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	mov r8, r0
	movs r0, #0
	str r0, [sp, #16]
	str r0, [sp, #12]
	ldr r3, .L_0802808c
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08027e6e
	ldr r3, .L_08028090
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08027e6e
	adds r3, r2, #0
.L_08027e50:
	subs r3, #1
	cmp r3, #0
	bne .L_08027e50
	movs r3, #95
.L_08027e58:
	subs r3, #1
	cmp r3, #0
	bge .L_08027e58
	movs r3, #63
.L_08027e60:
	subs r3, #1
	cmp r3, #0
	bge .L_08027e60
	movs r3, #63
.L_08027e68:
	subs r3, #1
	cmp r3, #0
	bge .L_08027e68
.L_08027e6e:
	movs r5, #128
	movs r3, #128
	mov r1, r8
	lsls r5, r5, #9
	lsls r3, r3, #7
	movs r0, #128
	str r5, [r1, #48]
	str r3, [r1, #52]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08027ea0
	ldr r0, .L_08028090
	movs r2, #2
	ldr r3, [r0]
	ands r3, r2
	cmp r3, #0
	beq .L_08027ea2
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #11
	str r3, [r2, #48]
	str r5, [r2, #52]
	b .L_08027ea2
.L_08027ea0:
	ldr r0, .L_08028090
.L_08027ea2:
	ldr r3, [r0]
	ldr r1, .L_08028094
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	movs r2, #255
	lsls r3, r3, #16
	lsls r2, r2, #8
	lsrs r1, r3, #16
	adds r2, #255
	str r3, [sp, #4]
	cmp r1, r2
	bne .L_08027eca
	ldr r0, [sp, #16]
	movs r3, #4
	orrs r0, r3
	str r0, [sp, #16]
	b .L_08028376
.L_08027eca:
	movs r2, #0
	str r2, [sp, #16]
	movs r3, #88
	mov r0, r8
	add r3, sp
	mov r11, r3
	ldr r3, [r0, #8]
	mov r2, r11
	str r3, [r2]
	ldr r3, [r0, #12]
	str r3, [r2, #4]
	ldr r3, [r0, #16]
	movs r0, #152
	str r3, [r2, #8]
	lsls r0, r0, #13
	bl Func_0801489c
	mov r3, r8
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08027faa
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #20]
	ldr r4, .L_08028098
	movs r5, #0
	movs r0, #128
	mov r9, r5
	lsls r0, r0, #12
.L_08027f0a:
	ldr r3, [r7]
	cmp r3, #0
	beq .L_08027f82
	adds r3, r7, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08027f82
	cmp r7, r8
	beq .L_08027f82
	mov r3, r8
	ldr r2, [r3, #12]
	ldr r3, [r7, #12]
	ldr r1, .L_0802809c
	subs r2, r2, r3
	adds r3, r2, r1
	cmp r3, #0
	bge .L_08027f38
	movs r3, #128
	lsls r3, r3, #13
	subs r3, r3, r2
.L_08027f38:
	cmp r3, r4
	bgt .L_08027f82
	mov r2, r8
	ldr r3, [r2, #8]
	ldr r1, [r7, #8]
	subs r3, r3, r1
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_08027f60
	ldr r3, [r2, #16]
	ldr r2, [r7, #16]
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_08027f60
	movs r5, #1
.L_08027f60:
	mov r2, r11
	ldr r3, [r2]
	subs r3, r3, r1
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_08027f82
	ldr r3, [r2, #8]
	ldr r2, [r7, #16]
	movs r1, #128
	subs r3, r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_08027f82
	b .L_080281ca
.L_08027f82:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r7, #128
	cmp r3, #63
	ble .L_08027f0a
	cmp r5, #0
	beq .L_08027faa
	mov r0, r8
	mov r1, r11
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08027faa
	ldr r1, [sp, #4]
	movs r0, #0
	asrs r1, r1, #16
	str r0, [sp, #16]
	str r1, [sp, #8]
	b .L_08028376
.L_08027faa:
	ldr r3, .L_0802808c
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08027fcc
	ldr r3, .L_08028090
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08027fcc
	ldr r3, [sp, #4]
	movs r2, #0
	asrs r3, r3, #16
	str r2, [sp, #16]
	str r3, [sp, #8]
	b .L_08028376
.L_08027fcc:
	mov r0, r8
	mov r1, r11
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080280a8
	mov r0, r8
	ldr r3, [r0, #8]
	add r5, sp, #76
	str r3, [r5]
	movs r7, #152
	ldr r3, [r0, #12]
	movs r2, #128
	str r3, [r5, #4]
	lsls r2, r2, #5
	ldr r3, [r0, #16]
	lsls r7, r7, #13
	str r3, [r5, #8]
	ldr r1, [sp, #4]
	adds r0, r7, #0
	lsrs r6, r1, #16
	adds r1, r6, r2
	adds r2, r5, #0
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080280a8
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r2, .L_080280a0
	str r3, [r5]
	adds r1, r6, r2
	ldr r3, [r0, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	adds r0, r7, #0
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080280a8
	mov r0, r8
	ldr r3, [r0, #8]
	movs r2, #128
	str r3, [r5]
	lsls r2, r2, #6
	ldr r3, [r0, #12]
	adds r1, r6, r2
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r0, #16]
	adds r0, r7, #0
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080280a8
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r2, .L_080280a4
	str r3, [r5]
	adds r1, r6, r2
	ldr r3, [r0, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	adds r0, r7, #0
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080280a8
	ldr r3, [sp, #4]
	asrs r3, r3, #16
	lsls r0, r3, #16
	str r3, [sp, #8]
	str r0, [sp, #0]
	b .L_080281ee
	.2byte 0x0000
.L_0802808c:
	.4byte Data_03001238
.L_08028090:
	.4byte gInput
.L_08028094:
	.4byte Data_0802ec5c
.L_08028098:
	.4byte 0x0007ffff
.L_0802809c:
	.4byte 0xfff00000
.L_080280a0:
	.4byte 0xfffff000
.L_080280a4:
	.4byte 0xffffe000
.L_080280a8:
	ldr r2, [sp, #4]
	movs r0, #128
	lsrs r3, r2, #16
	lsls r0, r0, #5
	adds r2, r3, r0
	ldr r0, .L_080283c4
	add r1, sp, #20
	strh r2, [r1]
	adds r2, r3, r0
	movs r0, #128
	lsls r0, r0, #6
	strh r2, [r1, #2]
	adds r2, r3, r0
	ldr r0, .L_080283c8
	strh r2, [r1, #4]
	adds r2, r3, r0
	movs r0, #192
	lsls r0, r0, #6
	strh r2, [r1, #6]
	adds r2, r3, r0
	strh r2, [r1, #8]
	ldr r2, .L_080283cc
	mov r10, r1
	adds r3, r3, r2
	mov r0, r10
	strh r3, [r0, #10]
	movs r1, #0
	mov r9, r1
	mov r7, r11
.L_080280e2:
	mov r2, r9
	lsls r3, r2, #1
	mov r0, r10
	ldrsh r0, [r0, r3]
	mov r2, r8
	str r0, [sp, #8]
	ldr r3, [r2, #8]
	str r3, [r7]
	ldr r3, [r2, #12]
	str r3, [r7, #4]
	ldr r3, [r2, #16]
	adds r2, r7, #0
	str r3, [r7, #8]
	lsls r3, r0, #16
	lsrs r6, r3, #16
	movs r0, #152
	lsls r0, r0, #13
	adds r1, r6, #0
	str r3, [sp, #0]
	bl Func_0801489c
	mov r0, r8
	adds r1, r7, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080281be
	mov r0, r8
	ldr r3, [r0, #8]
	add r5, sp, #76
	str r3, [r5]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #5
	str r3, [r5, #4]
	adds r1, r6, r2
	ldr r3, [r0, #16]
	movs r0, #152
	lsls r0, r0, #13
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080281be
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r2, .L_080283c4
	str r3, [r5]
	adds r1, r6, r2
	ldr r3, [r0, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #152
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080281be
	mov r0, r8
	ldr r3, [r0, #8]
	movs r2, #128
	str r3, [r5]
	lsls r2, r2, #6
	ldr r3, [r0, #12]
	adds r1, r6, r2
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r0, #16]
	movs r0, #152
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080281be
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r2, .L_080283c8
	str r3, [r5]
	adds r1, r6, r2
	ldr r3, [r0, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #152
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	beq .L_080281ee
.L_080281be:
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #6
	blt .L_080280e2
	b .L_080281d6
.L_080281ca:
	ldr r2, [sp, #4]
	movs r1, #0
	asrs r2, r2, #16
	str r1, [sp, #16]
	str r2, [sp, #8]
	b .L_08028376
.L_080281d6:
	mov r0, r8
	ldr r3, [r0, #8]
	mov r1, r11
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	str r3, [r1, #8]
	ldr r2, [sp, #16]
	movs r3, #1
	orrs r2, r3
	str r2, [sp, #16]
.L_080281ee:
	movs r3, #64
	mov r0, r8
	add r3, sp
	mov r11, r3
	ldr r3, [r0, #8]
	mov r1, r11
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	movs r0, #240
	str r3, [r1, #8]
	ldr r2, [sp, #0]
	lsls r0, r0, #12
	lsrs r1, r2, #16
	mov r2, r11
	bl Func_0801489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #20]
	movs r3, #63
	adds r6, r7, #0
	mov r9, r3
	adds r6, #8
.L_08028220:
	mov r0, r8
	ldrh r3, [r0, #32]
	subs r1, r3, #2
	ldr r3, [r7]
	cmp r3, #0
	bne .L_0802822e
	b .L_0802834a
.L_0802822e:
	adds r3, r7, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0802823e
	b .L_0802834a
.L_0802823e:
	cmp r7, r8
	bne .L_08028244
	b .L_0802834a
.L_08028244:
	ldrh r3, [r6, #24]
	adds r0, r6, #0
	subs r3, #2
	mov r2, r11
	bl Func_08026f80
	cmp r0, #0
	blt .L_0802834a
	ldr r3, [r6, #80]
	ldr r2, .L_080283d0
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #2
	cmp r3, r1
	bne .L_08028342
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r0, [r6, #8]
	ldr r1, [r6]
	subs r0, r0, r3
	ldr r3, [r2, #8]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, [r6]
	add r5, sp, #76
	str r3, [r5]
	lsls r0, r0, #16
	ldr r3, [r6, #4]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	asrs r3, r0, #16
	lsrs r0, r0, #16
	mov r10, r0
	movs r0, #128
	lsls r0, r0, #7
	mov r1, r10
	str r3, [sp, #8]
	bl Func_0801489c
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_08024f20
	cmp r0, #0
	bne .L_08028342
	ldr r3, [r6]
	movs r0, #168
	str r3, [r5]
	lsls r0, r0, #13
	ldr r3, [r6, #4]
	mov r1, r10
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	bl Func_0801489c
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08028342
	ldr r3, [r6]
	movs r1, #128
	str r3, [r5]
	lsls r1, r1, #5
	ldr r3, [r6, #4]
	movs r0, #168
	str r3, [r5, #4]
	add r1, r10
	ldr r3, [r6, #8]
	lsls r0, r0, #13
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08028342
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08028342
	ldr r3, [r6]
	ldr r1, .L_080283c4
	str r3, [r5]
	movs r0, #168
	ldr r3, [r6, #4]
	add r1, r10
	str r3, [r5, #4]
	lsls r0, r0, #13
	ldr r3, [r6, #8]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Func_0801489c
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08028342
	movs r0, #128
	lsls r0, r0, #7
	mov r1, r10
	adds r2, r6, #0
	bl Func_0801489c
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #48]
	str r3, [r6, #52]
	str r3, [r6, #56]
	ldr r0, [sp, #12]
	movs r1, #1
	orrs r0, r1
	str r0, [sp, #12]
	b .L_0802834a
.L_08028342:
	ldr r2, [sp, #16]
	movs r3, #2
	orrs r2, r3
	str r2, [sp, #16]
.L_0802834a:
	movs r3, #1
	negs r3, r3
	add r9, r3
	mov r0, r9
	adds r6, #128
	adds r7, #128
	cmp r0, #0
	blt .L_0802835c
	b .L_08028220
.L_0802835c:
	ldr r1, [sp, #16]
	cmp r1, #0
	bne .L_08028376
	ldr r2, [sp, #12]
	cmp r2, #0
	beq .L_08028376
	movs r3, #128
	lsls r3, r3, #7
	mov r0, r8
	str r3, [r0, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r0, #52]
.L_08028376:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_080283ac
	ldr r3, [sp, #16]
	movs r2, #3
	ands r2, r3
	cmp r2, #0
	beq .L_08028398
	movs r0, #194
	lsls r0, r0, #1
	adds r2, r1, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_080283a0
.L_08028398:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
.L_080283a0:
	ldr r3, .L_080283d4
	movs r0, #195
	ldr r3, [r3]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
.L_080283ac:
	ldr r1, [sp, #12]
	cmp r1, #0
	bne .L_080283b8
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_080283d8
.L_080283b8:
	mov r0, r8
	movs r1, #1
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_080283e0
	.2byte 0x0000
.L_080283c4:
	.4byte 0xfffff000
.L_080283c8:
	.4byte 0xffffe000
.L_080283cc:
	.4byte 0xffffd000
.L_080283d0:
	.4byte 0xff000200
.L_080283d4:
	.4byte gInput
.L_080283d8:
	mov r0, r8
	movs r1, #2
	bl ObjectDispatch_ApplyArgumentToChildren
.L_080283e0:
	ldr r3, .L_08028520
	ldr r1, .L_08028524
	movs r0, #140
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrh r2, [r3]
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0802840c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #252
	strh r3, [r2]
	ldr r2, [sp, #16]
	movs r3, #4
	orrs r2, r3
	str r2, [sp, #16]
.L_0802840c:
	ldr r3, [sp, #16]
	cmp r3, #0
	beq .L_08028466
	movs r3, #128
	mov r0, r8
	lsls r3, r3, #24
	str r3, [r0, #56]
	str r3, [r0, #60]
	str r3, [r0, #64]
	movs r3, #0
	str r3, [r0, #36]
	str r3, [r0, #44]
	ldr r1, [sp, #16]
	movs r3, #3
	ands r3, r1
	cmp r3, #0
	beq .L_08028454
	ldrh r2, [r0, #6]
	ldr r0, [sp, #4]
	movs r1, #128
	lsrs r3, r0, #16
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	lsls r1, r1, #5
	cmp r3, r1
	ble .L_08028446
	movs r3, #128
	lsls r3, r3, #3
.L_08028446:
	ldr r0, .L_08028528
	cmp r3, r0
	bge .L_0802844e
	ldr r3, .L_0802852c
.L_0802844e:
	adds r3, r2, r3
	mov r1, r8
	strh r3, [r1, #6]
.L_08028454:
	mov r3, r8
	adds r3, #100
	movs r2, #0
	strh r2, [r3]
	mov r2, r8
	adds r2, #102
	movs r3, #2
	strh r3, [r2]
	b .L_080284ee
.L_08028466:
	add r3, sp, #88
	ldr r1, [r3]
	ldr r2, [r3, #4]
	mov r0, r8
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	mov r2, r8
	ldr r1, [r2, #36]
	ldr r6, .L_08028530
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	mov r3, r8
	ldr r1, [r3, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r1, [sp, #16]
	mov r2, r8
	str r1, [r2, #36]
	str r1, [r2, #44]
	ldr r3, [sp, #8]
	adds r2, #36
	lsls r1, r3, #16
	lsrs r1, r1, #16
	bl Func_0801489c
	mov r2, r8
	adds r2, #100
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldrh r1, [r2]
	cmp r3, #0
	beq .L_080284ba
	subs r3, r1, #1
	strh r3, [r2]
.L_080284ba:
	mov r1, r8
	adds r1, #90
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r0, [sp, #4]
	mov r1, r8
	ldrh r2, [r1, #6]
	lsrs r3, r0, #16
	subs r3, r3, r2
	lsls r3, r3, #16
	movs r1, #128
	asrs r3, r3, #16
	lsls r1, r1, #5
	cmp r3, r1
	ble .L_080284e0
	movs r3, #128
	lsls r3, r3, #3
.L_080284e0:
	ldr r0, .L_08028528
	cmp r3, r0
	bge .L_080284e8
	ldr r3, .L_0802852c
.L_080284e8:
	adds r3, r2, r3
	mov r1, r8
	strh r3, [r1, #6]
.L_080284ee:
	mov r3, r8
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_08028500
	mov r0, r8
	adds r0, #8
	bl GetWorldMapCollision
.L_08028500:
	bl Func_08026e60
	mov r2, r8
	ldrh r3, [r2, #4]
	mov r0, r8
	adds r3, #1
	strh r3, [r0, #4]
	add sp, #100
	movs r0, #1
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08028520:
	.4byte gPartyState
.L_08028524:
	.4byte gInput
.L_08028528:
	.4byte 0xfffff000
.L_0802852c:
	.4byte 0xfffffc00
.L_08028530:
	.4byte IwramMulQ16
