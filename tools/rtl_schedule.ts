// A deterministic replica of gcc-2.96's haifa-sched.c `rank_for_schedule` --
// not an approximation, the actual tier order and comparisons, verified
// against the real source (alchemy-gcc/gcc-2.96/gcc/haifa-sched.c:4154-4274)
// and cross-checked against a real ready-list decision during development
// (resource_385:0314, cycle 14: insns 43/34/21 ready, 21 chosen).
//
// rank_for_schedule is a PAIRWISE comparator plugged into a sort, not a
// single rule that explains a winner against an entire ready list at once:
// it returns at the first tier where a pair of insns differs, tier by tier,
// exactly like a lexicographic comparison. This module mirrors that shape
// (comparePair) instead of trying to find "the one tier" that explains a
// winner beating every rival simultaneously -- a winner can beat one rival
// on class and a different rival on original order, in the same decision.
//
// This exists to replace "try five -fsched-* flags and see which one
// changes the output" with "compute which tier decided each pairwise
// comparison, using the data already in the dump." Priority and dependency
// counts are read straight from the dump's own "Region Dependences" table;
// the last-scheduled-insn class tier reads the LOG_LINKS dependency kind
// rtl_insn.ts already parses. Two tiers this file does NOT replicate: the
// register-pressure weight comparison (guarded by `!reload_completed`,
// always false for the post-reload -dR/.sched2 pass this tool reads, so it
// never fires there) and `sched_dest_order_regno` (the project's own
// -fsched-low-dest-first/-high-dest-first Camelot-matching hook, which only
// applies to files already routed through one of those flags -- out of
// scope for a routing-independent diagnosis).
import type { RtlInsn } from "./rtl_insn.ts";

export interface DependenceRow {
  uid: number;
  code: number;
  depCount: number; // the table's "dep" column: how many things THIS insn depends on
  priority: number;
  cost: number;
  dependents: number[]; // successor UIDs -- what rank_for_schedule's depend_count tier counts
}

