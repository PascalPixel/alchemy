.syntax unified
	.thumb
	.global Func_0811d79c
	.thumb_func
Func_0811d79c:
	push {lr}
	adds r3, r0, #0
	movs r0, #0
	cmp r3, #7
	bhi .L_0811d7de
	ldr r2, .L_0811d7e0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0811d7b0:
	.4byte .L_0811d7d0
	.4byte .L_0811d7d0
	.4byte .L_0811d7d8
	.4byte .L_0811d7d8
	.4byte .L_0811d7d0
	.4byte .L_0811d7d0
	.4byte .L_0811d7d8
	.4byte .L_0811d7d0
.L_0811d7d0:
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #1
	b .L_0811d7de
.L_0811d7d8:
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #4
.L_0811d7de:
	pop {pc}
.L_0811d7e0:
	.4byte .L_0811d7b0
