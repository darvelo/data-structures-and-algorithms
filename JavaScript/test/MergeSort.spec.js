var prototest = require('./prototest');
var mergeSort = require('../algorithms/MergeSort.js');

prototest({
  title: 'MergeSort',
  sortFunc: mergeSort,
  testArrays: true,
});
