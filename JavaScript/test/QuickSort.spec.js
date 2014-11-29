var testFactory = require('./testFactory');
var quickSort = require('../algorithms/QuickSort.js');

testFactory({
  title: 'QuickSort Not Many Duplicates',
  sortFunc: quickSort,
  sortFuncIterators: null,
  testArrays: true,
  testIterators: false,
  additionalTests: additionalTests,
});

testFactory({
  title: 'QuickSort Many Duplicates',
  sortFunc: quickSort,
  sortFuncIterators: null,
  args: [{ manyDuplicates: true }],
  testArrays: true,
  testIterators: false,
  additionalTests: additionalTests,
});

function additionalTests () {
  describe('custom compare function', function () {
    it('should sort in reverse order', function () {
      var array = [4,1,6,3,7,9,8,2,5];
      var cmp = function (a, b) {
        return b - a;
      };

      quickSort(array, { compareFunc: cmp });

      array.should.deep.equal([9,8,7,6,5,4,3,2,1]);
    });

    it('should sort objects by a key', function () {
      var array = [{ key: 'dude' }, { key: 'wheres' }, { key: 'my' }, { key: 'car' }];
      var cmp = function (a, b) {
        return a.key.length - b.key.length;
      };

      quickSort(array, { compareFunc: cmp });

      array.should.deep.equal([{ key: 'my' }, { key: 'car' }, { key: 'dude' }, { key: 'wheres' }]);
    });

    it('should sort with many duplicate keys', function () {
      var array = [1,4,3,2,5,3,7,6,7,4,4,4,4,7,6,3,3,3,3,3,1,2,2];
      quickSort(array);
      array.should.deep.equal([1,1,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,5,6,6,7,7,7]);
    });
  });
}
