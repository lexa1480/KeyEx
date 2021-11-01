#include <QCoreApplication>
#include "clnKeyEx.h"
#include "conKeyEx.h"

#include <iostream>
#include <gtest/gtest.h>
using namespace nplug;
using namespace std;


// key
const char c_szNoKey[] =		"NoKey";
const char c_szKeySub0[] =		"Sub0";
const char c_szKeySub1[] =		"Sub1";
const char c_szKeySubSub[] =	"SubSub";
// val
const char c_szNoVal[] =		"NoVal";
const char c_szValStr[] =		"ValStr";
const char c_szValInt[] =		"ValInt";
const char c_szValByte[] =		"ValByte";
const char c_szValVect[] =		"ValVect";
const char c_szValEmpty[] =		"ValEmpty";
// data
const char c_szStrData0[] =		"String0 NextWord\tLastWord";
const char c_szStrData1[] =		"String1";

const int			c_iData = -1;
const unsigned char c_byData = 15;

void CheckValuesEnumeration( CClnKeyEx& key );
bool IsNegative(int x, string val0, string val1);

//////////////////////////////////////////////////////////////////

void CheckKeyLifeTime( const CClnKeyEx& key )
{
    CClnKeyEx keyLocal = key;
    keyLocal.IsOpen();
}

void CheckKeyInit( CClnKeyEx& key )
{
    CClnKeyEx keySub0 = key.GetSubKey(c_szKeySub0, true);
    keySub0.GetSubKey(c_szKeySubSub, true);
    CClnKeyEx keySub1 = key.GetSubKey(c_szKeySub1, true);
    ASSERT_TRUE( keySub1.IsOpen() );
    ASSERT_FALSE( key.IsSubKey(c_szNoKey) );
    CClnKeyEx keySubSub = keySub0.GetSubKey(c_szKeySubSub, true);
    ASSERT_TRUE( keySubSub.IsOpen() );
    CClnKeyEx keyNoKey = key.GetSubKey(c_szNoKey, false);
    ASSERT_FALSE( keyNoKey.IsOpen() );
    ASSERT_FALSE( keyNoKey.IsSubKey( c_szNoKey ) );
    keySub0.GetSubKey(c_szKeySubSub, true); // check lifetime
    // deletion
    keyNoKey = key.GetSubKey( c_szNoKey, true ); // check lifetime
    ASSERT_TRUE( key.DeleteSubKey( c_szNoKey ) );
    unsigned uSubKeysCount = keyNoKey.GetSubKeysCount();
    ASSERT_EQ( 0, uSubKeysCount );

}

bool CheckKeyEnumeration( CClnKeyEx& key )
{
    bool bRes = true;
    for( unsigned uIndex=0; uIndex < key.GetSubKeysCount(); uIndex++ )
    {
        CClnKeyEx keyItem = key.GetSubKey( uIndex );
        std::string sName = keyItem.GetKeyName();
        bRes = ( !sName.empty()
                 && bRes );
        std::string sPath = keyItem.GetKeyPath();
        bRes = ( !sPath.empty()
                 && bRes );
    }
    return bRes;
}

void CheckKey( CClnKeyEx& key )
{
    string str;
    unsigned uCount = key.GetSubKeysCount();
    ASSERT_NE( 0, uCount );
    ASSERT_TRUE( key.GetSubKeyName( 0, str ) );
    ASSERT_TRUE( key.IsOpen() );
    ASSERT_TRUE( key.IsSubKey( c_szKeySub0 ) );
    CClnKeyEx keySub = key.GetSubKey( c_szKeySub0, false );
    ASSERT_TRUE( keySub.IsOpen() );
    ASSERT_TRUE( keySub.IsSubKey( c_szKeySubSub ) );
    CClnKeyEx keySubSub = keySub.GetSubKey( c_szKeySubSub, false );
    ASSERT_TRUE( keySubSub.IsOpen() );
    ASSERT_TRUE( key.IsSubKey( c_szKeySub0 ) );
    ASSERT_FALSE( key.IsSubKey( "NoKey" ) );
    CClnKeyEx keyNoKey = key.GetSubKey( "NoKey", false );
    ASSERT_FALSE( keyNoKey.IsOpen() );
    // enumeration
    ASSERT_TRUE( CheckKeyEnumeration( key ) );
    ASSERT_TRUE( CheckKeyEnumeration( keySub ) );
    ASSERT_TRUE( CheckKeyEnumeration( keySubSub ) );
    ASSERT_TRUE( CheckKeyEnumeration( keyNoKey ) );
}
/*
void CheckValuesInit( CClnKeyEx& key )
{
    string sVal = c_szStrData0;
    ASSERT_TRUE( key.SetValue( c_szValStr, sVal ) );
    int iVal = c_iData;
    ASSERT_TRUE( key.SetValue( c_szValInt,  iVal ) );
    unsigned char byVal = c_byData;
    ASSERT_TRUE( key.SetValue( c_szValByte, byVal ) );
    std::vector<std::string> vVal;
    vVal.push_back(c_szStrData0);
    vVal.push_back(c_szStrData1);
    ASSERT_TRUE( key.SetValue( c_szValVect, vVal ) );
    ASSERT_TRUE( key.SetValue( c_szValEmpty, "" ) );

    // deletion
    ASSERT_TRUE( key.SetValue( c_szNoVal, sVal ) );
    ASSERT_TRUE( key.DeleteValue( c_szNoVal ) );
}

void CheckValues( CClnKeyEx& key )
{
    unsigned uValCount = key.GetValuesCount();
    ASSERT_NE( 0, uValCount );
    std::string sValName;
    ASSERT_FALSE( key.GetValueName(1000, sValName ) );
    ASSERT_TRUE( key.GetValueName(0, sValName ) );

    string sVal;
    ASSERT_TRUE( key.IsValue( c_szValStr ) );
    ASSERT_TRUE( key.GetValue( c_szValStr, sVal ) );
    ASSERT_TRUE( sVal == c_szStrData0 );
    ASSERT_FALSE( key.IsValue( c_szNoVal ) );
    ASSERT_FALSE( key.GetValue( c_szNoVal, sVal ) );
    ASSERT_TRUE( sVal == c_szStrData0 );
    ASSERT_TRUE( key.GetValue( c_szValEmpty, sVal ) );
    ASSERT_TRUE( sVal.empty() );
    int iVal = 0;
    ASSERT_TRUE( key.IsValue( c_szValInt ) );
    ASSERT_TRUE( key.GetValue( c_szValInt,  iVal ) );
    ASSERT_EQ( c_iData, iVal );
    unsigned char byVal = 0;
    ASSERT_TRUE( key.IsValue( c_szValByte ) );
    ASSERT_TRUE( key.GetValue( c_szValByte, byVal ) );
    ASSERT_EQ( c_byData, byVal );
    std::vector<std::string> vVal;
    ASSERT_TRUE( key.IsValue( c_szValVect ) );
    ASSERT_TRUE( key.GetValue( c_szValVect, vVal ) );
    EXPECT_PRED3( IsNegative, vVal.size(), vVal[0], vVal[1] );
    ASSERT_TRUE( key.GetValue( c_szValEmpty, vVal ) );
    ASSERT_EQ( 0, vVal.size() );
    // enumeration
    CheckValuesEnumeration(key);
}

void CheckValuesEnumeration( CClnKeyEx& key )
{
    for( unsigned uIndex=0; uIndex < key.GetValuesCount(); uIndex++ )
    {
        std::string sValName;
        ASSERT_TRUE( key.GetValueName( uIndex, sValName ) );
        ASSERT_FALSE( sValName.empty() );
        std::string sData;
        ASSERT_TRUE( key.GetValue( sValName.c_str(), sData ) );
        if( sValName != c_szValEmpty )
            ASSERT_FALSE( sData.empty() );
    }
}

bool IsNegative(int x, string val0, string val1) {
  return (x == 2) && (val0 == c_szStrData0) && (val1 == c_szStrData1);
}
*/
TEST(TestCaseCheckKey, AllTestKey){
    CClnKeyEx key( CConKeyEx::CreateObj() );
    CheckKeyLifeTime( key );
    CheckKeyInit( key );
    CheckKey( key );
    //CheckValuesInit( key );
    //CheckValues( key );
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);


    return RUN_ALL_TESTS();
}






































