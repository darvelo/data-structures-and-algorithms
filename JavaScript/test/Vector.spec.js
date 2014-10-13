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

  describe('Manipulating the Vector', function () {
    it('can only push_back elements of the proper type', function () {
      var vi = new Vector('number');
      expect(vi.push_back.bind(vi, 42)).to.not.throw(Error);
      expect(vi.push_back.bind(vi)).to.throw(Error);
      expect(vi.push_back.bind(vi, 'nope!')).to.throw(Error);
      expect(vi.push_back.bind(vi, {})).to.throw(Error);
      expect(vi.push_back.bind(vi, [])).to.throw(Error);

      var vs = new Vector('string');
      expect(vs.push_back.bind(vs, 'yep!')).to.not.throw(Error);
      expect(vs.push_back.bind(vs)).to.throw(Error);
      expect(vs.push_back.bind(vs, 42)).to.throw(Error);
      expect(vs.push_back.bind(vs, {})).to.throw(Error);
      expect(vs.push_back.bind(vs, [])).to.throw(Error);
    });

    it('pushes back elements properly', function () {
      var v = new Vector('number');
      v.size().should.equal(0);
      v.push_back(12);
      v.size().should.equal(1);
      expect(v.at(0)).to.equal(12);
      v.push_back(15);
      v.size().should.equal(2);
      expect(v.at(1)).to.equal(15);

      v = new Vector('string', 2);
      v.size().should.equal(2);
      v.push_back('third');
      v.size().should.equal(3);
      expect(v.at(2)).to.equal('third');
      v.push_back('fourth');
      v.size().should.equal(4);
      expect(v.at(3)).to.equal('fourth');
    });

    it('pops elements', function () {
      var v = new Vector('string');
      v.size().should.equal(0);
      v.push_back('first');
      v.size().should.equal(1);
      v.push_back('second');
      v.size().should.equal(2);

      v.pop_back().should.equal(1);
      v.size().should.equal(1);
      v.pop_back().should.equal(0);
      v.size().should.equal(0);
    });

    it('clears a vector', function () {
      var v = new Vector('string', 10);
      v.size().should.equal(10);
      v.clear();
      v.size().should.equal(0);
    });

    it('can\'t access an element that isn\'t there', function () {
      var v = new Vector('number');
      expect(v.at(0)).to.be.an.instanceof(Error);
      expect(v.at(1)).to.be.an.instanceof(Error);
      expect(v.at(2)).to.be.an.instanceof(Error);
      expect(v.at(20)).to.be.an.instanceof(Error);
      v.push_back(100);
      expect(v.at(0)).to.not.be.an.instanceof(Error);
      expect(v.at(0)).to.equal(100);
      expect(v.at(1)).to.be.an.instanceof(Error);
      expect(v.at(2)).to.be.an.instanceof(Error);
      v.push_back(200);
      expect(v.at(1)).to.equal(200);
      expect(v.at(0)).to.not.be.an.instanceof(Error);
      expect(v.at(1)).to.not.be.an.instanceof(Error);
      expect(v.at(2)).to.be.an.instanceof(Error);
      v.push_back(300);
      expect(v.at(0)).to.not.be.an.instanceof(Error);
      expect(v.at(2)).to.equal(300);
      expect(v.at(1)).to.not.be.an.instanceof(Error);
      expect(v.at(2)).to.not.be.an.instanceof(Error);
    });

    it('accesses the back element', function () {
      var v = new Vector('string');
      expect(v.back()).to.be.an.instanceof(Error);
      v.push_back('yo!');
      expect(v.back()).to.not.be.an.instanceof(Error);
      v.back().should.equal('yo!');
    });

    it('can create a copy of its elements into an array', function () {
      var v = new Vector('number');
      v.push_back(1)
        .push_back(2)
        .push_back(3)
        .push_back(4)
        .push_back(5);

      v.size().should.equal(5);
      expect(v.copy()).to.deep.equal([1,2,3,4,5]);
    });

    it('can copy one vector into another', function () {
      var v1 = new Vector('number');
      v1.push_back(100);
      v1.push_back(200);
      v1.push_back(300);
      expect(v1.copy()).to.deep.equal([100,200,300]);

      var v2 = new Vector('number');
      v2.push_back(400);
      v2.push_back(500);
      v2.push_back(600);
      expect(v2.copy()).to.deep.equal([400,500,600]);

      expect(v1.copyFrom.bind(v1, v2)).to.not.throw(Error);
      expect(v1.copy()).to.deep.equal([400,500,600]);
    });

    it('fails to copy a vector of a different type', function () {
      var v1 = new Vector('number');
      v1.push_back(100);
      expect(v1.copy()).to.deep.equal([100]);

      var v2 = new Vector('string');
      v2.push_back('yo!');
      expect(v2.copy()).to.deep.equal(['yo!']);

      expect(v1.copyFrom.bind(v1, v2)).to.throw(Error);
      expect(v1.copy()).to.deep.equal([100]);
    });

    it('resizes a vector only to smaller than its size', function () {
      var v = new Vector('number');
      v.resize(100);
      v.size().should.equal(0);

      v.push_back(1);
      v.resize(100);
      v.size().should.equal(1);

      v.push_back(1);
      v.push_back(1);
      v.push_back(1);

      v.size().should.equal(4);
      v.resize(100);
      v.size().should.equal(4);
      v.resize(1);
      v.size().should.equal(1);
      v.resize(0);
      v.size().should.equal(0);
    });

    it('fails to resize when the new size is less than zero', function () {
      var v = new Vector('number');
      expect(v.resize.bind(v, -1)).to.throw(Error);
    });
  });

  describe('Traversing the Vector with Iterators', function () {
    it('returns an Error when returning an iterator to a non-existent position', function () {
      var v = new Vector('string');

      expect(v.begin().curr()).to.be.an.instanceof(Error);
      expect(v.end().curr()).to.be.an.instanceof(Error);

      v.push_back('yo!');
      expect(v.begin().curr()).to.not.be.an.instanceof(Error);
      expect(v.begin().curr()).to.be.a('string');
      expect(v.begin().curr()).to.equal('yo!');

      expect(v.begin().next()).to.be.an.instanceof(Error);
    });

    it('returns an iterator with methods begin and end', function () {
      var v = new Vector('string'),
          begin,
          end;

      v.push_back('yo!');

      expect(v.begin()).to.not.be.an.instanceof(Error);
      expect(v.end()).to.not.be.an.instanceof(Error);

      begin = v.begin();
      end = v.end();

      expect(begin.curr).to.be.a('function');
      expect(begin.next).to.be.a('function');
      expect(begin.prev).to.be.a('function');
      expect(end.curr).to.be.a('function');
      expect(end.next).to.be.a('function');
      expect(end.prev).to.be.a('function');
    });

    it('returns the same iterator when it only has one element', function () {
      var v = new Vector('string'),
          begin,
          end;

      v.push_back('yo!');

      begin = v.begin();
      end = v.end();

      expect(begin.getVector()).to.equal(v);
      expect(end.getVector()).to.equal(v);
      expect(begin.getIndex()).to.equal(0);
      expect(end.getIndex()).to.equal(1);

      expect(begin.next()).to.be.an.instanceof(Error);
      expect(begin.sameAs(end)).to.be.ok;
    });

    it('can traverse forward and backward', function () {
      var v = new Vector('string'),
          begin,
          end;

      v.push_back('hi')
        .push_back('world!')
        .push_back('i')
        .push_back('love')
        .push_back('you');

      begin = v.begin();
      end = v.end();

      begin.curr().should.equal('hi');
      begin.next().should.equal('world!');
      begin.next().should.equal('i');
      begin.next().should.equal('love');
      begin.next().should.equal('you');
      begin.next().should.be.an.instanceof(Error);
      begin.sameAs(end).should.be.ok;
      begin.prev().should.equal('you');
      begin.prev().should.equal('love');
      begin.prev().should.equal('i');
      begin.prev().should.equal('world!');
      begin.prev().should.equal('hi');
      begin.prev().should.be.an.instanceof(Error);

      end.curr().should.be.an.instanceof(Error);
      end.prev().should.equal('you');
      end.prev().should.equal('love');
      end.prev().should.equal('i');
      end.prev().should.equal('world!');
      end.prev().should.equal('hi');
      end.sameAs(v.begin()).should.be.ok;
      end.prev().should.be.an.instanceof(Error);
      end.next().should.equal('hi');
      end.next().should.equal('world!');
      end.next().should.equal('i');
      end.next().should.equal('love');
      end.next().should.equal('you');
      end.next().should.be.an.instanceof(Error);
    });

    it('can access the whole vector with a while loop', function () {
      var v = new Vector('number'),
          idx = 0,
          begin,
          end;

      v.push_back(1)
        .push_back(2)
        .push_back(3)
        .push_back(4)
        .push_back(5);

      begin = v.begin();
      end = v.end();

      while (!begin.sameAs(end)) {
        expect(begin.curr()).to.equal(v.at(idx++));
        begin.next();
      }
    });
  });
});
