function binarySearch (arr, val) {
  var low = 0,
      high = arr.length - 1,
      mid;

  while (low <= high) {
    mid = low + Math.floor((high - low) / 2);

    if (val < arr[mid]) {
      high = mid - 1;
    } else if (val > arr[mid]) {
      low = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
}

module.exports = binarySearch;
