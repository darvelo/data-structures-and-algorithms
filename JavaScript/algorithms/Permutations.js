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

    var n = array.length;
    var permutations = [];
    var p = permute(array.slice(0, -1));
    var plen = p.length;

    var direction = -1;
    var j = n-1;
    var bound;

    for (var i = 0; i < plen; ++i) {
        if (direction === -1) {
            p[i].push(array[n-1]);
            bound = -1;
        } else {
            p[i].unshift(array[n-1]);
            bound = n;
        }

        permutations.push(p[i].slice());
        while (j+direction !== bound) {
            swap(p[i], j, j+direction);
            permutations.push(p[i].slice());
            j += direction;
        }
        direction *= -1;
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
    var permutations;
    if (array.length < 2) {
        yield array;
        permutations = [];
    } else {
        permutations = permute(array.slice(0, -1));
    }

    var n = array.length;
    var direction = -1;
    var j = n-1;
    var bound;

    for (var p of permutations) {
        if (direction === -1) {
            p.push(array[n-1]);
            bound = -1;
        } else {
            p.unshift(array[n-1]);
            bound = n;
        }

        yield p.slice();
        while (j+direction !== bound) {
            swap(p, j, j+direction);
            yield p.slice();
            j += direction;
        }
        direction *= -1;
    }
}

console.log('Recursive implementation using a generator function:');
var count = 0;
for (var p of permuteGenerator(args)) {
    count++;
    console.log(p);
}
console.log('Number of permutations:', count);
