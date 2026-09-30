.syntax unified
	.thumb
	.global Func_080fad88
	.thumb_func
Func_080fad88:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	ldr r2, .L_080fadac
	adds r3, r5, #0
	adds r3, #62
	mov r12, r5
.L_080fad94:
	strh r2, [r3]
	subs r3, #2
	cmp r3, r12
	bge .L_080fad94
	ldr r3, .L_080fadac
	movs r7, #0
	mov r12, r3
	adds r0, #216
	movs r6, #0
	adds r4, r5, #0
	movs r1, #14
	b .L_080fadb0
.L_080fadac:
	.4byte 0x00000000
.L_080fadb0:
	mov r3, r12
	strh r3, [r6, r5]
	ldrh r2, [r0]
	adds r0, #2
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fadc4
	strh r2, [r4]
	adds r7, #1
	adds r4, #2
.L_080fadc4:
	subs r1, #1
	adds r6, #2
	cmp r1, #0
	bge .L_080fadb0
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
