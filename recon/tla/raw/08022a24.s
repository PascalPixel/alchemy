.syntax unified
	.thumb
	.global Func_08022a24
	.thumb_func
Func_08022a24:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	cmp r6, #0
	beq .L_08022a82
	cmp r5, #0
	beq .L_08022a82
	adds r0, r5, #0
	bl ResourceMetadata_ClearRecord
	ldr r3, [r6, #40]
	movs r0, #0
	cmp r5, r3
	beq .L_08022a52
	adds r2, r6, #0
	adds r2, #40
.L_08022a44:
	adds r0, #1
	cmp r0, #3
	bhi .L_08022a52
	adds r2, #4
	ldr r3, [r2]
	cmp r5, r3
	bne .L_08022a44
.L_08022a52:
	cmp r0, #4
	beq .L_08022a82
	lsls r3, r0, #2
	movs r2, #0
	adds r3, #40
	str r2, [r6, r3]
	adds r2, r0, #1
	movs r4, #0
	cmp r2, #3
	bhi .L_08022a7c
	lsls r3, r2, #2
	adds r3, r3, r6
	adds r1, r3, #0
	adds r1, #40
.L_08022a6e:
	ldmia r1!, {r3}
	cmp r3, #0
	beq .L_08022a76
	adds r4, #1
.L_08022a76:
	adds r2, #1
	cmp r2, #3
	bls .L_08022a6e
.L_08022a7c:
	cmp r4, #0
	bne .L_08022a82
	strb r0, [r6, #27]
.L_08022a82:
	pop {r5, r6, pc}
