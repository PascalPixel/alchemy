# 第7回 100ファイル humanization — 2026-07-28

大型のraw-layout関数を中心に100候補を調べた。初回79候補から、
rename-onlyと未解決ABIを統合レビューで除外した。

## 結果

- 100候補を確認。
- 32候補を実質的な整理として採用。
- 68候補を不一致、未解決契約、低価値変更として復元。
- 隣接する5本のcross-TU宣言も統一。
- 最終差分はC 37本、ヘッダ7本、本書1本。
- claimed C: 1,345本、失敗0、90,664 bytes。
- source-only: 8,388,608 bytes、未所有0。
- full ROM: 完全一致、fallback 0。
- 全self-test成功。

低位runtime、object factory、effect、menu result、共有state、
volatile phase、raw DMA wordの契約を共有ヘッダへまとめた。
型が機械ABIだけ一致していたcross-TU宣言を同じC型へ揃えた。

## 主な保留

- `src/0800bfa4.c`
- `src/0801b1ec.c`
- `src/080be02c.c`
- `src/080947e4.c`
- `src/080aaf58.c`
- `src/080b9a70.c`
- `src/080c9138.c`
- `src/080dfddc.c`
- `src/080f7e34.c`

残る59候補はrename-only、または安全な実質変更を作れなかったため復元した。

## 次の100候補

### C1 — early engine object/control

`08020a60`, `08021c34`, `08016758`, `080042c8`, `08005cf8`, `0800b684`,
`08020b14`, `0800bdd4`, `080040e8`, `08021e48`

### C2 — entity/table traversal

`080782a0`, `08078320`, `080797fc`, `08079e9c`, `08078aa0`, `080773f4`,
`08078618`, `080798b4`, `08079bf8`, `08078b60`

### C3 — effects/state

`08093a14`, `0809c3a4`, `08099070`, `080990cc`, `0808c2dc`, `08092a74`,
`0808b824`, `0808d5a4`, `08092ba8`, `080935b0`

### C4 — menu/object state

`080a9d3c`, `080a2268`, `080a7440`, `080a8b10`, `080a3ce4`, `080a9cbc`,
`080a9d84`, `080a3c08`, `080a3d24`, `080a195c`

### C5 — late menu/event interfaces

`080a3480`, `080a65e4`, `080a8508`, `080a1090`, `080aa544`, `080a735c`,
`080a3c98`, `080a5534`, `080ad69c`, `080e6948`

### C6 — battle runtime

`080b2da8`, `080b86ec`, `080b9acc`, `080b8f58`, `080b78e4`, `080b196c`,
`080b6cdc`, `080b606c`, `080b8f08`, `080b19cc`

### C7 — late battle/resource interfaces

`080b6e30`, `080be070`, `080b7514`, `080b7aac`, `080bbabc`, `080b0694`,
`080b6378`, `080c01bc`, `080c2410`, `080c0184`

### C8 — late runtime/system state

`080c90e4`, `080c2470`, `080ccbdc`, `080c23c0`, `080c0df4`, `080dbb24`,
`080d6504`, `080d6660`, `080df8b8`, `080f7db4`

### C9 — raw transfer/control

`080f6008`, `080f2ebc`, `080f40b4`, `080f03c0`, `08003ed4`, `08003f3c`,
`0800fa8c`, `0801fb48`, `0801ff14`, `080eceac`

### C10 — remaining substantive control

`08020088`, `0801ee68`, `08028ea8`, `080216b4`, `0801b36c`, `08078550`,
`08079728`, `08079754`, `0807987c`, `0807a628`
