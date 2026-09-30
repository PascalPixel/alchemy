.syntax unified
	.thumb
	.global Func_080c9f2c
	.thumb_func
Func_080c9f2c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_080c9fc8
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r3, r1
	ldrb r3, [r3]
	sub sp, #4
	ldr r5, .L_080c9fcc
	cmp r3, #8
	bne .L_080c9f4c
	movs r0, #0
	b .L_080c9fbe
.L_080c9f4c:
	cmp r3, #7
	bne .L_080c9f52
	ldr r5, .L_080c9fd0
.L_080c9f52:
	mov r1, sp
	bl ResourceMetadata_SumCommandLengthsFar + 0x8
	movs r1, #20
	adds r6, r0, #0
	ldr r0, [sp, #0]
	bl Math_ModU
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldr r1, .L_080c9fd4
	movs r2, #0
	str r0, [sp, #0]
	asrs r0, r1, #16
	mov r10, r2
	ldrh r2, [r5]
	cmp r3, r0
	beq .L_080c9fb6
	adds r7, r1, #0
	mov r8, r0
.L_080c9f7a:
	lsls r3, r2, #16
	ldr r2, [sp, #0]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_080c9faa
	movs r2, #2
	ldrsh r3, [r5, r2]
	asrs r2, r7, #16
	cmp r3, r2
	beq .L_080c9f92
	cmp r3, r6
	bne .L_080c9faa
.L_080c9f92:
	movs r3, #4
	ldrsh r0, [r5, r3]
	cmp r0, r2
	beq .L_080c9fa2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080c9faa
.L_080c9fa2:
	movs r2, #6
	ldrsh r1, [r5, r2]
	mov r10, r1
	b .L_080c9fb6
.L_080c9faa:
	adds r5, #8
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, r8
	bne .L_080c9f7a
.L_080c9fb6:
	adds r0, r6, #0
	bl Func_080ca514
	mov r0, r10
.L_080c9fbe:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080c9fc8:
	.4byte gPartyState
.L_080c9fcc:
	.4byte Data_080eedbc
.L_080c9fd0:
	.4byte Data_080eef34
.L_080c9fd4:
	.4byte 0xffff0000
