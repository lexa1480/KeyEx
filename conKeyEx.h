#ifndef CONKEYEX_H
#define CONKEYEX_H

#include <nplug/IKey.h>
#include <nplug/clnMap.h>
#include <nplug/conBaseInterface.h>
#include <cstring>
#include <clnKeyEx.h>
#include <vector>

#include <map>

namespace nplug
{

class CConKeyEx : public CConBaseInterface < IKey >
{
protected:
    std::string				m_sName;
    std::string				m_sFullPath;
    std::vector<CClnKeyEx>  m_vClnSub;
    CConKeyEx*              m_pConKeyHead;
    std::map<std::string, std::string> m_map;

    CConKeyEx()
    {
        m_sName = "";
        m_sFullPath = "";
        m_pConKeyHead = this;
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
    //keys
    virtual NBool	OpenKey( LPCNStr pKeyName, NBool bCreate );
    virtual NVoid	CloseKey();
    virtual NBool	IsOpen(){return true;}
    virtual NBool	IsChanged(){return false;}
    virtual NBool	GetSubKeysCount( NDword& dwCount );
    virtual NBool	GetSubKeyByIdx( NDword dwIdx, IKey*& pObj );
    virtual NBool	GetSubKey( LPCNStr pSubKeyName, IKey*& pObj, NBool bCreate );
    virtual NBool	GetSubKeyName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize );
    virtual NBool	IsSubKey( LPCNStr pSubKeyName );
    virtual NBool	DeleteSubKey( LPCNStr pSubKeyName );
    virtual NBool	DeleteAllSubKeys();

    //valeus
    virtual NBool	GetValuesCount( NDword& dwCount );
    virtual NBool	GetValueName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize );
    virtual NBool	IsValue( LPCNStr pValueName );
    virtual NBool	DeleteValue( LPCNStr pValueName );
    virtual NBool	DeleteAllValues();
    virtual NBool	GetValue( LPCNStr pValueName, NChar* pBuf, NDword& dwBufSize );
    virtual NBool	SetValue( LPCNStr pValueName, LPCNStr pBuf, NDword dwBufSize );

    //attrinuts
    virtual NBool	GetAttrCount( NDword& dwCount );
    virtual NBool	GetAttrName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize );
    virtual NBool	IsAttr( LPCNStr pAttrName );
    virtual NBool	DeleteAttr( LPCNStr pAttrName );
    virtual NBool	DeleteAllAttrs();
    virtual NBool	GetAttr( LPCNStr pAttrName, NChar* pBuf, NDword& dwBufSize );
    virtual NBool	SetAttr( LPCNStr pAttrName, LPCNStr pBuf, NDword dwBufSize );

    ////// CConKeyEx	//////////

    void					SetName( const char* pPath, const char* pName );
    void					Delete();

    virtual NBool   GetInfo(nplug::IMap* pParams);

    static CConKeyEx* CreateObj(){return new(std::nothrow) CConKeyEx();}
};

// inlines CConKeyEx
/// IKey
//keys
inline NVoid* CConKeyEx::QueryInterface( LPCNStr pInterfaceID )
{
    CConKeyEx* pConKeyEx = NULL;
    if (pInterfaceID && (std::string(pInterfaceID) == std::string(c_szPluginIdKey)))
    {
        pConKeyEx = new(std::nothrow) CConKeyEx();
        pConKeyEx->m_sName = this->m_sName;
        pConKeyEx->m_sFullPath = this->m_sFullPath;
        pConKeyEx->m_vClnSub = this->m_vClnSub;
        pConKeyEx->m_pConKeyHead = this->m_pConKeyHead;
        pConKeyEx->m_map = this->m_map;
    }
    return pConKeyEx;
}

