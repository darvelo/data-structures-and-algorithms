var testFactory = require('./testFactory');
var insertionSort = require('../algorithms/InsertionSort.js');
var insertionSortIterators = require('../algorithms/InsertionSortIterators.js');

testFactory({
  title: 'InsertionSort',
  sortFunc: insertionSort,
  sortFuncIterators: insertionSortIterators,
  testArrays: true,
  testIterators: true
});
