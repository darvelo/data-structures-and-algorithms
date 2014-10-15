function ListNode (data, prev, next) {
  this.data = data || null;
  this.prev = prev || null;
  this.next = next || null;
}

function ListIterator (list, node) {
  var current = node;

  return {
    curr: function () { return current; },
    next: function () {
      current = current.next;
      return this;
    },
    prev: function () {
      current = current.prev;
      return this;
    },
  };
}

function List (type) {
  if (typeof type !== 'number' && typeof type !== 'string') {
    throw new Error('The given initial type for the new List was not valid! Value: ' + type);
  }

  var head = new ListNode(),
      tail = new ListNode(),
      size = 0;

  head.next = tail;
  tail.prev = head;

  return {
    size: function () { return size; },
    empty: function () { return this.size() === 0; },

    insert: function (itr, val) {
      if (typeof val !== type) {
        throw new Error('Value: "%s" was not a valid type for this list!', typeof val);
      }

      var newNode = new ListNode(val, itr.prev().curr(), itr.next().curr());
      itr.prev().curr().next = itr.next().curr().prev = newNode;
      ++size;
      return this;
    },

    erase: function (itr) {

    },
  };
}

module.exports = List;
