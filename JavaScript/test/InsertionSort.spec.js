var chai = require('chai');
var insertionSort = require('../algorithms/InsertionSort.js');
var insertionSortIterators = require('../algorithms/InsertionSortIterators.js');
var List = require('../data-structures/List.js');

chai.should();

describe('InsertionSort', function () {
  describe('without iterators', function () {
    it('sorts an array of numbers', function () {
      var arr = [5,1,2,6,4,7,9,8,3];
      insertionSort(arr);
      arr.should.deep.equal([1,2,3,4,5,6,7,8,9]);
    });

    it('sorts an array of numbers with the ends already in the right position', function () {
      var arr = [1,5,2,6,4,7,8,3,9];
      insertionSort(arr);
      arr.should.deep.equal([1,2,3,4,5,6,7,8,9]);
    });

    it('sorts an array of strings', function () {
      var arr = ['how','now','brown','cow'];
      insertionSort(arr);
      arr.should.deep.equal(['brown', 'cow', 'how', 'now']);
    });

    it('sorts an array of strings with the ends already in the right position', function () {
      var arr = ['brown','how','cow','now'];
      insertionSort(arr);
      arr.should.deep.equal(['brown', 'cow', 'how', 'now']);
    });
  });

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

      insertionSortIterators(lst.begin(), lst.end());

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

      insertionSortIterators(lst.begin(), lst.end());

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

      insertionSortIterators(lst.begin(), lst.end());

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

      insertionSortIterators(lst.begin(), lst.end());

      printed = [];
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        printed.push(begin.data());
        begin.next();
      }

      printed.should.deep.equal(['brown','cow','how','now']);
    });
  });
});
