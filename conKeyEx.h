#ifndef CONKEYEX_H
#define CONKEYEX_H

#include <IKeyEx.h>
#include <nplug/conBaseInterface.h>
#include <cstddef>
#include <nplug/clnMap.h>
#include <cstring>

#include <iostream>
#include "Node.h"

namespace nplug
{

class CConKeyEx : public CConBaseInterface < IKeyEx >
{
protected:
    mutable boost::recursive_mutex 	m_lock;
    NBool					m_bOpen;
    std::string				m_strName;
    std::string				m_strFullPath;
    Node*    m_node;
    NBool   m_bHead;
    Node*    m_nHead;

    CConKeyEx()
    {
        m_bHead = false;
        m_bOpen = true;
    }

    CConKeyEx(bool bHead)
    {
        m_node = new Node();
        m_bHead = bHead;
        m_nHead = m_node;
        m_bOpen = true;
    }

public:
    virtual ~CConKeyEx()
    {
        if(m_bHead)
            delete m_nHead;
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
    void                    SetHead(Node* head);

    virtual NBool   GetInfo(nplug::IMap* pParams);

    static CConKeyEx* CreateObj(){return new(std::nothrow) CConKeyEx(true);}
};

// inlines CConKeyEx
/// Key
inline NVoid* CConKeyEx::QueryInterface( LPCNStr pInterfaceID )
{
    CConKeyEx* pConKeyEx = NULL;
    if (pInterfaceID && (std::string(pInterfaceID) == std::string(c_szPluginIdKey)))
    {
        pConKeyEx = new(std::nothrow) CConKeyEx();
        pConKeyEx->m_strName = this->m_strName;
        pConKeyEx->m_strFullPath = this->m_strFullPath;
        pConKeyEx->m_node = this->m_node;
        pConKeyEx->SetHead(this->m_nHead);
    }
    return pConKeyEx;
}

inline NBool CConKeyEx::OpenKey( LPCNStr pKeyName, NBool bCreate )
{
    boost::recursive_mutex::scoped_lock locker( m_lock );

    std::string strOpenPath(pKeyName);
    std::string strName = "";
    Node* hNode = m_nHead;

    int i = strOpenPath.find("/");
    if(i == 0)
    {
        strOpenPath = strOpenPath.substr(i+1);
        i = strOpenPath.find("/");
    }

    while(i != -1)
    {
        strName = strOpenPath.substr(0, i);
        strOpenPath = strOpenPath.substr(i+1);
        i = strOpenPath.find("/");

        Node* nNode = hNode->getSubNode(strName.c_str());
        if(nNode == NULL && bCreate)
        {
            nNode = new Node(hNode, strName, (std::string)hNode->getPath() + "/" + strName);
        }
        hNode = nNode;
    }

    Node* nNode = hNode->getSubNode(strOpenPath.c_str());
    if(nNode == NULL && bCreate)
    {
        nNode = new Node(hNode, strOpenPath, (std::string)hNode->getPath() + "/" + strOpenPath);
    }

    if(nNode != NULL){
       m_node = nNode;
       m_strName = nNode->getName();
       m_strFullPath = nNode->getPath();
       return true;
    }

    return false;
}

inline 	NVoid	CConKeyEx::CloseKey()
{

}

inline NBool	CConKeyEx::GetSubKeysCount( NDword& dwCount )
{
    m_node->getSubNodeCount(dwCount);
    return IsOpen();
}

inline NBool	CConKeyEx::GetSubKeyByIdx( NDword dwIdx, IKeyEx*& pObj )
{
    boost::recursive_mutex::scoped_lock locker( m_lock );
    NDword dwCount;
    m_node->getSubNodeCount(dwCount);
    if(dwIdx >= dwCount)
    {
        return false;
    }
    CConKeyEx* pConKeyEx = new(std::nothrow) CConKeyEx();
    Node* noded = m_node->getSubNodeByIdx(dwIdx);
    pConKeyEx->SetName( m_strFullPath.c_str(), noded->getName() );
    pConKeyEx->m_node = noded;
    pConKeyEx->SetHead(m_nHead);
    pObj = static_cast<IKeyEx*>( pConKeyEx );
    return true;
}

inline NBool	CConKeyEx::GetSubKey( LPCNStr pSubKeyName, IKeyEx*& pObj, NBool bCreate )
{
    boost::recursive_mutex::scoped_lock locker( m_lock );
    if( !IsOpen() )
    {
        return false;
    }

    if( pSubKeyName == 0 || ( pSubKeyName[0] == '\x0' ) )
    {
        return false;
    }

    Node* noded = m_node->getSubNode(pSubKeyName);
    if(noded != NULL || bCreate)
    {
        CConKeyEx* pConKeyEx = new(std::nothrow) CConKeyEx();
        if(noded == NULL)
        {
            noded = new Node(m_node, pSubKeyName, m_strFullPath + "/" + pSubKeyName);
        }
        pConKeyEx->SetName( m_strFullPath.c_str(), noded->getName() );
        pConKeyEx->m_node = noded;
        pConKeyEx->SetHead(m_nHead);
        pObj = static_cast<IKeyEx*>( pConKeyEx );
        return true;
    }
    return false;
}

inline NBool	CConKeyEx::GetSubKeyName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize )
{
    NBool bRes = false;

    boost::recursive_mutex::scoped_lock locker( m_lock );
    NDword dwCount;
    m_node->getSubNodeCount(dwCount);
    if(dwIdx < dwCount)
    {
        Node* node = m_node->getSubNodeByIdx(dwIdx);
        std::string str = node->getName();
        unsigned uDataSize = (NDword)str.length()+1;
        if( dwBufSize >= uDataSize )
        {
            strncpy( pBuf, str.c_str(), uDataSize );
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
    return (m_node->getSubNode(pSubKeyName) != NULL);
}

inline NBool	CConKeyEx::DeleteSubKey( LPCNStr pSubKeyName )
{
    return m_node->deleteSubNode( pSubKeyName );
}

inline NBool	CConKeyEx::DeleteAllSubKeys()
{
    return m_node->deleteAllSubNode();
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

inline void       CConKeyEx::SetHead(Node* head)
{
    m_nHead = head;
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


