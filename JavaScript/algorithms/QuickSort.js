// This is an example of entropy-optimal sorting, where arrays with
// large numbers of duplicate keys can be efficiently sorted. The
// trick is to partition the subarrays into three, in the order of:
//
//   * those values less than the pivot
//   * those values equal to the pivot
//   * those values greater than the pivot
//
// This method is great if you know your array has a large number of
// duplicate keys, but is less performant than standard quicksort if not,
// since there are many more swaps for non-duplicates here by comparison.

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

function defaultCompare (a, b) {
  return (a < b) ? -1 : (b < a) ? 1 : 0;
}

function quickSort (array, left, right, cmp) {
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

  quickSort(array, left, lt - 1, cmp);
  quickSort(array, gt + 1, right, cmp);
}

function sort (array, options) {
  options = options || {};
  var cmp = (typeof options.compareFunc === 'function') ? options.compareFunc : defaultCompare;
  quickSort(array, 0, array.length - 1, cmp);
}

module.exports = sort;
