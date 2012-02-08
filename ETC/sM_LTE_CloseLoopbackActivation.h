// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_CLOSELOOPACTIVATION_H_
#define _SM_LTE_CLOSELOOPACTIVATION_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_CloseLoopbackActivation ********************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_CloseLoopbackActivation, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlCloseUeTestLoop;
  std::string xmlCloseUeTestLoopComplete;
  int         UEIdentity;

  public:

  sM_LTE_CloseLoopbackActivation (
    std::string& _xmlCloseUeTestLoop,
    std::string& _xmlCloseUeTestLoopComplete
  )
  {
    xmlCloseUeTestLoop         = _xmlCloseUeTestLoop;
    xmlCloseUeTestLoopComplete = _xmlCloseUeTestLoopComplete;
    UEIdentity                 = 0;
  }

  // Destructor

  ~sM_LTE_CloseLoopbackActivation()
  {
  }

  void start(void);

};

#endif // _SM_LTE_CLOSELOOPACTIVATION_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/