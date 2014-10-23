function insertionSortIterators(begin, end) {
  if (begin.sameAs(end)) {
    return;
  }

  var i = begin.clone().next();

  while (!i.sameAs(end)) {
    var tmp = i.data(),
        j = i.clone();

    while (!j.sameAs(begin) && tmp < j.minus(1).data()) {
      j.setData(j.minus(1).data());
      j.prev();
    }

    j.setData(tmp);
    i.next();
  }
}

module.exports = insertionSortIterators;
