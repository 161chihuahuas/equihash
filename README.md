# ⛏️ equihash ~ *native equihash solver and verifier*

This package is a modern refactor of khovratovich/equihash, which has not seen updates since 2016. It borrows from digitalbazaar/equihash's native node.js addon, which has not been updated since 2017. This package reworks them to compile with the latest versions of node-gyp and g++.

## usage

```sh
npm install @yipsec/equihash
```

```js
import { solve, verify } from '@yipsec/equihash';
import { randomBytes } from 'crypto';

const N = 90;
const K = 5;

// example
async function demo() {
    const seed = randomBytes(32);
    const solution = await solve(seed, N, K);
    const valid = await verify(solution.proof, solution.nonce, N, K);

    console.log(solution); // {proof,nonce,n,k}
    console.log(valid); // true/false
}
```

## recommended parameters ( N, K )

### For cryptocurrencies

* (100/110/120, 4)
* (108/114/120/126, 5)

### For client puzzles

* (60/70/80/90,4)
* (90/96/102,5)

## links

* [Original C++ implementation of equihash proof-of-work by khovratovich](https://github.com/khovratovich/equihash)
* [Original Native Node.js module by digitalbazaar](https://github.com/digitalbazaar/equihash)

## copying

anti-copyright 2025, yipsec  
licensed under the lesser gnu general public license 3.0
