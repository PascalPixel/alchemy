#!/usr/bin/env bun
// Enforce the canonical Full-C Byte Share suffix against the metric report
// staged in Git's index. The working tree is deliberately not consulted.
import { DEFAULT_TARGET, parseDecompTarget, type DecompTargetId } from "./decomp_targets.ts";
import { currentProgress, formatSubject, parseSubject } from "./full_c_progress.ts";
import { canonicalJson } from "./canonical_json.ts";

interface MetricReport {
  format: 1;
  metric: "full-c-byte-share";
  target: DecompTargetId;
  full_c_bytes: number;
  executable_bytes: number;
  remaining_bytes: number;
  audit: "complete";
}

const LEGACY_REGION = /\[[0-9]{1,3}(?:,[0-9]{3})* of [0-9]{1,3}(?:,[0-9]{3})*\]$/;
const LEGACY_C_BYTES_AND_REGION =
  /\[[0-9]{1,3}(?:,[0-9]{3})* C bytes\]\s+\[[0-9]{1,3}(?:,[0-9]{3})* of [0-9]{1,3}(?:,[0-9]{3})*\]$/;
const LEGACY_ROM_BYTES = /\[[0-9,]+ of 8,388,608 bytes\]$/;
const DENOMINATOR_CORRECTION = /^metrics: correct executable denominator\b/;

function legacySubject(subject: string): boolean {
  return LEGACY_C_BYTES_AND_REGION.test(subject) ||
    LEGACY_REGION.test(subject) ||
    LEGACY_ROM_BYTES.test(subject);
}

function validatedReport(value: unknown, target: DecompTargetId): MetricReport {
  const report = value as Partial<MetricReport>;
  if (report.format !== 1 || report.metric !== "full-c-byte-share" ||
      report.target !== target || report.audit !== "complete") {
    throw new Error(`staged ${target} Full-C report is missing, incomplete, or has the wrong format`);
  }
  for (const [label, count] of [
    ["full_c_bytes", report.full_c_bytes],
    ["executable_bytes", report.executable_bytes],
    ["remaining_bytes", report.remaining_bytes],
  ] as const) {
    if (!Number.isSafeInteger(count) || count! < 0) throw new Error(`staged report ${label} is invalid`);
  }
  if (report.full_c_bytes! > report.executable_bytes! ||
      report.full_c_bytes! + report.remaining_bytes! !== report.executable_bytes) {
    throw new Error("staged report arithmetic is invalid");
  }
  return report as MetricReport;
}

export function checkCommitProgress(
  message: string,
  reportValue: unknown,
  previousSubject?: string,
  expectedDenominator?: number,
): void {
  const [subject = ""] = message.split(/\r?\n/, 1);
  const report = validatedReport(reportValue, (reportValue as MetricReport).target);
  const parsed = parseSubject(subject);
  if (!parsed) {
    throw new Error("commit subject must end with " +
      formatSubject(report.full_c_bytes, report.executable_bytes));
  }
  if (parsed.fullCBytes !== report.full_c_bytes ||
      parsed.executableBytes !== report.executable_bytes) {
    throw new Error(
      `commit suffix is stale; expected ${formatSubject(report.full_c_bytes, report.executable_bytes)}`,
    );
  }
  if (expectedDenominator !== undefined && parsed.executableBytes !== expectedDenominator) {
    throw new Error(`executable denominator changed from ${expectedDenominator} to ${parsed.executableBytes}`);
  }
  if (previousSubject === undefined) return;
  const previous = parseSubject(previousSubject);
  if (previous) {
    if (parsed.executableBytes !== previous.executableBytes) {
      if (!DENOMINATOR_CORRECTION.test(subject)) {
        throw new Error(
          `executable denominator changed from ${previous.executableBytes} to ${parsed.executableBytes}; ` +
          "use an explicit metrics: correct executable denominator commit",
        );
      }
    }
    if (parsed.fullCBytes < previous.fullCBytes) {
      throw new Error(
        `Full-C bytes regressed from ${previous.fullCBytes} to ${parsed.fullCBytes}`,
      );
    }
    return;
  }
  if (!legacySubject(previousSubject)) {
    throw new Error("previous commit has neither canonical Full-C nor recognized transition syntax");
  }
}

