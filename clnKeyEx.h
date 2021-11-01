#ifndef CLNKEYEX_H
#define CLNKEYEX_H

#include <IKeyEx.h>
#include <nplug/clnBaseInterface.h>
#include <nplug/clnMap.h>
#include <nplug/conMap.h>

#include <string.h>
#include <vector>

namespace nplug
{

///////////////////////////////////////////////////////////////////////////////
// CClnKey
class CClnKeyEx : public CClnBaseInterface < IKeyEx >
{
////////// Attributes:
public:
protected:

////////// Construction:
public:
    CClnKeyEx()
    {
        SetClientName( "clnKey" );
    }
    CClnKeyEx( IKeyEx* pInterface )
        :CClnBaseInterface < IKeyEx >(pInterface)
    {
        SetClientName( "clnKey" );
    }
    CClnKeyEx( const CClnKeyEx& clnFrom )
        :CClnBaseInterface < IKeyEx >(clnFrom.GetIF())
    {
        SetClientName( "clnKey" );
    }
    ~CClnKeyEx()
    {
    }

////////// Operations:
public:
    void		operator=( const CClnKeyEx& clnFrom )
    {
        Detach();
        ResetBase();
        Attach( clnFrom.m_pInterface );
    }
    //
    CClnKeyEx		Clone() const;
    void		Clear();

    /////////////////////
    // key
    bool		Open( const char* pKeyName, bool bCreate );
    void		Close();

    bool		IsOpen() const;
    bool		IsChanged() const;
    bool		IsSubKey( const char* pSubKeyName ) const;

    std::string GetKeyName() const;
    std::string GetKeyPath() const;
    CClnKeyEx     GetParent() const;
    unsigned	GetSubKeysCount() const;
    bool		GetSubKeyName( unsigned uIndex, std::string& str ) const;
    CClnKeyEx		GetSubKey( unsigned uIndex ) const;
    CClnKeyEx		GetSubKey( const char* pSubKeyName, bool bCreate ) const;

    bool		DeleteSubKey( const char* pSubKeyName );
    bool		DeleteAllSubKeys();

    /////////////////////
    // values
    bool		IsValue( const char* pValueName ) const;

    unsigned	GetValuesCount() const;
    bool		GetValueName(unsigned uIndex, std::string& str ) const;
    bool		GetValue( const char* pValueName, std::string& str ) const;
    template<typename T>
    bool		GetValue( const char* pValueName, T& tData ) const;
    bool        GetValue( LPCNStr pValueName, std::vector<std::string>& vData ) const;
    template<typename T>
    bool		GetValue( LPCNStr pValueName, std::vector<T>& vData ) const;
    bool		GetValue( const char* pValueName, char* pBuf, unsigned& dwBufSize ) const;

    bool		SetValue( const char* pValueName, const char* pBuf, unsigned dwBufSize = 0 );
    bool		SetValue( const char* pValueName, const std::string& str );
    template<typename T>
    bool		SetValue( const char* pValueName, const T& tData );
    template<typename T>
    bool		SetValue( LPCNStr pValueName, const std::vector<T>& vData );

    bool		DeleteValue( const char* pValueName );
    bool		DeleteAllValues();

    /////////////////////
    // attributes
    bool		IsAttr( const char* pAttrName ) const;

    unsigned	GetAttrsCount() const;
    bool		GetAttrName(unsigned uIndex, std::string& str ) const;
    bool		GetAttr( const char* pAttrName, std::string& str ) const;

    bool		SetAttr( const char* pAttrName, const char* pBuf, unsigned dwBufSize = 0 );

