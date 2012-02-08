// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_TESTMODEACTIVATION_H_
#define _SM_LTE_TESTMODEACTIVATION_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_TestModeActivation *************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_TestModeActivation, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlActivateTestMode;
  std::string xmlActivateTestModeComplete;
  int         UEid;

  public:

  sM_LTE_TestModeActivation (
    std::string& _xmlActivateTestMode,
    std::string& _xmlActivateTestModeComplete
  )
  {
    xmlActivateTestMode         = _xmlActivateTestMode;
    xmlActivateTestModeComplete = _xmlActivateTestModeComplete;
    UEid                        = 0;
  }

  // Destructor

  ~sM_LTE_TestModeActivation()
  {
  }

  void start(void);

};

#endif // _SM_LTE_TESTMODEACTIVATION_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/