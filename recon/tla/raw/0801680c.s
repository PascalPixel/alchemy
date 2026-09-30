.syntax unified
	.thumb
	.global Func_0801680c
	.thumb_func
Func_0801680c:
	push {r5, r6, r7, lr}
	ldr r5, .L_08016840
	adds r6, r1, #0
	ldr r4, [r5]
	ldr r7, .L_08016844
	cmp r4, #0
	beq .L_08016820
	movs r0, #1
	negs r0, r0
	b .L_0801683c
.L_08016820:
	ldr r2, .L_08016848
	ldrh r1, [r2]
	strh r2, [r2]
	movs r3, #128
	strb r3, [r7, #1]
	ldr r3, .L_0801684c
	str r0, [r5]
	strh r6, [r3]
	ldr r3, .L_08016850
	strb r4, [r3]
	movs r3, #1
	strb r3, [r7]
	strh r1, [r2]
	movs r0, #0
.L_0801683c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08016840:
	.4byte Data_020038d0
.L_08016844:
	.4byte Data_02003a70
.L_08016848:
	.4byte 0x04000208
.L_0801684c:
	.4byte Data_020036d4
.L_08016850:
	.4byte Data_020054c4
