const { expect } = require('chai');
const { spawn } = require('child_process');
const { join } = require('path');

describe('actumcrypto-util', function() {
	it('should be able to create a blockchain', function(done) {
		spawn(join(__dirname, '../actumcrypto/actumcrypto-util'), ['create', 'test-blockchain'], {stdio: ['pipe', 'pipe', 'inherit']}).on('close', function(code) {
			expect(code).to.equal(0);
			done();
		});
	});
});
describe('actumcryptod', function() {
	it('should be able to run without writing to stderr', function(done) {
		this.timeout(2000);
		let callbackUsed = false;
		let multichaind = spawn(join(__dirname, '../actumcrypto/actumcryptod'), ['test-blockchain']);
		multichaind.stderr.on('data', function(data) {
			if(!callbackUsed) {
				done(data);
				callbackUsed = true;
			}
		});
		multichaind.on('close', (code) => {
			if(!callbackUsed) {
				done(new Error(`Multichaind exited with code ${code}`));
				callbackUsed = true;
			}
		});
		setTimeout(done, 1500);
	});
});
