var chai = require('chai');
var expect = chai.expect;
var Stack = require('../data-structures/StackList');

chai.should();

describe('Stack', function () {
  var s;

  beforeEach(function () {
    s = new Stack();
  });

  it('throws an error when popping an empty stack', function () {
    expect(function () { s.pop(); }).to.throw(Error);
  });

  it('throws an error when getting the top of an empty stack', function () {
    expect(function () { s.top(); }).to.throw(Error);
  });

  it('pushes and pops elements in order', function () {
    s.push(1)
     .push(2)
     .push(3)
     .push(4);

    expect(s.pop()).to.equal(4);
    expect(s.pop()).to.equal(3);
    expect(s.pop()).to.equal(2);
    expect(s.pop()).to.equal(1);
    expect(function () { s.pop(); }).to.throw(Error);
  });

  it('gets the top element', function () {
    s.push(1);
    s.top().should.equal(1);
    s.push(2);
    s.top().should.equal(2);
    s.push(3);
    s.top().should.equal(3);
    s.pop();
    s.top().should.equal(2);
    s.pop();
    s.top().should.equal(1);
  });

  it('knows whether it\'s empty', function () {
    s.empty().should.be.ok;
    s.push(1);
    s.empty().should.not.be.ok;
    s.push(2);
    s.empty().should.not.be.ok;

    s.pop();
    s.empty().should.not.be.ok;
    s.pop();
    s.empty().should.be.ok;
  });

  it('clears the stack', function () {
    s.empty().should.be.ok;
    s.push(1);
    s.empty().should.not.be.ok;
    s.push(2);
    s.empty().should.not.be.ok;
    s.push(3);
    s.empty().should.not.be.ok;

    s.pop();
    s.empty().should.not.be.ok;
    s.clear();
    s.empty().should.be.ok;
  });
});
