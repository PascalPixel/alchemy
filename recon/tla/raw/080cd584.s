.syntax unified
	.thumb
	.global Func_080cd584
	.thumb_func
Func_080cd584:
	push {r5, lr}
	adds r5, r0, #0
	adds r1, r5, #0
	movs r0, #6
	bl Func_080ccd78
	movs r3, #1
	negs r3, r3
	cmp r0, #0
	beq .L_080cd5b6
	ldr r1, [r0, #8]
	cmp r1, #0
	beq .L_080cd5b6
	movs r2, #128
	lsls r2, r2, #9
	cmp r1, r2
	bge .L_080cd5ae
	adds r0, r3, #0
	bl EventRuntime_RunMessage
	b .L_080cd5b4
.L_080cd5ae:
	adds r0, r5, #0
	mov lr, r1
	.2byte 0xf800
.L_080cd5b4:
	movs r3, #0
.L_080cd5b6:
	adds r0, r3, #0
	pop {r5, pc}
	.2byte 0x0000
