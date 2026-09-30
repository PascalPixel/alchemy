.syntax unified
	.thumb
	.global Func_081a0fb8
	.thumb_func
Func_081a0fb8:
	push {r5, r6, r7, lr}
	ldr r7, .L_081a1008
	movs r6, #0
	movs r5, #31
.L_081a0fc0:
	adds r1, r6, #0
	ldr r0, [r7]
	movs r2, #1
	subs r5, #1
	bl Func_081a101c
	adds r6, #24
	cmp r5, #0
	bge .L_081a0fc0
	ldr r0, .L_081a100c
	bl Func_08014644
	ldr r0, .L_081a1010
	bl Func_08014644
	ldr r3, .L_081a1014
	movs r2, #128
	ldr r3, [r3]
	movs r5, #0
	movs r1, #0
	lsls r2, r2, #1
.L_081a0fea:
	adds r5, #1
	stmia r3!, {r1}
	cmp r5, r2
	bne .L_081a0fea
	ldr r2, .L_081a1014
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #224
	ldr r0, [r2]
	adds r3, #212
	lsls r1, r1, #19
	ldr r2, .L_081a1018
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r5, r6, r7, pc}
.L_081a1008:
	.4byte Data_081a20bc
.L_081a100c:
	.4byte Func_081a0cac
.L_081a1010:
	.4byte Func_081a0d78
.L_081a1014:
	.4byte Data_02007510
.L_081a1018:
	.4byte 0x84000100
