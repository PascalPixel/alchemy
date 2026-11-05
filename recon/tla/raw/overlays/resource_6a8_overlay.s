.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020083bc,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	adds r5, r0, #0
	adds r6, r5, #0
	ldr r7, [r3, #108]
	adds r6, #100
	movs r3, #0
	mov r8, r2
	mov r10, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_ja_020083e4
	movs r0, #18
	b .L_ja_020083ee
.L_ja_020083e4:
	cmp r3, #1
	bne .L_ja_020083ec
	movs r0, #16
	b .L_ja_020083ee
.L_ja_020083ec:
	movs r0, #17
.L_ja_020083ee:
	bl Actor_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl Func_020002d8
	cmp r0, #0
	bne .L_ja_02008440
	ldr r3, .L_ja_0200844c
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
	bne .L_ja_02008428
	mov r2, r8
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_ja_02008436
.L_ja_02008428:
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02008436
	movs r3, #1
	mov r10, r3
.L_ja_02008436:
	adds r0, r5, #0
	movs r2, #32
	mov r3, r10
	bl Func_020002d8
.L_ja_02008440:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200844c:
	.4byte gMainRoutineWork
	.endif
	.section .bss,"aw",%nobits
	.global gData_020062a4
gData_020062a4:
