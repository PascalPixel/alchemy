.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.balign 4
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	movs r0, #9
	bl Actor_GetById
	mov r10, r0
	movs r0, #10
	bl Actor_GetById
	mov r9, r0
	movs r0, #23
	bl Actor_GetById
	adds r6, r0, #0
	ldr r2, [r6, #12]
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r5, [r6, #80]
	bl Actor_SetPositionAndResetMotion
	mov r2, r8
	cmp r2, #0
	beq .L_0200809a
	cmp r2, #2
	beq .L_0200809a
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002b68
	movs r7, #0
	b .L_02008118
.L_0200809a:
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002b68
	movs r7, #0
.L_020080a4:
	mov r2, r10
	ldr r3, [r2, #12]
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2, #12]
	movs r2, #224
	ldr r3, [r6, #12]
	lsls r2, r2, #3
	adds r2, #174
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_020081b0
	ldr r3, [r6, #8]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r7, #1
	ldrh r3, [r5, #18]
	adds r3, #32
	strh r3, [r5, #18]
	bl wait
	cmp r7, #63
	ble .L_020080a4
	b .L_0200811c
.L_020080dc:
	mov r2, r9
	ldr r3, [r2, #12]
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	mov r2, r9
	str r3, [r2, #12]
	movs r2, #224
	ldr r3, [r6, #12]
	lsls r2, r2, #3
	adds r2, #174
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r2, #160
	ldr r3, [r6, #8]
	lsls r2, r2, #4
	adds r2, #61
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r2, #255
	ldrh r3, [r5, #18]
	lsls r2, r2, #8
	adds r2, #224
	adds r3, r3, r2
	strh r3, [r5, #18]
	movs r0, #1
	bl wait
	adds r7, #1
.L_02008118:
	cmp r7, #63
	ble .L_020080dc
.L_0200811c:
	mov r3, r8
	subs r3, #2
	cmp r3, #1
	bhi .L_020081a4
	movs r0, #30
	bl FieldEvent_Wait
	mov r0, r10
	bl Func_0200105c
	mov r0, r9
	bl Func_0200105c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl Func_02002b68
	movs r7, #255
.L_02008142:
	movs r0, #9
	bl Actor_GetById
	movs r5, #200
	ldr r3, [r0, #12]
	lsls r5, r5, #5
	adds r5, #153
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #10
	bl Actor_GetById
	ldr r3, [r0, #12]
	subs r7, #1
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #1
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	bl wait
	cmp r7, #0
	bge .L_02008142
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02002b68
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #80
	bl Func_02002b68
	movs r0, #10
	bl FieldEvent_Wait
	bl Sound_WaitForJingle
	mov r3, r10
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
	mov r3, r9
	adds r3, #85
	strb r2, [r3]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
.L_020081a4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081b0:
	.4byte 0xfffff5c3
	.section .bss,"aw",%nobits
	.global gData_0200333c
gData_0200333c:
	.space 0x00000004
	.global gData_02003340
gData_02003340:
	.space 0x00000020
	.global gData_02003360
gData_02003360:
	.space 0x00000004
	.global gData_02003364
gData_02003364:
	.space 0x0000001c
	.global gData_02003380
gData_02003380:
	.space 0x00000002
	.global gData_02003382
gData_02003382:
	.space 0x00000002
	.global gData_02003384
gData_02003384:
	.space 0x00000040
	.global Data_020033c4
Data_020033c4:
	.space 0x000000c0
	.global gData_02003484
gData_02003484:
	.space 0x00000002
	.global gData_02003486
gData_02003486:
	.space 0x00000002
	.global gData_02003488
gData_02003488:
