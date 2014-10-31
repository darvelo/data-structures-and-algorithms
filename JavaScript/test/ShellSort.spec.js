var prototest = require('./prototest');
var shellSort = require('../algorithms/ShellSort.js');

prototest({
  title: 'ShellSort',
  sortFunc: shellSort,
  testArrays: true,
});
