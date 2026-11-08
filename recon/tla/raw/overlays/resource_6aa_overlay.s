.syntax unified
	.thumb
	.section .text.x0200a63c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_0200263c
	.thumb_func
Func_0200263c:
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a7d0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3, #0]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a660
	b .L_ja_0200a7c0
.L_ja_0200a660:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3, #0]
	bl Actor_GetById
	ldr r2, .L_ja_0200a7d4
	adds r5, r0, #0
	mov sl, r2
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r3, .L_ja_0200a7d8
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02004bc0
.L_ja_0200a698:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3, #0]
	ldr r2, [r5, #16]
	bl Func_02004c18
	mov r6, sl
	mov r7, sl
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a710
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
.L_ja_0200a6ce:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	beq .L_ja_0200a6fe
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a7b4
.L_ja_0200a6fe:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a6ce
	b .L_ja_0200a7b4
.L_ja_0200a710:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	bgt .L_ja_0200a7b4
	cmp r0, #0
	bge .L_ja_0200a756
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a7d0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3, #0]
	movs r1, #6
	negs r2, r2
	bl Func_02004ce8
	b .L_ja_0200a7b4
.L_ja_0200a756:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a7dc
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a7e0
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a782
	adds r2, r4, #0
.L_ja_0200a782:
	ldr r0, .L_ja_0200a7e4
	cmp r2, r0
	bge .L_ja_0200a78a
	adds r2, r0, #0
.L_ja_0200a78a:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a7e0
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a79c
	adds r2, r4, #0
.L_ja_0200a79c:
	cmp r2, r0
	bge .L_ja_0200a7a2
	adds r2, r0, #0
.L_ja_0200a7a2:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, sl
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_0200a698
.L_ja_0200a7b4:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a7c0:
	mov r0, sl
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a7d0:
	.4byte gMainRoutineWork
.L_ja_0200a7d4:
	.4byte 0x0200254c
.L_ja_0200a7d8:
	.4byte Func_02002614
.L_ja_0200a7dc:
	.4byte 0x000fffff
.L_ja_0200a7e0:
	.4byte 0xfff80000
.L_ja_0200a7e4:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a9c8,"ax",%progbits
	.balign 4
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200ab30
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a9e4
	b .L_0200ab3c
.L_0200a9e4:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a9e8:
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
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ab1c
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
	ldr r1, .L_0200ab34
	adds r0, r7, #0
	mov r9, r2
	bl Func_02004bd0
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
	ldr r3, .L_0200ab38
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
	ldr r2, .L_0200ab30
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
	ldr r1, .L_0200ab30
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
	ldr r1, .L_0200ab2c
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200ab1c:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200ab28
	b .L_0200a9e8
.L_0200ab28:
	b .L_0200ab3c
	.2byte 0x0000
.L_0200ab2c:
	.4byte 0xfffffc00
.L_0200ab30:
	.4byte gData_020023c4 + 0x188
.L_0200ab34:
	.4byte gData_020056d0
.L_0200ab38:
	.4byte IwramMulQ16
.L_0200ab3c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_02006878
Data_02006878:
	.space 0x00000004
	.global gData_0200687c
gData_0200687c:
	.space 0x00000004
	.global Data_02006880
Data_02006880:
	.space 0x00000004
	.global gData_02006884
gData_02006884:
	.space 0x00000004
	.global gData_0200e888
gData_0200e888:
	.space 0x00000004
	.global gData_0200e88c
gData_0200e88c:
	.space 0x00000004
	.global gData_02006890
gData_02006890:
	.space 0x00000004
	.global Data_02006894
Data_02006894:
	.space 0x00000004
	.global gData_02006898
gData_02006898:
	.space 0x00000004
	.global gData_0200689c
gData_0200689c:
	.space 0x00000004
	.global gData_020068a0
gData_020068a0:
	.space 0x00000002
	.global gData_020068a2
gData_020068a2:
	.space 0x00000002
	.global Data_020068a4
Data_020068a4:
	.space 0x0000000c
	.global gData_020068b0
gData_020068b0:
	.space 0x00000004
	.global Data_020068b4
Data_020068b4:
