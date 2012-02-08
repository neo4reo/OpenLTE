// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_OPENLOOPACTIVATION_H_
#define _SM_LTE_OPENLOOPACTIVATION_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_OpenLoopbackActivation *********************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_OpenLoopbackActivation, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlOpenUeTestLoop;
  std::string xmlOpenUeTestLoopComplete;
  int         UEid;

public:
	
	sM_LTE_OpenLoopbackActivation (
								   std::string& _xmlOpenUeTestLoop,
								   std::string& _xmlOpenUeTestLoopComplete
								   )
	{
		xmlOpenUeTestLoop         = _xmlOpenUeTestLoop;
		xmlOpenUeTestLoopComplete = _xmlOpenUeTestLoopComplete;
		UEid                      = 0;
	}
	
	// Destructor
	
	~sM_LTE_OpenLoopbackActivation()
	{
	}
	
	void start(void);
	
};

#endif // _SM_LTE_OPENLOOPACTIVATION_H_


/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/