inline NBool CConKeyEx::OpenKey( LPCNStr pKeyName, NBool bCreate )
{
    if(this == m_pConKeyHead)
        return false;

    std::string sOpenPath;
    std::string sName = "";
    CConKeyEx* pParentThis = m_pConKeyHead;
    CConKeyEx* pParentNew = m_pConKeyHead;
    CConKeyEx* pNew = m_pConKeyHead;
    IKey* pIKeySwap;

    sOpenPath = this->m_sFullPath;
    int indx = sOpenPath.find("/");
    if(indx == 0)
    {
        sOpenPath = sOpenPath.substr(indx+1);
        indx = sOpenPath.find("/");
    }

    while(indx != -1)
    {
        sName = sOpenPath.substr(0, indx);
        sOpenPath = sOpenPath.substr(indx+1);
        indx = sOpenPath.find("/");
        pIKeySwap = NULL;
        pParentThis->GetSubKey(sName.c_str(), pIKeySwap, false);
        pParentThis = static_cast<CConKeyEx*>(pIKeySwap);
    }

    CConKeyEx* CloneKey = reinterpret_cast<CConKeyEx*>(this->QueryInterface(c_szPluginIdKey));
    std::vector<CClnKeyEx>::iterator it = pParentThis->m_vClnSub.begin();
    while(it != pParentThis->m_vClnSub.end())
    {
        if(it->GetIF() == this)
        {
            it->Attach(CloneKey);
            break;
        }
        it++;
    }

    sOpenPath = pKeyName;
    if(sOpenPath == "")
    {
        this->m_sName = m_pConKeyHead->m_sName;
        this->m_sFullPath = m_pConKeyHead->m_sFullPath;
        this->m_vClnSub = m_pConKeyHead->m_vClnSub;
        this->m_map = m_pConKeyHead->m_map;
    }
    else
    {
        int indx = sOpenPath.find("/");
        if(indx == 0)
        {
            sOpenPath = sOpenPath.substr(indx+1);
            indx = sOpenPath.find("/");
        }

        while(indx != -1)
        {
            sName = sOpenPath.substr(0, indx);
            sOpenPath = sOpenPath.substr(indx+1);
            indx = sOpenPath.find("/");
            pIKeySwap = NULL;
            if( !pParentNew->GetSubKey(sName.c_str(), pIKeySwap, bCreate) )
                return false;

            pParentNew = static_cast<CConKeyEx*>(pIKeySwap);
        }

        pIKeySwap = NULL;
        if( !pParentNew->GetSubKey(sOpenPath.c_str(), pIKeySwap, bCreate) )
            return false;

        pNew = static_cast<CConKeyEx*>(pIKeySwap);


        std::vector<CClnKeyEx>::iterator it = pParentNew->m_vClnSub.begin();
        while(it != pParentNew->m_vClnSub.end())
        {
            if(it->GetIF() == pNew)
            {
                this->m_sName = pNew->m_sName;
                this->m_sFullPath = pNew->m_sFullPath;
                this->m_vClnSub = pNew->m_vClnSub;
                this->m_map = pNew->m_map;

                pNew->m_sName = "";
                pNew->m_sFullPath = "";
                pNew->m_vClnSub.clear();
                pNew->m_pConKeyHead = NULL;
                pNew->m_map.clear();

                it->Attach(this);
                break;
            }
            it++;
        }
    }
    return true;
}

inline 	NVoid	CConKeyEx::CloseKey()
{
}

inline NBool	CConKeyEx::GetSubKeysCount( NDword& dwCount )
{
    dwCount = m_vClnSub.size();
    return true;
}

inline NBool	CConKeyEx::GetSubKeyByIdx( NDword dwIdx, IKey*& pObj )
{
    if(dwIdx < m_vClnSub.size())
    {
        pObj = m_vClnSub.at(dwIdx).GetIF();
        return true;
    }

    return false;
}

inline NBool	CConKeyEx::GetSubKey( LPCNStr pSubKeyName, IKey*& pObj, NBool bCreate )
{
    if( pSubKeyName == 0 || ( pSubKeyName[0] == '\x0' ) )
    {
        return false;
    }

    std::vector<CClnKeyEx>::const_iterator it = m_vClnSub.begin();
    while(it != m_vClnSub.end())
    {
        if(it->GetKeyName() == pSubKeyName)
        {
            pObj = it->GetIF();
            return true;
        }
        it++;
    }

    if(bCreate)
    {
        CConKeyEx* pConKeyEx = new(std::nothrow) CConKeyEx();
        pConKeyEx->SetName(m_sFullPath.c_str(), pSubKeyName);
        pConKeyEx->m_pConKeyHead = this->m_pConKeyHead;

        CClnKeyEx key;
        key.Attach( pConKeyEx );

        m_vClnSub.push_back(key);
        pObj = static_cast<IKey*>( pConKeyEx );
        return true;
    }

    return false;
}

inline NBool	CConKeyEx::GetSubKeyName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize )
{
    NBool bRes = false;

    if(dwIdx < m_vClnSub.size())
    {
        std::string sName = m_vClnSub.at(dwIdx).GetKeyName();
        unsigned uDataSize = (NDword)sName.length()+1;
            if( dwBufSize >= uDataSize )
            {
                strncpy( pBuf, sName.c_str(), uDataSize );
                dwBufSize = uDataSize;
                bRes = true;
            }
            else
            {
                dwBufSize = uDataSize;
            }
    }

    return bRes;
}

inline NBool	CConKeyEx::IsSubKey( LPCNStr pSubKeyName )
{
    std::vector<CClnKeyEx>::const_iterator it = m_vClnSub.begin();
    while(it != m_vClnSub.end())
    {
        if(it->GetKeyName() == pSubKeyName)
        {
            return true;
        }
        it++;
    }

    return false;
}

inline NBool	CConKeyEx::DeleteSubKey( LPCNStr pSubKeyName )
{
    std::vector<CClnKeyEx>::iterator it = m_vClnSub.begin();
    while(it != m_vClnSub.end())
    {
        if(it->GetKeyName() == pSubKeyName)
        {
            reinterpret_cast<CConKeyEx*>(it->GetIF())->Delete();
            m_vClnSub.erase(it);
            return true;
        }
        it++;
    }

    return false;
}

