# 第5回 100ファイル humanization — 2026-07-28

100候補を並行処理し、統合レビューでコメントだけの変更を進捗から
除外した。

## 結果

- 100候補を確認。
- 57候補を実質的な整理として採用。
- 12候補をABI・安全性・生成コードの壁として保留。
- コメントだけ、または意味のない変更31候補を完全に復元。
- 隣接する10本の型・ABIも整理。
- 最終差分はC 67本、ヘッダ3本、本書1本。
- claimed C: 1,345本、失敗0、90,664 bytes。
- source-only: 8,388,608 bytes、未所有0。
- full ROM: 完全一致、fallback 0。
- 全self-test成功。

DMA/SIOの`volatile`、effect step、render output、far runtime、
符号付きshift/overflow、隠しレジスタ引数を整理した。
日本語コメントだけを追加したファイルはhumanization数へ含めない。

## 保留

- `src/08004970.c`
- `src/08006c68.c`
- `src/08006dec.c`
- `src/08006f84.c`
- `src/08007098.c`
- `src/0801b9a8.c`
- `src/0801b9ec.c`
- `src/0801ba34.c`
- `src/080215e0.c`
- `src/080218dc.c`
- `src/080e3944.c`
- `src/080e3a3c.c`

## 次の100候補

### M1 — motion/script object core

`0800ca2c`, `0800ca44`, `0800ca58`, `0800ca98`, `0800d130`, `0800da40`,
`0800da78`, `0800daa0`, `0800dcdc`, `0800ea18`

### M2 — map work and transition pipeline

`08016594`, `080167ac`, `08016868`, `08017364`, `080174d8`, `08017620`,
`08017a64`, `08018790`, `080187ac`, `080187fc`

### M3 — battle UI/effect work

`0801ce48`, `0801ce6c`, `0801ce90`, `0801cee0`, `0801e3c8`, `0801ed40`,
`0801edcc`, `0801ef08`, `0801f704`, `0801f730`

### M4 — inventory/container continuation

`0807822c`, `08078ad0`, `080792c4`, `08079ae8`, `08079c30`, `08079c5c`,
`08079c8c`, `08079d1c`, `08079d7c`, `0807a0cc`

### M5 — object motion/action callbacks I

`08096c80`, `08096d2c`, `08096d84`, `08097174`, `0809728c`, `0809748c`,
`08097608`, `08097948`, `08097a54`, `08097a7c`

### M6 — object motion/action callbacks II

`0809802c`, `08098070`, `08098184`, `08098294`, `080985a8`, `08098a84`,
`08098c08`, `08099040`, `08099920`, `080999a8`

### M7 — menu/action descriptor setup I

`080a1bdc`, `080a1c2c`, `080a1c6c`, `080a1cb0`, `080a23c0`, `080a23f4`,
`080a33d4`, `080a34c0`, `080a355c`, `080a38a8`

### M8 — menu/action descriptor setup II

`080a3cf8`, `080a3d6c`, `080a3d9c`, `080a3e28`, `080a413c`, `080a4754`,
`080a4db4`, `080a4e20`, `080a4e44`, `080a5fe0`

### M9 — late battle/menu runtime

`080b6ae0`, `080b6b40`, `080b770c`, `080b7b30`, `080b8ec4`, `080b90f8`,
`080b9d34`, `080ba27c`, `080bb8e8`, `080bb928`

### M10 — audio sequencer channel state

`080fa514`, `080fa83c`, `080fa8d4`, `080facf8`, `080fb2cc`, `080fb334`,
`080fb430`, `080fb75c`, `080fb768`, `080fb77c`
