// This is an example of entropy-optimal sorting, where arrays can
// can be efficiently sorted whether or not they contain large numbers
// of duplicate keys. The trick is to partition the subarrays into three,
// in the order of:
//
//   * those values less than the pivot
//   * those values equal to the pivot
//   * those values greater than the pivot
//
// This method is great if you know your array has a large number of
// duplicate keys, but is less performant than standard quicksort if not,
// since there are many more swaps for non-duplicates here by comparison.
//
// Therefore, `quickSortNotManyDuplicates()` will first partition into 4,
// where two cursors at opposite ends of the subarray swap values that are
// equal to the pivot onto both ends of the subarray, and those values less
// than or greater than are partitioned in between. Once the first pass has
// completed, those values on the ends are then swapped into the middle,
// leaving the whole subarray properly partitioned into three as described above.

function swap (array, i, j) {
  var tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
}

function median3 (array, left, right, cmp) {
  var mid = left + ((right - left) >> 1);

  if (cmp(array[mid], array[left]) < 0) {
    swap(array, left, mid);
  }

  if (cmp(array[right], array[left]) < 0) {
    swap(array, left, right);
  }

  if (cmp(array[right], array[mid]) < 0) {
    swap(array, mid, right);
  }

  // place pivot where it can be moved as the algorithm progresses
  swap(array, mid, left);
  return array[left];
}

// this can be overridden by options passed by the user
function defaultCompare (a, b) {
  return (a < b) ? -1 : (b < a) ? 1 : 0;
}

// this sorting method will perform fewer swaps if there are many values equal to the pivot.
function quickSortManyDuplicates (array, left, right, cmp) {
  if (right <= left) {
    return;
  }

  var pivot = median3(array, left, right, cmp),
      i  = left + 1,
      lt = left,
      gt = right,
      result;

  while (i <= gt) {
    result = cmp(array[i], pivot);

    if (result < 0) {
      swap(array, lt++, i++);
    } else if (result > 0) {
      swap(array, i, gt--);
    } else {
      ++i;
    }
  }

  quickSortManyDuplicates(array, left, lt - 1, cmp);
  quickSortManyDuplicates(array, gt + 1, right, cmp);
}

// this sorting method will perform fewer swaps if there *are not* many values equal to the pivot.
function quickSortNotManyDuplicates (array, left, right, cmp) {
  if (right <= left) {
    return;
  }

  var pivot = median3(array, left, right, cmp),
      lt = left,
      gt = right,
      i  = left,
      j  = right;

  while (1) {
    if (cmp(array[i], pivot) === 0) {
      swap(array, i, lt++);
    }

    if (cmp(array[j], pivot) === 0) {
      swap(array, j, gt--);
    }

    while (cmp(array[++i], pivot) < 0);
    while (cmp(array[--j], pivot) > 0);

    if (i < j) {
      swap(array, i, j);
    } else {
      break;
    }
  }

  // move the values on both ends that are equal to the pivot,
  // into the middle of the subarray to leave it properly partitioned.
  while (lt > left) {
    swap(array, j--, --lt);
  }
  while (gt < right) {
    swap(array, i++, ++gt);
  }

  quickSortNotManyDuplicates(array, left, j, cmp);
  quickSortNotManyDuplicates(array, i, right, cmp);
}

function sort (array, options) {
  options = options || {};
  var cmp = (typeof options.compareFunc === 'function') ? options.compareFunc : defaultCompare;
  var sortFunc = (options.manyDuplicates) ? quickSortManyDuplicates : quickSortNotManyDuplicates;

  sortFunc(array, 0, array.length - 1, cmp);
}

module.exports = sort;
