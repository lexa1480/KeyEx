#ifndef CONKEYEX_H
#define CONKEYEX_H

#include <IKeyEx.h>
#include <nplug/clnMap.h>
#include <nplug/conBaseInterface.h>
#include <cstddef>
#include <cstring>

#include <iostream>

namespace nplug
{

class CConKeyEx : public CConBaseInterface < IKeyEx >
{
protected:

    NBool					m_bOpen;
    std::string				m_strName;
    std::string				m_strFullPath;


    CConKeyEx()
    {
        m_bOpen = true;
    }

public:
    virtual ~CConKeyEx()
    {

    }


    ///////// IBaseInterface	//////////

    virtual NVoid*	QueryInterface( LPCNStr pInterfaceID );
    virtual NBool	Open( IMap* ){return false;}
    virtual NVoid	Close(){CloseKey();}
    const NChar*   GetPluginId(){return c_szPluginIdKey;}

    ////// IKey	//////////
    virtual NBool	OpenKey( LPCNStr pKeyName, NBool bCreate );
    virtual NVoid	CloseKey();
    virtual NBool	IsOpen(){return m_bOpen;}
    virtual NBool	IsChanged(){return false;}
    virtual NBool	GetSubKeysCount( NDword& dwCount );
    virtual NBool	GetSubKeyByIdx( NDword dwIdx, IKeyEx*& pObj );
    virtual NBool	GetSubKey( LPCNStr pSubKeyName, IKeyEx*& pObj, NBool bCreate );
    virtual NBool	GetSubKeyName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize );
    virtual NBool	IsSubKey( LPCNStr pSubKeyName );
    virtual NBool	DeleteSubKey( LPCNStr pSubKeyName );
    virtual NBool	DeleteAllSubKeys();

    ////// CConKeyEx	//////////

    void					SetName( const char* pPath, const char* pName );

    virtual NBool   GetInfo(nplug::IMap* pParams);

    static CConKeyEx* CreateObj(){return new(std::nothrow) CConKeyEx();}
};

// inlines CConKeyEx
/// Key
inline NVoid* CConKeyEx::QueryInterface( LPCNStr pInterfaceID )
{
    CConKeyEx* pConKeyEx = NULL;
    if (pInterfaceID && (std::string(pInterfaceID) == std::string(c_szPluginIdKey)))
    {


    }
    return pConKeyEx;
}

inline NBool CConKeyEx::OpenKey( LPCNStr pKeyName, NBool bCreate )
{

}

inline 	NVoid	CConKeyEx::CloseKey()
{

}

inline NBool	CConKeyEx::GetSubKeysCount( NDword& dwCount )
{

}

inline NBool	CConKeyEx::GetSubKeyByIdx( NDword dwIdx, IKeyEx*& pObj )
{

}

inline NBool	CConKeyEx::GetSubKey( LPCNStr pSubKeyName, IKeyEx*& pObj, NBool bCreate )
{

}

inline NBool	CConKeyEx::GetSubKeyName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize )
{

}

inline NBool	CConKeyEx::IsSubKey( LPCNStr pSubKeyName )
{

}

inline NBool	CConKeyEx::DeleteSubKey( LPCNStr pSubKeyName )
{

}

inline NBool	CConKeyEx::DeleteAllSubKeys()
{

}

/// CConKeyEx
inline void CConKeyEx::SetName( const char* pPath, const char* pName )
{
    if( pName == 0 || pName[0] == '\x0' )
    {
        m_strName = pPath;
        m_strFullPath = pPath;
    }
    else
    {
        m_strName = pName;
        m_strFullPath = pPath;
        m_strFullPath = m_strFullPath + "/" + m_strName;
    }
}

/// INFO
inline NBool CConKeyEx::GetInfo( nplug::IMap* pParams )
{
    bool bResult = 0;

    CClnMap clnMap(pParams);

    std::string strCommand;
    if ( clnMap.GetParam(c_sCmdCommand, strCommand) !=0 )
    {
        if (strCommand == c_sCmdGetKeyName)
           bResult = clnMap.SetParam( c_sKeyName, m_strName );
        else if (strCommand == c_sCmdGetKeyPath)
        {
            bResult = clnMap.SetParam( c_sKeyPath, m_strFullPath );
        }
    }

    return bResult;
}































































}//namespace nplug

#endif // CONKEYEX_H


