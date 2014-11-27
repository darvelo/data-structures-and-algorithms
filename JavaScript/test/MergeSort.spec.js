var testFactory = require('./testFactory');
var mergeSort = require('../algorithms/MergeSort.js');

testFactory({
  title: 'MergeSort',
  sortFunc: mergeSort,
  sortFuncIterators: null,
  testArrays: true,
  testIterators: false
});
