// this mergesort method is a nearly verbatim copy from Mike Bostock's fantastic article,
// [Visualizing Algorithms](http://bost.ocks.org/mike/algorithms), a highly recommended read.
// it's a bottom-up mergesort optimized for time, avoiding extra copies
// into a temporary array by switching inputs to the `merge` function.
// the bitshift operators double the size of the subarrays on each pass.
function MergeSort (array) {
  var n = array.length,
      a0 = array,
      a1 = new Array(n),
      i, m, left, center, right;

  for (m = 1; m < n; m <<= 1) {
    for (i = 0; i < n; i += m << 1) {
      left = i;
      center = Math.min(i + m, n);
      right = Math.min(i + (m << 1), n);
      merge(a0, a1, left, center, right);
    }

    array = a0, a0 = a1, a1 = array;
  }

  // the final sorted array from merge() is a1, but is swapped with a0 above.
  // depending on the number of elements, the final merge operation may actually have
  // taken place on the temporary array, so in that case the elements must be copied
  // back into the array originally passed into this function.
  if (array === a1) {
    for (i = 0; i < n; ++i) {
      array[i] = a0[i];
    }
  }
}

// merge subarray elements from a0 into a1 in sorted order,
// keeping the original order of equal-value elements.
function merge(a0, a1, left, center, right) {
  var i, j;
  for (i = left, j = center; left < right; ++left) {
    if (i < center && (j >= right || a0[i] <= a0[j])) {
      a1[left] = a0[i++];
    } else {
      a1[left] = a0[j++];
    }
  }
}

module.exports = MergeSort;
