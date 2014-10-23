var chai = require('chai');
var expect = chai.expect;
var List = require('../data-structures/List');

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

    it('fails to add an element of an improper type', function () {
      lst = new List('number');
      lst.size().should.equal(0);
      lst.empty().should.be.ok;
      expect(function () { lst.push_back('yo!'); }).to.throw(Error);
      expect(function () { lst.push_front('yo!'); }).to.throw(Error);
      expect(function () { lst.insert(lst.begin(), 'yo!'); }).to.throw(Error);
      expect(function () { lst.push_back(2); }).to.not.throw(Error);
      expect(function () { lst.push_front(2); }).to.not.throw(Error);
      expect(function () { lst.insert(lst.begin(), 2); }).to.not.throw(Error);
      lst.size().should.equal(3);
      lst.empty().should.be.not.ok;

      lst.destroyList();

      lst = new List('string');
      lst.size().should.equal(0);
      lst.empty().should.be.ok;
      expect(function () { lst.push_back(2); }).to.throw(Error);
      expect(function () { lst.push_front(2); }).to.throw(Error);
      expect(function () { lst.insert(lst.begin(), 2); }).to.throw(Error);
      expect(function () { lst.push_back('yo!'); }).to.not.throw(Error);
      expect(function () { lst.push_front('yo!'); }).to.not.throw(Error);
      expect(function () { lst.insert(lst.begin(), 'yo!'); }).to.not.throw(Error);
      lst.size().should.equal(3);
      lst.empty().should.be.not.ok;
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

      lst.end().prev().data().should.equal('heyy3');
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

      lst.end().prev().data().should.equal('heyy0');
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

      lst.end().prev().data().should.equal('heyy0');
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

      lst.end().prev().data().should.equal('heyy3');
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

      lst.end().prev().data().should.equal(5);
    });
  });

  describe('removing elements', function () {
    afterEach(function () {
      lst.destroyList();
    });

    it('destroys a node properly', function () {
      var fakeNode = {
        data: 'fake',
        prev: 'fake',
        next: 'fake',
      };

      lst = new List('string');
      lst.destroyNode(fakeNode);

      expect(fakeNode.data).to.equal(null);
      expect(fakeNode.prev).to.equal(null);
      expect(fakeNode.next).to.equal(null);
    });

    it('clears a list', function () {
      lst = new List('number');
      lst.push_back(1)
        .push_back(1)
        .push_back(1)
        .push_back(1)
        .push_back(1);

      lst.size().should.equal(5);
      lst.clear();
      lst.size().should.equal(0);
      lst.begin().sameAs(lst.end()).should.be.ok;
    });

    it('can pop_front', function () {
      var i = 1,
          begin;

      lst = new List('number');

      lst.push_back(0)
        .push_back(1)
        .push_back(2);

      lst.size().should.equal(3);
      lst.pop_front();

      begin = lst.begin();

      while(!begin.sameAs(lst.end())) {
        begin.data().should.equal(i++);
        begin.next();
      }

      lst.end().prev().data().should.equal(2);
    });

    it('can pop_back', function () {
      var i = 0,
          begin;

      lst = new List('number');

      lst.push_back(0)
        .push_back(1)
        .push_back(2);

      lst.size().should.equal(3);
      lst.pop_back();

      begin = lst.begin();

      while(!begin.sameAs(lst.end())) {
        begin.data().should.equal(i++);
        begin.next();
      }

      lst.end().prev().data().should.equal(1);
    });

    it('can erase from the front', function () {
      var i = 1,
          begin;

      lst = new List('number'),
      lst.size().should.equal(0);

      lst.push_back(0)
        .push_back(1)
        .push_back(2);

      lst.size().should.equal(3);
      lst.erase(lst.begin());

      begin = lst.begin();

      while(!begin.sameAs(lst.end())) {
        begin.data().should.equal(i++);
        begin.next();
      }

      lst.end().prev().data().should.equal(2);
    });

    it('can erase from the back', function () {
      var i = 0,
          begin;

      lst = new List('number'),
      lst.size().should.equal(0);

      lst.push_back(0)
        .push_back(1)
        .push_back(2);

      lst.size().should.equal(3);
      lst.erase(lst.end().prev());

      begin = lst.begin();

      while(!begin.sameAs(lst.end())) {
        begin.data().should.equal(i++);
        begin.next();
      }

      lst.end().prev().data().should.equal(1);
    });

    it('can erase from somewhere in the middle', function () {
      var i,
          itr;

      lst = new List('number'),
      lst.size().should.equal(0);
      lst.empty().should.be.ok;

      itr = lst.begin();

      itr = lst.insert(itr, 3);
      itr = lst.insert(itr, 1);
      itr.next();
      itr = lst.insert(itr, 2);
      itr.next().next();
      itr = lst.insert(itr, 5);
      itr = lst.insert(itr, 4);
      itr = lst.insert(itr, 999);
      itr.next().next().next();

      itr.sameAs(lst.end()).should.be.ok;
      lst.size().should.equal(6);
      lst.empty().should.not.be.ok;

      itr.prev().prev().prev();
      lst.erase(itr);

      i = 1;
      itr = lst.begin();

      while (!itr.sameAs(lst.end())) {
        itr.data().should.equal(i++);
        itr.next();
      }

      lst.end().prev().data().should.equal(5);
    });
  });

  describe('accessing elements', function () {
    afterEach(function () {
      lst.destroyList();
    });

    it('gets the front element', function () {
      lst = new List('string');
      lst.push_front('world!');
      lst.push_front('hello');
      expect(lst.front()).to.equal('hello');
    });

    it('gets the front element with an iterator', function () {
      lst = new List('string');
      lst.push_front('world!');
      lst.push_front('hello');
      expect(lst.begin().data()).to.equal('hello');
    });

    it('gets the back element', function () {
      lst = new List('string');
      lst.push_back('hello');
      lst.push_back('world!');
      expect(lst.back()).to.equal('world!');
    });

    it('gets the back element with an iterator', function () {
      lst = new List('string');
      lst.push_back('hello');
      lst.push_back('world!');
      expect(lst.end().prev().data()).to.equal('world!');
    });

    it('sets data using an iterator', function () {
      var itr;

      lst = new List('string');
      lst.push_back('hello');
      lst.push_back('world!');

      itr = lst.begin();
      expect(itr.data()).to.equal('hello');

      itr.next();
      expect(itr.data()).to.equal('world!');

      itr.prev();
      expect(itr.data()).to.equal('hello');

      itr.setData('hi,');
      expect(itr.data()).to.equal('hi,');

      itr.next();
      expect(itr.data()).to.equal('world!');

      itr.setData('W3rld!');
      expect(itr.data()).to.equal('W3rld!');

      itr.prev();
      expect(itr.data()).to.equal('hi,');

      itr.next();
      expect(itr.data()).to.equal('W3rld!');
    });

    it('gets a clone of the current iterator', function () {
      var itr, clone;

      lst = new List('string');
      lst.push_back('hello');
      lst.push_back('world!');

      itr = lst.begin();
      clone = itr.clone();

      expect(itr.data()).to.equal(clone.data());

      itr = lst.begin().next();

      expect(itr.data()).to.not.equal(clone.data());

      clone.next();

      expect(itr.data()).to.equal(clone.data());

      itr = itr.clone();

      expect(itr.data()).to.equal(clone.data());
    });

    it('uses iterator plus operations to get an iterator of distant data', function () {
      var itr, newItr;

      lst = new List('string');
      lst.push_back('hello');
      lst.push_back('world,');
      lst.push_back('here');
      lst.push_back('i');
      lst.push_back('am!');

      itr = lst.begin();

      newItr = itr.plus(0);
      expect(itr.data()).to.equal('hello');
      expect(newItr.data()).to.equal('hello');

      // newItr is actually new and doesn't affect the other iterator
      newItr = itr.plus(1);
      expect(itr.data()).to.equal('hello');
      expect(newItr.data()).to.equal('world,');

      newItr = itr.plus(2);
      expect(newItr.data()).to.equal('here');
      newItr = itr.plus(3);
      expect(newItr.data()).to.equal('i');
      newItr = itr.plus(4);
      expect(newItr.data()).to.equal('am!');
    });

    it('uses iterator minus operations to get an iterator of distant data', function () {
      var itr, newItr;

      lst = new List('string');
      lst.push_back('hello');
      lst.push_back('world,');
      lst.push_back('here');
      lst.push_back('i');
      lst.push_back('am!');

      itr = lst.end();
      newItr = itr.minus(0);
      expect(itr.data()).to.equal(null);
      expect(newItr.data()).to.equal(null);

      // newItr is actually new and doesn't affect the other iterator
      newItr = itr.minus(1);
      expect(itr.data()).to.equal(null);
      expect(newItr.data()).to.equal('am!');

      newItr = itr.minus(2);
      expect(newItr.data()).to.equal('i');
      newItr = itr.minus(3);
      expect(newItr.data()).to.equal('here');
      newItr = itr.minus(4);
      expect(newItr.data()).to.equal('world,');
      newItr = itr.minus(5);
      expect(newItr.data()).to.equal('hello');
    });
  });
});
