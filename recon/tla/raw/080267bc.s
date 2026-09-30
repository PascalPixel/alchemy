.syntax unified
	.thumb
	.global Func_080267bc
	.thumb_func
Func_080267bc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	movs r1, #0
	mov r8, r0
	ldr r3, .L_08026b10
	movs r0, #2
	str r1, [sp, #16]
	str r1, [sp, #12]
	str r0, [sp, #8]
	ldr r1, .L_08026b14
	movs r2, #143
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r2, [r3]
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_08026802
	movs r3, #128
	lsls r3, r3, #9
	mov r0, r8
	str r3, [r0, #48]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	movs r1, #5
	str r3, [r0, #52]
	str r1, [sp, #8]
	b .L_08026812
.L_08026802:
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r8
	str r3, [r2, #48]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	str r3, [r2, #52]
.L_08026812:
	ldr r3, .L_08026b14
	ldr r1, .L_08026b18
	ldr r3, [r3]
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
	bne .L_0802683c
	ldr r0, [sp, #16]
	movs r3, #4
	orrs r0, r3
	str r0, [sp, #16]
	b .L_08026ccc
.L_0802683c:
	movs r2, #0
	str r2, [sp, #16]
	movs r3, #88
	mov r0, r8
	add r3, sp
	mov r11, r3
	ldr r3, [r0, #8]
	mov r2, r11
	str r3, [r2]
	movs r5, #128
	ldr r3, [r0, #12]
	lsls r5, r5, #12
	str r3, [r2, #4]
	ldr r3, [r0, #16]
	adds r0, r5, #0
	str r3, [r2, #8]
	bl Func_0801489c
	mov r3, r8
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08026916
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #20]
	ldr r4, .L_08026b1c
	movs r6, #0
	mov r9, r6
	adds r0, r5, #0
.L_0802687c:
	ldr r3, [r7]
	cmp r3, #0
	beq .L_080268f4
	adds r3, r7, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080268f4
	cmp r7, r8
	beq .L_080268f4
	mov r3, r8
	ldr r2, [r3, #12]
	ldr r3, [r7, #12]
	ldr r1, .L_08026b20
	subs r2, r2, r3
	adds r3, r2, r1
	cmp r3, #0
	bge .L_080268aa
	movs r3, #128
	lsls r3, r3, #13
	subs r3, r3, r2
.L_080268aa:
	cmp r3, r4
	bgt .L_080268f4
	mov r2, r8
	ldr r3, [r2, #8]
	ldr r1, [r7, #8]
	subs r3, r3, r1
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080268d2
	ldr r3, [r2, #16]
	ldr r2, [r7, #16]
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080268d2
	movs r6, #1
.L_080268d2:
	mov r2, r11
	ldr r3, [r2]
	subs r3, r3, r1
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080268f4
	ldr r3, [r2, #8]
	ldr r2, [r7, #16]
	movs r1, #128
	subs r3, r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080268f4
	b .L_08026b08
.L_080268f4:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r7, #128
	cmp r3, #63
	ble .L_0802687c
	cmp r6, #0
	beq .L_08026916
	mov r0, r8
	mov r1, r11
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026916
	movs r0, #0
	str r0, [sp, #16]
	b .L_08026ccc
.L_08026916:
	ldr r3, .L_08026b24
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802692e
	ldr r3, .L_08026b14
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0802692e
	b .L_08026b08
.L_0802692e:
	mov r0, r8
	mov r1, r11
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080269e6
	mov r2, r8
	ldr r3, [r2, #8]
	add r5, sp, #76
	str r3, [r5]
	movs r7, #128
	ldr r3, [r2, #12]
	movs r0, #128
	str r3, [r5, #4]
	lsls r0, r0, #5
	ldr r3, [r2, #16]
	lsls r7, r7, #12
	str r3, [r5, #8]
	ldr r3, [sp, #4]
	adds r2, r5, #0
	lsrs r6, r3, #16
	adds r1, r6, r0
	adds r0, r7, #0
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080269e6
	mov r1, r8
	ldr r3, [r1, #8]
	ldr r2, .L_08026b28
	str r3, [r5]
	adds r0, r7, #0
	ldr r3, [r1, #12]
	str r3, [r5, #4]
	ldr r3, [r1, #16]
	adds r1, r6, r2
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080269e6
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
	bne .L_080269e6
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r2, .L_08026b2c
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
	ldr r3, [sp, #4]
	str r3, [sp, #0]
	cmp r0, #0
	bne .L_080269e6
	b .L_08026b4c
.L_080269e6:
	ldr r1, [sp, #4]
	add r0, sp, #20
	mov r10, r0
	movs r0, #128
	lsrs r3, r1, #16
	lsls r0, r0, #5
	adds r2, r3, r0
	ldr r0, .L_08026b28
	mov r1, r10
	strh r2, [r1]
	adds r2, r3, r0
	movs r0, #128
	lsls r0, r0, #6
	strh r2, [r1, #2]
	adds r2, r3, r0
	ldr r0, .L_08026b2c
	strh r2, [r1, #4]
	adds r2, r3, r0
	movs r0, #192
	lsls r0, r0, #6
	strh r2, [r1, #6]
	adds r2, r3, r0
	strh r2, [r1, #8]
	ldr r2, .L_08026b30
	mov r0, r10
	adds r3, r3, r2
	strh r3, [r0, #10]
	movs r1, #0
	mov r9, r1
	mov r7, r11
.L_08026a22:
	mov r2, r9
	lsls r3, r2, #1
	mov r0, r10
	ldrsh r2, [r0, r3]
	mov r0, r8
	ldr r3, [r0, #8]
	lsls r2, r2, #16
	str r3, [r7]
	lsrs r6, r2, #16
	ldr r3, [r0, #12]
	adds r1, r6, #0
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	str r3, [r7, #8]
	lsls r0, r0, #12
	str r2, [sp, #0]
	adds r2, r7, #0
	bl Func_0801489c
	mov r0, r8
	adds r1, r7, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026afc
	mov r1, r8
	ldr r3, [r1, #8]
	add r5, sp, #76
	str r3, [r5]
	movs r2, #128
	ldr r3, [r1, #12]
	lsls r2, r2, #5
	str r3, [r5, #4]
	movs r0, #128
	ldr r3, [r1, #16]
	lsls r0, r0, #12
	adds r1, r6, r2
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026afc
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r2, .L_08026b28
	str r3, [r5]
	adds r1, r6, r2
	ldr r3, [r0, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026afc
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
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026afc
	mov r0, r8
	ldr r3, [r0, #8]
	ldr r2, .L_08026b2c
	str r3, [r5]
	adds r1, r6, r2
	ldr r3, [r0, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	bl Func_0801489c
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	beq .L_08026b4c
.L_08026afc:
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #6
	blt .L_08026a22
	b .L_08026b34
.L_08026b08:
	movs r1, #0
	str r1, [sp, #16]
	b .L_08026ccc
	.2byte 0x0000
.L_08026b10:
	.4byte gPartyState
.L_08026b14:
	.4byte gInput
.L_08026b18:
	.4byte Data_0802ec5c
.L_08026b1c:
	.4byte 0x0007ffff
.L_08026b20:
	.4byte 0xfff00000
.L_08026b24:
	.4byte Data_03001238
.L_08026b28:
	.4byte 0xfffff000
.L_08026b2c:
	.4byte 0xffffe000
.L_08026b30:
	.4byte 0xffffd000
.L_08026b34:
	mov r2, r8
	ldr r3, [r2, #8]
	mov r0, r11
	str r3, [r0]
	ldr r3, [r2, #12]
	str r3, [r0, #4]
	ldr r3, [r2, #16]
	str r3, [r0, #8]
	ldr r1, [sp, #16]
	movs r3, #1
	orrs r1, r3
	str r1, [sp, #16]
.L_08026b4c:
	mov r0, r8
	ldr r3, [r0, #8]
	add r2, sp, #64
	str r3, [r2]
	mov r11, r2
	ldr r3, [r0, #12]
	str r3, [r2, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	str r3, [r2, #8]
	ldr r2, [sp, #0]
	lsls r0, r0, #11
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
.L_08026b7a:
	mov r0, r8
	ldrh r3, [r0, #32]
	subs r1, r3, #2
	ldr r3, [r7]
	cmp r3, #0
	bne .L_08026b88
	b .L_08026ca0
.L_08026b88:
	adds r3, r7, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_08026b98
	b .L_08026ca0
.L_08026b98:
	cmp r7, r8
	bne .L_08026b9e
	b .L_08026ca0
.L_08026b9e:
	ldrh r3, [r6, #24]
	adds r0, r6, #0
	subs r3, #2
	mov r2, r11
	bl Func_08026f80
	cmp r0, #0
	blt .L_08026ca0
	ldr r3, [r6, #80]
	ldr r2, .L_08026dc4
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #2
	cmp r3, r1
	bne .L_08026c98
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
	lsrs r0, r0, #16
	str r3, [r5, #4]
	mov r10, r0
	ldr r3, [r6, #8]
	movs r0, #128
	lsls r0, r0, #7
	mov r1, r10
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_08024f20
	cmp r0, #0
	bne .L_08026c98
	ldr r3, [r6]
	movs r0, #160
	str r3, [r5]
	lsls r0, r0, #12
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
	bne .L_08026c98
	ldr r3, [r6]
	movs r1, #128
	str r3, [r5]
	lsls r1, r1, #5
	ldr r3, [r6, #4]
	movs r0, #160
	str r3, [r5, #4]
	add r1, r10
	ldr r3, [r6, #8]
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Func_0801489c
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026c98
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026c98
	ldr r3, [r6]
	ldr r1, .L_08026dc8
	str r3, [r5]
	movs r0, #160
	ldr r3, [r6, #4]
	add r1, r10
	str r3, [r5, #4]
	lsls r0, r0, #12
	ldr r3, [r6, #8]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Func_0801489c
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08026c98
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
	ldr r3, [sp, #12]
	movs r0, #1
	orrs r3, r0
	str r3, [sp, #12]
	b .L_08026ca0
.L_08026c98:
	ldr r1, [sp, #16]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #16]
.L_08026ca0:
	movs r2, #1
	negs r2, r2
	add r9, r2
	mov r3, r9
	adds r6, #128
	adds r7, #128
	cmp r3, #0
	blt .L_08026cb2
	b .L_08026b7a
.L_08026cb2:
	ldr r0, [sp, #16]
	cmp r0, #0
	bne .L_08026ccc
	ldr r1, [sp, #12]
	cmp r1, #0
	beq .L_08026ccc
	movs r3, #128
	lsls r3, r3, #7
	mov r2, r8
	str r3, [r2, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r2, #52]
.L_08026ccc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	cmp r3, #0
	beq .L_08026cf6
	ldr r0, [sp, #16]
	movs r2, #3
	ands r2, r0
	cmp r2, #0
	beq .L_08026cee
	movs r1, #194
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08026cf6
.L_08026cee:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r2, [r3]
.L_08026cf6:
	ldr r1, [sp, #12]
	cmp r1, #0
	beq .L_08026d06
	mov r0, r8
	movs r1, #8
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_08026d38
.L_08026d06:
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_08026d30
	ldr r3, .L_08026dcc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	movs r5, #9
	cmp r3, #0
	bne .L_08026d26
	movs r5, #22
.L_08026d26:
	mov r0, r8
	adds r1, r5, #0
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_08026d38
.L_08026d30:
	mov r0, r8
	ldr r1, [sp, #8]
	bl ObjectDispatch_ApplyArgumentToChildren
.L_08026d38:
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_08026d86
	movs r3, #128
	lsls r3, r3, #24
	mov r0, r8
	str r3, [r0, #56]
	str r3, [r0, #60]
	str r3, [r0, #64]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_08026d76
	ldr r2, [sp, #4]
	ldrh r1, [r0, #6]
	lsrs r3, r2, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_08026d68
	adds r3, r2, #0
.L_08026d68:
	ldr r2, .L_08026dc8
	cmp r3, r2
	bge .L_08026d70
	adds r3, r2, #0
.L_08026d70:
	adds r3, r1, r3
	mov r0, r8
	strh r3, [r0, #6]
.L_08026d76:
	mov r3, r8
	adds r3, #100
	movs r2, #0
	strh r2, [r3]
	mov r2, r8
	adds r2, #102
	movs r3, #2
	b .L_08026da4
.L_08026d86:
	add r3, sp, #88
	ldr r1, [r3]
	ldr r2, [r3, #4]
	mov r0, r8
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	mov r2, r8
	adds r2, #100
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldrh r1, [r2]
	cmp r3, #0
	beq .L_08026da6
	subs r3, r1, #1
.L_08026da4:
	strh r3, [r2]
.L_08026da6:
	bl Func_08026e60
	mov r1, r8
	ldrh r3, [r1, #4]
	mov r2, r8
	adds r3, #1
	movs r0, #1
	strh r3, [r2, #4]
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08026dc4:
	.4byte 0xff000200
.L_08026dc8:
	.4byte 0xfffff000
.L_08026dcc:
	.4byte gPartyState
