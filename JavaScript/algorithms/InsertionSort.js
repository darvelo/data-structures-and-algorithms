function insertionSort(arr) {
  if (!arr.length) {
    return;
  }

  var i, j;

  for (i = 1; i < arr.length; ++i) {
    var tmp = arr[i];

    for (j = i; j > 0 && tmp < arr[j - 1]; --j) {
      arr[j] = arr[j -1];
    }

    arr[j] = tmp;
  }
}

module.exports = insertionSort;
