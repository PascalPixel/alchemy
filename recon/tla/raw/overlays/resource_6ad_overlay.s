.syntax unified
	.thumb
	.section .text.x02008170,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {lr}
	bl Func_02005688
	pop {pc}
	.else
	.balign 4
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {r5, r6, lr}
	ldr r3, .L_020081ec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Actor_GetById
	ldrh r3, [r0, #6]
	movs r2, #12
	lsrs r6, r3, #12
	adds r3, r6, #2
	ands r3, r2
	lsls r6, r3, #12
	ldr r3, [r0, #8]
	mov r5, sp
	str r3, [r5]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	ldr r2, .L_020081f0
	adds r1, r6, #0
	ands r3, r2
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005680
	cmp r0, #0
	beq .L_020081e2
	ldr r3, [r0, #8]
	adds r1, r6, #0
	str r3, [r5]
	adds r2, r5, #0
	ldr r3, [r0, #12]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005680
	cmp r0, #0
	bne .L_020081e6
.L_020081e2:
	bl Func_02005688
.L_020081e6:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081ec:
	.4byte gMainRoutineWork
.L_020081f0:
	.4byte 0xfff80000
	.endif
	.endif
	.section .text.x0200855c,"ax",%progbits
	.balign 4
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Actor_GetById
	movs r1, #11
	mov r8, r0
	mov r11, r1
.L_02008574:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r8
	ldr r6, [r2, #8]
	lsls r5, r5, #4
	adds r6, r6, r5
	lsls r0, r0, #4
	subs r6, r6, r0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r1, r8
	ldr r2, [r1, #12]
	lsls r5, r5, #3
	adds r5, r5, r2
	ldr r2, [r1, #16]
	adds r3, r0, #0
	lsls r3, r3, #4
	movs r0, #74
	adds r3, r3, r2
	adds r1, r6, #0
	adds r0, #255
	adds r2, r5, #0
	bl Func_02005498
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200864c
	bl Random16Far
	movs r2, #224
	lsls r2, r2, #8
	lsrs r0, r0, #2
	adds r7, r0, r2
	bl Random16Far
	lsls r0, r0, #3
	mov r10, r0
	bl Random16Far
	movs r3, #128
	lsrs r0, r0, #1
	lsls r3, r3, #7
	adds r3, r3, r0
	mov r9, r3
	bl Random16Far
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #255
	cmp r0, r1
	bhi .L_020085f0
	adds r0, r6, #0
	ldr r1, .L_02008664
	bl Func_02005490
	b .L_020085f8
.L_020085f0:
	adds r0, r6, #0
	ldr r1, .L_02008668
	bl Func_02005490
.L_020085f8:
	movs r1, #0
	adds r0, r6, #0
	bl Actor_SetSingleChildFlags
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r7, #2
	adds r3, r3, r2
	str r3, [r6, #40]
	adds r0, r7, #0
	bl Math_Cosine
	ldr r5, .L_0200866c
	adds r1, r0, #0
	mov r0, r10
	mov lr, r5
	.2byte 0xf800
	str r0, [r6, #44]
	adds r0, r7, #0
	bl Math_Sine
	adds r1, r0, #0
	mov r0, r10
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [r6, #52]
	mov r3, r9
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r6, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r6, #36]
	str r3, [r6, #68]
	adds r0, r6, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_0200864c:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bge .L_02008574
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008664:
	.4byte gData_02005b68
.L_02008668:
	.4byte gData_02005bac
.L_0200866c:
	.4byte IwramMulQ16
	.section .text.x02009ffc,"ax",%progbits
	.balign 4
	.global Func_02001ffc
	.thumb_func
Func_02001ffc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200a154
	ldr r2, .L_0200a158
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_0200a058
	adds r3, #15
.L_0200a058:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200a0e8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001d5c
	cmp r0, #0
	beq .L_0200a0e8
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0200a0e8
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_0200a0bc
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_0200a0b4
	movs r3, #1
	b .L_0200a0ba
.L_0200a0b4:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_0200a0ba:
	strh r3, [r1]
.L_0200a0bc:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_0200a0e8:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_0200a148
	mov r5, r8
	adds r5, #84
.L_0200a0f8:
	bl Actor_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200a13a
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001dc0
	cmp r0, #0
	beq .L_0200a13a
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_0200a134
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_0200a13a
.L_0200a134:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_0200a13a:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_0200a148
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_0200a0f8
.L_0200a148:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a154:
	.4byte gMainRoutineWork
.L_0200a158:
	.4byte gData_020023c4 + 0x188
	.section .text.x0200a800,"ax",%progbits
	.balign 4
	.global Func_02002800
	.thumb_func
Func_02002800:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02005690
	mov r8, r0
	bl Actor_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a842
.L_0200a826:
	ldr r2, .L_0200a8e8
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Unit_Get
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_0200a826
.L_0200a842:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02005650
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_020055c0
	cmp r5, r7
	bge .L_0200a8dc
.L_0200a86e:
	ldr r1, .L_0200a8e8
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl Unit_Get
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a896
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a8d6
.L_0200a896:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a8d6
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a8d6:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a86e
.L_0200a8dc:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a8e8:
	.4byte gMainRoutineWork
	.section .text.x0200a97c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_0200297c
	.thumb_func
Func_0200297c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a84c
	b .L_ja_0200a9ac
.L_ja_0200a84c:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002800
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r2, .L_ja_0200a9c0
	adds r5, r0, #0
	mov r10, r2
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	ldr r3, .L_ja_0200a9c4
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02005480
.L_ja_0200a884:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	bl Func_020054d8
	mov r6, r10
	mov r7, r10
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a8fc
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #36]
	mov r8, r2
	movs r1, #0
	ldrsh r3, [r7, r1]
	lsls r3, r3, #17
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
.L_ja_0200a8ba:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_020054f8
	cmp r0, #0
	beq .L_ja_0200a8ea
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a9a0
.L_ja_0200a8ea:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a8ba
	b .L_ja_0200a9a0
.L_ja_0200a8fc:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_020054f8
	cmp r0, #0
	bgt .L_ja_0200a9a0
	cmp r0, #0
	bge .L_ja_0200a942
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a9bc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_020055a8
	b .L_ja_0200a9a0
.L_ja_0200a942:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a9c8
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a9cc
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a96e
	adds r2, r4, #0
.L_ja_0200a96e:
	ldr r0, .L_ja_0200a9d0
	cmp r2, r0
	bge .L_ja_0200a976
	adds r2, r0, #0
.L_ja_0200a976:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a9cc
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a988
	adds r2, r4, #0
.L_ja_0200a988:
	cmp r2, r0
	bge .L_ja_0200a98e
	adds r2, r0, #0
.L_ja_0200a98e:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, r10
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_0200a884
.L_ja_0200a9a0:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a9ac:
	mov r0, r10
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a9bc:
	.4byte gMainRoutineWork
.L_ja_0200a9c0:
	.4byte gData_020023c4 + 0x188
.L_ja_0200a9c4:
	.4byte Func_02002954
.L_ja_0200a9c8:
	.4byte 0x000fffff
.L_ja_0200a9cc:
	.4byte 0xfff80000
.L_ja_0200a9d0:
	.4byte 0xffff0000
	.else
	.balign 4
	.global Func_0200297c
	.thumb_func
Func_0200297c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200ab70
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a99e
	b .L_0200ab5c
.L_0200a99e:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002800
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r2, .L_0200ab74
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	ldr r3, .L_0200ab78
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_02005480
.L_0200a9d6:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_020054d8
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200aa52
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200aa10:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_020054f8
	cmp r0, #0
	beq .L_0200aa44
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200ab50
.L_0200aa44:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #9
	ble .L_0200aa10
	b .L_0200ab50
.L_0200aa52:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_020054f8
	cmp r0, #0
	bgt .L_0200ab50
	cmp r0, #0
	bge .L_0200aa9c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200ab70
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_020055a8
	b .L_0200ab50
.L_0200aa9c:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200aab0:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200aae0
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200aae0
	cmp r5, r6
	beq .L_0200aae0
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_02005538
	cmp r0, #0
	blt .L_0200aae0
	movs r0, #1
	bl wait
	b .L_0200ab50
.L_0200aae0:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200aab0
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200ab7c
	lsls r3, r3, #17
	ldr r0, .L_0200ab80
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200ab1e
	adds r2, r4, #0
.L_0200ab1e:
	ldr r0, .L_0200ab84
	cmp r2, r0
	bge .L_0200ab26
	adds r2, r0, #0
.L_0200ab26:
	subs r3, r1, r2
	ldr r1, .L_0200ab80
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200ab38
	adds r2, r4, #0
.L_0200ab38:
	cmp r2, r0
	bge .L_0200ab3e
	adds r2, r0, #0
.L_0200ab3e:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_0200a9d6
.L_0200ab50:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200ab5c:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200ab70:
	.4byte gMainRoutineWork
.L_0200ab74:
	.4byte gData_020023c4 + 0x188
.L_0200ab78:
	.4byte Func_02002954
.L_0200ab7c:
	.4byte 0x000fffff
.L_0200ab80:
	.4byte 0xfff80000
.L_0200ab84:
	.4byte 0xffff0000
	.endif
	.section .text.x0200ad08,"ax",%progbits
	.balign 4
	.global Func_02002d08
	.thumb_func
Func_02002d08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200ae70
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200ad24
	b .L_0200ae7c
.L_0200ad24:
	movs r2, #0
	str r2, [sp, #0]
.L_0200ad28:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02005498
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ae5c
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200ae74
	adds r0, r7, #0
	mov r9, r2
	bl Func_02005490
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200ae78
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200ae70
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200ae70
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200ae6c
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200ae5c:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200ae68
	b .L_0200ad28
.L_0200ae68:
	b .L_0200ae7c
	.2byte 0x0000
.L_0200ae6c:
	.4byte 0xfffffc00
.L_0200ae70:
	.4byte gData_020023c4 + 0x188
.L_0200ae74:
	.4byte gData_02005dbc
.L_0200ae78:
	.4byte IwramMulQ16
.L_0200ae7c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200af80,"ax",%progbits
	.balign 4
	.global Func_02002f80
	.thumb_func
Func_02002f80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #192
	movs r0, #100
	lsls r3, r3, #18
	adds r0, r0, r5
	ldr r6, [r3, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #56
	mov r8, r0
	cmp r3, #0
	beq .L_0200afa8
	b .L_0200b234
.L_0200afa8:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0200afc2
	movs r3, #1
	orrs r0, r3
.L_0200afc2:
	adds r3, r5, #0
	adds r3, #91
	strb r0, [r3]
	add r7, sp, #44
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #12
	ldr r3, [r5, #12]
	adds r2, r7, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	ldrh r1, [r5, #6]
	bl Vector_AddPolarOffsetFar
	ldr r1, [r7]
	ldr r2, [r7, #8]
	movs r0, #0
	bl Func_020054d8
	cmp r0, #7
	bne .L_0200b038
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #64]
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	mov r0, r8
	movs r3, #1
	strh r3, [r0]
	movs r0, #145
	bl Func_020056c0
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl Func_02005510
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005510
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_0200b038
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_0200b038:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r5, #8]
	ldr r1, [r5, #16]
	asrs r3, r3, #20
	str r3, [sp, #32]
	movs r3, #184
	lsls r3, r3, #1
	ldr r4, [sp, #32]
	asrs r1, r1, #20
	adds r2, r0, r3
	ldr r2, [r2]
	lsls r3, r1, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [sp, #28]
	movs r4, #212
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r2, [r2]
	subs r4, #92
	adds r2, r2, r3
	str r2, [sp, #24]
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	asrs r3, r3, #20
	str r3, [sp, #20]
	adds r3, r0, r4
	adds r4, #52
	ldr r2, [r3]
	adds r3, r0, r4
	ldr r3, [r3]
	adds r4, #4
	asrs r3, r3, #20
	str r3, [sp, #16]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r0, #1
	asrs r3, r3, #20
	adds r3, r1, r3
	subs r3, #2
	asrs r2, r2, #20
	negs r0, r0
	str r3, [sp, #8]
	str r0, [sp, #40]
	subs r3, r1, #1
	adds r1, r1, r2
	subs r1, #1
	mov r8, r3
	mov r11, r1
.L_0200b0a4:
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	ldr r2, [sp, #20]
	movs r4, #1
	adds r3, r0, r1
	subs r3, #1
	negs r4, r4
	mov r9, r3
	str r4, [sp, #36]
	adds r3, r0, r2
	adds r6, r0, #0
	subs r3, #1
	subs r6, #1
	mov r10, r3
.L_0200b0c0:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	str r3, [sp, #12]
	adds r2, r3, r1
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200b120
	movs r3, #0
	strb r3, [r2, #2]
	mov r2, r10
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_020054f0
	mov r4, r8
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_020054e8
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020056c0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002e8c
.L_0200b120:
	ldr r4, [sp, #12]
	ldr r0, [sp, #24]
	adds r2, r4, r0
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200b174
	movs r3, #0
	strb r3, [r2, #2]
	ldr r2, [sp, #8]
	mov r1, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_020054f0
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r6, [sp, #0]
	bl Func_020054e8
	movs r0, #143
	lsls r0, r0, #2
	bl Func_020056c0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002e8c
.L_0200b174:
	ldr r0, [sp, #36]
	movs r4, #1
	adds r0, #1
	add r9, r4
	adds r6, #1
	add r10, r4
	str r0, [sp, #36]
	cmp r0, #1
	ble .L_0200b0c0
	ldr r1, [sp, #8]
	ldr r2, [sp, #40]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	add r8, r4
	add r11, r4
	str r2, [sp, #40]
	cmp r2, #1
	ble .L_0200b0a4
	ldr r3, [r5, #24]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	bge .L_0200b1b2
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200b1b2:
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	bl Func_020054b8
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200b278
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r6, .L_0200b27c
	bl Actor_GetById
	ldr r1, [r5, #8]
	ldr r3, [r0, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200b1e8
	movs r1, #160
	lsls r1, r1, #13
	cmp r2, r1
	blt .L_0200b1f2
	b .L_0200b268
.L_0200b1e8:
	movs r2, #160
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_0200b268
.L_0200b1f2:
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	ldr r4, .L_0200b280
	ldr r1, .L_0200b284
	subs r3, r3, r2
	adds r3, r3, r4
	cmp r3, r1
	bhi .L_0200b268
	ldr r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_0200b216
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_0200b220
	b .L_0200b268
.L_0200b216:
	movs r4, #160
	subs r3, r0, r3
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_0200b268
.L_0200b220:
	movs r3, #2
	strh r3, [r6, #4]
	ldrh r3, [r6, #10]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, #2
	adds r2, r7, r0
	str r5, [r6, #108]
	strh r3, [r2]
	b .L_0200b268
.L_0200b234:
	cmp r3, #1
	bne .L_0200b268
	adds r3, r5, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0200b25a
	ldr r2, .L_0200b288
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	bl Func_02002d08
	b .L_0200b268
.L_0200b25a:
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r5, #8]
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r2, [r5, #108]
.L_0200b268:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b278:
	.4byte gMainRoutineWork
.L_0200b27c:
	.4byte gData_020023c4 + 0x188
.L_0200b280:
	.4byte 0x0007ffff
.L_0200b284:
	.4byte 0x001ffffe
.L_0200b288:
	.4byte 0xfffff000
	.section .text.x0200b304,"ax",%progbits
	.balign 4
	.global Func_02003304
	.thumb_func
Func_02003304:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Actor_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200b394
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200b390
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Actor_SetPositionAndResetMotion
	ldr r3, .L_0200b398
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200b39c
	mov r2, r9
	adds r6, r6, r3
	b .L_0200b3a0
	.2byte 0x0000
.L_0200b390:
	.4byte 0x00000000
.L_0200b394:
	.4byte 0xfff40000
.L_0200b398:
	.4byte Func_02002f80
.L_0200b39c:
	.4byte 0xffffc000
.L_0200b3a0:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_020056c0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c3ec,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
	.global Func_020043ec
	.thumb_func
Func_020043ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200c298
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_0200c29c
	ldr r3, .L_ja_0200c294
	adds r7, r0, #0
	strh r3, [r2]
.L_ja_0200c25e:
	bl Func_0200410c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_ja_0200c2a0
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_ja_0200c2a4
.L_ja_0200c294:
	.4byte 0x00000000
.L_ja_0200c298:
	.4byte gMainRoutineWork
.L_ja_0200c29c:
	.4byte gData_020074c8
.L_ja_0200c2a0:
	.4byte 0xfff00000
.L_ja_0200c2a4:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_020054d0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_0200c310
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_ja_0200c320
	ldr r3, .L_ja_0200c314
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_ja_0200c318
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_0200c31c
	cmp r3, r2
	bne .L_ja_0200c34c
	b .L_ja_0200c514
.L_ja_0200c310:
	.4byte IwramMulQ16
.L_ja_0200c314:
	.4byte gInput
.L_ja_0200c318:
	.4byte gData_02005f38
.L_ja_0200c31c:
	.4byte 0xffff0000
.L_ja_0200c320:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_ja_0200c348
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_0200c34c
.L_ja_0200c348:
	.4byte 0xffffc000
.L_ja_0200c34c:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	beq .L_ja_0200c3ce
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200c3ce
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_020054b8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005480
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_020054c0
	ldr r3, .L_ja_0200c524
	str r3, [r7, #108]
	b .L_ja_0200c4aa
.L_ja_0200c3ce:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_0200c4f6
.L_ja_0200c3e2:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200c4ca
	ldr r3, .L_ja_0200c528
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200c4ca
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_ja_0200c41a:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_ja_0200c444
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200c444
	cmp r5, r7
	beq .L_ja_0200c444
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02005538
	cmp r0, #0
	bge .L_ja_0200c4ca
.L_ja_0200c444:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_0200c41a
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_020054b8
	b .L_ja_0200c482
.L_ja_0200c47a:
	movs r0, #1
	bl wait
	adds r5, #1
.L_ja_0200c482:
	cmp r5, #179
	bgt .L_ja_0200c49a
	adds r0, r7, #0
	bl Func_02005530
	cmp r0, #0
	bne .L_ja_0200c49a
	ldr r3, .L_ja_0200c528
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_0200c47a
.L_ja_0200c49a:
	ldr r3, .L_ja_0200c528
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200c4f6
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_ja_0200c4f0
.L_ja_0200c4aa:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	bne .L_ja_0200c3e2
.L_ja_0200c4ca:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_020054b8
	adds r0, r7, #0
	bl Func_020054c0
	movs r0, #2
	bl wait
	b .L_ja_0200c25e
.L_ja_0200c4f0:
	movs r0, #10
	bl wait
.L_ja_0200c4f6:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005480
.L_ja_0200c514:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200c524:
	.4byte Func_02004080
.L_ja_0200c528:
	.4byte gData_020074c8
	.else
	.global Func_020043ec
	.thumb_func
Func_020043ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200c44c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200c450
	ldr r3, .L_0200c448
	adds r7, r0, #0
	strh r3, [r2]
.L_0200c412:
	bl Func_0200410c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200c454
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_0200c458
.L_0200c448:
	.4byte 0x00000000
.L_0200c44c:
	.4byte gMainRoutineWork
.L_0200c450:
	.4byte gData_020074c8
.L_0200c454:
	.4byte 0xfff00000
.L_0200c458:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_020054d0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200c4c4
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200c4d4
	ldr r3, .L_0200c4c8
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200c4cc
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200c4d0
	cmp r3, r2
	bne .L_0200c500
	b .L_0200c6ca
.L_0200c4c4:
	.4byte IwramMulQ16
.L_0200c4c8:
	.4byte gInput
.L_0200c4cc:
	.4byte gData_02005f38
.L_0200c4d0:
	.4byte 0xffff0000
.L_0200c4d4:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200c4fc
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200c500
.L_0200c4fc:
	.4byte 0xffffc000
.L_0200c500:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	beq .L_0200c57a
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200c57a
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_020054b8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005480
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	movs r5, #0
	b .L_0200c5a2
.L_0200c57a:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200c6ca
.L_0200c58e:
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200c59a
	b .L_0200c6ca
.L_0200c59a:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200c5a2:
	cmp r5, #179
	bgt .L_0200c5b0
	adds r0, r7, #0
	bl Func_02005530
	cmp r0, #0
	beq .L_0200c58e
.L_0200c5b0:
	ldr r3, .L_0200c6fc
	str r3, [r7, #108]
	b .L_0200c67e
.L_0200c5b6:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200c69e
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200c6ca
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200c5ee:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200c618
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200c618
	cmp r5, r7
	beq .L_0200c618
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02005538
	cmp r0, #0
	bge .L_0200c69e
.L_0200c618:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200c5ee
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_020054b8
	b .L_0200c656
.L_0200c64e:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200c656:
	cmp r5, #179
	bgt .L_0200c66e
	adds r0, r7, #0
	bl Func_02005530
	cmp r0, #0
	bne .L_0200c66e
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200c64e
.L_0200c66e:
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200c6ca
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200c6c4
.L_0200c67e:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	bne .L_0200c5b6
.L_0200c69e:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_020054b8
	adds r0, r7, #0
	bl Func_020054c0
	movs r0, #2
	bl wait
	b .L_0200c412
.L_0200c6c4:
	movs r0, #10
	bl wait
.L_0200c6ca:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005480
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c6f8:
	.4byte gData_020074c8
.L_0200c6fc:
	.4byte Func_02004080
	.endif
	.section .text.x0200c938,"ax",%progbits
	.balign 4
	.global Func_02004938
	.thumb_func
Func_02004938:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	mov r8, r3
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_0200ca3c
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_0200c97e
.L_0200c964:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_0200c97e
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_0200c964
.L_0200c97e:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200c98a
	movs r0, #0
	b .L_0200ca32
.L_0200c98a:
	ldr r6, .L_0200ca40
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200c998
	negs r1, r4
.L_0200c998:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_0200c9a2
	negs r3, r3
.L_0200c9a2:
	adds r3, r1, r3
	asrs r7, r3, #4
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_0200c9b2
	negs r5, r1
.L_0200c9b2:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_0200c9bc
	negs r2, r2
.L_0200c9bc:
	adds r5, r5, r2
	mov r10, r5
	ldr r6, [r0, #8]
	mov r3, r10
	ldr r5, [r0, #16]
	asrs r3, r3, #4
	mov r10, r3
	lsls r3, r4, #16
	adds r6, r6, r3
	lsls r3, r1, #16
	adds r5, r5, r3
	movs r3, #164
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	asrs r6, r6, #20
	asrs r1, r3, #20
	movs r3, #166
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	lsls r2, r1, #16
	asrs r3, r3, #20
	lsls r3, r3, #16
	asrs r5, r5, #20
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r6, r2
	adds r3, r5, r3
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	mov r3, r10
	bl Func_020054e8
	movs r3, #255
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02004a44
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02004a44
	movs r0, #1
.L_0200ca32:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200ca3c:
	.4byte gData_02005f58
.L_0200ca40:
	.4byte gData_02005f64
	.section .text.x0200ca8c,"ax",%progbits
	.balign 4
	.global Func_02004a8c
	.thumb_func
Func_02004a8c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r5, r6, #0
	sub sp, #40
	adds r1, r6, #0
	adds r5, #12
	add r0, sp, #24
	adds r1, #16
	adds r2, r5, #0
	bl Func_02004bf4
	adds r4, r0, #0
	cmp r4, #0
	bne .L_0200cab6
	b .L_0200cbd6
.L_0200cab6:
	ldr r5, [r5]
	ldr r0, .L_0200cbe8
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200cac6
	negs r2, r2
.L_0200cac6:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200cad0
	negs r3, r3
.L_0200cad0:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_0200cae0
	negs r2, r2
.L_0200cae0:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200caea
	negs r3, r3
.L_0200caea:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_0200cbec
	ldr r1, .L_0200cbf0
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r9, r1
	mov r2, r9
	ands r2, r3
	lsls r3, r3, #16
	mov r10, r3
	movs r3, #0
	str r3, [r6, #20]
	mov r11, r3
	adds r3, r4, #0
	adds r3, #34
	str r3, [sp, #8]
	ldr r1, [sp, #8]
	movs r3, #2
	strb r3, [r1]
	mov r9, r2
	ldr r3, [r4, #8]
	add r3, r9
	str r3, [r6]
	ldr r3, [r4, #16]
	add r3, r10
	str r3, [r6, #8]
	ldr r3, [r4, #12]
	str r3, [sp, #32]
.L_0200cb28:
	ldr r3, [sp, #20]
	ldr r2, .L_0200cbe8
	lsls r3, r3, #2
	str r3, [sp, #4]
	adds r3, #1
	ldrsb r2, [r2, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #12]
	movs r1, #0
	mov r8, r1
	str r3, [sp, #36]
	cmp r8, r2
	bge .L_0200cb96
.L_0200cb46:
	ldr r3, .L_0200cbe8
	ldr r1, [sp, #4]
	add r5, sp, #28
	ldrsb r2, [r3, r1]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r7, r2
	bge .L_0200cb80
.L_0200cb5e:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_020054f8
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_0200cba8
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_0200cb5e
.L_0200cb80:
	add r2, sp, #28
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r3, [sp, #12]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	blt .L_0200cb46
.L_0200cb96:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_0200cb28
.L_0200cba8:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_0200cbd8
	mov r1, r9
	ldr r3, [r4, #8]
	mov r2, r11
	muls r2, r1
	adds r3, r3, r2
	str r3, [r6]
	movs r0, #1
	ldr r3, [r4, #12]
	str r3, [r6, #4]
	mov r3, r10
	mov r2, r11
	muls r2, r3
	ldr r3, [r4, #16]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_0200cbd8
.L_0200cbd6:
	movs r0, #0
.L_0200cbd8:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cbe8:
	.4byte gData_02005f64
.L_0200cbec:
	.4byte gData_02005f7c
.L_0200cbf0:
	.4byte 0xffff0000
	.section .text.x0200cbf4,"ax",%progbits
	.global Func_02004bf4
	.thumb_func
Func_02004bf4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	str r1, [sp, #4]
	str r2, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200cd00
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Actor_GetById
	adds r7, r0, #0
	ldrh r3, [r7, #6]
	ldr r1, [sp, #8]
	lsrs r3, r3, #12
	str r3, [r1]
	movs r2, #8
	adds r5, #52
	mov r11, r2
	mov lr, r5
.L_0200cc30:
	mov r3, lr
	ldr r6, [r3]
	movs r5, #0
.L_0200cc36:
	ldr r3, [r6, #80]
	ldr r2, .L_0200cd04
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r1, [r3, r0]
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	bne .L_0200ccda
	ldr r0, [sp, #8]
	movs r2, #10
	ldrsh r1, [r7, r2]
	ldr r3, [r0]
	ldr r2, .L_0200cd08
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldr r4, .L_0200cd0c
	asrs r2, r3, #16
	adds r1, r1, r2
	asrs r1, r1, #4
	mov r9, r1
	movs r1, #18
	ldrsh r2, [r7, r1]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r2, r2, r3
	asrs r2, r2, #4
	mov r8, r2
	movs r2, #10
	ldrsh r0, [r6, r2]
	lsls r2, r5, #2
	ldrsb r3, [r4, r2]
	adds r3, r0, r3
	asrs r3, r3, #4
	mov r10, r3
	movs r3, #18
	ldrsh r1, [r6, r3]
	adds r3, r2, #1
	ldrsb r3, [r4, r3]
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r12, r3
	adds r3, r2, #2
	ldrsb r3, [r4, r3]
	adds r2, #3
	adds r0, r0, r3
	ldrsb r3, [r4, r2]
	asrs r0, r0, #4
	adds r1, r1, r3
	asrs r1, r1, #4
	cmp r10, r9
	bgt .L_0200ccda
	cmp r9, r0
	bge .L_0200ccda
	cmp r12, r8
	bgt .L_0200ccda
	cmp r8, r1
	bge .L_0200ccda
	ldr r0, [sp, #0]
	movs r3, #1
	ands r3, r5
	str r5, [r0]
	cmp r3, #0
	beq .L_0200ccc8
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r10, r3
	beq .L_0200ccda
	ldr r2, [sp, #4]
	mov r1, r11
	str r1, [r2]
	adds r0, r6, #0
	b .L_0200ccf0
.L_0200ccc8:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r12, r3
	beq .L_0200ccda
	ldr r0, [sp, #4]
	mov r3, r11
	str r3, [r0]
	adds r0, r6, #0
	b .L_0200ccf0
.L_0200ccda:
	adds r5, #1
	cmp r5, #5
	bls .L_0200cc36
	movs r2, #1
	add r11, r2
	movs r1, #4
	mov r3, r11
	add lr, r1
	cmp r3, #63
	bls .L_0200cc30
	movs r0, #0
.L_0200ccf0:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cd00:
	.4byte gMainRoutineWork
.L_0200cd04:
	.4byte gData_02005f58
.L_0200cd08:
	.4byte gData_02005f7c
.L_0200cd0c:
	.4byte gData_02005f64
	.section .text.x0200cd10,"ax",%progbits
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #88]
	str r1, [sp, #92]
	str r2, [sp, #96]
	str r3, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #133
	str r3, [sp, #28]
	ldr r3, .L_0200cfb0
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r10, r0
	ldr r0, [r0]
	bl Actor_GetById
	mov r8, r0
	ldr r0, [sp, #104]
	bl Actor_GetById
	mov r3, r8
	ldr r3, [r3, #48]
	mov r4, r8
	str r3, [sp, #20]
	adds r6, r0, #0
	ldr r4, [r4, #52]
	mov r0, sp
	adds r0, #32
	str r0, [sp, #12]
	str r4, [sp, #16]
	ldr r2, [sp, #100]
	ldr r3, [r6, #8]
	movs r1, #0
	str r3, [r0]
	mov r9, r1
	ldr r3, [r6, #16]
	mov r1, sp
	adds r1, #44
	str r3, [r0, #8]
	ldr r5, .L_0200cfb4
	str r1, [sp, #8]
	lsls r7, r2, #2
	ldrsb r1, [r5, r7]
	ldr r3, [r6, #8]
	lsls r2, r1, #16
	adds r3, r3, r2
	ldr r2, [sp, #8]
	asrs r3, r3, #20
	str r3, [r2]
	mov lr, r3
	adds r3, r7, #1
	ldrsb r4, [r5, r3]
	ldr r3, [r6, #16]
	ldr r0, [sp, #8]
	lsls r2, r4, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r0, #8]
	adds r0, r1, #0
	mov r12, r3
	cmp r0, #0
	bge .L_0200cda0
	negs r0, r0
.L_0200cda0:
	adds r3, r7, #2
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bge .L_0200cdaa
	negs r1, r1
.L_0200cdaa:
	adds r3, r0, r1
	asrs r3, r3, #4
	adds r1, r4, #0
	str r3, [sp, #24]
	cmp r1, #0
	bge .L_0200cdb8
	negs r1, r1
.L_0200cdb8:
	adds r3, r7, #3
	ldrsb r2, [r5, r3]
	cmp r2, #0
	bge .L_0200cdc2
	negs r2, r2
.L_0200cdc2:
	adds r3, r1, r2
	asrs r3, r3, #4
	str r3, [sp, #0]
	mov r11, r3
	movs r3, #0
	str r3, [sp, #4]
	mov r1, lr
	mov r2, r12
	ldr r3, [sp, #24]
	movs r0, #0
	bl Func_02004a44
	mov r1, r10
	movs r2, #200
	ldr r0, [r1]
	lsls r2, r2, #5
	movs r1, #128
	lsls r1, r1, #8
	adds r2, #153
	bl Actor_SetSpeedParameters
	mov r2, r10
	ldr r0, [r2]
	movs r1, #8
	bl Actor_SetModeById
	movs r0, #15
	bl wait
	ldr r4, [sp, #12]
	ldr r1, [sp, #88]
	ldr r3, [r4]
	ldr r2, [sp, #96]
	subs r1, r1, r3
	ldr r3, [r4, #8]
	asrs r1, r1, #17
	subs r2, r2, r3
	mov r3, r10
	asrs r2, r2, #17
	ldr r0, [r3]
	bl Actor_OffsetPositionAndResetMotion
	mov r4, r10
	ldr r0, [r4]
	bl Actor_GetById
	ldr r3, .L_0200cfb8
	str r3, [r0, #108]
	movs r0, #4
	bl wait
	movs r1, #2
	adds r0, r6, #0
	bl Func_02005480
	movs r0, #239
	bl Func_020056c0
	movs r2, #200
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [sp, #104]
	lsls r1, r1, #8
	adds r2, #153
	bl Actor_SetSpeedParameters
	adds r0, r6, #0
	ldr r1, [sp, #88]
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	bl Func_020054b8
	ldr r3, .L_0200cfb0
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r3, r0
	ldr r0, [r5]
	bl Actor_WaitForMoveById
	ldr r0, [r5]
	movs r1, #2
	bl Actor_SetModeById
	movs r1, #152
	movs r2, #200
	lsls r1, r1, #7
	lsls r2, r2, #5
	ldr r0, [r5]
	adds r1, #204
	adds r2, #153
	bl Actor_SetSpeedParameters
	ldr r2, .L_0200cfbc
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl Actor_OffsetPositionAndResetMotion
	ldr r3, [sp, #108]
	cmp r3, #0
	beq .L_0200ce98
	mov lr, r3
	.2byte 0xf800
.L_0200ce98:
	ldr r0, [r5]
	bl Actor_WaitForMoveById
	movs r1, #1
	ldr r0, [r5]
	bl Actor_SetModeById
	mov r3, r8
	movs r2, #0
	str r2, [r3, #108]
	ldr r4, [sp, #20]
	movs r5, #255
	str r4, [r3, #48]
	ldr r0, [sp, #16]
	str r0, [r3, #52]
	adds r0, r6, #0
	bl Func_020054c0
	movs r0, #149
	lsls r0, r0, #1
	bl Func_020056c0
	movs r0, #213
	bl Func_020056c0
	ldr r2, [r6, #12]
	ldr r1, [sp, #88]
	ldr r3, [sp, #96]
	adds r0, r6, #0
	bl Actor_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Func_02005480
	ldr r1, .L_0200cfb4
	ldr r0, [sp, #88]
	ldrsb r3, [r1, r7]
	adds r2, r7, #1
	lsls r3, r3, #16
	adds r0, r0, r3
	ldrsb r3, [r1, r2]
	mov r10, r1
	ldr r1, [sp, #96]
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r4, [sp, #28]
	asrs r0, r0, #20
	asrs r1, r1, #20
	str r0, [sp, #88]
	str r1, [sp, #96]
	mov r9, r2
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r3, [r3]
	adds r2, #4
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r4, r2
	ldr r6, [r3]
	mov r4, r8
	asrs r6, r6, #20
	adds r3, r4, r0
	adds r2, r6, r1
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r3, r11
	ldr r2, [sp, #24]
	bl Func_020054e8
	mov r0, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r0, [sp, #0]
	ldr r3, [sp, #24]
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02004a44
	mov r3, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r5, [sp, #4]
	bl Func_02004a44
	ldr r0, [sp, #12]
	mov r4, r10
	ldrsb r3, [r4, r7]
	ldr r1, [r0]
	ldr r2, [sp, #8]
	lsls r3, r3, #16
	adds r1, r1, r3
	asrs r1, r1, #20
	str r1, [r2]
	mov r3, r9
	ldrsb r2, [r4, r3]
	ldr r3, [r0, #8]
	ldr r4, [sp, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r4, #8]
	add r8, r1
	adds r6, r6, r3
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #24]
	mov r0, r8
	adds r1, r6, #0
	mov r3, r11
	bl Func_020054e8
	ldr r0, [sp, #8]
	mov r3, r11
	ldr r1, [r0]
	ldr r2, [r0, #8]
	movs r4, #0
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r4, [sp, #4]
	bl Func_02004a44
	bl Func_02005668
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r3}
	add sp, #16
	bx r3
	.2byte 0x0000
.L_0200cfb0:
	.4byte gMainRoutineWork
.L_0200cfb4:
	.4byte gData_02005f64
.L_0200cfb8:
	.4byte Func_02004fc0
.L_0200cfbc:
	.4byte gData_02005f7c
	.section .text.x0200d048,"ax",%progbits
	.balign 4
	.global Func_02005048
	.thumb_func
Func_02005048:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_0200d1dc
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_0200d1e0
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_0200d1e4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_0200d1e8
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_0200d1ec
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_0200d09a
	b .L_0200d1ce
.L_0200d09a:
	ldr r2, .L_0200d1f0
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_0200d0a8
	b .L_0200d1be
.L_0200d0a8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_0200d0b0
	b .L_0200d1be
.L_0200d0b0:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_0200d1f4
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200d126
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_0200d1be
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_0200d1be
	cmp r4, #239
	bgt .L_0200d1be
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200d1f8
	b .L_0200d162
.L_0200d126:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_0200d1be
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_0200d1be
	cmp r4, #175
	bgt .L_0200d1be
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200d1fc
.L_0200d162:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_0200d200
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_0200d1a0
	adds r0, r5, #0
	bl Func_02005698
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_0200d1b4
.L_0200d1a0:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_0200d1b4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02005440
	adds r6, #12
.L_0200d1be:
	ldr r3, .L_0200d1ec
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_0200d1ce
	b .L_0200d09a
.L_0200d1ce:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200d1dc:
	.4byte 0xffff0000
.L_0200d1e0:
	.4byte gData_020074cc
.L_0200d1e4:
	.4byte gResourceTableEntries
.L_0200d1e8:
	.4byte Data_0200f510
.L_0200d1ec:
	.4byte gData_020074ce
.L_0200d1f0:
	.4byte gData_020074d0
.L_0200d1f4:
	.4byte gData_020075d0
.L_0200d1f8:
	.4byte 0x40002000
.L_0200d1fc:
	.4byte 0xc000a000
.L_0200d200:
	.4byte gData_020075d2
	.section .bss,"aw",%nobits
	.global Data_02007064
Data_02007064:
	.space 0x00000004
	.global gData_02007068
gData_02007068:
	.space 0x00000020
	.global gData_02007088
gData_02007088:
	.space 0x00000004
	.global gData_0200708c
gData_0200708c:
	.space 0x0000001c
	.global gData_020070a8
gData_020070a8:
	.space 0x00000420
	.global gData_020074c8
gData_020074c8:
	.space 0x00000004
	.global gData_020074cc
gData_020074cc:
	.space 0x00000002
	.global gData_020074ce
gData_020074ce:
	.space 0x00000002
	.global gData_020074d0
gData_020074d0:
	.space 0x00000040
	.global Data_0200f510
Data_0200f510:
	.space 0x000000c0
	.global gData_020075d0
gData_020075d0:
	.space 0x00000002
	.global gData_020075d2
gData_020075d2:
