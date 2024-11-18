# equihash.node *modernized node.js native addon*

This package is a modern refactor of khovratovich/equihash, which has not seen updates since 2016. It borrows from digitalbazaar/equihash's native node.js addon, which has not been updated since 2017. This package reworks them to compile with the latest versions of node-gyp and g++.

## usage

```sh
npm install @tacticalchihuahua/equihash
```

```js
import { solve, verify } from '@tacticalchihuahua/equihash';
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

## license

 <p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/lilyannehall/equihash">@tacticalchihuahua/equihash</a> by <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://github.com/lilyannehall">Lily Anne Hall</a> is marked with <a href="https://creativecommons.org/publicdomain/zero/1.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">CC0 1.0<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/zero.svg?ref=chooser-v1" alt=""></a></p> 
