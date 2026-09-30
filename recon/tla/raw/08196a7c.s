.syntax unified
	.thumb
	.global Func_08196a7c
	.thumb_func
Func_08196a7c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #0
	sub sp, #8
	adds r6, r0, #0
	mov r8, r3
	movs r7, #0
	b .L_08196d48
.L_08196a90:
	subs r3, r1, #1
	cmp r3, #10
	bls .L_08196a98
	b .L_08196d2c
.L_08196a98:
	ldr r2, .L_08196d70
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08196aa0:
	.4byte .L_08196acc
	.4byte .L_08196b18
	.4byte .L_08196b36
	.4byte .L_08196c0e
	.4byte .L_08196c54
	.4byte .L_08196c9a
	.4byte .L_08196ce0
	.4byte .L_08196b54
	.4byte .L_08196b92
	.4byte .L_08196bd0
	.4byte .L_08196afa
.L_08196acc:
	cmp r1, r7
	beq .L_08196af0
	ldr r5, .L_08196d74
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196d78
.L_08196ae8:
	mov r1, r8
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196af0:
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r1, [r6]
	b .L_08196d2c
.L_08196afa:
	cmp r1, r7
	beq .L_08196af0
	ldr r5, .L_08196d7c
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196d80
	b .L_08196ae8
.L_08196b18:
	cmp r1, r7
	beq .L_08196af0
	ldr r5, .L_08196d84
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196d88
	b .L_08196ae8
.L_08196b36:
	cmp r1, r7
	beq .L_08196af0
	ldr r5, .L_08196d8c
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196d90
	b .L_08196ae8
