var chai = require('chai');
var expect = chai.expect;
var Queue = require('../Queue');

chai.should();

describe('Queue', function () {
  it('throws when created with an unsupported type', function () {
    expect(new Queue('string')).to.be.an('object');
    expect(new Queue('number')).to.be.an('object');
    expect(function () { return new Queue(); }).to.throw(Error);
    expect(function () { return new Queue('nope!'); }).to.throw(Error);
    expect(function () { return new Queue({}); }).to.throw(Error);
  });

  it('enqueues and dequeues elements', function () {
    var q = new Queue('string');
    q.enqueue('yo');
    q.enqueue('world!');
    expect(q.dequeue()).to.equal('yo');
    expect(q.dequeue()).to.equal('world!');

    q = new Queue('number');
    q.enqueue(5);
    q.enqueue(10);
    expect(q.dequeue()).to.equal(5);
    expect(q.dequeue()).to.equal(10);
  });

  it('throws when enqueueing an element with the wrong type', function () {
    var q = new Queue('number');
    expect(function () { q.enqueue({}); }).to.throw(Error);
    expect(function () { q.enqueue('hi!'); }).to.throw(Error);
    expect(function () { q.enqueue(null); }).to.throw(Error);
    expect(function () { q.enqueue(); }).to.throw(Error);
  });

  it('throws when dequeueing an empty Queue', function () {
    var q = new Queue('number');
    expect(function () { q.dequeue(); }).to.throw(Error);
  });
});
