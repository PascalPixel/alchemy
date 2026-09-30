.syntax unified
	.thumb
	.global Func_0803f778
	.thumb_func
Func_0803f778:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	movs r5, #0
.L_0803f782:
	bl Func_0803f800
	adds r0, r5, #0
	bl Menu_SelectTopEntry
	adds r5, r0, #0
	bl Func_0803f810
	cmp r5, #4
	bhi .L_0803f7fa
	ldr r2, .L_0803f7fc
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0803f7a0:
	.4byte .L_0803f7b4
	.4byte .L_0803f7c8
	.4byte .L_0803f7d6
	.4byte .L_0803f7e0
	.4byte .L_0803f7ee
.L_0803f7b4:
	bl Func_080c82b0
	cmp r0, #0
	bne .L_0803f7be
	movs r0, #255
.L_0803f7be:
	movs r2, #177
	lsls r2, r2, #1
	adds r3, r6, r2
	strh r0, [r3]
	b .L_0803f7fa
.L_0803f7c8:
	bl Func_080f8008
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0803f7fa
	b .L_0803f782
.L_0803f7d6:
	bl Func_080f8030
	cmp r0, #0
	beq .L_0803f7fa
	b .L_0803f782
.L_0803f7e0:
	bl Func_080f8000
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_0803f7fa
	b .L_0803f782
.L_0803f7ee:
	bl Func_080f8010
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_0803f782
.L_0803f7fa:
	pop {r5, r6, pc}
.L_0803f7fc:
	.4byte .L_0803f7a0