    bool		DeleteAttr( const char* pAttrName );
    bool		DeleteAllAttrs();
};

///////////////////////////////////////////////////////////////////////////////
// CClnKey inlines
inline CClnKeyEx	CClnKeyEx::Clone() const
{
    IKeyEx* pCloneKey = NULL;
    NPLUG_RUN_IF_METHOD( pCloneKey = reinterpret_cast<IKeyEx*>(m_pInterface->QueryInterface(c_szPluginIdKey)) );
    return pCloneKey;
}
inline void		CClnKeyEx::Clear()
{
}

inline bool		CClnKeyEx::Open( const char* pKeyName, bool bCreate )
{
    bool bRes = false;
    const std::string c_strPrevPath = GetKeyPath();
    std::cout << "6" << pKeyName << bCreate << std::endl;
    this->Close();
    std::cout << "7"  << std::endl;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->OpenKey( pKeyName, bCreate ) != 0 ) );
    std::cout << "777777" <<  std::endl;
    m_pInterface->CloseKey();
    if (!bRes) // restore key
        NPLUG_RUN_IF_METHOD( ( m_pInterface->OpenKey( c_strPrevPath.c_str(), false ) != 0 ) );
    return bRes;
}

inline void		CClnKeyEx::Close()
{
    NPLUG_RUN_IF_METHOD( m_pInterface->CloseKey() );
}

inline bool		CClnKeyEx::IsOpen() const
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->IsOpen() != 0 ) );
    return bRes;
}

inline bool		CClnKeyEx::IsChanged() const
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->IsChanged() != 0 ) );
    return bRes;
}

inline bool		CClnKeyEx::IsSubKey( const char* pSubKeyName ) const
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->IsSubKey( pSubKeyName ) != 0 ) );
    return bRes;
}

inline bool		CClnKeyEx::DeleteSubKey( const char* pSubKeyName )
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->DeleteSubKey( pSubKeyName ) != 0 ) );
    return bRes;
}

inline bool		CClnKeyEx::DeleteAllSubKeys()
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->DeleteAllSubKeys() != 0 ) );
    return bRes;
}

inline unsigned	CClnKeyEx::GetSubKeysCount() const
{
    NDword dwCount = 0;
    NPLUG_RUN_IF_METHOD( m_pInterface->GetSubKeysCount( dwCount ) );
    return dwCount;
}

inline bool		CClnKeyEx::GetSubKeyName( unsigned uIndex, std::string& str ) const
{
    bool bRes = false;
    NDword dwBufSize = 0;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetSubKeyName( uIndex, 0, dwBufSize ) != 0 ) );
    if( dwBufSize )
    {
        std::string strData;
        strData.resize( dwBufSize );
        NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetSubKeyName( uIndex, (NChar*)strData.data(), dwBufSize ) != 0 ) );
        if ( bRes )
            str.swap( strData );
    }
    return bRes;
}

inline CClnKeyEx		CClnKeyEx::GetSubKey( unsigned uIndex ) const
{
    CClnKeyEx key;
    if( IsOpen() )
    {
        IKeyEx* pIKeyEx = 0;
        NPLUG_RUN_IF_METHOD( m_pInterface->GetSubKeyByIdx( uIndex, pIKeyEx ) );
        if( pIKeyEx )
            key.Attach( pIKeyEx );
    }
    return key;
}

inline CClnKeyEx		CClnKeyEx::GetSubKey( const char* pSubKeyName, bool bCreate ) const
{
    CClnKeyEx key;
    if( IsOpen() )
    {
        IKeyEx* pIKeyEx = 0;
        NPLUG_RUN_IF_METHOD( m_pInterface->GetSubKey( pSubKeyName, pIKeyEx, bCreate ) );
        if( pIKeyEx )
            key.Attach( pIKeyEx );
    }
    return key;
}

inline CClnKeyEx CClnKeyEx::GetParent() const
{
    std::cout << "1" << std::endl;
    CClnKeyEx parentKey(this->Clone());
    this->m_pInterface->CloseKey();
    std::cout << "2 " << parentKey.GetKeyName() << " + " << this->GetKeyName() << std::endl;
    std::string strKeyPath = this->GetKeyPath();
    std::cout << "3" << strKeyPath << std::endl;
    strKeyPath = strKeyPath.substr(0, strKeyPath.rfind("/"));
    std::cout << "4" << strKeyPath << std::endl;
    parentKey.Open(strKeyPath.c_str(), false);
    std::cout << "5" << std::endl;
    return parentKey;
}
/*
inline unsigned	CClnKeyEx::GetValuesCount() const
{
    NDword dwCount = 0;
    NPLUG_RUN_IF_METHOD(  m_pInterface->GetValuesCount( dwCount ) );
    return dwCount;
}
inline bool		CClnKeyEx::GetValueName(unsigned uIndex, std::string& str ) const
{
    bool bRes = false;
    NDword dwBufSize = 0;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetValueName( uIndex, 0, dwBufSize ) != 0 ) );
    if( dwBufSize )
    {
        std::string strData;
        strData.resize( dwBufSize );
        NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetValueName( uIndex, (NChar*)strData.data(), dwBufSize ) != 0 ) );
        if ( bRes )
            str = strData.c_str(); // last zero-character removal
    }
    return bRes;
}
inline bool		CClnKeyEx::IsValue( const char* pValueName ) const
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->IsValue( pValueName ) != 0 ) );
    return bRes;
}
inline bool		CClnKeyEx::DeleteValue( const char* pValueName )
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->DeleteValue( pValueName ) != 0 ) );
    return bRes;
}
inline bool		CClnKeyEx::DeleteAllValues()
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->DeleteAllValues() != 0 ) );
    return bRes;
}
inline bool		CClnKeyEx::GetValue( const char* pValueName, char* pBuf, unsigned& dwBufSize ) const
{
    bool bRes = false;
    unsigned dwBufSizeIn = dwBufSize;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetValue( pValueName, pBuf, dwBufSize ) != 0 ) );
    if( bRes && dwBufSizeIn > dwBufSize )
    {
        pBuf[dwBufSize] = 0;
    }
    return bRes;
}
inline bool		CClnKeyEx::GetValue( const char* pValueName, std::string& str ) const
{
    bool bRes = false;
    NDword dwBufSize = 0;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetValue( pValueName, 0, dwBufSize ) != 0 ) );
    if( bRes || ( dwBufSize > 0 ) )
    {
        if( dwBufSize )
        {
            std::string strData;
            strData.resize( dwBufSize );
            NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetValue( pValueName, (NChar*)strData.data(), dwBufSize ) != 0 ) );
            if ( bRes )
            {
                if( dwBufSize == 1 && ( strData[0] == '\x0' ) )
                    str.clear();
                else
                    str.swap( strData );
            }
        }
        else
        {
            str.clear();
        }
    }
    return bRes;
}
*/
template<typename T>
inline bool		CClnKeyEx::GetValue( const char* pValueName, T& tData ) const
{
    std::string str;
    bool bRes = GetValue( pValueName, str );
    if( bRes )
    {
        std::stringstream ss( str );
        ss >> tData;
    }
    return bRes;
}
template<typename T>
inline bool	CClnKeyEx::GetValue( LPCNStr pValueName, std::vector<T>& vData ) const
{
    std::string strVal;
    bool bRes = GetValue( pValueName, strVal );
    if( bRes )
    {
        vData.clear();
        LPCNStr pPtr = strVal.data();
        do
        {
            std::string str = pPtr;
            if( !str.empty() )
            {
                std::stringstream ss( str );
                T tData;
                ss >> std::noskipws >> tData;
                vData.push_back( tData );
            }
            else
                break;
            pPtr += ( str.length()+1 ) * sizeof( NChar );
        }
        while( pPtr - strVal.data() <= NInt( strVal.size() ) );
    }
    return bRes;
}
inline bool	CClnKeyEx::GetValue( LPCNStr pValueName, std::vector<std::string>& vData ) const
{
    std::string strVal;
    bool bRes = GetValue( pValueName, strVal );
    if( bRes )
    {
        vData.clear();
        LPCNStr pPtr = strVal.data();
        do
        {
            std::string str = pPtr;
            if( !str.empty() )
            {
                vData.push_back( str );
            }
            else
                break;
            pPtr += ( str.length()+1 ) * sizeof( NChar );
        }
        while( pPtr - strVal.data() <= NInt( strVal.size() ) );
    }
    return bRes;
}
/*
inline bool		CClnKeyEx::SetValue( const char* pValueName, const char* pBuf, unsigned dwBufSize )
{
    if( !pBuf )
        return false;

    if( dwBufSize == 0 )
    {
#ifdef WIN32
        dwBufSize = lstrlen( pBuf )+1;
#else
        dwBufSize = strlen( pBuf )+1;
#endif
    }
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->SetValue( pValueName, pBuf, dwBufSize ) != 0 ) );
    return bRes;
}
*/
inline bool		CClnKeyEx::SetValue( const char* pValueName, const std::string& str )
{
    ptrdiff_t pdtBufSize = ( str.end() > str.begin() ) ? ( str.end() - str.begin() ) : 0;
    return SetValue( pValueName, str.c_str(), (unsigned)pdtBufSize );
}

