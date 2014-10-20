function Stack () {
  var array = [];

  return {
    push: function (element) {
      array.push(element);
      return this;
    },

    pop: function () {
      if (array.length === 0) {
        throw new Error('Stack is empty -- cannot pop!');
      }

      return array.pop();
    },

    top: function () {
      if (array.length === 0) {
        throw new Error('Stack is empty -- cannot get top!');
      }

      return array[array.length - 1];
    },

    empty: function () {
      return array.length === 0;
    },

    clear: function () {
      while (!this.empty()) {
        this.pop();
      }

      return this;
    },
  };
}

module.exports = Stack;
