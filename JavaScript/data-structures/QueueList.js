function Node(data, next) {
  this.data = data;
  this.next = next || null;
}

function Queue() {
  this.head = this.tail = null;
}

Queue.prototype.enqueue = function(data) {
  var node = new Node(data);

  if (!this.head) {
    this.head = this.tail = node;
  } else {
    this.tail = this.tail.next = node;
  }

  return this;
};

Queue.prototype.dequeue = function() {
  if (!this.head) {
    throw new Error('Empty Queue. Cannot dequeue!');
  }

  var node = this.head;
  var data = node.data;

  this.head = node.next;
  node.next = null;

  if (node === this.tail) {
    this.tail = null;
  }

  return data;
};

module.exports = Queue;
