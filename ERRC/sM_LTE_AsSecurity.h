// ****************************************************************************
//    COPYRIGHT :   (c) 2012  Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_ASSECURITY_H_
#define _SM_LTE_ASSECURITY_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_AsSecurity *********************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_AsSecurity, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  int         cipheringAlgorithm;
  int         integrityProtAlgorithm;
  std::string xmlSecurityModeCommand;
  std::string xmlSecurityModeComplete;
  int         UEid;

  public:

  sM_LTE_AsSecurity (
    std::string& _xmlSecurityModeCommand,
    int          _cipheringAlgorithm,
    int          _integrityProtAlgorithm,
    std::string& _xmlSecurityModeComplete
  )
  {
    xmlSecurityModeCommand  = _xmlSecurityModeCommand;
    cipheringAlgorithm      = _cipheringAlgorithm;
    integrityProtAlgorithm  = _integrityProtAlgorithm;
    xmlSecurityModeComplete = _xmlSecurityModeComplete;
    UEid                    = 0;
  }

  // Destructor

  ~sM_LTE_AsSecurity()
  {
  }

  void start(void);

};

#endif // _SM_LTE_ASSECURITY_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/