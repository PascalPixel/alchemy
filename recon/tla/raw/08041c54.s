.syntax unified
	.thumb
	.global UiWindow_DrawDividerLine
	.thumb_func
UiWindow_DrawDividerLine:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	adds r7, r1, #0
	adds r5, r0, #0
	mov r8, r2
	ldr r6, [sp, #28]
	mov r9, r3
	cmp r7, r10
	beq .L_08041c76
	b .L_08041dd4
.L_08041c76:
	cmp r8, r6
	bne .L_08041c7c
	b .L_08041f4a
.L_08041c7c:
	cmp r8, r6
	bls .L_08041c86
	mov r4, r8
	mov r8, r6
	adds r6, r4, #0
.L_08041c86:
	movs r1, #12
	ldrsh r0, [r5, r1]
	movs r2, #14
	ldrsh r1, [r5, r2]
	mov r2, r8
	add r1, r8
	subs r3, r6, r2
	add r0, r10
	movs r2, #1
	bl Func_08041abc
	movs r1, #14
	ldrsh r3, [r5, r1]
	movs r1, #12
	ldrsh r2, [r5, r1]
	add r3, r8
	lsls r3, r3, #6
	lsls r2, r2, #1
	add r3, r9
	mov r1, r10
	adds r3, r3, r2
	lsls r2, r1, #1
	adds r3, r3, r2
	adds r0, r3, #0
	mov r4, r8
	adds r0, #8
	cmp r4, r6
	bls .L_08041cc0
	b .L_08041f4a
.L_08041cc0:
	ldrh r1, [r0]
	cmp r4, r8
	bne .L_08041d28
	ldr r2, .L_08041f54
	adds r3, r1, r2
	cmp r3, #15
	bhi .L_08041dc0
	ldr r2, .L_08041f58
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08041cd8:
	.4byte .L_08041dc6
	.4byte .L_08041db8
	.4byte .L_08041dc6
	.4byte .L_08041dc6
	.4byte .L_08041dc6
	.4byte .L_08041d20
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041d18
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc6
.L_08041d18:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #24
	b .L_08041dc6
.L_08041d20:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #9
	b .L_08041dc6
.L_08041d28:
	cmp r4, r6
	bne .L_08041d90
	ldr r2, .L_08041f54
	adds r3, r1, r2
	cmp r3, #16
	bhi .L_08041dc0
	ldr r2, .L_08041f5c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08041d3c:
	.4byte .L_08041db8
	.4byte .L_08041dc6
	.4byte .L_08041dc6
	.4byte .L_08041dc6
	.4byte .L_08041dc6
	.4byte .L_08041d88
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041d80
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc0
	.4byte .L_08041dc6
.L_08041d80:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #25
	b .L_08041dc6
.L_08041d88:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #10
	b .L_08041dc6
.L_08041d90:
	ldr r2, .L_08041f54
	adds r3, r1, r2
	cmp r3, #5
	bhi .L_08041dc0
	ldr r2, .L_08041f60
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08041da0:
	.4byte .L_08041db8
	.4byte .L_08041db8
	.4byte .L_08041dc6
	.4byte .L_08041dc6
	.4byte .L_08041dc6
	.4byte .L_08041db8
.L_08041db8:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #13
	b .L_08041dc6
.L_08041dc0:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #15
.L_08041dc6:
	adds r4, #1
	strh r1, [r0]
	adds r0, #64
	cmp r4, r6
	bhi .L_08041dd2
	b .L_08041cc0
.L_08041dd2:
	b .L_08041f4a
.L_08041dd4:
	cmp r8, r6
	beq .L_08041dda
	b .L_08041f4a
.L_08041dda:
	cmp r7, r10
	bne .L_08041de0
	b .L_08041f4a
.L_08041de0:
	cmp r7, r10
	bls .L_08041dea
	adds r4, r7, #0
	mov r7, r10
	mov r10, r4
.L_08041dea:
	movs r3, #12
	ldrsh r0, [r5, r3]
	movs r2, #14
	ldrsh r1, [r5, r2]
	mov r3, r10
	add r1, r8
	subs r2, r3, r7
	adds r0, r0, r7
	movs r3, #1
	bl Func_08041abc
	movs r1, #14
	ldrsh r3, [r5, r1]
	movs r1, #12
	ldrsh r2, [r5, r1]
	add r3, r8
	lsls r3, r3, #6
	lsls r2, r2, #1
	add r3, r9
	adds r3, r3, r2
	lsls r2, r7, #1
	adds r3, r3, r2
	adds r0, r3, #0
	adds r4, r7, #0
	adds r0, #8
	cmp r4, r10
	bls .L_08041e22
	b .L_08041f4a
.L_08041e22:
	ldrh r1, [r0]
	cmp r4, r7
	bne .L_08041e94
	ldr r2, .L_08041f54
	adds r3, r1, r2
	cmp r3, #17
	bls .L_08041e32
	b .L_08041f38
.L_08041e32:
	ldr r2, .L_08041f64
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08041e3c:
	.4byte .L_08041f3e
	.4byte .L_08041f3e
	.4byte .L_08041f3e
	.4byte .L_08041f30
	.4byte .L_08041f3e
	.4byte .L_08041f38
	.4byte .L_08041e8c
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041e84
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f3e
.L_08041e84:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #26
	b .L_08041f3e
.L_08041e8c:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #11
	b .L_08041f3e
.L_08041e94:
	cmp r4, r10
	bne .L_08041f04
	ldr r2, .L_08041f54
	adds r3, r1, r2
	cmp r3, #18
	bhi .L_08041f38
	ldr r2, .L_08041f68
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08041ea8:
	.4byte .L_08041f3e
	.4byte .L_08041f3e
	.4byte .L_08041f30
	.4byte .L_08041f3e
	.4byte .L_08041f3e
	.4byte .L_08041f38
	.4byte .L_08041efc
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041ef4
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f38
	.4byte .L_08041f3e
.L_08041ef4:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #27
	b .L_08041f3e
.L_08041efc:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #12
	b .L_08041f3e
.L_08041f04:
	ldr r2, .L_08041f54
	adds r3, r1, r2
	cmp r3, #6
	bhi .L_08041f38
	ldr r2, .L_08041f6c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08041f14:
	.4byte .L_08041f3e
	.4byte .L_08041f3e
	.4byte .L_08041f30
	.4byte .L_08041f30
	.4byte .L_08041f3e
	.4byte .L_08041f38
	.4byte .L_08041f30
.L_08041f30:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #13
	b .L_08041f3e
.L_08041f38:
	movs r1, #240
	lsls r1, r1, #8
	adds r1, #14
.L_08041f3e:
	adds r4, #1
	strh r1, [r0]
	adds r0, #2
	cmp r4, r10
	bhi .L_08041f4a
	b .L_08041e22
.L_08041f4a:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08041f54:
	.4byte 0xffff0ff7
.L_08041f58:
	.4byte .L_08041cd8
.L_08041f5c:
	.4byte .L_08041d3c
.L_08041f60:
	.4byte .L_08041da0
.L_08041f64:
	.4byte .L_08041e3c
.L_08041f68:
	.4byte .L_08041ea8
.L_08041f6c:
	.4byte .L_08041f14
