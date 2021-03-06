function Iterator (vector, idx) {
  if (!vector.isVector()) {
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
      return (vector === iter.getVector() && idx === iter.getIndex());
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
      isVector = true,
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
    isVector: function () { return isVector; },

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
      objects.length = 0;
    },

    at: function (idx) {
      if (idx < 0 || idx > objects.length - 1) {
        return new Error('Out of bounds!');
      }

      return objects[idx];
    },

    back: function () {
      return this.at(objects.length - 1);
    },

    begin: function () {
      return new Iterator(this, 0);
    },

    end: function () {
      return new Iterator(this, objects.length);
    },

    copy: function () {
      return objects.slice();
    },

    copyFrom: function (vector) {
      if (!vector.isVector()) {
        throw new Error('Tried to copy from a non-Vector!');
      }

      var copy = vector.copy();

      if (!Array.isArray(copy)) {
        throw new Error('Vector#copy did not return a proper array!');
      }

      if (copy.length && typeof copy[0] !== type) {
        throw new Error('The vector was not of the same type as the vector being copied!');
      }

      objects = copy;
    },

    resize: function (newSize) {
      if (newSize < 0) {
        throw new Error('Out of bounds!');
      }

      // only resize to smaller than current size
      if (newSize > this.size()) {
        return;
      }

      objects.length = newSize;
    },
  };
}

module.exports = Vector;
