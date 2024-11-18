'use strict';

const addon = require('bindings')('equihash');

function solve(input, n = 90, k = 5) {
  return new Promise((resolve, reject) => {
    addon.solve({
      n,
      k,
      seed: input
    }, (err, solution, nonce, n, k) => {
      if (err) {
        return reject(err);
      }
      resolve({ proof: solution, nonce, n, k });
    });
  });
};

function verify(input, proof, nonce = 1, n = 90, k = 5) {
  if (proof.length < 128) {
    return Promise.reject(new Error('Invalid proof length'));
  }

  const valid = addon.verify({ 
    n,
    k,
    nonce,
    seed: input,
    value: proof
  });

  return Promise.resolve(valid);
};

module.exports = { solve, verify };
