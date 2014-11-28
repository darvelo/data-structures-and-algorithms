var chai = require('chai');
var List = require('../data-structures/List.js');

var expect = chai.expect;
chai.should();

function bindArgs (func, args) {
  var slice = Array.prototype.slice;
  return function () {
    return func.apply(null, slice.apply(arguments).concat(args));
  };
}

function makeTests (options) {
  if (!options.title || !options.sortFunc) {
    throw new Error('makeTests was missing key options!');
  }

  var args = options.args || [],
      sortFunc = bindArgs(options.sortFunc, args),
      sortFuncIterators = bindArgs(options.sortFuncIterators || sortFunc, args);

  describe(options.title, function () {
    if (options.testArrays) {
      testArrays(sortFunc);
    }

    if (options.testIterators) {
      testIterators(sortFunc, sortFuncIterators);
    }

    if (typeof options.additionalTests === 'function') {
      options.additionalTests();
    }
  });
}

function testArrays (sortFunc) {
  describe('works using arrays', function () {
    it('works on an empty array', function () {
      var arr = [];
      expect(sortFunc.bind(null, arr)).to.not.throw(Error);
      expect(arr.length).to.equal(0);
    });

    it('works on an array of size 1', function () {
      var arr = [1];
      expect(sortFunc.bind(null, arr)).to.not.throw(Error);
      expect(arr).to.deep.equal([1]);
    });

    it('sorts an array of size 2', function () {
      var arr = [85,20];
      sortFunc(arr);
      expect(arr).to.deep.equal([20,85]);

      arr = [20,85];
      sortFunc(arr);
      expect(arr).to.deep.equal([20,85]);
    });

    it('sorts an array of numbers', function () {
      var arr = [5,1,2,6,4,7,9,8,3];
      sortFunc(arr);
      arr.should.deep.equal([1,2,3,4,5,6,7,8,9]);
    });

    it('sorts a very large array of numbers', function () {
      var arr = [],
      i = 0;

      while (i++ < 1000) {
        arr.push(Math.floor(Math.random() * 5000));
      }

      sortFunc(arr);

      for (i = 1; i < arr.length; ++i) {
        expect(arr[i]).to.be.gte(arr[i-1]);
      }
    });

    it('sorts an array of numbers with the ends already in the right position', function () {
      var arr = [1,5,2,6,4,7,8,3,9];
      sortFunc(arr);
      arr.should.deep.equal([1,2,3,4,5,6,7,8,9]);
    });

    it('sorts an array of strings', function () {
      var arr = ['how','now','brown','cow'];
      sortFunc(arr);
      arr.should.deep.equal(['brown', 'cow', 'how', 'now']);
    });

    it('sorts an array of strings with the ends already in the right position', function () {
      var arr = ['brown','how','cow','now'];
      sortFunc(arr);
      arr.should.deep.equal(['brown', 'cow', 'how', 'now']);
    });
  });
}

function testIterators (sortFunc, sortFuncIterators) {
  describe('with iterators', function () {
    it('sorts a list of numbers', function () {
      var lst = new List('number'),
      printed = [],
      begin;

      lst.push_back(5)
      .push_back(1)
      .push_back(2)
      .push_back(6)
      .push_back(4)
      .push_back(7)
      .push_back(9)
      .push_back(8)
      .push_back(3);

      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal([5,1,2,6,4,7,9,8,3]);

      sortFuncIterators(lst.begin(), lst.end());

      printed = [];
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal([1,2,3,4,5,6,7,8,9]);
    });

    it('sorts a list of numbers with the ends already in the right position', function () {
      var lst = new List('number'),
      printed = [],
      begin;

      lst.push_back(1)
      .push_back(5)
      .push_back(2)
      .push_back(6)
      .push_back(4)
      .push_back(7)
      .push_back(8)
      .push_back(3)
      .push_back(9);

      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal([1,5,2,6,4,7,8,3,9]);

      sortFuncIterators(lst.begin(), lst.end());

      printed = [];
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal([1,2,3,4,5,6,7,8,9]);
    });

    it('sorts a list of strings', function () {
      var lst = new List('string'),
      printed = [],
      begin;

      lst.push_back('how')
      .push_back('now')
      .push_back('brown')
      .push_back('cow');

      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal(['how','now','brown','cow']);

      sortFuncIterators(lst.begin(), lst.end());

      printed = [];
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal(['brown','cow','how','now']);
    });

    it('sorts a list of strings with the ends already in the right position', function () {
      var lst = new List('string'),
      printed = [],
      begin;

      lst.push_back('brown')
      .push_back('how')
      .push_back('cow')
      .push_back('now');

      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal(['brown','how','cow','now']);

      sortFuncIterators(lst.begin(), lst.end());

      printed = [];
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal(['brown','cow','how','now']);
    });
  });
}

module.exports = makeTests;
