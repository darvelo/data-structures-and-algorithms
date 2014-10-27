function shellSort (array) {
  var gapCandidate = 1,
      gaps = [], gapsIdx, gap,
      i, j, tmp;

  // build a "near-optimal" list of gaps (no proven optimal list exists).
  // MUST start with `1` for final InsertionSort to take over.
  while (gapCandidate < array.length) {
    gaps.push(gapCandidate);
    gapCandidate = Math.floor(gapCandidate * 2.25);
  }

  // go through gaps in descending order to build progressively larger h-sorted subarrays
  for (gapsIdx = gaps.length - 1; gapsIdx !== -1; --gapsIdx) {
    gap = gaps[gapsIdx];

    for (i = gap; i < array.length; ++i) {
      tmp = array[i];

      // InsertionSort on interleaved subarrays
      for (j = i; j >= gap && tmp < array[j - gap]; j -= gap) {
        array[j] = array[j - gap];
      }

      array[j] = tmp;
    }
  }
}

module.exports = shellSort;
