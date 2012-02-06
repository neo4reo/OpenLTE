// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_CELL_BARRING_H_
#define _SM_LTE_CELL_BARRING_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_CellBarred**********************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_CellBarringInitializedByUE, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  int         CellHandle;
  int         WaitTimer;

  public:

  sM_LTE_CellBarringInitializedByUE(
    int          _CellHandle,
    int          _WaitTimer = 300000
  )
  {
    CellHandle              = _CellHandle;
    WaitTimer               = _WaitTimer;
  }

  // Destructor

  ~sM_LTE_CellBarringInitializedByUE()
  {
  }

  void start(void);
};

#endif // _SM_LTE_CELL_BARRING_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/