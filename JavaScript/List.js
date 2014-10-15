function ListNode (data, prev, next) {
  this.data = data || null;
  this.prev = prev || null;
  this.next = next || null;
}

function ListIterator (list, node) {
  return {
    getList: function () { return list; },
    getNode: function () { return node; },
    data: function () { return node.data; },
    next: function () {
      node = node.next;
      return this;
    },
    prev: function () {
      node = node.prev;
      return this;
    },
    sameAs: function (itr) {
      if (list !== itr.getList()) {
        throw new Error('Tried to compare iterators to different lists!');
      }

      return node === itr.getNode();
    },
    destroyNode: function () {
      node = node.next = node.prev = node.data = null;
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

    checkVal: function (val) {
      if (typeof val !== type) {
        throw new Error('Value: "%s" was not a valid type for this list!', typeof val);
      }
    },

    insert: function (itr, val) {
      this.checkVal(val);

      var newNode = new ListNode(val, itr.prev().getNode(), itr.next().getNode());
      itr.prev().getNode().next = itr.next().getNode().prev = newNode;
      ++size;
      return this;
    },

    erase: function (itr) {
      itr.prev().getNode().next = itr.next().getNode();
      itr.next().getNode().prev = itr.prev().getNode();

      // allow garbage collection
      itr.destroyNode();

      --size;
      return this;
    },

    clear: function () {
      while (!this.empty()) {
        this.pop_front();
      }

      return this;
    },

    push_front: function (val) {
      this.checkVal(val);
      this.insert(this.begin(), val);
      return this;
    },

    pop_front: function () {
      this.erase(this.begin());
      return this;
    },

    push_back: function (val) {
      this.checkVal(val);
      this.insert(this.end(), val);
      return this;
    },

    pop_back: function () {
      this.erase(this.end().prev());
      return this;
    },

    begin: function () {
      return new ListIterator(this, head.next);
    },

    end: function () {
      return new ListIterator(this, tail);
    },
  };
}

module.exports = List;
