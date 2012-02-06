// ****************************************************************************
//    COPYRIGHT :   (c) 2011 Dennis M Senyonjo
// ****************************************************************************

#ifndef _SM_LTE_DLNASMESSAGEHANDLER_H_
#define _SM_LTE_DLNASMESSAGEHANDLER_H_

/* INCLUDE FILES *************************************************************/
#include <stdarg.h>

#include "lte_common.h"

/* CLASS DECLARATION *********************************************************/

/* sM_LTE_DlNasMsgHandler ***********************************************************/
// use MDDB::Eps::Nas:: instead of above enumerator !!
enum DlMessageId_e
{
    ESM_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST,
    EMM_STATUS,
    ESM_PDN_CONNECTIVITY_REJECT,
    ESM_PDN_DISCONNECT_REJECT
};


DERIVED_STATEMACHINE_DLL( sM_LTE_DlNasMsgHandler, sM_LteMlDefault, DLLLTECOMMON )
{
    protected:
 
        DlMessageId_e   DlMessageId;
        std::string     xmlMessage;
        va_list         m_argslist;

    public:

        sM_LTE_DlNasMsgHandler (
            DlMessageId_e   _DlMessageId,
            std::string     _xmlMessage
        )
        {
            DlMessageId     = _DlMessageId;
            xmlMessage      = _xmlMessage;
            m_argslist = 0;
        }
        
        // Constructer with va_list
        sM_LTE_DlNasMsgHandler (
            DlMessageId_e   _DlMessageId,
            std::string     _xmlMessage,
            va_list         _args
        )
        {
            DlMessageId     = _DlMessageId;
            xmlMessage      = _xmlMessage;
            m_argslist          = _args;
        }
        // Constructer with variable arguments
        sM_LTE_DlNasMsgHandler (
            DlMessageId_e   _DlMessageId,
            std::string     _xmlMessage,
            void            *_args, ...
        )
        {
            DlMessageId     = _DlMessageId;
            xmlMessage      = _xmlMessage;
            
            va_start(m_argslist, _args);
        }

        // Destructor

        ~sM_LTE_DlNasMsgHandler()
        {
            if( m_argslist != 0) va_end(m_argslist);
        }

        void start(void);

};

#endif // _SM_LTE_DLNASMESSAGEHANDLER_H_

/******************************************************************************/
/* END OF FILE                                                                */
/******************************************************************************/