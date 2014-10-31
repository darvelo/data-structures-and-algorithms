var prototest = require('./prototest');
var insertionSort = require('../algorithms/InsertionSort.js');
var insertionSortIterators = require('../algorithms/InsertionSortIterators.js');

prototest({
  title: 'InsertionSort',
  sortFunc: insertionSort,
  sortFuncIterators: insertionSortIterators,
  testArrays: true,
  testIterators: true
});
