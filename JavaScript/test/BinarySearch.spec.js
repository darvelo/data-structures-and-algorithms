var chai = require('chai');
var binarySearch = require('../algorithms/BinarySearch');

chai.should();

describe('Binary Search', function () {
  var arr;

  beforeEach(function () {
    arr = [1,2,3,4,5,6,7,8,9];
  });

  it('finds an item at index 0', function () {
    binarySearch(arr, 1).should.equal(0);
  });

  it('finds an item at the last index', function () {
    binarySearch(arr, 9).should.equal(8);
  });

  it('finds an item in the middle', function () {
    binarySearch(arr, 5).should.equal(4);
  });

  it('finds an item between the beginning and the middle', function () {
    binarySearch(arr, 2).should.equal(1);
    binarySearch(arr, 3).should.equal(2);
    binarySearch(arr, 4).should.equal(3);
  });

  it('finds an item between the end and the middle', function () {
    binarySearch(arr, 6).should.equal(5);
    binarySearch(arr, 7).should.equal(6);
    binarySearch(arr, 8).should.equal(7);
  });

  it('fails to find an item less than the lowest value', function () {
    binarySearch(arr, 0).should.equal(-1);
  });

  it('fails to find an item greater than the highest value', function () {
    binarySearch(arr, 11).should.equal(-1);
  });
});
