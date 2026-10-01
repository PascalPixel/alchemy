.syntax unified
	.thumb
	.global ObjectGroup_ApplyRandomChildValues
	.thumb_func
ObjectGroup_ApplyRandomChildValues:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r3, r0, #0
	adds r3, #84
	ldrb r1, [r3]
	sub sp, #4
	cmp r1, #1
	bne .L_080db968
	ldr r2, [r0, #80]
	cmp r2, #0
	beq .L_080db968
	ldrb r3, [r2, #17]
	ands r3, r1
	cmp r3, #0
	bne .L_080db968
	ldrb r1, [r2, #27]
	cmp r1, #0
	beq .L_080db964
	ldr r3, .L_080db970
	adds r7, r2, #0
	ldr r3, [r3]
	adds r7, #40
	mov r8, r3
	adds r6, r1, #0
.L_080db94e:
	mov r0, r8
	movs r1, #6
	str r2, [sp, #0]
	bl __umodsi3
	ldmia r7!, {r5}
	subs r6, #1
	strb r0, [r5, #5]
	ldr r2, [sp, #0]
	cmp r6, #0
	bne .L_080db94e
.L_080db964:
	movs r3, #1
	strb r3, [r2, #25]
.L_080db968:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080db970:
	.4byte Data_0300122c
