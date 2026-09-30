.syntax unified
	.thumb
	.global Func_08014220
	.thumb_func
Func_08014220:
	push {lr}
	ldr r1, .L_0801423c
	movs r2, #128
	movs r0, #0
	lsls r2, r2, #2
.L_0801422a:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, #255
	bne .L_08014234
	adds r0, #1
.L_08014234:
	subs r2, #1
	cmp r2, #0
	bne .L_0801422a
	pop {pc}
.L_0801423c:
	.4byte Data_02003410