function git(args: string[]): string {
  const process = Bun.spawnSync(["git", ...args], { stdout: "pipe", stderr: "pipe" });
  if (process.exitCode !== 0) {
    throw new Error(process.stderr.toString().trim() || `git ${args.join(" ")} failed`);
  }
  return process.stdout.toString();
}

function stagedReport(target: DecompTargetId): MetricReport {
  const path = `metrics/${target}-progress.json`;
  let text: string;
  try {
    text = git(["show", `:${path}`]);
  } catch {
    throw new Error(`stage ${path} before committing`);
  }
  return validatedReport(JSON.parse(text), target);
}

/**
 * True when the committed content of the derived progress report already
 * matches what regenerating it right now would produce.
 *
 * `git show :path` reads the INDEX -- the exact bytes this commit will carry --
 * so a report that is already correct passes whether or not it was touched,
 * and a report that would change fails unless the new content is staged.
 */
function reportIsCurrent(
  target: DecompTargetId,
  readCommitted: (path: string) => string | null = (path) => {
    try {
      return git(["show", `:${path}`]);
    } catch {
      return null; // not tracked, or not in the index at all
    }
  },
  regenerate: (target: DecompTargetId) => string =
    (id) => canonicalJson(currentProgress(id)),
): boolean {
  const committed = readCommitted(`metrics/${target}-progress.json`);
  if (committed === null) return false;
  return committed.trim() === regenerate(target).trim();
}

function stagedPaths(): string[] {
  const unmerged = git(["ls-files", "-u"]).trim();
  if (unmerged) throw new Error("cannot validate Full-C progress with unmerged index entries");
  return git(["diff", "--cached", "--name-only", "--diff-filter=ACMRT"])
    .split(/\r?\n/).filter(Boolean);
}

function reportRequired(paths: readonly string[], target: DecompTargetId): boolean {
  const registryRoots = target === "gs1-en"
    ? /^(?:src\/|asm\/|include\/|assets\/code\/resource_.+_(?:overlay\.s|c_[0-9a-f]{8}\.c)$|metrics\/gs1-en-executable\.json$)/
    : /^(?:games\/gs2\/(?:src|asm|include)\/|metrics\/gs2-en-executable\.json$)/;
  return paths.some((path) => registryRoots.test(path));
}

