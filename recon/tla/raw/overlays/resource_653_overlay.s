.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009b50,"ax",%progbits
	.global Func_02001b50
	.thumb_func
Func_02001b50:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r9, r0
	ldr r0, [r0]
	bl Actor_GetById
	adds r6, r0, #0
	ldr r2, [r6, #8]
	ldr r1, [r6, #16]
	asrs r3, r2, #20
	asrs r0, r1, #20
	mov r10, r3
	ldr r3, [r6, #12]
	mov r8, r0
	movs r0, #128
	lsls r0, r0, #13
	movs r7, #0
	cmp r3, r0
	bgt .L_02009c76
	ldr r0, .L_02009c84
	adds r3, r2, r0
	movs r2, #240
	lsls r2, r2, #16
	cmp r3, r2
	bhi .L_02009c76
	ldr r0, .L_02009c88
	movs r2, #200
	adds r3, r1, r0
	lsls r2, r2, #15
	cmp r3, r2
	bhi .L_02009c76
	mov r3, r10
	cmp r3, #27
	bne .L_02009bda
	mov r0, r8
	cmp r0, #12
	bne .L_02009bda
	movs r0, #13
	bl Actor_GetById
	mov r2, r9
	adds r5, r0, #0
	ldr r0, [r2]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #13
	movs r1, #4
	bl Actor_SetModeById
	adds r3, r6, #0
	adds r3, #85
	strb r7, [r3]
	movs r0, #130
	ldr r3, [r5, #12]
	lsls r0, r0, #2
	str r3, [r6, #12]
	bl GameFlag_SetBit
	movs r7, #1
.L_02009bda:
	mov r3, r10
	cmp r3, #37
	bne .L_02009c22
	mov r0, r8
	cmp r0, #14
	bne .L_02009c22
	movs r0, #14
	bl Actor_GetById
	ldr r3, .L_02009c80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #14
	movs r1, #4
	bl Actor_SetModeById
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #133
	ldr r3, [r5, #12]
	lsls r0, r0, #1
	str r3, [r6, #12]
	adds r0, #255
	bl GameFlag_SetBit
	movs r7, #1
.L_02009c22:
	cmp r7, #0
	beq .L_02009c30
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009c76
.L_02009c30:
	movs r0, #13
	movs r1, #0
	bl Actor_SetModeById
	movs r1, #0
	movs r0, #14
	bl Actor_SetModeById
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	movs r0, #130
	strb r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #126
	adds r0, #255
	bl GameFlag_ClearBit
.L_02009c76:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009c80:
	.4byte gMainRoutineWork
.L_02009c84:
	.4byte 0xfe700000
.L_02009c88:
	.4byte 0xff4c0000
	.section .text.x0200aec2,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200ba14,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	push {lr}
	ldr r2, .L_ja_0200ba04
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #0
	str r3, [r2]
	subs r3, #13
	ldrb r2, [r1, #23]
	movs r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	bl Func_02005338
	pop {pc}
	.2byte 0x0000
.L_ja_0200ba04:
	.4byte gData_02006294
	.else
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	push {lr}
	ldr r2, .L_0200ba54
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #0
	str r3, [r2]
	subs r3, #13
	ldrb r2, [r1, #23]
	movs r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	bl Func_02005338
	ldr r3, .L_0200ba58
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200ba52
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200ba52:
	pop {pc}
.L_0200ba54:
	.4byte gData_02006294
.L_0200ba58:
	.4byte gMainRoutineWork
	.endif
	.section .bss,"aw",%nobits
	.global gData_02006294
gData_02006294:
	.space 0x00000004
	.global gData_02006298
gData_02006298:
	.space 0x00000004
	.global gData_0200629c
gData_0200629c:
	.space 0x00000004
	.global gData_020062a0
gData_020062a0:
	.space 0x00000004
	.global gData_020062a4
gData_020062a4:
	.space 0x00000004
	.global gData_020062a8
gData_020062a8:
	.space 0x00000002
	.global gData_020062aa
gData_020062aa:
	.space 0x00000002
	.global gData_020062ac
gData_020062ac:
