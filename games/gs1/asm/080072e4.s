@ コンパイラ実行時処理の_call_via_rXに一致するサム用モード間サンク。
@ alchemy-gccは間接呼出しにbl _call_via_rNを用い、各サンクは
@ レジスタ分岐後、mov r8,r8のnopでワード境界まで埋める。
	.thumb

	.macro call_via register
	bx \register
	mov r8, r8
	.endm

	call_via r0
	call_via r1
	call_via r2
	call_via r3
	call_via r4
	call_via r5
	call_via r6
	call_via r7
	call_via r8
	call_via r9
	call_via sl
	call_via fp
	call_via ip
	call_via sp
