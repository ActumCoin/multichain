#include "actumcrypto/actumcrypto.h"

#include <boost/foreach.hpp>
#include <exception>
#include "core/main.h"

using namespace std;

void RewardMinedBlock(CWallet* pwallet, double amount) {
  set<boost::variant<CNoDestination, CKeyID, CScriptID> >::iterator it = pwallet->GetAccountAddresses("").begin();

  CBitcoinAddress address(*it);
  if (!address.IsValid()) return;

  cout << "address: " << address.ToString() << " amount: " << amount << "\n";

  CAmount nAmount = 0;

  mc_Script *lpScript=mc_gState->m_TmpBuffers->m_RpcScript3;
  lpScript->Clear();
  unsigned char buf[MC_AST_ASSET_FULLREF_BUF_SIZE];
  memset(buf,0,MC_AST_ASSET_FULLREF_BUF_SIZE);
  int multiple=1;
  mc_EntityDetails entity;

  try {
    ParseEntityIdentifier("acm", &entity, MC_ENT_TYPE_ASSET);
    memcpy(buf,entity.GetFullRef(),MC_AST_ASSET_FULLREF_SIZE);
  } catch(...) {
    return;
  }



  multiple=entity.GetAssetMultiple();

  Value raw_qty = 5;

  int64_t quantity = (int64_t)(raw_qty.get_real() * multiple + 0.499999);
  if(quantity<0)
  {
    return;
  }

  mc_SetABQuantity(buf,quantity);

  mc_Buffer *lpBuffer=mc_gState->m_TmpBuffers->m_RpcABNoMapBuffer2;
  lpBuffer->Clear();

  lpBuffer->Add(buf);

  lpScript->SetAssetQuantities(lpBuffer,MC_SCR_ASSET_SCRIPT_TYPE_FOLLOWON);

  // Wallet comments
  CWalletTx wtx;

  if(!AddressCanReceive(address.Get()))
  {
    return;
  }


  mc_Script *lpDetailsScript=mc_gState->m_TmpBuffers->m_RpcScript1;
  lpDetailsScript->Clear();

  mc_Script *lpDetails=mc_gState->m_TmpBuffers->m_RpcScript2;
  lpDetails->Clear();

  lpDetails->AddElement();

  vector<CTxDestination> addresses;
  vector<CTxDestination> fromaddresses = ParseAddresses("*",false,false);
  CScript scriptOpReturn=CScript();
  int errorCode=RPC_INVALID_PARAMETER;
  string strError;
  lpDetailsScript->Clear();

  int err;
  size_t bytes;
  const unsigned char *script;
  size_t elem_size;
  const unsigned char *elem;

  script=lpDetails->GetData(0,&bytes);

  addresses.push_back(address.Get());

  if(mc_gState->m_Assets->FindEntityByFullRef(&entity,buf))
  {
    if(entity.AllowedFollowOns())
    {
      if(fromaddresses.size() == 1)
      {
        CKeyID *lpKeyID=boost::get<CKeyID> (&fromaddresses[0]);
        if(lpKeyID == NULL)
        {
          return;
        }
      }
      else
      {
        bool issuer_found=false;
        BOOST_FOREACH(const PAIRTYPE(CBitcoinAddress, CAddressBookData)& item, pwalletMain->mapAddressBook)
        {
          const CBitcoinAddress& address = item.first;
          CKeyID keyID;

          if(address.GetKeyID(keyID))
          {
            if(mc_gState->m_Permissions->CanIssue(entity.GetTxID(),(unsigned char*)(&keyID)))
            {
              issuer_found=true;
            }
          }
        }
        if(!issuer_found)
        {
          return;
        }
      }
    }
  }


  EnsureWalletIsUnlocked();
  {
     LOCK (pwalletMain->cs_wallet_send);

     SendMoneyToSeveralAddresses(addresses, 0, wtx, lpScript, scriptOpReturn, fromaddresses);
  }

}