template<typename T>
inline bool		CClnKeyEx::SetValue( const char* pValueName, const T& tData )
{
    std::stringstream ss;
    ss << tData;
    return SetValue( pValueName, ss.str().c_str() );
}
template<typename T>
inline bool	CClnKeyEx::SetValue( LPCNStr pValueName, const std::vector<T>& vData )
{
    std::stringstream ss;
    for( int i=0; i< int( vData.size() ); i++ )
        ss << vData[i] << '\x0';
    ss << '\x0';
    return SetValue( pValueName, ss.str() );
}
/*
inline unsigned	CClnKeyEx::GetAttrsCount() const
{
    NDword      dwCount = 0;
    NPLUG_RUN_IF_METHOD( m_pInterface->GetAttrCount( dwCount ) );
    return dwCount;
}
inline bool		CClnKeyEx::GetAttrName(unsigned uIndex, std::string& str ) const
{
    bool    bRes      = false;
    NDword  dwBufSize = 0;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetAttrName(uIndex, 0, dwBufSize ) != 0 ) );
    if (dwBufSize)
    {
        std::string strData;
        strData.resize( dwBufSize );
        NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetAttrName( uIndex, (NChar*)strData.data(), dwBufSize ) != 0 ) );
        if ( bRes )
            str.swap( strData );
    }
    return bRes;
}
inline bool		CClnKeyEx::IsAttr( const char* pAttrName ) const
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->IsAttr(pAttrName) != 0 ) );
    return bRes;
}
inline bool		CClnKeyEx::DeleteAttr( const char* pAttrName )
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->DeleteAttr(pAttrName) != 0 ) );
    return bRes;
}
inline bool		CClnKeyEx::DeleteAllAttrs()
{
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->DeleteAllAttrs() != 0 ) );
    return bRes;
}
inline bool		CClnKeyEx::GetAttr( const char* pAttrName, std::string& str ) const
{
    bool    bRes      = false;
    NDword  dwBufSize = 0;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetAttr(pAttrName, 0, dwBufSize ) != 0 ) );
    if (dwBufSize)
    {
        std::string strData;
        strData.resize( dwBufSize );
        NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->GetAttr( pAttrName, (NChar*)strData.data(), dwBufSize ) != 0 ) );
        if ( bRes )
            str.swap(strData);
    }
    return bRes;
}
inline bool		CClnKeyEx::SetAttr( const char* pAttrName, const char* pBuf, unsigned dwBufSize/* = 0 *//*)// Последний /* Fake
{
    if( !pBuf )
        return false;

    if( dwBufSize == 0 )
    {
#ifdef WIN32
        dwBufSize = lstrlen( pBuf )+1;
#else
        dwBufSize = strlen( pBuf )+1;
#endif
    }
    bool bRes = false;
    NPLUG_RUN_IF_METHOD( bRes = ( m_pInterface->SetAttr( pAttrName, pBuf, dwBufSize ) != 0 ) );
    return bRes;
}
*/

inline std::string CClnKeyEx::GetKeyName() const
{
    CClnMap clnMap( CConMap::CreateObj() );
    clnMap.Set( c_sCmdCommand, c_sCmdGetKeyName);

    std::string strKeyName;
    if( GetInfo(clnMap) )
    {
        clnMap.GetParam(c_sKeyName, strKeyName);
    }

    return strKeyName;
}
inline std::string CClnKeyEx::GetKeyPath() const
{
    std::string strKeyPath;

    CClnMap clnMap( CConMap::CreateObj() );
    clnMap.Set( c_sCmdCommand,   c_sCmdGetKeyPath );
    if( GetInfo(clnMap) )
    {
        clnMap.GetParam(c_sKeyPath, strKeyPath);
    }

    return strKeyPath;
}


}// namespace nplug

#endif // CLNKEYEX_H
