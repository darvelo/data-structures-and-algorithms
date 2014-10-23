function ListNode (data, prev, next) {
  this.data = (typeof data === 'undefined' ? null :  data);
  this.prev = (typeof prev === 'undefined' ? null :  prev);
  this.next = (typeof next === 'undefined' ? null :  next);
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
  };
}

function List (type) {
  if (type !== 'number' && type !== 'string') {
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
    destroyList: function () {
      this.clear();
      this.destroyNode(head);
      this.destroyNode(tail);
    },

    destroyNode: function (node) {
      node.next = node.prev = node.data = null;
    },

    checkVal: function (val) {
      if (typeof val !== type) {
        throw new Error('Value: "' + typeof val + '" was not a valid type for this list!');
      }
    },

    insert: function (itr, val) {
      this.checkVal(val);

      var currNode = itr.getNode(),
          newNode = new ListNode(val, currNode.prev, currNode);

      currNode.prev.next = currNode.prev = newNode;
      ++size;

      return new ListIterator(this, newNode);
    },

    erase: function (itr) {
      var currNode = itr.getNode(),
          nextNode = currNode.next;

      currNode.prev.next = currNode.next;
      currNode.next.prev = currNode.prev;
      --size;

      // allow garbage collection
      this.destroyNode(currNode);

      return new ListIterator(this, nextNode);
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
      var begin = this.begin();

      if (!begin.sameAs(this.end())) {
        this.erase(begin);
      }

      return this;
    },

    push_back: function (val) {
      this.checkVal(val);
      this.insert(this.end(), val);
      return this;
    },

    pop_back: function () {
      var end = this.end();

      if (!end.sameAs(this.begin())) {
        this.erase(end.prev());
      }

      return this;
    },

    begin: function () {
      return new ListIterator(this, head.next);
    },

    end: function () {
      return new ListIterator(this, tail);
    },

    front: function () {
      return head.next.data;
    },

    back: function () {
      return tail.prev.data;
    }
  };
}

module.exports = List;
