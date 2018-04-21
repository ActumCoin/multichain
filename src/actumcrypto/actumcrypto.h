#ifndef ACTUMCRYPTO_H
#define ACTUMCRYPTO_H

#include "entities/asset.h"
#include "protocol/multichainscript.h"
#include "wallet/wallet.h"
#include "rpc/rpcwallet.h"
#include "script/standard.h"

void RewardMinedBlock(CWallet* pwallet, double amount);

#endif // ACTUMCRYPTO_H
