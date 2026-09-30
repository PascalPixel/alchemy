.syntax unified
	.thumb
	.global Func_0802a52c
	.thumb_func
Func_0802a52c:
	push {lr}
	movs r3, #1
	subs r1, #1
	negs r3, r3
	ldr r2, .L_0802a54c
	cmp r1, r3
	beq .L_0802a54a
	mov r12, r3
.L_0802a53c:
	ldrb r3, [r0]
	subs r1, #1
	ldrb r3, [r2, r3]
	strb r3, [r0]
	adds r0, #1
	cmp r1, r12
	bne .L_0802a53c
.L_0802a54a:
	pop {pc}
.L_0802a54c:
	.4byte Runtime_ByteRemapTable
