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
	.section .bss,"aw",%nobits
	.balign 16
	.global gData_02002fc0
gData_02002fc0:
