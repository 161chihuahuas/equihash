import { solve, verify } from './index.cjs';
import { expect } from 'chai';
import { randomBytes } from 'crypto';


describe('@module equihash', function() {

  const TEST_SEED_HEX = '255ba09309998d317780a5b3b4ab8904373ddcdefb255f33649a4bc99044150f';
  const TEST_SEED_BUF = Buffer.from(TEST_SEED_HEX, 'hex');
  const TEST_PROOF_HEX = 'fb750000889a0000351e0000888a00001ad200002bdf0000f70600007e850000661a000058a700008e020000ce6d0000a43c000023c2000090b60000dad700009f310000d9a20000f7900000a9d1000067360000b3d0000092d9000010ec00004a47000052920000beaa0000d5ef0000644f00008dc900003c7900008e860000';
  const TEST_PROOF_BUF = Buffer.from(TEST_PROOF_HEX, 'hex');
  const TEST_NONCE = 6;

  it('should generate a solution from the input', function(done) {
    solve(TEST_SEED_BUF, 90, 5).then((solution) => {
      try {
        expect(solution.proof.toString('hex')).to.equal(TEST_PROOF_HEX);
        expect(solution.nonce).to.equal(TEST_NONCE);
      } catch (e) {
        done(e)
      }
      done();
    }, done);
  });

  it('should verify proof from the input', function(ok) {
    verify(TEST_SEED_BUF, TEST_PROOF_BUF, TEST_NONCE, 90, 5).then((valid) => {
      expect(valid).to.equal(true);
      ok();
    }, ok);
  });

  it('should not verify proof from the bad input', function(ok) {
    verify(TEST_SEED_BUF, randomBytes(32), 0, 90, 5).then((valid) => {
      expect(valid).to.equal(false);
      ok();
    }, (e) => {
        expect(e.message).to.equal('Invalid proof length');
        ok();
      });
  });

  it('should not verify proof from the bad input', function(ok) {
    verify(TEST_SEED_BUF, TEST_PROOF_BUF, 0, 90, 5).then((valid) => {
      expect(valid).to.equal(false);
      ok();
    }, (e) => {
        expect(e.message).to.equal('Invalid proof length');
        ok();
      });
  });
});
