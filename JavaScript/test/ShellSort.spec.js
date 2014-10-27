var chai = require('chai');
var expect = chai.expect;
var shellSort = require('../algorithms/ShellSort.js');

chai.should();

describe('ShellSort', function () {
  it('works on an empty array', function () {
    var arr = [];
    expect(shellSort.bind(null, arr)).to.not.throw(Error);
    expect(arr.length).to.equal(0);
  });

  it('works on an empty array', function () {
    var arr = [1];
    expect(shellSort.bind(null, arr)).to.not.throw(Error);
    expect(arr).to.deep.equal([1]);
  });

  it('sorts an array of size 2', function () {
    var arr = [85,20];
    shellSort(arr);
    expect(arr).to.deep.equal([20,85]);

    arr = [20,85];
    shellSort(arr);
    expect(arr).to.deep.equal([20,85]);
  });

  it('sorts an array of numbers', function () {
    var arr = [5,1,2,6,4,7,9,8,3];
    shellSort(arr);
    arr.should.deep.equal([1,2,3,4,5,6,7,8,9]);
  });

  it('sorts a very large array of numbers', function () {
    var arr = [],
        i = 0;

    while (i++ < 1000) {
      arr.push(Math.random() * 5000);
    }

    shellSort(arr);

    for (i = 1; i < arr.length; ++i) {
      expect(arr[i]).to.be.gte(arr[i-1]);
    }
  });

  it('sorts an array of numbers with the ends already in the right position', function () {
    var arr = [1,5,2,6,4,7,8,3,9];
    shellSort(arr);
    arr.should.deep.equal([1,2,3,4,5,6,7,8,9]);
  });

  it('sorts an array of strings', function () {
    var arr = ['how','now','brown','cow'];
    shellSort(arr);
    arr.should.deep.equal(['brown', 'cow', 'how', 'now']);
  });

  it('sorts an array of strings with the ends already in the right position', function () {
    var arr = ['brown','how','cow','now'];
    shellSort(arr);
    arr.should.deep.equal(['brown', 'cow', 'how', 'now']);
  });
});
