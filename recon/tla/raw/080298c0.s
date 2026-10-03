.syntax unified
	.thumb
	.global Func_080298c0
	.thumb_func
Func_080298c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #92
	adds r7, r0, #0
	movs r0, #0
	str r0, [sp, #28]
	ldr r3, [r7, #12]
	ldr r1, [r7, #16]
	adds r4, r7, #0
	adds r4, #34
	ldr r6, [r7, #8]
	ldr r5, .L_08029c4c
	str r3, [sp, #20]
	str r4, [sp, #12]
	mov r8, r1
	ands r6, r5
	mov r2, r8
	ands r2, r5
	ldrb r0, [r4]
	adds r1, r6, #0
	mov r8, r2
	bl Func_0802d45c
	ldr r1, [sp, #12]
	str r0, [sp, #24]
	add r5, r8
	ldrb r0, [r1]
	adds r2, r5, #0
	adds r1, r6, #0
	bl Func_0802d45c
	adds r2, r7, #0
	adds r2, #100
	str r0, [sp, #16]
	str r2, [sp, #8]
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #0
	beq .L_08029930
	ldr r3, [r7, #56]
	movs r6, #128
	lsls r6, r6, #24
	cmp r3, r6
	beq .L_08029924
	b .L_0802a100
.L_08029924:
	add r0, sp, #28
	ldrh r0, [r0]
	adds r1, r2, #0
	strh r0, [r1]
	ldr r1, [sp, #24]
	str r1, [r7, #20]
.L_08029930:
	adds r2, r7, #0
	adds r2, #102
	str r2, [sp, #4]
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #0
	bne .L_08029940
	b .L_08029c14
.L_08029940:
	ldr r6, [sp, #16]
	ldr r0, [sp, #20]
	cmp r6, r0
	bge .L_08029984
	ldr r5, .L_08029c50
	ldr r1, [sp, #12]
	add r5, r8
	ldrb r2, [r1]
	ldr r0, [r7, #8]
	ldr r3, [r7, #20]
	adds r1, r5, #0
	bl Func_0802d7b0
	cmp r0, #2
	bne .L_08029968
	ldr r3, [r7, #12]
	ldr r2, .L_08029c54
	adds r3, r3, r2
	str r3, [r7, #12]
	b .L_08029984
.L_08029968:
	adds r0, r7, #0
	adds r3, r5, #0
	ldr r1, [r7, #8]
	ldr r2, [sp, #16]
	bl Object_SetMoveTarget
	ldr r4, [sp, #8]
	movs r3, #1
	strh r3, [r4]
	add r6, sp, #28
	ldrh r6, [r6]
	ldr r0, [sp, #4]
	strh r6, [r0]
	b .L_0802a100
.L_08029984:
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	cmp r0, r1
	ble .L_0802999e
	ldr r2, [sp, #4]
	movs r3, #0
	strh r3, [r2]
	adds r2, r7, #0
	adds r2, #85
	movs r3, #3
	str r0, [r7, #20]
	strb r3, [r2]
	b .L_0802a100
.L_0802999e:
	movs r3, #0
	str r3, [sp, #32]
	ldr r3, .L_08029c58
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080299ae
	ldr r3, .L_08029c5c
	ldr r3, [r3]
.L_080299ae:
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	ldr r3, .L_08029c5c
	ldr r1, .L_08029c60
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	movs r0, #255
	lsls r3, r3, #16
	mov r9, r3
	mov r6, r9
	lsrs r6, r6, #16
	lsls r0, r0, #8
	mov r8, r6
	adds r0, #255
	cmp r8, r0
	bne .L_080299e4
	movs r1, #4
	str r1, [sp, #32]
	b .L_08029b12
.L_080299e4:
	ldr r3, [r7, #8]
	add r6, sp, #80
	str r3, [r6]
	ldr r2, [sp, #32]
	movs r3, #128
	lsls r3, r3, #12
	mov r10, r3
	str r2, [r6, #4]
	str r2, [r6, #8]
	mov r0, r10
	mov r1, r8
	adds r2, r6, #0
	bl Vector_AddPolarOffset
	ldr r3, [r7, #12]
	ldr r2, [r6, #8]
	adds r0, r7, #0
	subs r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	movs r1, #3
	str r3, [r6, #8]
	bl ObjectDispatch_ApplyArgumentToChildren
	ldr r3, [r7, #8]
	add r5, sp, #68
	str r3, [r5]
	ldr r4, [sp, #32]
	mov r0, r10
	mov r1, r8
	adds r2, r5, #0
	str r4, [r5, #4]
	str r4, [r5, #8]
	bl Vector_AddPolarOffset
	ldr r3, [r7, #12]
	ldr r2, [r5, #8]
	adds r0, r7, #0
	subs r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r5, #0
	str r3, [r5, #8]
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029afe
	ldr r3, [r7, #8]
	movs r1, #128
	str r3, [r5]
	ldr r0, [sp, #32]
	lsls r1, r1, #5
	str r0, [r5, #4]
	str r0, [r5, #8]
	add r1, r8
	mov r0, r10
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r3, [r7, #12]
	ldr r2, [r5, #8]
	adds r0, r7, #0
	subs r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r5, #0
	str r3, [r5, #8]
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029afe
	ldr r3, [r7, #8]
	mov r0, r10
	str r3, [r5]
	ldr r1, [sp, #32]
	adds r2, r5, #0
	str r1, [r5, #4]
	str r1, [r5, #8]
	ldr r1, .L_08029c64
	add r1, r8
	bl Vector_AddPolarOffset
	ldr r3, [r7, #12]
	ldr r2, [r5, #8]
	adds r0, r7, #0
	subs r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r5, #0
	str r3, [r5, #8]
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029afe
	ldr r3, [r7, #8]
	movs r1, #128
	str r3, [r5]
	ldr r2, [sp, #32]
	lsls r1, r1, #6
	str r2, [r5, #4]
	str r2, [r5, #8]
	add r1, r8
	mov r0, r10
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r3, [r7, #12]
	ldr r2, [r5, #8]
	adds r0, r7, #0
	subs r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r5, #0
	str r3, [r5, #8]
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029afe
	ldr r3, [r7, #8]
	ldr r1, .L_08029c68
	str r3, [r5]
	ldr r3, [sp, #32]
	add r1, r8
	str r3, [r5, #4]
	str r3, [r5, #8]
	mov r0, r10
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r3, [r7, #12]
	ldr r2, [r5, #8]
	adds r0, r7, #0
	subs r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r5, #0
	str r3, [r5, #8]
	bl Func_0802d87c
	cmp r0, #0
	beq .L_08029b12
.L_08029afe:
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	ldr r4, [sp, #32]
	movs r3, #1
	orrs r4, r3
	str r4, [sp, #32]
.L_08029b12:
	ldr r6, [sp, #32]
	cmp r6, #0
	beq .L_08029b28
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	b .L_08029b74
.L_08029b28:
	mov r0, r9
	asrs r0, r0, #16
	str r0, [sp, #28]
	add r3, sp, #80
	ldr r2, [r3, #4]
	ldr r1, [r3]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	ldr r1, [r7, #36]
	ldr r6, .L_08029c6c
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	ldr r1, [r7, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r1, [sp, #32]
	str r1, [r7, #36]
	str r1, [r7, #44]
	ldr r2, [sp, #28]
	lsls r1, r2, #16
	adds r2, r7, #0
	lsrs r1, r1, #16
	adds r2, #36
	bl Vector_AddPolarOffset
	ldr r3, [r7, #44]
	negs r3, r3
	str r3, [r7, #40]
	ldr r3, [sp, #32]
.L_08029b74:
	str r3, [r7, #44]
	ldr r3, .L_08029c70
	ldr r3, [r3]
	mov r8, r3
	mov r4, r8
	movs r3, #15
	ands r4, r3
	mov r8, r4
	cmp r4, #0
	bne .L_08029c0c
	bl Random16
	ldr r1, [r7, #8]
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	movs r6, #192
	subs r1, r1, r3
	lsls r6, r6, #10
	add r4, sp, #36
	adds r1, r1, r6
	str r1, [r4]
	ldr r2, [r7, #12]
	movs r0, #30
	str r2, [r4, #4]
	ldr r3, [r7, #16]
	adds r0, #255
	str r3, [r4, #8]
	bl Func_08023220
	adds r5, r0, #0
	cmp r5, #0
	beq .L_08029c0c
	ldr r3, [r7, #20]
	ldr r1, .L_08029c74
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl ObjectDispatch_Initialize
	ldr r3, .L_08029c78
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	ldr r0, [sp, #32]
	cmp r0, #0
	bne .L_08029bdc
	movs r0, #149
	lsls r0, r0, #2
	bl Audio_PlayCue
.L_08029bdc:
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #61
	str r3, [r5, #72]
	movs r3, #152
	lsls r3, r3, #7
	adds r3, #204
	mov r1, r8
	str r1, [r5, #40]
	str r3, [r5, #24]
	str r3, [r5, #28]
	cmp r6, #0
	beq .L_08029c0c
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	mov r2, r8
	strb r2, [r6, #26]
	ldrb r2, [r6, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r6, #9]
.L_08029c0c:
	adds r0, r7, #0
	bl Func_08029838
	b .L_0802a100
.L_08029c14:
	movs r3, #0
	str r3, [sp, #32]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	ldr r5, .L_08029c5c
	ldr r1, .L_08029c60
	ldr r3, [r5]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	movs r6, #255
	lsls r3, r3, #16
	lsls r6, r6, #8
	lsrs r1, r3, #16
	adds r6, #255
	str r3, [sp, #0]
	cmp r1, r6
	bne .L_08029c7c
	movs r0, #4
	str r0, [sp, #32]
	b .L_08029f68
	.2byte 0x0000
.L_08029c4c:
	.4byte 0xfff00000
.L_08029c50:
	.4byte 0xfff80000
.L_08029c54:
	.4byte 0xffff0000
.L_08029c58:
	.4byte gDebugMode
.L_08029c5c:
	.4byte gInput
.L_08029c60:
	.4byte Data_0802ec5c
.L_08029c64:
	.4byte 0xfffff000
.L_08029c68:
	.4byte 0xffffe000
.L_08029c6c:
	.4byte IwramMulQ16
.L_08029c70:
	.4byte gFrameCount
.L_08029c74:
	.4byte Data_0802ed28
.L_08029c78:
	.4byte Func_08029810
.L_08029c7c:
	ldr r3, [r7, #8]
	add r2, sp, #80
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #12
	str r3, [r2, #8]
	mov r11, r2
	bl Vector_AddPolarOffset
	ldr r3, .L_08029ff4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08029cb0
	ldr r3, [r5]
	ldr r4, [sp, #0]
	movs r2, #128
	lsls r2, r2, #2
	asrs r4, r4, #16
	ands r3, r2
	str r4, [sp, #28]
	cmp r3, #0
	beq .L_08029cb0
	b .L_08029f68
.L_08029cb0:
	adds r0, r7, #0
	mov r1, r11
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029d64
	ldr r3, [r7, #8]
	add r5, sp, #68
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r6, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r6, r6, #12
	str r3, [r5, #8]
	ldr r0, [sp, #0]
	movs r2, #128
	mov r8, r6
	lsls r2, r2, #5
	lsrs r6, r0, #16
	adds r1, r6, r2
	mov r0, r8
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029d64
	ldr r3, [r7, #8]
	mov r0, r8
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	ldr r3, .L_08029ff8
	adds r1, r6, r3
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029d64
	ldr r3, [r7, #8]
	movs r4, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r4, r4, #6
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r1, r6, r4
	mov r0, r8
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029d64
	ldr r3, [r7, #8]
	ldr r0, .L_08029ffc
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r1, r6, r0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	mov r0, r8
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r1, r5, #0
	adds r0, r7, #0
	bl Func_0802d87c
	ldr r1, [sp, #0]
	asrs r1, r1, #16
	str r1, [sp, #28]
	cmp r0, #0
	bne .L_08029d64
	b .L_08029f68
.L_08029d64:
	ldr r4, [sp, #0]
	add r2, sp, #48
	ldr r1, .L_08029ff8
	movs r6, #128
	lsrs r3, r4, #16
	mov r10, r2
	lsls r6, r6, #5
	adds r2, r3, r6
	mov r0, r10
	strh r2, [r0]
	movs r6, #128
	adds r2, r3, r1
	ldr r1, .L_08029ffc
	mov r4, r10
	lsls r6, r6, #6
	strh r2, [r4, #2]
	adds r2, r3, r6
	strh r2, [r0, #4]
	movs r6, #192
	adds r2, r3, r1
	ldr r1, .L_0802a000
	lsls r6, r6, #6
	strh r2, [r4, #6]
	adds r2, r3, r6
	strh r2, [r0, #8]
	adds r3, r3, r1
	mov r2, r10
	strh r3, [r2, #10]
	movs r3, #0
	mov r9, r3
	mov r8, r11
.L_08029da2:
	mov r4, r9
	lsls r3, r4, #1
	mov r6, r10
	ldrsh r6, [r6, r3]
	mov r1, r8
	str r6, [sp, #28]
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r1]
	ldr r3, [r7, #12]
	lsls r0, r0, #12
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	mov r2, r8
	str r3, [r1, #8]
	lsls r3, r6, #16
	lsrs r6, r3, #16
	adds r1, r6, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	mov r1, r8
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029e74
	ldr r3, [r7, #8]
	add r5, sp, #68
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r2, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r2, r2, #5
	movs r0, #128
	adds r1, r6, r2
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029e74
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r0, r0, #12
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	ldr r3, .L_08029ff8
	adds r1, r6, r3
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029e74
	ldr r3, [r7, #8]
	movs r4, #128
	str r3, [r5]
	ldr r3, [r7, #12]
	lsls r4, r4, #6
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	movs r0, #128
	adds r1, r6, r4
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08029e74
	ldr r3, [r7, #8]
	ldr r0, .L_08029ffc
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r1, r6, r0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	beq .L_08029f68
.L_08029e74:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #6
	blt .L_08029da2
	ldr r0, [r7, #8]
	mov r3, r11
	str r0, [r3]
	ldr r3, [r7, #12]
	mov r4, r11
	str r3, [r4, #4]
	ldr r1, [r7, #16]
	movs r5, #1
	str r1, [r4, #8]
	ldr r6, [sp, #32]
	ldr r4, .L_0802a004
	orrs r6, r5
	str r6, [sp, #32]
	movs r2, #64
	ldr r3, [r4]
	ands r3, r2
	cmp r3, #0
	beq .L_08029ef4
	ldr r2, [sp, #20]
	movs r6, #128
	lsls r6, r6, #12
	adds r3, r2, r6
	ldr r2, [sp, #16]
	cmp r2, r3
	ble .L_08029ef4
	ldr r3, .L_0802a008
	movs r2, #2
	ands r0, r3
	ands r1, r3
	ldr r3, .L_0802a00c
	adds r6, r0, r6
	adds r3, r3, r1
	mov r8, r3
	adds r0, r6, #0
	mov r1, r8
	bl Func_080c8770
	cmp r0, #0
	beq .L_08029ef2
	adds r3, r7, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	ldr r3, [r7, #12]
	movs r4, #128
	lsls r4, r4, #9
	adds r3, r3, r4
	str r3, [r7, #12]
	ldr r6, [sp, #4]
	adds r0, r7, #0
	strh r5, [r6]
	movs r1, #3
	str r2, [r7, #36]
	str r2, [r7, #40]
	str r2, [r7, #44]
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_0802a100
.L_08029ef2:
	ldr r4, .L_0802a004
.L_08029ef4:
	ldr r3, [r4]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08029f68
	ldr r2, .L_0802a008
	ldr r3, [r7, #8]
	movs r0, #128
	ands r3, r2
	lsls r0, r0, #12
	adds r6, r3, r0
	ldr r3, [r7, #16]
	movs r1, #192
	ands r3, r2
	lsls r1, r1, #13
	ldr r2, [sp, #12]
	adds r1, r1, r3
	mov r8, r1
	ldrb r0, [r2]
	adds r1, r6, #0
	mov r2, r8
	bl Func_0802d45c
	ldr r3, [sp, #20]
	cmp r0, r3
	bge .L_08029f68
	adds r0, r6, #0
	mov r1, r8
	movs r2, #2
	bl Func_080c8770
	cmp r0, #0
	beq .L_08029f68
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	ldr r4, [sp, #24]
	ldr r6, .L_0802a010
	ldr r1, [r7, #8]
	adds r0, r7, #0
	mov r3, r8
	adds r2, r4, r6
	bl Object_SetMoveTarget
	ldr r0, [sp, #8]
	movs r3, #1
	strh r3, [r0]
	ldr r1, [sp, #4]
	adds r0, r7, #0
	strh r3, [r1]
	str r5, [r7, #36]
	str r5, [r7, #40]
	str r5, [r7, #44]
	movs r1, #3
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_0802a100
.L_08029f68:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_08029f9e
	ldr r3, [sp, #32]
	movs r2, #3
	ands r2, r3
	cmp r2, #0
	beq .L_08029f8a
	movs r4, #194
	lsls r4, r4, #1
	adds r2, r1, r4
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08029f92
.L_08029f8a:
	movs r6, #194
	lsls r6, r6, #1
	adds r3, r1, r6
	strh r2, [r3]
.L_08029f92:
	ldr r3, .L_0802a004
	movs r0, #195
	ldr r3, [r3]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
.L_08029f9e:
	movs r1, #1
	adds r0, r7, #0
	bl ObjectDispatch_ApplyArgumentToChildren
	ldr r1, [sp, #32]
	cmp r1, #0
	beq .L_0802a014
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	movs r3, #3
	ands r3, r1
	cmp r3, #0
	beq .L_08029fe6
	ldr r2, [sp, #0]
	ldrh r1, [r7, #6]
	lsrs r3, r2, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_08029fda
	adds r3, r2, #0
.L_08029fda:
	ldr r2, .L_08029ff8
	cmp r3, r2
	bge .L_08029fe2
	adds r3, r2, #0
.L_08029fe2:
	adds r3, r1, r3
	strh r3, [r7, #6]
.L_08029fe6:
	movs r3, #98
	adds r3, r3, r7
	mov r8, r3
	mov r4, r8
	movs r3, #0
	b .L_0802a064
	.2byte 0x0000
.L_08029ff4:
	.4byte gDebugMode
.L_08029ff8:
	.4byte 0xfffff000
.L_08029ffc:
	.4byte 0xffffe000
.L_0802a000:
	.4byte 0xffffd000
.L_0802a004:
	.4byte gInput
.L_0802a008:
	.4byte 0xfff00000
.L_0802a00c:
	.4byte 0xfff80000
.L_0802a010:
	.4byte 0xfffe0000
.L_0802a014:
	add r3, sp, #80
	ldr r2, [r3, #4]
	ldr r1, [r3]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	ldr r1, [r7, #36]
	ldr r6, .L_0802a118
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	ldr r1, [r7, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r6, [sp, #32]
	str r6, [r7, #36]
	str r6, [r7, #44]
	ldr r2, [sp, #28]
	lsls r1, r2, #16
	adds r2, r7, #0
	adds r2, #36
	lsrs r1, r1, #16
	bl Vector_AddPolarOffset
	movs r3, #98
	adds r3, r3, r7
	ldrb r2, [r3]
	mov r8, r3
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0802a066
	adds r3, #255
	mov r4, r8
.L_0802a064:
	strb r3, [r4]
.L_0802a066:
	mov r6, r8
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_0802a0fa
	ldr r0, [sp, #32]
	cmp r0, #0
	bne .L_0802a0fa
	ldr r3, [r7, #16]
	ldr r4, .L_0802a11c
	movs r0, #178
	lsls r0, r0, #1
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	adds r3, r3, r4
	bl Func_08023220
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0802a0fa
	ldr r3, [r7, #20]
	ldr r1, .L_0802a120
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl ObjectDispatch_Initialize
	adds r2, r5, #0
	adds r2, #35
	movs r3, #8
	add r0, sp, #32
	strb r3, [r2]
	ldrb r0, [r0]
	movs r3, #100
	adds r2, #64
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	movs r0, #149
	lsls r0, r0, #2
	bl Audio_PlayCue
	cmp r6, #0
	beq .L_0802a0f4
	movs r3, #192
	lsls r3, r3, #18
	movs r1, #1
	adds r0, r6, #0
	ldr r5, [r3, #108]
	bl Animation_ApplyChildArgument
	ldrb r3, [r6, #9]
	movs r0, #13
	negs r0, r0
	adds r2, r0, #0
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #74
	add r1, sp, #32
	ldrb r1, [r1]
	adds r5, r5, r3
	ldrb r3, [r5]
	strb r1, [r6, #26]
	movs r1, #3
	ands r3, r1
	lsls r3, r3, #2
	ands r2, r0
	orrs r2, r3
	strb r2, [r6, #9]
.L_0802a0f4:
	movs r3, #5
	mov r4, r8
	strb r3, [r4]
.L_0802a0fa:
	adds r0, r7, #0
	bl Func_08029838
.L_0802a100:
	ldrh r3, [r7, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r7, #4]
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802a118:
	.4byte IwramMulQ16
.L_0802a11c:
	.4byte 0xfffe0000
.L_0802a120:
	.4byte Data_0802ece0
