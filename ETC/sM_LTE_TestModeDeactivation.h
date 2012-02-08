// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_TESTMODEDEACTIVATION_H_
#define _SM_LTE_TESTMODEDEACTIVATION_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_TestModeDeactivation ***********************************************/

DERIVED_STATE_DLL( sM_LTE_TestModeDeactivation, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlDeactivateTestMode;
  std::string xmlDeactivateTestModeComplete;
  int         UEid;

  public:

  sM_LTE_TestModeDeactivation (
    std::string& _xmlDeactivateTestMode,
    std::string& _xmlDeactivateTestModeComplete
  )
  {
    xmlDeactivateTestMode         = _xmlDeactivateTestMode;
    xmlDeactivateTestModeComplete = _xmlDeactivateTestModeComplete;
    UEid                          = 0;
  }

  // Destructor

  ~sM_LTE_TestModeDeactivation()
  {
  }

  void start(void);

};

#endif // _SM_LTE_TESTMODEDEACTIVATION_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/