.syntax unified
	.thumb
	.global Func_08016854
	.thumb_func
Func_08016854:
	push {r5, r6, lr}
	ldr r5, .L_0801688c
	ldr r6, .L_08016890
	ldr r4, [r5]
	cmp r4, #0
	beq .L_08016866
	movs r0, #1
	negs r0, r0
	b .L_08016884
.L_08016866:
	ldr r2, .L_08016894
	ldrh r1, [r2]
	strh r2, [r2]
	movs r3, #129
	strb r3, [r6, #1]
	ldr r3, .L_08016898
	str r0, [r5]
	strh r4, [r3]
	movs r3, #1
	strb r3, [r6]
	ldr r0, .L_08016888
	ldr r3, .L_0801689c
	strb r0, [r3]
	strh r1, [r2]
	movs r0, #0
.L_08016884:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08016888:
	.4byte 0x00000000
.L_0801688c:
	.4byte Data_020055d0
.L_08016890:
	.4byte Data_02003a70
.L_08016894:
	.4byte 0x04000208
.L_08016898:
	.4byte Data_02005354
.L_0801689c:
	.4byte Data_020054c4
