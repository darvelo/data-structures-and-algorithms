var chai = require('chai');
var expect = chai.expect;
var List = require('../List');

chai.should();

describe('List', function () {
  it('makes a list -- checks it twice', function () {
    var lst = new List('number');
    expect(lst.size()).to.equal(0);
    expect(lst.empty()).to.be.ok;

    lst = new List('string');
    expect(lst.size()).to.equal(0);
    expect(lst.empty()).to.be.ok;
  });
});