.L_08196b54:
	cmp r1, r7
	beq .L_08196b70
	ldr r0, .L_08196d94
	bl Runtime_BumpAllocate
	movs r3, #128
	mov r8, r0
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196d98
	mov r1, r8
	ldr r2, .L_08196d9c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196b70:
	ldr r3, [r6, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	bne .L_08196b8c
	mov r5, sp
	mov r0, r10
	adds r1, r5, #0
	bl Func_08196a28
	cmp r0, #0
	beq .L_08196b8e
	str r5, [r6, #16]
	b .L_08196b8e
.L_08196b8c:
	mov r5, sp
.L_08196b8e:
	adds r0, r6, #0
	b .L_08196d24
.L_08196b92:
	cmp r1, r7
	beq .L_08196bae
	ldr r0, .L_08196da0
	bl Runtime_BumpAllocate
	movs r3, #128
	mov r8, r0
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196da4
	mov r1, r8
	ldr r2, .L_08196da8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196bae:
	ldr r3, [r6, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	bne .L_08196bca
	mov r5, sp
	mov r0, r10
	adds r1, r5, #0
	bl Func_08196a28
	cmp r0, #0
	beq .L_08196bcc
	str r5, [r6, #16]
	b .L_08196bcc
.L_08196bca:
	mov r5, sp
.L_08196bcc:
	adds r0, r6, #0
	b .L_08196d24
.L_08196bd0:
	cmp r1, r7
	beq .L_08196bec
	ldr r0, .L_08196dac
	bl Runtime_BumpAllocate
	movs r3, #128
	mov r8, r0
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196db0
	mov r1, r8
	ldr r2, .L_08196db4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196bec:
	ldr r3, [r6, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	bne .L_08196c08
	mov r5, sp
	mov r0, r10
	adds r1, r5, #0
	bl Func_08196a28
	cmp r0, #0
	beq .L_08196c0a
	str r5, [r6, #16]
	b .L_08196c0a
.L_08196c08:
	mov r5, sp
.L_08196c0a:
	adds r0, r6, #0
	b .L_08196d24
.L_08196c0e:
	cmp r1, r7
	beq .L_08196c32
	ldr r5, .L_08196db8
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196dbc
	mov r1, r8
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196c32:
	ldr r3, [r6, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	bne .L_08196c4e
	mov r5, sp
	mov r0, r10
	adds r1, r5, #0
	bl Func_08196a28
	cmp r0, #0
	beq .L_08196c50
	str r5, [r6, #16]
	b .L_08196c50
.L_08196c4e:
	mov r5, sp
.L_08196c50:
	adds r0, r6, #0
	b .L_08196d24
.L_08196c54:
	cmp r1, r7
	beq .L_08196c78
	ldr r5, .L_08196dc0
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196dc4
	mov r1, r8
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196c78:
	ldr r3, [r6, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	bne .L_08196c94
	mov r5, sp
	mov r0, r10
	adds r1, r5, #0
	bl Func_08196a28
	cmp r0, #0
	beq .L_08196c96
	str r5, [r6, #16]
	b .L_08196c96
.L_08196c94:
	mov r5, sp
.L_08196c96:
	adds r0, r6, #0
	b .L_08196d24
.L_08196c9a:
	cmp r1, r7
	beq .L_08196cbe
	ldr r5, .L_08196dc8
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196dcc
	mov r1, r8
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196cbe:
	ldr r3, [r6, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	bne .L_08196cda
	mov r5, sp
	mov r0, r10
	adds r1, r5, #0
	bl Func_08196a28
	cmp r0, #0
	beq .L_08196cdc
	str r5, [r6, #16]
	b .L_08196cdc
.L_08196cda:
	mov r5, sp
.L_08196cdc:
	adds r0, r6, #0
	b .L_08196d24
.L_08196ce0:
	cmp r1, r7
	beq .L_08196d04
	ldr r5, .L_08196dd0
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	mov r8, r0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_08196dd4
	mov r1, r8
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08196d04:
	ldr r3, [r6, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	bne .L_08196d20
	mov r5, sp
	mov r0, r10
	adds r1, r5, #0
	bl Func_08196a28
	cmp r0, #0
	beq .L_08196d22
	str r5, [r6, #16]
	b .L_08196d22
.L_08196d20:
	mov r5, sp
.L_08196d22:
	adds r0, r6, #0
.L_08196d24:
	mov lr, r8
	.2byte 0xf800
	ldr r1, [r6]
	b .L_08196d2e
.L_08196d2c:
	mov r5, sp
.L_08196d2e:
	ldr r3, [r6, #16]
	adds r7, r1, #0
	cmp r3, r5
	bne .L_08196d46
	ldr r0, [r3, #4]
	movs r3, #4
	negs r3, r3
	ands r0, r3
	bl Sys_Free
	mov r3, r10
	str r3, [r6, #16]
.L_08196d46:
	adds r6, #28
.L_08196d48:
	ldr r3, [r6, #16]
	ldr r1, [r6]
	mov r10, r3
	cmp r1, r7
	beq .L_08196d60
	mov r3, r8
	cmp r3, #0
	beq .L_08196d60
	mov r0, r8
	bl Sys_Free
	ldr r1, [r6]
.L_08196d60:
	cmp r1, #0
	beq .L_08196d66
	b .L_08196a90
.L_08196d66:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08196d70:
	.4byte .L_08196aa0
.L_08196d74:
	.4byte 0x00000200
.L_08196d78:
	.4byte Data_08138fc8
.L_08196d7c:
	.4byte 0x00000200
.L_08196d80:
	.4byte Data_0813b7a0
.L_08196d84:
	.4byte 0x00000210
.L_08196d88:
	.4byte Data_081391c8
.L_08196d8c:
	.4byte 0x00000220
.L_08196d90:
	.4byte Data_081393d8
.L_08196d94:
	.4byte 0x000004b4
.L_08196d98:
	.4byte Data_081395f8
.L_08196d9c:
	.4byte 0x8400012d
.L_08196da0:
	.4byte 0x000004d8
.L_08196da4:
	.4byte Data_0813ae14
.L_08196da8:
	.4byte 0x84000136
.L_08196dac:
	.4byte 0x000004b4
.L_08196db0:
	.4byte Data_0813b2ec
.L_08196db4:
	.4byte 0x8400012d
.L_08196db8:
	.4byte 0x000004c4
.L_08196dbc:
	.4byte Data_08139aac
.L_08196dc0:
	.4byte 0x000004d4
.L_08196dc4:
	.4byte Data_08139f70
.L_08196dc8:
	.4byte 0x000004e0
.L_08196dcc:
	.4byte Data_0813a444
.L_08196dd0:
	.4byte 0x000004f0
.L_08196dd4:
	.4byte Data_0813a924