inline NBool	CConKeyEx::DeleteAllSubKeys()
{
    std::vector<CClnKeyEx>::iterator it = m_vClnSub.begin();
    while(it != m_vClnSub.end())
    {
        reinterpret_cast<CConKeyEx*>(it->GetIF())->Delete();
        m_vClnSub.erase(it);
    }

    return true;
}

//valeus
inline NBool	CConKeyEx::GetValuesCount( NDword& dwCount )
{
    dwCount = m_map.size();
    return true;
}

inline NBool	CConKeyEx::GetValueName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize )
{
    NBool bRes = false;

    if(dwIdx >= m_map.size())
        return false;

    std::map<std::string, std::string> :: iterator it = m_map.begin();
    for(int i = 0; i < (int)dwIdx; i++)
        it++;

    std::string strValueName = it->first;
    unsigned uDataSize = (NDword)strValueName.length()+1;
    if( dwBufSize >= uDataSize )
    {
        strncpy( pBuf, strValueName.c_str(), uDataSize );
        dwBufSize = uDataSize;
        bRes = true;
    }
    else
    {
        dwBufSize = uDataSize;
    }

    return bRes;
}

inline NBool	CConKeyEx::IsValue( LPCNStr pValueName )
{
    if( m_map.find(pValueName) == m_map.end() )
        return false;
    return true;
}

inline NBool	CConKeyEx::DeleteValue( LPCNStr pValueName )
{
    if( m_map.find(pValueName) == m_map.end() )
        return false;

    m_map.erase(pValueName);
    return true;
}

inline NBool	CConKeyEx::DeleteAllValues()
{
    m_map.clear();
    return true;
}

inline NBool	CConKeyEx::GetValue( LPCNStr pValueName, NChar* pBuf, NDword& dwBufSize )
{
    NBool bRes = false;

    if( m_map.find(pValueName) != m_map.end() )
    {
        std::string strData = m_map[pValueName];
        if( strData.empty() )
        {
            if( pBuf )
                pBuf[0] = 0;
        }
        else
        {
            ptrdiff_t pdtDataSize = strData.end() - strData.begin();
            if( (ptrdiff_t)dwBufSize >= pdtDataSize )
            {
                memcpy( pBuf, strData.c_str(), pdtDataSize );
                dwBufSize = (NDword)pdtDataSize;
                bRes = true;
            }
            else
            {
                dwBufSize = (NDword)pdtDataSize;
            }
        }
    }

    return bRes;
}

inline NBool    CConKeyEx::SetValue( LPCNStr pValueName, LPCNStr pBuf, NDword dwBufSize )
{
    std::string sResult(pBuf,dwBufSize);
    m_map[pValueName] = sResult;
    return true;
}

//attributes
inline NBool	CConKeyEx::GetAttrCount( NDword& /*dwCount*/ ){ return false; }
inline NBool	CConKeyEx::GetAttrName( NDword /*dwIdx*/, NChar* /*pBuf*/, NDword& /*dwBufSize*/ ){ return false; }
inline NBool	CConKeyEx::IsAttr( LPCNStr /*pAttrName*/ ){ return false; }
inline NBool	CConKeyEx::DeleteAttr( LPCNStr /*pAttrName*/ ){ return false; }
inline NBool	CConKeyEx::DeleteAllAttrs(){ return false; }
inline NBool	CConKeyEx::GetAttr( LPCNStr /*pAttrName*/, NChar* /*pBuf*/, NDword& /*dwBufSize*/ ){ return false; }
inline NBool	CConKeyEx::SetAttr( LPCNStr /*pAttrName*/, LPCNStr /*pBuf*/, NDword /*dwBufSize*/ ){ return false; }

/// CConKeyEx
inline void CConKeyEx::SetName( const char* pPath, const char* pName )
{
    if( pName == 0 || pName[0] == '\x0' )
    {
        m_sName = pPath;
        m_sFullPath = pPath;
    }
    else
    {
        m_sName = pName;
        m_sFullPath = pPath;
        m_sFullPath = m_sFullPath + "/" + m_sName;
    }
}


inline void CConKeyEx::Delete()
{
    std::vector<CClnKeyEx>::iterator it = m_vClnSub.begin();
    while(it != m_vClnSub.end())
    {
        reinterpret_cast<CConKeyEx*>(it->GetIF())->Delete();
        m_vClnSub.erase(it);
    }

    m_sName = "";
    m_sFullPath = "";
    m_pConKeyHead = NULL;
    m_map.clear();
}

/// INFO
inline NBool CConKeyEx::GetInfo( nplug::IMap* pParams )
{
    bool bResult = 0;

    CClnMap clnMap(pParams);

    std::string sCommand;
    if ( clnMap.GetParam(c_sCmdCommand, sCommand) !=0 )
    {
        if (sCommand == c_sCmdGetKeyName)
           bResult = clnMap.SetParam( c_sKeyName, m_sName );
        else if (sCommand == c_sCmdGetKeyPath)
        {
            bResult = clnMap.SetParam( c_sKeyPath, m_sFullPath );
        }
    }

    return bResult;
}































































}//namespace nplug

#endif // CONKEYEX_H


