function Queue (type) {
  if (type !== 'string' && type !== 'number') {
    throw new Error('Incorrect type for new Queue type: ' + type);
  }

  var array = [];

  return {
    enqueue: function (element) {
      if (typeof element !== type) {
        throw new Error('Tried to enqueue a new element with an incorrect type: ' + type);
      }

      array.unshift(element);
      return this;
    },

    dequeue: function () {
      if (!array.length) {
        throw new Error('Queue was empty. Cannot dequeue!');
      }

      return array.pop();
    },
  };
}

module.exports = Queue;
