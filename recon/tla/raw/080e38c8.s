.syntax unified
	.thumb
	.global Func_080e38c8
	.thumb_func
Func_080e38c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #92]
	sub sp, #28
	mov r8, r2
	adds r2, r3, #0
	adds r2, #224
	ldr r2, [r2]
	str r2, [sp, #12]
	ldr r3, [r3, #108]
	str r3, [sp, #8]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #151
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e39e0
	ldr r3, .L_080e3a6c
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_080e39e0
	movs r3, #0
	movs r2, #192
	mov r11, r3
	movs r7, #192
	lsls r2, r2, #5
	movs r3, #197
	movs r6, #148
	lsls r7, r7, #5
	adds r2, #156
	movs r0, #128
	lsls r3, r3, #5
	lsls r6, r6, #5
	adds r7, #152
	add r2, r8
	lsls r0, r0, #5
	add r3, r8
	add r6, r8
	add r7, r8
	mov r10, r2
	add r0, r8
	mov r9, r3
.L_080e3934:
	ldr r3, [r6, #24]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_080e39d2
	ldr r3, [sp, #12]
	adds r5, r0, #0
	ldr r0, [r3, #20]
	cmp r0, #0
	bne .L_080e394c
	ldr r2, [sp, #12]
	ldr r0, [r2, #16]
.L_080e394c:
	bl Func_080db9cc
	strh r0, [r5, #30]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #164
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e3974
	ldr r3, .L_080e3a70
	mov r2, r10
	str r3, [r7]
	movs r3, #33
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #5
	b .L_080e3986
.L_080e3974:
	movs r3, #230
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r7]
	mov r2, r10
	movs r3, #25
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #6
.L_080e3986:
	mov r2, r9
	str r3, [r2]
	movs r3, #196
	lsls r3, r3, #5
	add r3, r8
	ldr r3, [r3]
	movs r2, #0
	str r3, [r6]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #132
	add r3, r8
	ldr r3, [r3]
	str r3, [r6, #4]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #136
	add r3, r8
	ldr r3, [r3]
	str r2, [r6, #20]
	str r3, [r6, #8]
	movs r3, #0
	str r3, [r6, #12]
	ldr r3, .L_080e3a74
	adds r2, r6, #0
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #144
	add r3, r8
	ldrh r1, [r3]
	ldr r0, [r7]
	adds r2, #12
	bl Func_0801489c
	movs r3, #0
	str r3, [r6, #24]
	b .L_080e39e0
.L_080e39d2:
	movs r2, #1
	add r11, r2
	mov r3, r11
	adds r0, #40
	adds r6, #28
	cmp r3, #15
	ble .L_080e3934
.L_080e39e0:
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #150
	movs r3, #0
	add r2, r8
	strb r3, [r2]
	mov r11, r3
	movs r3, #128
	lsls r3, r3, #5
	movs r2, #182
	add r3, r8
	movs r6, #148
	lsls r2, r2, #5
	str r3, [sp, #4]
	lsls r6, r6, #5
	add r2, r8
	add r6, r8
	add r7, sp, #16
	mov r9, r2
.L_080e3a06:
	ldr r2, [r6, #24]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	bne .L_080e3a12
	b .L_080e3bdc
.L_080e3a12:
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #61
	muls r3, r2
	ldr r5, [sp, #4]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #20]
	str r3, [r5, #24]
	movs r1, #192
	ldr r3, [r6, #24]
	lsls r1, r1, #5
	adds r1, #140
	add r1, r8
	asrs r3, r3, #1
	movs r2, #3
	ldrh r1, [r1]
	add r3, r11
	ands r3, r2
	lsls r3, r3, #5
	adds r1, r1, r3
	ldr r3, .L_080e3a64
	ldr r2, .L_080e3a68
	ands r1, r3
	ldrh r3, [r5, #8]
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #144
	add r3, r8
	ldrh r3, [r3]
	movs r2, #128
	lsls r2, r2, #8
	cmp r3, r2
	bls .L_080e3a96
	ldrh r3, [r5, #30]
	subs r3, #1
	b .L_080e3a9a
.L_080e3a64:
	.4byte 0x000003ff
.L_080e3a68:
	.4byte 0xfffffc00
.L_080e3a6c:
	.4byte Data_0300122c
.L_080e3a70:
	.4byte 0x00026666
.L_080e3a74:
	.4byte 0xfffecccd
.L_080e3a78:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #150
	add r3, r8
	strb r4, [r3]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #148
	add r3, r8
	mov r2, r10
	strh r2, [r3]
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
	b .L_080e3b12
.L_080e3a96:
	ldrh r3, [r5, #30]
	adds r3, #1
.L_080e3a9a:
	strh r3, [r5, #30]
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_080eb298
	ldr r3, [r6, #24]
	movs r2, #192
	adds r3, #1
	str r3, [r6, #24]
	lsls r2, r2, #5
	adds r2, #156
	add r2, r8
	ldr r2, [r2]
	cmp r3, r2
	bne .L_080e3abe
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
.L_080e3abe:
	ldr r5, [sp, #8]
	movs r3, #0
	mov r10, r3
	adds r5, #20
.L_080e3ac6:
	ldmia r5!, {r1}
	cmp r1, #0
	beq .L_080e3b08
	ldr r3, [r1]
	cmp r3, #0
	beq .L_080e3b08
	adds r3, r1, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r4, #1
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080e3b08
	ldr r2, [sp, #12]
	ldr r3, [r2, #16]
	cmp r1, r3
	beq .L_080e3b08
	ldr r3, [r2, #20]
	cmp r1, r3
	beq .L_080e3b08
	ldrh r3, [r1, #32]
	adds r2, r1, #0
	adds r2, #8
	subs r3, #2
	adds r0, r6, #0
	movs r1, #2
	str r4, [sp, #0]
	bl Func_080dbe80
	ldr r4, [sp, #0]
	cmp r0, #0
	bge .L_080e3a78
.L_080e3b08:
	movs r3, #1
	add r10, r3
	mov r2, r10
	cmp r2, #79
	ble .L_080e3ac6
.L_080e3b12:
	movs r3, #196
	lsls r3, r3, #5
	add r3, r8
	ldr r1, [r6]
	ldr r3, [r3]
	asrs r2, r1, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_080e3b38
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #136
	add r3, r8
	ldr r2, [r6, #8]
	ldr r3, [r3]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_080e3b56
.L_080e3b38:
	str r1, [r7]
	adds r1, r7, #0
	ldr r3, [r6, #4]
	str r3, [r7, #4]
	ldr r3, [r6, #8]
	str r3, [r7, #8]
	ldr r3, [sp, #12]
	ldr r0, [r3, #16]
	bl Func_08020210
	cmp r0, #0
	ble .L_080e3b56
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
.L_080e3b56:
	ldr r3, [r6, #24]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_080e3bba
	movs r3, #1
	mov r2, r11
	ands r3, r2
	cmp r3, #0
	beq .L_080e3bba
	ldr r3, [r6]
	str r3, [r7]
	bl Random16
	ldr r3, [r6, #4]
	lsls r0, r0, #2
	subs r3, r3, r0
	str r3, [r7, #4]
	movs r0, #160
	ldr r3, [r6, #8]
	adds r2, r7, #0
	str r3, [r7, #8]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #144
	add r3, r8
	ldrh r1, [r3]
	movs r3, #128
	lsls r3, r3, #8
	adds r1, r1, r3
	lsls r0, r0, #12
	bl Func_0801489c
	bl Random16
	adds r1, r0, #0
	movs r0, #192
	adds r2, r7, #0
	lsls r0, r0, #12
	bl Func_0801489c
	ldr r3, [r7]
	mov r2, r9
	str r3, [r2]
	ldr r3, [r7, #4]
	str r3, [r2, #4]
	ldr r3, [r7, #8]
	str r3, [r2, #8]
	movs r3, #0
	str r3, [r2, #24]
.L_080e3bba:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #150
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080e3bf4
	movs r3, #197
	lsls r3, r3, #5
	add r3, r8
	ldr r2, [r3]
	adds r0, r6, #0
	movs r1, #64
	bl BattleFx_IntegrateVector3
.L_080e3bdc:
	ldr r2, [sp, #4]
	movs r3, #28
	add r9, r3
	movs r3, #1
	adds r2, #40
	add r11, r3
	str r2, [sp, #4]
	mov r2, r11
	adds r6, #28
	cmp r2, #15
	bgt .L_080e3bf4
	b .L_080e3a06
.L_080e3bf4:
	movs r3, #192
	lsls r3, r3, #5
	movs r6, #182
	adds r3, #142
	movs r5, #162
	lsls r6, r6, #5
	add r3, r8
	lsls r5, r5, #5
	movs r2, #15
	add r6, r8
	mov r10, r3
	add r7, sp, #16
	add r5, r8
	mov r11, r2
.L_080e3c10:
	ldr r2, [r6, #24]
	cmp r2, #0
	blt .L_080e3c7a
	movs r3, #7
	asrs r2, r2, #2
	ands r2, r3
	mov r3, r10
	ldrh r1, [r3]
	ldr r3, .L_080e3c5c
	lsls r2, r2, #3
	adds r1, r1, r2
	ands r1, r3
	ldr r2, .L_080e3c60
	ldrh r3, [r5, #8]
	adds r0, r7, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	ldr r3, [r6]
	str r3, [r7]
	ldr r3, [r6, #4]
	str r3, [r7, #4]
	ldr r3, [r6, #8]
	str r3, [r7, #8]
	bl Func_080dc390
	ldr r3, [r7]
	adds r0, r5, #0
	str r3, [r5, #12]
	ldr r3, [r7, #8]
	str r3, [r5, #16]
	bl Func_080eb01c
	ldr r3, [r6, #4]
	movs r2, #204
	lsls r2, r2, #8
	b .L_080e3c64
	.2byte 0x0000
.L_080e3c5c:
	.4byte 0x000003ff
.L_080e3c60:
	.4byte 0xfffffc00
.L_080e3c64:
	adds r2, #204
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
	cmp r3, #32
	bne .L_080e3c7a
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
.L_080e3c7a:
	movs r3, #1
	negs r3, r3
	add r11, r3
	mov r2, r11
	adds r5, #40
	adds r6, #28
	cmp r2, #0
	bge .L_080e3c10
	ldr r3, [sp, #12]
	movs r4, #192
	ldrh r2, [r3, #2]
	ldr r3, .L_080e3cf4
	lsls r4, r4, #5
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #146
	adds r4, #144
	add r3, r8
	add r4, r8
	ldrh r1, [r3]
	ldrh r0, [r4]
	lsls r2, r2, #16
	lsrs r2, r2, #16
	subs r3, r1, r0
	subs r1, r1, r2
	subs r2, r0, r2
	lsls r1, r1, #16
	lsls r2, r2, #16
	lsrs r2, r2, #16
	lsrs r1, r1, #16
	subs r1, r1, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #8
	asrs r3, r3, #16
	cmp r1, r2
	bne .L_080e3cce
	movs r2, #128
	lsls r2, r2, #3
	adds r3, r0, r2
	b .L_080e3ce2
.L_080e3cce:
	movs r2, #128
	lsls r2, r2, #3
	cmp r3, r2
	ble .L_080e3cd8
	adds r3, r2, #0
.L_080e3cd8:
	ldr r2, .L_080e3cf8
	cmp r3, r2
	bge .L_080e3ce0
	adds r3, r2, #0
.L_080e3ce0:
	adds r3, r0, r3
.L_080e3ce2:
	strh r3, [r4]
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e3cf4:
	.4byte 0xffffc000
.L_080e3cf8:
	.4byte 0xfffffc00