function selfTest(): void {
  const report: MetricReport = {
    format: 1,
    metric: "full-c-byte-share",
    target: "gs1-en",
    full_c_bytes: 123456,
    executable_bytes: 1234567,
    remaining_bytes: 1111111,
    audit: "complete",
  };
  checkCommitProgress(
    "metrics: transition [C 123,456/1,234,567 bytes]",
    report,
    "src: prior [95,358 C bytes] [1,368 of 2,000]",
  );
  checkCommitProgress(
    "docs: repeat [C 123,456/1,234,567 bytes]",
    report,
    "metrics: prior [C 123,456/1,234,567 bytes]",
  );
  const invalid: Array<[string, MetricReport, string?]> = [
    ["missing suffix", report],
    ["bad commas [C 123456/1,234,567 bytes]", report],
    ["missing unit [C 123,456/1,234,567]", report],
    ["spaces [C 123,456 / 1,234,567 bytes]", report],
    ["stale [C 123,455/1,234,567 bytes]", report],
    ["wrong total [C 123,456/1,234,568 bytes]", report],
    ["regress [C 123,456/1,234,567 bytes]", report,
      "prior [C 123,457/1,234,567 bytes]"],
    ["post-transition legacy [123 of 456]", report,
      "prior [C 123,456/1,234,567 bytes]"],
  ];
  for (const [message, metric, previous] of invalid) {
    let rejected = false;
    try {
      checkCommitProgress(message, metric, previous);
    } catch {
      rejected = true;
    }
    if (!rejected) throw new Error(`invalid commit progress was accepted: ${message}`);
  }
  const stale = { ...report, full_c_bytes: 1, remaining_bytes: 1234566 };
  let rejected = false;
  try {
    checkCommitProgress("stale report [C 123,456/1,234,567 bytes]", stale);
  } catch {
    rejected = true;
  }
  if (!rejected) throw new Error("staged/report mismatch was accepted");
  checkCommitProgress(
    "metrics: correct executable denominator after boundary audit [C 123,456/1,234,568 bytes]",
    { ...report, executable_bytes: 1234568, remaining_bytes: 1111112 },
    "prior [C 123,456/1,234,567 bytes]",
  );
  // --- the report-currency rule, both branches -------------------------------
  // Intent: the derived report must not be STALE. Not: it must appear in every
  // commit. A change that touches executable source without altering compiled
  // output regenerates to an identical file, so there is nothing to stage.
  const freshReport = '{"full_c_bytes":1}';
  const staleReport = '{"full_c_bytes":0}';
  if (!reportIsCurrent("gs1-en", () => freshReport, () => freshReport)) {
    throw new Error("self-test: an already-current report was rejected");
  }
  // Trailing-whitespace differences are not staleness.
  if (!reportIsCurrent("gs1-en", () => freshReport + "\n", () => freshReport)) {
    throw new Error("self-test: whitespace-only difference treated as stale");
  }
  if (reportIsCurrent("gs1-en", () => staleReport, () => freshReport)) {
    throw new Error("self-test: a stale report was accepted");
  }
  // A report absent from the index is stale, not current.
  if (reportIsCurrent("gs1-en", () => null, () => freshReport)) {
    throw new Error("self-test: a missing report was accepted");
  }

  // --- an unsuffixed parent subject is REPORTED, never a crash ---------------
  // parseSubject returns undefined for a subject carrying no counter. main()'s
  // denominator guard once compared that against null, so `undefined !== null`
  // was true and the property read threw a TypeError before the designed
  // diagnostic could fire -- which blocked every commit after such a parent.
  if (parseSubject("docs") !== undefined) {
    throw new Error("self-test: parseSubject no longer returns undefined for an unsuffixed subject");
  }
  let unsuffixedRejected = false;
  try {
    checkCommitProgress("subject [C 123,456/1,234,567 bytes]", report, "docs");
  } catch (error) {
    unsuffixedRejected = true;
    if (error instanceof TypeError) {
      throw new Error("self-test: unsuffixed parent subject crashed instead of being reported");
    }
  }
  if (!unsuffixedRejected) throw new Error("self-test: unsuffixed parent subject was accepted");
  console.log("self-test=ok suffix=full-c-byte-share");
}

async function main(argv: string[]): Promise<void> {
  if (argv[0] === "--self-test") return selfTest();
  let target: DecompTargetId = DEFAULT_TARGET;
  const targetIndex = argv.indexOf("--target");
  if (targetIndex >= 0) {
    target = parseDecompTarget(argv[targetIndex + 1]);
    argv.splice(targetIndex, 2);
  }
  const path = argv[0];
  if (!path) throw new Error("usage: check_commit_progress.ts [--target TARGET] COMMIT_MESSAGE");
  const paths = stagedPaths();
  const reportPath = `metrics/${target}-progress.json`;
  // The rule is that the derived report must not be STALE -- not that it must
  // appear in every commit. Those differ for a change that touches executable
  // source without altering compiled output (a comment added to an exact `src/`
  // file, say): regenerating produces an identical file, so there is nothing
  // for git to stage and the old "must be staged" form was unsatisfiable.
  //
  // So: regenerate, and require no diff against what this commit will contain.
  // If regeneration changes the report it must be staged; if it changes
  // nothing, the requirement is already met.
  if (reportRequired(paths, target) && !reportIsCurrent(target)) {
    throw new Error(
      `${reportPath} is stale: regenerate it (bun tools/full_c_progress.ts --write-report) ` +
      "and stage it with executable-source changes",
    );
  }
  const report = stagedReport(target);
  const previous = git(["log", "-1", "--format=%s"]).trim() || undefined;
  const previousMetric = previous === undefined ? null : parseSubject(previous);
  // parseSubject returns undefined for a subject that carries no counter (a
  // commit that bypassed this hook). Guard both empties: one such commit on the
  // branch would otherwise crash the hook for every commit that follows it.
  if (previousMetric != null && report.executable_bytes !== previousMetric.executableBytes &&
      !paths.includes(`metrics/${target}-executable.json`)) {
    throw new Error(`denominator correction requires staged metrics/${target}-executable.json`);
  }
  checkCommitProgress(await Bun.file(path).text(), report, previous);
}

if (import.meta.main) await main(Bun.argv.slice(2));
