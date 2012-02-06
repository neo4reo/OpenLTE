// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_ULNASMESSAGEHANDLER_H_
#define _SM_LTE_ULNASMESSAGEHANDLER_H_

/* INCLUDE FILES *************************************************************/

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_UlNasMsgHandler ****************************************************/

DERIVED_STATEMACHINE_DLL( sM_LTE_UlNasMsgHandler, sM_LteMlDefault, DLLLTECOMMON )
{
  protected:

  const char * MessageId;

  std::string xmlEmmMessage;
  std::string xmlEsmMessage;
  std::string xmlEmmEsmMessage;

  public:

  sM_LTE_UlNasMsgHandler (
    const char * _MessageId,
    std::string& _xmlEmmMessage,
    std::string& _xmlEsmMessage
  )
  {
    MessageId     = _MessageId;
    xmlEmmMessage = _xmlEmmMessage;
    xmlEsmMessage = _xmlEsmMessage;
  }

  sM_LTE_UlNasMsgHandler (
    const char * _MessageId,
    std::string& _xmlEmmEsmMessage
  )
  {
    MessageId        = _MessageId;
    xmlEmmEsmMessage = _xmlEmmEsmMessage;
  }

  // Destructor

  ~sM_LTE_UlNasMsgHandler()
  {
  }

  void start(void);

};

#endif // _SM_LTE_ULNASMESSAGEHANDLER_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/