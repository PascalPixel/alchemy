.syntax unified
	.thumb
	.section .text.x02008300,"ax",%progbits
	.global Func_02000300
	.thumb_func
Func_02000300:
	push {r5, r6, r7, lr}
	movs r7, #0
	movs r6, #0
.L_02008306:
	movs r0, #168
	movs r1, #0
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #2
	bl Func_02001dc0
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	bl Random16Far
	movs r3, #192
	lsls r3, r3, #11
	lsls r0, r0, #2
	adds r0, r0, r3
	str r0, [r5, #48]
	bl Random16Far
	movs r3, #128
	lsls r0, r0, #11
	lsls r3, r3, #4
	lsrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, .L_02008380
	str r0, [r5, #52]
	str r3, [r5, #108]
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r3, #2
	strh r6, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r7, #1
	strb r3, [r1, #9]
	adds r6, #5
	cmp r7, #7
	bne .L_02008306
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008380:
	.4byte Func_02000244
	.section .text.x020085d6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200921e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009256,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009a1e,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global gData_02002718
gData_02002718:
	.space 0x00000004
	.global gData_0200271c
gData_0200271c:
	.space 0x00000020
	.global gData_0200273c
gData_0200273c:
	.space 0x00000004
	.global gData_02002740
gData_02002740:
	.space 0x0000001c
	.global gData_0200275c
gData_0200275c:
	.space 0x00000002
	.global gData_0200275e
gData_0200275e:
	.space 0x00000002
	.global gData_02002760
gData_02002760:
	.space 0x00000040
	.global gData_020027a0
gData_020027a0:
	.space 0x000000c0
	.global gData_02002860
gData_02002860:
	.space 0x00000002
	.global gData_02002862
gData_02002862:
	.space 0x00000002
	.global gData_02002864
gData_02002864:
