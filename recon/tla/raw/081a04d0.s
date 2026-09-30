.syntax unified
	.thumb
	.global Func_081a04d0
	.thumb_func
Func_081a04d0:
	push {r5, lr}
	sub sp, #4
	cmp r0, #0
	bne .L_081a04e4
	movs r1, #192
	movs r5, #160
	ldr r3, .L_081a0518
	lsls r1, r1, #19
	lsls r5, r5, #19
	b .L_081a04ea
.L_081a04e4:
	ldr r3, .L_081a051c
	ldr r1, .L_081a0520
	ldr r5, .L_081a0524
.L_081a04ea:
	mov r4, sp
	str r3, [r4]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	ldr r2, .L_081a0528
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #0
	str r3, [r4]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r1, r5, #0
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #4
	pop {r5, pc}
.L_081a0518:
	.4byte 0x01010101
.L_081a051c:
	.4byte 0x81818181
.L_081a0520:
	.4byte 0x06008000
.L_081a0524:
	.4byte 0x05000100
.L_081a0528:
	.4byte 0x85001e00
