#!/usr/bin/env bun
// コミット件名の進捗表記を検査する。表記は「C化済み実行領域の累計」で、
// 形式は [X of Y]。XはC一致済みの実行関数領域数、Yは X と残る
// C化対象アセンブリ領域数の合計。バイト被覆表記は役目を終えて廃止した。
// 旧形式 [... of 8,388,608 bytes] の直前件名からの移行も受け付ける。

const SUFFIX = /\[([0-9]{1,3}(?:,[0-9]{3})*) of ([0-9]{1,3}(?:,[0-9]{3})*)\]$/;
const LEGACY = / of 8,388,608 bytes\]$/;

function commas(value: number): string {
  return value.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

function count(value: string, label: string): number {
  const parsed = Number(value.replaceAll(",", ""));
  if (!Number.isSafeInteger(parsed) || parsed < 0 || commas(parsed) !== value) {
    throw new Error(`${label} is not a canonical count`);
  }
  return parsed;
}

function subjectCounts(subject: string): [number, number] | undefined {
  if (LEGACY.test(subject)) return undefined;
  const match = subject.match(SUFFIX);
  if (match === null) return undefined;
  return [count(match[1], "converted count"), count(match[2], "target total")];
}

export function checkCommitProgress(message: string, previousSubject?: string): void {
  const [subject = ""] = message.split(/\r?\n/, 1);
  const counts = subjectCounts(subject);
  if (counts === undefined) {
    throw new Error("commit subject lacks the [converted of total] C-conversion suffix");
  }
  const [converted, total] = counts;
  if (converted > total) throw new Error("converted count exceeds the conversion target total");
  if (previousSubject === undefined) return;
  const previous = subjectCounts(previousSubject);
  if (previous !== undefined && converted < previous[0]) {
    throw new Error(`converted count ${commas(converted)} regressed from ${commas(previous[0])}`);
  }
}

function selfTest(): void {
  checkCommitProgress("decomp: test [812 of 2,203]", "decomp: prior [797 of 2,203]");
  checkCommitProgress("decomp: first [797 of 2,203]", "closure: legacy [8,388,608 of 8,388,608 bytes]");
  checkCommitProgress("decomp: total shift [812 of 2,199]", "decomp: prior [797 of 2,203]");
  for (const [message, previous] of [
    ["decomp: missing suffix", "decomp: prior [797 of 2,203]"],
    ["decomp: malformed [812 of 2203]", "decomp: prior [797 of 2,203]"],
    ["decomp: overshoot [2,204 of 2,203]", "decomp: prior [797 of 2,203]"],
    ["decomp: regression [700 of 2,203]", "decomp: prior [797 of 2,203]"],
  ] as const) {
    let rejected = false;
    try {
      checkCommitProgress(message, previous);
    } catch {
      rejected = true;
    }
    if (!rejected) throw new Error("invalid commit progress was accepted");
  }
}

async function main(args: string[]): Promise<void> {
  if (args[0] === "--self-test") {
    selfTest();
    console.log("self-test=ok");
    return;
  }
  const path = args[0];
  if (path === undefined) throw new Error("usage: check_commit_progress.ts COMMIT_MESSAGE");
  const previous = Bun.spawnSync(["git", "log", "-1", "--format=%s"], { stdout: "pipe", stderr: "pipe" });
  if (previous.exitCode !== 0) throw new Error(previous.stderr.toString().trim() || "cannot read previous commit");
  checkCommitProgress(await Bun.file(path).text(), previous.stdout.toString().trim() || undefined);
}

if (import.meta.main) await main(Bun.argv.slice(2));
