.syntax unified
	.thumb
	.global Func_08014b50
	.thumb_func
Func_08014b50:
	push {r5, lr}
	adds r5, r1, #0
	subs r3, r5, #1
	cmp r3, #9
	bls .L_08014b5c
	movs r5, #10
.L_08014b5c:
	bl Text_FormatSignedDecimalToWork
	ldr r0, .L_08014b6c
	subs r0, r0, r5
	bl Func_08014ae0
	pop {r5, pc}
	.2byte 0x0000
.L_08014b6c:
	.4byte Data_0300125a
