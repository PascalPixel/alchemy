.syntax unified
	.thumb
	.global Func_080149f8
	.thumb_func
Func_080149f8:
	push {lr}
	ldr r3, .L_08014a1c
	ldr r4, .L_08014a20
	movs r1, #15
	adds r2, r3, #7
	mov r12, r3
.L_08014a04:
	adds r3, r0, #0
	ands r3, r1
	ldrb r3, [r4, r3]
	lsrs r0, r0, #4
	strb r3, [r2]
	subs r2, #1
	cmp r2, r12
	bge .L_08014a04
	ldr r2, .L_08014a1c
	movs r3, #0
	strb r3, [r2, #8]
	pop {pc}
.L_08014a1c:
	.4byte gNumberTextBuffer
.L_08014a20:
	.4byte Data_08017cd0
