@ 戦闘モーション: オブジェクトの尺度・速度欄を初期化し、
@ [r6+12] × 0x14ccc を Q16 乗算した値で開始位置を設定する。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global BattleMotion_InitializeObject
	.global Func_080b81c8
	.thumb_func
BattleMotion_InitializeObject:
Func_080b81c8:
	push	{r5, r6, lr}
	bl	Func_080b7dd0
	adds	r6, r0, #0
	ldr	r5, [r6, #0]
@ 尺度 0x10000、速度 0x40000、係数 0x9999、フラグ類を 0 に。
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #48]
	str	r3, [r5, #40]
	ldr	r3, [pc, #56]
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #90
	movs	r3, #0
	str	r3, [r5, #68]
	adds	r0, r5, #0
	strb	r3, [r2, #0]
	bl	Func_08009140
	ldr	r3, [pc, #40]
	ldr	r0, [r6, #12]
	ldr	r1, [pc, #40]
	movs	r0, r0
@ IwramMulQ16ReturnIp。
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	movs	r2, #0
	bl	Func_08009150
	adds	r0, r5, #0
	movs	r1, #5
	bl	Func_08009080
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00009999
	.4byte 0x03000118
	.4byte 0x00014ccc
