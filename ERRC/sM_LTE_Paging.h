//* ***************************************************************************
//    COPYRIGHT :   (c) 2011  Dennis M Senyonjo v0.3
// ***************************************************************************/

#ifndef _SM_LTE_PAGING_H_
#define _SM_LTE_PAGING_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_Paging**************************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_Paging, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  std::string xmlPaging;

  int CellNumber;
  int UEIdentity;
  int imsiLength;
  const char* imsi_digits;
  bool Failure;

  public:

  sM_LTE_Paging (
    std::string& _xmlPaging,
    int          _CellNumber,
    bool         _Failure = false
  )
  {
    xmlPaging              = _xmlPaging;
    CellNumber             = _CellNumber;
    UEIdentity             = 0;
    Failure                = _Failure;
  }

  // Destructor

  ~sM_LTE_Paging()
  {
  }

  void start(void);

};

#endif // _SM_LTE_PAGING_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/