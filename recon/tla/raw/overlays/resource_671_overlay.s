.syntax unified
	.thumb
	.section .text.x0200cd48,"ax",%progbits
	.balign 4
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r2, r11
	sub sp, #8
	adds r2, #1
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl Actor_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	adds r3, r7, #0
	adds r3, #100
	mov r2, r9
	strh r2, [r3]
	movs r2, #7
	mov r10, r2
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	movs r2, #1
	mov r8, r2
	subs r3, #4
	movs r5, #2
	mov r2, r8
	strb r5, [r3]
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	ldr r0, [sp, #0]
	ldr r6, [sp, #0]
	bl Actor_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #8
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #2
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #10
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005624
	movs r2, #0
	mov r10, r2
.L_0200ce40:
	movs r3, #1
	mov r9, r3
	movs r5, #0
.L_0200ce46:
	movs r2, #4
	add r2, r11
	mov r8, r2
	mov r6, r11
	cmp r11, r8
	bge .L_0200ce7c
.L_0200ce52:
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200ce6c
	cmp r3, #7
	beq .L_0200ce6c
	movs r3, #0
	mov r9, r3
.L_0200ce6c:
	adds r2, r6, #0
	adds r0, r7, #0
	adds r1, r5, #0
	adds r6, #1
	bl Func_02004938
	cmp r6, r8
	blt .L_0200ce52
.L_0200ce7c:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #15
	ble .L_0200ce46
	mov r2, r9
	cmp r2, #0
	bne .L_0200ce98
	movs r3, #1
	add r10, r3
	mov r2, r10
	cmp r2, #127
	ble .L_0200ce40
.L_0200ce98:
	movs r0, #20
	bl wait
	ldr r3, [sp, #4]
	movs r0, #3
	cmp r3, #0
	beq .L_0200cf32
	mov r0, r11
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r6, #0
	movs r5, #0
	cmp r3, #0
	beq .L_0200ceb8
	ldr r6, [r0, #76]
.L_0200ceb8:
	ldr r0, [sp, #0]
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200ced0
	ldr r0, [r0, #76]
	cmp r6, #0
	beq .L_0200cece
	cmp r6, r0
	bls .L_0200ced0
.L_0200cece:
	adds r6, r0, #0
.L_0200ced0:
	mov r0, r11
	adds r0, #2
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cee0
	ldr r5, [r0, #76]
.L_0200cee0:
	mov r0, r11
	adds r0, #3
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cefa
	ldr r0, [r0, #76]
	cmp r5, #0
	beq .L_0200cef8
	cmp r5, r0
	bls .L_0200cefa
.L_0200cef8:
	adds r5, r0, #0
.L_0200cefa:
	cmp r6, #0
	beq .L_0200cf2a
	movs r0, #1
	cmp r5, #0
	beq .L_0200cf32
	movs r0, #0
	cmp r6, r5
	beq .L_0200cf32
	cmp r6, r5
	bcs .L_0200cf1c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl GameFlag_SetBit
	movs r0, #1
	b .L_0200cf32
.L_0200cf1c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl GameFlag_SetBit
	movs r0, #2
	b .L_0200cf32
.L_0200cf2a:
	movs r0, #2
	cmp r5, #0
	bne .L_0200cf32
	movs r0, #3
.L_0200cf32:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .bss,"aw",%nobits
	.global Data_02006fc8
Data_02006fc8:
	.space 0x00000020
	.global Data_02006fe8
Data_02006fe8:
	.space 0x00000020
	.global Data_02007008
Data_02007008:
	.space 0x00000020
	.global Data_02007028
Data_02007028:
	.space 0x00000020
	.global Data_02007048
Data_02007048:
	.space 0x00000020
	.global gData_02007068
gData_02007068:
