function Iterator (vector, idx) {
  if (!vector.isVector) {
    throw new Error('Value passed to Iterator was not a Vector type!');
  }

  var current = vector.at(idx);

  return {
    getVector: function () {
      return vector;
    },

    getIndex: function () {
      return idx;
    },

    sameAs: function (iter) {
      return (vector === iter.getVector() && this.idx === iter.getIndex());
    },

    curr: function () {
      return current;
    },

    next: function () {
      current = vector.at(++idx);
      return current;
    },

    prev: function () {
      current = vector.at(--idx);
      return current;
    },
  };
}

function Vector (type, size) {
  var objects = [],
      i;

  if (type !== 'string' && type !== 'number') {
    throw new Error('Incorrect type for new Vector type: ' + type);
  }

  if (typeof size === 'undefined') {
    size = 0;
  } else if (typeof size !== 'number') {
    throw new Error('Incorrect type for size of new Vector: ' + size);
  }

  // setup initial vector elements
  for (i = 0; i < size; ++i) {
    if (type === 'string') {
      objects[i] = '';
    } else if (type === 'number') {
      objects[i] = 0;
    }
  }

  return {
    isVector: true,

    push_back: function (val) {
      if (typeof val !== type) {
        throw new Error('Vector#push_back: Type mismatch on type ' + type + ' and value ' + val);
      }

      objects.push(val);
      return this;
    },

    pop_back: function () {
      objects.pop();
      return objects.length;
    },

    size: function () {
      return objects.length;
    },

    empty: function () {
      return objects.length === 0;
    },

    clear: function () {
      objects = [];
    },

    at: function (idx) {
      if (idx < 0 || idx > objects.length - 1) {
        throw new Error('Out of bounds!');
      }

      return objects[idx];
    },

    back: function () {
      return objects[objects.length - 1];
    },

    begin: function () {
      return new Iterator(this, 0);
    },

    end: function () {
      return new Iterator(this, objects.length-1);
    },

    copy: function () {
      return objects.slice(0);
    },

    copyFrom: function (vector) {
      if (! vector instanceof Vector) {
        throw new Error('Tried to copy from a non-Vector!');
      }

      objects = vector.copy();
    },

    resize: function (newSize) {
      if (newSize < 0) {
        throw new Error('Out of bounds!');
      }

      objects = objects.slice(0, newSize);
    },
  };
}

module.exports = Vector;
