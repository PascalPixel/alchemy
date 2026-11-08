.syntax unified
	.thumb
	.section .text.x0200842a,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.else
	.2byte 0x0000
	.endif
	.section .text.x0200c280,"ax",%progbits
	.global Func_02004280
	.thumb_func
Func_02004280:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200c334
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200c338
	sub sp, #4
	cmp r2, r3
	beq .L_0200c298
	b .L_0200c41c
.L_0200c298:
	movs r3, #192
	movs r2, #241
	lsls r3, r3, #18
	lsls r2, r2, #1
	ldr r7, [r3, #32]
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #4
	ble .L_0200c2ae
	b .L_0200c3ac
.L_0200c2ae:
	adds r2, #50
	adds r3, r1, r2
	ldr r0, [r3]
	bl Unit_Get
	adds r2, r0, #0
	movs r5, #0
	adds r2, #13
.L_0200c2be:
	ldrb r3, [r0]
	adds r0, #1
	adds r5, r5, r3
	cmp r0, r2
	ble .L_0200c2be
	adds r0, r5, #0
	bl Func_020044f8
	bl Func_02004fe4
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r3, #6
	adds r3, #255
	adds r2, r7, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #12
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #2
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200c330
	orrs r3, r2
	strh r3, [r0]
	bl Func_020048a0
	ldr r5, .L_0200c33c
	ldr r3, [r5]
	cmp r3, #3
	bne .L_0200c340
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c340
	movs r1, #208
	movs r2, #176
	movs r0, #64
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02004ebc
	b .L_0200c340
.L_0200c330:
	.4byte 0x00000001
.L_0200c334:
	.4byte gMainRoutineWork
.L_0200c338:
	.4byte 0x000000b1
.L_0200c33c:
	.4byte gData_020023c4 + 0x288
.L_0200c340:
	ldr r3, [r5]
	cmp r3, #9
	bne .L_0200c354
	movs r1, #208
	movs r2, #144
	movs r0, #65
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02004ebc
.L_0200c354:
	ldr r0, [r5]
	movs r2, #165
	lsls r2, r2, #4
	adds r0, r0, r2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c366
	b .L_0200c4c6
.L_0200c366:
	movs r0, #8
	bl Actor_GetById
	adds r6, r0, #0
	bl Func_02004904
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #8
	adds r7, r6, #0
	ands r1, r5
	movs r3, #0
	adds r7, #89
	lsrs r1, r1, #14
	strb r3, [r7]
	adds r1, #2
	adds r0, r6, #0
	bl Func_02004d9c
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r5
	cmp r3, #0
	beq .L_0200c39a
	ldr r3, .L_0200c4cc
	b .L_0200c39c
.L_0200c39a:
	ldr r3, .L_0200c4d0
.L_0200c39c:
	str r3, [r6, #108]
	movs r3, #1
	movs r0, #128
	strb r3, [r7]
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200c4c6
.L_0200c3ac:
	cmp r3, #8
	bgt .L_0200c3b2
	b .L_0200c4c6
.L_0200c3b2:
	ldr r2, .L_0200c4d4
	movs r3, #1
	str r3, [r2]
	movs r3, #188
	lsls r3, r3, #1
	adds r5, r7, r3
	movs r3, #127
	strh r3, [r5, #40]
	movs r1, #128
	movs r3, #254
	movs r2, #0
	lsls r1, r1, #9
	lsls r3, r3, #6
	str r2, [r5, #24]
	str r2, [r5, #28]
	str r2, [r5, #32]
	str r2, [r5, #36]
	str r1, [r5, #16]
	str r1, [r5, #20]
	strh r3, [r5, #42]
	adds r3, r7, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r6, .L_0200c4d8
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	ldr r1, [r5, #20]
	adds r0, r0, r3
	str r0, [r5]
	adds r3, r7, #0
	adds r3, #232
	ldr r0, [r3]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #12]
	adds r0, r0, r3
	str r0, [r5, #4]
	bl Func_02004dc4
	movs r0, #1
	bl wait
	ldr r1, .L_0200c4dc
	ldr r3, .L_0200c4e0
	ldr r0, .L_0200c4e4
	movs r2, #24
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0200c4e4
	bl Func_02004de4
	b .L_0200c4c6
.L_0200c41c:
	ldr r3, .L_0200c4e8
	cmp r2, r3
	bne .L_0200c474
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #9
	bhi .L_0200c4c6
	ldr r2, .L_0200c4ec
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200c43c:
	.4byte .L_0200c4b0
	.4byte .L_0200c4b0
	.4byte .L_0200c46c
	.4byte .L_0200c46c
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c464
	.4byte .L_0200c464
.L_0200c464:
	movs r0, #102
	bl Func_02004274
	b .L_0200c4c6
.L_0200c46c:
	movs r0, #103
	bl Func_02004274
	b .L_0200c4c6
.L_0200c474:
	ldr r3, .L_0200c4f0
	cmp r2, r3
	bne .L_0200c4c6
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #6
	bhi .L_0200c4c6
	ldr r2, .L_0200c4f4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200c494:
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c4b8
	.4byte .L_0200c4b0
	.4byte .L_0200c4c0
	.4byte .L_0200c4b8
.L_0200c4b0:
	movs r0, #100
	bl Func_02004274
	b .L_0200c4c6
.L_0200c4b8:
	movs r0, #101
	bl Func_02004274
	b .L_0200c4c6
.L_0200c4c0:
	movs r0, #102
	bl Func_02004274
.L_0200c4c6:
	movs r0, #0
	add sp, #4
	pop {r5, r6, r7, pc}
.L_0200c4cc:
	.4byte Func_02000aac
.L_0200c4d0:
	.4byte Func_02000a60
.L_0200c4d4:
	.4byte gData_020023c4 + 0x288
.L_0200c4d8:
	.4byte IwramMulQ16
.L_0200c4dc:
	.4byte gData_020053e4
.L_0200c4e0:
	.4byte IwramCopyWords
.L_0200c4e4:
	.4byte Data_0202de00
.L_0200c4e8:
	.4byte 0x000000b2
.L_0200c4ec:
	.4byte .L_0200c43c
.L_0200c4f0:
	.4byte 0x000000b5
.L_0200c4f4:
	.4byte .L_0200c494
	.section .bss,"aw",%nobits
	.global gData_0200646c
gData_0200646c:
	.space 0x00000080
	.global gData_020064ec
gData_020064ec:
	.space 0x00000004
	.global gData_020064f0
gData_020064f0:
	.space 0x00000004
	.global Data_020064f4
Data_020064f4:
	.space 0x00000004
	.global gData_020064f8
gData_020064f8:
