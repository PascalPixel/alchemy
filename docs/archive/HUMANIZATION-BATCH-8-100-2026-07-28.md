> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# 第8回 100ファイル humanization — 2026-07-28

第7回で選んだ100候補を確認し、cross-TU ABI、共有state、
volatile I/O、raw-layout accessを中心に整理した。
広範囲のhumanizationは本回で一区切りとし、以後は新規C化を
直接支える型・ABI・命名だけを扱う。

## 結果

- 100候補を確認。
- 39候補を実質的な整理として採用。
- 61候補を不一致、未解決契約、低価値変更として復元または保留。
- 採用した共有interfaceに合わせ、隣接する19本も更新。
- 最終差分はC 58本、ヘッダ5本、本書1本。
- claimed C: 1,345本、失敗0、90,664 bytes。
- source-only: 8,388,608 bytes、未所有0。
- ordinary assembly debt: 654 regions、400,836 bytes。
- full ROM: 完全一致、fallback 0。
- 全self-test成功。

主な成果は、`Func_080773d8`、`Func_08005cf8`、
`Func_08018850`、`Func_080b5098` familyのcross-TU宣言統一、
guarded node chain、global progress view、layout guard、
DMA/SIOのvolatile契約である。

## 主な保留

- `src/0800b684.c`
- `src/08078b60.c`
- `src/08093a14.c`
- `src/08099070.c`
- `src/080990cc.c`
- `src/080a8b10.c`
- `src/080aa544.c`
- `src/080a735c.c`
- `src/080b2da8.c`
- `src/080b8f58.c`
- `src/080b19cc.c`
- `src/080b6e30.c`
- `src/080b7aac.c`
- `src/080f7db4.c`
- `src/0801ff14.c`
- `src/08020088.c`
- `src/080b78e4.c`
- `src/080b6378.c`
- `src/080042c8.c`
- `src/08078320.c`
- `src/08079bf8.c`
- `src/0808c2dc.c`

`080c2384`–`080c2454`のtable familyは共有型を試したが、
consumerごとに必要な宣言が一致せず、2本のexact buildを壊した。
このfamilyの変更は全て復元した。

## 次の作業

次回からは100ファイルhumanizationを行わない。
81–320 instructionのordinary assembly regionと、
既に一致実績のあるcompiler/structure familyを優先して新規C化する。
humanizationは個別conversionを成立させる場合に限る。
