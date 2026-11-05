.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
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
