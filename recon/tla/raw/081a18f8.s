.syntax unified
	.thumb
	.global Func_081a18f8
	.thumb_func
Func_081a18f8:
	push {lr}
	ldr r3, .L_081a1924
	ldr r3, [r3]
	cmp r0, #0
	bne .L_081a1912
	movs r2, #0
	movs r1, #0
.L_081a1906:
	adds r2, #1
	strb r1, [r3, #4]
	adds r3, #12
	cmp r2, #7
	bls .L_081a1906
	b .L_081a1920
.L_081a1912:
	movs r2, #0
	movs r1, #120
.L_081a1916:
	adds r2, #1
	strb r1, [r3, #4]
	adds r3, #12
	cmp r2, #7
	bls .L_081a1916
.L_081a1920:
	pop {pc}
	.2byte 0x0000
.L_081a1924:
	.4byte Data_0200752c