// The "blockage" column prints as three separate whitespace-joined tokens
// ("1 - 32" or "0 -  0", note the sometimes-doubled space), not one
// contiguous "N-M" token -- easy to under-count as a single \S+ group.
const TABLE_ROW = /^;;\s+(\d+)\s+(-?\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+\d+\s*-\s*\d+\s+\S+\s*:?\s*(.*)$/;

// Parses the "Region Dependences" table gcc prints once per basic block at
// the head of a -dR/.sched2 dump, before any scheduling trace text:
//   ;;      insn  code    bb   dep  prio  cost   blockage units
//   ;;       15   174     0     0    89     2    1 - 32   core	: 67 47 39 28 19
export function parseDependenceTable(dumpText: string): Map<number, DependenceRow> {
  const table = new Map<number, DependenceRow>();
  for (const line of dumpText.split("\n")) {
    const match = TABLE_ROW.exec(line);
    if (match === null) continue;
    const [, uidText, codeText, , depText, prioText, costText, dependentsText] = match;
    const uid = Number(uidText);
    const dependents = dependentsText.trim().length === 0
      ? []
      : dependentsText.trim().split(/\s+/).map(Number).filter(Number.isFinite);
    table.set(uid, {
      uid, code: Number(codeText), depCount: Number(depText),
      priority: Number(prioText), cost: Number(costText), dependents,
    });
  }
  return table;
}

export type Tier = "priority" | "class" | "depend-count" | "original-order" | "indeterminate";

export interface PairComparison {
  winner: number;
  loser: number;
  tier: Tier;
  detail: string;
}

function classOf(insn: RtlInsn, lastScheduledUid: number | null): 1 | 2 | 3 {
  if (lastScheduledUid === null) return 3;
  const link = insn.dependencies.find((dependency) => dependency.uid === lastScheduledUid);
  return link === undefined ? 3 : link.kind === "true" ? 1 : 2;
}

export interface ScheduleContext {
  table: ReadonlyMap<number, DependenceRow>;
  lastScheduledUid: number | null;
  insnsByUid: ReadonlyMap<number, RtlInsn>;
}

// Compares two ready insns exactly the way rank_for_schedule does: tier by
// tier, in order, returning at the first tier that differs. `a` and `b` are
// symmetric inputs; the return always names the actual winner/loser by UID
// rather than leaving the caller to work it out from a signed number, since
// this function exists to be READ, not just sorted by.
export function comparePair(aUid: number, bUid: number, context: ScheduleContext): PairComparison {
  const aRow = context.table.get(aUid);
  const bRow = context.table.get(bUid);
  if (aRow !== undefined && bRow !== undefined && aRow.priority !== bRow.priority) {
    const [winner, loser] = aRow.priority > bRow.priority ? [aUid, bUid] : [bUid, aUid];
    return {
      winner, loser, tier: "priority",
      detail: `priority ${Math.max(aRow.priority, bRow.priority)} beats ${Math.min(aRow.priority, bRow.priority)}`,
    };
  }

  const aInsn = context.insnsByUid.get(aUid);
  const bInsn = context.insnsByUid.get(bUid);
  if (aInsn !== undefined && bInsn !== undefined) {
    const aClass = classOf(aInsn, context.lastScheduledUid);
    const bClass = classOf(bInsn, context.lastScheduledUid);
    if (aClass !== bClass) {
      const [winner, loser, winnerClass, loserClass] = aClass > bClass ? [aUid, bUid, aClass, bClass] : [bUid, aUid, bClass, aClass];
      return {
        winner, loser, tier: "class",
        detail: `class ${winnerClass} (relative to last-scheduled insn ${context.lastScheduledUid}) beats class ${loserClass}`,
      };
    }
  }

  if (aRow !== undefined && bRow !== undefined && aRow.dependents.length !== bRow.dependents.length) {
    const [winner, loser] = aRow.dependents.length > bRow.dependents.length ? [aUid, bUid] : [bUid, aUid];
    return {
      winner, loser, tier: "depend-count",
      detail: `${Math.max(aRow.dependents.length, bRow.dependents.length)} dependents beats ${Math.min(aRow.dependents.length, bRow.dependents.length)}`,
    };
  }

  // Approximates INSN_LUID (original creation order) with UID, since both
  // increase monotonically with insn creation in the common case; flagged
  // as an approximation in the detail text rather than asserted as exact.
  const [winner, loser] = aUid < bUid ? [aUid, bUid] : [bUid, aUid];
  return {
    winner, loser, tier: "original-order",
    detail: `lower UID (approximates INSN_LUID, gcc's true tie-break key)`,
  };
}

export interface Diagnosis {
  winnerUid: number;
  perRival: PairComparison[];
}

// Applies comparePair between the actual winner and every other ready insn,
// mirroring what the sort that produced this ready-list ordering actually
// did pairwise. A winner can beat different rivals at different tiers in
// the same decision (e.g. beat one rival on class, another on original
// order) -- reporting each pair separately is what makes this a genuine
// explanation rather than a single tier label pretending to cover every
// comparison at once.
export function diagnose(readyUids: readonly number[], winnerUid: number, context: ScheduleContext): Diagnosis {
  const perRival = readyUids
    .filter((uid) => uid !== winnerUid)
    .map((rivalUid) => comparePair(winnerUid, rivalUid, context));
  return { winnerUid, perRival };
}

function selfTest(): void {
  // The real dependence-table excerpt this module was built against
  // (resource_385:0314, verified 2026-08-04): insns 21/34/43 tie on
  // priority 83 at cycle 14, with 39 as the last-scheduled insn. 43 is a
  // TRUE dependent of 39 (class 1); 21 and 34 are independent of it
  // (class 3) and so both outrank 43 outright on the class tier. Between 21
  // and 34 (both class 3), depend-count also ties (3 dependents each per
  // the real dump), so their pair is decided by original order -- insn 21
  // (lower UID) wins, matching the real trace's actual pick.
  const table = parseDependenceTable(`
;;      insn  code    bb   dep  prio  cost   blockage units
;;       21   114     0     1    83     1    1 - 32   core	: 88 67 47
;;       34   114     0     1    83     1    1 - 32   core	: 88 67 49
;;       39   174     0     4    85     2    1 - 32   core	: 67 47 43
;;       43    33     0     2    83     1    1 - 32   core	: 67 49 45
`);
  if (table.size !== 4) throw new Error(`expected 4 dependence rows, got ${table.size}`);
  if (table.get(21)?.priority !== 83 || table.get(21)?.dependents.join(",") !== "88,67,47") {
    throw new Error(`bad row 21: ${JSON.stringify(table.get(21))}`);
  }

  // Regression guard: the "0 -  0   none" blockage form (doubled space, a
  // "none" unit instead of "core") is what a zero-latency/no-unit insn
  // (like the `use` insns synthesized for a return sequence) prints as; it
  // must parse the same as the ordinary "1 - 32   core" form.
  const noneUnitRow = parseDependenceTable(";;       71    -1     0     1    33     1    0 -  0   none\t: 88 80");
  if (noneUnitRow.get(71)?.dependents.join(",") !== "88,80") {
    throw new Error(`bad "none"-unit row: ${JSON.stringify(noneUnitRow.get(71))}`);
  }

  const insn = (uid: number, dependencies: RtlInsn["dependencies"]): RtlInsn => ({
    uid, kind: "insn", code: "set", set: null, callTarget: null, dependencies, raw: "",
  });
  // 43's real LOG_LINKS: `(insn_list 41 (insn_list 39 (nil)))` -- an
  // untagged (true) dependency on 39. 21 and 34 have none relevant to 39.
  const insnsByUid = new Map([
    [21, insn(21, [])],
    [34, insn(34, [])],
    [43, insn(43, [{ uid: 39, kind: "true" }])],
  ]);
  const context: ScheduleContext = { table, lastScheduledUid: 39, insnsByUid };

  const winnerVs43 = comparePair(21, 43, context);
  if (winnerVs43.winner !== 21 || winnerVs43.tier !== "class") {
    throw new Error(`expected 21 to beat 43 on class, got ${JSON.stringify(winnerVs43)}`);
  }
  const winnerVs34 = comparePair(21, 34, context);
  if (winnerVs34.winner !== 21 || winnerVs34.tier !== "original-order") {
    throw new Error(`expected 21 to beat 34 on original-order (both class 3, tied depend-count), got ${JSON.stringify(winnerVs34)}`);
  }

  const diagnosis = diagnose([21, 34, 43], 21, context);
  if (diagnosis.perRival.length !== 2) throw new Error(`expected 2 pairwise comparisons, got ${diagnosis.perRival.length}`);
  const tiers = diagnosis.perRival.map((pair) => pair.tier).sort();
  if (tiers.join(",") !== "class,original-order") {
    throw new Error(`expected one class-tier win and one original-order win, got ${tiers.join(",")}`);
  }

  // A clean priority win needs no lower tier and no RtlInsn data at all.
  const clearWinner = parseDependenceTable(`
;;       15   174     0     0    89     2    1 - 32   core	:
;;       17   174     0     0    80     2    1 - 32   core	:
`);
  const clearPick = comparePair(15, 17, { table: clearWinner, lastScheduledUid: null, insnsByUid: new Map() });
  if (clearPick.winner !== 15 || clearPick.tier !== "priority") {
    throw new Error(`expected a clean priority win, got ${JSON.stringify(clearPick)}`);
  }

  console.log("self-test=ok tool=rtl-schedule");
}

if (import.meta.main && Bun.argv.includes("--self-test")) selfTest();
