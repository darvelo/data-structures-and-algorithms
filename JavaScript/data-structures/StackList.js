function Node(data, next) {
    this.data = data;
    this.next = next || null;
}

function Stack() {
    this.head = this.tail = null;
}

Stack.prototype.push = function(data) {
    var node = new Node(data, this.head);
    this.head = node;
    return this;
};

Stack.prototype.pop = function() {
    if (!this.head) {
        throw new Error('Stack is empty, cannot pop!');
    }

    var node = this.head;
    this.head = node.next;
    node.next = null;
    return node.data;
};

Stack.prototype.top = function() {
    if (!this.head) {
        throw new Error('Stack is empty, cannot peek at top!');
    }

    return this.head.data;
};

Stack.prototype.empty = function() {
    return this.head === null;
};

Stack.prototype.clear = function() {
    while (!this.empty()) {
        this.pop();
    }

    return this;
};

module.exports = Stack;
