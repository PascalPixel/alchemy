.syntax unified
	.thumb
	.section .text.x0200814c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #100
	mov r8, r2
	ldrh r2, [r6]
	ldr r7, [r3, #108]
	movs r3, #0
	mov r10, r3
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02008176
	movs r0, #10
	b .L_ja_02008178
.L_ja_02008176:
	movs r0, #9
.L_ja_02008178:
	bl Actor_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl Func_02000088
	cmp r0, #0
	bne .L_ja_020081ca
	ldr r3, .L_ja_020081d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	movs r2, #176
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_ja_020081b2
	mov r2, r8
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_ja_020081c0
.L_ja_020081b2:
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_020081c0
	movs r3, #1
	mov r10, r3
.L_ja_020081c0:
	adds r0, r5, #0
	movs r2, #24
	mov r3, r10
	bl Func_02000088
.L_ja_020081ca:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_020081d4:
	.4byte gMainRoutineWork
	.endif
	.section .text.x0200a2f8,"ax",%progbits
	.balign 4
	.global Func_020022f8
	.thumb_func
Func_020022f8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #8
	bl Actor_GetById
	adds r6, r0, #0
	ldr r2, [r6, #80]
	movs r0, #192
	lsls r0, r0, #2
	mov r8, r2
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #48]
	movs r0, #227
	bl Func_020025a0
	movs r1, #176
	movs r2, #160
	movs r3, #184
	adds r0, r6, #0
	lsls r1, r1, #16
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Func_02002450
	movs r7, #0
	movs r5, #15
.L_0200a33c:
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r0, #1
	subs r3, r3, r7
	strh r3, [r2, #18]
	subs r5, #1
	bl wait
	adds r7, #40
	cmp r5, #0
	bge .L_0200a33c
	movs r1, #168
	movs r2, #128
	movs r3, #184
	adds r0, r6, #0
	lsls r1, r1, #16
	lsls r2, r2, #13
	lsls r3, r3, #16
	bl Func_02002450
	movs r7, #200
	lsls r7, r7, #1
	movs r5, #25
.L_0200a36a:
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r0, #1
	subs r3, r3, r7
	strh r3, [r2, #18]
	subs r5, #1
	bl wait
	adds r7, #40
	cmp r5, #0
	bge .L_0200a36a
	adds r0, r6, #0
	bl Func_02002458
	movs r0, #2
	bl FieldEvent_Wait
	movs r0, #240
	bl Func_020025a0
	movs r5, #0
	mov r3, r8
	strh r5, [r3, #18]
	movs r3, #168
	lsls r3, r3, #16
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #16
	str r3, [r6, #16]
	movs r2, #12
	movs r3, #10
	str r5, [r6, #12]
	str r5, [r6, #40]
	str r5, [r6, #36]
	movs r0, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02002470
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.balign 16
	.global gData_02002fc0
gData_02002fc0:
