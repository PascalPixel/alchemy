.syntax unified
	.thumb
	.global Func_080e1420
	.thumb_func
Func_080e1420:
	push {lr}
	cmp r1, #0
	bne .L_080e1430
	str r1, [r0, #108]
	movs r1, #0
	bl Animation_ApplyChildValuesFar
	b .L_080e143c
.L_080e1430:
	cmp r1, #1
	bne .L_080e1438
	ldr r3, .L_080e1440
	b .L_080e143a
.L_080e1438:
	ldr r3, .L_080e1444
.L_080e143a:
	str r3, [r0, #108]
.L_080e143c:
	pop {pc}
	.2byte 0x0000
.L_080e1440:
	.4byte Func_080e13b0
.L_080e1444:
	.4byte Func_080e13e8
