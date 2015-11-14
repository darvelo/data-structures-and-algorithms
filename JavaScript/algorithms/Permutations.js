// This is my recursive implementation of the Steinhaus-Johnson-Trotter algorithm

function swap(array, i, j) {
    var tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

function permute(array) {
    if (array.length < 2) {
        return [array];
    }

    var permutations = [];
    var p = permute(array.slice(0, -1));
    var plen = p.length;
    var n = array.length;
    var bound = -1;
    var j;

    for (var i = 0; i < plen; ++i) {
        j = n-1;
        p[i].push(array[n-1]);
        permutations.push(p[i].slice());
        while (j-1 !== bound) {
            swap(p[i], j, j-1);
            permutations.push(p[i].slice());
            j -= 1;
        }
    }

    return permutations;
}

var args = process.argv.slice(2);
var permutations = permute(args);
console.log('Recursive implementation:');
console.log(permutations);
console.log('Number of permutations:', permutations.length);
console.log();


/*****************************
 * with a generator function *
 *****************************/

function* permuteGenerator(array) {
    var n = array.length;
    var bound = -1;
    var j, p, permutations;

    if (n < 2) {
        yield array;
        permutations = [];
    } else {
        permutations = permute(array.slice(0, -1));
    }

    for (p of permutations) {
        j = n-1;
        p.push(array[n-1]);
        yield p.slice();
        while (j-1 !== bound) {
            swap(p, j, j-1);
            yield p.slice();
            j -= 1;
        }
    }
}

console.log('Recursive implementation using a generator function:');
var count = 0;
for (var p of permuteGenerator(args)) {
    count++;
    console.log(p);
}
console.log('Number of permutations:', count);
