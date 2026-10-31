.syntax unified
	.thumb
	.section .text.x0200929c,"ax",%progbits
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_02009308
.L_020092b8:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_02009304
	adds r0, r7, #0
	bl Actor_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_020092e0
	ldr r3, [r6, #28]
	ldr r1, .L_0200931c
	adds r3, r3, r1
	str r3, [r6, #28]
.L_020092e0:
	mov r2, r9
	cmp r2, #1
	bne .L_02009312
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02001320
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_02009312
.L_02009304:
	adds r5, #6
	movs r1, #255
.L_02009308:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_020092b8
.L_02009312:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200931c:
	.4byte 0xffffe100
	.section .bss,"aw",%nobits
	.balign 16
	.global gData_02001be0
gData_02001be0:
	.space 0x00000028
	.global gData_02001c08
gData_02001c08:
