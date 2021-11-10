#ifndef IKEYEX_H
#define IKEYEX_H

#include <nplug/IBaseInterface.h>
#include <NitaDataTypes.h>
#include <iostream>

namespace nplug
{

const char c_szPluginIdKey[] = "Key";

struct IKeyEx : public IBaseInterface
{
    // keys
    virtual NBool	OpenKey( LPCNStr pKeyName, NBool bCreate ) = 0;
    virtual NVoid	CloseKey() = 0;
    virtual NBool	IsOpen() = 0;
    virtual NBool	IsChanged() = 0;
    virtual NBool	GetSubKeysCount( NDword& dwCount ) = 0;
    virtual NBool	GetSubKeyByIdx( NDword dwNum, IKeyEx*& pObj ) = 0;
    virtual NBool	GetSubKey( LPCNStr pSubKeyName, IKeyEx*& pObj, NBool bCreate ) = 0;
    virtual NBool	GetSubKeyName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize ) = 0;
    virtual NBool	IsSubKey( LPCNStr pSubKeyName ) = 0;
    virtual NBool	DeleteSubKey( LPCNStr pSubKeyName ) = 0;
    virtual NBool	DeleteAllSubKeys() = 0;
    // values
    virtual NBool	GetValuesCount( NDword& dwCount ) = 0;
    virtual NBool	GetValueName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize ) = 0;
    virtual NBool	IsValue( LPCNStr pValueName ) = 0;
    virtual NBool	DeleteValue( LPCNStr pValueName ) = 0;
    virtual NBool	DeleteAllValues() = 0;
    virtual NBool	GetValue( LPCNStr pValueName, NChar* pBuf, NDword& dwBufSize ) = 0;
    virtual NBool	SetValue( LPCNStr pValueName, LPCNStr pBuf, NDword dwBufSize ) = 0;
    // attributes
//    virtual NBool	GetAttrCount( NDword& dwCount ) = 0;
//    virtual NBool	GetAttrName( NDword dwIdx, NChar* pBuf, NDword& dwBufSize ) = 0;
//    virtual NBool	IsAttr( LPCNStr pAttrName ) = 0;
//    virtual NBool	DeleteAttr( LPCNStr pAttrName ) = 0;
//    virtual NBool	DeleteAllAttrs() = 0;
//    virtual NBool	GetAttr( LPCNStr pAttrName, NChar* pBuf, NDword& dwBufSize ) = 0;
//    virtual NBool	SetAttr( LPCNStr pAttrName, LPCNStr pBuf, NDword dwBufSize ) = 0;
};

const char c_sCmdCommand[] =	"Command";
const char c_sCmdGetKeyName[] =	"GetKeyName";
const char c_sKeyName[] =		"KeyName";
const char c_sCmdGetKeyPath[] =	"GetKeyPath";
const char c_sKeyPath[] =		"KeyPath";

}//namespace nplug

#endif // IKEYEX_H
