var chai = require('chai');
var expect = chai.expect;
var Vector = require('../Vector');

chai.should();

describe('Vector', function () {
  describe('Creating a Vector', function () {
    it('creates a vector', function () {
      var v = new Vector('number');
      expect(v.isVector).to.be.ok;
    });

    it('creates a vector of zero-size by default', function () {
      var v = new Vector('number');
      expect(v.size()).to.equal(0);
      expect(v.empty()).to.be.ok;
    });

    it('creates a vector of given size with default values', function () {
      var vNumber = new Vector('number', 5);
      expect(vNumber.size()).to.equal(5);
      expect(vNumber.copy()).to.deep.equal([0,0,0,0,0]);
      expect(vNumber.empty()).to.not.be.ok;

      var vString = new Vector('string', 3);
      expect(vString.size()).to.equal(3);
      expect(vString.copy()).to.deep.equal(['','','']);
      expect(vString.empty()).to.not.be.ok;
    });

    it('fails to created a vector without a proper type', function () {
      expect(Vector).to.throw(Error);
      expect(Vector.bind(null, 'nonType')).to.throw(Error);
    });

    it('fails to created a vector without a proper size', function () {
      expect(Vector.bind(null, 'number', 'woo!')).to.throw(Error);
    });
  });
});
