.syntax unified
	.thumb
	.global Func_08014b30
	.thumb_func
Func_08014b30:
	push {r5, lr}
	adds r5, r1, #0
	subs r3, r5, #1
	cmp r3, #7
	bls .L_08014b3c
	movs r5, #8
.L_08014b3c:
	bl Func_080149f8
	ldr r0, .L_08014b4c
	subs r0, r0, r5
	bl Func_08014ae0
	pop {r5, pc}
	.2byte 0x0000
.L_08014b4c:
	.4byte Data_03001258
