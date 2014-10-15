var chai = require('chai');
var expect = chai.expect;
var List = require('../List');

chai.should();

describe('List', function () {
  var lst;

  describe('creating a list', function () {
    afterEach(function () {
      lst.destroyList();
    });

    it('makes a list -- checks it twice', function () {
      lst = new List('number');
      expect(lst.size()).to.equal(0);
      expect(lst.empty()).to.be.ok;

      lst.destroyList();

      lst = new List('string');
      expect(lst.size()).to.equal(0);
      expect(lst.empty()).to.be.ok;
    });

    it('fails to make a list without a proper type', function () {
      expect(function () { lst = new List('string'); }).to.not.throw(Error);
      expect(function () { lst = new List('number'); }).to.not.throw(Error);
      expect(function () { return new List(); }).to.throw(Error);
      expect(function () { return new List('wrongType'); }).to.throw(Error);
    });
  });

  describe('adding elements', function () {
    afterEach(function () {
      lst.destroyList();
    });

    it('an empty list has begin() equal to end()', function () {
      lst = new List('number');
      lst.begin().sameAs(lst.end()).should.be.ok;
      expect(lst.end().getNode().prev).to.be.an('object');
      expect(lst.end().getNode().next).to.equal(null);
      expect(lst.end().getNode().data).to.equal(null);
      expect(lst.end().data()).to.equal(null);
      expect(lst.begin().prev().getNode().next).to.be.an('object');
      expect(lst.begin().prev().getNode().prev).to.equal(null);
      expect(lst.begin().prev().getNode().data).to.equal(null);
      expect(lst.begin().prev().data()).to.equal(null);
    });

    it('push_back elements', function () {
      var i,
          begin;

      lst = new List('string'),
      lst.size().should.equal(0);

      for (i = 0; i !== 4; ++i) {
        lst.push_back('heyy' + i);
        lst.size().should.equal(i+1);
      }

      i = 0;
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        begin.data().should.equal('heyy' + i++);
        begin.next();
      }
    });

    it('push_front elements', function () {
      var i,
          begin;

      lst = new List('string'),
      lst.size().should.equal(0);

      for (i = 0; i !== 4; ++i) {
        lst.push_front('heyy' + i);
        lst.size().should.equal(i+1);
      }

      i = 3;
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        begin.data().should.equal('heyy' + i--);
        begin.next();
      }
    });

    it('insert elements from the front', function () {
      var i,
          begin;

      lst = new List('string'),
      lst.size().should.equal(0);

      for (i = 0; i !== 4; ++i) {
        lst.insert(lst.begin(), 'heyy' + i);
        lst.size().should.equal(i+1);
      }

      i = 3;
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        begin.data().should.equal('heyy' + i--);
        begin.next();
      }
    });

    it('insert elements from the back', function () {
      var i,
          begin;

      lst = new List('string'),
      lst.size().should.equal(0);

      for (i = 0; i !== 4; ++i) {
        lst.insert(lst.end(), 'heyy' + i);
        lst.size().should.equal(i+1);
      }

      i = 0;
      begin = lst.begin();

      while (!begin.sameAs(lst.end())) {
        begin.data().should.equal('heyy' + i++);
        begin.next();
      }
    });

    it('inserts elements from somewhere in the middle', function () {
      var i,
          itr;

      lst = new List('number'),
      lst.size().should.equal(0);

      itr = lst.begin();

      itr = lst.insert(itr, 3);
      itr = lst.insert(itr, 1);
      itr.next();
      itr = lst.insert(itr, 2);
      itr.next().next();
      itr = lst.insert(itr, 5);
      itr = lst.insert(itr, 4);
      itr.next().next();

      itr.sameAs(lst.end()).should.be.ok;
      lst.size().should.equal(5);

      i = 1;
      itr = lst.begin();

      while (!itr.sameAs(lst.end())) {
        itr.data().should.equal(i++);
        itr.next();
      }
    });
  });
});
