# Generated metrics

This directory contains tracked, source-derived measurements for each build
target. The files contain addresses, interval classifications, counts, and
evidence references—never ROM bytes or disassembly output.

For `gs1-en`:

- `gs1-en-executable.json` is the audited executable denominator;
- `gs1-en-progress.json` is exact C divided by that denominator;
- `gs1-en-coverage-map.json` supplies the dashboard and README charts; and
- `full-c-history.{json,csv}` records the first-parent exact-C history.

Use the repository commands rather than editing generated files:

```sh
make progress        # calculate the live exact-C metric
make progress-check  # compare tracked reports with the source tree
make coverage        # rewrite the map, charts, and README cache keys
make coverage-check  # check those artifacts without writing
```

An executable inventory marked `audit: "incomplete"` fails closed: it cannot
publish a progress percentage. A denominator correction is a separate audited
change, not ordinary decompilation progress. See
[`../docs/FULL-C-BYTE-SHARE.md`](../docs/FULL-C-BYTE-SHARE.md) for the metric
contract.
