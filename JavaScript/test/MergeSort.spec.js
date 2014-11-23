var prototest = require('./prototest');
var mergeSort = require('../algorithms/MergeSort.js');

prototest({
  title: 'MergeSort',
  sortFunc: mergeSort,
  sortFuncIterators: null,
  testArrays: true,
  testIterators: false
});
