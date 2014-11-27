var testFactory = require('./testFactory');
var shellSort = require('../algorithms/ShellSort.js');

testFactory({
  title: 'ShellSort',
  sortFunc: shellSort,
  sortFuncIterators: null,
  testArrays: true,
  testIterators: false
});
