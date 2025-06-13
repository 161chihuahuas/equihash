/*
 * Code by Dmitry Khovratovich, 2016
 * CC0 license
 * Refactored by Lily Anne Hall, 2024
 */

#ifndef __POW
#define __POW

#include <cstdint>
#include <cstdio>
#include <vector>

const int SEED_LENGTH = 4;    // Length of seed in dwords ;
const int NONCE_LENGTH = 24;  // Length of nonce in bytes;
const int MAX_NONCE = 0xFFFFF;
const int MAX_N = 32;  // Max length of n in bytes, should not exceed 32
const int LIST_LENGTH = 5;
const unsigned FORK_MULTIPLIER = 3;  // Maximum collision factor

/* The block used to initialize the PoW search
   @v actual values
*/
namespace _POW {

class Seed {
  std::vector<uint32_t> v;

 public:
  Seed() { v.resize(SEED_LENGTH, 0); }
  explicit Seed(uint32_t x) { v.resize(SEED_LENGTH, x); }
  explicit Seed(const unsigned* data, unsigned length) {
    unsigned copyLength = SEED_LENGTH;
    v.resize(SEED_LENGTH, 0);
    if (length <= SEED_LENGTH) {
      copyLength = length;
    }
    std::copy(data, data + copyLength, v.begin());
  }
  Seed(const Seed& r) { v = r.v; }
  Seed& operator=(const Seed& r) {
    v = r.v;
    return *this;
  }
  const uint32_t& operator[](unsigned i) const { return v[i]; }
};

/* Different nonces for PoW search
   @v actual values
   */
typedef uint32_t Nonce;
typedef uint32_t Input;

/*Actual proof of work
 */
struct Proof {
  const unsigned n;
  const unsigned k;
  const Seed seed;
  const Nonce nonce;
  const std::vector<Input> inputs;
  Proof(unsigned n_v, unsigned k_v, Seed I_v, Nonce V_v,
        std::vector<Input> inputs_v)
      : n(n_v), k(k_v), seed(I_v), nonce(V_v), inputs(inputs_v){};
  Proof() : n(0), k(1), seed(0), nonce(0), inputs(std::vector<Input>()){};

  bool Test();
};

class Tuple {
 public:
  std::vector<uint32_t> blocks;
  Input reference;
  Tuple(unsigned i) { blocks.resize(i); }
  Tuple& operator=(const Tuple& r) {
    blocks = r.blocks;
    reference = r.reference;
    return *this;
  }
};

class Fork {
 public:
  Input ref1, ref2;
  Fork(){};
  Fork(Input r1, Input r2) : ref1(r1), ref2(r2){};
};

/*Algorithm class for creating proof
Assumes that n/(k+1) <=32
*
*/
class Equihash {
  std::vector<std::vector<Tuple>> tupleList;
  std::vector<unsigned> filledList;
  std::vector<Proof> solutions;
  std::vector<std::vector<Fork>> forks;
  unsigned n;
  unsigned k;
  Seed seed;
  Nonce nonce;

 public:
  /*
  Initializes memory.
  */
  Equihash(unsigned n_in, unsigned k_in, Seed s) : n(n_in), k(k_in), seed(s){};
  ~Equihash(){};
  Proof FindProof();
  void FillMemory(uint32_t length);  // fill with hash
  void InitializeMemory();           // allocate memory
  void ResolveCollisions(bool store);
  std::vector<Input> ResolveTree(Fork fork);
  std::vector<Input> ResolveTreeByLevel(Fork fork, unsigned level);
  void PrintTuples(FILE* fp);
};
}  // namespace _POW

#endif  // define __POW
