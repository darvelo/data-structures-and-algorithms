var chai = require('chai');
var expect = chai.expect;
var Queue = require('../data-structures/QueueList');

chai.should();

describe('Queue', function () {
  it('enqueues and dequeues elements', function () {
    var undef;
    var obj = {test: 'object'};
    var q = new Queue('string');
    q.enqueue('yo');
    q.enqueue('world!');
    q.enqueue(null);
    q.enqueue();
    q.enqueue(obj);
    expect(q.dequeue()).to.equal('yo');
    expect(q.dequeue()).to.equal('world!');
    expect(q.dequeue()).to.equal(null);
    expect(q.dequeue()).to.equal(undef);
    expect(q.dequeue()).to.equal(obj);

    q = new Queue('number');
    q.enqueue(5);
    q.enqueue(10);
    expect(q.dequeue()).to.equal(5);
    expect(q.dequeue()).to.equal(10);
  });

  it('throws when dequeueing an empty Queue', function () {
    var q = new Queue('number');
    expect(function () { q.dequeue(); }).to.throw(Error);
  });
});
